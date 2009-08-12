/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/NativeFile.h"

#ifdef VAPOR_PLATFORM_WINDOWS
	#include <io.h>
	#define F_OK 0
#else
	#include <unistd.h>
#endif

#ifdef VAPOR_PLATFORM_WINDOWS
	// disable Visual C++ fopen deprecation warning
	#pragma warning(disable : 4996)
#endif

namespace vapor {

//-----------------------------------//

NativeFile::NativeFile(std::string path)
	: path(path), fp(nullptr)
{
	fp = fopen(path.c_str(), "rb");
}

//-----------------------------------//

NativeFile::NativeFile(const char* _path)
	: path(_path), fp(nullptr)
{
	fp = fopen(_path, "rb");
}

//-----------------------------------//

NativeFile::~NativeFile() 
{
	close();
}

//-----------------------------------//

void NativeFile::close()
{
	// close the file
	if(fp != nullptr) {
		fclose(fp);
	}
}

//-----------------------------------//

long NativeFile::getSize()
{
	// hold the current file position
	long curr = ftell(fp);
	
	// seek to the end of the file and get position
	fseek(fp, 0, SEEK_END);
	long fileSize = ftell(fp);
	
	// seek again to the previously current position
	fseek(fp, curr, SEEK_SET);

	return fileSize;
}

//-----------------------------------//

long NativeFile::read(void* buffer, long sz)
{	
	return fread(buffer, 1, sz, fp);
}

//-----------------------------------//

bool NativeFile::exists()
{
	return _access(path.c_str(), F_OK) == 0;
}

//-----------------------------------//

bool NativeFile::exists(std::string path)
{
	return _access(path.c_str(), F_OK) == 0;
}

//-----------------------------------//

} // end namespace