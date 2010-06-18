/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/input/InputManager.h"
#include "vapor/input/Device.h"
#include "vapor/input/Keyboard.h"
#include "vapor/input/Mouse.h"
#include "vapor/input/Joystick.h"

namespace vapor {

//-----------------------------------//

InputManager::~InputManager()
{
	foreach( InputDevice* const device, devices )
	{
		delete device;
	}
}

//-----------------------------------//

void InputManager::addDevice( InputDevice* device )
{
	if( !device )
	{
		warn( "input", "Tried to add an invalid input device" );
		return;
	}
	
	devices.push_back( device );

	info( "input", "Registered a new input device: '%s'", 
		InputDeviceType::getString( device->getType() ).c_str() );
}

//-----------------------------------//

Keyboard* InputManager::getKeyboard() const
{
	foreach( InputDevice* device, devices )
	{
		if( device->getType() == InputDeviceType::Keyboard )
		{
			return static_cast< Keyboard* > ( device );
		}
	}

	return nullptr;
}

//-----------------------------------//

Mouse* InputManager::getMouse() const
{
	foreach( InputDevice* device, devices )
	{
		if( device->getType() == InputDeviceType::Mouse )
		{
			return static_cast< Mouse* > ( device );
		}
	}

	return nullptr;
}

//-----------------------------------//

void InputManager::processEvent( const InputEvent& event )
{
	foreach( InputDevice* const device, devices )
	{
		device->processEvent( event );
	}
}

//-----------------------------------//

void InputManager::createDefaultDevices()
{
	addDevice( new Keyboard() );
	addDevice( new Mouse() );
}

//-----------------------------------//

} // end namespace

