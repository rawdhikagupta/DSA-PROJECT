#include <stdio.h>
#include <stdlib.h>

#define MAX_CHILDREN 4
#define MAX_OBJECTS 4

int identifier = 0;

typedef struct Rectangle {
    long xmin; // minimum x-coordinate
    long ymin; // minimum y-coordinate
    long xmax; // maximum x-coordinate
    long ymax; // maximum y-coordinate
} Rectangle;

typedef struct DataPoint {
    long x; // x-coordinate of the data point
    long y; // y-coordinate of the data point
} DataPoint;

typedef struct RTreeNode {
    int tuple_identifier;
    int numChildren;
    int numObjects;
    int level;
    struct RTreeNode* children[MAX_CHILDREN];
    struct DataPoint* objects[MAX_OBJECTS];
    Rectangle mbr;
    struct RTreeNode* parent;
} RTreeNode;

typedef struct RTree {
    int numNodes;
    int height;
    RTreeNode* root;
} RTree;

RTreeNode* createNewNode(RTree* tree) {

    RTreeNode* node = (RTreeNode*) malloc (sizeof(RTreeNode));

    if (node == NULL) {
        printf("Could not create a new node...\n");
        exit(1);
    }
    
    node->numChildren = 0;
    node->numObjects = 0;
    node->tuple_identifier = identifier++;
    node->parent = NULL;
    tree->numNodes++;
    
    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    for (int i = 0; i < MAX_OBJECTS; i++) node->objects[i] = NULL;
    
    node->mbr.xmax = 0;
    node->mbr.xmin = 0;
    node->mbr.ymax = 0;
    node->mbr.ymin = 0;

    // level, parent are to be identified depending on where the node is being created

    return node;
}

RTree* createRTree() {
    
    printf("Creating an R Tree...\n");

    RTree* tree = (RTree*) malloc (sizeof(RTree));

    if (tree == NULL) {
        printf("Could not create the R Tree...\n");
        exit(1);
    }

    tree->numNodes = 0;
    tree->height = 0;

    RTreeNode* root = createNewNode(tree);
    root->level = 0;
    root->parent = NULL;

    tree->root = root;

    return tree;
}

int max(int a, int b) {
    if (a > b) return a;
    else return b;
}

int min(int a, int b) {
    if (a < b) return a;
    else return b;
}

long area(Rectangle rect) {
    return (rect.xmax - rect.xmin) * (rect.ymax - rect.ymin);
}

RTreeNode* chooseLeaf(DataPoint* point, RTree* tree) {

    RTreeNode* node = tree->root;

    while (node->level != 0) {

        int index = 0;
        long minExpansion[node->numChildren];
        long least;

        for (int i = 0; i < node->numChildren; i++) {
            Rectangle originalRect = node->children[i]->mbr;
            Rectangle expandedRect = originalRect;

            if (point->x < originalRect.xmin) expandedRect.xmin = point->x;
            if (point->y < originalRect.ymin) expandedRect.ymin = point->y;
            if (point->x > originalRect.xmax) expandedRect.xmax = point->x;
            if (point->y > originalRect.ymax) expandedRect.ymax = point->y;

            minExpansion[i] = area(expandedRect) - area(originalRect);
        }

        least = minExpansion[0];

        for (int i = 1; i < node->numChildren; i++) {
            if (minExpansion[i] < least) {
                least = minExpansion[i];
                index = i;
            }
        }

        // Check for tie
        long minArea = area(node->children[index]->mbr);
        for (int i = 0; i < node->numChildren && i != index; i++) {
            if (minExpansion[i] == least && area(node->children[i]->mbr) < minArea) {
                index = i;
                minArea = area(node->children[i]->mbr);
            }
        }

        node = node->children[index];
    }
    
    return node;
}

void adjustTree(RTreeNode* node) {

    Rectangle newMBR;

    if (node->level == 0) {
        // leaf node mbr adjustment
        newMBR.xmin = node->objects[0]->x;
        newMBR.xmax = node->objects[0]->x;
        newMBR.ymin = node->objects[0]->y;
        newMBR.ymax = node->objects[0]->y;

        // Expand newMBR to include all objects in the node
        for (int i = 1; i < node->numObjects; i++) {
            if (node->objects[i]->x <= newMBR.xmin) {
                newMBR.xmin = node->objects[i]->x;
            }
            if (node->objects[i]->y <= newMBR.ymin) {
                newMBR.ymin = node->objects[i]->y;
            }
            if (node->objects[i]->x > newMBR.xmax) {
                newMBR.xmax = node->objects[i]->x;
            }
            if (node->objects[i]->y > newMBR.ymax) {
                newMBR.ymax = node->objects[i]->y;
            }
        }
    } else {
        // internal node mbr adjustment
        newMBR = node->children[0]->mbr;

        // Expand MBR to include all child nodes
        for (int i = 1; i < node->numChildren; i++) {
            if (node->children[i]->mbr.xmin < newMBR.xmin) {
                newMBR.xmin = node->children[i]->mbr.xmin;
            }
            if (node->children[i]->mbr.ymin < newMBR.ymin) {
                newMBR.ymin = node->children[i]->mbr.ymin;
            }     
            if (node->children[i]->mbr.xmax > newMBR.xmax) {
                newMBR.xmax = node->children[i]->mbr.xmax;
            }
            if (node->children[i]->mbr.ymax > newMBR.ymax) {
                newMBR.ymax = node->children[i]->mbr.ymax;
            }
        }
    }
    node->mbr = newMBR;
    if (node->parent != NULL) adjustTree(node->parent);
}

int mod(int num) {
    if (num < 0) return num * -1;
    else return num;
}

int distanceBetweenPointAndAxis(int horAxis, int split, DataPoint point) {
    if (horAxis) 
        return mod(point.y - split);
    else 
        return mod(point.x - split);
}

void insertNodes(RTreeNode* parent, RTreeNode* nodes[], int counter) {
    for (int i = 0; i < counter; i++) {
        parent->children[parent->numChildren] = nodes[i];
        nodes[i]->parent = parent;
        parent->numChildren++;
    }
}

DataPoint centre(Rectangle rect) {
    //printf("within centre\n");
    DataPoint centre;
    centre.x = (rect.xmax + rect.xmin) / 2;
    centre.y = (rect.ymax + rect.ymin) / 2;
    return centre;
}

void transferNodes(RTreeNode* full, RTreeNode* vacant, int split, int horAxis) {
    int index = 0;
    int minDistance = distanceBetweenPointAndAxis(horAxis, split, centre(full->children[0]->mbr));
    for (int i = 1; i < MAX_CHILDREN; i++) {
        int dist = distanceBetweenPointAndAxis(horAxis, split, centre(full->children[i]->mbr));
        if (dist < minDistance) {
            minDistance = dist;
            index = i;
        }
    }
    vacant->children[vacant->numChildren] = full->children[index];
    full->children[index]->parent = vacant;
    vacant->numChildren++;
    full->children[index] = NULL;
    full->numChildren--;
    // shift rest of the nodes into the null space
    for (int i = index; i < MAX_CHILDREN - 1; i++) full->children[i] = full->children[i + 1];
    full->children[MAX_CHILDREN - 1] = NULL;
}

long calculateOverlap(Rectangle r1, Rectangle r2) {
    long overlap = 0;
    // Calculate the x-axis overlap
    long x_overlap = min(r1.xmax, r2.xmax) - max(r1.xmin, r2.xmin);
    // Calculate the y-axis overlap
    long y_overlap = min(r1.ymax, r2.ymax) - max(r1.ymin, r2.ymin);
    // If there is overlap, calculate the total overlap
    if (x_overlap > 0 && y_overlap > 0)
    {
        overlap += 1;
        overlap = x_overlap * y_overlap;
    }
    return overlap;
}

// Returns one of the nodes after splitting
void splitNode(RTreeNode* node, RTreeNode* child, RTree* tree) {

    long x_cen = (node->mbr.xmin + node->mbr.xmax) / 2;
    long y_cen = (node->mbr.ymin + node->mbr.ymax) / 2;

    RTreeNode* parent = node->parent;
    tree->numNodes--;

    RTreeNode* N1 = createNewNode(tree);
    N1->parent = parent;
    N1->level = node->level;
    RTreeNode* N2 = createNewNode(tree);
    N2->parent = parent;
    N2->level = node->level;

    RTreeNode* C0[MAX_CHILDREN];
    RTreeNode* C1[MAX_CHILDREN];
    RTreeNode* C2[MAX_CHILDREN];
    RTreeNode* C3[MAX_CHILDREN];

    int c0_counter = 0;
    int c1_counter = 0;
    int c2_counter = 0;
    int c3_counter = 0;

    RTreeNode* temp[MAX_CHILDREN + 1];
    temp[0] = child;
    for (int i = 1; i < MAX_CHILDREN + 1; i++) temp[i] = node->children[i - 1];

    for (int i = 0; i < MAX_CHILDREN + 1; i++) {
        long x_obj = centre(temp[i]->mbr).x;
        long y_obj = centre(temp[i]->mbr).y;
        if (x_obj >= x_cen) {
            if (y_obj >= y_cen && c2_counter < 4) {
                C2[c2_counter] = temp[i];
                c2_counter++;
            } else if (y_obj <= y_cen && c3_counter < 4) {
                C3[c3_counter] = temp[i];
                c3_counter++;
            }
        } else {
            if (y_obj >= y_cen && c1_counter < 4) {
                C1[c1_counter] = temp[i];
                c1_counter++;
            } else if (y_obj <= y_cen && c0_counter < 4) {
                C0[c0_counter] = temp[i];
                c0_counter++;
            }
        }
    }
    // If split is along horizontal axis then 1 else 0
    int horAxis = 0;

    if (c0_counter > c2_counter) {
        insertNodes(N1, C0, c0_counter);
        insertNodes(N2, C2, c2_counter);
    } else {
        insertNodes(N2, C0, c0_counter);
        insertNodes(N1, C2, c2_counter);
    }

    if (c1_counter > c3_counter) {
        insertNodes(N2, C1, c1_counter);
        insertNodes(N1, C3, c3_counter);
        if (c0_counter > c2_counter) horAxis = 1;
        else horAxis = 0;
    } else {
        if (c1_counter < c3_counter) {
            insertNodes(N1, C1, c1_counter);
            insertNodes(N2, C3, c3_counter);
            if (c0_counter > c2_counter) horAxis = 0;
            else horAxis = 1;
        } else {
            // calculate total overlap between all C1 mbrs and mbrs of N1 children
            int totalOverlapC1 = 0;
            for (int i = 0; i < c1_counter; i++) {
                for (int j = 0; j < N1->numChildren; j++) {
                    totalOverlapC1 += calculateOverlap(C1[i]->mbr, N1->children[j]->mbr);
                }
            }

            // calculate total overlap between all C3 mbrs and mbrs of N1 children
            int totalOverlapC3 = 0;
            for (int i = 0; i < c3_counter; i++) {
                for (int j = 0; j < N1->numChildren; j++) {
                    totalOverlapC3 += calculateOverlap(C3[i]->mbr, N1->children[j]->mbr);
                }
            }

            // Choose group of objects with least total overlap
            if (totalOverlapC1 < totalOverlapC3) {
                insertNodes(N1, C1, c1_counter);
                insertNodes(N2, C3, c3_counter);
                if (c0_counter > c2_counter) horAxis = 0;
                else horAxis = 1;
            }
            
            if (totalOverlapC3 < totalOverlapC1) {
                insertNodes(N2, C1, c1_counter);
                insertNodes(N1, C3, c3_counter);
                if (c0_counter > c2_counter) horAxis = 1;
                else horAxis = 0;
            } else {
                int totalAreaC1 = 0.0;
                for (int i = 0; i < c1_counter; i++) {
                    totalAreaC1 += area(C1[i]->mbr);
                }

                int totalAreaC3 = 0.0;
                for (int i = 0; i < c3_counter; i++) {
                    totalAreaC3 += area(C3[i]->mbr);
                }

                if (totalAreaC1 < totalAreaC3) {
                    insertNodes(N1, C1, c1_counter);
                    insertNodes(N2, C3, c3_counter);
                    if (c0_counter > c2_counter) horAxis = 0;
                    else horAxis = 1;
                } else {
                    insertNodes(N2, C1, c1_counter);
                    insertNodes(N1, C3, c3_counter);
                    if (c0_counter > c2_counter) horAxis = 1;
                    else horAxis = 0;
                }
            }
        }
    }

    int split_axis = horAxis? y_cen: x_cen;

    // Check for m=2 and M=4 condition in L1
    if (N1->numChildren >= MAX_CHILDREN) transferNodes(N1, N2, split_axis, horAxis);
    else if (N2->numChildren >= MAX_CHILDREN) transferNodes(N2, N1, split_axis, horAxis);

    // If the root that was just split is a root node
    if (node->parent == NULL) {
        printf("Splitting root node %d\n", node->tuple_identifier);
        RTreeNode* newRoot = createNewNode(tree);
        newRoot->level = node->level + 1;
        newRoot->parent = NULL;
        N1->parent = newRoot;
        N2->parent = newRoot;
        newRoot->children[0] = N1;
        newRoot->children[1] = N2;
        newRoot->numChildren = 2;
        tree->root = newRoot;
        tree->height++;
        free(node);
        return;
    }

    int tuple_identifier = node->tuple_identifier;
    for (int i = 0; i < parent->numChildren; i++) {
        if (parent->children[i]->tuple_identifier == tuple_identifier) {
            parent->children[i] = N1;
            break;
        } 
    }

    free(node);

    if (parent->numChildren == MAX_CHILDREN) {
        splitNode(parent, N2, tree);
        return;
    }
    
    parent->children[parent->numChildren] = N2;
    parent->numChildren++;
}

void insertPoints(RTreeNode* leaf, DataPoint* points[], int counter) {
    for (int i = 0; i < counter; i++) {
        leaf->objects[leaf->numObjects] = points[i];
        leaf->numObjects++;
    }
}

void transferPoints(RTreeNode* full, RTreeNode* vacant, int split, int horAxis) {
    int index = 0;
    int minDistance = distanceBetweenPointAndAxis(horAxis, split, *full->objects[0]);
    for (int i = 1; i < MAX_OBJECTS; i++) {
        int dist = distanceBetweenPointAndAxis(horAxis, split, *full->objects[i]);
        if (dist < minDistance) {
            minDistance = dist;
            index = i;
        }
    }
    vacant->objects[vacant->numObjects] = full->objects[index];
    vacant->numObjects++;
    full->objects[index] = NULL;
    full->numObjects--;
    // shift the rest of the pointers
    for (int i = index; i < MAX_OBJECTS - 1; i++) full->objects[i] = full->objects[i + 1];
    full->objects[MAX_OBJECTS - 1] = NULL;
}

void splitLeaf(RTreeNode* leaf, DataPoint* point, RTree* tree) {

    long x_cen = (leaf->mbr.xmin + leaf->mbr.xmax) / 2;
    long y_cen = (leaf->mbr.ymin + leaf->mbr.ymax) / 2;

    RTreeNode* parent = leaf->parent;
    tree->numNodes--;

    RTreeNode* L1 = createNewNode(tree);
    L1->parent = parent;
    L1->level = 0;
    RTreeNode* L2 = createNewNode(tree);
    L2->parent = parent;
    L2->level = 0;

    DataPoint* C0[MAX_OBJECTS];
    DataPoint* C1[MAX_OBJECTS];
    DataPoint* C2[MAX_OBJECTS];
    DataPoint* C3[MAX_OBJECTS];

    int c0_counter = 0;
    int c1_counter = 0;
    int c2_counter = 0;
    int c3_counter = 0;

    DataPoint* temp[MAX_OBJECTS + 1];
    temp[0] = point;
    for (int i = 1; i < MAX_OBJECTS + 1; i++) temp[i] = leaf->objects[i - 1];

    for (int i = 0; i < MAX_OBJECTS + 1; i++) {

        long x_obj = temp[i]->x;
        long y_obj = temp[i]->y;

        if (x_obj >= x_cen) {
            if (y_obj >= y_cen && c2_counter < 4) {
                C2[c2_counter] = temp[i];
                c2_counter++;
            } else if (y_obj <= y_cen && c3_counter < 4) {
                C3[c3_counter] = temp[i];
                c3_counter++;
            }
        } else {
            if (y_obj >= y_cen && c1_counter < 4) {
                C1[c1_counter] = temp[i];
                c1_counter++;
            } else if (y_obj <= y_cen && c0_counter < 4) {
                C0[c0_counter] = temp[i];
                c0_counter++;
            }
        }
    }

    // If split is along horizontal axis then 1 else 0
    int horAxis = 0;

    if (c0_counter > c2_counter) {
        insertPoints(L1, C0, c0_counter);
        insertPoints(L2, C2, c2_counter);
    } else {
        insertPoints(L2, C0, c0_counter);
        insertPoints(L1, C2, c2_counter);
    }

    if (c1_counter > c3_counter) {
        insertPoints(L2, C1, c1_counter);
        insertPoints(L1, C3, c3_counter);
        if (c0_counter > c2_counter) horAxis = 1;
        else horAxis = 0;
    } else {
        insertPoints(L1, C1, c1_counter);
        insertPoints(L2, C3, c3_counter);
        if (c0_counter > c2_counter) horAxis = 0;
        else horAxis = 1;
    }

    int split_axis = horAxis? y_cen: x_cen;

    // Check for m=2 and M=4 condition in L1
    if (L1->numObjects == MAX_OBJECTS) {
        printf("Transfer from L1 to L2 required...\n");
        transferPoints(L1, L2, split_axis, horAxis);
    }
    else if (L2->numObjects == MAX_OBJECTS) {
        printf("Transfer from L1 to L2 required...\n");    
        transferPoints(L2, L1, split_axis, horAxis);
    }

    if (leaf->parent == NULL) {
        // the leaf is the root node
        printf("Root has to be split...call from splitLeaf\n");
        RTreeNode* newRoot = createNewNode(tree);
        newRoot->level = leaf->level + 1;
        newRoot->parent = NULL;
        L1->parent = newRoot;
        L2->parent = newRoot;
        newRoot->children[0] = L1;
        newRoot->children[1] = L2;
        newRoot->numChildren = 2;
        tree->root = newRoot;
        tree->height++;
        free(leaf);
        adjustTree(L1);
        adjustTree(L2);
        return;
    }

    int tuple_identifier = leaf->tuple_identifier;
    for (int i = 0; i < parent->numChildren; i++) {
        if (parent->children[i]->tuple_identifier == tuple_identifier) {
            printf("%d replaced by %d\n", leaf->tuple_identifier, L1->tuple_identifier);
            parent->children[i] = L1;
            adjustTree(L1);
            break;
        } 
    }

    free(leaf);

    if (parent->numChildren == MAX_CHILDREN) {
        splitNode(parent, L2, tree);
        adjustTree(L2);
        adjustTree(L1);
        return;
    }
    
    parent->children[parent->numChildren] = L2;
    parent->numChildren++;
    adjustTree(L2);
}

void insertDataPoint(DataPoint* point, RTree* tree) {

    RTreeNode* leaf = chooseLeaf(point, tree);

    if (leaf->numObjects == MAX_OBJECTS) {
        splitLeaf(leaf, point, tree);
        return;
    }

    leaf->objects[leaf->numObjects] = point;
    leaf->numObjects++;
    adjustTree(leaf);
}

void printTree(RTreeNode* node) {
    if (node == NULL)
    {
        return;
    }

    if (node->numChildren == 0)
    { // Leaf node
        for (int i = 0; i < node->numObjects; i++)
        {
        printf("(%ld, %ld) ", node->objects[i]->x, node->objects[i]->y);
        }
        printf("\n");
    }
    else
    { // Internal node
    if(node->parent!=NULL) {
        for (int i = 0; i < node->numChildren; i++)
        {
            printTree(node->children[i]);
        }
    } else if(node->parent == NULL) {
        for (int i = 0; i < node->numChildren; i++)
        {
        printTree(node->children[i]);
        }
        }
    }
}

void freeNodes(RTreeNode* node) {
    
    if (node->level == 0) {
        for (int i = 0; i < node->numObjects; i++) free(node->objects[i]);
        free(node);
        return;
    }


    for (int i = 0; i < node->numChildren; i++) {
        freeNodes(node->children[i]); 
    }
}

void freeTree(RTree* tree) {
    // free all the dynamically allocated memory
    freeNodes(tree->root);
    free(tree);
}

int search(struct RTreeNode* node, Rectangle s)
{
    if (node->numChildren == 0)
    {
        // Check each entry in the leaf node
       
            // Check for overlap
            if (calculateOverlap(node->mbr, s) == 1)
            {
                return 1;
            } 
            else 
            {
            return 0;
            }
        
    }
    else
    {
        // Check each entry in the non-leaf node
        for (int i = 0; i < node->numChildren; i++)
        {
            // Check for overlap
            if (calculateOverlap(node->children[i]->mbr, s) == 1)
            {
                // Recurse on the child node
               return search(node->children[i], s);
            }
        }
    }

    return 0;
}

int main() {

    FILE *fp;
    fp = fopen("testdata.txt", "r");
    if (fp == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    long x, y;
    RTree* tree = createRTree();
    int i = 0;

    while (fscanf(fp, "%ld %ld", &x, &y) == 2) {

        DataPoint* point = (DataPoint*) malloc (sizeof(DataPoint));

        if (point == NULL) {
            printf("Cannot allocate more memory to point...\n");
            break;
        }

        point->x = x;
        point->y = y;
        
        insertDataPoint(point, tree);
        i++;
    }

    printTree(tree->root);

    printf("Done printing...number of nodes added = %d\n", i);

    freeTree(tree);

    printf("Done deallocating...\n");
}
