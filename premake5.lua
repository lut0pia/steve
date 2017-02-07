#!lua
solution "Steve"
	configurations {"Debug", "Release"}

	project "Steve"
		location("prj/".._ACTION)
		language "C++"
		files {"**.cpp","**.h"}
		targetdir "bin"
		debugdir "bin"

		configuration {"windows"}
			links {"winmm"}

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
			flags {"Optimize"}
