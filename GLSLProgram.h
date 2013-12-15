#pragma once

#include "GL/glew.h"
#include <string>
#include <map>
#include <vector>

/// For now, just 1 vertex's type can be attach to the program
/// In order add more than 1 vertex's type to the program, the shaders should be named (eg. "shader1", "shader2" & so on)
class CGLSLProgram
{
public:
    enum SHADERTYPE {VERTEX = 0, FRAGMENT, GEOMETRY, TESSELATION};

	GLuint m_uIdProgram;		//id of the program
	GLuint m_vIdShader[4];	//ids of the loaded shaders; the 4th is empty always
	CGLSLProgram(void);
	~CGLSLProgram(void);

	void loadShader(std::string strFileName, SHADERTYPE typeShader);
	void create();
	void create_link();
	void link();
	void enable();
	void disable();
	void addAttribute(std::string strParName);
	void addSubroutine(std::string strFunctionName, unsigned int iShaderType);
	void addUniform(std::string strParName);
	void addUniformSubroutine(std::string strParName, int iShaderType);
	GLint getLocation(std::string strParName);
	GLuint getId();
	void setSubroutine(std::string strUniformName, std::string strSubRoutine, int iShaderType);
	std::map<std::string, GLint> m_mapSubroutines;
	void recompileShader(std::string strFileName, SHADERTYPE typeShader);
private:
	std::map<std::string, GLint> m_mapVarShader;
	std::vector<GLuint> m_vRoutinesIds;
	bool loadShaderFile(std::string strFilename, GLuint iHandle);
	void checkLinkingErrors();
};
