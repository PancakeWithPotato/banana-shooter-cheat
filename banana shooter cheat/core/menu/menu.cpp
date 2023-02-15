#include "menu.hpp"
#include "../../utilities/includes.hpp"
#include "../hooks/hooks.hpp"
#include "style.hpp"
#include <algorithm>

void meowpicker(const char* label, ImVec4& color)
{
	ImGui::SameLine();
	ImGui::PushID(label);

	//ImGui::Text("%s", label);
	//ImGui::SameLine();

	ImVec2 button_size(20.f, 0.0f);
	button_size.y = ImGui::GetFrameHeight();

	ImGui::PushStyleColor(ImGuiCol_Button, color);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(color.x + 0.1f, color.y + 0.1f, color.z + 0.1f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(color.x + 0.2f, color.y + 0.2f, color.z + 0.2f, 1.0f));
	//since in this case, every single child has the same width, we can hardcode our setcursorposx, and it will look fine (315)
	constexpr float availWeWant = 315.f;
	ImGui::SetCursorPosX(availWeWant);
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

	static const std::string strUser = g_Hack->username;

	static std::string strRaw;

	if (!bInit)
	{
		//ugly implementation, but idrc
		D3DX11_IMAGE_LOAD_INFO info;
		ID3DX11ThreadPump* pump{ nullptr };
		D3DX11CreateShaderResourceViewFromMemory(g_Hooks->pDevice, sillyCatPNG, sizeof(sillyCatPNG), &info,
			pump, &image, 0);
		g_Visuals.init();
		formatDate(strRaw, __DATE__);
	}
	static const std::string strBuildDate = "Build date: \n" + strRaw;
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

	ImGui::Text(strBuildDate.data());

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

	//picture
	ImGui::Begin("dummy", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs);
	ImGui::SetWindowPos({ g_Visuals.v2ScreenSize.x - 150, g_Visuals.v2ScreenSize.y - 150 });
	ImGui::Image((void*)image, {150,150});
	ImGui::End();

	//lua window
	this->renderLua();
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

void Menu::renderLua() 
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, { 430.f, 165 });
	//lua stuff (separate window)
	ImGui::Begin("Meowware - LUA", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar);
	if (ImGui::Button("Open folder", ImVec2({ 100.f, 35 })))
		g_Lua.openDir();
	static const auto cursor = ImGui::GetCursorPosY();
	static std::string buttonText = "Load";

	//1 to skip, 0 to not skip
	static int shouldSkip = 0; 
	//this is very bad but oh well
	ImGui::SameLine();
	ImGui::SetCursorPosX(275); //size - childsize - 10

	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.10, 0.10, 0.10, 1.00f));
	ImGui::BeginChild("##luas", { 145, 110 });
	for (auto& i : g_Lua.allLuas)
	{
		//this is still horrible, I'm almost crying ;C.. blastbrean, cmon, fix my code
		shouldSkip = 0;
		for (const auto& k : g_Lua.luas)
		{
			if (k.luaName == i) {
				shouldSkip = 1;
				break;
			}

		}
		if (shouldSkip == 1)
			continue;
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
		if (ImGui::Button(i.c_str(), ImVec2(135, 25))) {
			g_Lua.selectedLua = i;
			buttonText = "Load";
		}
			
	}
	ImGui::EndChild();
	ImGui::SameLine();
	ImGui::SetCursorPosX(120); //earlier pos - childsize
	ImGui::BeginChild("##loadedluas", { 145, 110 });
	for (auto& i : g_Lua.luas)
	{
		if (ImGui::Button(i.luaName.c_str(), ImVec2(135, 25))) 
		{
			g_Lua.selectedLua = i.luaName;
			buttonText = "Unload";
		}
			
	}
	ImGui::EndChild();
	ImGui::PopStyleColor();
	ImGui::SetCursorPosY(cursor);
	if (ImGui::Button("Refresh", ImVec2({ 100.f, 35 })))
		g_Lua.getLuas();

	if (ImGui::Button(buttonText.c_str(), ImVec2({ 100.f, 35 })))
	{
		if (buttonText == "Load")
			g_Lua.openLua(g_Lua.selectedLua);
		else
			g_Lua.destroy(g_Lua.selectedLua);
	}
	ImGui::Text("Selected LUA: %s, loaded in luas: %i", g_Lua.selectedLua.c_str(), (int)g_Lua.luas.size());

	ImGui::End();

	ImGui::PopStyleVar();
}
