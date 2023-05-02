#include <stdio.h>
#include <stdlib.h>
#define MAX_CHILDREN 4
#include "dsa_project.h"
//`struct Rectangle:` to store The minimum bound rectangle 

RTree *createRTree()
{
  RTree *tree = (RTree *)malloc(sizeof(RTree));
  if (tree == NULL)
  {
    // malloc fails to allocate memory
    printf("malloc(sizeof(RTree)) is the problem");
    return NULL;
  }
  tree->numNodes = 0;
  tree->height = 0;

  // initialize the root node
  struct RTreeNode *root = (struct RTreeNode *)malloc(sizeof(struct RTreeNode));
  if (root == NULL)
  {
    // malloc fails to allocate memory
    printf("malloc(sizeof(RootNode)) is the problem");
    free(tree);
    return NULL;
  }
  root->numChildren = 0;
  root->mbr.xmin = 0;
  root->mbr.ymin = 0;
  root->mbr.xmax = 0;
  root->mbr.ymax = 0;
  for (int i = 0; i < MAX_CHILDREN; i++)
  {
    root->child_pointer[i] = NULL;
    root->objects[i].x = 0;
    root->objects[i].y = 0;
  }
  root->tuple_identifier = 0;
  tree->root = root;

  return tree;
}

RTreeNode *createNewNode(RTree *T)
{

  RTreeNode *newnode = (RTreeNode *)malloc(sizeof(struct RTreeNode));
  if (newnode == NULL)
  {
    // malloc fails to allocate memory
    printf("malloc(sizeof(RTreeNode)) is the problem");
    return NULL;
  }
  newnode->numChildren = 0;
  newnode->numObjects = 0;
  newnode->tuple_identifier = T->numNodes++;
  newnode->parent = NULL;
  return newnode;
}


int main()
{
  // Reading data from the data.txt and storing all the points in an array called points
  FILE *fp;
  fp = fopen("data.txt", "r");
  if (fp == NULL)
  {
    printf("Error opening file");
    exit(1);
  }

  int count = 0;
  int i = 0;
  int x, y;

  while (fscanf(fp, "%d %d", &x, &y) == 2)
  {
    count++;
  }

  DataPoint *points = (DataPoint *)malloc(count * sizeof(struct DataPoint));

  fclose(fp);

  FILE *fp2 = fopen("data.txt", "r");

  for (int i = 0; i < count; i++)
  {
    fscanf(fp2, "%d %d", &points[i].x, &points[i].y);
  }

  // Printing the points to check
  /*for (int j = 0; j < count; j++)
  {
    printf("x:%d,", points[j].x);
    printf("y:%d\n", points[j].y);
  }*/
  printf("\n");

  RTree *tree;
  tree = createRTree();
  RTreeNode *N = createNewNode(tree);
  tree->root = N;
  for (int i = 0; i < count; i++)
  {
    insertDataPoint(points[i], tree);
  }
  printf("\n");
  preOrderPrintRTree(tree->root);
  //printf("\n%d", tree->root->numChildren);
  printf("End of tree printing\n");
  //printf("Project 4");

}
