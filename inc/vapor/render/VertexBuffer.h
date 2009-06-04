/************************************************************************
*
* vaporEngine by triton � (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"
#include "vapor/render/Buffer.h"
#include "vapor/render/VertexElement.h"

namespace vapor {
	namespace render {

/**
 * Represents a vertex buffer.
 */

class VertexBuffer : public Buffer
{
public:

	VertexBuffer(uint numElems, VertexDeclaration decl, BufferUsage::Enum bu, BufferType::Enum bt)
		: bufferUsage(bu), bufferType(bt), numElements(numElems), vertexDeclaration(decl)
	{}

	virtual ~VertexBuffer() { }

	// Map a buffer
	virtual void* map() = 0;
	
	// Unmap a buffer
	virtual void unmap() = 0;

protected:

	// number of elements in this buffer
	uint numElements;

	// declaration of vertex elements
	VertexDeclaration vertexDeclaration;
	
	// usage of this buffer
	BufferUsage::Enum bufferUsage;
	
	// type of this buffer
	BufferType::Enum bufferType;
};

} } // end namespaces