#include<iostream>
//import glfw and glad
#include<glad/glad.h>
#include<GLFW/glfw3.h>

// Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
//Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";

int main() {
	
	//I was forced to write comments by the tutorial :(
	//init bruh
	glfwInit();


	//tells glfw what version it's using
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//tells opengl what profile we want to use
	//core profile == no backcompat
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	//vertex coordinates
	GLfloat vertices[] = {
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, //upper left
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
		0.0f, 0.7f * float(sqrt(3)) * 2 / 3, 0.0f
	};

	//creates a window sized 800 * 800 with the name My first triangle
	GLFWwindow* window = glfwCreateWindow(800, 800, "My first triangle", NULL, NULL);

	// error check if window creation failed
	if (window == NULL) {
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}

	//i have no idea what context means but I'm guessing
	glfwMakeContextCurrent(window);

	// Load GLAD because apparenlty that makes everything so much easier
	gladLoadGL();

	// set viewport
	glViewport(0, 0, 800, 800);


	//create vertex shader
	//vertex shader draws 3 points on a screen oh and get the reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//pull from vertex shader source above and compiles
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//create fragment shader and get the reference 
	//fragment shader interpolates whatever is in between the triangles and draws things on it is what I gathered
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// create shader program and get the reference
	GLuint shaderProgram = glCreateProgram();

	//attach the vertex & fragment shaders to the shader program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	// after attaching, link it to the shader program
	glLinkProgram(shaderProgram);

	//delete since both are now attached to the program
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//VBO Vertex Buffer Object
	// stores the stuff needed to draw the vertex
	// like the vertex data bro
	//VAO == Vertex Array Object
	// stores stuff needed to supply vertex data, stores both vertex data AND buffer objects providing vertex data arrays
	// apparenlty it just references the buffers but not their contents
	// think of it as a bundle of pointers towards the VBO
	// they go both hand in hand
	GLuint VBO, VAO;

	//generate the vertex arrays and reference at VAO
	glGenVertexArrays(1, &VAO);
	//generate the buffers and reference them at VBO
	glGenBuffers(1, &VBO);

	// binds a vertex array given a reference
	glBindVertexArray(VAO);
	// binds a buffer array given a reference, GL_ARRAY_BUFFER is the type
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//pipe in the vertices that I want to draw from earlier
	//GL_STATIC_DRAW means that the I will not change the vertices, STATIC
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	//specifies the format of the vertices that you're piping in
	//index 0, size of 3, the type (float), if it's normalized (no), how large it is, and a weird looking as void pointer
	//REFER TO VAO WHY THIS IS USEFUL
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//enable attribute array
	glEnableVertexAttribArray(0);

	//bind the VBO and the VAO to INDEX 0
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// set viewport color
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

	// clear buffer and introduce new color
	glClear(GL_COLOR_BUFFER_BIT);

	// says what it does on the tin
	glfwSwapBuffers(window);
	
	// check for window events (i.e if you close it)
	while (!glfwWindowShouldClose(window)) {
		//make the color like this
		glClearColor(0.07f, 0.013f, 0.17f, 1.0f);
		//clear the buffer and assing the new color
		glClear(GL_COLOR_BUFFER_BIT);
		//use use the shader program
		glUseProgram(shaderProgram);
		//bind vertex array so that opengl knows how to interpret the VBO
		glBindVertexArray(VAO);
		//draw le triangle
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//make sure to swap the buffers so that it actually shows
		glfwSwapBuffers(window);


		glfwPollEvents();
	}
	
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	//destroy when window closes and terminate
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}