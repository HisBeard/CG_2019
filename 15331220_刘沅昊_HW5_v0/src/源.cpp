#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <cmath>
#include <vector>
#include "shader.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 15.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool isFirstInFPS = false;

// timer
float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间
int choose = 3;

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
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
	// world space positions of our cubes
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -5.0f),
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

	float ortho[4] = { -15.0f, 15.0f, -15.0f, 15.0f }; 
	float perspect[3] = { 45.0f, 1.0f, 1.0f };
	float near_far[2] = { 0.1f, 100.0f };

	// --------------------------
	// 渲染循环
	// --------------------------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

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
			ImGui::RadioButton("Ortho projection", &choose, 0);
			ImGui::RadioButton("Perspective projection", &choose, 1);
			ImGui::Separator();
			ImGui::Text("Cube Basic2");
			ImGui::RadioButton("View changing", &choose, 2);
			ImGui::Separator();
			ImGui::Text("Cube Bonus");
			ImGui::RadioButton("Class camera", &choose, 3);

			switch (choose)
			{
			case 0:
				ImGui::InputFloat4("Othro paras", ortho, 2);
				ImGui::InputFloat2("Near and Far", near_far, 2);
				break;
			case 1:
				ImGui::InputFloat3("Perspect paras", perspect, 2);
				ImGui::InputFloat2("Near and Far", near_far, 2);
				break;
			case 2:
				ImGui::Text("View Changing");
				break;
			case 3:
				isFirstInFPS = true;
				break;
			default:
				break;
			}

			ImGui::End();
		}


		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);

		float Radius = 20.0f;
		float camX = sin(glfwGetTime()) * Radius;
		float camZ = cos(glfwGetTime()) * Radius;

		switch (choose) {
		case 0:
			model = glm::translate(model, glm::vec3(-1.5f, 0.5f, -1.5f));
			my_shader.setMat4("model", glm::value_ptr(model));
			view = glm::lookAt(
				glm::vec3(10.0f, 10.0f, 20.0f),
				glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, 1.0f, 0.0f)
			);
			projection = glm::ortho(ortho[0], ortho[1], ortho[2], ortho[3], near_far[0], near_far[1]);
			break;
		case 1:
			model = glm::translate(model, glm::vec3(-1.5f, 0.5f, -1.5f));
			my_shader.setMat4("model", glm::value_ptr(model));
			view = glm::lookAt(
				glm::vec3(10.0f, 10.0f, 20.0f),
				glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, 1.0f, 0.0f)
			);
			projection = glm::perspective(perspect[0], perspect[1] / perspect[2], near_far[0], near_far[1]);
			break;
		case 2:
			for (unsigned int i = 0; i < 3; i++)
			{
				// calculate the model matrix for each object and pass it to shader before drawing
				glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
				model = glm::translate(model, cubePositions[i]);
				float angle = 20.0f * i;
				model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
				my_shader.setMat4("model", glm::value_ptr(model));

				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
			view = glm::lookAt(glm::vec3(camX, 1.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			projection = glm::perspective(45.0f, (float)SCR_WIDTH / (float)SCR_WIDTH, 0.1f, 100.0f);
			break;
		case 3:
			my_shader.setMat4("model", glm::value_ptr(model));
			if (isFirstInFPS) {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
			view = camera.GetViewMatrix();
			projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		default:
			break;
		}

		//激活程序，每个着色器调用和渲染调用都会使用这个程序对象
		my_shader.use();
		my_shader.setMat4("view", glm::value_ptr(view));
		my_shader.setMat4("projection", glm::value_ptr(projection));
		
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

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
	// 检查用户是否按下了返回键(Esc)，退出程序
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	// W 控制前进
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	// S 控制后退
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	// A 控制左移
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	// D 控制右移
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	// 空格 控制上升
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
	// 左Ctrl 控制下降
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);
	// C 改变模式
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		isFirstInFPS = false;
		choose = 0;
	}
}


// --------------------------
// 设置视口的维度
// --------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// 前两个参数是视口左下角的位置，后两个是视口的宽度和高度
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}