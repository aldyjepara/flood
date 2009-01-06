/************************************************************************
*
* vaporEngine by triton (2008)
*
*	<http://www.portugal-a-programar.org>
*
* DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
* Version 2, December 2004
* 
* DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
* TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
* 
* 0. You just DO WHAT THE FUCK YOU WANT TO.
*
************************************************************************/

#include "vapor/resources/FontManager.h"

namespace vapor {
	namespace resources {

FontManager::FontManager()
{
	if(initFT()) {
		info("resources::ttf", "Initialized FreeType %d.%d.%d", 
			FREETYPE_MAJOR, FREETYPE_MINOR, FREETYPE_PATCH);
	} else {
		error("resources::ttf", "Failed to initialize FreeType");
		exit(EXIT_FAILURE);
	}
}

FontManager::~FontManager()
{
	if(FT_Done_FreeType(ft) != 0) {
		warn("resources::ttf", "Failed to destroy FreeType");
	}
}

bool FontManager::initFT() 
{
	// initialize FreeType library
	if(FT_Init_FreeType(&ft) == 0)
		return true;
	else
		return false;
}

} } // end namespaces
