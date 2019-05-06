#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);


// GLSL������ɫ����Դ����
const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"   ourColor = aColor;\n"
"}\0";
// Ƭ����ɫ����Դ����
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
	// ʵ����GLFW����
	// --------------------------
	glfwInit();  // ��ʼ��GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  // ����GLFW����һ�������Ǻܶ�ö��ֵ
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);  // �ڶ��������ǵ�һ��������ֵ������
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// --------------------------
	// ����һ�����ڶ���
	// --------------------------
	GLFWwindow* window = glfwCreateWindow(800, 800, "CG_hw2", NULL, NULL); // (width, height, name, , ,)
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);  // ֪ͨGLFW������������
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // �Դ���ע��һ���ص�����,��ÿ�δ��ڴ�С��������ʱ�򱻵���

	// --------------------------
	// ��ʼ��GLAD
	// --------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// ����ImGui������
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 150");

	// --------------------------
	// ������ɫ��
	// --------------------------
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);  // ����һ����ɫ������
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);  // �������ɫ��Դ�븽�ӵ���ɫ��������
	// Ҫ�������ɫ��������Ϊ��һ������,�ڶ�������ָ���˴��ݵ�Դ���ַ�������������ֻ��һ���������������Ƕ�����ɫ��������Դ��
	glCompileShader(vertexShader);  // ���������ɫ��
	// ������ʱ����
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// --------------------------
	// Ƭ����ɫ��
	// --------------------------
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // ����һ��������ɫ��
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// ������ʱ����
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// --------------------------
	// ������ɫ��
	// --------------------------
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();  // ����һ���������
	// ��֮ǰ�������ɫ�����ӵ��������
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// ���������ɫ�������Ƿ�ʧ��
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	// ����ɫ���������ӵ���������Ժ�ɾ����ɫ������
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// ��ʼ����������
	ImVec4 A_color = ImVec4(1.0f, 0.0f, 0.0f, 1.00f);
	ImVec4 B_color = ImVec4(1.0f, 0.0f, 0.0f, 1.00f);
	ImVec4 C_color = ImVec4(0.0f, 1.0f, 0.0f, 1.00f);
	ImVec4 D_color = ImVec4(0.0f, 0.0f, 1.0f, 1.00f);
	ImVec4 E_color = ImVec4(0.0f, 0.0f, 1.0f, 1.00f);
	ImVec4 F_color = ImVec4(0.0f, 1.0f, 0.0f, 1.00f);

	unsigned int indices[] = {
		0, 4, 5, // ��1��������
		1, 2, 3, // ��2��������
		0, 1, 4, // ��3��������
		1, 3, 4  // ��4��������
	};

	unsigned int VBO, VAO, EBO;
	bool ImGui = true;
	bool the_same_color = false;
	bool draw_rectangle = false;
	bool draw_trangle = false;
	bool bonus_draw_line = false;
	bool bonus_draw_another_trangle = false;

	// --------------------------
	// ��Ⱦѭ��
	// --------------------------
	while (!glfwWindowShouldClose(window))
	{
		// ָ���������㣬ÿ�����㶼��һ��3Dλ���Լ���ɫ
		float vertices[] = {
			// ����              // ��ɫ
			-0.3f, 0.3f, 0.0f,  A_color.x, A_color.y, A_color.z,  // A
			0.0f, 0.3f, 0.0f, B_color.x, B_color.y, B_color.z,  // B
			0.6f, 0.3f, 0.0f,  C_color.x, C_color.y, C_color.z,   // C
			0.3f, -0.3f, 0.0f,  D_color.x, D_color.y, D_color.z,   // D
			0.0f, -0.3f, 0.0f,  E_color.x, E_color.y, E_color.z,   // E
			-0.6f, -0.3f, 0.0f,  F_color.x, F_color.y, F_color.z   // F
		};

		glGenVertexArrays(1, &VAO);  // �����������
		glGenBuffers(1, &VBO);  // ���㻺�����
		glGenBuffers(1, &EBO);  // �����������
		glBindVertexArray(VAO);  // ��VAO

		glBindBuffer(GL_ARRAY_BUFFER, VBO);  // ���´����Ļ���󶨵�GL_ARRAY_BUFFERĿ����
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  // ���û���������ݸ��Ƶ���ǰ�󶨻���

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);  // ���´����Ļ���󶨵�GL_ELEMENT_ARRAY_BUFFERĿ����
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// λ������
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);  // ���ö�������ָ��
		glEnableVertexAttribArray(0);
		// ��ɫ����
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		//glEnableVertexAttribArray(1);
		glUseProgram(shaderProgram);  // ����һ���������


		// ����ImGui
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

		// ��Ⱦ������ɫ
		int view_width, view_height;
		glfwGetFramebufferSize(window, &view_width, &view_height);
		glViewport(0, 0, view_width, view_height);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// ����ɫ��
		if (draw_trangle) {
			glEnableVertexAttribArray(1);
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		}

		// ������
		if (draw_rectangle) {
			glEnableVertexAttribArray(1);
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
		}

		// �߶�
		if (bonus_draw_line) {
			glEnableVertexAttribArray(1);
			glBindVertexArray(VAO);
			glDrawArrays(GL_LINE_STRIP, 0, 3);
		}

		// ���������
		if (bonus_draw_another_trangle) {
			glEnableVertexAttribArray(1);
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		// 
		glfwSwapBuffers(window);

		// --------------------------
		// �ͷ�/ɾ��֮ǰ�ķ����������Դ
		// --------------------------
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}

	glfwTerminate();
	return 0;
}

// --------------------------
// ʵ��һЩ�������,��Ҫһ�������Լ�һ��������Ϊ����
// --------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  // ����û��Ƿ����˷��ؼ�(Esc)
		glfwSetWindowShouldClose(window, true);
}

// --------------------------
// �����ӿڵ�ά��
// --------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);  // ǰ�����������ӿ����½ǵ�λ�ã����������ӿڵĿ�Ⱥ͸߶�
}