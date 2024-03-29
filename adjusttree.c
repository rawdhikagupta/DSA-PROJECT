#include "dsa_project.h"
void calculateMBR(RTreeNode *node)
{
  if (node == NULL) {
    return;
  }

  Rectangle mbr;

  if (node->numChildren == 0) { // Leaf node
    if (node->numObjects == 0) { // Empty leaf node
      mbr.xmin = INT_MAX;
      mbr.ymin = INT_MAX;
      mbr.xmax = INT_MIN;
      mbr.ymax = INT_MIN;
    } else {
      // Initialize MBR to the first object
      mbr.xmin = node->objects[0].x;
      mbr.xmax = node->objects[0].x;
      mbr.ymin = node->objects[0].y;
      mbr.ymax = node->objects[0].y;

      // Expand MBR to include all objects in the node
      for (int i = 1; i < node->numObjects; i++) {
        if (node->objects[i].x <= mbr.xmin) {
          mbr.xmin = node->objects[i].x;
        }
        if (node->objects[i].y <= mbr.ymin) {
          mbr.ymin = node->objects[i].y;
        }
        if (node->objects[i].x > mbr.xmax) {
          mbr.xmax = node->objects[i].x;
        }
        if (node->objects[i].y > mbr.ymax) {
          mbr.ymax = node->objects[i].y;
        }
      }
    }
  } else { // Internal node
    // Initialize MBR to the first child
    mbr = node->child_pointer[0]->mbr;

    // Expand MBR to include all child nodes
    for (int i = 0; i < node->numChildren; i++) {
      if (node->child_pointer[i]->mbr.xmin < mbr.xmin) {
        mbr.xmin = node->child_pointer[i]->mbr.xmin;
      }
      if (node->child_pointer[i]->mbr.ymin < mbr.ymin) {
        mbr.ymin = node->child_pointer[i]->mbr.ymin;
      }
      if (node->child_pointer[i]->mbr.xmax > mbr.xmax) {
        mbr.xmax = node->child_pointer[i]->mbr.xmax;
      }
      if (node->child_pointer[i]->mbr.ymax > mbr.ymax) {
        mbr.ymax = node->child_pointer[i]->mbr.ymax;
      }
    }
  }

  // Update MBR of the node
  node->mbr = mbr;
}
void adjustMBRs(RTreeNode *node) {
    RTreeNode *current = node;
    while (current != NULL) {
        calculateMBR(current);
        current = current->parent;
    }
}
void AdjustTree(RTreeNode *N, RTreeNode *N1, RTreeNode *N2, RTree *T)
{
  if (N->parent != NULL)
  {
    int index;
    // for loop to find the index of current node that is being split
    for (int i = 0; i < N->parent->numChildren; i++)
    {

      if (N->parent->child_pointer[i]->tuple_identifier == N->tuple_identifier)
      {
        index = i;
        break;
      }
    }
    N->parent->numChildren++;
    // include the new nodes in the child pointers of the parent
    for (int j = N->parent->numChildren-1; j > index; j--)
    {
      N->parent->child_pointer[j+1] = N->parent->child_pointer[j];
    }

    // set the new child pointers in the parent node
    N->parent->child_pointer[index] = N1;
    N1->parent = N->parent;
    N->parent->child_pointer[index + 1] = N2;
    N2->parent = N->parent;
    N->parent = NULL;
    N = NULL;
    
    RTreeNode *NN = N1->parent;
    adjustMBRs(N1);
    adjustMBRs(N2);

    // Update MBR of node to incorporate the new DataPoint
    if (N2->parent->numChildren > MAX_CHILDREN)
    {
      splitNode(N2->parent, T);
    }
  }

  else
  {
    RTreeNode *newroot = createNewNode(T);
    newroot->child_pointer[0] = N;
    newroot->numChildren++;
    calculateMBR(newroot); 
    N->parent = newroot;
    T->root = newroot;
    AdjustTree(N,N1,N2,T);
    
  }
}