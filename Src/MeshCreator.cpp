#include "MeshCreator.h"

#include <cmath>

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

MeshData MeshCreator::CreateIcosahedron()
{
  /*Based on: https://www.danielsieger.com/blog/2021/01/03/generating-platonic-solids.html */
  const float phi = (1.0f + sqrtf(5.0f)) * 0.5f;
  
  const float a = 1.0f;
  const float b = 1.0f / phi;

  MeshData mesh;

  mesh.vertices = {
    // pos.x, pos.y , pos.z , norm.x  , norm.y  , norm.z
    0       , b     , -a    , 0       , b       , -a    ,
    b       , a     , 0     , b       , a       , 0     ,
    -b      , a     , 0     , -b      , a       , 0     ,
    0       , b     , a     , 0       , b       , a     ,
    0       , -b    , a     , 0       , -b      , a     ,
    -a      , 0     , b     , -a      , 0       , b     ,
    0       , -b    , -a    , 0       , -b      , -a    ,
    a       , 0     , -b    , a       , 0       , -b    ,
    a       , 0     , b     , a       , 0       , b     ,
    -a      , 0     , -b    , -a      , 0       , -b    ,
    b       , -a    , 0     , b       , -a      , 0     ,
    -b      , -a    , 0     , -b      , -a      , 0    
  };

  mesh.indices = {
    2, 1, 0,
    1, 2, 3,
    5, 4, 3,
    4, 8, 3,
    7, 6, 0,
    6, 9, 0,
    11, 10, 4,
    10, 11, 6,
    9, 5, 2,
    5, 9, 11,
    8, 7, 1,
    7, 8, 10,
    2, 5, 3,
    8, 1, 3,
    9, 2, 0,
    1, 7, 0,
    11, 9, 6,
    7, 10, 6,
    5, 11, 4,
    10, 8, 4
  };

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
