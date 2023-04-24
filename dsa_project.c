#include <stdio.h>
#include <stdlib.h>
#define MAX_CHILDREN 4

//`struct Rectangle:` to store The minimum bound rectangle 

typedef struct Rectangle {
double xmin; // minimum x-coordinatei
double ymin; // minimum y-coordinate
double xmax; // maximum x-coordinate
double ymax; // maximum y-coordinate
}Rectangle;

//`struct DataPoint:`

struct DataPoint {
double x; // x-coordinate of the data point
double y; // y-coordinate of the data point
};

//`struct RTreeNode`: A structure to store a node of the R-tree, which contains either objects or other nodes.In our case, an object is basically a two dimensional data point. 

struct RTreeNode {
int numChildren; // number of child nodes or objects
int level; // level of the node in the tree (leaf nodes have level 0)
struct RTreeNode* child_pointer[MAX_CHILDREN]; // array of child pointers for internal nodes
struct DataPoint objects[MAX_CHILDREN]; // array of objects for leaf nodes
char * tuple_identifier; // each node has a unique string tuple_identifier
Rectangle mbr; // minimum bounding rectangle that bounds all objects in the node
struct RTreeNode* parent; // pointer to the parent node
};

//`struct RootNode`: A structure to store the root node of the R-tree, which contains all the objects in the tree.

struct RootNode {
int numChildren; // number of child nodes
struct RTreeNode * child_pointer[MAX_CHILDREN]; // array of child pointers for internal nodes
struct DataPoint objects[MAX_CHILDREN]; // array of objects for leaf nodes
char * tuple_identifier; // assuming we’re using something like N1, NN, LL as a tuple_identifier
Rectangle mbr; // minimum bounding rectangle that bounds all objects in the tree
};

//so basically the root node has the minimum bounded rectangle and address of the child nodes, so it has to have a rectangle, a pointer array to a rtree nodes (pointers to all its children saved in an array). 

//`struct RTree:` 

struct RTree {
int number_of_nodes; 
int height;
struct RootNode * root;
};

int main()
{
    printf("Project 4"); 
}