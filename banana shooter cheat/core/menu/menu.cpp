#include "menu.hpp"
#include "../../utilities/includes.hpp"
#include "../hooks/hooks.hpp"
#include "style.hpp"

void Menu::Render() 
{
	if (GetAsyncKeyState(VK_INSERT) & 1) {
		open = !open;

		ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
		ImGui::GetIO().WantCaptureMouse = g_Menu.open;
		ImGui::GetIO().MouseDrawCursor = g_Menu.open;

		if (open) {
			g_Sdk.setCursorLockedMode(g_Sdk.NONE);
		}
		else {
			g_Sdk.setCursorLockedMode(g_Sdk.LOCKED);
		}
	}

	if (!open)
		return;

	SetCapture(g_Hooks->window);
	ImGui::Begin("Banana Shooter Hack", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar);

	ImGui::Columns(2, nullptr, false);
	ImGui::SetColumnOffset(1, 125);
	if (ImGui::Button("Combat", ImVec2(115, 60)))
		this->TabCount = TAB_COMBAT;
	if (ImGui::Button("Visals", ImVec2(115, 60)))
		this->TabCount = TAB_VISUALS;
	if (ImGui::Button("Configs", ImVec2(115, 60)))
		this->TabCount = TAB_CONFIG;
	ImGui::NextColumn();
	switch (this->TabCount)
	{
	case TAB_CONFIG:
		this->RenderConfig();
		break;
	case TAB_COMBAT:
		this->RenderCombat();
		break;
	case TAB_VISUALS:
		this->RenderVisuals();
		break;
	default:
		ImGui::Text("Something went wrong :(");
		break;
	}

	ImGui::End();
}

void Menu::RenderVisuals() {
	ImGui::Text("We got none lolz");
}

void Menu::RenderCombat() 
{
	ImGui::BeginChild("##combat", { 350, 240 }, true);
	ImGui::Checkbox("Eliminate recoil", &g_Config::NoRecoil);
	ImGui::Checkbox("Explosive bullets", &g_Config::ExplosiveBullets);
	ImGui::Checkbox("Eliminate reload time", &g_Config::NoReload);
	ImGui::EndChild();
}

void Menu::RenderConfig() 
{
	ImGui::BeginChild("##configs", { 350, 240});
	ImGui::Text("Once we have enough features, we will put configs \nthere, until then;idk what to use it for, we'll see i guess.");
	static bool bbool = false;
	ImGui::Checkbox("Test checkbox", &bbool);
	ImGui::EndChild();
}
