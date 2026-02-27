cmake_minimum_required(VERSION 3.16)

# TODO: Need to make an automatic script to install llvm or give a warning for the user to install llvm
# This is so that our compiler uses Clang
if(WIN32)
  set(CMAKE_CXX_COMPILER "C:/Program Files/LLVM/bin/clang++.exe" CACHE STRING "Clang-cl C++ Compiler" FORCE)
  set(CMAKE_C_COMPILER "C:/Program Files/LLVM/bin/clang.exe" CACHE STRING "Clang-cl C Compiler" FORCE)
  set(CMAKE_LINKER "lld-link.exe" CACHE FILEPATH "LLD Linker")
  set(CMAKE_RC_COMPILER "C:Program Files/LLVM/bin/llvm-rc.exe" CACHE FILEPATH "RC COMPILER")
elseif(UNIX)
  set(CMAKE_CXX_COMPILER "/usr/bin/clang++-21" CACHE STRING "Clang-cl C++ Compiler" FORCE)
  set(CMAKE_C_COMPILER "/usr/bin/clang-21" CACHE STRING "Clang-cl C Compiler" FORCE)
  set(CMAKE_LINKER "/usr/bin/lld-link-21" CACHE FILEPATH "LLD Linker")
  set(CMAKE_RC_COMPILER "/usr/bin/llvm-rc-21" CACHE FILEPATH "RC COMPILER")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_WARN_DEPRECATED OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(JSON_BuildTest OFF CACHE INTERNAL "")

# Always build static libs to avoid DLL issues on Windows
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

project(VoxelGame)

find_package(Git QUIET)
if(GIT_FOUND)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        RESULT_VARIABLE _git_submodule_result
        OUTPUT_QUIET ERROR_QUIET
    )
    if(NOT _git_submodule_result EQUAL 0)
    message(WARNING "git submodule update --init --recursive failed with exit code ${_git_submodule_result}")
  endif()
else()
  message(STATUS "Git not found; skipping automatic git submodule initialization")
endif()

#ADD Raylib as an Repository

add_subdirectory(vendor/raylib SYSTEM)

add_subdirectory(vendor/raylib-nuklear SYSTEM)

#ADD SOURCE and HEADER FILES

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS src/*.cpp)
#file(GLOB_RECURSE HDR_FILES CONFIGURE_DEPENDS include/*.h Game/include/Networing/*.h) # TODO: We need to remove header files from executable

add_executable(VoxelGame
    ${SRC_FILES}
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/vendor/raylib-nuklear/include)

source_group("Source Files" FILES ${SRC_FILES})

target_link_libraries(VoxelGame PUBLIC raylib)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/resources
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources)
            
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/saves
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/saves)