/*
=================
cSkybox.cpp
=================
*/

#include "cSkybox.h"
/*
cSkybox::skyboxMaterial()
{
	m_skyboxTexture = 0;
}

skyboxMaterial::~skyboxMaterial()
{
}

void skyboxMaterial::destory()
{
	if (m_skyboxTexture)
	{
		glDeleteTextures(1, &m_skyboxTexture);
	}
}

void skyboxMaterial::bind()
{
	glDepthMask(GL_FALSE);
	glUseProgram(m_ShaderProgram);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture);

	GLint vertexPosLocation = glGetAttribLocation(m_ShaderProgram, "vertexPosition");
	glBindAttribLocation(m_ShaderProgram, vertexPosLocation, "vertexPosition");

	glEnableVertexAttribArray(vertexPosLocation);
	glVertexAttribPointer(vertexPosLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
}

void skyboxMaterial::unbind()
{
	glDepthMask(GL_TRUE);
}

void skyboxMaterial::loadSkyboxTexture
(const std::string& PosXFilename,
const std::string& NegXFilename,
const std::string& PosYFilename,
const std::string& NegYFilename,
const std::string& PosZFilename,
const std::string& NegZFilename)
{
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &m_skyboxTexture);

	glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);

	loadSkyboxSide(PosXFilename, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
	loadSkyboxSide(NegXFilename, GL_TEXTURE_CUBE_MAP_NEGATIVE_X);

	loadSkyboxSide(PosZFilename, GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
	loadSkyboxSide(NegZFilename, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);

	loadSkyboxSide(PosYFilename, GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
	loadSkyboxSide(NegYFilename, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
}

GLuint skyboxMaterial::getskyboxTexture()
{
	return m_skyboxTexture;
}*/