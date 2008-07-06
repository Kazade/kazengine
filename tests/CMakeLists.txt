PROJECT(KAZENGINE_TESTS)

SET( KAZENGINE_SRCS
../src/core/engineinterface.cpp
../src/core/guienvironment.cpp
../src/core/opengl2driver.cpp
../src/core/sdlwindow.cpp

../src/scene/camera.cpp
../src/scene/entity.cpp
../src/scene/frustum.cpp
../src/scene/quake3scenenode.cpp
../src/scene/scenemanager.cpp
../src/scene/scenenode.cpp
../src/scene/skybox.cpp

../src/map/base_map.cpp
../src/map/basic_face.cpp
../src/map/load_test.cpp
../src/map/map_face.cpp
../src/map/map_texture.cpp
../src/map/map_vertex.cpp

../src/resources/resourcemanager.cpp

../src/utilities/logger.cpp
)

INCLUDE_DIRECTORIES ( ../src )
LINK_LIBRARIES( /usr/lib/libboost_unit_test_framework.so )
ADD_EXECUTABLE(scene_manager_test, scenemanager.cpp ${KAZENGINE_SRCS} )
