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

void calculateMBR(RTreeNode *node)
{
  Rectangle mbr;

  if (node->level == 0)
  { // Leaf node
    if (node->numObjects == 0)
    { // Empty leaf node
      mbr.xmin = 0;
      mbr.ymin = 0;
      mbr.xmax = 0;
      mbr.ymax = 0;
    }
    else
    {
      // Initialize MBR to the first object
      mbr.xmin = node->objects[0].x;
      mbr.xmax = node->objects[0].x;

      // Expand MBR to include all objects in the node
      for (int i = 1; i < node->numObjects; i++)
      {
        if (node->objects[i].x <= mbr.xmin)
        {
          mbr.xmin = node->objects[i].x;
        }
        if (node->objects[i].y <= mbr.ymin)
        {
          mbr.ymin = node->objects[i].y;
        }
        if (node->objects[i].x > mbr.xmax)
        {
          mbr.xmax = node->objects[i].x;
        }
        if (node->objects[i].y > mbr.ymax)
        {
          mbr.ymax = node->objects[i].y;
        }
      }
    }
  }
  else
  { // Internal node
    // Initialize MBR to the first child
    mbr = node->child_pointer[0]->mbr;

    // Expand MBR to include all child nodes
    for (int i = 1; i < node->numChildren; i++)
    {
      if (node->child_pointer[i]->mbr.xmin < mbr.xmin)
      {
        mbr.xmin = node->child_pointer[i]->mbr.xmin;
      }
      if (node->child_pointer[i]->mbr.ymin < mbr.ymin)
      {
        mbr.ymin = node->child_pointer[i]->mbr.ymin;
      }
      if (node->child_pointer[i]->mbr.xmax > mbr.xmax)
      {
        mbr.xmax = node->child_pointer[i]->mbr.xmax;
      }
      if (node->child_pointer[i]->mbr.ymax > mbr.ymax)
      {
        mbr.ymax = node->child_pointer[i]->mbr.ymax;
      }
    }
  }

  // Update MBR of the node
  node->mbr = mbr;
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


void insertPoints(RTreeNode *N, DataPoint CN[MAX_CHILDREN], int counter)
{

  for (int i = 0; i < counter; i++)
  {
    N->objects[i] = CN[i];
    N->numObjects++;
  }
}

void insertPointers(RTreeNode *N, RTreeNode *CN[MAX_CHILDREN], int counter)
{

  for (int i = 0; i < counter; i++)
  {
    N->child_pointer[i] = CN[i];
    N->numChildren++;
  }
}

void splitLeaf(RTreeNode *N)
{

  int x_cen = (N->mbr.xmin + N->mbr.xmax) / 2;
  int y_cen = (N->mbr.ymin + N->mbr.ymax) / 2;

  // Splitting the mbr's in the four corners
  DataPoint C0[MAX_CHILDREN];
  DataPoint C1[MAX_CHILDREN];
  DataPoint C2[MAX_CHILDREN];
  DataPoint C3[MAX_CHILDREN];

  // Creating two new nodes
  RTreeNode *N1 = createNewNode();
  RTreeNode *N2 = createNewNode();

  int c0_counter, c1_counter, c2_counter, c3_counter;

  for (int i = 0; i < N->numObjects; i++)
  {
    Rectangle r = N->child_pointer[i]->mbr;
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
  if (c1_counter > c3_counter)
  {
    insertPoints(N2, C1, c1_counter);
    insertPoints(N1, C3, c3_counter);
  }
  else
  {
    if (c3_counter > c1_counter)
    {
      insertPoints(N2, C3, c3_counter);
      insertPoints(N1, C1, c1_counter);
    }

    else
    {
      // calculate total area of C1 points and C3 points and move least area ones , we dont need overlap here as the MBR area of data points is 0
      int totalAreaC1 = calculatePointArea(C1, c1_counter);
      int totalAreaC3 = calculatePointArea(C3, c3_counter);
      if (totalAreaC1 < totalAreaC3)
      {
        insertPoints(N2, C3, c3_counter);
        insertPoints(N1, C1, c1_counter);
      }
      else
      {
        insertPoints(N2, C1, c1_counter);
        insertPoints(N1, C3, c3_counter);
      }
    }
  }
}

void splitNode(RTreeNode *N)
{

  int x_cen = (N->mbr.xmin + N->mbr.xmax) / 2;
  int y_cen = (N->mbr.ymin + N->mbr.ymax) / 2;

  // Splitting the mbr's in the four corners
  RTreeNode *C0[MAX_CHILDREN];
  RTreeNode *C1[MAX_CHILDREN];
  RTreeNode *C2[MAX_CHILDREN];
  RTreeNode *C3[MAX_CHILDREN];

  // Creating two new nodes
  RTreeNode *N1 = createNewNode();
  RTreeNode *N2 = createNewNode();

  int c0_counter, c1_counter, c2_counter, c3_counter;

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
          totalOverlapC1 += calculateOverlap(C1[i]->mbr, N1->child_pointer[i]->mbr);
        }
      }

      // calculate total overlap between all C3 mbrs and mbrs of N1 children
      int totalOverlapC3 = 0;
      for (int i = 0; i < c3_counter; i++)
      {
        for (int j = 0; j < N1->numChildren; j++)
        {
          totalOverlapC3 += calculateOverlap(C3[i]->mbr, N1->child_pointer[i]->mbr);
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
      // if tie break still exists choose C1 or C3 with lowest area
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
}

void AdjustTree(RTreeNode *N, RTreeNode *N1, RTreeNode *N2)
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
    else
    {
      printf("Node not found \n");
    }
  }
  N->parent->numChildren++;
  // include the new nodes in the child pointers of the parent
  for (int j = N->parent->numChildren - 1; j > index; j--)
  {
    N->parent->child_pointer[j + 1] = N->parent->child_pointer[j];
  }

  // set the new child pointers in the parent node
  N->parent->child_pointer[index] = N1;
  N1->parent = N->parent;
  N->parent->child_pointer[index + 1] = N2;
  N2->parent = N->parent;
  N->parent = NULL;

  calculateMBR(N1);
  calculateMBR(N2);
  calculateMBR(N1->parent);

  if (N1->parent->numChildren > MAX_CHILDREN)
  {
    splitNode(N1->parent);
  }
}

