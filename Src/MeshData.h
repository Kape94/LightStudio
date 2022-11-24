#ifndef _MESH_DATA_H
#define _MESH_DATA_H

#include <vector>

struct MeshData {
  std::vector<float> vertices;
  std::vector<unsigned> indices;
};

#endif
