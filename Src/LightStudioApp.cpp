#include "LightStudioApp.h"

#include <vector>

//-----------------------------------------------------------------------------

void LightStudioApp::Initialize()
{
  pipeline.Create();

  std::vector<float> vertices = {
    -1.0f, -1.0f, -1.0f, -0.57f, -0.57f, -0.57f,
    1.0f, -1.0f, -1.0f, 0.57f, -0.57f, -0.57f,
    1.0f, 1.0f, -1.0f, 0.57f, 0.57f, -0.57f,
    -1.0f, 1.0f, -1.0f, -0.57f, 0.57f, -0.57f,
    -1.0f, 1.0f, 1.0f, -0.57f, 0.57f, 0.57f,
    1.0f, 1.0f, 1.0f, 0.57f, 0.57f, 0.57f,
    1.0f, -1.0f, 1.0f, 0.57f, -0.57f, 0.57f,
    -1.0f, -1.0f, 1.0f, -0.57f, -0.57f, 0.57f
  };
  const unsigned nFloats = vertices.size();

  std::vector<unsigned> indices = {
    0, 1, 2, 0, 2, 3, // front
    6, 7, 4, 6, 4, 5, // back
    7, 0, 3, 7, 3, 4, // left
    1, 6, 5, 1, 5, 2, // right
    3, 2, 5, 3, 5, 4, // top
    7, 6, 1, 7, 1, 0  // bottom 
  };
  const unsigned nIndices = indices.size();
  buffer.Create(
    vertices.data(),
    nFloats,
    indices.data(),
    nIndices,
    { {3, 0}/*position*/, {3, 1}/*normal*/ }
  );

  camera.SetPosition({ 0.0, 0.0, 5.0 });
  camera.LookAtPoint({ 0.0, 0.0, 0.0 });

  model.InitializeDefaultParameters();

  glEnable(GL_DEPTH_TEST);
}

//-----------------------------------------------------------------------------

void LightStudioApp::Render()
{
  pipeline.Use(model, camera);
  buffer.Render();
}

//-----------------------------------------------------------------------------

void LightStudioApp::Update(
  AppToolkit::IAppUtils& appUtils
)
{
  cameraController.Update(camera, appUtils);
}

//-----------------------------------------------------------------------------

void LightStudioApp::Present()
{
  ui.PresentUI(model);
}

//-----------------------------------------------------------------------------

void LightStudioApp::Cleanup()
{
  buffer.Delete();
  pipeline.Cleanup();
}

//-----------------------------------------------------------------------------
