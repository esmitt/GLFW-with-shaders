#define GLFW_DLL
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "GLSLProgram.h"
#include <stdlib.h>
#include <string>
#include <iostream>

#pragma comment(lib, "glfw3dll.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

using namespace std;

///< Only wrapping the glfw functions
namespace glfwFunc
{
	GLFWwindow* glfwWindow;
	const unsigned int WINDOW_WIDTH = 512;
	const unsigned int WINDOW_HEIGHT = 512;
	const float NCP = 0.01f;
	const float FCP = 52.f;
	const float fAngle = 45.f;
	string strNameWindow = "Hello GLFW";

	CGLSLProgram m_program;
	glm::mat4x4 mProjMatrix, mModelViewMatrix;
	GLuint m_idVAO;

	///< Function to build a simple triangle
	void buildTriangle()
	{
		float vfVertexT [] = {-0.6f, -0.4f, 0.f, 0.6f, -0.4f, 0.f, 0.f, 0.6f, 0.f};
		float vfColorT [] = {1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f};
		GLuint idVBO;

		glGenVertexArrays(1, &m_idVAO);
    glBindVertexArray(m_idVAO );		

		glGenBuffers(1, &idVBO);
    glBindBuffer(GL_ARRAY_BUFFER, idVBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vfVertexT) + sizeof(vfColorT), NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vfVertexT), vfVertexT);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vfVertexT), sizeof(vfColorT), vfColorT);
		
		glEnableVertexAttribArray(m_program.getLocation("vVertex"));
    glVertexAttribPointer(m_program.getLocation("vVertex"), 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0)); //Vertex
    glEnableVertexAttribArray(m_program.getLocation("vColor"));
    glVertexAttribPointer(m_program.getLocation("vColor"), 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vfVertexT))); //Colors
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	
	///
	/// Init all data and variables.
	/// @return true if everything is ok, false otherwise
	///
	bool initialize()
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glewExperimental = GL_TRUE;
		if(glewInit() != GLEW_OK) 
		{
			cout << "- glew Init failed :(" << endl;
			return false;
		}
		std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
		//load the shaders
		m_program.loadShader("shaders/basic.vert", CGLSLProgram::VERTEX);
    m_program.loadShader("shaders/basic.frag", CGLSLProgram::FRAGMENT);
    m_program.create_link();
    m_program.enable();
            m_program.addAttribute("vVertex");
            m_program.addAttribute("vColor");
            m_program.addUniform("mProjection");
            m_program.addUniform("mModelView");
    m_program.disable();
		buildTriangle();
		return true;
	}
	
	///< Callback function used by GLFW to capture some possible error.
	void errorCB(int error, const char* description)
	{
		cout << description << endl;
	}

	///
	/// The keyboard function call back
	/// @param window id of the window that received the event
	/// @param iKey the key pressed or released
	/// @param iScancode the system-specific scancode of the key.
	/// @param iAction can be GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT
	/// @param iMods Bit field describing which modifier keys were held down (Shift, Alt, & so on)
	///
	void keyboardCB(GLFWwindow* window, int iKey, int iScancode, int iAction, int iMods)
	{
		if (iAction == GLFW_PRESS)
		{
			switch (iKey)
			{
				case GLFW_KEY_ESCAPE:
				case GLFW_KEY_Q:
					glfwSetWindowShouldClose(window, GL_TRUE);
					break;
			}
		}
	}
	
	///< The resizing function
	void resizeCB(GLFWwindow* window, int iWidth, int iHeight)
	{
		if(iHeight == 0) iHeight = 1;
		float ratio = iWidth / float(iHeight);
		glViewport(0, 0, iWidth, iHeight);
		mProjMatrix = glm::perspective(fAngle, ratio, NCP, FCP);
	}

	///< The main rendering function.
	void draw()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.15f, 0.15f, 0.15f, 1.f);
		
		mModelViewMatrix = glm::translate(glm::mat4(), glm::vec3(0,0,-2.f)); 
		
		m_program.enable();
			glUniformMatrix4fv(m_program.getLocation("mModelView"), 1, GL_FALSE, glm::value_ptr(mModelViewMatrix));
			glUniformMatrix4fv(m_program.getLocation("mProjection"), 1, GL_FALSE, glm::value_ptr(mProjMatrix));
			glBindVertexArray(m_idVAO);
				glDrawArrays(GL_TRIANGLES, 0, 3);
			glBindVertexArray(0);
    m_program.disable();

		glfwSwapBuffers(glfwFunc::glfwWindow);
	}
	
	/// Here all data must be destroyed + glfwTerminate
	void destroy()
	{
		if(glIsVertexArray(m_idVAO)) glDeleteVertexArrays(1, &m_idVAO);
		glfwDestroyWindow(glfwFunc::glfwWindow);
		glfwTerminate();
	}
};

int main(int argc, char** argv)
{
	glfwSetErrorCallback(glfwFunc::errorCB);
	if (!glfwInit())	exit(EXIT_FAILURE);
	glfwFunc::glfwWindow = glfwCreateWindow(glfwFunc::WINDOW_WIDTH, glfwFunc::WINDOW_HEIGHT, glfwFunc::strNameWindow.c_str(), NULL, NULL);
	if (!glfwFunc::glfwWindow)
  {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
	glfwMakeContextCurrent(glfwFunc::glfwWindow);
	if(!glfwFunc::initialize()) exit(EXIT_FAILURE);
	glfwFunc::resizeCB(glfwFunc::glfwWindow, glfwFunc::WINDOW_WIDTH, glfwFunc::WINDOW_HEIGHT);	//just the 1st time
	glfwSetKeyCallback(glfwFunc::glfwWindow, glfwFunc::keyboardCB);
	glfwSetWindowSizeCallback(glfwFunc::glfwWindow, glfwFunc::resizeCB);
	// main loop!
	while (!glfwWindowShouldClose(glfwFunc::glfwWindow))
  {
		glfwFunc::draw();
    glfwPollEvents();	//or glfwWaitEvents()
  }
	glfwFunc::destroy();
	return EXIT_SUCCESS;
}