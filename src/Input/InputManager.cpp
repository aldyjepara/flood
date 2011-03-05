/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "Input/InputManager.h"
#include "Input/Device.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Input/Joystick.h"

namespace vapor {

//-----------------------------------//

InputManager::InputManager()
{}

//-----------------------------------//

InputManager::~InputManager()
{
	for( uint i = 0; i < devices.size(); i++ )
	{
		InputDevice* device = devices[i];
		delete device;
	}
}

//-----------------------------------//

void InputManager::addDevice( InputDevice* device )
{
	if( !device )
	{
		Log::warn( "Tried to add an invalid input device" );
		return;
	}
	
	devices.push_back( device );

	Log::info( "Registered a new input device: '%s'", 
		InputDeviceType::getString( device->getType() ).c_str() );
}

//-----------------------------------//

Keyboard* InputManager::getKeyboard() const
{
	for( uint i = 0; i < devices.size(); i++ )
	{
		InputDevice* device = devices[i];
		if( device->getType() == InputDeviceType::Keyboard )
			return static_cast< Keyboard* > ( device );
	}

	return nullptr;
}

//-----------------------------------//

Mouse* InputManager::getMouse() const
{
	for( uint i = 0; i < devices.size(); i++ )
	{
		InputDevice* device = devices[i];
		if( device->getType() == InputDeviceType::Mouse )
			return static_cast< Mouse* > ( device );
	}

	return nullptr;
}

//-----------------------------------//

void InputManager::processEvent( const InputEvent& event )
{
	for( uint i = 0; i < devices.size(); i++ )
	{
		InputDevice* device = devices[i];
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

