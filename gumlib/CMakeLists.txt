# CMakeList.txt : CMake project for gumlib, include source and define
# project specific logic here.
#
set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")

if(MSVC)
    add_compile_options(/wd5050)
endif()


set(PROJECT_NAME
    "gumlib"
)

project (${PROJECT_NAME})

# find vcpkg packages
find_package(glfw3		CONFIG REQUIRED)
find_package(glad		CONFIG REQUIRED)
find_package(glm		CONFIG REQUIRED)
find_package(spdlog		CONFIG REQUIRED)
find_package(imgui		CONFIG REQUIRED)
find_package(assimp		CONFIG REQUIRED)
find_package(Freetype	CONFIG REQUIRED)
find_package(tracy		CONFIG REQUIRED)

add_library(${PROJECT_NAME} STATIC src/stb_implementation.cpp)

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
    spdlog::spdlog
    imgui::imgui
    assimp::assimp
    Freetype::Freetype
)

target_link_libraries(${PROJECT_NAME} PUBLIC
        glm::glm
)

# expose stb/miniaudio header files
target_include_directories(${PROJECT_NAME} PRIVATE ${Stb_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${MINIAUDIO_INCLUDE_DIRS})


# TODO: Add tests and install targets if needed.
