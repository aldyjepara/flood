/************************************************************************
*
* Flood Project � (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"
#include "Core/Telemetry.h"
#include "Core/Memory.h"
#include "Core/Log.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

static Allocator* gs_TelemetryAllocator = nullptr;

void TelemetryInitialize()
{
	gs_TelemetryAllocator = AllocatorCreateHeap( AllocatorGetHeap() );
	AllocatorSetGroup(gs_TelemetryAllocator, "Telemetry");
}

void TelemetryCreate()
{
}

void TelemetryDestroy()
{
}

//-----------------------------------//

NAMESPACE_CORE_END