//Start at the root of the tree.
//If the current node=leaf node, then print all the objects in that node and return.
//If the current node=internal node, then recursively traverse each of its child nodes in order (from left to right).


void preOrderTraversal(struct RTreeNode* node) 
{
    if (node == NULL) {
        return;
    }
    if (node->numChildren == 0) {
        // The current node is a leaf node, print all the objects in it.
        for (int i = 0; i < MAX_CHILDREN; i++) {
            if (node->objects[i].x != 0 && node->objects[i].y != 0) {
                printf("(%d,%d) ", node->objects[i].x, node->objects[i].y);
            }
        }
        return;
    }
    // The current node is an internal node, recursively traverse each of its child nodes.
    for (int i = 0; i < MAX_CHILDREN; i++) {
        if (node->child_pointer[i] != NULL) {
            preOrderTraversal(node->child_pointer[i]);
        }
    }
}

