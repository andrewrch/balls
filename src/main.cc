#include "window.h"
#include "options.h"
#include "surface_app.h"

using namespace surface;

int main(int argc, char* argv[]) {
  // Process command line and config file args
  Options options;
  if (!options.parseArgs(argc, argv)) {
    options.printHelp();
    return 0;
  }

  // Create the window - this creates an OpenGL context
  Window window(options);

  SurfaceApp app(window, options);
  app.mainLoop();
  return 0;
}
