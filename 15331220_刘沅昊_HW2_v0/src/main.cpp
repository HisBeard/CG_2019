#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);


// GLSL顶点着色器的源代码
const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"   ourColor = aColor;\n"
"}\0";
// 片段着色器的源代码
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(ourColor, 1.0f);\n"
"}\n\0";

int main()
{
	// --------------------------
	// 实例化GLFW窗口
	// --------------------------
	glfwInit();  // 初始化GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  // 配置GLFW，第一个参数是很多枚举值
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);  // 第二个参数是第一个参数的值，整数
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// --------------------------
	// 创建一个窗口对象
	// --------------------------
	GLFWwindow* window = glfwCreateWindow(800, 800, "CG_hw2", NULL, NULL); // (width, height, name, , ,)
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);  // 通知GLFW窗口是上下文
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // 对窗口注册一个回调函数,在每次窗口大小被调整的时候被调用

	// --------------------------
	// 初始化GLAD
	// --------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// 设置ImGui上下文
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 150");

	// --------------------------
	// 顶点着色器
	// --------------------------
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);  // 创建一个着色器对象
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);  // 把这个着色器源码附加到着色器对象上
	// 要编译的着色器对象作为第一个参数,第二个参数指定了传递的源码字符串数量，这里只有一个。第三个参数是顶点着色器真正的源码
	glCompileShader(vertexShader);  // 编译这个着色器
	// 检测编译时错误
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// --------------------------
	// 片段着色器
	// --------------------------
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // 创建一个顶点着色器
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// 检测编译时错误
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// --------------------------
	// 链接着色器
	// --------------------------
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();  // 创建一个程序对象
	// 把之前编译的着色器附加到程序对象
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// 检测链接着色器程序是否失败
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	// 把着色器对象链接到程序对象以后，删除着色器对象
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// 初始化各种数据
	ImVec4 A_color = ImVec4(1.0f, 0.0f, 0.0f, 1.00f);
	ImVec4 B_color = ImVec4(1.0f, 0.0f, 0.0f, 1.00f);
	ImVec4 C_color = ImVec4(0.0f, 1.0f, 0.0f, 1.00f);
	ImVec4 D_color = ImVec4(0.0f, 0.0f, 1.0f, 1.00f);
	ImVec4 E_color = ImVec4(0.0f, 0.0f, 1.0f, 1.00f);
	ImVec4 F_color = ImVec4(0.0f, 1.0f, 0.0f, 1.00f);

	unsigned int indices[] = {
		0, 4, 5, // 第1个三角形
		1, 2, 3, // 第2个三角形
		0, 1, 4, // 第3个三角形
		1, 3, 4  // 第4个三角形
	};

	unsigned int VBO, VAO, EBO;
	bool ImGui = true;
	bool the_same_color = false;
	bool draw_rectangle = false;
	bool draw_trangle = false;
	bool bonus_draw_line = false;
	bool bonus_draw_another_trangle = false;

	// --------------------------
	// 渲染循环
	// --------------------------
	while (!glfwWindowShouldClose(window))
	{
		// 指定六个顶点，每个顶点都有一个3D位置以及颜色
		float vertices[] = {
			// 坐标              // 颜色
			-0.3f, 0.3f, 0.0f,  A_color.x, A_color.y, A_color.z,  // A
			0.0f, 0.3f, 0.0f, B_color.x, B_color.y, B_color.z,  // B
			0.6f, 0.3f, 0.0f,  C_color.x, C_color.y, C_color.z,   // C
			0.3f, -0.3f, 0.0f,  D_color.x, D_color.y, D_color.z,   // D
			0.0f, -0.3f, 0.0f,  E_color.x, E_color.y, E_color.z,   // E
			-0.6f, -0.3f, 0.0f,  F_color.x, F_color.y, F_color.z   // F
		};

		glGenVertexArrays(1, &VAO);  // 顶点数组对象
		glGenBuffers(1, &VBO);  // 顶点缓冲对象
		glGenBuffers(1, &EBO);  // 索引缓冲对象
		glBindVertexArray(VAO);  // 绑定VAO

		glBindBuffer(GL_ARRAY_BUFFER, VBO);  // 把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  // 把用户定义的数据复制到当前绑定缓冲

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);  // 把新创建的缓冲绑定到GL_ELEMENT_ARRAY_BUFFER目标上
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// 位置属性
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);  // 设置顶点属性指针
		glEnableVertexAttribArray(0);
		// 颜色属性
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		//glEnableVertexAttribArray(1);
		glUseProgram(shaderProgram);  // 创建一个程序对象


		// 创建ImGui
		glfwPollEvents();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Color Pad");

		ImGui::ColorEdit3("Basic triangle -- Point A color", (float*)&A_color);
		ImGui::ColorEdit3("Basic triangle -- Point B color", (float*)&B_color);
		ImGui::ColorEdit3("Basic triangle -- Point C color", (float*)&C_color);
		ImGui::ColorEdit3("Bonus triangle -- Point D color", (float*)&D_color);
		ImGui::ColorEdit3("Bonus triangle -- Point E color", (float*)&E_color);
		ImGui::ColorEdit3("Bonus triangle -- Point F color", (float*)&F_color);

		ImGui::Checkbox("Triangle", &draw_trangle);
		ImGui::Checkbox("Rectangle", &draw_rectangle);
		ImGui::Checkbox("Line", &bonus_draw_line);
		ImGui::Checkbox("Two triangles", &bonus_draw_another_trangle);
		ImGui::End();

		// 渲染窗口颜色
		int view_width, view_height;
		glfwGetFramebufferSize(window, &view_width, &view_height);
		glViewport(0, 0, view_width, view_height);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// 三角色带
		if (draw_trangle) {
			glEnableVertexAttribArray(1);
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		}

		// 画矩形
		if (draw_rectangle) {
			glEnableVertexAttribArray(1);
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
		}

		// 线段
		if (bonus_draw_line) {
			glEnableVertexAttribArray(1);
			glBindVertexArray(VAO);
			glDrawArrays(GL_LINE_STRIP, 0, 3);
		}

		// 多个三角形
		if (bonus_draw_another_trangle) {
			glEnableVertexAttribArray(1);
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		// 
		glfwSwapBuffers(window);

		// --------------------------
		// 释放/删除之前的分配的所有资源
		// --------------------------
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}

	glfwTerminate();
	return 0;
}

// --------------------------
// 实现一些输入控制,需要一个窗口以及一个按键作为输入
// --------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  // 检查用户是否按下了返回键(Esc)
		glfwSetWindowShouldClose(window, true);
}

// --------------------------
// 设置视口的维度
// --------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);  // 前两个参数是视口左下角的位置，后两个是视口的宽度和高度
}