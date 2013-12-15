#include "GLSLProgram.h"
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

CGLSLProgram::CGLSLProgram(void)
{
	m_vIdShader[VERTEX] = 0;
	m_vIdShader[FRAGMENT] = 0;
	m_vIdShader[GEOMETRY] = 0;
	m_vIdShader[TESSELATION] = 0;
	m_mapVarShader.clear();
	m_mapSubroutines.clear();
}


CGLSLProgram::~CGLSLProgram(void)
{
	m_mapVarShader.clear();
	m_mapSubroutines.clear();
	if(m_uIdProgram > 0)
	{
		glDeleteProgram(m_uIdProgram);
		cout << "Program deleted! " << endl;
	}
}

///
bool CGLSLProgram::loadShaderFile(std::string strFilename, GLuint iHandle)
{
	std::ifstream shaderSource(strFilename.c_str());
	if (!shaderSource.is_open())
  {
   std::cerr<< " File not found "<< strFilename.c_str()<< endl;
   return false;
  }
	// now read in the data
  string strSource = std::string((std::istreambuf_iterator<char>(shaderSource)), std::istreambuf_iterator<char>());
  shaderSource.close();
  strSource+="\0";
	//pass the code to OGL
	const char* data=strSource.c_str();
  glShaderSource(iHandle, 1, &data, NULL);
	return true;
}

void CGLSLProgram::recompileShader(std::string strFileName, SHADERTYPE typeShader)
{
	glDetachShader(m_uIdProgram, m_vIdShader[typeShader]);
	loadShader(strFileName, typeShader);
	glAttachShader(m_uIdProgram, m_vIdShader[typeShader]);
	//link the program
	glLinkProgram(m_uIdProgram);
	checkLinkingErrors();
	glDeleteShader(m_vIdShader[typeShader]);
	std::cout << "de pinga" << std::endl;
}

void CGLSLProgram::loadShader(std::string strFileName, SHADERTYPE typeShader)
{
	GLuint hShader = 0;
	GLint status;

	//Create shader object
	switch (typeShader)
  {
    case VERTEX : { hShader = glCreateShader(GL_VERTEX_SHADER); break; }
    case FRAGMENT : { hShader = glCreateShader(GL_FRAGMENT_SHADER); break; }
    case GEOMETRY : { hShader = glCreateShader(GL_GEOMETRY_SHADER); break; }
	case TESSELATION : { hShader = 0; std::cerr<<"not implemented.... yet :-)" << std::endl; }
  }

	if(loadShaderFile(strFileName, hShader))
	{
		//now compile the shader
		glCompileShader(hShader);
		glGetShaderiv(hShader, GL_COMPILE_STATUS, &status);
		if(status == GL_FALSE)
		{
			char infoLog[1024];
			glGetShaderInfoLog(hShader, 1024, NULL, infoLog);
			cout << "The shader at " << strFileName.c_str() << " failed to compile with the following errors:" << endl 
			<< infoLog << endl;
			glDeleteShader(hShader);
		}
		else	//here, everything is OK
		{
			cout << "The shader at " << strFileName.c_str() << " was compiled without errors." << endl;
			m_vIdShader[typeShader] = hShader;
		}
	}
	else
	{
		std::cerr<< "something wrong loading the shader located in " << strFileName.c_str() << "." << std::endl;
		glDeleteShader(hShader);
	}
}

void CGLSLProgram::checkLinkingErrors()
{
	GLint infologLength = 0;
	glGetProgramiv(m_uIdProgram, GL_INFO_LOG_LENGTH, &infologLength);
  //std::cerr<<"Link Log Length "<<infologLength<<"\n";

  if(infologLength > 1)
  {
    char *infoLog = new char[infologLength];
    GLint charsWritten  = 0;

    glGetProgramInfoLog(m_uIdProgram, infologLength, &charsWritten, infoLog);

    std::cerr<<infoLog<<std::endl;
    delete [] infoLog;
    glGetProgramiv(m_uIdProgram, GL_LINK_STATUS, &infologLength);
    if(infologLength == GL_FALSE)
    {
      std::cerr<<"Program link failed exiting \n";
      exit(EXIT_FAILURE);
    }
  }
}

void CGLSLProgram::create()
{
	m_uIdProgram = glCreateProgram();
	//attach the shaders
	if(m_vIdShader[VERTEX] > 0)
		glAttachShader(m_uIdProgram, m_vIdShader[VERTEX]);
	if(m_vIdShader[FRAGMENT] > 0)
		glAttachShader(m_uIdProgram, m_vIdShader[FRAGMENT]);
	if(m_vIdShader[GEOMETRY] > 0)
		glAttachShader(m_uIdProgram, m_vIdShader[GEOMETRY]);
		//delete the shaders
	glDeleteShader(m_vIdShader[VERTEX]);
	glDeleteShader(m_vIdShader[FRAGMENT]);
	glDeleteShader(m_vIdShader[GEOMETRY]);
	checkLinkingErrors();
}

void CGLSLProgram::create_link()
{
	m_uIdProgram = glCreateProgram();
	//attach the shaders
	if(m_vIdShader[VERTEX] > 0)
		glAttachShader(m_uIdProgram, m_vIdShader[VERTEX]);
	if(m_vIdShader[FRAGMENT] > 0)
		glAttachShader(m_uIdProgram, m_vIdShader[FRAGMENT]);
	if(m_vIdShader[GEOMETRY] > 0)
		glAttachShader(m_uIdProgram, m_vIdShader[GEOMETRY]);
	//link the program
	glLinkProgram(m_uIdProgram);
	//check errors on linking
	checkLinkingErrors();
	//delete the shaders
	glDeleteShader(m_vIdShader[VERTEX]);
	glDeleteShader(m_vIdShader[FRAGMENT]);
	glDeleteShader(m_vIdShader[GEOMETRY]);
}

void CGLSLProgram::link()
{
	//link the program
	glLinkProgram(m_uIdProgram);
	//check errors on linking
	checkLinkingErrors();
}

void CGLSLProgram::enable()
{
	glUseProgram(m_uIdProgram);
}

void CGLSLProgram::disable()
{
	glUseProgram(0);
}

GLuint CGLSLProgram::getId()
{
	return m_uIdProgram;
}

void CGLSLProgram::addAttribute(std::string strParName)
{
	m_mapVarShader[strParName] = glGetAttribLocation(m_uIdProgram, strParName.c_str());
}

GLint CGLSLProgram::getLocation(std::string strParName)
{
	std::map<string, GLint>::iterator it = m_mapVarShader.find(strParName);
	if(it == m_mapVarShader.end())
		return -1;
	else
		return m_mapVarShader[strParName];
}

void CGLSLProgram::addUniform(std::string strParName)
{
	m_mapVarShader[strParName] = glGetUniformLocation(m_uIdProgram, strParName.c_str());
}

void CGLSLProgram::addUniformSubroutine(std::string strParName, int iShaderType)
{
	//m_vRoutinesIds.front()
	m_mapVarShader[strParName] = glGetSubroutineUniformLocation(m_uIdProgram, iShaderType, strParName.c_str());
}

void CGLSLProgram::addSubroutine(std::string strFunctionName, unsigned int iShaderType)
{
	//m_vRoutinesIds.push_back(glGetSubroutineIndex(m_uIdProgram, iShaderType, strFunctionName.c_str()));
	m_mapSubroutines[strFunctionName] = glGetSubroutineUniformLocation(m_uIdProgram, iShaderType, strFunctionName.c_str());
}

//only for 1 subroutine at time
void CGLSLProgram::setSubroutine(std::string strUniformName, std::string strSubRoutine, int iShaderType)
{
	int index = getLocation(strUniformName);
	if(index > -1)
	{
		GLuint routine_index = glGetSubroutineIndex(m_uIdProgram, iShaderType, strSubRoutine.c_str());
		glUniformSubroutinesuiv(iShaderType, 1, &routine_index);
	}
}