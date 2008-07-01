#include <iostream>
#include <fstream>

#include "map/loaders/quake3_bsp_map.h"

using std::ifstream;

int main(int argc, char** argv) {
	std::cout << argv[0] << std::endl;
	quake3_bsp_map test_map;

	ifstream file_in("./box.bsp", std::ios::binary);
	if (!file_in.is_open()) {
			std::cout << "File does not exist" << std::endl;
			return 1;
	}

	file_load_status result = test_map.load(file_in);
	if (result != FILE_LOAD_SUCCESS) {
			std::cout << test_map.get_last_error() << std::endl;
	}

	file_in.close();
	return 0;
}
