#include "menu.hpp"
#include "../../utilities/includes.hpp"
#include "../hooks/hooks.hpp"
#include "style.hpp"

static void HelpMarker(const char* desc) //thx ImGui
{
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

void Menu::Render() 
{
	if (GetAsyncKeyState(VK_INSERT) & 1) {
		open = !open;
		
		if (g_Sdk.localInGame()) {
			if (open) {
				g_Sdk.setCursorLockedMode(g_Sdk.NONE);
			}
			else {
				g_Sdk.setCursorLockedMode(g_Sdk.LOCKED);
			}
			
			ImGui::GetIO().MouseDrawCursor = open;
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
	if (ImGui::Button("Configs", ImVec2(115, 60)))
		this->TabCount = TAB_CONFIG;

	//show build date
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
	ImGui::Checkbox("Magic bullets", &g_Config::bMagicBullets);
	ImGui::SameLine(); 
	HelpMarker("Spawns bullets at the enemies.");
	static const char* stuff[2] = {"Head", "Body"};
	if (g_Config::bMagicBullets) {
		ImGui::PushItemWidth(ImGui::CalcTextSize("Head").x + 15);
		ImGui::ListBox("Magic bullets hitgroup", &g_Config::iMagicBullets, stuff, IM_ARRAYSIZE(stuff));
		ImGui::PopItemWidth(); //very pro code (i will kill myself)
	}
	ImGui::EndChild();
}

void Menu::RenderConfig() 
{
	ImGui::BeginChild("##configs", { 350, 240});
	ImGui::Text("Once we have enough features, we will put\n configs there, until then;idk what to use it for, we'll see i guess.");
	static bool bbool = false;
	ImGui::Checkbox("Test checkbox", &bbool);
	ImGui::EndChild();
}

void Menu::FormatDate(std::string& date, const std::string& raw) 
{
	//this code is so horrible LMAO
	size_t firstspace = raw.find_first_of(' ');
	date = raw.substr(0, firstspace);;
	date.append(raw.substr(firstspace + 1));
	this->bInit = true;
}
