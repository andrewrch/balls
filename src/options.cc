#include <options.h>
#include <functional>
#include <fstream>

namespace po = boost::program_options;

namespace surface {

po::options_description Options::getCommandLineOptions() {

  po::options_description desc("Command line only options");
  desc.add_options()
    ("help,h", "produce help message")
    ("config,c", po::value<std::string>()->default_value("surface.cfg"),
     "Location of config file")
  ;
  return desc;
}

po::options_description Options::getGeneralOptions() {
  po::options_description desc("General options");
  desc.add_options()
    ("sf", po::value<std::string>()->
     notifier(std::bind1st(std::mem_fun(&Options::setSurfFragShader), this)),
     "Location of surface fragment shader")
    ("sv", po::value<std::string>()->
     notifier(std::bind1st(std::mem_fun(&Options::setSurfVertShader), this)),
     "Location of surface vertex shader")
    ("bf", po::value<std::string>()->
     notifier(std::bind1st(std::mem_fun(&Options::setBallFragShader), this)),
     "Location of ball fragment shader")
    ("bv", po::value<std::string>()->
     notifier(std::bind1st(std::mem_fun(&Options::setBallVertShader), this)),
     "Location of ball vertex shader")
    ("lf", po::value<std::string>()->
     notifier(std::bind1st(std::mem_fun(&Options::setLightFragShader), this)),
     "Location of light fragment shader")
    ("lv", po::value<std::string>()->
     notifier(std::bind1st(std::mem_fun(&Options::setLightVertShader), this)),
     "Location of light vertex shader")

    ("ww", po::value<unsigned int>()->
     notifier(std::bind1st(std::mem_fun(&Options::setWindowWidth), this)),
     "Window width")
    ("wh", po::value<unsigned int>()->
     notifier(std::bind1st(std::mem_fun(&Options::setWindowHeight), this)),
     "Window height")

    ("sw", po::value<unsigned int>()->
     notifier(std::bind1st(std::mem_fun(&Options::setSurfaceWidth), this)),
     "Surface width")
    ("sh", po::value<unsigned int>()->
     notifier(std::bind1st(std::mem_fun(&Options::setSurfaceHeight), this)),
     "surface height")
  ;
  return desc;
}

bool Options::parseArgs(int argc, char* argv[]) {
  po::variables_map args;
  auto cmd = getCommandLineOptions().add(getGeneralOptions());
  try {
  po::store(po::parse_command_line(argc, argv, cmd), args);
    if (args.count("help")) {
      return false;
    }
    po::notify(args);
  } catch (std::exception& e) {
    return false;
  }

  // Handle config file as well if it's there
  auto cfg = getGeneralOptions();
  std::ifstream conf(args["config"].as<std::string>());
  if (!conf.is_open()) {
    std::cerr << "Can't open config file ";
    std::cerr << args["config"].as<std::string>() << std::endl;
    return false;
  }

  try {
    po::store(po::parse_config_file<char>(conf, cfg), args);
  }
  catch (boost::program_options::error& e) {
    std::cerr << "\n*** Error parsing config file: " << e.what() << "\n\n";
    return false;
  }
  po::notify(args);
  return true;
}

void Options::printHelp(std::ostream& o) {
  auto opts = getCommandLineOptions().add(getGeneralOptions());
  o << "Program Options: \n\n";
  o << opts << std::endl;
}

// Main
Options::Options() {
}

glm::vec2 Options::getWindowSize() const {
  return glm::vec2(windowWidth, windowHeight);
}

glm::vec2 Options::getSurfaceSize() const {
  return glm::vec2(surfaceRows, surfaceCols);
}

const std::string& Options::getSurfFragShader() const {
  return surfFrag;
}

const std::string& Options::getSurfVertShader() const {
  return surfVert;
}

const std::string& Options::getBallFragShader() const {
  return ballFrag;
}

const std::string& Options::getBallVertShader() const {
  return ballVert;
}

const std::string& Options::getLightFragShader() const {
  return lightFrag;
}

const std::string& Options::getLightVertShader() const {
  return lightVert;
}

}
