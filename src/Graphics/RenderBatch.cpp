/************************************************************************
*
* Flood Project � (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Graphics/API.h"
#include "Graphics/RenderBatch.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

RenderBatch* RenderBatchCreate( Allocator* alloc )
{
	RenderBatch* rend = Allocate(alloc, Renderable);
	return rend;
}

//-----------------------------------//

RenderBatchRange::RenderBatchRange()
	: start(0)
	, end(0)
{
}

//-----------------------------------//

RenderBatch::RenderBatch()
	: type( PrimitiveType::Triangles )
	, mode( PrimitiveRasterMode::Solid )
	, stage( RenderLayer::Normal )
	, priority(0)
{
	ub = AllocateGraphics(UniformBuffer);
}

//-----------------------------------//

RenderBatch::RenderBatch(const RenderBatch& b)
	: range(b.range)
	, stage(b.stage)
	, priority(b.priority)
	, type(b.type)
	, mode(b.mode)
	, gb(b.gb)
	, ub(b.ub)
	, material(b.material)
{
}

//-----------------------------------//

RenderBatch::~RenderBatch()
{

}

//-----------------------------------//

NAMESPACE_GRAPHICS_END