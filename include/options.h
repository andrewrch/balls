#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <boost/program_options.hpp>
#include <glm/glm.hpp>

namespace surface {
  class Options {
    public:
      Options();
      ~Options() = default;
      Options(const Options&) = delete;
      Options& operator=(const Options&) = delete;

      /**
       * Call this to parse config file and command line arguments
       * @return bool if failed or if help requested.
       */
      bool parseArgs(int argc, char* argv[]);

      /**
       * Prints the program options
       *
       * @param ostream By default prints to standard out.
       */
      void printHelp(std::ostream& = std::cout);

      // Getters
      glm::vec2 getWindowSize() const;
      glm::vec2 getSurfaceSize() const;
      const std::string& getSurfFragShader() const;
      const std::string& getSurfVertShader() const;
      const std::string& getBallFragShader() const;
      const std::string& getBallVertShader() const;
      const std::string& getLightFragShader() const;
      const std::string& getLightVertShader() const;

      // And setters for boost program options
      inline void setWindowWidth(unsigned int w);
      inline void setWindowHeight(unsigned int h);
      inline void setSurfaceWidth(unsigned int w);
      inline void setSurfaceHeight(unsigned int h);
      inline void setSurfFragShader(std::string s);
      inline void setSurfVertShader(std::string s);
      inline void setBallFragShader(std::string s);
      inline void setBallVertShader(std::string s);
      inline void setLightFragShader(std::string s);
      inline void setLightVertShader(std::string s);

    private:
      // Shaders
      std::string ballFrag, ballVert, surfVert, surfFrag, lightVert, lightFrag;
      // Window size
      unsigned int windowWidth, windowHeight;
      // Surface vertices
      unsigned int surfaceRows, surfaceCols;

      // Setting up the options
      boost::program_options::options_description getCommandLineOptions();
      boost::program_options::options_description getGeneralOptions();
  };

// Inline implementations

inline void Options::setWindowWidth(unsigned int w) {
  windowWidth = w;
}
inline void Options::setWindowHeight(unsigned int h) {
  windowHeight = h;
}
inline void Options::setSurfaceWidth(unsigned int w) {
  surfaceCols = w;
}
inline void Options::setSurfaceHeight(unsigned int h) {
  surfaceRows = h;
}
inline void Options::setSurfFragShader(std::string s) {
  surfFrag = s;
}
inline void Options::setSurfVertShader(std::string s) {
  surfVert = s;
}
inline void Options::setBallFragShader(std::string s) {
  ballFrag = s;
}
inline void Options::setBallVertShader(std::string s) {
  ballVert = s;
}
inline void Options::setLightFragShader(std::string s) {
  lightFrag = s;
}
inline void Options::setLightVertShader(std::string s) {
  lightVert = s;
}

}
