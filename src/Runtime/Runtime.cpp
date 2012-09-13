/************************************************************************
*
* Flood Project � (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#define INSTANTIATE_TEMPLATES

#include "Runtime/API.h"
#include "Runtime/Runtime.h"

NAMESPACE_RUNTIME_BEGIN

//-----------------------------------//

Runtime::Runtime()
{

}

//-----------------------------------//

void Runtime::onInit()
{
	// Initialize the Mono scripting runtime.
	monoRuntime.initialize();
}

//-----------------------------------//

void Runtime::onCleanup()
{
	// Cleanup the Mono scripting runtime.
	monoRuntime.cleanup();
}

//-----------------------------------//

NAMESPACE_RUNTIME_END