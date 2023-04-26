void splitNode(RTreeNode * N)
{

  int x_cen = (N->mbr.xmin + N->mbr.xmax) / 2;
  int y_cen = (N->mbr.ymin + N->mbr.ymax) / 2;

  //Splitting the mbr's in the four corners 
  RTreeNode * C0[MAX_CHILDREN];
  RTreeNode * C1[MAX_CHILDREN]; 
  RTreeNode * C2[MAX_CHILDREN]; 
  RTreeNode * C3[MAX_CHILDREN]; 

  //Creating two new nodes 
  RTreeNode * N1 = createNewNode();
  RTreeNode * N2 = createNewNode();

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
  //Distributing the child pointers according to the corners’ counters
  if(c0_counter>c2_counter)
  {
    for(int i=0; i<c0_counter;i++)
    {
      N1->child_pointer[i] = C0[i]; 
      N1->numChildren++; 
    }
  }
  else
  {
    for(int i=0; i<c2_counter;i++)
    {
      N1->child_pointer[i] = C2[i]; 
      N1->numChildren++; 
    }
  }
  if(c1_counter>c3_counter)
  {
    for(int i=0; i<c1_counter;i++)
    {
      N1->child_pointer[i] = C1[i]; 
      N1->numChildren++; 
    }
  }
  else
  {
    for(int i=0; i<c3_counter;i++)
    {
      N1->child_pointer[i] = C3[i]; 
      N1->numChildren++; 
    }
  }
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