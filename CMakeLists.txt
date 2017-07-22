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
    src/win32/*.h 
)
file(GLOB RendererFiles
    src/renderer/*.cc 
    src/renderer/*.h 
)
file(GLOB RendererImguiFiles
    src/renderer/imgui/*.cc 
    src/renderer/imgui/*.cpp
    src/renderer/imgui/*.h 
)
file(GLOB RendererBuffersFiles
    src/renderer/buffers/*.cc 
    src/renderer/buffers/*.h 
)
file(GLOB RendererCommandsFiles
    src/renderer/commands/*.cc 
    src/renderer/commands/*.h 
)
file(GLOB RendererCamerasFiles
    src/renderer/cameras/*.cc 
    src/renderer/cameras/*.h 
)
file(GLOB ContentFiles
    src/content/*.cc 
    src/content/*.h 
)
file(GLOB ContentStbFiles
    src/content/stb/*.cc 
    src/content/stb/*.h 
)
file(GLOB CoreFiles
    src/core/*.cc 
    src/core/*.h 
)
file(GLOB CoreCoreFiles
    src/core/core/*.cc 
    src/core/core/*.h 
)
file(GLOB CoreSceneFiles
    src/core/scene/*.cc 
    src/core/scene/*.h 
)
file(GLOB CoreDebugFiles
    src/core/debug/*.cc 
    src/core/debug/*.h 
)
file(GLOB UtilFiles
    src/util/*.cc 
    src/util/*.h 
)

# Put all source/header files under the right source groups
source_group("win32"                FILES       ${Win32Files})
source_group("renderer"             FILES       ${RendererFiles})
source_group("renderer\\imgui"      FILES       ${RendererImguiFiles})
source_group("renderer\\buffers"    FILES       ${RendererBuffersFiles})
source_group("renderer\\commands"   FILES       ${RendererCommandsFiles})
source_group("renderer\\cameras"    FILES       ${RendererCamerasFiles})
source_group("content"              FILES       ${ContentFiles})
source_group("content\\stb"         FILES       ${ContentStbFiles})
source_group("core"                 FILES       ${CoreFiles})
source_group("core\\core"           FILES       ${CoreCoreFiles})
source_group("core\\scene"          FILES       ${CoreSceneFiles})
source_group("core\\debug"          FILES       ${CoreDebugFiles})
source_group("util"                 FILES       ${UtilFiles})

# Add the libraries and executables to the main solution
add_library(blowbox_win32           STATIC      ${Win32Files})
add_library(blowbox_renderer        STATIC      ${RendererFiles} ${RendererImguiFiles} ${RendererBuffersFiles} ${RendererCommandsFiles} ${RendererCamerasFiles})
add_library(blowbox_content         STATIC      ${ContentFiles} ${ContentStbFiles})
add_library(blowbox_util            STATIC      ${UtilFiles})
add_executable(blowbox_core                     ${CoreFiles} ${CoreCoreFiles} ${CoreSceneFiles} ${CoreDebugFiles})

set_target_properties(blowbox_core PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

# Make the blowbox_core project the startup project in Visual Studio
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT blowbox_core)

# Link & include all the sublibraries in the main blowbox_core project
target_link_libraries(blowbox_core blowbox_renderer)
target_link_libraries(blowbox_core blowbox_content)
target_link_libraries(blowbox_core blowbox_win32)
target_link_libraries(blowbox_core blowbox_util)
include_directories("src" "deps/EASTL/test/packages/EAAssert/include")

set (BUILD_SHARED_LIBS_TEMP ${BUILD_SHARED_LIBS})
set (BUILD_SHARED_LIBS OFF CACHE BOOL "whether shared libs should be built by default" FORCE)
set (EASTL_BUILD_TESTS ON CACHE BOOL "whether to build the tests for EASTL" FORCE)
# Linking EASTL
add_subdirectory("deps/EASTL")
target_link_libraries(blowbox_core      EASTL)
target_link_libraries(blowbox_renderer  EASTL)
target_link_libraries(blowbox_content   EASTL)
target_link_libraries(blowbox_win32     EASTL)
target_link_libraries(blowbox_util      EASTL)

target_link_libraries(blowbox_core      EAStdC)
target_link_libraries(blowbox_renderer  EAStdC)
target_link_libraries(blowbox_content   EAStdC)
target_link_libraries(blowbox_win32     EAStdC)
target_link_libraries(blowbox_util      EAStdC)

target_link_libraries(blowbox_core      EATest)
target_link_libraries(blowbox_renderer  EATest)
target_link_libraries(blowbox_content   EATest)
target_link_libraries(blowbox_win32     EATest)
target_link_libraries(blowbox_util      EATest)

set (BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS_TEMP} CACHE BOOL "whether shared libs should be built by default" FORCE)

# Linking Assimp
set(CMAKE_CXX_FLAGS_DEBUG_CACHED "${CMAKE_CXX_FLAGS_DEBUG}")
set(CMAKE_CXX_FLAGS_DEBUG "/MDd /Ob2 /O2 /DNDEBUG")
add_subdirectory("deps/assimp-4.0.0")
target_link_libraries(blowbox_core      assimp)
target_link_libraries(blowbox_renderer  assimp)
target_link_libraries(blowbox_content   assimp)
target_link_libraries(blowbox_win32     assimp)
target_link_libraries(blowbox_util      assimp)
include_directories("deps/assimp-4.0.0/include")
include_directories("${CMAKE_CURRENT_BINARY_DIR}/deps/assimp-4.0.0/include")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG_CACHED}")

# Linking GLFW
add_subdirectory("deps/glfw-3.2.1")
target_link_libraries(blowbox_core      glfw)
target_link_libraries(blowbox_renderer  glfw)
target_link_libraries(blowbox_content   glfw)
target_link_libraries(blowbox_win32     glfw)
target_link_libraries(blowbox_util      glfw)
include_directories("deps/glfw-3.2.1/include")

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

set_target_properties(blowbox_core                          PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

# Organize all projects into folders
set_target_properties(blowbox_core                          PROPERTIES FOLDER blowbox)
set_target_properties(blowbox_content                       PROPERTIES FOLDER blowbox)
set_target_properties(blowbox_renderer                      PROPERTIES FOLDER blowbox)
set_target_properties(blowbox_win32                         PROPERTIES FOLDER blowbox)
set_target_properties(blowbox_util                          PROPERTIES FOLDER blowbox)

set_target_properties(assimp                                PROPERTIES FOLDER deps/assimp)

if (TARGET assimp_cmd)
    set_target_properties(assimp_cmd                            PROPERTIES FOLDER deps/assimp)
endif(TARGET assimp_cmd)

if (TARGET assimp_viewer)
    set_target_properties(assimp_viewer                     PROPERTIES FOLDER deps/assimp)
endif (TARGET assimp_viewer)

if (TARGET gtest)
    set_target_properties(gtest                             PROPERTIES FOLDER deps/assimp)
endif (TARGET gtest)

if (TARGET unit)
    set_target_properties(unit                              PROPERTIES FOLDER deps/assimp)
endif (TARGET unit)

set_target_properties(uninstall                             PROPERTIES FOLDER deps/assimp)
set_target_properties(UpdateAssimpLibsDebugSymbolsAndDLLs   PROPERTIES FOLDER deps/assimp)
set_target_properties(zlibstatic                            PROPERTIES FOLDER deps/assimp)
set_target_properties(IrrXML                                PROPERTIES FOLDER deps/assimp)

set_target_properties(EASTL                                 PROPERTIES FOLDER deps/eastl)
set_target_properties(EABase_ide                            PROPERTIES FOLDER deps/eastl)
set_target_properties(EAAssert_ide                          PROPERTIES FOLDER deps/eastl)
set_target_properties(EAMain_ide                            PROPERTIES FOLDER deps/eastl)
set_target_properties(EAThread_ide                          PROPERTIES FOLDER deps/eastl)
set_target_properties(EAStdC                                PROPERTIES FOLDER deps/eastl)
set_target_properties(EASTLTest                             PROPERTIES FOLDER deps/eastl)
set_target_properties(EATest                                PROPERTIES FOLDER deps/eastl)
if (BUILD_TESTING)
    set_target_properties(Continuous                            PROPERTIES FOLDER deps/eastl)
    set_target_properties(Experimental                          PROPERTIES FOLDER deps/eastl)
    set_target_properties(Nightly                               PROPERTIES FOLDER deps/eastl)
    set_target_properties(NightlyMemoryCheck                    PROPERTIES FOLDER deps/eastl)
endif (BUILD_TESTING)

set_target_properties(glfw                                  PROPERTIES FOLDER deps/glfw)

if (GLFW_BUILD_EXAMPLES)
    set_target_properties(boing                             PROPERTIES FOLDER deps/glfw/examples)
    set_target_properties(gears                             PROPERTIES FOLDER deps/glfw/examples)
    set_target_properties(heightmap                         PROPERTIES FOLDER deps/glfw/examples)
    set_target_properties(particles                         PROPERTIES FOLDER deps/glfw/examples)
    set_target_properties(simple                            PROPERTIES FOLDER deps/glfw/examples)
    set_target_properties(splitview                         PROPERTIES FOLDER deps/glfw/examples)
    set_target_properties(wave                              PROPERTIES FOLDER deps/glfw/examples)
endif (GLFW_BUILD_EXAMPLES)

if (GLFW_BUILD_TESTS)
    set_target_properties(clipboard                         PROPERTIES FOLDER deps/glfw/tests)
    set_target_properties(cursor                            PROPERTIES FOLDER deps/glfw/tests)
    set_target_properties(empty                             PROPERTIES FOLDER deps/glfw/tests)
    set_target_properties(events                            PROPERTIES FOLDER deps/glfw/tests)
    set_target_properties(gamma                             PROPERTIES FOLDER deps/glfw/tests)
    set_target_properties(glfwinfo                          PROPERTIES FOLDER deps/glfw/tests)
    set_target_properties(icon                              PROPERTIES FOLDER deps/glfw/tests)
    set_target_properties(iconify                           PROPERTIES FOLDER deps/glfw/tests)
    set_target_properties(joysticks                         PROPERTIES FOLDER deps/glfw/tests)
    set_target_properties(monitors                          PROPERTIES FOLDER deps/glfw/tests)
    set_target_properties(msaa                              PROPERTIES FOLDER deps/glfw/tests)
    set_target_properties(reopen                            PROPERTIES FOLDER deps/glfw/tests)
    set_target_properties(sharing                           PROPERTIES FOLDER deps/glfw/tests)
    set_target_properties(tearing                           PROPERTIES FOLDER deps/glfw/tests)
    set_target_properties(threads                           PROPERTIES FOLDER deps/glfw/tests)
    set_target_properties(timeout                           PROPERTIES FOLDER deps/glfw/tests)
    set_target_properties(title                             PROPERTIES FOLDER deps/glfw/tests)
    set_target_properties(windows                           PROPERTIES FOLDER deps/glfw/tests)
endif (GLFW_BUILD_TESTS)

if (TARGET docs)
    set_target_properties(docs                              PROPERTIES FOLDER deps/glfw/docs)
endif (TARGET docs)