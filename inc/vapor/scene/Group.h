/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"
#include "vapor/scene/Node.h"

namespace vapor {
	namespace scene {

//-----------------------------------//

class VAPOR_API Group : public Node
{
public:

	Group();
	virtual ~Group();

	// adds child to the end of the child list.  Returns the index of
	// the new child node that has been added.
	virtual int add( NodePtr child );

	// inserts child after child at i'th index.  Returns the index of
	// the new child inserted, if there is no existing
	// child at 'i' then -1 is returned.
	virtual int insert( int i, NodePtr child );

	// removes child at index i, if i is out of bounds returns false,
	// if i is in bounds and child was removed, returns true
	virtual bool remove( int i );

	// returns null pointer if cannot find index i
	NodePtr get( uint i ) const;

	// Gets the children of this group.
	const std::vector< NodePtr >& getChildren() { return children; }

	// returns index for child with the same pointer, if child cannot
	// be found, -1 is returned
	int index( NodePtr child ) const;

	// returns number of children in this group
	int count( ) const;

	// updates this group of nodes
	virtual void update();

	// serialization
	virtual const std::string save( int indent = 0 );

	// gets the name of this node
	virtual const std::string name();

protected:

	std::vector< NodePtr > children;
};

//-----------------------------------//

typedef tr1::shared_ptr< Group > GroupPtr;

//-----------------------------------//

} } // end namespaces