/************************************************************************
*
* Flood Project � (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Engine/API.h"
#include "Graphics/RenderTarget.h"
#include "Engine/Input/InputManager.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

typedef void* WindowHandle;

enum class WindowStyles
{
	None = 0,
	TopLevel   = 1 << 0,
	MiniFrame  = 1 << 1,
};

class API_ENGINE WindowSettings : public Settings
{
public:

	WindowSettings( uint16 width = 640, uint16 height = 480,
		const String& title = "Untitled", bool fullscreen = false );

	String title;
	bool fullScreen;
	WindowHandle handle;
	WindowStyles styles;
};

//-----------------------------------//

/**
 * In most platforms (PCs) this will be just a normal window on the
 * desktop, but on some platforms (consoles, for instance) this might
 * be slighty different, so some methods might not make much sense.
 */

class API_ENGINE Window : public RenderTarget
{
public:

	Window (const WindowSettings& settings);

	// Creates a new render context.
	virtual RenderContext* createContext(const RenderContextSettings&) = 0;

	// Updates the window content.
	void update() OVERRIDE {}

	// Sets this rendering target as the current.
	void makeCurrent() OVERRIDE {}

	// Shows/hides the window.
	virtual void show( bool visible = true ) {}

	// Handles the event loop of the window.
	virtual bool pumpEvents() { return true; }

	// Sets the title of the window.
	virtual void setTitle (const String& title) {}

	// Sets the visibility of the mouse cursor.
	virtual void setCursorVisible( bool state ) {}

	// Gets the visibility of the mouse cursor.
	virtual bool isCursorVisible() const { return false; }

	// Sets the capture of the mouse cursor.
	virtual void setCursorCapture( bool state ) {}

	// Gets the cursor position on screen.
	virtual Vector2i getCursorPosition() const { return Vector2i(0,0); }

	// Sets the cursor position on screen.
	virtual void setCursorPosition( int x, int y );

	// Gets if the window has focus.
	virtual bool hasFocus() { return false; }

	// Gets the input manager.
	virtual InputManager* getInput() { return nullptr; }

	// Gets the window settings.
	GETTER(Settings, const Settings&, settings)

	// Event fired when the window is closed.
	Event0<> onWindowClose;

	// Event fired when the window focus changes.
	Event1<bool> onWindowFocusChange;

protected:

	// Handles the resize event.
	void handleWindowResize();

	// Handles the close event.
	void handleWindowClose();

	// Handles the focus event.
	void handleWindowFocus( bool focusLost );

	// Holds the window settings.
	WindowSettings settings;

	// Render context associated with this window.
	RenderContextPtr renderContext;
};

//-----------------------------------//

NAMESPACE_ENGINE_END