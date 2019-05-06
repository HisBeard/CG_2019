#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <cmath>
#include <vector>
#include "shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

int main()
{
	// --------------------------
	// 实例化GLFW窗口
	// --------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// --------------------------
	// 创建一个窗口对象
	// --------------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CG_HW4", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// --------------------------
	// 初始化GLAD
	// --------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	
	glClear(GL_DEPTH_BUFFER_BIT);
	// glDisable(GL_DEPTH_TEST); // 深度检测
	glEnable(GL_DEPTH_TEST);

	// 创造着色器程序
	Shader my_shader = Shader(".\\shader\\shader.vs", ".\\shader\\shader.fs");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// 背面
		-2.0f, -2.0f, -2.0f,  1.0f, 1.0f, 1.0f,
		2.0f, -2.0f, -2.0f,   1.0f, 1.0f, 1.0f,
		2.0f,  2.0f, -2.0f,   1.0f, 1.0f, 1.0f,
		2.0f,  2.0f, -2.0f,   1.0f, 1.0f, 1.0f,
		-2.0f,  2.0f, -2.0f,  1.0f, 1.0f, 1.0f,
		-2.0f, -2.0f, -2.0f,  1.0f, 1.0f, 1.0f,
		// 左
		-2.0f,  2.0f,  2.0f,  0.7f, 1.0f, 1.0f,
		-2.0f,  2.0f, -2.0f,  0.7f, 1.0f, 1.0f,
		-2.0f, -2.0f, -2.0f,  0.7f, 1.0f, 1.0f,
		-2.0f, -2.0f, -2.0f,  0.7f, 1.0f, 1.0f,
		-2.0f, -2.0f,  2.0f,  0.7f, 1.0f, 1.0f,
		-2.0f,  2.0f,  2.0f,  0.7f, 1.0f, 1.0f,
		// 正面
		-2.0f, -2.0f,  2.0f,  0.0f, 0.0f, 0.5f,
		2.0f, -2.0f,  2.0f,   0.0f, 0.0f, 0.5f,
		2.0f,  2.0f,  2.0f,   0.0f, 0.0f, 0.5f,
		2.0f,  2.0f,  2.0f,   0.0f, 0.0f, 0.5f,
		-2.0f,  2.0f,  2.0f,  0.0f, 0.0f, 0.5f,
		-2.0f, -2.0f,  2.0f,  0.0f, 0.0f, 0.5f,
		// 右
		2.0f,  2.0f,  2.0f,   0.1f, 0.5f, 0.0f,
		2.0f,  2.0f, -2.0f,   0.1f, 0.5f, 0.0f,
		2.0f, -2.0f, -2.0f,   0.1f, 0.5f, 0.0f,
		2.0f, -2.0f, -2.0f,   0.1f, 0.5f, 0.0f,
		2.0f, -2.0f,  2.0f,   0.1f, 0.5f, 0.0f,
		2.0f,  2.0f,  2.0f,   0.1f, 0.5f, 0.0f,
		// 下
		-2.0f, -2.0f, -2.0f,  1.0f, 0.7f, 0.1f,
		2.0f, -2.0f, -2.0f,   1.0f, 0.7f, 0.1f,
		2.0f, -2.0f,  2.0f,   1.0f, 0.7f, 0.1f,
		2.0f, -2.0f,  2.0f,   1.0f, 0.7f, 0.1f,
		-2.0f, -2.0f,  2.0f,  1.0f, 0.7f, 0.1f,
		-2.0f, -2.0f, -2.0f,  1.0f, 0.7f, 0.1f,
		// 上
		-2.0f,  2.0f, -2.0f,  0.5f, 0.1f, 0.7f,
		2.0f,  2.0f, -2.0f,   0.5f, 0.1f, 0.7f,
		2.0f,  2.0f,  2.0f,   0.5f, 0.1f, 0.7f,
		2.0f,  2.0f,  2.0f,   0.5f, 0.1f, 0.7f,
		-2.0f,  2.0f,  2.0f,  0.5f, 0.1f, 0.7f,
		-2.0f,  2.0f, -2.0f,  0.5f, 0.1f, 0.7f,
	};
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	//把顶点数据复制到缓冲的内存中
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//颜色属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	// 设置ImGui上下文
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 150");

	int choose = 1;
	int direction = 1;
	float translate = 0.3f;
	float scale_factor = 1.0f;

	// --------------------------
	// 渲染循环
	// --------------------------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		// 创建ImGui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		{
			ImGui::Begin("Console");

			ImGui::Text("Cube Basic1");
			ImGui::RadioButton("Translation", &choose, 1);
			ImGui::Separator();
			ImGui::Text("Cube Basic2");
			ImGui::RadioButton("Rotation", &choose, 2);
			ImGui::Separator();
			ImGui::Text("Cube Basic3");
			ImGui::RadioButton("Scaling", &choose, 3);
			ImGui::Separator();
			ImGui::Text("Cube Bonus");
			ImGui::RadioButton("Orbiting", &choose, 4);

			ImGui::End();
		}


		glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);

		std::vector<glm::mat4> star;
		star.assign(2, glm::mat4(1));

		view = glm::lookAt(glm::vec3(0.0f, 10.0f, 30.0f),
							glm::vec3(0.0f, 0.0f, 0.0f),
							glm::vec3(0.0f, 10.0f, 0.0f));
		projection = glm::perspective(45.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		//激活程序，每个着色器调用和渲染调用都会使用这个程序对象
		my_shader.use();
		my_shader.setMat4("view", glm::value_ptr(view));
		my_shader.setMat4("projection", glm::value_ptr(projection));
		switch (choose)
		{
		case 1:
			// 平移
			model = glm::translate(model, glm::vec3(translate, 0.0f, 0.0f));
			my_shader.setMat4("model", glm::value_ptr(model));
			translate = translate + 0.005f * direction;
			if (translate > 2.0f) direction = -1;
			if (translate < -2.0f) direction = 1;
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			break;
		case 2:
			// 旋转
			model = glm::rotate(model, (float)glfwGetTime() * 10.0f, glm::vec3(1.0f, 0.0f, 1.0f));
			my_shader.setMat4("model", glm::value_ptr(model));
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			break;
		case 3:
			// 缩放
			model = glm::scale(model, glm::vec3(scale_factor, scale_factor, scale_factor));
			my_shader.setMat4("model", glm::value_ptr(model));
			scale_factor = scale_factor + 0.005f * direction;
			if (scale_factor > 2.0f) direction = -1;
			if (scale_factor < 0.3f) direction = 1;
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			break;
		case 4:
			// 太阳只有自转
			star[0] = glm::rotate(star[0], (float)glfwGetTime() * 1.0f, glm::vec3(0.0f, 0.0f, 1.0f));
			// 地球有自转和公转
			star[1] = glm::rotate(star[1], (float)glfwGetTime() * 5.0f, glm::vec3(0.0, 0.0, 1.0));
			star[1] = glm::translate(star[1], glm::vec3(10.0, 10.0, 0.0));

			glBindVertexArray(VAO);
			my_shader.setMat4("model", glm::value_ptr(star[0]));
			glDrawArrays(GL_TRIANGLES, 0, 36);
			my_shader.setMat4("model", glm::value_ptr(star[1]));
			glDrawArrays(GL_TRIANGLES, 0, 36);
			break;
		default:
			break;
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// --------------------------
	// 释放/删除之前的分配的所有资源
	// --------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);


	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();

	return 0;
}

// --------------------------
// 实现一些输入控制,需要一个窗口以及一个按键作为输入
// --------------------------
void processInput(GLFWwindow *window)
{
	// 检查用户是否按下了返回键(Esc)
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}


// --------------------------
// 设置视口的维度
// --------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// 前两个参数是视口左下角的位置，后两个是视口的宽度和高度
	glViewport(0, 0, width, height);
}