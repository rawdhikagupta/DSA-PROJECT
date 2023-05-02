#include "dsa_project.h"

void search(struct RTree *node, DataPoint D)
{
    // If node is not a leaf, check each entry E to see if E.I overlaps D
    if (node->numChildren > 0)
    {
        for (int i = 0; i < node->numChildren; i++)
        {
            if (calculateOverlap(node->child_pointer[i]->mbr, D.rect) > 0)
            {
                // If there is overlap, start a recursive search on the child node
                search(node->child_pointer[i], D);
            }
        }
    }
    else
    {
        // If node is a leaf, check each entry E to see if E.I overlaps D
        for (int i = 0; i < node->numObjects; i++)
        {
            if (calculateOverlap(node->objects[i].rect, D.rect) > 0)
            {
                // If there is overlap, print the area of the DataPoint
                printf("Area: %d\n", calculatePointArea(&D, 1));
            }
        }
    }
}
