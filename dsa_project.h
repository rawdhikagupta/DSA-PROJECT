#ifndef DSA_PROJECT_H
#define DSA_PROJECT_H

#define MAX_CHILDREN 4
#define MAX_OBJECTS 4

//`struct Rectangle:` to store The minimum bound rectangle 

typedef struct Rectangle {
int xmin; // minimum x-coordinate
int ymin; // minimum y-coordinate
int xmax; // maximum x-coordinate
int ymax; // maximum y-coordinate
} Rectangle;

//`struct DataPoint:`

typedef struct DataPoint {
int x; // x-coordinate of the data point
int y; // y-coordinate of the data point
} DataPoint;

//`struct RTreeNode`: A structure to store a node of the R-tree, which contains either objects or other nodes.In our case, an object is basically a two dimensional data point. 

typedef struct RTreeNode {
int numChildren; // number of child nodes or objects
int numObjects;
int level; // level of the node in the tree (leaf nodes have level 0)
struct RTreeNode* child_pointer[MAX_CHILDREN]; // array of child pointers for internal nodes, null for leaf nodes 
struct DataPoint objects[MAX_CHILDREN]; // array of objects for leaf nodes
int tuple_identifier; // each node has a unique string tuple_identifier
Rectangle mbr; // minimum bounding rectangle that bounds all objects in the node
struct RTreeNode* parent; // pointer to the parent node
} RTreeNode;

//`struct RootNode`: A structure to store the root node of the R-tree, which contains all the objects in the tree.

/*
struct RootNode {
int numChildren; // number of child nodes
struct RTreeNode * child_pointer[MAX_CHILDREN]; // array of child pointers for internal nodes
struct DataPoint objects[MAX_CHILDREN]; // array of objects for leaf nodes
int tuple_identifier; // assuming we’re using something like N1, NN, LL as a tuple_identifier
Rectangle mbr; // minimum bounding rectangle that bounds all objects in the tree
};
*/

//so basically the root node has the minimum bounded rectangle and address of the child nodes, so it has to have a rectangle, a pointer array to a rtree nodes (pointers to all its children saved in an array). 

//`struct RTree:` 

typedef struct RTree {
int numNodes; //Number of total nodes in the tree 
int height;   //The height of the tree 
struct RTreeNode * root;
} RTree;

int area(Rectangle R);

#endif