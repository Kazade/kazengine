#include <boost/test/unit_test.hpp>

using boost::unit_test_framework::test_suite;
using boost::unit_test_framework::test_case;

class scene_manager_test {
public:
	scene_manager_test() {

	}

	void test_add_scene_node() {

	}

	void test_add_scene_node() {

	}

	
private:
	shared_ptr<scene_manager> m_scene_manager;
};

class scene_manager_test_suite : public test_suite {
public:
	scene_manager_test_suite():
		test_suite("scene_manager_test_suite") {
		
		//Must use boost's shared_ptr here
		boost::shared_ptr<scene_manager_test> instance(new scene_manager_test());
		test_case* add_scene_node_test_case = BOOST_CLASS_TEST_CASE(&scene_manager_test::test_add_scene_node, instance);
		add(add_scene_node_test_case);
	}
};
