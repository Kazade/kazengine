#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include "scene/scenemanager.h"
#include "scene/quake3scenenode.h"

BOOST_AUTO_TEST_CASE( test1 ) {
	shared_ptr<scene_manager_interface> smgr(new scene_manager);

	//Create a new quake3 map node
	scene_node_interface* quake3map = smgr->add_built_in_scene_node(SNT_QUAKE3_BSP);

	BOOST_ASSERT(quake3map);

	//Down cast to the quake3 scene node that it is
	quake3_scene_node* node = node_down_cast<quake3_scene_node>(quake3map);

	BOOST_ASSERT(node);
}

BOOST_AUTO_TEST_CASE( test2 ) {
    int i = 0;

    // reports 'error in "test2": check i == 2 failed [0 != 2]'
    BOOST_CHECK_EQUAL( i, 2 );
    BOOST_CHECK_EQUAL( i, 0 );
}

