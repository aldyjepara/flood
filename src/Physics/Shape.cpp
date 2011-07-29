/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"

#ifdef VAPOR_PHYSICS_BULLET

#include "Physics/Shape.h"
#include "Physics/Body.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_ABSTRACT_CHILD_CLASS(Shape, Component)
REFLECT_CLASS_END()

//-----------------------------------//

Shape::~Shape()
{ }

//-----------------------------------//

void Shape::removeBody()
{
	const BodyPtr& body = weakBody.lock();

	if( body )
		body->removeWorld();
}

//-----------------------------------//

NAMESPACE_ENGINE_END

#endif