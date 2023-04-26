//This function takes a pointer to a R-tree node as input and recursively traverses through the tree, printing the MBR values of each internal node

void printRTree(struct RTreeNode * node) {
    if (node == NULL) {
        return;
    }
    
    if (node->level == 0) { // Leaf node
        printf("Leaf Node - Objects: ");
        for (int i = 0; i < node->numChildren; i++) {
            printf("(%d, %d) ", node->objects[i].x, node->objects[i].y);
        }
        printf("\n");
    } else { // Internal node
        printf("Internal Node - MBR: {(%d, %d), (%d, %d)}\n", node->mbr.xmin, node->mbr.ymax, node->mbr.xmax, node->mbr.ymin);
        for (int i = 0; i < node->numChildren; i++) {
            printRTree(node->child_pointer[i]);
        }
    }
}