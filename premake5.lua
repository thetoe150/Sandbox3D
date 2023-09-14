workspace"Sandbox"
	location"../"
	configurations{"Debug","Release"}

project"Sanbox"
	location"build/"
	kind"ConsoleApp"
	language"C++"
	files{
		"src/**.cpp",
		"src/**.c",
		"inc/**.hpp",
		"inc/**.h",
	}
	includedirs"inc/"

	libdirs"lib/"
	links{"glfw3dll", "assimp"}

	targetdir"bin/"
	targetname"Sandbox"

	filter"configurations:Debug"
	defines{"DEBUG"}
	symbols"On"

	filter"configurations:Release"
	defines{"RELEASE"}
	optimize"On"

	filter{}
