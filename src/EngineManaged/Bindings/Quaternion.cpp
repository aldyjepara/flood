/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Quaternion.h"
#include "EulerAngles.h"
#include "Vector.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::Quaternion::Quaternion(::Quaternion* native)
{
    X = native->x;
    Y = native->y;
    Z = native->z;
    W = native->w;
}

Flood::Quaternion::Quaternion(System::IntPtr native)
{
    auto __native = (::Quaternion*)native.ToPointer();
    X = __native->x;
    Y = __native->y;
    Z = __native->z;
    W = __native->w;
}

Flood::Quaternion::Quaternion(float u)
{
    ::Quaternion _native(u);
    this->X = _native.x;
    this->Y = _native.y;
    this->Z = _native.z;
    this->W = _native.w;
}

Flood::Quaternion::Quaternion(float a, float b, float c, float d)
{
    ::Quaternion _native(a, b, c, d);
    this->X = _native.x;
    this->Y = _native.y;
    this->Z = _native.z;
    this->W = _native.w;
}

Flood::Quaternion::Quaternion(Flood::EulerAngles angles)
{
    auto _marshal0 = ::EulerAngles();
    _marshal0.x = angles.X;
    _marshal0.y = angles.Y;
    _marshal0.z = angles.Z;
    ::Quaternion _native(_marshal0);
    this->X = _native.x;
    this->Y = _native.y;
    this->Z = _native.z;
    this->W = _native.w;
}

bool Flood::Quaternion::operator==(Flood::Quaternion q)
{
    auto _this0 = ::Quaternion();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    _this0.w = (*this).W;
    auto _marshal0 = ::Quaternion();
    _marshal0.x = q.X;
    _marshal0.y = q.Y;
    _marshal0.z = q.Z;
    _marshal0.w = q.W;
    auto arg0 = _marshal0;
    auto ret = _this0.operator==(arg0);
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    W = _this0.w;
    return ret;
}

bool Flood::Quaternion::operator!=(Flood::Quaternion q)
{
    auto _this0 = ::Quaternion();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    _this0.w = (*this).W;
    auto _marshal0 = ::Quaternion();
    _marshal0.x = q.X;
    _marshal0.y = q.Y;
    _marshal0.z = q.Z;
    _marshal0.w = q.W;
    auto arg0 = _marshal0;
    auto ret = _this0.operator!=(arg0);
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    W = _this0.w;
    return ret;
}

void Flood::Quaternion::SetIdentity()
{
    auto _this0 = ::Quaternion();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    _this0.w = (*this).W;
    _this0.identity();
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    W = _this0.w;
}

Flood::Quaternion Flood::Quaternion::Conjugate(Flood::Quaternion q)
{
    auto _this0 = ::Quaternion();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    _this0.w = (*this).W;
    auto _marshal0 = ::Quaternion();
    _marshal0.x = q.X;
    _marshal0.y = q.Y;
    _marshal0.z = q.Z;
    _marshal0.w = q.W;
    auto arg0 = _marshal0;
    auto ret = _this0.conjugate(arg0);
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    W = _this0.w;
    return Flood::Quaternion((::Quaternion*)&ret);
}

Flood::Quaternion Flood::Quaternion::operator*(Flood::Quaternion a)
{
    auto _this0 = ::Quaternion();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    _this0.w = (*this).W;
    auto _marshal0 = ::Quaternion();
    _marshal0.x = a.X;
    _marshal0.y = a.Y;
    _marshal0.z = a.Z;
    _marshal0.w = a.W;
    auto arg0 = _marshal0;
    auto ret = _this0.operator*(arg0);
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    W = _this0.w;
    return Flood::Quaternion((::Quaternion*)&ret);
}

Flood::Quaternion Flood::Quaternion::operator*=(Flood::Quaternion a)
{
    auto _this0 = ::Quaternion();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    _this0.w = (*this).W;
    auto _marshal0 = ::Quaternion();
    _marshal0.x = a.X;
    _marshal0.y = a.Y;
    _marshal0.z = a.Z;
    _marshal0.w = a.W;
    auto arg0 = _marshal0;
    auto &ret = _this0.operator*=(arg0);
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    W = _this0.w;
    return Flood::Quaternion((::Quaternion*)&ret);
}

float Flood::Quaternion::Magnitude()
{
    auto _this0 = ::Quaternion();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    _this0.w = (*this).W;
    auto ret = _this0.magnitude();
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    W = _this0.w;
    return ret;
}

void Flood::Quaternion::Normalize()
{
    auto _this0 = ::Quaternion();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    _this0.w = (*this).W;
    _this0.normalize();
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    W = _this0.w;
}

void Flood::Quaternion::SetToRotateAboutAxis(Flood::Vector3 axis, float theta)
{
    auto _this0 = ::Quaternion();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    _this0.w = (*this).W;
    auto _marshal0 = ::Vector3();
    _marshal0.x = axis.X;
    _marshal0.y = axis.Y;
    _marshal0.z = axis.Z;
    auto arg0 = _marshal0;
    _this0.setToRotateAboutAxis(arg0, theta);
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    W = _this0.w;
}

float Flood::Quaternion::SafeAcos(float x)
{
    auto _this0 = ::Quaternion();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    _this0.w = (*this).W;
    auto ret = _this0.safeAcos(x);
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    W = _this0.w;
    return ret;
}

float Flood::Quaternion::Dot(Flood::Quaternion a)
{
    auto _this0 = ::Quaternion();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    _this0.w = (*this).W;
    auto _marshal0 = ::Quaternion();
    _marshal0.x = a.X;
    _marshal0.y = a.Y;
    _marshal0.z = a.Z;
    _marshal0.w = a.W;
    auto arg0 = _marshal0;
    auto ret = _this0.dot(arg0);
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    W = _this0.w;
    return ret;
}

Flood::Quaternion Flood::Quaternion::Slerp(Flood::Quaternion q, float t)
{
    auto _this0 = ::Quaternion();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    _this0.w = (*this).W;
    auto _marshal0 = ::Quaternion();
    _marshal0.x = q.X;
    _marshal0.y = q.Y;
    _marshal0.z = q.Z;
    _marshal0.w = q.W;
    auto arg0 = _marshal0;
    auto ret = _this0.slerp(arg0, t);
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    W = _this0.w;
    return Flood::Quaternion((::Quaternion*)&ret);
}

void Flood::Quaternion::ToRotateAboutX::set(float value)
{
    auto theta = value;
    auto _this0 = ::Quaternion();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    _this0.w = (*this).W;
    _this0.setToRotateAboutX(theta);
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    W = _this0.w;
}

void Flood::Quaternion::ToRotateAboutY::set(float value)
{
    auto theta = value;
    auto _this0 = ::Quaternion();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    _this0.w = (*this).W;
    _this0.setToRotateAboutY(theta);
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    W = _this0.w;
}

void Flood::Quaternion::ToRotateAboutZ::set(float value)
{
    auto theta = value;
    auto _this0 = ::Quaternion();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    _this0.w = (*this).W;
    _this0.setToRotateAboutZ(theta);
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    W = _this0.w;
}

float Flood::Quaternion::RotationAngle::get()
{
    auto _this0 = ::Quaternion();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    _this0.w = (*this).W;
    auto ret = _this0.getRotationAngle();
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    W = _this0.w;
    return ret;
}

Flood::Vector3 Flood::Quaternion::RotationAxis::get()
{
    auto _this0 = ::Quaternion();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    _this0.w = (*this).W;
    auto ret = _this0.getRotationAxis();
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    W = _this0.w;
    return Flood::Vector3((::Vector3*)&ret);
}

Flood::EulerAngles Flood::Quaternion::EulerAngles::get()
{
    auto _this0 = ::Quaternion();
    _this0.x = (*this).X;
    _this0.y = (*this).Y;
    _this0.z = (*this).Z;
    _this0.w = (*this).W;
    auto ret = _this0.getEulerAngles();
    X = _this0.x;
    Y = _this0.y;
    Z = _this0.z;
    W = _this0.w;
    return Flood::EulerAngles((::EulerAngles*)&ret);
}

Flood::Quaternion Flood::Quaternion::Identity::get()
{
    return Flood::Quaternion((::Quaternion*)&::Quaternion::Identity);
}

