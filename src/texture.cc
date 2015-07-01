#include <iostream>
#include "texture.h"

namespace surface {

Texture::Texture() : textureObject(0), empty(true) {
  // Allocate space for texture
  glGenTextures(1, &textureObject);
}

Texture::Texture(Texture&& rhs) : textureObject(rhs.textureObject),
                                  empty(rhs.empty) {
  rhs.textureObject = 0;
  rhs.empty = true;
}

Texture& Texture::operator=(Texture&& rhs) {
  textureObject = rhs.textureObject;
  rhs.textureObject = 0;
  empty = rhs.empty;
  rhs.empty = true;
  return *this;
}

void Texture::setImage(const std::string& f) {
  cv::Mat image = cv::imread(f, CV_LOAD_IMAGE_COLOR);
  if (image.data == nullptr) {
    throw std::runtime_error("Problem loading texture: " + f);
  }
  setImage(image);
}

void Texture::setImage(const cv::Mat& image) {
  matToTexture(image);
  empty = false;
}

void Texture::initEmpty(unsigned int width,
                        unsigned int height) {
  glBindTexture(GL_TEXTURE_2D, textureObject);
  glTexImage2D(GL_TEXTURE_2D,
               0,
               GL_RGBA32F,
               width,
               height,
               0,
               GL_RGBA,
               GL_FLOAT,
               nullptr);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::addToFrameBuffer(GLuint framebuffer, GLenum attachment, GLenum target) const {
  glFramebufferTexture2D(GL_FRAMEBUFFER,
                         attachment,
                         target,
                         textureObject,
                         0);
}

// Function turn a cv::Mat into a texture
void Texture::matToTexture(const cv::Mat& mat)
{
  // Bind to our texture handle
  glBindTexture(GL_TEXTURE_2D, textureObject);
  //use fast 4-byte alignment (default anyway) if possible
  glPixelStorei(GL_PACK_ALIGNMENT, (mat.step & 3) ? 1 : 4);
  //set length of one complete row in destination data (doesn't need to equal img.cols)
  glPixelStorei(GL_PACK_ROW_LENGTH, mat.step/mat.elemSize());

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // Set incoming texture format
  GLenum inputColourFormat = GL_BGR;
  GLenum inputType = GL_UNSIGNED_BYTE;
  GLenum internalFormat = GL_RGBA;
  // Create the texture
  glTexImage2D(GL_TEXTURE_2D,     // Type of texture
               0,                 // Pyramid level (for mip-mapping) - 0 is the top level
               internalFormat,    // Internal colour format to convert to
               mat.cols,          // Image width
               mat.rows,          // Image height
               0,                 // Border width in pixels (can either be 1 or 0)
               inputColourFormat, // Input image format (i.e. GL_RGB, GL_RGBA, GL_BGR etc.)
               inputType,         // Image data type
               mat.data);         // The actual image data itself
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bind(GLenum textureUnit) const {
  glActiveTexture(textureUnit);
  glBindTexture(GL_TEXTURE_2D, textureObject);
}

void Texture::unbind(GLenum textureUnit) const {
  glActiveTexture(textureUnit);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bindCube(GLenum textureUnit) const {
  glActiveTexture(textureUnit);
  glBindTexture(GL_TEXTURE_CUBE_MAP, textureObject);
}

void Texture::unbindCube(GLenum textureUnit) const {
  glActiveTexture(textureUnit);
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

}
