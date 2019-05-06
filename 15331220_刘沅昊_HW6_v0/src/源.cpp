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
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CG_HW6", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// tell GLFW to capture our mouse
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// --------------------------
	// 初始化GLAD
	// --------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	// glDisable(GL_DEPTH_TEST); // 深度检测
	glEnable(GL_DEPTH_TEST);

	// build and compile our shader zprogram
	// ------------------------------------
	Shader lightingShader = Shader(".//Shader//lighting.vs", ".//Shader//lighting.fs");
	Shader lampShader = Shader(".//Shader//lamp.vs", ".//Shader//lamp.fs");

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

	// first, configure the cube's VAO (and VBO)
	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// note that we update the lamp's position attribute's stride to reflect the updated buffer data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// 设置ImGui上下文
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 150");

	// camera paras
	float camPos[3] = { 2.0f, 5.0f, 20.0f };

	// light paras
	float light_pos[] = { 2.0f, 2.0f, 5.0f };
	float ambient = 0.1;
	float specular = 0.5;
	float shininess = 32;
	float diffuse = 1.0;

	int shading_choose = 1;
	bool is_lamp_moving = false;

	// --------------------------
	// 渲染循环
	// --------------------------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		// set up imgui 
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// 1. Show a simple window.
		// This is the menu of gui
		// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".


		{
			ImGui::Begin("Menu");
			ImGui::Text("Basic");
			ImGui::RadioButton("Phong Shading", &shading_choose, 1);
			ImGui::RadioButton("Gouraud Shading", &shading_choose, 2);
			ImGui::Separator();
			ImGui::Text("Camera position");
			ImGui::SliderFloat("camera.x", &camPos[0], -20.0f, 20.0f);
			ImGui::SliderFloat("camera.y", &camPos[1], -20.0f, 20.0f);
			ImGui::SliderFloat("camera.z", &camPos[2], -20.0f, 20.0f);
			ImGui::Separator();
			ImGui::Text("Shading factor");
			ImGui::SliderFloat("ambient", &ambient, 0.0f, 1.0f);
			ImGui::SliderFloat("diffuse", &diffuse, 0.0f, 3.0f);
			ImGui::SliderFloat("specular", &specular, 0.0f, 1.0f);
			ImGui::SliderFloat("shininess", &shininess, 0.0f, 256.0f);
			ImGui::Separator();
			ImGui::Text("Lamp position");
			ImGui::Checkbox("Lamp Moving", &is_lamp_moving);
			ImGui::SliderFloat("lamp.x", &light_pos[0], -20.0f, 20.0f);
			ImGui::SliderFloat("lamp.y", &light_pos[1], -20.0f, 20.0f);
			ImGui::SliderFloat("lamp.z", &light_pos[2], -20.0f, 20.0f);

			ImGui::End();
		}

		// render
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::vec3 lightPos(light_pos[0], light_pos[1], light_pos[2]);

		// view/projection transformations
		glm::mat4 projection = glm::perspective(45.0f, 1.0f / 1.0f, 0.1f, 100.0f);
		glm::mat4 view = glm::lookAt(
			glm::vec3(camPos[0], camPos[1], camPos[2]),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0, 1, 0)
		);

		// be sure to activate shader when setting uniforms/drawing objects
		lightingShader.use();

		lightingShader.setInt("shading_choose", shading_choose);

		lightingShader.setVec3("objectColor", 0.8f, 0.4f, 0.61f);
		lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		lightingShader.setVec3("lightPos", lightPos);
		lightingShader.setVec3("viewPos", glm::vec3(camPos[0], camPos[1], camPos[2]));

		lightingShader.setMat4("view", view);
		lightingShader.setMat4("projection", projection);

		lightingShader.setFloat("ambientFactor", ambient);
		lightingShader.setFloat("specularFactor", diffuse);
		lightingShader.setFloat("diffuseFactor", specular);
		lightingShader.setInt("shininessFactor", shininess);

		if (is_lamp_moving) {
			light_pos[0] = lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
			light_pos[1] = lightPos.y = sin(glfwGetTime() / 2.0f) * 1.0f;
		}

		lightingShader.setFloat3("lightPos", glm::value_ptr(lightPos));
		lightingShader.setFloat3("viewPos", glm::value_ptr(glm::vec3(camPos[0], camPos[1], camPos[2])));

		// world transformation
		glm::mat4 model = glm::mat4(1.0f);
		lightingShader.setMat4("model", model);
		lightingShader.setMat4("model", model);

		// render the cube
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// also draw the lamp object
		lampShader.use();
		lampShader.setMat4("projection", projection);
		lampShader.setMat4("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
		lampShader.setMat4("model", model);

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// --------------------------
	// 释放/删除之前的分配的所有资源
	// --------------------------
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);


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
}


// --------------------------
// 设置视口的维度
// --------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// 前两个参数是视口左下角的位置，后两个是视口的宽度和高度
	glViewport(0, 0, width, height);
}
