/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/Serialization.h"
#include "vapor/vfs/File.h"
#include <clocale>

namespace vapor {

//-----------------------------------//

Json::Value toJson( const math::Vector3& vec )
{
	Json::Value v;
	v[0u] = vec.x;
	v[1u] = vec.y;
	v[2u] = vec.z;
	return v;
}

//-----------------------------------//

void serializeToFile( Json::Value& root, const std::string& name )
{
	// Always use the platform independent "C" locale when writing JSON,
	// jsoncpp assumes this and will write wrong data with another locale.

	char *current_locale = setlocale(LC_NUMERIC, "C");

	vfs::File file( name, vfs::AccessMode::Write );
	file.write( root.toStyledString() );

	setlocale(LC_NUMERIC, current_locale);
}

//-----------------------------------//

} // end namespace 