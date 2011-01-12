/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "math/Vector3.h"
#include "math/Color.h"
#include "render/Buffer.h"

namespace vapor {

//-----------------------------------//

/**
 * Attribute of a vertex element.
 */

struct VertexAttribute
{
	enum Enum
	{
		Position = 0,
		Normal = 2,
		Color = 3,
		SecondaryColor = 4,
		FogCoord = 5,
		TexCoord0 = 8,
		TexCoord1,
		TexCoord2,
		TexCoord3,
		TexCoord4,
		TexCoord5,
		TexCoord6,
		TexCoord7,
	};
};

//-----------------------------------//

struct Attribute
{
	Attribute() {}

	Attribute(const Attribute& rhs)
	  : components(rhs.components)
	  , size(rhs.size)
	  , data(rhs.data)
	  , type(rhs.type)
	{}

	int components;
	int size;
	int type;
	std::vector<byte> data;
};

//-----------------------------------//

typedef std::map<VertexAttribute::Enum, Attribute> AttributeMap;
typedef std::pair<const VertexAttribute::Enum, Attribute> AttributeMapPair;

/**
 * Represents a vertex buffer. One limitation here is that all data is 
 * tied to the vertex so if you want a normal per primitive and not per 
 * vertex you will have to duplicate that normal for each vertex for now.
 */

class VAPOR_API VertexBuffer : public Buffer
{
public:

	VertexBuffer(
		BufferUsage::Enum = BufferUsage::Static,
		BufferAccess::Enum = BufferAccess::Write);
	
	virtual ~VertexBuffer();

	// Binds the vertex buffer.
	bool bind();
	
	// Unbinds the vertex buffer.
	bool unbind();

	// Binds the attributes pointers.
	void bindPointers();

	// Unbinds the attribute pointers.
	void unbindPointers();

	// Binds the attribute pointers.
	void bindGenericPointers();

	// Unbinds the attribute pointers.
	void unbindGenericPointers();
	
	// Clears this vertex buffer. All vertex data will be gone.
	void clear();

	// Returns if the this buffer is valid.
	bool isValid() const;

	// Returns true if the vertex buffer is built, false otherwhise.
	bool isBuilt() const;

	// Updates the internal buffer with current attributes.
	bool build();

	// Forces a rebuild of the vertex buffer the next update.
	void forceRebuild();

	// Returns an attribute data in the vertex buffer.
	std::vector<Vector3>& getAttribute( VertexAttribute::Enum ) const;

	// Returns the vertices attribute in the vertex buffer.
	std::vector<Vector3>& getVertices() const;

	// Returns the total size in bytes of the buffer.
	uint getSize() const;

	// Returns the number of vertex attributes.
	uint getNumAttributes() const;

	// Returns the number of vertices in each attribute.
	uint getNumVertices() const;

	// Sets the attribute data.
	bool set( VertexAttribute::Enum attr, const std::vector<Vector3>& data );

	// Sets the attribute data.
	bool set( VertexAttribute::Enum attr, const std::vector<Color>& data );

	// Sets the attribute data.
	bool set( VertexAttribute::Enum attr, const std::vector<float>& data );

private:

	// Checks that each entry in the map has the same size.
	bool checkSize() const;

	// Tracks if the buffer has been built.
	bool built;
	
	// Holds all the vertex attributes.
	mutable AttributeMap attributes;

	// Holds the number of vertices inside.
	mutable uint numVertices;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( VertexBuffer );

//-----------------------------------//

} // end namespace