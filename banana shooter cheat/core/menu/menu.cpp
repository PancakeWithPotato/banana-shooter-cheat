#include "menu.hpp"
#include "../../utilities/includes.hpp"

void Menu::Render() {
	if (GetAsyncKeyState(VK_INSERT) & 1)
		open = !open;

	if (!open)
		return;

	ImGui::Begin("Banana Shooter Hack", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Checkbox("No recoil", &g_Config::NoRecoil);
	ImGui::Checkbox("No reload", &g_Config::NoReload);
	ImGui::Checkbox("Explosive bullets", &g_Config::ExplosiveBullets);

	ImGui::End();
}
