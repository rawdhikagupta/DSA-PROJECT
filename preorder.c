// Pre-order traversal function to visit all nodes in the R-tree

void preOrderTraversal(struct RTreeNode * node, void (*callback)(struct RTreeNode *)) 
{
  if (node == NULL) {
    return;
  }
  callback(node);
  for (int i = 0; i < node->numChildren; i++) {
    preOrderTraversal(node->child_pointer[i], callback);
  }
}
