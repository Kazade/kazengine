#ifndef BSP_NODE_H_INCLUDED
#define BSP_NODE_H_INCLUDED

enum bsp_node_index {
	BNI_FRONT = 0,
	BNI_BACK = 1
};

struct bsp_node {
	int m_plane_index; ///< Index to the planes in the map
	int m_children[2];
	int m_mins[3];
	int m_maxs[3];
};

#endif // BSP_NODE_H_INCLUDED
