#include "dsa_project.h"

void search(RTreeNode *node, DataPoint D) {

    if (node->isLeaf) { // S2 [Search leaf node]

        for (int i = 0; i < node->numObjects; i++) {
            if (calculateOverlap(node->objects[i].mbr, D.mbr)) {
                printf("Area: %d\n", calculatePointArea(node->objects[i].points, node->objects[i].num_points));
            }
        }

    } else { // S1 [Search Subtree]

        for (int i = 0; i < node->numChildren; i++) {
            if (calculateOverlap(node->child_pointer[i]->mbr, D.mbr)) {
                search(node->child_pointer[i], D);
            }
        }

    }
}
