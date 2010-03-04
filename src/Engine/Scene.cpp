/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/scene/Scene.h"
#include "vapor/scene/Transform.h"

using namespace vapor::math;

namespace vapor { namespace scene {

//-----------------------------------//

Scene::Scene() : Group( "Scene" )
{

}

//-----------------------------------//

Scene::~Scene()
{

}

//-----------------------------------//

void Scene::update( double delta )
{
	Group::update( delta );

	//MatrixStack transformStack;
	//transformStack.push( Matrix4x3::Identity );
	//updateTransformAndBV( shared_from_this(), transformStack );
}

//-----------------------------------//

void Scene::updateTransformAndBV( NodePtr /*node*/, MatrixStack& /*transformStack*/ )
{
	// TODO: benckmark and profile this, smells slow
	//PROFILE;

	//// on the way down part
	//TransformPtr transform = node->getTransform();

	//// this is used to know if we need to pop a matrix from the
	//// stack on the way up part, because some nodes will not have
	//// pushed matrices on the stack on the way down part.
	//bool needsPop = false;
 
	//// concatenate this matrix on the stack with the top most
	//// there will be at least one matrix in stack, top most is identity
	//if ( transform )
	//{
	//	transformStack.push( transform->getLocalTransform() * transformStack.top() );

	//	//if( !transform->isPhysicsDriven )
	//		transform->setAbsoluteTransform( transformStack.top() );
	//	
	//	needsPop = true;
	//}

	//GroupPtr group( std::dynamic_pointer_cast< Group >( node ) );

	//if( group )
	//{
	//	foreach( NodePtr gnode, group->getChildren() )
	//	{
	//		updateTransformAndBV( gnode, transformStack );
	//	}
	//}

	//// on the way up part
	//node->update( delta );
	//    
	//// remove the matrix from the stack
	//if ( needsPop )
	//{
	//	transformStack.pop();
	//}
}

//-----------------------------------//

Node* Scene::getEntityPtr( const std::string& name ) const
{
	return getEntity(name).get();
}

//-----------------------------------//

NodePtr Scene::getEntity( const std::string& name ) const
{
	//if( getName() == name ) return shared_from_this();

	foreach( const NodePtr& node, getChildren() )
	{
		if( node->getName() == name )
			return node;
	}

	return NodePtr();
}

//-----------------------------------//

} } // end namespaces