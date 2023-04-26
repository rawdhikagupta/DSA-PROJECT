//Start at the root node and traverse down the tree
//is an internal node, check if child nodes intersect with the search point's MBR + continue recursively
// If node is leaf node, check if data points match the search point, and return if match is found
//If we reach a leaf node without finding a match, we return null.


struct RTreeNode* searchRTree(RTree* tree, DataPoint point) {
    struct RTreeNode* currNode = tree->root;
    
    // Traverse the tree, following the path that intersects with the search point's MBR
    while (currNode != NULL) {
        // If the current node is a leaf node, check if any of its data points match the search point
        if (currNode->level == 0) {
            for (int i = 0; i < currNode->numChildren; i++) {
                if (currNode->objects[i].x == point.x && currNode->objects[i].y == point.y) {
                    return currNode;
                }
            }
            return NULL;
        }
        // If the current node is an internal node, recursively search its child nodes
        else {
            for (int i = 0; i < currNode->numChildren; i++) {
                if (currNode->child_pointer[i]->mbr.xmin <= point.x && currNode->child_pointer[i]->mbr.xmax >= point.x && 
                    currNode->child_pointer[i]->mbr.ymin <= point.y && currNode->child_pointer[i]->mbr.ymax >= point.y) {
                    currNode = currNode->child_pointer[i];
                    break;
                }
            }
        }
    }
    return NULL;
}
