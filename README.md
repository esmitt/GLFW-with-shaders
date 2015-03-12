# A simple Triangle with GLFW + GLEW

This is a basic template for using [GLFW], [GLEW] and the math library [GLM] over Windows and Linux. The project was generated over Visual Studio 2010, using the correct DLL library of GLFW for that compiler (instead the static version) in its 32-bit version. The code was compiled and executed on Windows 7 (actually, it really doesn't matter).
The project contains the basic draw, keyboard, initialize and resize function over GLFW and the usage of vertex and fragment shader. The rendering is using a vertex array object (VAO), drawing a simple colored triangle. Also, contains the definition of GLFW_DLL including the 

    #pragma comment
at beginning of code.

Functions and variables related to GLFW are encapsulated into a namespace called

    glfwFunc
(nothing special about the name). The code include all the libraries (yes, I know that is not efficient) to be used directly without download any additional library (based on the mentioned platform/libraries).

### Linux
To compile in Linux:

```sh
g++ GLSLProgram.cpp main.cpp -o main.o -lGLEW -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi
```
The final render is the same that I wrote for [this basic template of GLFW].

[GLFW]:http://www.glfw.org/
[GLEW]:http://glew.sourceforge.net/
[GLM]:http://glm.g-truc.net/"
[this basic template of GLFW]:https://github.com/esmitt/GLFW-Basic