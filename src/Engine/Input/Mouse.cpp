/************************************************************************
*
* Flood Project � (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Engine/API.h"
#include "Engine/Input/Mouse.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

#define CAST_EVENT(type, var, evt)		\
	type var = static_cast<type>(evt);

void Mouse::processEvent( const InputEvent& event )
{
	if( event.deviceType != InputDeviceType::Mouse )
		return;

	CAST_EVENT(const MouseEvent&, mevt, event)
	
	switch( mevt.eventType )
	{
		case MouseEventType::MousePress:
		{
			CAST_EVENT(const MouseButtonEvent&, mbe, mevt)
			mouseButtonPressed( mbe );
			break;
		}
		
		case MouseEventType::MouseRelease:
		{
			CAST_EVENT(const MouseButtonEvent&, mbe, mevt)
			mouseButtonReleased( mbe );
			break;
		}
		
		case MouseEventType::MouseMove:
		{
			CAST_EVENT(const MouseMoveEvent&, mm, mevt)
			mouseMoved( mm );
			break;
		}

		case MouseEventType::MouseDrag:
		{
			CAST_EVENT(const MouseDragEvent&, mde, mevt)			
			mouseDragged( mde );
			break;
		}

		case MouseEventType::MouseWheelMove:
		{
			CAST_EVENT(const MouseWheelEvent&, mwe, mevt)
			mouseWheelMove( mwe );
			break;
		}
		
		case MouseEventType::MouseEnter:
		{
			mouseEnter();
			break;
		}
		
		case MouseEventType::MouseExit:
		{
			mouseExit();
			break;
		}
	}
}

//-----------------------------------//

bool Mouse::isButtonPressed( MouseButton button ) const
{
	const MouseInfo& mouseInfo = getMouseInfo();

	switch( button )
	{
		case MouseButton::Left:
			return mouseInfo.leftButton;
		case MouseButton::Right:
			return mouseInfo.rightButton;
		case MouseButton::Middle:
			return mouseInfo.middleButton;
		case MouseButton::Mouse4:
			return mouseInfo.aux1Button;
		case MouseButton::Mouse5:
			return mouseInfo.aux2Button;
	}

	LogWarn( "Could not map enum: Invalid mouse button" );
	return false;
}
	
//-----------------------------------//

void Mouse::mouseMoved(const MouseMoveEvent& event)
{	
	mouseInfo.x = event.x;
	mouseInfo.y = event.y;

	onMouseMove(event);
}

//-----------------------------------//

void Mouse::mouseDragged(const MouseDragEvent& de)
{
	MouseDragEvent& event = (MouseDragEvent&) de;

	event.dx = mouseInfo.x - event.x;
	event.dy = mouseInfo.y - event.y;
	event.info = &mouseInfo;
	
	mouseInfo.x = event.x;
	mouseInfo.y = event.y;

	onMouseDrag(event);
}

//-----------------------------------//

void Mouse::mouseButtonPressed(const MouseButtonEvent& mbp)
{
	mouseInfo.x = mbp.x;
	mouseInfo.y = mbp.y;
	
	setMouseButton( mbp.button, true );
	onMouseButtonPress( mbp );
}

//-----------------------------------//

void Mouse::mouseButtonReleased(const MouseButtonEvent& mbe)
{
	mouseInfo.x = mbe.x;
	mouseInfo.y = mbe.y;
	
	setMouseButton( mbe.button, false );
	onMouseButtonRelease( mbe );
}

//-----------------------------------//

void Mouse::mouseEnter()
{
	mouseInfo.insideWindow = true;
	onMouseEnter();
}

//-----------------------------------//

void Mouse::mouseExit()
{
	mouseInfo.insideWindow = false;
	onMouseExit();
}

//-----------------------------------//

void Mouse::mouseWheelMove(const MouseWheelEvent& mevt)
{
	onMouseWheelMove( mevt );
}

//-----------------------------------//

void Mouse::setMouseButton( MouseButton button, bool state )
{
	switch(button)
	{
	case MouseButton::Left:
		mouseInfo.leftButton = state;
		break;
		
	case MouseButton::Right:
		mouseInfo.rightButton = state;
		break;
		
	case MouseButton::Middle:
		mouseInfo.middleButton = state;
		break;
		
	case MouseButton::Mouse4:
		mouseInfo.aux1Button = state;
		break;
		
	case MouseButton::Mouse5:
		mouseInfo.aux2Button = state;
		break;
	}
}

//-----------------------------------//

MouseInfo::MouseInfo()
	: x(0), y(0)
	, leftButton(false)
	, rightButton(false)
	, middleButton(false)
	, aux1Button(false)
	, aux2Button(false)
	, insideWindow(false)
{ }

//-----------------------------------//

MouseEvent::MouseEvent( MouseEventType eventType )
	: InputEvent( InputDeviceType::Mouse )
	, eventType( eventType )
{ }

//-----------------------------------//

MouseMoveEvent::MouseMoveEvent()
	: MouseEvent( MouseEventType::MouseMove )
{ }

//-----------------------------------//

MouseDragEvent::MouseDragEvent()
	: MouseEvent( MouseEventType::MouseDrag )
{ }

//-----------------------------------//

MouseButtonEvent::MouseButtonEvent( MouseEventType eventType )
	: MouseEvent(eventType)
{ }

//-----------------------------------//

MouseWheelEvent::MouseWheelEvent()
	: MouseEvent( MouseEventType::MouseWheelMove )
{ }

//-----------------------------------//

NAMESPACE_ENGINE_END
