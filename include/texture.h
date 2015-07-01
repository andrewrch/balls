#pragma once

#include <string>
#include <GL/glew.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>

namespace surface {

class Texture
{
  public:
    /**
     * Texture constructor.
     *
     * Allocates space for a texture.
     */
    Texture();

    /**
     * Texture destructor.
     *
     * Clears up any allocated texture memory
     */
    ~Texture() {
      if (textureObject)
        glDeleteTextures(1, &textureObject);
    }
    Texture(Texture&& rhs);
    Texture& operator=(Texture&& rhs);

    Texture(const Texture& other) = delete;
    Texture operator=(const Texture& other) = delete;

    /**
     * setImage function.
     *
     * Update the texture.
     * @param filename The filename, uses OpenCV to read the image, so can 
     * handle various image types.
     * @param type The image type, can be either Texture::colour or
     * Texture::intensity.
     * @throw std::runtime_error if type does not match either colour or 
     * intensity.
     */
    void setImage(const std::string& filename);

    /**
     * Alternative setImage function.
     *
     * If we already have a CV matrix
     *
     * @param image CV matrix containing image data.
     */
    void setImage(const cv::Mat& image);

    /**
     * bind method.
     *
     * Binds the texture to a given texture unit.
     *
     * @param textureUnit The text unit to bind this texture too.
     */
    void bind(GLenum textureUnit = GL_TEXTURE0) const;
    void bindCube(GLenum textureUnit = GL_TEXTURE0) const;

    /**
     * unbind method.
     *
     * Unbinds the texture from a given texture unit.
     *
     * @param textureUnit The text unit to unbind this texture from.
     */
    void unbind(GLenum textureUnit = GL_TEXTURE0) const;
    void unbindCube(GLenum textureUnit = GL_TEXTURE0) const;

    /**
     * initEmpty.
     *
     * Initialises an empty texture.  Uses the given image type to work out
     * correct internal format for the texture.  We can them use this texture 
     * memory to render to.
     *
     * @param width The width of the texture.
     * @param height The height of the texture.
     */
    void initEmpty(unsigned int width,
                   unsigned int height);

    /**
     * isEmpty method.
     *
     * @return true if the texture is empty
     */
    bool isEmpty() const {
      return empty;
    }

    /**
     * addToFrameBuffer method.
     *
     * Binds this texture to an attachment on a framebuffer
     *
     * @param framebuffer The framebuffer to attach to.
     * @param attachment Where we would like to attach the texture to on the
     *                   framebuffer.
     */
    void addToFrameBuffer(GLuint framebuffer, GLenum attachment,
                          GLenum target = GL_TEXTURE_2D) const;

    /** colour var.  To load a colour image. */
    const static unsigned int colour = CV_LOAD_IMAGE_COLOR;
  private:
    void init(const std::string& filename);
    void matToTexture(const cv::Mat& image);
    GLuint textureObject;
    bool empty;
};

}
