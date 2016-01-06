#include <stdio.h>
#include "shadow_map.h"
//#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#define M_PI 3.14


namespace surface {

void initUpDirections(glm::vec3 up_directions[6]) {
  up_directions[0][0] = 0.0;
  up_directions[0][1] = -1.0;
  up_directions[0][2] = 0.0;

  up_directions[1][0] = 0.0;
  up_directions[1][1] = -1.0;
  up_directions[1][2] = 0.0;

  up_directions[2][0] = 0.0;
  up_directions[2][1] = 0.0;
  up_directions[2][2] = 1.0;

  up_directions[3][0] = 0.0;
  up_directions[3][1] = 0.0;
  up_directions[3][2] = -1.0;

  up_directions[4][0] = 0.0;
  up_directions[4][1] = -1.0;
  up_directions[4][2] = 0.0;

  up_directions[5][0] = 0.0;
  up_directions[5][1] = -1.0;
  up_directions[5][2] = 0.0;
}

void setLightDirections(glm::vec3 light_directions[6], glm::vec3 light_pos) {
  // +X
  light_directions[0][0] = light_pos[0] + 1;
  light_directions[0][1] = light_pos[1];
  light_directions[0][2] = light_pos[2];
  // -X
  light_directions[1][0] = light_pos[0] - 1;
  light_directions[1][1] = light_pos[1];
  light_directions[1][2] = light_pos[2];
  // +Y
  light_directions[2][0] = light_pos[0];
  light_directions[2][1] = light_pos[1] + 1;
  light_directions[2][2] = light_pos[2];
  // -Y
  light_directions[3][0] = light_pos[0];
  light_directions[3][1] = light_pos[1] - 1;
  light_directions[3][2] = light_pos[2];
  // +Z
  light_directions[4][0] = light_pos[0];
  light_directions[4][1] = light_pos[1];
  light_directions[4][2] = light_pos[2] + 1;
  // -Z
  light_directions[5][0] = light_pos[0];
  light_directions[5][1] = light_pos[1];
  light_directions[5][2] = light_pos[2] - 1;
}

ShadowMap::ShadowMap(int width, int height,
                     const glm::vec3& lightPos) :
  width(width),
  height(height),
  shader({ Shader("shaders/shadow.glslv", ShaderType::VERT),
           Shader("shaders/shadow.glslf", ShaderType::FRAG)}),
  lightPos(lightPos)
{
  // Generate the framebuffer
  glGenFramebuffers(1, &fb);

  // Create the depth buffer
  glGenTextures(1, &depth);
  glBindTexture(GL_TEXTURE_2D, depth);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0,
               GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  shadowMap.bindCube(GL_TEXTURE0 + 1);
  for (unsigned int i = 0; i < 6; i++) {
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
                 GL_R32F, width, height,
                 0, GL_RED, GL_FLOAT, 0);
  }

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
  shadowMap.unbindCube();

  glBindFramebuffer(GL_FRAMEBUFFER, fb);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth, 0);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  initUpDirections(upDirections);
  setLightPos(lightPos);

  lightProj = glm::perspective(float(M_PI) / 2, width / (float) height, 0.0001f, 100.0f);
}

void ShadowMap::setLightPos(const glm::vec3& pos) {
  // For shader
  lightPos = pos;
  setLightDirections(lightDirections, pos);
  for (unsigned int i = 0; i < 6; i++) {
    viewMats[i] = glm::lookAt(pos, lightDirections[i], upDirections[i]);
  }
}

void ShadowMap::bind(unsigned int face) {
  glBindFramebuffer(GL_FRAMEBUFFER, fb);
  glViewport(0, 0, width, height);
  glCullFace(GL_FRONT);
  shadowMap.addToFrameBuffer(fb, GL_COLOR_ATTACHMENT0,
                             GL_TEXTURE_CUBE_MAP_POSITIVE_X + face);
  glDrawBuffer(GL_COLOR_ATTACHMENT0);
  glReadBuffer(GL_NONE);
  // Check framebuffer
  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    fprintf(stderr, "Something went wrong making shadow cube %d\n", glCheckFramebufferStatus(GL_FRAMEBUFFER));
  }
}

void ShadowMap::unbind() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  //shadowMap.unbindCube();
  glCullFace(GL_BACK);
  glDrawBuffer(GL_BACK);
  glReadBuffer(GL_BACK);
}

}
