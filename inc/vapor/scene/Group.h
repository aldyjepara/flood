/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/scene/Entity.h"

namespace vapor {

//-----------------------------------//

/**
 * Groups are nodes that have the special property of being able to be
 * attached to each other. If you attach a node to another node, they will
 * be in a hierarchical relationship and the parent node will influence the 
 * child node transformations.
 */

class VAPOR_API Group : public Entity
{
	DECLARE_CLASS_()

public:

	Group();
	Group( const std::string& name );

	// Adds child to the end of the child list.
	void add( const EntityPtr& node );

	// Removes node from the group.
	bool remove( const EntityPtr& node );

	// Finds a node in the group via its name.
	EntityPtr findEntity( const std::string& name ) const;

	// Gets the nodes of the group.
	GETTER(Entities, const std::vector<EntityPtr>&, nodes)

	// Updates this group of nodes.
	virtual void update( double delta );

	// Event gets called when a node is added.
	Event1<const EntityPtr&> onEntityAdded;

	// Event gets called when a node is removed.
	Event1<const EntityPtr&> onEntityRemoved;

	// Event gets called when its contents change.
	Event0<> onChanged;

protected:

	// Holds the nodes of the group.
	std::vector<EntityPtr> nodes;
};

TYPEDEF_SHARED_POINTER_FROM_TYPE( Group );

//-----------------------------------//

} // end namespace