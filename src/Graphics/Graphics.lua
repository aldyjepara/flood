Graphics = {}
Graphics.name = "Graphics"
Graphics.shared = true
Graphics.defines = {}

project "Graphics"

	uuid "01BBC20D-8550-413A-9176-BB5911A68F82"

	if Graphics.shared then
		kind "SharedLib"
		table.insert(Graphics.defines, "API_GRAPHICS_DLL")
		defines { "API_GRAPHICS_DLL_EXPORT" }
	else
		kind "StaticLib"
	end
	
	SetupNativeProjects()

	defines { Core.defines, Resources.defines, Graphics.defines }

	files
	{
		"Graphics.lua",
		path.join(incdir,"Graphics/**.h"),
		"**.cpp",
		"**.h",	
	}
	
	vpaths
	{
		["*"] = { ".", path.join(incdir,"Graphics") },
	}

	includedirs
	{
		incdir,
	}

	Graphics.libdirs =
	{
	}

	Graphics.links =
	{
		"opengl32",
	}

	links { Core.name, Resources.name, Graphics.links }
	
	Graphics.deps =
	{
		"GLEW",
	}
	
	deps(Graphics.deps)