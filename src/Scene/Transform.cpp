/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "Scene/Transform.h"
#include "Scene/Geometry.h"
#include "Scene/Entity.h"
#include "Render/DebugGeometry.h"
#include "Math/Helpers.h"

namespace vapor {

//-----------------------------------//

BEGIN_CLASS_PARENT(Transform, Component)
	FIELD_PRIMITIVE(Vector3, position)
	FIELD_PRIMITIVE(Quaternion, rotation)
	FIELD_PRIMITIVE(Vector3, scale)
END_CLASS()

//-----------------------------------//

Transform::Transform()
	: wasChanged(false)
	, needsBoundsUpdate(true)
	, externalTransform(false)
	, scale(1)
{ }

//-----------------------------------//

//void Transform::translate( const Vector3& offset )
//{
//	translate( offset.x, offset.y, offset.z );
//}
//
////-----------------------------------//
//
//void Transform::translate( float x, float y, float z )
//{
//	setChanged();
//
//	position.x += x;
//	position.y += y;
//	position.z += z;
//}
//
////-----------------------------------//
//
//void Transform::scale( const Vector3& value )
//{
//	scale( value.x, value.y, value.z );
//}
//
////-----------------------------------//
//
//void Transform::scale( float x, float y, float z )
//{
//	setChanged();
//
//	scaling.x *= x;
//	scaling.y *= y;
//	scaling.z *= z;
//}
//
////-----------------------------------//
//
//void Transform::rotate( const Vector3& rot )
//{
//	rotate( rot.x, rot.y, rot.z );
//}
//
////-----------------------------------//
//
//void Transform::rotate( float xang, float yang, float zang )
//{
//	setChanged();
//
//	rotation.x += xang;
//	rotation.y += yang;
//	rotation.z += zang;
//}

//-----------------------------------//

void Transform::setPosition( const Vector3& position )
{
	setChanged();
	this->position = position;
}

//-----------------------------------//

void Transform::setRotation( const Quaternion& rotation )
{
	setChanged();
	this->rotation = rotation;
}

//-----------------------------------//

void Transform::setScale( const Vector3& scale )
{
	setChanged();
	this->scale = scale;
}

//-----------------------------------//

void Transform::reset()
{
	setChanged();

	position.zero();
	rotation.identity();
	scale = 1;
}

//-----------------------------------//

Vector3 Transform::getWorldPosition() const
{
	return getAbsoluteTransform() * getPosition();
}

//-----------------------------------//

Matrix4x3 Transform::lookAt( const Vector3& lookAtVector, const Vector3& upVector )
{
	const Vector3& eye = position;

	Vector3 zaxis = (eye - lookAtVector).normalize();
	Vector3 xaxis = upVector.cross(zaxis).normalize();
	Vector3 yaxis = zaxis.cross(xaxis);

	Matrix4x3 mat;
	mat.m11 = xaxis.x;
	mat.m12 = yaxis.x;
	mat.m13 = zaxis.x;

	mat.m21 = xaxis.y;
	mat.m22 = yaxis.y;
	mat.m23 = zaxis.y;

	mat.m31 = xaxis.z;
	mat.m32 = yaxis.z;
	mat.m33 = zaxis.z;

	mat.tx = -xaxis.dot(eye);
	mat.ty = -yaxis.dot(eye);
	mat.tz = -zaxis.dot(eye);

	return mat;
}

//-----------------------------------//

void Transform::setChanged(bool state)
{
	wasChanged = state;
}

//-----------------------------------//

const Matrix4x3& Transform::getAbsoluteTransform() const
{
	return transform;
}

//-----------------------------------//

void Transform::setAbsoluteTransform( const Matrix4x3& newTransform )
{
	setChanged();

	transform = newTransform;
	externalTransform = true;
}

//-----------------------------------//

Matrix4x3 Transform::getLocalTransform() const
{
	Matrix4x3 matScale = Matrix4x3::createScale(scale);
	Matrix4x3 matRotation = Matrix4x3::createFromQuaternion(rotation);
	Matrix4x3 matTranslation = Matrix4x3::createTranslation(position);
	
	return matScale*matRotation*matTranslation;
}

//-----------------------------------//

void Transform::markBoundingVolumeDirty()
{
	needsBoundsUpdate = true;
}

//-----------------------------------//

bool Transform::requiresBoundingVolumeUpdate() const
{
	return needsBoundsUpdate;
}

//-----------------------------------//

void Transform::updateBoundingVolume()
{
	bounds.reset();

	const std::vector<GeometryPtr>& geoms = entity->getGeometry();
	
	for( uint i = 0; i < geoms.size(); i++ )
	{
		const GeometryPtr& geometry = geoms[i];
		bounds.add( geometry->getBoundingVolume() );
	}
	
	if( bounds.isInfinite() )
		bounds.setZero();

	// Update debug renderable.
	debugRenderable = buildBoundingRenderable( bounds );
	needsBoundsUpdate = false;
}

//-----------------------------------//

void Transform::update( float delta )
{
	if( !externalTransform )
		transform = getLocalTransform();

	if( requiresBoundingVolumeUpdate() )
		updateBoundingVolume();

	if( wasChanged )
	{
		onTransform();
		setChanged(false);
	}

	//externalTransform = false;
}

//-----------------------------------//

BoundingBox Transform::getWorldBoundingVolume() const
{
	const Matrix4x3& transform = getAbsoluteTransform();
	return bounds.transform(transform);
}

//-----------------------------------//

} // end namespace