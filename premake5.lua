#!lua
solution "Steve"
	configurations {"Debug", "Release"}
	location("prj/".._ACTION)

	project "Steve"
		language "C++"
		files {"**.cpp","**.h"}
		targetdir "bin"
		debugdir "bin"

		configuration {"Debug"}
			targetname "Steve_dbg"
			kind "ConsoleApp"
			objdir("obj/".._ACTION.."/dbg")
			defines {"L_DEBUG"}
			symbols "On"

		configuration {"Release"}
			targetname "Steve"
			kind "ConsoleApp"
			objdir("obj/".._ACTION.."/rls")
			optimize "On"
