/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/render/Program.h"
#include "vapor/render/Texture.h"
#include "vapor/render/Shader.h"
#include "vapor/render/GL.h"

namespace vapor {

//-----------------------------------//

namespace BlendingSource
{
	enum Enum
	{
		Zero = GL_ZERO,
		One = GL_ONE,
		DestinationColor = GL_DST_COLOR,
		InverseDestinationColor = GL_ONE_MINUS_DST_COLOR,
		SourceAlpha = GL_SRC_ALPHA,
		InverseSourceAlpha = GL_ONE_MINUS_SRC_ALPHA,
		DestinationAlpha = GL_DST_ALPHA,
		InverseDestinationAlpha = GL_ONE_MINUS_DST_ALPHA,
		SourceAlphaSaturate = GL_SRC_ALPHA_SATURATE
	};
}

//-----------------------------------//

namespace BlendingDestination
{
	enum Enum
	{
		Zero = GL_ZERO,
		One = GL_ONE,
		SourceColor = GL_SRC_COLOR,
		InverseSourceColor = GL_ONE_MINUS_SRC_COLOR,
		SourceAlpha = GL_SRC_ALPHA,
		InverseSourceAlpha = GL_ONE_MINUS_SRC_ALPHA,
		DestinationAlpha = GL_DST_ALPHA,
		InverseDestinationAlpha = GL_ONE_MINUS_DST_ALPHA
	};
}

//-----------------------------------//

typedef std::map< uint, TexturePtr > TextureMap;
typedef std::pair< const uint, TexturePtr > TextureMapPair;

/**
 * Rendering material.
 */

class VAPOR_API Material : public ReferenceCounted
{
	friend class RenderDevice;

public:

	//Material( const std::string& name );
	Material( const std::string& name, const std::string& program = "diffuse" );

	// Gets the textual name of the material.
	GETTER(Name, const std::string&, name);

	// Adds a texture to the material.
	void setTexture( uint unit, const std::string& tex );

	// Adds a texture to the material.
	void setTexture( uint unit, const TexturePtr& tex );

	// Adds an image as texture to the material.
	void setTexture( uint unit, const ImagePtr& tex );

	// Gets/sets the associated program.
	ACESSOR(Program, const std::string&, program)
	
	// Gets the associated program.	
	ProgramPtr getProgram();

	// Gets/sets the depth writing of the material.
	ACESSOR(DepthWrite, bool, depthWrite)

	// Gets/sets the depth testing of the material.
	ACESSOR(DepthTest, bool, depthTest)

	// Gets/sets the line width of the material.
	ACESSOR(LineWidth, float, lineWidth)

	// Gets/sets the line smoothing of the material.
	ACESSOR(LineSmoothing, bool, lineSmooth)

	// Gets/sets the backface culling of the material.
	ACESSOR(BackfaceCulling, bool, cullBackfaces)

	// Gets the blending options for this material.
	GETTER(BlendingSource, BlendingSource::Enum, source)
	
	// Gets the blending options for this material.
	GETTER(BlendingDestination, BlendingDestination::Enum, destination)

	// Is blending enabled?
	// Blending is automatically enabled if you set a custom option.
	bool isBlendingEnabled() const;

	// Sets the blending options for this material.
	void setBlending( BlendingSource::Enum, BlendingDestination::Enum );

	// Gets the textures in the material.
	GETTER(Textures, const TextureMap&, textures)

	// Binds the material object.
	void bind();

	// Unbinds the material object.
	void unbind();

	// Serialization.
	//void serialize( Json::Value value );

protected:

	// Common material initialization.
	void init();

	// Name of the material.
	std::string name;

	// Program of the material.
	std::string program;

	// Textures.
	TextureMap textures;

	// Backface culling.
	bool cullBackfaces;
	
	// Depth settings.
	bool depthTest;
	bool depthWrite;

	// Line settings.
	bool lineSmooth;
	float lineWidth;

	// Blending settings.
	BlendingSource::Enum source;
	BlendingDestination::Enum destination;
	bool _isBlendingEnabled;

	// Default line width.
	static float DefaultLineWidth;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Material );

//-----------------------------------//

} // end namespace