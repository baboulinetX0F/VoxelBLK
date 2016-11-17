#include "UIManager.h"

#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>

UIManager::UIManager()
{
	
}


UIManager::~UIManager()
{
}

void UIManager::init(Renderer * renderer)
{
	_renderer = renderer;
	ImGui_ImplGlfwGL3_Init(renderer->getWindow(), false);	
}

void UIManager::NewFrame()
{
	ImGui_ImplGlfwGL3_NewFrame(true);
}

void UIManager::Render()
{
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Text("Vertices rendered : %d ", _renderer->getVerticesRendered());
	ImGui::Render();
}

