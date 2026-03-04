set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")

if(MSVC)
    add_compile_options(/wd5050)
endif()

set(PROJECT_NAME 
	"asteroids"
)

project(${PROJECT_NAME})

set(SOURCES
	"src/main.cpp"
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE gumlib)

# define modules
target_sources(${PROJECT_NAME}
    PRIVATE FILE_SET CXX_MODULES
        BASE_DIRS src
        FILES 
            src/asteroid.ixx
            src/game.ixx
            src/asteroid.ixx
)
