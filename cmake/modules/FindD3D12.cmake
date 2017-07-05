# Loosely based on https://github.com/Microsoft/DirectXShaderCompiler/blob/master/cmake/modules/FindD3D12.cmake

# Find the win10 SDK path.
get_filename_component(WIN10_SDK_PATH "[HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Microsoft\\Microsoft SDKs\\Windows\\v10.0;InstallationFolder]" ABSOLUTE CACHE)

find_path(D3D12_INCLUDE_DIR    # Set variable D3D12_INCLUDE_DIR
          d3d12.h                # Find a path with d3d12.h
          HINTS "${WIN10_SDK_PATH}/Include/${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/um"
          DOC "path to WIN10 SDK header files"
          HINTS
          )

find_path(DXGI_INCLUDE_DIR    # Set variable DXGI_INCLUDE_DIR
          dxgi1_4.h           # Find a path with dxgi1_4.h
          HINTS "${WIN10_SDK_PATH}/Include/${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/shared"
          DOC "path to WIN10 SDK header files"
          HINTS
          )

if (CMAKE_SIZEOF_VOID_P EQUAL "8")
    set (WIN10_SDK_LIB_PATH "${WIN10_SDK_PATH}/Lib/${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/um/x64")
elseif (CMAKE_SIZEOF_VOID_P EQUAL "4")
    set (WIN10_SDK_LIB_PATH "${WIN10_SDK_PATH}/Lib/${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/um/x84")
endif (CMAKE_SIZEOF_VOID_P EQUAL "8")

find_library(D3D12_LIBRARY          NAMES d3d12.lib         HINTS ${WIN10_SDK_LIB_PATH} )
find_library(DXGI_LIBRARY           NAMES dxgi.lib          HINTS ${WIN10_SDK_LIB_PATH} )
find_library(D3DCOMPILER_LIBRARY    NAMES d3dcompiler.lib   HINTS ${WIN10_SDK_LIB_PATH} )

set(D3D12_LIBRARIES     
    ${D3D12_LIBRARY} 
    ${DXGI_LIBRARY}
    ${DXGUID_LIBRARY}
    ${D3DCOMPILER_LIBRARY}
)    
set(D3D12_INCLUDE_DIRS  ${D3D12_INCLUDE_DIR} ${DXGI_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(D3D12 "We tried searching for the correct d3d12 include directory and the d3d12 libraries, but couldn't find them on your PC. Please make sure you have installed the Windows 10 SDK. This should place the files automatically in the correct folders."
                                  D3D12_INCLUDE_DIRS D3D12_LIBRARIES)