#include "Shape.h"

//---------------------------------------------------------------------------------------

void Shape::Create(
  const std::string& meshName, 
  const MeshData& meshData
)
{
  name = meshName;

  const unsigned nVertices = static_cast<unsigned>(meshData.vertices.size());
  const unsigned nIndices = static_cast<unsigned>(meshData.indices.size());
  buffer.Create(
    meshData.vertices.data(),
    nVertices,
    meshData.indices.data(),
    nIndices,
    { {3, 0/*position*/}, {3, 1/*normal*/}}
  );
}

//---------------------------------------------------------------------------------------

void Shape::Render()
{
  buffer.Render();
}

//---------------------------------------------------------------------------------------

void Shape::Cleanup()
{
  buffer.Delete();
}

//---------------------------------------------------------------------------------------

const std::string& Shape::Name() const
{
  return name;
}

//---------------------------------------------------------------------------------------
