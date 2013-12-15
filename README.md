<h1>A simple Triangle using GLFW and GLEW</h1>

<p>This is a basic template for using <a href="http://www.glfw.org/">GLFW</a>, < a href="http://glew.sourceforge.net/">GLEW</a> and the math library <a href="http://glm.g-truc.net/">GLM</a> over Windows. The projects was generated over Visual Studio 2010, using the correct DLL library of GLFW for this compiler (instead the static version) in its 32-bit version. The code was compiled and executed on Windows 7 (actually, it really doesn't matter).</p>
<p>The project contains the basic draw, keyboard, initialize and resize function over GLFW and the usage of vertex and fragment shader. The code render using a vertex array object (VAO), drawing a simple colored triangle. Also, contains the definition of GLFW_DLL and the usage of <code>#pragma comment</code>.</p>
<p>Functions and variables related to GLFW are encapsulated into a namespace called <code>glfwFunc</code> (nothing special about the name). The code include all the libraries (yes, I know that is not efficient) to be used directly without download any additional library (based on my specifications).</p>
<p>The final render is the same that I wrote for <a href="https://github.com/esmitt/GLFW-Basic">this basic template of GLFW</a>.</p>

GLFW-with-shaders
=================
