/************************************************************************
*
* Flood Project � (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Engine/API.h"
#include "Core/Object.h"
#include "Core/References.h"

FWD_DECL_INTRUSIVE(RenderBatch)
FWD_DECL_INTRUSIVE(Camera)
FWD_DECL_INTRUSIVE(Entity)

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

enum class DebugDrawFlags;
class DebugDrawer;

/**
 * Represents a component, a specific piece of functionality that can be
 * added to each entity in the scene. A component will tipically hold a
 * piece of data and update it's state each frame if it needs to do it.
 * A component will also be able to register methods for scripting.
 */

API_SCENE REFLECT_DECLARE_CLASS(Component)

class API_SCENE Component : public Object
{
	DECLARE_UNCOPYABLE(Component)
	REFLECT_DECLARE_OBJECT(Component)

public:

	Component();
	virtual ~Component();
	
	// Gets the associated node of this component.
	Entity* getEntity() const;

	// Sets the associated node of this component.
	void setEntity( Entity* entity );

	// Called once per frame to update the component.
	virtual void update( float delta ) = 0;

	// Called just before the camera culls this component.
	//virtual void onPreCull( const CameraPtr& camera );

	// Called just before the camera renders this component.
	virtual void onPreRender( const Camera& camera );

	// Gets if the debug.renderable is visible.
	virtual bool isDebugRenderableVisible() const;
	
	// Sets the debug.renderable as visible.
	virtual void setDebugRenderableVisible( bool visible );

	// Called when it is time to draw debug data.
	virtual void onDebugDraw( DebugDrawer&, DebugDrawFlags );

	// Gets/sets if the debug.renderable inherits the transform.
	ACCESSOR(DebugInheritsTransform, bool, debugInheritsTransform)

protected:

	// Entity that owns this component.
	Entity* entity;

	// Is the debug representation visible.
	bool debugVisible;

	// Does the debug inherit the transform.
	mutable bool debugInheritsTransform;

	// Debug.renderable of component.
	mutable RenderBatchPtr debugRenderable;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Component )

//-----------------------------------//

NAMESPACE_ENGINE_END