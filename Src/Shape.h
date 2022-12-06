#ifndef _SHAPE_H
#define _SHAPE_H

#include "Buffer.h"
#include "MeshData.h"

#include <string>

class Shape {
  public:

    void Create(
      const std::string& meshName,
      const MeshData& meshData
    );

    void Render();

    void Cleanup();

    const std::string& Name() const;

  private:

    std::string name;
    Buffer buffer;
};

#endif
