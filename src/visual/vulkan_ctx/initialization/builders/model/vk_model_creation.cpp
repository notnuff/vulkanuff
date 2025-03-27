#define TINYOBJLOADER_IMPLEMENTATION

#include "vk_model_creation.h"
#include <tiny_obj_loader.h>

#include <unordered_map>
#include <hash_map>

#include "common/models_pathes.h"
#include "common/vertex.h"



#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

namespace std {
  template<> struct hash<Vertex> {
    size_t operator()(Vertex const& vertex) const {
      return ((hash<glm::vec3>()(vertex.pos) ^
             (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
             (hash<glm::vec2>()(vertex.texCoord) << 1);
    }
  };
}

void VkModelCreation::DoBuild() {
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  std::string warn, err;

  if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, MODEL_PATH.c_str())) {
    throw std::runtime_error(warn + err);
  }

  std::unordered_map<Vertex, uint32_t> uniqueVertices{};

  for (const auto& shape : shapes) {
    for (const auto& index : shape.mesh.indices) {
      Vertex vertex{};

      vertex.pos = {
        attrib.vertices[3 * index.vertex_index + 0],
        attrib.vertices[3 * index.vertex_index + 1],
        attrib.vertices[3 * index.vertex_index + 2]
      };

      vertex.texCoord = {
        attrib.texcoords[2 * index.texcoord_index + 0],
        1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
      };

      vertex.color = {1.0f, 1.0f, 1.0f};

      if ( !uniqueVertices.contains(vertex) ) {
        uniqueVertices[vertex] = pCtx->modelVertices.size();
        pCtx->modelVertices.push_back(vertex);
      }

      pCtx->modelIndices.push_back(uniqueVertices[vertex]);
    }

  }



}

void VkModelCreation::DoDestroy() {

}