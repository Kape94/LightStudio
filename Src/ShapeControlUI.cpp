#include "ShapeControlUI.h"

#include "imgui.h"

//---------------------------------------------------------------------------------------

void ShapeControlUI::PresentUI(
  ShapesManager& shapesManager
)
{
  ImGui::Begin("Shapes");

  const std::string currentShapeName = shapesManager.SelectedShape().Name();
  if (ImGui::BeginCombo("Shape", currentShapeName.c_str())) {
    for (const Shape& shape : shapesManager.Shapes()) {
      const std::string shapeName = shape.Name();
      const bool isSelected = shapeName == currentShapeName;
      if (ImGui::Selectable(shape.Name().c_str(), isSelected)) {
        shapesManager.SelectShapeByName(shapeName);
      }
      if (isSelected) {
        ImGui::SetItemDefaultFocus();
      }
    }

    ImGui::EndCombo();
  }
  ImGui::End();
}

//---------------------------------------------------------------------------------------
