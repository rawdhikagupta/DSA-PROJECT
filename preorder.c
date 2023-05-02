#include "dsa_project.h"

void checkSubTree(struct RTreeNode *node) {

    //check if the node lies in left subtree or right subtree
    int xcenter, parent_xcenter = 0;

    if (node->numChildren > 0)
    {
        int xcenter = (node->mbr.xmin + node->mbr.xmax) / 2;
        int parent_xcenter = (node->parent->mbr.xmin + node->parent->mbr.xmax) / 2;

            if (parent_xcenter > xcenter) // Node is in left subtree
            {
                for (int i = 0; i < node->numChildren; i++)
                {
                    preorderRTree(node->child_pointer[i]);
                }
            }
            else // Node is in right subtree
                {
                    for (int i = 0; i < node->numChildren; i++)
                {
                    preorderRTree(node->child_pointer[i]);
                }
                }
}

void preOrderTraversal(struct RTreeNode *node) {

//check if current node is NULL
    if (node == NULL)
        {
            return;
        }

//print root node
    if(node->parent == NULL) 
    {
        printf("RootNode - MBR: {(%d, %d), (%d, %d)}\n", node->mbr.xmin, node->mbr.ymin, node->mbr.xmax, node->mbr.ymax);
    }
    else if (node->numChildren == 0)
    { // Leaf node
        printf("Leaf Node - Objects: ");
        for (int i = 0; i < node->numObjects; i++)
            {
                printf("(%d, %d) ", node->objects[i].x, node->objects[i].y);
            }
    printf("\n");
    }
    else if
    { // Internal node
        if(node->parent!=NULL)
    {
            printf("\nInternal Node - MBR: {(%d, %d), (%d, %d)}\n", node->mbr.xmin, node->mbr.ymin, node->mbr.xmax, node->mbr.ymax);
            for (int i = 0; i < node->numChildren; i++)
        {
            printRTree(node->child_pointer[i]);
        }
   }


}
}




