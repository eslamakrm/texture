#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void moveTriangle(float Vertices[]);
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
char *vertexShaderSource = "#version 330 core\n                             \
	                         layout(location = 0)  in vec3 aPos;             \
							 layout(location = 1)  in vec3 aColor;			 \
							 out vec3 color  ;                               \
							 void main()                                     \
                             {                                               \
                             gl_Position = vec4(aPos, 1.0);					 \
							 color = aColor;						    	 \
                             }";
char *fragmentSahderSource = "#version 330 core\n                           \
	                            out vec4 FragColor;                          \
							    in vec3 color;							     \
								void main()								     \
								{											 \
								FragColor = vec4(color,1.0);     			 \
								}";
unsigned int vbo, vao;
using namespace std;
int vertexuniformLocation;
float vertices[] = { //with colors
	-0.5f, -0.5f, 0.2f, 1.0f , 0.0f , 0.0f,
	0.5f, -0.5f, 0.5f , 0.0f , 1.0f , 0.0f,
	0.0f, 0.5f, 0.3f,0.0f , 0.0f , 1.0f
};

int gradiantTriangle()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);




#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif




														 ///////////making buffer into GPU to save vertices to it////////////////////////
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}
	///////////////////////////////////////////////////////////////
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

	glCompileShader(vertexShader);
	int success;
	char infolog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infolog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infolog << endl;

	}
	// glfw window creation
	// --------------------
	unsigned int fragmentshader;
	fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentshader, 1, &fragmentSahderSource, NULL);
	glCompileShader(fragmentshader);
	glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentshader, 512, NULL, infolog);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infolog << endl;

	}

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentshader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infolog);
		cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infolog << endl;

	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentshader);
	///////////////////////////////////////////////////////////////
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo); //generate buffer with key vbo
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo); //bind the buffer with type array buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //copy vertices into the buffer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(vbo);








	// render loop
	// -----------




	while (!glfwWindowShouldClose(window))
	{

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(vbo);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();

		vertexuniformLocation = glGetUniformLocation(shaderProgram, "color");



	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	//if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	//{
	//	vertices[1] = 0.5f;
	//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //copy vertices into the buffer
	//}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		if (vertexuniformLocation != -1)
			glUniform4f(vertexuniformLocation, 1.0f, 0.0f, 0.0f, 1.0f);

	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		if (vertexuniformLocation != -1)
			glUniform4f(vertexuniformLocation, 0.0f, 1.0f, 0.0f, 1.0f);

	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		if (vertexuniformLocation != -1)
			glUniform4f(vertexuniformLocation, 0.0f, 0.0f, 1.0f, 1.0f);


}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void moveTriangle(float vertices[])
{

}