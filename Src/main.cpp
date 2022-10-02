#include "AppToolkit/AppToolkit.h"

int main() 
{
  AppToolkit::IApp& app = AppToolkit::GetApp();

  AppToolkit::WindowProperties winProps;
  winProps.name = "Test window";
  winProps.width = 1280; 
  winProps.height = 960;

  app.DefineWindowProperties(winProps);

  app.Run();

  return 0;
}