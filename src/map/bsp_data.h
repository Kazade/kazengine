#ifndef BSP_DATA_H_INCLUDED
#define BSP_DATA_H_INCLUDED

#include "bsp_node.h"
#include "bsp_leaf.h"
#include "bsp_cluster.h"

struct bsp_data {
    bsp_cluster clusters;

	vector<bsp_node> nodes;
	vector<bsp_leaf> leaves;
	vector<int> leaf_faces;
	vector<int> leaf_brushes;
};

#endif // BSP_DATA_H_INCLUDED
