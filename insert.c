#include "dsa_project.h"
#include <stdlib.h>

RTreeNode *chooseLeaf(DataPoint D, RTreeNode *R)
{

  // If R is a leaf then return N
  if (R->numChildren == 0)
    return R;

  // Else, choose which subtree to follow
  RTreeNode *F;
  int minExpansion[MAX_CHILDREN];

  for (int i = 0; i < R->numChildren; i++)
  {
    Rectangle ogRect = R->mbr;
    Rectangle expandedRect = ogRect;

    if (D.x < ogRect.xmin)
      expandedRect.xmin = D.x;
    if (D.y < ogRect.ymin)
      expandedRect.ymin = D.y;
    if (D.x > ogRect.xmax)
      expandedRect.xmax = D.x;
    if (D.y > ogRect.ymax)
      expandedRect.ymax = D.y;

    minExpansion[i] = area(expandedRect) - area(ogRect);
  }

  // Choose the least out of all expansions
  int least = minExpansion[0];
  F = R->child_pointer[0];

  for (int i = 1; i < R->numChildren; i++)
  {
    if (minExpansion[i] < least)
    {
      least = minExpansion[i];
      F = R->child_pointer[i];
    }

    if (minExpansion[i] == least)
    {
      if (area(F->mbr) > area(R->child_pointer[i]->mbr))
        F = R->child_pointer[i];
    }
  }

  // After F is chosen
  return chooseLeaf(D, F);
}

int insertDataPoint(DataPoint D, RTree *T)
{

  // Call chooseLeaf to find the F in which D can be placed
  RTreeNode *leaf = chooseLeaf(D, T->root);

  // Update MBR of node to incorporate the new DataPoint
  leaf->objects[leaf->numObjects] = D;
  leaf->numObjects++;
  adjustMBRs(leaf);

  // If leaf node has space -> place D
  if (leaf->numObjects < MAX_OBJECTS)
  {
    return 1; // Successful insertion
  }

  // Else call splitNode
  splitLeaf(leaf, T);
  // Adjust tree to reflect the changes in upper levels

  // If the adjustment process leads to root F split, then create a new root F

  return 1; // Successful insertion
}