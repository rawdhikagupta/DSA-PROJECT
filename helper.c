// Helper function to check if two rectangles overlap

int overlap(Rectangle r1, Rectangle r2) {
  if (r1.xmin > r2.xmax || r1.ymin > r2.ymax || r2.xmin > r1.xmax || r2.ymin > r1.ymax) {
    return 0;
  }
  return 1;
}

// Helper function to calculate the area of a rectangle

int area(Rectangle r) {
  return (r.xmax - r.xmin) * (r.ymax - r.ymin);
}

// Helper function to calculate the minimum bounding rectangle that bounds two rectangles

Rectangle mbr(Rectangle r1, Rectangle r2) {
  Rectangle r;
  r.xmin = (r1.xmin < r2.xmin) ? r1.xmin : r2.xmin;
  r.ymin = (r1.ymin < r2.ymin) ? r1.ymin : r2.ymin;
  r.xmax = (r1.xmax > r2.xmax) ? r1.xmax : r2.xmax;
  r.ymax = (r1.ymax > r2.ymax) ? r1.ymax : r2.ymax;
  return r;
}