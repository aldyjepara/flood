/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/terrain/Terrain.h"
#include "vapor/math/Math.h"

namespace vapor { namespace scene {

using namespace vapor::resources;
using namespace vapor::render;

//-----------------------------------//

// Range of valid dimensions for heightmaps.
static const short validDimensions[] =
{
	2,    3,    5,    9, 
	17,   33,   65,   129,
	257,  513,  1025, 2049
};

//-----------------------------------//

const std::string& Terrain::type = "Terrain";

//-----------------------------------//

Terrain::Terrain( const TerrainSettings& settings )
	: settings( settings )
{
	cellMaterial.reset( new Material( "PageMaterial" ) );
	cellMaterial->setTexture( 0, "PineTrunk.png" );
	cellMaterial->setProgram( "tex_toon" );
}

//-----------------------------------//

Terrain::~Terrain()
{

}

//-----------------------------------//

CellPtr Terrain::createCell( const ImagePtr& heightmap, ushort x, ushort y )
{
	if( !validateHeightmap( heightmap ) )
	{
		return nullptr;
	}

	std::vector<float> heights;
	convertHeightmap( heightmap, heights );

	CellPtr page( new Cell( settings, heights, x, y ) );

	page->setMaterial( cellMaterial );
	addRenderable( page );

	// Forces AABB generation next update.
	isDirty = true;
	
	return page;
}

//-----------------------------------//

void Terrain::convertHeightmap( const resources::ImagePtr& heightmap, std::vector<float>& heights )
{
	// TODO: Can't handle any other pixel format right now...
	assert( heightmap->getPixelFormat() == PixelFormat::R8G8B8A8 );

	const std::vector<byte> data = heightmap->getBuffer();

	for( uint i = 0; i < data.size(); i += 4 )
	{
		byte R = data[i];
		byte G = data[i+1];
		byte B = data[i+2];

		float S = 255*3;
		float height = (R/S)+(G/S)+(B/S);
		
		heights.push_back( height );
	}
}

//-----------------------------------//

bool Terrain::validateHeightmap( const ImagePtr& heightmap )
{
	const ushort width = heightmap->getWidth();
	const ushort height = heightmap->getHeight();

	// Check the heightmap for the right dimensions.
	// First condition: width and height should be the same.
	if( width != height )
	{
		warn( "terrain", "Invalid heightmap (width != height)" );
		return false;
	}

	// The dimension should obey the formula 2^n+1.
	// Check an embedded LUT with the valid dimensions.
	
	bool valid = false;
	for( int i = 0; i < VAPOR_ARRAY_SIZE(validDimensions); i++ )
	{
		if( width == validDimensions[i] ) 
		{
			valid = true;
			break;
		}
	}

	if( !valid )
	{
		warn( "terrain", "Invalid heightmap dimensions (should be 2^n+1)" );
		return false;
	}

	return true;
}

//-----------------------------------//

render::MaterialPtr Terrain::getMaterial() const
{
	return cellMaterial;
}

//-----------------------------------//

void Terrain::setMaterial( const render::MaterialPtr& material )
{
	this->cellMaterial = material;
}

//-----------------------------------//

resources::ImagePtr Terrain::getHeightmap() const
{
	return heightmap;
}

//-----------------------------------//

void Terrain::setHeightmap( const resources::ImagePtr& heightmap )
{
	this->heightmap = heightmap;
}

//-----------------------------------//

void Terrain::update( double delta )
{
	Geometry::update( delta );
}

//-----------------------------------//

const std::string& Terrain::getType() const
{
	return type;
}

//-----------------------------------//

} } // end namespaces