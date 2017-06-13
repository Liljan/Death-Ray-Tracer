#include "GL/glew.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include "glfwContext.h"
#include <iostream>
#include <fstream>

#include <iostream>
#include <string>

#include "Image.h"

int width = 1080, height = 720;
int rays_per_pixel = 1;
float fov = 45.0f;

static char file_name_buffer[64] = "file_name";
static const char* FILE_ENDING = ".ppm";

static char* render_message = "Ready to render...";

void GL_calls();

void render() {
	Image img(width, height);

	render_message = "Rendering...";
	img.fillImage(glm::vec3(1.0f, 1.0f, 0.4f));

	std::string full_path = file_name_buffer;
	full_path.append(FILE_ENDING);

	img.saveAsPPM(full_path.c_str());

	render_message = "Successfully rendered image";
}

int main() {
	glfwContext glfw;
	GLFWwindow* current_window = nullptr;

	glfw.init(400, 200, "Procedural Planet Maker");
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

			ImGui::InputInt("Width", &width);
			ImGui::InputInt("Height", &height);

			ImGui::InputInt("Rays per pixel", &rays_per_pixel, 1, 1);
			ImGui::InputFloat("Field of view", &fov, 1.0f, 1.5f, 1.0f);

			ImGui::InputText("File name", file_name_buffer, sizeof(file_name_buffer));

			if (ImGui::Button("Render"))
				render();

			ImGui::Separator();
			ImGui::Spacing();

			ImGui::Text(render_message);

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

