#include "dsa_project.h"
#include <stdio.h>

int max(int a, int b)
{
  if (a > b)
  {
    return a;
  }
  else
  {
    return b;
  }
}

int min(int a, int b)
{
  if (a < b)
  {
    return a;
  }
  else
  {
    return b;
  }
}

int area(Rectangle r)
{
  return (r.xmax - r.xmin) * (r.ymax - r.ymin);
}

int calculateOverlap(Rectangle r1, Rectangle r2)
{
  int overlap = 0;
  // Calculate the x-axis overlap
  int x_overlap = min(r1.xmax, r2.xmax) - max(r1.xmin, r2.xmin);
  // Calculate the y-axis overlap
  int y_overlap = min(r1.ymax, r2.ymax) - max(r1.ymin, r2.ymin);
  // If there is overlap, calculate the total overlap
  if (x_overlap > 0 && y_overlap > 0)
  {
    overlap += 1;
    overlap = x_overlap * y_overlap;
  }
  return overlap;
}

int calculatePointArea(DataPoint *points, int num_points)
{
  // Initialize the minimum and maximum coordinates to the first point in the set
  float min_x = points[0].x;
  float min_y = points[0].y;
  float max_x = points[0].x;
  float max_y = points[0].y;

  // Iterate over the remaining points and update the minimum and maximum coordinates
  for (int i = 1; i < num_points; i++)
  {
    DataPoint p = points[i];
    if (p.x < min_x)
    {
      min_x = p.x;
    }
    else if (p.x > max_x)
    {
      max_x = p.x;
    }
    if (p.y < min_y)
    {
      min_y = p.y;
    }
    else if (p.y > max_y)
    {
      max_y = p.y;
    }
  }

  // Create a rectangle structure with the minimum and maximum coordinates
  Rectangle rect;
  rect.xmin = min_x;
  rect.ymin = min_y;
  rect.xmax = max_x;
  rect.ymax = max_y;
  return area(rect);
}


void insertPoints(RTreeNode *N, DataPoint CN[], int counter)
{

  for (int i = 0; i < counter; i++)
  {
    N->objects[N->numObjects++] = CN[i];
  }
}

void insertPointers(RTreeNode *N, RTreeNode *CN[], int counter)
{

  for (int i = 0; i < counter; i++)
  {
    N->child_pointer[N->numChildren++] = CN[i];
  }
}

void splitLeaf(RTreeNode *N, RTree *T)
{

  int x_cen = (N->mbr.xmin + N->mbr.xmax) / 2;
  int y_cen = (N->mbr.ymin + N->mbr.ymax) / 2;

  // Splitting the mbr's in the four corners
  DataPoint C0[MAX_CHILDREN];
  DataPoint C1[MAX_CHILDREN];
  DataPoint C2[MAX_CHILDREN];
  DataPoint C3[MAX_CHILDREN];

  // Creating two new nodes
  T->numNodes--;
  RTreeNode *N1 = createNewNode(T);
  RTreeNode *N2 = createNewNode(T);
  

  int c0_counter = 0;
  int c1_counter = 0;
  int c2_counter = 0;
  int c3_counter = 0;

  for (int i = 0; i < N->numObjects; i++)
  {

    int x_obj = N->objects[i].x;
    int y_obj = N->objects[i].y;

    if (x_obj > x_cen)
    {
      if (y_obj > y_cen)
      {
        C2[c2_counter] = N->objects[i];
        c2_counter++;
      }
      else
      {
        C3[c3_counter] = N->objects[i];
        c3_counter++;
      }
    }
    else
    {
      if (y_obj > y_cen)
      {
        C1[c1_counter] = N->objects[i];
        c1_counter++;
      }
      else
      {
        C0[c0_counter] = N->objects[i];
        c0_counter++;
      }
    }
  }
  // Distributing the child pointers according to the corners’ counters
  if (c0_counter > c2_counter)
  {
    insertPoints(N1, C0, c0_counter);
    insertPoints(N2, C2, c2_counter);
  }
  else
  {
    insertPoints(N1, C2, c2_counter);
    insertPoints(N2, C0, c0_counter);
  }
  // arbitrarily distribute points for leaf
  if (c1_counter >= c3_counter)
  {
    insertPoints(N2, C1, c1_counter);
    insertPoints(N1, C3, c3_counter);
  }
  else
  {
      insertPoints(N2, C3, c3_counter);
      insertPoints(N1, C1, c1_counter);
  }

  AdjustTree(N, N1, N2, T);
}

void splitNode(RTreeNode *N, RTree *T)
{

  int x_cen = (N->mbr.xmin + N->mbr.xmax) / 2;
  int y_cen = (N->mbr.ymin + N->mbr.ymax) / 2;

  // Splitting the mbr's in the four corners
  RTreeNode *C0[MAX_CHILDREN];
  RTreeNode *C1[MAX_CHILDREN];
  RTreeNode *C2[MAX_CHILDREN];
  RTreeNode *C3[MAX_CHILDREN];

  // Creating two new nodes
   T->numNodes --;
  RTreeNode *N1 = createNewNode(T);
  RTreeNode *N2 = createNewNode(T);
 

  int c0_counter = 0;
  int c1_counter = 0;
  int c2_counter = 0;
  int c3_counter = 0;

  for (int i = 0; i < N->numChildren; i++)
  {
    Rectangle r = N->child_pointer[i]->mbr;
    int x_obj = (r.xmax + r.xmin) / 2;
    int y_obj = (r.ymax + r.ymin) / 2;

    if (x_obj > x_cen)
    {
      if (y_obj > y_cen)
      {
        C2[c2_counter] = N->child_pointer[i];
        c2_counter++;
      }
      else
      {
        C3[c3_counter] = N->child_pointer[i];
        c3_counter++;
      }
    }
    else
    {
      if (y_obj > y_cen)
      {
        C1[c1_counter] = N->child_pointer[i];
        c1_counter++;
      }
      else
      {
        C0[c0_counter] = N->child_pointer[i];
        c0_counter++;
      }
    }
  }
  // Distributing the child pointers according to the corners’ counters
  if (c0_counter > c2_counter)
  {
    insertPointers(N1, C0, c0_counter);
    insertPointers(N2, C2, c2_counter);
  }
  else
  {
    insertPointers(N2, C0, c0_counter);
    insertPointers(N1, C2, c2_counter);
  }
  if (c1_counter > c3_counter)
  {
    insertPointers(N2, C1, c1_counter);
    insertPointers(N1, C3, c3_counter);
  }
  else
  {
    if (c3_counter > c1_counter)
    {

      insertPointers(N1, C1, c1_counter);
      insertPointers(N2, C3, c3_counter);
    }

    else
    {
      // calculate total overlap between all C1 mbrs and mbrs of N1 children
      int totalOverlapC1 = 0;
      for (int i = 0; i < c1_counter; i++)
      {
        for (int j = 0; j < N1->numChildren; j++)
        {
          totalOverlapC1 += calculateOverlap(C1[i]->mbr, N1->child_pointer[j]->mbr);
        }
      }

      // calculate total overlap between all C3 mbrs and mbrs of N1 children
      int totalOverlapC3 = 0;
      for (int i = 0; i < c3_counter; i++)
      {
        for (int j = 0; j < N1->numChildren; j++)
        {
          totalOverlapC3 += calculateOverlap(C3[i]->mbr, N1->child_pointer[j]->mbr);
        }
      }

      // Choose group of objects with least total overlap
      if (totalOverlapC1 < totalOverlapC3)
      {
        insertPointers(N1, C1, c1_counter);
        insertPointers(N2, C3, c3_counter);
      }
      if (totalOverlapC3 < totalOverlapC1)
      {
        insertPointers(N2, C1, c1_counter);
        insertPointers(N1, C3, c3_counter);
      }

      else
      {
        int totalAreaC1 = 0.0;
        for (int i = 0; i < c1_counter; i++)
        {
          totalAreaC1 += area(C1[i]->mbr);
        }

        int totalAreaC3 = 0.0;
        for (int i = 0; i < c3_counter; i++)
        {
          totalAreaC3 += area(C3[i]->mbr);
        }

        if (totalAreaC1 < totalAreaC3)
        {
          insertPointers(N1, C1, c1_counter);
          insertPointers(N2, C3, c3_counter);
        }
        else
        {
          insertPointers(N2, C1, c1_counter);
          insertPointers(N1, C3, c3_counter);
        }
      }
    }
  }
  AdjustTree(N, N1, N2, T);
}
