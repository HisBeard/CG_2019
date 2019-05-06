#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <algorithm>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void Bresenham_line(int x0, int y0, int x1, int y1);
void Bresenham_circle(int cx, int cy, int r);
void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3);


// ���ڲ���
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;
int now_screen_width = SCR_WIDTH;
int now_screen_height = SCR_HEIGHT;

int max_array_size = now_screen_width * now_screen_height * 2;
float *points_to_draw = new float[max_array_size];
int points_to_draw_index = 0;

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
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CG_hw3", NULL, NULL);  // (width, height, name, , ,)
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); // ֪ͨGLFW������������
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  // �Դ���ע��һ���ص�����,��ÿ�δ��ڴ�С��������ʱ�򱻵���

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

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);  // �����������
	glGenBuffers(1, &VBO);  // ���㻺�����
	glGenBuffers(1, &EBO);  // �����������
	glBindVertexArray(VAO);  // ��VAO

	int triangle_point[6] = { 0, 0, 400, 800, 800, 0 };
	static int choose = 0;
	bool if_raserization = false;
	int circle_radius = 200;

	// --------------------------
	// ��Ⱦѭ��
	// --------------------------
	while (!glfwWindowShouldClose(window))
	{
		points_to_draw_index = 0;

		glBindBuffer(GL_ARRAY_BUFFER, VBO);  // ���´����Ļ���󶨵�GL_ARRAY_BUFFERĿ����
		glBufferData(GL_ARRAY_BUFFER, max_array_size * sizeof(float), points_to_draw, GL_STATIC_DRAW);  // ���û���������ݸ��Ƶ���ǰ�󶨻���

		// λ������
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		processInput(window);
		glfwPollEvents();

		// ����ImGui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Console");
		

		ImGui::Text("Basic1");
		ImGui::RadioButton("Triangle", &choose, 0);
		ImGui::SliderInt("x1", &triangle_point[0], 0, now_screen_width);
		ImGui::SliderInt("y1", &triangle_point[1], 0, now_screen_height);
		ImGui::SliderInt("x2", &triangle_point[2], 0, now_screen_width);
		ImGui::SliderInt("y2", &triangle_point[3], 0, now_screen_height);
		ImGui::SliderInt("x3", &triangle_point[4], 0, now_screen_width);
		ImGui::SliderInt("y3", &triangle_point[5], 0, now_screen_height);
		ImGui::Checkbox("Raserization", &if_raserization);

		ImGui::Separator();
		ImGui::Text("Basic2");
		ImGui::RadioButton("Circle", &choose, 1);
		ImGui::SliderInt("circle radius", &circle_radius, 0, (int)std::min(now_screen_width, now_screen_height) / 2);
		ImGui::End();


		if (choose == 0) {
			if (if_raserization) {
				DrawTriangle(triangle_point[0], triangle_point[1], triangle_point[2],
					triangle_point[3], triangle_point[4], triangle_point[5]);
			} else {
				Bresenham_line(triangle_point[0], triangle_point[1], triangle_point[2], triangle_point[3]);
				Bresenham_line(triangle_point[0], triangle_point[1], triangle_point[4], triangle_point[5]);
				Bresenham_line(triangle_point[2], triangle_point[3], triangle_point[4], triangle_point[5]);
			}
		}
		if (choose == 1) {
			Bresenham_circle(now_screen_width / 2, now_screen_height / 2, circle_radius);
		}

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO);

		// ��Ⱦ
		glDrawArrays(GL_POINTS, 0, points_to_draw_index / 3);
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
	return 0;
}

void Bresenham_circle(int cx, int cy, int r) {
	int x = 0, y = r;
	int d = 1 - r;
	while (x <= y) {
		points_to_draw[points_to_draw_index++] = (float)(cx + x) * 2.0f / now_screen_width - 1.0f;
		points_to_draw[points_to_draw_index++] = (float)(cy + y) * 2.0f / now_screen_height - 1.0f;
		points_to_draw[points_to_draw_index++] = 0.0f;
		points_to_draw[points_to_draw_index++] = (float)(cx - x) * 2.0f / now_screen_width - 1.0f;
		points_to_draw[points_to_draw_index++] = (float)(cy + y) * 2.0f / now_screen_height - 1.0f;
		points_to_draw[points_to_draw_index++] = 0.0f;
		points_to_draw[points_to_draw_index++] = (float)(cx + x) * 2.0f / now_screen_width - 1.0f;
		points_to_draw[points_to_draw_index++] = (float)(cy - y) * 2.0f / now_screen_height - 1.0f;
		points_to_draw[points_to_draw_index++] = 0.0f;
		points_to_draw[points_to_draw_index++] = (float)(cx - x) * 2.0f / now_screen_width - 1.0f;
		points_to_draw[points_to_draw_index++] = (float)(cy - y) * 2.0f / now_screen_height - 1.0f;
		points_to_draw[points_to_draw_index++] = 0.0f;
		points_to_draw[points_to_draw_index++] = (float)(cx + y) * 2.0f / now_screen_width - 1.0f;
		points_to_draw[points_to_draw_index++] = (float)(cy + x) * 2.0f / now_screen_height - 1.0f;
		points_to_draw[points_to_draw_index++] = 0.0f;
		points_to_draw[points_to_draw_index++] = (float)(cx - y) * 2.0f / now_screen_width - 1.0f;
		points_to_draw[points_to_draw_index++] = (float)(cy + x) * 2.0f / now_screen_height - 1.0f;
		points_to_draw[points_to_draw_index++] = 0.0f;
		points_to_draw[points_to_draw_index++] = (float)(cx + y) * 2.0f / now_screen_width - 1.0f;
		points_to_draw[points_to_draw_index++] = (float)(cy - x) * 2.0f / now_screen_height - 1.0f;
		points_to_draw[points_to_draw_index++] = 0.0f;
		points_to_draw[points_to_draw_index++] = (float)(cx - y) * 2.0f / now_screen_width - 1.0f;
		points_to_draw[points_to_draw_index++] = (float)(cy - x) * 2.0f / now_screen_height - 1.0f;
		points_to_draw[points_to_draw_index++] = 0.0f;
		if (d <= 0) {
			d += x * 2 + 3;
		} else {
			d += (x - y) * 2 + 5;
			y--;
		}
		x++;
	}
}

void Bresenham_line(int x0, int y0, int x1, int y1) {
	int temp;
	if ((x0 > x1) && (y0 > y1)) {
		temp = x0; x0 = x1; x1 = temp;
		temp = y0; y0 = y1; y1 = temp;
	}

	int dirx = 1;
	if (!(y0 > y1) && (x0 > x1)) dirx = -1;
	if ((y0 > y1) && !(x0 > x1)) { 
		dirx = -1;
		temp = x0; x0 = x1; x1 = temp;
		temp = y0; y0 = y1; y1 = temp;
	}

	// �ж�б���Ƿ����45��
	int flag = 0;
	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);
	if (dx < dy) {
		temp = dx; dx = dy; dy = temp;
		flag = 1;
	}

	int nowx = x0;
	int nowy = y0;
	int different = 2 * dy - dx;
	for (int i = 0; i <= dx; i++) {
		points_to_draw[points_to_draw_index++] = (float)nowx * 2.0f / now_screen_width - 1.0f;
		points_to_draw[points_to_draw_index++] = (float)nowy * 2.0f / now_screen_height - 1.0f;
		points_to_draw[points_to_draw_index++] = 0.0f;
		if (different <= 0) {
			if (flag == 0) nowx += dirx;
			else nowy += 1;
			different += 2 * dy;
		}
		else {
			nowx += dirx;
			nowy += 1;
			different += 2 * (dy - dx);
		}
	}
}

void DrawTriangle1(int x1, int y1, int x2, int y2, int x3, int y3) // ��ʵ��ƽ��������
{
	for (int y = y1; y <= y2; ++y)
	{
		int xs, xe;
		xs = (y - y1) * (x2 - x1) / (y2 - y1) + x1 + 0.5;
		xe = (y - y1) * (x3 - x1) / (y3 - y1) + x1 + 0.5;
		Bresenham_line(xs, y, xe, y);
	}
}

void DrawTriangle2(int x1, int y1, int x2, int y2, int x3, int y3) // ��ʵ��ƽ��������
{
	for (int y = y1; y <= y3; ++y)
	{
		int xs, xe;
		xs = (y - y1) * (x3 - x1) / (y3 - y1) + x1 + 0.5;
		xe = (y - y2) * (x3 - x2) / (y3 - y2) + x2 + 0.5;
		Bresenham_line(xs, y, xe, y);
	}
}

void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3) // ������ʵ��������
{
	if (y1 == y2)
	{
		if (y3 <= y1) // ƽ��
		{
			DrawTriangle1(x3, y3, x1, y1, x2, y2);
		}
		else // ƽ��
		{
			DrawTriangle2(x1, y1, x2, y2, x3, y3);
		}
	}
	else if (y1 == y3)
	{
		if (y2 <= y1) // ƽ��
		{
			DrawTriangle1(x2, y2, x1, y1, x3, y3);
		}
		else // ƽ��
		{
			DrawTriangle2(x1, y1, x3, y3, x2, y2);
		}

	}
	else if (y2 == y3)
	{
		if (y1 <= y2) // ƽ��
		{
			DrawTriangle1(x1, y1, x2, y2, x3, y3);
		}
		else // ƽ��
		{
			DrawTriangle2(x2, y2, x3, y3, x1, y1);
		}
	}
	else
	{
		double xtop, ytop, xmiddle, ymiddle, xbottom, ybottom;
		if (y1 < y2 && y2 < y3) // y1 y2 y3
		{
			xtop = x1;
			ytop = y1;
			xmiddle = x2;
			ymiddle = y2;
			xbottom = x3;
			ybottom = y3;
		}
		else if (y1 < y3 && y3 < y2) // y1 y3 y2
		{
			xtop = x1;
			ytop = y1;
			xmiddle = x3;
			ymiddle = y3;
			xbottom = x2;
			ybottom = y2;
		}
		else if (y2 < y1 && y1 < y3) // y2 y1 y3
		{
			xtop = x2;
			ytop = y2;
			xmiddle = x1;
			ymiddle = y1;
			xbottom = x3;
			ybottom = y3;
		}
		else if (y2 < y3 && y3 < y1) // y2 y3 y1
		{
			xtop = x2;
			ytop = y2;
			xmiddle = x3;
			ymiddle = y3;
			xbottom = x1;
			ybottom = y1;
		}
		else if (y3 < y1 && y1 < y2) // y3 y1 y2
		{
			xtop = x3;
			ytop = y3;
			xmiddle = x1;
			ymiddle = y1;
			xbottom = x2;
			ybottom = y2;
		}
		else if (y3 < y2 && y2 < y1) // y3 y2 y1
		{
			xtop = x3;
			ytop = y3;
			xmiddle = x2;
			ymiddle = y2;
			xbottom = x1;
			ybottom = y1;
		}
		int xl; // ������ymiddleʱ��x������������������߻����ұ�
		xl = (ymiddle - ytop) * (xbottom - xtop) / (ybottom - ytop) + xtop + 0.5;

		if (xl <= xmiddle) // ��������
		{
			// ��ƽ��
			DrawTriangle1(xtop, ytop, xl, ymiddle, xmiddle, ymiddle);

			// ��ƽ��
			DrawTriangle2(xl, ymiddle, xmiddle, ymiddle, xbottom, ybottom);
		}
		else // ��������
		{
			// ��ƽ��
			DrawTriangle1(xtop, ytop, xmiddle, ymiddle, xl, ymiddle);

			// ��ƽ��
			DrawTriangle2(xmiddle, ymiddle, xl, ymiddle, xbottom, ybottom);
		}
	}
}


void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	now_screen_width = width;
	now_screen_height = height;
	max_array_size = (int)std::max(now_screen_width, now_screen_height) * 10;
	points_to_draw = new float[max_array_size];
	glViewport(0, 0, width, height);
}