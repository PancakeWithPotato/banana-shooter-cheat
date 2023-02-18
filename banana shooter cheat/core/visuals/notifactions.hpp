#pragma once
#include "visuals.hpp"
#define iMaxNotifs 30
#define iNotifTime 8
struct Notif_t
{
	std::string strName;
	double dPostTime;
	double dEndTime = dPostTime + iNotifTime;
	bool bAnimated = false;
	bool bIDGenerated = false;
	double dAnimationProgress = -1 * (ImGui::CalcTextSize(strName.c_str()).x / 2);
	uint64_t uAnimationID = 0xDEAD;
};

class CNotifications
{
private:
	std::vector<Notif_t> notifs;
	void ClearNotifs();
public:
	void AddNotif(std::string text);
	void Render();
}; inline CNotifications g_Notifs;
