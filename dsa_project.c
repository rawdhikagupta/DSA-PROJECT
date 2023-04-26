#include <stdio.h>
#include <stdlib.h>
#define MAX_CHILDREN 4

//`struct Rectangle:` to store The minimum bound rectangle 

typedef struct Rectangle {
int xmin; // minimum x-coordinate
int ymin; // minimum y-coordinate
int xmax; // maximum x-coordinate
int ymax; // maximum y-coordinate
}Rectangle;

//`struct DataPoint:`

typedef struct DataPoint {
int x; // x-coordinate of the data point
int y; // y-coordinate of the data point
}DataPoint;

//`struct RTreeNode`: A structure to store a node of the R-tree, which contains either objects or other nodes.In our case, an object is basically a two dimensional data point. 

struct RTreeNode {
int numChildren; // number of child nodes or objects
int level; // level of the node in the tree (leaf nodes have level 0)
struct RTreeNode* child_pointer[MAX_CHILDREN]; // array of child pointers for internal nodes, null for leaf nodes 
struct DataPoint objects[MAX_CHILDREN]; // array of objects for leaf nodes
int tuple_identifier; // each node has a unique string tuple_identifier
Rectangle mbr; // minimum bounding rectangle that bounds all objects in the node
struct RTreeNode* parent; // pointer to the parent node
};

//`struct RootNode`: A structure to store the root node of the R-tree, which contains all the objects in the tree.

struct RootNode {
int numChildren; // number of child nodes
struct RTreeNode * child_pointer[MAX_CHILDREN]; // array of child pointers for internal nodes
struct DataPoint objects[MAX_CHILDREN]; // array of objects for leaf nodes
int tuple_identifier; // assuming we’re using something like N1, NN, LL as a tuple_identifier
Rectangle mbr; // minimum bounding rectangle that bounds all objects in the tree
};

//so basically the root node has the minimum bounded rectangle and address of the child nodes, so it has to have a rectangle, a pointer array to a rtree nodes (pointers to all its children saved in an array). 

//`struct RTree:` 

typedef struct RTree {
int numNodes; //Number of total nodes in the tree 
int height;   //The height of the tree 
struct RootNode * root;
}RTree;

RTree * createRTree() {
  RTree * tree = (RTree *) malloc(sizeof(RTree));
  if (tree == NULL) {
    // malloc fails to allocate memory
    printf("malloc(sizeof(RTree)) is the problem");
    return NULL;
  }
  tree->numNodes = 0; 
  tree->height = 0; 

  // initialize the root node
  struct RootNode * root = (struct RootNode*) malloc(sizeof(struct RootNode));
  if (root == NULL) {
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
  for (int i = 0; i < MAX_CHILDREN; i++) {
    root->child_pointer[i] = NULL;
    root->objects[i].x = 0;
    root->objects[i].y = 0;
  }
  root->tuple_identifier = 0;
  tree->root = root;

  return tree;
}
int main()
{
  //Reading data from the data.txt and storing all the points in an array called points
    FILE *fp;
    fp = fopen("data.txt", "r");
    if(fp == NULL)
    {
        printf("Error opening file");
        exit(1);
    }

    int count  = 0;
    int i=0; 
    int x, y; 

    while (fscanf(fp, "%d %d", &x, &y) == 2) {
    count++;
  }

  DataPoint * points = (DataPoint *)malloc(count*sizeof(struct DataPoint));

  fclose(fp);

 FILE *fp2 = fopen("data.txt", "r");

  for(int i = 0 ;i<count ; i++){
      fscanf(fp, "%d %d", &points[i].x, &points[i].y);
  }

    //Printing the points to check 
    for(int j=0; j<count;j++)
    {
      printf("x:%d,", points[j].x); 
      printf("y:%d\n", points[j].y);
    }
    
    RTree * tree; 
    tree = createRTree(); 
    
    printf("Project 4"); 

}
