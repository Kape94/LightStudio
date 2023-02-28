#include "LightStudioApp.h"

#include "MeshCreator.h"
#include "MeshData.h"

//-----------------------------------------------------------------------------

void LightStudioApp::Initialize()
{
  effectManager.Initialize();
  shapesManager.Initialize();

  camera.SetPosition({ 0.0, 0.0, 5.0 });
  camera.LookAtPoint({ 0.0, 0.0, 0.0 });

  model.InitializeDefaultParameters();

  glEnable(GL_DEPTH_TEST);
}

//-----------------------------------------------------------------------------

void LightStudioApp::Render()
{
  IEffect& effect = effectManager.GetCurrentEffect();
  effect.Use(model, camera);
  shapesManager.SelectedShape().Render();
  effect.PostProcess(model, camera);
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
  ui.Present(model, effectManager, shapesManager);
}

//-----------------------------------------------------------------------------

void LightStudioApp::Cleanup()
{
  shapesManager.Cleanup();
  effectManager.Cleanup();
}

//-----------------------------------------------------------------------------
