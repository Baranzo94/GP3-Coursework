/*#ifndef CSKYBOXTEXTURE_H
#define CSKYBOXTEXTURE_H

#include <glew.h>

#include <iostream>
#include <string>

//GLuint convertSDLSurfaceToGLTexture(SDL_Surface * surface);

GLuint loadTextureFromFile(const std::string& filename);
GLuint loadTextureFromFont(const std::string& fontFilename, int pointSize, const std::string& text);

void loadCubeMapSide(const std::string& filename, GLenum cubeSide);

#endif*/