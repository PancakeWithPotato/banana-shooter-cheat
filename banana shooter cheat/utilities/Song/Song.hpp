#pragma once
#include "../includes.hpp"
#include <TlHelp32.h>

enum EPlayModes : short {
	NOT_RUNNING = 1,
	STOPPED,
	PLAYING
};

struct Song_t {
	std::string strAuthor;
	std::string strSongname;
};

class Spotify 
{
private:
	DWORD m_dwPID;
	DWORD GetProcessID();
public:
	Spotify() {
		m_dwPID = GetProcessID();
	}
	Song_t CurrentlyPlaying;
	EPlayModes CurrentState;
	Song_t GetCurrentSong();
	std::string strComplete;

	void Update();
}; inline Spotify g_Spotify;