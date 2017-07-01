# set minimum version
cmake_minimum_required (VERSION 3.9)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

# Set solution name 
project(blowbox)

# Turn on folder support to organize projects in solutions into folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "deps/cmake")

# Blowbox versioning
set(BLOWBOX_VERSION_MAJOR 1)
set(BLOWBOX_VERSION_MINOR 0)
add_definitions(-DBLOWBOX_VERSION_MAJOR=${BLOWBOX_VERSION_MAJOR})
add_definitions(-DBLOWBOX_VERSION_MINOR=${BLOWBOX_VERSION_MINOR})

# Throw an error if we're not on Windows
if (NOT WIN32)
    message (FATAL_ERROR "Blowbox only works on Windows unfortunately. It cannot be compiled on other systems. This is because Blowbox is strictly a D3D12 based engine.")
endif (NOT WIN32)

# Figure out whether we're building 64 bit or 32 bit
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set (BLOWBOX_IS_64BIT ON BOOL "Whether we're configuring for 64 bit or 32 bit")
else ()
    set (BLOWBOX_IS_64BIT OFF BOOL "Whether we're configuring for 64 bit or 32 bit")
endif(CMAKE_SIZEOF_VOID_P EQUAL 8)

if (NOT BLOWBOX_IS_64BIT)
    message(FATAL_ERROR "Blowbox can only be compiled as 64 bit")
endif (NOT BLOWBOX_IS_64BIT)

# Find all source/header files for the different projects
file(GLOB Win32Files 
    src/win32/*.cc 
    src/win32/*/*.cc 
    src/win32/*/*/*.cc
    src/win32/*/*/*/*.cc
    src/win32/*.h 
    src/win32/*/*.h 
    src/win32/*/*/*.h
    src/win32/*/*/*/*.h
)
file(GLOB RendererFiles
    src/renderer/*.cc 
    src/renderer/*/*.cc 
    src/renderer/*/*/*.cc
    src/renderer/*/*/*/*.cc
    src/renderer/*.h 
    src/renderer/*/*.h 
    src/renderer/*/*/*.h
    src/renderer/*/*/*/*.h
)
file(GLOB ContentFiles
    src/content/*.cc 
    src/content/*/*.cc 
    src/content/*/*/*.cc
    src/content/*/*/*/*.cc
    src/content/*.h 
    src/content/*/*.h 
    src/content/*/*/*.h
    src/content/*/*/*/*.h
)
file(GLOB CoreFiles
    src/core/*.cc 
    src/core/*/*.cc 
    src/core/*/*/*.cc
    src/core/*/*/*/*.cc
    src/core/*.h 
    src/core/*/*.h 
    src/core/*/*/*.h
    src/core/*/*/*/*.h
)

# Put all source/header files under the right source groups
source_group("win32"            FILES       ${Win32Files})
source_group("renderer"         FILES       ${RendererFiles})
source_group("content"          FILES       ${ContentFiles})
source_group("core"             FILES       ${CoreFiles})

# Add the libraries and executables to the main solution
add_library(blowbox_win32       STATIC      ${Win32Files})
add_library(blowbox_renderer    STATIC      ${RendererFiles})
add_library(blowbox_content     STATIC      ${ContentFiles})
add_executable(blowbox_core                 ${CoreFiles})

# Make the blowbox_core project the startup project in Visual Studio
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT blowbox_core)

# Link & include all the sublibraries in the main blowbox_core project
target_link_libraries(blowbox_core blowbox_renderer)
target_link_libraries(blowbox_core blowbox_content)
target_link_libraries(blowbox_core blowbox_win32)
include_directories(blowbox_core PUBLIC src)

set (BUILD_SHARED_LIBS_TEMP ${BUILD_SHARED_LIBS})
set (BUILD_SHARED_LIBS OFF CACHE BOOL "whether shared libs should be built by default" FORCE)
# Linking EASTL
add_subdirectory("deps/EASTL")
target_link_libraries(blowbox_core      EASTL)
target_link_libraries(blowbox_renderer  EASTL)
target_link_libraries(blowbox_content   EASTL)
target_link_libraries(blowbox_win32     EASTL)
set (BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS_TEMP} CACHE BOOL "whether shared libs should be built by default" FORCE)

# Linking Assimp
add_subdirectory("deps/assimp-3.3.1")
target_link_libraries(blowbox_core      assimp)
target_link_libraries(blowbox_renderer  assimp)
target_link_libraries(blowbox_content   assimp)
target_link_libraries(blowbox_win32     assimp)
include_directories("deps/assimp-3.3.1/include")

# Linking D3D12
if (WIN32)
    find_package(D3D12 REQUIRED)
    if(D3D12_FOUND)
        message(STATUS "D3D12 library and include directories/files could be found.")
        
        target_include_directories(blowbox_renderer PUBLIC ${D3D12_INCLUDE_DIRS})        
        target_link_libraries(blowbox_renderer ${D3D12_LIBRARIES})
    else ()
        message(FATAL_ERROR "D3D12 library and include directories/files couldn't be found.")
    endif(D3D12_FOUND)
endif(WIN32)

# Organize all projects into folders
set_target_properties(assimp                                PROPERTIES FOLDER deps/assimp)
set_target_properties(assimp_cmd                            PROPERTIES FOLDER deps/assimp)
if (TARGET assimp_viewer)
	set_target_properties(assimp_viewer                         PROPERTIES FOLDER deps/assimp)
endif (TARGET assimp_viewer)
set_target_properties(gtest                                 PROPERTIES FOLDER deps/assimp)
set_target_properties(uninstall                             PROPERTIES FOLDER deps/assimp)
set_target_properties(unit                                  PROPERTIES FOLDER deps/assimp)
set_target_properties(UpdateAssimpLibsDebugSymbolsAndDLLs   PROPERTIES FOLDER deps/assimp)
set_target_properties(zlibstatic                            PROPERTIES FOLDER deps/assimp)
set_target_properties(EABase_ide                            PROPERTIES FOLDER deps/EASTL)
set_target_properties(EASTL                                 PROPERTIES FOLDER deps/EASTL)
set_target_properties(blowbox_core                          PROPERTIES FOLDER blowbox)
set_target_properties(blowbox_content                       PROPERTIES FOLDER blowbox)
set_target_properties(blowbox_renderer                      PROPERTIES FOLDER blowbox)
set_target_properties(blowbox_win32                         PROPERTIES FOLDER blowbox)