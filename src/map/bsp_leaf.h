#ifndef BSP_LEAF_H_INCLUDED
#define BSP_LEAF_H_INCLUDED

struct bsp_leaf {
	int cluster;
	int area;
	int mins[3];
	int maxs[3];
	int leaf_face;
	int leaf_face_count;
	int leaf_brush;
	int leaf_brush_count;
};

#endif // BSP_LEAF_H_INCLUDED
