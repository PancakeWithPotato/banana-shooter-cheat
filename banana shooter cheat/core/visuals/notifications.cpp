#include "notifactions.hpp"

void CNotifications::ClearNotifs()
{
	if (this->notifs.empty())
		return;
	auto time = ImGui::GetTime();
	if (this->notifs.size() >= iMaxNotifs) 
	{
		for (size_t i = 0; i < this->notifs.size(); i++)
		{
			if(this->notifs.size() >= iMaxNotifs)
				this->notifs.erase(this->notifs.begin() + i);
		}
	}
	for (int i = 0; i < this->notifs.size(); ++i)
	{
		if (this->notifs.at(i).dEndTime <= time) 
			this->notifs.erase(this->notifs.begin() + i);
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
		auto& notif = this->notifs.at(i);
		if (!notif.bIDGenerated) {
			notif.uAnimationID += rand() % 500;
			//g_Debug.logState(::SUCCESS, "Id is: %i", notif.uAnimationID);
			notif.bIDGenerated = true;
		}
		
		if (notif.dAnimationProgress < 3.5)
			notif.dAnimationProgress += Animations::Tween(notif.uAnimationID, { .duration = 1.f, .state = !notif.bAnimated, .from = (float)notif.dAnimationProgress, .dest = 3.5f }); //as state, i could pass in true, since it doesnt matter if i change it to false or not, cuz im doing "+=", not "=". I Do this, since it starts as a minus value, and just simply using "=", would make it a value between 0 and 1, thus not minus.
		//i may add animations later, idk
		ImVec2 renderpos = {(float)notif.dAnimationProgress, (0.f + i * 15.f)}; //offset between the texts is 15
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
	Notif_t notif{ .strName = text, .dPostTime = ImGui::GetTime() };
	this->notifs.emplace_back(notif);
}
