#include "menu.hpp"
#include "../../utilities/includes.hpp"
#include "../hooks/hooks.hpp"
#include "style.hpp"

namespace ImGui {
	static void HelpMarker(const char* desc) {
		ImGui::SameLine();
		ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(desc);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}

}

void Menu::Render()  {
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

	SetupStyles();
	ImGui::Begin("Banana Shooter Hack", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar);

	ImGui::Columns(2, nullptr, false);
	ImGui::SetColumnOffset(1, 125);
	if (ImGui::Button("Combat", ImVec2(115, 60)))
		this->TabCount = TAB_COMBAT;
	if (ImGui::Button("Visuals", ImVec2(115, 60)))
		this->TabCount = TAB_VISUALS;
	if (ImGui::Button("Misc", ImVec2(115, 60)))
		this->TabCount = TAB_MISC;

	ImGui::SetCursorPos({ ImGui::GetCursorPos().x, ImGui::GetCursorPos().y + 20 });
	static std::string strRaw;
	
	if (!this->bInit)
		this->FormatDate(strRaw, __DATE__);

	static const std::string strBuildDate = "Build date: \n" + strRaw;
	ImGui::Text(strBuildDate.data());
	static const std::string strUser = "User: " + g_Hack->strName;
	ImGui::Text(strUser.data());
	ImGui::NextColumn();

	switch (this->TabCount)
	{
	case TAB_MISC:
		this->RenderMisc();
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
	ImGui::BeginChild("##visuals", { 350,260 }, true);
	ImGui::SliderFloat("Movemenet bob speed", &g_Config::Visuals::fBobSpeed, 0, 150);
	ImGui::HelpMarker("Setting to 0 will result in no movement bob.");
	ImGui::EndChild();
}

void Menu::RenderCombat() 
{
	ImGui::BeginChild("##combat", { 350, 260 }, true);
	ImGui::Checkbox("Aimbot", &g_Config::Combat::Aimbot);

	//ImGui::PushItemWidth((ImGui::CalcTextSize("Head").x + 15));
	ImGui::Combo("Aimbot hitbox", &g_Config::Combat::AimbotHitbox, "Head\0Body");
	//ImGui::PopItemWidth();

	ImGui::Checkbox("No reload", &g_Config::Combat::NoReload);
	ImGui::Checkbox("No recoil", &g_Config::Combat::NoRecoil);

	ImGui::Checkbox("Explosive bullets", &g_Config::Combat::ExplosiveBullets);

	ImGui::SliderInt("Bullet count", &g_Config::Combat::BulletsCount, 1, 100);
	ImGui::HelpMarker("Will shoot x more bullets.");
	
	ImGui::SameLine(); 
	
	ImGui::EndChild();
}

void Menu::RenderMisc() 
{
	ImGui::BeginChild("##configs", { 350, 240 }, true);

	if (ImGui::Button("Force start game", ImVec2(125, 30)))
		g_Config::Misc::StartGame = true;

	ImGui::EndChild();
}

void Menu::FormatDate(std::string& date, const std::string& raw) 
{
	//this code is so horrible LMAO
#define NORMAL_SIZE  11 //size for single digit dates (like Jan 2)
	if (raw.length() > NORMAL_SIZE)
	{
		size_t firstspace = raw.find_first_of(' ');
		date = raw.substr(0, firstspace);;
		date.append(raw.substr(firstspace + 1));
	}
	else
		date = raw;
	
	this->bInit = true;
}
