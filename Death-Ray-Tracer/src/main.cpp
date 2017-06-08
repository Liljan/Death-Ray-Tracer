#include "GL/glew.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include "glfwContext.h"
#include <iostream>
#include <fstream>

#include "Image.h"

void GL_calls();

int main() {
	glfwContext glfw;
	GLFWwindow* current_window = nullptr;

	glfw.init(300, 200, "Procedural Planet Maker");
	glfw.getCurrentWindow(current_window);
	glfwSetInputMode(current_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	// Setup ImGui binding
	ImGui_ImplGlfw_Init(current_window, true);

	//start GLEW extension handler
	glewExperimental = GL_TRUE;
	GLenum l_GlewResult = glewInit();
	if (l_GlewResult != GLEW_OK)
		std::cout << "glewInit() error." << std::endl;

	float value = 0.0f;

	// RENDER LOOP \__________________________________________/

	while (!glfwWindowShouldClose(current_window))
	{
		glfwPollEvents();

		ImGui_ImplGlfw_NewFrame();
		{
			ImGui::Begin("Death Ray Tracer");

			ImGui::SliderFloat("Elevation", &value, 0.0f, 0.2f);

			ImGui::End();
		}

		// __________ RENDERING SETTINGS _______

		GL_calls();

		glUseProgram(0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		// Rendering imgui
		int display_w, display_h;
		glfwGetFramebufferSize(current_window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		ImGui::Render();

		glfwSwapBuffers(current_window);
	}

	ImGui_ImplGlfw_Shutdown();

	Image img(100,100);
	img.fillImage(glm::vec3(1.0f, 1.0f, 0.4f));
	img.saveAsPPM("Brett.ppm");

	return 0;
}


void GL_calls()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	glDisable(GL_TEXTURE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

