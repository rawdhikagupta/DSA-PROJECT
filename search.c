#include "dsa_project.h"

int search(struct RTreeNode* node, Rectangle s)
{
    if (node->numChildren == 0)
    {
        // Check each entry in the leaf node
       
            // Check for overlap
            if (calculateOverlap(node->mbr, s) == 1)
            {
                return 1;
            } 
            else 
            {
                return 0;
            }
        
    }
    else
    {
        // Check each entry in the non-leaf node
        for (int i = 0; i < node->numChildren; i++)
        {
            // Check for overlap
            if (calculateOverlap(node->child_pointer[i]->mbr, s) == 1)
            {
                // Recurse on the child node
                search(node->child_pointer[i], s);
            }
        }
    }
}
