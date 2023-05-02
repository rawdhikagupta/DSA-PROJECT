#include "dsa_project.h"

void search(RTreeNode* node, DataPoint D)
{
    if (node->isLeaf)
    {
        // Check each entry in the leaf node
        for (int i = 0; i < node->numObjects; i++)
        {
            // Check for overlap
            if (calculateOverlap(node->objects[i].mbr, D.mbr) > 0)
            {
                printf("Object area: %d\n", calculatePointArea(node->objects[i].points, node->objects[i].numPoints));
            }
        }
    }
    else
    {
        // Check each entry in the non-leaf node
        for (int i = 0; i < node->numChildren; i++)
        {
            // Check for overlap
            if (calculateOverlap(node->child_pointer[i]->mbr, D.mbr) > 0)
            {
                // Recurse on the child node
                search(node->child_pointer[i], D);
            }
        }
    }
}

