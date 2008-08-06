#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "scenemanager_test.h"

using boost::unit_test_framework::test_suite;

test_suite* init_unit_test_suite(int, char**) {
	std::auto_ptr<test_suite> top_test_suite(BOOST_TEST_SUITE("Master test suite"));

	top_test_suite->add(new scene_manager_test_suite());
	return top_test_suite.release();
}

