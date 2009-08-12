/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/CompileOptions.h"

#ifdef VAPOR_WINDOWING_WIN32

#include "vapor/Platform.h"
#include "vapor/render/Window.h"

#ifndef VAPOR_PLATFORM_WINDOWS
	#error This code is Windows only. Disable VAPOR_WINDOWING_WIN32.
#endif

namespace vapor {
	namespace render {

/**
 * Another window implementation. Uses only native Windows API calls. 
 * This will hopefully be faster than using SDL windowing for all the
 * platforms, and indeed I've noticed that SDL is a little bit unreliable
 * sometimes. SDL also doesn't provide OpenGL 3.0 support, and we might
 * to play a little bit with it soon. :)
 */

class Win32Window : public Window
{
public:

	Win32Window(Settings& settings);
	virtual ~Win32Window();

	// Swaps the buffers (updates the display).
	void update();

	// Handle the window message events.
	bool pumpEvents();

	// Sets the title of the window.
	void setTitle(const std::string& title) const;

	// Sets the cursor visibility.
	void setCursor(bool state) const;

	// Sets this context as the current.
	void makeCurrent();

private:

	// These do the real work!
	bool createWindow();
	bool createContext();
	bool registerClass();

	// Get a string of the latest Windows error.
	std::string getErrorMessage();

	// Instance
	HINSTANCE hInstance;

	// Device context
	HDC hDC;

	// Window handle
	HWND hWnd;

	// GL context
	HGLRC hRC;
};

LONG WINAPI WindowProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

} } // end namespaces

#endif