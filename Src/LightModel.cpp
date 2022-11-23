#include "LightModel.h"

//---------------------------------------------------------------------------------------

void LightModel::InitializeDefaultParameters()
{
  material.ambient = { 1.0f, 0.0f, 1.0f };
  material.diffuse = { 1.0f, 0.0f, 1.0f };
  material.specular = { 1.0f, 0.0f, 1.0f };
  material.shininess = 32.0f;

  for (size_t i = 0; i < MAX_LIGHTS; ++i) {
    Light& light = GetLight(i);

    light.ambient = { 0.2f, 0.2f, 0.2f };
    light.diffuse = { 0.5f, 0.5f, 0.5f };
    light.specular = { 1.0f, 1.0f, 1.0f };
    light.direction = { 0.0f, 0.0f, 1.0f };

    light.position = { 0.0f, 0.0f, -5.0f };
    light.quadratic = 0.2f;
    light.linear = 0.22f;
    light.constant = 1.0f;

    light.enabled = false;
    light.isDirectional = true;
  }

  GetLight(0).enabled = true;
}

//---------------------------------------------------------------------------------------

Material& LightModel::GetMaterial()
{
  const LightModel& constThis = *this;
  return const_cast<Material&>(constThis.GetMaterial());
}

//---------------------------------------------------------------------------------------

const Material& LightModel::GetMaterial() const
{
  return material;
}

//---------------------------------------------------------------------------------------

Light& LightModel::GetLight(
  const unsigned index
)
{
  const LightModel& constThis = *this;
  return const_cast<Light&>(constThis.GetLight(index));
}

//---------------------------------------------------------------------------------------

const Light& LightModel::GetLight(
  const unsigned index
) const
{
  if (index >= MAX_LIGHTS) {
    return lights[MAX_LIGHTS - 1];
  }

  return lights[index];
}

//---------------------------------------------------------------------------------------
