// Search function to find all objects in the R-tree that overlap with a given rectangle

void search(Rectangle exampleRect, struct RTreeNode * node, void (*callback)(DataPoint)) 
{
  if (node == NULL) {
    return;
  }

  if (node->level == 0) {
    for (int i = 0; i < node->numChildren; i++) {
      if (overlap(exampleRect, node->objects[i].rect)) {
        callback(node->objects[i]);
      }
    }
  } else {
    for (int i = 0; i < node->numChildren; i++) {
      if (overlap(exampleRect, node->child_pointer[i]->mbr)) {
        search(exampleRect, node->child_pointer[i], callback);
      }
    }
  }
}
