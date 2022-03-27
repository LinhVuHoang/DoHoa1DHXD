//khai báo và định thư viện stb_image
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

//include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//function
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

const GLuint WIDTH = 800, HEIGHT = 600;

const GLchar* vertexShaderSource = "#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"layout(location = 1) in vec3 aColor;\n"
"layout(location = 2) in vec2 aTexCoord;\n"  //vị trí thuộc tính đỉnh TexCoord là =2

"out vec3 ourColor;\n"
"out vec2 TexCoord;\n"

"uniform mat4 transform;\n"   //ma tran bien doi( bao gom: xoay, dịch chuyển, scale)
"void main()\n"
"{\n"
"	gl_Position = transform * vec4(aPos , 1.0f);\n"  ///gl_Position giá trị đỉnh đầu ra của vertex shader
"	ourColor = aColor;\n"
"	TexCoord = aTexCoord;\n"
"}\n";
const GLchar* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"

"in vec3 ourColor;\n"
"in vec2 TexCoord;\n"

"uniform sampler2D ourTexture;\n"

"void main()\n"
"{\n"
"	FragColor = texture(ourTexture, TexCoord)*vec4(ourColor,1.0f);\n"
"}\n";

int main(void)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Tạo 1 cửa sổ GLFWwindow 
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Only quạt", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	//Đặt biến glewExperimental về true  (bắt buộc)
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	// Định nghĩa kích thước viewport
	 // Define the viewport dimensions
	int widthW, heightW;
	glfwGetFramebufferSize(window, &widthW, &heightW);    // lấy kích thước framebuffer   (chú ý)
	glViewport(0, 0, widthW, heightW);;

	//khởi tạo shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// Check for compile time errors
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// Check for compile time errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Link shaders
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	
	//bước 1: khai báo vertex input (vertex data)
	GLfloat vertices[] = {
		// vị trí - position     
		0.0f,0.25f,0.0f,	0.0f,0.0,1.0f,		0.0f,0.0f,
		0.25f,0.25f,0.0f,	0.0f,0.0,1.0f,		1.0f,0.0f,
		0.25f,0.0f,0.0f,	0.0f,0.0,1.0f,		0.5f,1.0f,

		0.0f,0.25f,0.0f,	0.0f,0.0,1.0f,		0.0f,0.0f,
		0.25f,0.0f,0.0f,	0.0f,0.0,1.0f,		1.0f,0.0f,
		0.0f,-0.5f,0.0f,	0.0f,0.0,1.0f,		0.5f,1.0f
	};
	//Bước 2: Khởi tạo VBO, VAO
		//b2.1 VAO
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	//bind VAO
	glBindVertexArray(VAO);
	//b2.2 VBO
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // liên kết (bind) VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//position attribute 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//Color attribute 
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//texture coordinate attribute 
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind VBO , cho phép gọi hàm glVertexAttribPointer trong VBO
//unbind VAO
	glBindVertexArray(0);

	///Load và tạo 1 texture
	//khởi tạo texture
	GLuint texture_wood;
	glGenTextures(1, &texture_wood);
	glBindTexture(GL_TEXTURE_2D, texture_wood);
	//cài đặt tham số texture wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//cài đặt tham số texture filtering 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//tải image, tạo texture + mipmaps
	int width, height, nrChannels;
	unsigned char* image = stbi_load("texture_wood2.jpg", &width, &height, &nrChannels, 0);
	if (image)
	{
		///tạo mipmaps
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		//đưa ra thông báo lỗi load image
		std::cout << "Failed to Load texture" << std::endl;
	}
	//delete image 
	stbi_image_free(image);

	//unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);  //unbind texture
	//Game Loop
	while (!glfwWindowShouldClose(window))
	{
		// check sự kiện  (ấn nút bàn phím, chuột,...)
		glfwPollEvents();
		//Render
		//xóa color buffer
		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindTexture(GL_TEXTURE_2D, texture_wood);
		//Buoc 3 Vẽ hình 1 hình tam giác

			///xác định ma trận biến đổi (các bạn muốn biến đổi đối tượng như nào )
		//hinh 1
		glm::mat4 Trans1 = glm::mat4(1.0f);
		glm::mat4 TransT11 = glm::mat4(1.0f);
		glm::mat4 TransR11 = glm::mat4(1.0f);
		glm::mat4 TransT21 = glm::mat4(1.0f);
		//dịch chuyển theo x,y,z lần lượt là 0.5f,-0.5f,0.0f
		TransT11 = glm::translate(TransT11, glm::vec3(0.0f, 0.5f, 0.0f)); 
		   // scale nhỏ đi 
		Trans1 = glm::scale(Trans1, glm::vec3(0.5f, 0.5f, 0.5f)); 
		// roate 
		TransR11 = glm::rotate(TransR11, (float)glfwGetTime() / 20.0f * 60.0f, glm::vec3(0.0f, 0.0f, 1.0f)); ///Trans=Trans*Rotate

		TransT21 = glm::translate(TransT21, glm::vec3(0.0f, -0.5f, 0.0f)); 

		Trans1 = Trans1 * TransT21 * TransR11 * TransT11;
		////Tìm vị trí của uniform tên là "transform"
		GLuint UniformLocation_Transform = glGetUniformLocation(shaderProgram , "transform");

		glUniformMatrix4fv(UniformLocation_Transform, 1, GL_FALSE, glm::value_ptr(Trans1));

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		//hinh 2
		glm::mat4 Trans2 = glm::mat4(1.0f);
		glm::mat4 TransT12 = glm::mat4(1.0f);
		glm::mat4 TransR12 = glm::mat4(1.0f);
		glm::mat4 TransT22 = glm::mat4(1.0f);
		//dịch chuyển theo x,y,z lần lượt là 0.5f,-0.5f,0.0f
		TransT12 = glm::translate(TransT12, glm::vec3(0.0f, 0.5f, 0.0f));
		   // scale nhỏ đi 
		Trans2 = glm::scale(Trans2, glm::vec3(0.5f, 0.5f, 0.5f)); 
		//	// roate 
		TransR12 = glm::rotate(TransR12, (float)glfwGetTime() / 20.0f * 60.0f + glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)); ///Trans=Trans*Rotate

		TransT22 = glm::translate(TransT22, glm::vec3(0.0f, -0.5f, 0.0f)); 

		Trans2 = Trans2 * TransT22 * TransR12 * TransT12;
		////Tìm vị trí của uniform tên là "transform" 
		UniformLocation_Transform = glGetUniformLocation(shaderProgram , "transform");
	
		glUniformMatrix4fv(UniformLocation_Transform, 1, GL_FALSE, glm::value_ptr(Trans2));

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		//hinh 3
		glm::mat4 Trans3 = glm::mat4(1.0f);
		glm::mat4 TransT13 = glm::mat4(1.0f);
		glm::mat4 TransR13 = glm::mat4(1.0f);
		glm::mat4 TransT23 = glm::mat4(1.0f);
		//dịch chuyển theo x,y,z lần lượt là 0.5f,-0.5f,0.0f
		TransT13 = glm::translate(TransT13, glm::vec3(0.0f, 0.5f, 0.0f)); 
		   // scale nhỏ đi
		Trans3 = glm::scale(Trans3, glm::vec3(0.5f, 0.5f, 0.5f)); 

		TransR13 = glm::rotate(TransR13, (float)glfwGetTime() / 20.0f * 60.0f + glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f)); ///Trans=Trans*Rotate

		TransT23 = glm::translate(TransT23, glm::vec3(0.0f, -0.5f, 0.0f)); 

		Trans3 = Trans3 * TransT23 * TransR13 * TransT13;
		////Tìm vị trí của uniform tên là "transform"
		UniformLocation_Transform = glGetUniformLocation(shaderProgram , "transform");
		glUniformMatrix4fv(UniformLocation_Transform, 1, GL_FALSE, glm::value_ptr(Trans3));

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		//hinh 4
		glm::mat4 Trans4 = glm::mat4(1.0f);
		glm::mat4 TransT14 = glm::mat4(1.0f);
		glm::mat4 TransR14 = glm::mat4(1.0f);
		glm::mat4 TransT24 = glm::mat4(1.0f);
		//dịch chuyển theo x,y,z lần lượt là 0.5f,-0.5f,0.0f
		TransT14 = glm::translate(TransT14, glm::vec3(0.0f, 0.5f, 0.0f)); ///Trans=Trans*glm::vec3(0.0f, 0.5f,0.0f)
		   // scale nhỏ đi 0.3 lần     
		Trans4 = glm::scale(Trans4, glm::vec3(0.5f, 0.5f, 0.5f)); ///Trans=Trans*glm::vec3(0.5f, 0.5f, 0.5f)
		//	// roate 90 độ quanh trục y
		TransR14 = glm::rotate(TransR14, (float)glfwGetTime() / 20.0f * 60.0f + glm::radians(270.0f), glm::vec3(0.0f, 0.0f, 1.0f)); ///Trans=Trans*Rotate

		TransT24 = glm::translate(TransT24, glm::vec3(0.0f, -0.5f, 0.0f)); ///Trans=Trans*glm::vec3(0.0f, -0.5f,0.0f)

		Trans4 = Trans4 * TransT24 * TransR14 * TransT14;
		////Tìm vị trí của uniform tên là "transform" 
		UniformLocation_Transform = glGetUniformLocation(shaderProgram , "transform");
		
		glUniformMatrix4fv(UniformLocation_Transform, 1, GL_FALSE, glm::value_ptr(Trans4));

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0); 
		
		//hinh 5
		glm::mat4 Trans5 = glm::mat4(1.0f);

		Trans5 = glm::scale(Trans5, glm::vec3(0.5f, 0.5f, 0.5f)); ///Trans=Trans*glm::vec3(0.5f, 0.5f, 0.5f)

		////Tìm vị trí của uniform tên là "transform" 
		UniformLocation_Transform = glGetUniformLocation(shaderProgram, "transform");

		glUniformMatrix4fv(UniformLocation_Transform, 1, GL_FALSE, glm::value_ptr(Trans5));

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		///swap
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);


	//Terminate GLFW, xóa và dọn dẹp tài nguyên sau khi thoát
	glfwTerminate();

	return 0;

}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	std::cout << key << std::endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}