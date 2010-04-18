/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Example.h"

//-----------------------------------//

Example::Example(const char** argv)
	: Framework("Example", argv), fpsUpdateTime( 0.0f )
{

}

//-----------------------------------//

void Example::onInit()
{
	//physicsManager->createWorld();
	getVFS()->mountDefaultLocations();
}

//-----------------------------------//

void Example::onSetupResources() 
{
	ResourceManagerPtr rm = getResourceManager();
	
	ImagePtr img = rm->loadResource< Image >( "triton.png" );
	snd = rm->loadResource< resources::Sound >( "stereo.ogg" );
}

//-----------------------------------//

std::string getFPS( float lastFrameTime )
{
	std::string str = "FPS: ";
	if( lastFrameTime == 0 ) return str;
	std::string fps( num_to_str(int( 1.0f / lastFrameTime ) ) );
	return str + fps;
}

//-----------------------------------//

void Example::onSetupScene() 
{
	ScenePtr scene = getSceneManager();
	ResourceManagerPtr const rm = getResourceManager();
	render::DevicePtr const rd = getRenderDevice();
	ProgramManagerPtr const pm = rd->getProgramManager();

	// TODO: make shaders automatically loaded?
	ProgramPtr tex( new GLSL_Program( 
			rm->loadResource< GLSL_Shader >( "tex.vs" ),
			rm->loadResource< GLSL_Shader >( "tex.fs" ) ) );

	ProgramPtr diffuse( new GLSL_Program( 
			rm->loadResource< GLSL_Shader >( "diffuse.vs" ),
			rm->loadResource< GLSL_Shader >( "diffuse.fs" ) ) );

	ProgramPtr toon( new GLSL_Program( 
			rm->loadResource< GLSL_Shader >( "toon.vs" ),
			rm->loadResource< GLSL_Shader >( "toon.fs" ) ) );

	ProgramPtr tex_toon( new GLSL_Program( 
			rm->loadResource< GLSL_Shader >( "tex_toon.vs" ),
			rm->loadResource< GLSL_Shader >( "tex_toon.fs" ) ) );

	pm->registerProgram( "diffuse", diffuse );
	pm->registerProgram( "tex", tex );
	pm->registerProgram( "toon", toon );
	pm->registerProgram( "tex_toon", tex_toon );

	// Create a new Camera
	NodePtr camera( new Node( "MainCamera" ) );
	cam.reset( new FirstPersonCamera( getInputManager(), getRenderDevice() ) );
	camera->addComponent( TransformPtr( new Transform( 0.0f, 20.0f, -65.0f ) ) );
	camera->addComponent( cam );
	scene->add( camera );

	fbo = new FBO( Settings() );
	TexturePtr fbo_tex = fbo->createRenderTexture();
	
	viewport2 = fbo->addViewport(cam);
	viewport2->setClearColor( Colors::Green );

	MaterialPtr fbo_mat( new Material( "FBO1", tex ) );
	fbo_mat->setTexture( 0, fbo_tex );

	RenderablePtr quad( new Quad(100.0f, 100.0f) );
	quad->setMaterial( fbo_mat );

	NodePtr fbo_node( new Node( "FBOquad" ) );
	fbo_node->addTransform();
	fbo_node->addComponent( GeometryPtr( new Geometry(quad) ) );
	fbo_node->getTransform()->rotate( 90.0f, 0.0f, 0.0f );
	scene->add( fbo_node );

	//MeshPtr mesh = rm->loadResource<Mesh>( "ct.ms3d" );
	//foreach( const RenderablePtr& rend, mesh->getGeometry()->getRenderables() )
	//	rend->getMaterial()->setProgram( tex );

	//NodePtr ct( new Node( "ct" ) );
	//ct->addTransform();
	//ct->addComponent( mesh->getGeometry() );
	//scene->add(ct);
	
	// Materials too?
	MaterialPtr mat2( new Material( "FontMaterial", tex ) );
	FontPtr font = rm->loadResource< Font >( "Verdana.font" );
	label.reset( new Label( getFPS( lastFrameTime ), font, mat2 ) );
	NodePtr fps( new Node( "FPSNode" ) );
	fps->addTransform();
	fps->addComponent( label );
	fps->getTransform()->translate( -300.0f, 220.0f, 0.0f );
	scene->add( fps );

	MaterialPtr mat( new Material( "GridMaterial", diffuse ) );
	NodePtr grid( new Node( "Grid" ) );
	grid->addTransform();
	grid->addComponent( ComponentPtr( new Grid( mat ) ) );
	scene->add( grid );

	foreach( const RenderablePtr& rend, 
		grid->getComponent<Geometry>("Grid")->getRenderables() )
	{
		rend->getMaterial()->setProgram( diffuse );
	}

	NodePtr lnode( new Node("Light") );
	lnode->addTransform();
	LightPtr light( new Light( LightType::Point ) );
	light->diffuseColor = Colors::Red;
	light->ambientColor = Colors::Yellow;
	lnode->addComponent( light );
	scene->add( lnode );

	MaterialPtr cellMaterial( new Material("CellMaterial") );
	cellMaterial->setTexture( 0, "PineTrunk.png" );
	cellMaterial->setProgram( "tex_toon" );

	TerrainSettings settings;
	settings.CellSize = 512;
	settings.NumberTiles = 32;
	settings.MaxHeight = 100;
	settings.Material = cellMaterial;

	TerrainPtr terrain( new Terrain( settings ) );

	NodePtr terreno( new Node( "Terreno" ) );
	terreno->addTransform();
	terreno->addComponent( terrain );
	scene->add( terreno );

	const ImagePtr& heightmap = rm->loadResource<Image>( "height2.png" );
	terrain->addCell( heightmap, 0, 0 );

	window = rd->getRenderWindow();
	viewport = window->addViewport(cam);
	viewport->setClearColor( Color(0.0f, 0.10f, 0.25f) );
}

//-----------------------------------//
 
void Example::onUpdate( double delta ) 
{
	if( fpsUpdateTime <= 1.0f )
	{
		fpsUpdateTime += delta;
	}
	else
	{
		label->setText( getFPS(lastFrameTime) );
		fpsUpdateTime = 0.0f;
	}
}

//-----------------------------------//

void Example::onRender()
{
	// Render into the FBO first
	fbo->bind();
	viewport2->update();
	fbo->unbind();

	// Render the scene
	viewport->getRenderTarget()->makeCurrent();
	viewport->update();
	window->update();
}

//-----------------------------------//

void Example::onKeyPressed( const KeyEvent& keyEvent )
{
	if( keyEvent.keyCode == Keys::Space )
		debug( "time: %f", frameTimer.getElapsedTime() );

	if( keyEvent.keyCode == Keys::Pause )
		Log::showDebug = !Log::showDebug;

	if( keyEvent.keyCode == Keys::F )
		debug( "fps: %d", int( 1.0f / lastFrameTime ) );

	if( keyEvent.keyCode == Keys::G )
	{
		debug( "min/avg/max: %f / %f / %f", 
					minFrameTime, avgFrameTime, maxFrameTime );
	}

	if( keyEvent.keyCode == Keys::J )
	{
		Json::Value sc;
		getSceneManager()->serialize( sc );
		File file( "Example.scene", vfs::AccessMode::Write );
		file.write( sc.toStyledString() );
	}
}

//-----------------------------------//

void Example::onButtonPressed( const input::MouseButtonEvent& event )
{
	debug( "%hd,%hd", event.x, event.y );
}

//-----------------------------------//