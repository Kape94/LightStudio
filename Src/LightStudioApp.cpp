#include "LightStudioApp.h"

#include "MeshCreator.h"
#include "MeshData.h"

//-----------------------------------------------------------------------------

void LightStudioApp::Initialize()
{
  pipeline.Create();

  const MeshData cubeData = MeshCreator::CreateCube();
  const unsigned nFloats = cubeData.vertices.size();
  const unsigned nIndices = cubeData.indices.size();
  buffer.Create(
    cubeData.vertices.data(),
    nFloats,
    cubeData.indices.data(),
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
