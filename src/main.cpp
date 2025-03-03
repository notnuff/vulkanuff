#include <iostream>

#include "visual/visual_app.h"

int main() {
  VisualApp app;

  try {
    app.Run();

  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}