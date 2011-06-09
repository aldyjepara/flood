/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Scene/Entity.h"

namespace vapor {

//-----------------------------------//

/**
 * Groups are nodes that have the special property of being able to be
 * attached to each other. If you attach a node to another node, they will
 * be in a hierarchical relationship and the parent node will influence the 
 * child node transformations.
 */

REFLECT_DECLARE_CLASS(Group)

class API_ENGINE Group : public Entity
{
	REFLECT_DECLARE_OBJECT(Group)

public:

	Group();
	Group( const String& name );

	// Adds an entity to the group.
	void add( const EntityPtr& entity );

	// Removes an entity from the group.
	bool remove( const EntityPtr& entity );

	// Finds an entity in the group via its name.
	EntityPtr findEntity( const String& name ) const;

	// Gets the entities of the group.
	GETTER(Entities, const std::vector<EntityPtr>&, entities)

	// Updates the group.
	virtual void update( float delta );

	// Fix-up serialization.
	virtual void fixUp() OVERRIDE;

	// Event gets called when an entity is added.
	Event1<const EntityPtr&> onEntityAdded;

	// Event gets called when an entity is removed.
	Event1<const EntityPtr&> onEntityRemoved;

	// Event gets called when its contents change.
	Event0<> onEntityChanged;

protected:

	// Holds the entities of the group.
	std::vector<EntityPtr> entities;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Group );

//-----------------------------------//

} // end namespace