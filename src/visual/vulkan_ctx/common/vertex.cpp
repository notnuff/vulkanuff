//
// Created by notnuff on 16.02.25.
//

#include "vertex.h"

std::vector<Vertex> testVertices = {
  {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
  {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
  {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
  {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
};

std::vector<uint32_t> testIndices = {
  0, 1, 2, 2, 3, 0
};