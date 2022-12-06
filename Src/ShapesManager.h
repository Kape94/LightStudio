#ifndef _SHAPES_MANAGER_H
#define _SHAPES_MANAGER_H

#include "Shape.h"

#include <string>
#include <vector>

class ShapesManager {
  public:

    void Initialize();

    void Cleanup();

    const std::vector<Shape>& Shapes() const;

    void SelectShapeByName(
      const std::string& shapeName
    );

    Shape& SelectedShape();

    const Shape& SelectedShape() const;

  private:

    size_t selectedIndex = 0;
    std::vector<Shape> shapes;
};

#endif
