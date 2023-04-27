#include "dsa_project.h"
#include <stdlib.h>

RTreeNode* chooseLeaf(DataPoint D, RTreeNode* R) {

    // If R is a leaf then return N
    if (R->numChildren == 0) return R; 

    // Else, choose which subtree to follow
    RTreeNode* F;
    int minExpansion[MAX_CHILDREN];

    for (int i = 0; i < MAX_CHILDREN; i++) {
        Rectangle ogRect = R->mbr;
        Rectangle expandedRect = ogRect;

        if (D.x < ogRect.xmin) expandedRect.xmin = D.x;
        if (D.y < ogRect.ymin) expandedRect.ymin = D.y;
        if (D.x > ogRect.xmax) expandedRect.xmax = D.x;
        if (D.y > ogRect.ymax) expandedRect.ymax = D.y;

        minExpansion[i] = area(expandedRect) - area(ogRect);
    }

    // Choose the least out of all expansions
    int least = minExpansion[0];
    F = R->child_pointer[0];

    for (int i = 1; i < MAX_CHILDREN; i++) {
        if (minExpansion[i] < least) {
            least = minExpansion[i];
            F = R->child_pointer[i];
        }

        if (minExpansion[i] == least) {
            if (area(F->mbr) > area(R->child_pointer[i]->mbr)) F = R->child_pointer[i];
        }
    }

    // After F is chosen
    return chooseLeaf(D, F);
}

int insertDataPoint(DataPoint D, RTree* T) {

    // Call chooseLeaf to find the F in which D can be placed
    RTreeNode* leaf = chooseLeaf(D, T->root); 

    // If F has space -> place D
    if (leaf->numObjects < MAX_OBJECTS) {
        leaf->objects[leaf->numObjects] = D;
        leaf->numObjects++;

        // Update MBR of node to incorporate the new DataPoint
        if (D.x < leaf->mbr.xmin) leaf->mbr.xmin = D.x;
        if (D.y < leaf->mbr.ymin) leaf->mbr.ymin = D.y;
        if (D.x > leaf->mbr.xmax) leaf->mbr.xmax = D.x;
        if (D.y > leaf->mbr.ymax) leaf->mbr.ymax = D.y;
    }
    // Else call splitNode
    else {
        splitNode(leaf);
        insertDataPoint(D, T);
    }

    // Adjust tree to reflect the changes in upper levels

    // If the adjustment process leads to root F split, then create a new root F

    return 1; // Successful insertion
}