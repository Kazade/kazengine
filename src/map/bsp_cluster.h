#ifndef BSP_CLUSTER_H_INCLUDED
#define BSP_CLUSTER_H_INCLUDED

struct bsp_cluster {
    int totalClusters;
    int size;
    vector<unsigned char> bitSet;
};

#endif // BSP_CLUSTER_H_INCLUDED
