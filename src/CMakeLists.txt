SET( SOURCES
main.cpp
core/engineinterface.cpp
core/guienvironment.cpp
core/opengl2driver.cpp
core/sdlwindow.cpp
map/base_map.cpp
map/basic_face.cpp
map/map_face.cpp
map/map_texture.cpp
map/map_vertex.cpp
map/loaders/quake3_bsp_map.cpp
map/renderers/basic_map_renderer.cpp
map/renderers/bsp_map_renderer.cpp
resources/devil_texture.cpp
resources/resource_manager.cpp
scene/camera.cpp
scene/entity.cpp
scene/frustum.cpp
scene/quake3scenenode.cpp
scene/scenemanager.cpp
scene/scenenode.cpp
scene/skybox.cpp
utilities/logger.cpp
)

SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# REQUIRED does not work in CMake <=2.4.6 for SDL
Find_Package ( SDL REQUIRED )

# Workaround for the non-working REQUIRED flag
if ( NOT SDL_FOUND )
   message ( FATAL_ERROR "SDL not found!" )
endif ( NOT SDL_FOUND )

FIND_PACKAGE(OpenGL REQUIRED)
INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})

#FIND_PACKAGE(GLEW REQUIRED)
#INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIR})

FIND_PACKAGE(Boost REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

FIND_PACKAGE(Devil REQUIRED)
INCLUDE_DIRECTORIES(${Devil_INCLUDE_DIR})

FIND_PACKAGE(PhysFS REQUIRED)
INCLUDE_DIRECTORIES(${PHYSFS_INCLUDE_DIR})

link_libraries (
	${SDL_LIBRARY}
#	${GLEW_LIBRARY}
	${OPENGL_LIBRARY}
	${Boost_LIBRARY_DIRS}
	${Boost_LIBRARY_DIR}/libboost_thread.so
	${Devil_LIBRARIES}
	${PHYSFS_LIBRARY}
	libKazmath.so
	libKazmathxx.so
	SDLmain # Sadly not included in SDL_LIBRARY variable
)

INCLUDE_DIRECTORIES( . ${OPENGL_INCLUDE_DIR} )

ADD_EXECUTABLE(kazengine ${SOURCES})
