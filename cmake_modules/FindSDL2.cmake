#------------------------------------------------------------------------------
# Usage: find_package(SDL2 [REQUIRED] [COMPONENTS main])
#
# Sets variables:
#	 SDL2_INCLUDE_DIRS
#	 SDL2_LIBS
#	 SDL2_DLLS
#------------------------------------------------------------------------------

include(FindPackageHandleStandardArgs)

# Check if "main" was specified as a component
set(_SDL2_use_main FALSE)
foreach(_SDL2_component ${SDL2_FIND_COMPONENTS})
	if(_SDL2_component STREQUAL "main")
		set(_SDL2_use_main TRUE)
	else()
		message(WARNING "Unrecognized component \"${_SDL2_component}\"")
	endif()
endforeach()

if(WIN32)
	set(SDL2_ROOT "${CMAKE_CURRENT_LIST_DIR}/../external/sdl2-precompiled")

	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		set(SDL2_LIB_PATH_PREFIX "x64")
	else()
		set(SDL2_LIB_PATH_PREFIX "x86")
	endif()

	if(MINGW)
		set(SDL2_ROOT "${SDL2_ROOT}/mingw")
		if (EXISTS "${SDL2_ROOT}/libs/${SDL2_LIB_PATH_PREFIX}/libSDL2.dll.a")
			set(SDL2_LIBS "${SDL2_ROOT}/libs/${SDL2_LIB_PATH_PREFIX}/libSDL2.dll.a")
			set(SDL2_DLLS "${SDL2_ROOT}/../msvc/libs/${SDL2_LIB_PATH_PREFIX}/SDL2.dll")
			if(_SDL2_use_main)
				list(APPEND SDL2_LIBS "${SDL2_ROOT}/libs/${SDL2_LIB_PATH_PREFIX}/libSDL2main.a")
			endif()
		endif()
	elseif(MSVC)
		set(SDL2_ROOT "${SDL2_ROOT}/msvc")
		if (EXISTS "${SDL2_ROOT}/libs/${SDL2_LIB_PATH_PREFIX}/SDL2.lib")
			set(SDL2_LIBS "${SDL2_ROOT}/libs/${SDL2_LIB_PATH_PREFIX}/SDL2.lib")
			set(SDL2_DLLS "${SDL2_ROOT}/libs/${SDL2_LIB_PATH_PREFIX}/SDL2.dll")
			if(_SDL2_use_main)
				list(APPEND SDL2_LIBS "${SDL2_ROOT}/libs/${SDL2_LIB_PATH_PREFIX}/SDL2main.lib")
			endif()
		endif()
	endif()
	set(SDL2_INCLUDE_DIRS "${SDL2_ROOT}/include")

	unset(SDL2_LIB_PATH_PREFIX)

	mark_as_advanced(SDL2_ROOT)
	find_package_handle_standard_args(SDL2 DEFAULT_MSG SDL2_INCLUDE_DIRS SDL2_LIBS SDL2_DLLS)
else()
	# On MacOS, should be installed via Macports
	# On Ubuntu, install with: apt-get install libsdl2-dev
	find_path(SDL2_INCLUDE_DIRS SDL.h PATH_SUFFIXES SDL2)
	find_library(_SDL2_LIB SDL2)
	set(SDL2_LIBS ${SDL2})
	if(_SDL2_use_main)
		find_library(_SDL2main_LIB SDL2)
		list(APPEND SDL2_LIBS ${_SDL2main_LIB})
	endif()

	mark_as_advanced(SDL2_INCLUDE_DIRS _SDL2_LIB _SDL2main_LIB)
	find_package_handle_standard_args(SDL2 DEFAULT_MSG SDL2_INCLUDE_DIRS SDL2_LIBS)
endif()

unset(_SDL2_use_main)
