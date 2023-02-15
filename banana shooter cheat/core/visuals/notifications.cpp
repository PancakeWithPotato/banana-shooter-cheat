#include "notifactions.hpp"

void CNotifications::ClearNotifs()
{
	if (this->notifs.empty())
		return;
	auto time = ImGui::GetTime();
	for (int i = 0; i < this->notifs.size(); ++i)
	{
		if (this->notifs.at(i).dEndTime <= time) 
		{
			std::cout << "Cleared notif " << this->notifs.at(i).strName << std::endl;
			this->notifs.erase(this->notifs.begin() + i);
		}
	}
}
void CNotifications::Render() 
{
	//clear any notifs we dont want to render 
	this->ClearNotifs();

	//check if we still have anything to render
	if (this->notifs.empty())
		return;

	for (int i = 0; i < this->notifs.size(); ++i)
	{
		const auto notif = this->notifs.at(i);
		ImVec2 renderpos = { 0.f, 0.f + i * 25.f}; //offset between the texts is 25
		g_Visuals.renderText(notif.strName.c_str(), renderpos, { 1,1,1,1 });
	}
}
void CNotifications::AddNotif(std::string text) 
{
	if (this->notifs.size() >= iMaxNotifs) 
	{	this->ClearNotifs();
		Notif_t notif{ .strName = text, .dPostTime = ImGui::GetTime() };
		this->notifs.emplace_back(notif);
	}
	this->notifs.emplace_back(text);
}
