#include "menu.hpp"
#include "../../utilities/includes.hpp"
#include "../hooks/hooks.hpp"
#include "style.hpp"
#include <algorithm>

void meowpicker(const char* label, ImVec4& color)
{
	ImGui::SameLine();
	ImGui::PushID(label);

	ImGui::Text("%s", label);
	ImGui::SameLine();

	ImVec2 button_size(17.f, 0.0f);
	button_size.y = ImGui::CalcTextSize(label).y;

	ImGui::PushStyleColor(ImGuiCol_Button, color);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(color.x + 0.1f, color.y + 0.1f, color.z + 0.1f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(color.x + 0.2f, color.y + 0.2f, color.z + 0.2f, 1.0f));

	if (ImGui::Button("##colorpicker", button_size))
	{
		ImGui::OpenPopup("picker");
	}
	else if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(1))
	{
		ImGui::OpenPopup("context_menu");
	}

	ImGui::PopStyleColor(3);

	if (ImGui::BeginPopup("picker"))
	{
		ImGui::Text("Color");
		ImGui::Separator();

		ImGui::ColorEdit4("##edit", (float*)&color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoSidePreview);

		if (ImGui::Button("Close"))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (ImGui::BeginPopup("context_menu"))
	{
		if (ImGui::MenuItem("Copy"))
		{
			g_Config::elements["global,copied_color,n"] = color;
		}

		if (ImGui::MenuItem("Paste"))
		{
			color = g_Config::get<ImVec4>("global,copied_color,n");
		}

		ImGui::EndPopup();
	}

	ImGui::PopID();
}


namespace ImGui {
	static void HelpMarker(const char* format, ...) {
		ImGui::SameLine();
		ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::Text(format);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}
}

void Menu::render()  {
	if (GetAsyncKeyState(VK_INSERT) & 1) {
		open = !open;
		
		if (g_Sdk.localInGame()) {
			if (open) {
				g_Sdk.setCursorLockedMode(g_Sdk.NONE);
			}
			else {
				g_Sdk.setCursorLockedMode(g_Sdk.LOCKED);
			}
		}
	}

	if (!open)
		return;

	ImGui::SetupStyles();
	ImGui::Begin("Banana Shooter Hack", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar);
	ImGui::Columns(2, nullptr, false);

	ImGui::SetColumnOffset(1, 125);
	if (ImGui::Button("Combat", ImVec2(115, 60)))
		TabCount = TAB_COMBAT;

	if (ImGui::Button("Visuals", ImVec2(115, 60)))
		TabCount = TAB_VISUALS;

	if (ImGui::Button("Misc", ImVec2(115, 60)))
		TabCount = TAB_MISC;

	ImGui::SetCursorPos({ ImGui::GetCursorPos().x, ImGui::GetCursorPos().y + 20 });
	static std::string strRaw;
	
	if (!bInit) 
	{	
		g_Visuals.init();
		formatDate(strRaw, __DATE__);
	}

	static const std::string strBuildDate = "Build date: \n" + strRaw;

	ImGui::Text(strBuildDate.data());

	static const std::string strUser = g_Hack->username;

	ImGui::Text(strUser.data());
	ImGui::NextColumn();

	switch (TabCount)
	{
	case TAB_MISC:
		renderMisc();
		break;
	case TAB_COMBAT:
		renderCombat();
		break;
	case TAB_VISUALS:
		renderVisuals();
		break;
	}

	ImGui::End();
}

void Menu::renderVisuals() {
	ImGui::BeginChild("##visuals", { 350,260 }, true);
	ImGui::SliderFloat("Movemenet bob speed", &g_Config::get<float>("visuals,bob_speed,f"), 0, 150);
	ImGui::HelpMarker("Setting to 0 will result in no movement bob.");
	ImGui::Checkbox("Enemy box", &g_Config::get<bool>("visuals,enemy_box,b"));
	if (g_Config::get<bool>("visuals,enemy_box,b")) 
		meowpicker("##colorpickier", g_Config::get<ImVec4>("visuals,enemy_box_color,c"));
		
	ImGui::EndChild();
}

void Menu::renderCombat() 
{
	ImGui::BeginChild("##combat", { 350, 260 }, true);
	ImGui::Checkbox("Aimbot", &g_Config::get<bool>("combat,aimbot_enabled,b"));

	ImGui::Combo("Aimbot hitbox", &g_Config::get<int>("combat,aimbot_target,i"), "Head\0Body");

	ImGui::Checkbox("No reload", &g_Config::get<bool>("combat,noreload,b"));
	ImGui::Checkbox("No recoil", &g_Config::get<bool>("combat,norecoil,b"));

	ImGui::Checkbox("Explosive bullets", &g_Config::get<bool>("combat,explosive_bullets,b"));

	ImGui::SliderInt("Bullet count", &g_Config::get<int>("combat,bullet_count,i"), 1, 100);
	ImGui::HelpMarker("Will shoot more bullets");
	
	ImGui::SameLine(); 
	
	ImGui::EndChild();
}

void Menu::renderMisc() 
{
	ImGui::BeginChild("##configs", { 350, 240 }, true);

	ImGui::Checkbox("Spotify playback detection", &g_Config::get<bool>("misc,spotify,b"));

	ImGui::SetCursorPos({ ImGui::GetWindowSize().x - 200, ImGui::GetWindowSize().y - 200 });
	ImGui::PushStyleColor(ImGuiCol_ChildBg, { 0.33f, 0.34f, 0.37f, 1.f });
	ImGui::BeginChild("Configs", { 195,187 });
	ImGui::SetCursorPosX(ImGui::GetCursorPos().x + 5);
	ImGui::Spacing();
	ImGui::BeginChild("##configss", { 185, 90 });
	for (auto& i : g_Config::strConfigs) 
	{
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
		if (ImGui::Button(i.c_str(), ImVec2(175, 25))) 
		{
			g_Config::strConfiginput = i;

		}
	}
	ImGui::EndChild();
	ImGui::Spacing();
	
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
	
	ImGui::InputText("Name", &g_Config::strConfiginput);
	ImGui::Spacing();
	ImGui::Spacing();
	
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
	if (ImGui::Button("Save", { 87.5f, 22 }))
		g_Config::save(g_Config::strConfiginput);
	ImGui::SameLine();
	
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
	if (ImGui::Button("Load", { 87.5f, 22 }))
		g_Config::load(g_Config::strConfiginput);
	
	ImGui::Spacing();
	
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
	if (ImGui::Button("Refresh", { 87.5f,22 }))
		g_Config::getConfigs();
	
	ImGui::SameLine();
	
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
	if (ImGui::Button("Open", { 87.5f, 22 }))
		g_Config::openDir();

	ImGui::EndChild();
	ImGui::PopStyleColor();
	ImGui::EndChild();
}

void Menu::formatDate(std::string& date, const std::string& raw)  {
	constexpr int normalSize = 11;

	if (raw.length() > normalSize) {
		size_t firstspace = raw.find_first_of(' ');
		date = raw.substr(0, firstspace);
		date.append(raw.substr(firstspace + 1));
	}
	else
		date = raw;
	
	bInit = true;
}
