
/*
=====================
cSkybox.h
=====================
*/

#ifndef _SKYBOX_H
#define _SKYBOX_H

#include "windowOGL.h"
#include "glm\glm.hpp"
#include "glx\glext.h"
#include "wglext.h"
#include "GL\glut.h"
#include "cShapes.h"
#include <vector>

class skyboxMaterial
{
public:
	skyboxMaterial();
	~skyboxMaterial();

	void destory();
	void bind();
	void unbind();

	void loadSkyboxTexture(
		const std::string& PosXFilename,
		const std::string& NegXFilename,
		const std::string& PosYFilename,
		const std::string& NegYFilename,
		const std::string& PosZFilename,
		const std::string& NegZFilename);

	GLuint getskyboxTexture();

private:

	GLuint m_skyboxTexture;

};



#endif