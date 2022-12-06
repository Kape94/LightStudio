#include "ShapesManager.h"

#include "MeshCreator.h"

#include <algorithm>

//---------------------------------------------------------------------------------------

void ShapesManager::Initialize()
{
  Shape cube;
  cube.Create("Cube", MeshCreator::CreateCube());

  Shape icosahedron;
  icosahedron.Create("Icosahedron", MeshCreator::CreateIcosahedron());

  shapes.emplace_back(cube);
  shapes.emplace_back(icosahedron);

  selectedIndex = 0;
}

//---------------------------------------------------------------------------------------

void ShapesManager::Cleanup()
{
  for (Shape& shape : shapes) {
    shape.Cleanup();
  }
}

//---------------------------------------------------------------------------------------

const std::vector<Shape>& ShapesManager::Shapes() const
{
  return shapes;
}

//---------------------------------------------------------------------------------------

void ShapesManager::SelectShapeByName(
  const std::string& shapeName
)
{
  auto it = std::find_if(shapes.begin(), shapes.end(), [shapeName](Shape& shape) {
    return shape.Name() == shapeName;
  });

  if (it != shapes.end()) {
    selectedIndex = std::distance(shapes.begin(), it);
  }
}

//---------------------------------------------------------------------------------------

Shape& ShapesManager::SelectedShape()
{
  return shapes.at(selectedIndex);
}

//---------------------------------------------------------------------------------------

const Shape& ShapesManager::SelectedShape() const
{
  return shapes.at(selectedIndex);
}

//---------------------------------------------------------------------------------------
