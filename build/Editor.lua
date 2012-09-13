Editor = {}
Editor.defines = {}

function SetupWxWidgets()
	wxWidgets = {}
	
	wxWidgets.includedirs =
	{ 
		"../deps/wxWidgets/include",
		"../deps/wxWidgets/include/msvc"
	}

	wxWidgets.libdirs =
	{ 
		"../deps/wxWidgets/lib/vc_dll",
	}	
	
	wxWidgets.links = { "" }

	wxWidgets.defines = { '_LIB', 'WXUSINGDLL', 'WXMONOLITHIC' }

	defines { wxWidgets.defines }

	local c = configuration "windows"
		defines { "__WXMSW__" }
		
	configuration { "windows", "Debug" }
		links { "wxmsw29ud","wxmsw29ud_gl" }
		
	configuration { "windows", "Release" }
		links { "wxmsw29u","wxmsw29u_gl" }

	configuration (c)
end

project "Editor"

	debugdir "../bin"

	builddeps { Core.name, Resources.name, Graphics.name, Engine.name, Pipeline.name }
	
	defines
	{
		Core.defines,
		Resources.defines,
		Graphics.defines,
		Engine.defines,
		Pipeline.defines
	}

	editor_flags = flags_common
	table.remove(editor_flags, #editor_flags)
	
	flags { editor_flags }

	pchheader "Editor/API.h"
	pchsource "../src/Editor/Editor.cpp"
	
	configuration "Debug"
		kind "ConsoleApp"
	
	configuration "Release"
		kind "WindowedApp"
		flags { "WinMain" }
		
	configuration "*"	
	
	files
	{
		"Editor.lua",
		"../inc/Editor/**.h",
		"../src/Editor/**.h",
		"../src/Editor/**.cpp",
		"../src/Editor/**.rc",
	}
	
	vpaths
	{
		[""] = { "src/Editor/", "inc/Editor/" },
	}

	Editor.deps =
	{
		"Mono"
	}
	
	SetupWxWidgets()

	includedirs
	{
		"../inc/",
		"../src/",
		"../inc/Editor",
		"../src/Editor",
		"../src/Editor/Widgets",
		wxWidgets.includedirs
	}

	libdirs
	{
		"lib/",
		Core.libdirs,
		Resources.libdirs,
		Engine.libdirs,
		Pipeline.libdirs,
		wxWidgets.libdirs,
	}
	
	links
	{
		Engine.name,
		Engine.links,
		Pipeline.name,
		Pipeline.links,
		wxWidgets.links
	}

	deps { Editor.deps }
		
	configuration "vs*"
		defines { "_CRT_SECURE_NO_WARNINGS" }

	configuration "windows"
		links { Mono.links }		
