//This function takes a pointer to a R-tree node as input and recursively traverses through the tree, printing the MBR values of each internal node
#include "dsa_project.h"
void printRTree(struct RTreeNode *node)
{
  if (node == NULL)
  {
    return;
  }

  if (node->numChildren == 0)
  { // Leaf node
    printf("Leaf Node - Objects: ");
    for (int i = 0; i < node->numObjects; i++)
    {
      printf("(%d, %d) ", node->objects[i].x, node->objects[i].y);
    }
    printf("\n");
  }
  else
  { // Internal node
  if(node->parent!=NULL)
    {
      printf("\nInternal Node - MBR: {(%d, %d), (%d, %d)}\n", node->mbr.xmin, node->mbr.ymin, node->mbr.xmax, node->mbr.ymax);
      for (int i = 0; i < node->numChildren; i++)
      {
        printRTree(node->child_pointer[i]);
      }
   }
    else if(node->parent == NULL)
    {
      printf("RootNode - MBR: {(%d, %d), (%d, %d)}\n", node->mbr.xmin, node->mbr.ymin, node->mbr.xmax, node->mbr.ymax);
    for (int i = 0; i < node->numChildren; i++)
    {
      printRTree(node->child_pointer[i]);
    }
    }
  }
}