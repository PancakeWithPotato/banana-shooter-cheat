#pragma once
#include "visuals.hpp"
#define iMaxNotifs 30
#define iNotifTime 6
struct Notif_t
{
	std::string strName;
	double dPostTime;
	double dEndTime = dPostTime + iNotifTime;

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
