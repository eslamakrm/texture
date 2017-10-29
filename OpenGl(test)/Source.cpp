#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <list>
#include <tuple>
#include <algorithm>
#include <time.h>
#include <random>
#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>

#include <learnopengl/shader_s.h>



#define SNAKEHEADX vertex[0]
#define SNAKEHEADY vertex[1]

using namespace std;
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 800;
enum DIRECTIONS {
	UP, DOWN, LEFT, RIGHT
};
int snakedirection = LEFT;
//////////////////////////
void texture();
void updateSnakeBody(int, float, float);
void gradiantTriangle();
void colorChangingTriangle();
////////////////////////////
void mouseButton_callback(GLFWwindow* window, int button, int action, int mods);
void MousePosition_callback(GLFWwindow* window, double x, double y);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void windowSize_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void DrawSquare();
void MouseLine(bool);
void snakeGame();
unsigned int vbo, vao;
int vertexuniformLocation;
GLFWwindow* window;
bool leftClickPressed = FALSE;
bool leftClickReleased = FALSE;
int SCRwidth = SCR_WIDTH, SCRheight = SCR_HEIGHT;

int arrayindex = 0;
GLfloat vertex[20000000];

int main()
{
#pragma region Window



	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3.3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	///////////making buffer into GPU to save vertices to it///////////////////////
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
#pragma endregion

	for (int i = 0; i < 10000; i++)
	{
		vertex[i] = 2.0;
	}
	//bool usingPoints = false; //replace with true to use points instead of lines
	//gradiantTriangle();
	//colorChangingTriangle();
	//DrawSquare();
	//MouseLine(usingPoints);
	//snakeGame();
	texture();
	return 0;
}


void gradiantTriangle()
{
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

	float vertices[] = { //with colors
		-0.5f, -0.5f, 0.2f, 1.0f , 0.0f , 0.0f,
		0.5f, -0.5f, 0.5f , 0.0f , 1.0f , 0.0f,
		0.0f, 0.5f, 0.3f,0.0f , 0.0f , 1.0f
	};


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


}

void colorChangingTriangle()
{
	char *vertexShaderSource = "#version 330 core\n                             \
	                         layout(location = 0)  in vec3 aPos;             \
							 			                                     \
							                               \
							 void main()                                     \
                             {                                               \
                             gl_Position = vec4(aPos, 1.0);		 \
												    	 \
                             }";
	char *fragmentSahderSource = "#version 330 core\n                           \
	                            out vec4 FragColor ;                          \
							    uniform vec4 color = {0.0f,1.0f,1.0f,1.0f};					     \
								void main()								     \
								{											 \
								FragColor = color;     			 \
								}";

	float vertices[] = {
		-0.5f, -0.5f, 0.2f,
		0.5f, -0.5f, 0.5f ,
		0.0f, 0.5f, 0.3f,

	};




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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


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
}

void DrawSquare()
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	char *vertexShaderSource = "#version 330 core\n                             \
	                         layout(location = 0)  in vec3 aPos;             \
							 			                                     \
							                               \
							 void main()                                     \
                             {                                               \
                             gl_Position = vec4(aPos, 1.0);		 \
												    	 \
                             }";

	char *fragmentSahderSource = "#version 330 core\n                           \
	                            out vec4 FragColor ;                          \
							    uniform vec4 color = {0.0f,1.0f,1.0f,1.0f};					     \
								void main()								     \
								{											 \
								FragColor = color;     			 \
								}";


	float vertices[] = {
		0.5f, 0.5f, 0.0f, // top right
		0.5f, -0.5f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, // bottom left
		-0.5f, 0.5f, 0.0f // top left
	};

	unsigned int indices[] = { // note that we start from 0!
		0, 1, 3, // first triangle
		1, 2, 3 // second triangle
	};

	unsigned int EBO;

	///////////////////////////////////////////////////

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

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	glBindVertexArray(vbo);

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
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();

		vertexuniformLocation = glGetUniformLocation(shaderProgram, "color");



	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();

	//////////////////////////////////////////////////
}

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

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && snakedirection != DOWN)
		snakedirection = UP;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && snakedirection != UP)
		snakedirection = DOWN;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && snakedirection != RIGHT)
		snakedirection = LEFT;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && snakedirection != LEFT)
		snakedirection = RIGHT;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void MouseLine(bool usingPoints)
{

	glfwSetWindowSizeCallback(window, windowSize_callback);
	glfwSetCursorPosCallback(window, MousePosition_callback);
	glfwSetMouseButtonCallback(window, mouseButton_callback);

	//const char *vertexShaderSource = "#version 330 core\n"
	//	"layout (location = 0) in vec3 aPos;\n"
	//	"void main()\n"
	//	"{\n"
	//	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	//	"}\0";
	//const char *fragmentShaderSource = "#version 330 core\n"
	//	"out vec4 FragColor;\n"
	//	"uniform vec4 color = {0.0f,1.0f,1.0f,1.0f};"
	//	"void main()\n"
	//	"{\n"
	//	"   FragColor = color;\n"
	//	"}\n\0";

	//glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
	//unsigned int vertexShader;
	//vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

	//glCompileShader(vertexShader);
	//int success;
	//char infolog[512];
	//glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	//if (!success)
	//{
	//	glGetShaderInfoLog(vertexShader, 512, NULL, infolog);
	//	cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infolog << endl;

	//}
	//// glfw window creation
	//// --------------------
	//unsigned int fragmentshader;
	//fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
	//glShaderSource(fragmentshader, 1, &fragmentShaderSource, NULL);
	//glCompileShader(fragmentshader);
	//glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &success);

	//if (!success)
	//{
	//	glGetShaderInfoLog(fragmentshader, 512, NULL, infolog);
	//	cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infolog << endl;

	//}

	//unsigned int shaderProgram;
	//shaderProgram = glCreateProgram();
	//glAttachShader(shaderProgram, vertexShader);
	//glAttachShader(shaderProgram, fragmentshader);
	//glLinkProgram(shaderProgram);
	//glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	//if (!success)
	//{
	//	glGetProgramInfoLog(shaderProgram, 512, NULL, infolog);
	//	cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infolog << endl;

	//}

	//glDeleteShader(vertexShader);
	//glDeleteShader(fragmentshader);
	///////////////////////////////////////////////////////////////

	glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, 1);
	unsigned int VBO, VAO;

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, arrayindex, vertex, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind


	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.

	glEnable(GL_PROGRAM_POINT_SIZE);
	glPointSize(20);


	//glVertexAttribI2i(vbo, 100, 200);

	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	list<int>releaseTime;
	while (!glfwWindowShouldClose(window))
	{

		processInput(window);
		//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// draw our first triangle
		//glUseProgram(shaderProgram);
		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
								//glLineWidth(100);
		glEnable(GL_LINE_SMOOTH);


		//glDrawArrays(GL_POINTS, 0, arrayindex);
		int i;
		for (i = 1; i<arrayindex / 2; i++)
			if (usingPoints)
				glDrawArrays(GL_POINTS, i, 2);
			else
				glDrawArrays(GL_LINES, i, 2);

		// glBindVertexArray(0); // no need to unbind it every time 

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
		if (leftClickPressed)
		{
			double x, y;
			//glfwGetWindowSize(window, &SCRwidth, &SCRheight);
			glfwGetCursorPos(window, &x, &y);

			vertex[arrayindex++] = (x - SCRwidth / 2) / (SCRwidth / 2);    //400 = window width/2
			vertex[arrayindex++] = (SCRheight / 2 - y) / (SCRheight / 2);   //400 = window height/2
		}
		if (leftClickReleased)
			releaseTime.push_back(arrayindex);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------


		/*	vertexuniformLocation = glGetUniformLocation(shaderProgram, "color");
		if (lineVertices.size()  < 6)
		continue;
		glDrawArrays(GL_LINES, 0, lineVertices.size()/3);*/


	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();




}

void MousePosition_callback(GLFWwindow* window, double x, double y)
{

	/*if (glfwGetKey(window,GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
	cout << x;
	}
	else
	{
	cout << glfwGetKey(window, GLFW_MOUSE_BUTTON_LEFT);
	}
	leftClickPressed = false;
	*/
}

void mouseButton_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		cout << "Pressed";
		if (action == GLFW_PRESS)
			leftClickPressed = true;
		else
			leftClickPressed = false;
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		leftClickReleased = true;
		leftClickPressed = false;
	}
	else
		leftClickReleased = false;




	//leftClickPressed = true;

	//glfwGetCursorPos(window, &x, &y);
	//cout << "X:" << x <<"Y:"<<y<<endl;
	//
	//glfwPollEvents();





}

void windowSize_callback(GLFWwindow* window, int width, int height)
{
	SCRheight = height;
	SCRwidth = width;
}


void snakeGame()
{
	unsigned int VBO, VAO,EBO;

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, arrayindex, vertex, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	vertex[200] = 0.8f;
	vertex[201] = 0.8f;

	vertex[202] = 0.8f;
	vertex[203] = -0.8f;

	vertex[204] = -0.8f;
	vertex[205] = -0.8f;

	vertex[206] = -0.8f;
	vertex[207] = 0.8f;


	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	unsigned int indices[] = { // note that we start from 0!
		 100,101,
		 102,103,
		 100,103,
		 101,102
		
	};

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	
	glEnable(GL_PROGRAM_POINT_SIZE);
	glPointSize(5);
	glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, 0);
	//glVertexAttribI2i(vbo, 100, 200);

	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	float xPos = 0.0f;
	float yPos = 0.0f;
	int snakelen = 0;
	vertex[snakelen++] = xPos;
	vertex[snakelen++] = yPos;
	vertex[snakelen++] = 0.01;
	vertex[snakelen++] = yPos;


	long startTime = clock();
	long BallStart = clock();
	float BallXpos;
	float BallYpos;

	BallXpos = 0.1f;
	BallYpos = 0.1f;
	vertex[100] = 0.1f;
	vertex[101] = 0.1f;
	
	bool flag = true;
	srand(time(NULL));
	while (!glfwWindowShouldClose(window))
	{
		
		processInput(window);
		//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// draw our first triangle
		//glUseProgram(shaderProgram);
		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
								//glLineWidth(100);
	
		long endTime = clock();
		if (endTime - startTime >= 50)
		{
			switch (snakedirection)
			{
			case UP:
				yPos = yPos + 0.01f;
				break;
			case DOWN:
				yPos = yPos - 0.01f;
				break;
			case LEFT:
				xPos = xPos - 0.01f;
				break;
			case RIGHT:
				xPos = xPos + 0.01f;
				break;
			default:
				break;
			}
			updateSnakeBody(snakelen, xPos, yPos);
			
			startTime = endTime;
		}
		//glDrawArrays(GL_POINTS, 0, arrayindex);

		/*	switch (snakedirection)
		{
		case UP:

		default:
		break;
		}
		*/
		/////////////////////////////////// die conditions //////////////////////////////////////
		for (int ii = 2; ii < snakelen; ii += 2) {
			if (SNAKEHEADX == vertex[ii] && SNAKEHEADY == vertex[ii + 1]) {
				return;
			}
		}
		
		if (SNAKEHEADX >= 0.799f || SNAKEHEADX <= -0.799f || SNAKEHEADY >= 0.799f || SNAKEHEADY <= -0.799f)
			return;

	 ///////////////////////////////////////////////////////////////////////////////////////////
		if ((SNAKEHEADX+0.01>= BallXpos && SNAKEHEADX - 0.01 <= BallXpos && SNAKEHEADY + 0.01 >= BallYpos && SNAKEHEADY - 0.01 <= BallYpos) )
		{
		
		
		
			float newPosX;
			float newPosY;
			float newPosX2;
			float newPosY2;
			if (vertex[snakelen - 1] == vertex[snakelen - 3]) //y changing
			{
				if (vertex[snakelen - 1] > vertex[snakelen - 3])
				{
					newPosX = vertex[snakelen - 2];
					newPosY = vertex[snakelen - 1] + 0.01f;
					newPosX2 = vertex[snakelen - 2];
					newPosY2 = vertex[snakelen - 1] + 0.02f;
				}
				else
				{
					newPosX = vertex[snakelen - 2];
					newPosY = vertex[snakelen - 1] - 0.01f;
					newPosX2 = vertex[snakelen - 2];
					newPosY2 = vertex[snakelen - 1] - 0.02f;
				}
			}
			else //x changing
			{
				if (vertex[snakelen - 1] > vertex[snakelen - 3])
				{
					newPosY = vertex[snakelen - 1];
					newPosX = vertex[snakelen - 2] + 0.01f;
					newPosY2 = vertex[snakelen - 2];
					newPosX2 = vertex[snakelen - 1] + 0.02f;
				}
				else
				{
					newPosY = vertex[snakelen - 1];
					newPosX = vertex[snakelen - 2] - 0.01f;
					newPosY2 = vertex[snakelen - 2];
					newPosX2 = vertex[snakelen - 1] - 0.02f;
				}
			}
		
			vertex[snakelen++] = newPosX;
			vertex[snakelen++] = newPosY;
			vertex[snakelen++] = newPosX;
			vertex[snakelen++] = newPosY;
			BallXpos = ((float(rand()) / float(RAND_MAX)) * (1.5)) - 0.8;
			BallYpos = ((float(rand()) / float(RAND_MAX)) * (1.5)) - 0.8;
			vertex[100] = BallXpos;
			vertex[101] = BallYpos;
		}

		glDrawArrays(GL_POINTS, 0, 300);
		glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, 0);

		// glBindVertexArray(0); // no need to unbind it every time 

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------


		/*	vertexuniformLocation = glGetUniformLocation(shaderProgram, "color");
		if (lineVertices.size()  < 6)
		continue;
		glDrawArrays(GL_LINES, 0, lineVertices.size()/3);*/


	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();



}


void updateSnakeBody(int len,float xpos,float ypos)
{
	for (int i = len+1; i >= 0; i--)
	{
		vertex[i + 2] = vertex[i];
		vertex[i + 1] = vertex[i - 1];
	}
	SNAKEHEADX = xpos;
	SNAKEHEADY = ypos;
	
}



void texture()
{



	
		// build and compile our shader zprogram
		// ------------------------------------
		Shader ourShader("4.1.shader.vs", "4.1.shader.fs");

		// set up vertex data (and buffer(s)) and configure vertex attributes
		// ------------------------------------------------------------------
		float vertices[] = {
			// positions          // colors           // texture coords
			0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
			0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
		};
		unsigned int indices[] = {
			0, 1, 3, // first triangle
			1, 2, 3  // second triangle
		};
		unsigned int VBO, VAO, EBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		stbi_set_flip_vertically_on_load(true);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// texture coord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);


		// load and create a texture 
		// -------------------------
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
											   // set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// load image, create texture and generate mipmaps
		int width, height, nrChannels;
		// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
		//unsigned char *data = stbi_load(FileSystem:getPath("C:\\Users\\Soha\\source\\repos\\Project1\\Project1\\n.jpg").c_str(), &width, &height, &nrChannels, 0);

		unsigned char *data = stbi_load("n.JPG", &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);


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

			// bind Texture
			glBindTexture(GL_TEXTURE_2D, texture);

			// render container
			ourShader.use();
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
			// -------------------------------------------------------------------------------
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		// optional: de-allocate all resources once they've outlived their purpose:
		// ------------------------------------------------------------------------
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);

		// glfw: terminate, clearing all previously allocated GLFW resources.
		// ------------------------------------------------------------------
		glfwTerminate();
		
	}

	
