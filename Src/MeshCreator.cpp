#include "MeshCreator.h"

//---------------------------------------------------------------------------------------
// MeshCreatorImpl prototype
//---------------------------------------------------------------------------------------

namespace MeshCreatorImpl {
  
  void FillCubeVertexData(
    MeshData& mesh
  );

  void FillCubeIndexData(
    MeshData& mesh
  );

}

//---------------------------------------------------------------------------------------
// MeshCreator
//---------------------------------------------------------------------------------------

MeshData MeshCreator::CreateCube()
{
  MeshData mesh;

  MeshCreatorImpl::FillCubeVertexData(mesh);
  MeshCreatorImpl::FillCubeIndexData(mesh);

  return mesh;
}

//---------------------------------------------------------------------------------------
// MeshCreatorImpl
//---------------------------------------------------------------------------------------

void MeshCreatorImpl::FillCubeVertexData(
  MeshData& mesh
)
{
  mesh.vertices = {
    -1.0f, -1.0f, -1.0f, -0.57f, -0.57f, -0.57f,
    1.0f, -1.0f, -1.0f, 0.57f, -0.57f, -0.57f,
    1.0f, 1.0f, -1.0f, 0.57f, 0.57f, -0.57f,
    -1.0f, 1.0f, -1.0f, -0.57f, 0.57f, -0.57f,
    -1.0f, 1.0f, 1.0f, -0.57f, 0.57f, 0.57f,
    1.0f, 1.0f, 1.0f, 0.57f, 0.57f, 0.57f,
    1.0f, -1.0f, 1.0f, 0.57f, -0.57f, 0.57f,
    -1.0f, -1.0f, 1.0f, -0.57f, -0.57f, 0.57f
  };
}

//---------------------------------------------------------------------------------------

void MeshCreatorImpl::FillCubeIndexData(
  MeshData& mesh
)
{
  mesh.indices = {
    0, 1, 2, 0, 2, 3, // front
    6, 7, 4, 6, 4, 5, // back
    7, 0, 3, 7, 3, 4, // left
    1, 6, 5, 1, 5, 2, // right
    3, 2, 5, 3, 5, 4, // top
    7, 6, 1, 7, 1, 0  // bottom 
  };
}

//---------------------------------------------------------------------------------------
