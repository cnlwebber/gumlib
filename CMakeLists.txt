# CMakeList.txt : CMake project for gumlib, include source and define
# project specific logic here.
#


cmake_minimum_required (VERSION 4.2)
set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(PROJECT_NAME
    "gumlib"
)

# we usin modules

project (${PROJECT_NAME})

if(MSVC)
    add_compile_options(/wd5050)
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_MODULE_STD ON) 

# find vcpkg packages
find_package(glfw3		CONFIG REQUIRED)
find_package(glad		CONFIG REQUIRED)
find_package(glm		CONFIG REQUIRED)
find_package(spdlog		CONFIG REQUIRED)
find_package(imgui		CONFIG REQUIRED)
find_package(assimp		CONFIG REQUIRED)
find_package(Freetype	CONFIG REQUIRED)
find_package(tracy		CONFIG REQUIRED)


# Add source to this project's executable.

set(SOURCES
	"src/main.cpp"
	"src/stb_implementation.cpp"
)

add_executable (${PROJECT_NAME} ${SOURCES})

# define modules
target_sources(${PROJECT_NAME}
    PUBLIC FILE_SET CXX_MODULES
        BASE_DIRS src
        FILES 
            src/gumlib.ixx
            src/component.ixx
            src/entity.ixx
            src/registry.ixx
            src/config.ixx
            src/view.ixx
            src/system.ixx
            src/context.ixx
            src/input.ixx
            src/window.ixx
)

if(TARGET Tracy::Tracy)
    target_compile_definitions(${PROJECT_NAME} PRIVATE TRACY_ENABLE)
endif()

# link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE 
    Tracy::TracyClient
    glfw 
    glad::glad
    glm::glm
    spdlog::spdlog
    imgui::imgui
    assimp::assimp
    Freetype::Freetype
)

# expose stb/miniaudio header files
target_include_directories(${PROJECT_NAME} PRIVATE ${Stb_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${MINIAUDIO_INCLUDE_DIRS})


# TODO: Add tests and install targets if needed.
