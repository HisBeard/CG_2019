#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Shader.h"

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#define IMGUI
//#define DEBUG

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
vector<glm::vec3>::iterator getNearPoints(const float xpos, const float ypos, const float threshold);
vector<glm::vec3> getPointsInEachLayers(vector<glm::vec3> p, int layer);

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Error %d: %s\n", error, description);
}

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Global value
vector<glm::vec3> p;
vector<glm::vec3>::iterator currPointIter;
bool isLeftButtonPressed = false;
int pointNum = 0;
int isBonus = 0;
float t = 0;

int main()
{
	// --------------------------
	// 实例化GLFW窗口
	// --------------------------
	glfwSetErrorCallback(glfw_error_callback);
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// --------------------------
	// 创建一个窗口对象
	// --------------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CG_HW8", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

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

	// build and compile our shader zprogram
	// ------------------------------------
	Shader curveShader = Shader(".\\shader\\bezierCurze.vs", ".\\shader\\bezierCurze.fs");
	Shader pointShader = Shader(".\\shader\\point.vs", ".\\shader\\point.fs");

	// 生成顶点数据 t
	float step = 0.001;
	vector<float> data;
	data.resize(int(1 / step));
	for (int i = 0; i < data.size(); ++i) {
		data[i] = i * step;
	}

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), data.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	p.clear();
	p.resize(5);
	currPointIter = p.end();

	for (int i = 0; i < 5; ++i) {
		p[i] = glm::vec3(-100.0f, -100.0f, -100.0f);
	}

	GLuint pVAO, pVBO;
	glGenVertexArrays(1, &pVAO);
	glGenBuffers(1, &pVBO);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// ------------------------------------------------------------------------------- 
		glfwPollEvents();

		int scr_width, scr_height;
		glfwGetFramebufferSize(window, &scr_width, &scr_height);
		glViewport(0, 0, scr_width, scr_height);

		// Rendering
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Render Control Points
		if (isLeftButtonPressed) {
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			currPointIter = getNearPoints(xpos, ypos, 180);
			if (currPointIter != p.end()) {
				*currPointIter = glm::vec3(xpos, ypos, 0.0f);
			}
		}
		glBindVertexArray(pVAO);
		glBindBuffer(GL_ARRAY_BUFFER, pVBO);
		auto controlPoints2dataVector = [](vector<glm::vec3> p) -> vector<GLfloat> {
			vector<GLfloat> res;
			res.clear();
			for (int i = 0; i < p.size(); ++i) {
				if (p[i] != glm::vec3(-100.0f, -100.0f, -100.0f)) {
					res.push_back(p[i].x);
					res.push_back(p[i].y);
					res.push_back(p[i].z);
				}
			}
			// 将数据归一化到[-1, 1]
			for (int i = 0; i < res.size(); i = i + 3) {
				auto norx = (2 * res[i]) / SCR_WIDTH - 1;
				auto nory = 1 - (2 * res[i + 1]) / SCR_HEIGHT;
				res[i] = norx;
				res[i + 1] = nory;
			}
			return res;
		};
		// 绘制控制顶点和其间的连线
		auto pointData = controlPoints2dataVector(p);
		glBufferData(GL_ARRAY_BUFFER, pointData.size() * sizeof(GLfloat), pointData.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		pointShader.use();
		glPointSize(10.0f);
		glDrawArrays(GL_POINTS, 0, pointData.size() / 3);
		glDrawArrays(GL_LINE_STRIP, 0, pointData.size() / 3);
		glBindVertexArray(0);

		auto glfwPos2nocPos = [](const glm::vec3 p) -> glm::vec3 {
			glm::vec3 res;
			res.x = (2 * p.x) / SCR_WIDTH - 1;
			res.y = 1 - (2 * p.y) / SCR_HEIGHT;
			res.z = 0.0f;
			return res;
		};

		// Render Bezier Curve
		float curveColor[3] = { 1.0f, 1.0f, 1.0f };
		curveShader.use();
		curveShader.setInt("pointNum", pointNum);
		curveShader.setFloat3("curveColor", curveColor);
		for (int i = 0; i < p.size(); i++) {
			curveShader.setVec3("p" + to_string(i), glfwPos2nocPos(p[i]));
		}
		glBindVertexArray(VAO);
		glPointSize(1.0f);
		glDrawArrays(GL_POINTS, 0, data.size());
		glBindVertexArray(0);

		// bonus
		if (isBonus == 1) {
			int layer = pointNum - 1;
			cout << t << endl;
			vector<glm::vec3> tp = p;
			for (int i = layer; i >= 1; i--) {
				tp = getPointsInEachLayers(tp, i);
				glBindVertexArray(pVAO);
				glBindBuffer(GL_ARRAY_BUFFER, pVBO);
				auto pointData = controlPoints2dataVector(tp);
				glBufferData(GL_ARRAY_BUFFER, pointData.size() * sizeof(GLfloat), pointData.data(), GL_STATIC_DRAW);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
				glEnableVertexAttribArray(0);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				pointShader.use();
				glPointSize(5.0f);
				glDrawArrays(GL_POINTS, 0, pointData.size() / 3);
				glDrawArrays(GL_LINE_STRIP, 0, pointData.size() / 3);
				glBindVertexArray(0);
			}
		}

		glfwSwapBuffers(window);
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	// Cleanup
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		isBonus = 1;
		t += 0.0001;
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE) {
		isBonus = 0;
		t = 0;
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void addPoint(const float xpos, const float ypos) {
	auto emptyItem = find(p.begin(), p.end(), glm::vec3(-100.0f, -100.0f, -100.0f));
	for (auto iter = p.begin(); iter != p.end(); ++iter) {
		if (*iter == glm::vec3(-100.0f, -100.0f, -100.0f)) {
			*iter = glm::vec3(xpos, ypos, 0.0f);
			break;
		}
	}
}

vector<glm::vec3>::iterator getNearPoints(const float xpos, const float ypos, const float threshold) {
	vector<glm::vec3>::iterator  res = p.end();
	auto dist = [&xpos, &ypos](const vector<glm::vec3>::iterator iter) -> float {
		return pow((xpos - iter->x), 2) + pow((ypos - iter->y), 2);
	};
	for (auto iter = p.begin(); iter != p.end(); ++iter) {
		auto dis = dist(iter);
		if (dis < threshold) {
			if (res == p.end()) { res = iter; }
			else { res = (dist(res) < dis) ? res : iter; }
		}
	}
	return res;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		// add one point on the canvas  && move the selected points
		if (action == GLFW_PRESS) {
			isLeftButtonPressed = true;
			if (p.end() == getNearPoints(xpos, ypos, 180) && pointNum < 5) {
				// add the selected point
				addPoint(xpos, ypos);
				pointNum++;
			}
		}

		if (action == GLFW_RELEASE) {
			currPointIter = p.end();
			isLeftButtonPressed = false;
		}
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		// delete the last point
		for (auto iter = p.rbegin(); iter != p.rend(); ++iter) {
			if (*iter != glm::vec3(-100.0f, -100.0f, -100.0f)) {
				*iter = glm::vec3(-100.0f, -100.0f, -100.0f);
				pointNum--;
				break;
			}
		}
	}
}

vector<glm::vec3> getPointsInEachLayers(vector<glm::vec3> p, int layer) {
	vector<glm::vec3> res;
	for (int i = 0; i < layer; i++) {
		glm::vec3 pt = (1 - t) * p[i] + t * p[i + 1];
		res.push_back(pt);
	}
	return res;
}