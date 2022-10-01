#include "AppToolkit/AppToolkit.h"

int main() 
{
  AppToolkit::IApp& app = AppToolkit::GetApp();

  app.Initialize();
  app.Run();

  return 0;
}