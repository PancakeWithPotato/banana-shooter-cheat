#include "spotify.hpp"

DWORD Spotify::GetProcessID()
{
	HANDLE hSnap;
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);
	hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (!hSnap || hSnap == INVALID_HANDLE_VALUE)
		return 0;

	if (!Process32First(hSnap, &entry))
		return 0;

	while (Process32Next(hSnap, &entry))
	{
		if (_stricmp("spotify.exe", entry.szExeFile) == 0)
			return entry.th32ProcessID;
	}
	CloseHandle(hSnap);
	return 0;
}

Song_t Spotify::GetCurrentSong()
{
	static int iTitleLength = 0;
	static std::string szTitle;
	static DWORD dwTmpID = 0;

	this->m_dwPID = this->GetProcessID();

	if (this->m_dwPID == 0)
	{
		this->CurrentState = NOT_RUNNING;
		return{ "", "" };
	}

	for (HWND hWindow = GetTopWindow(0); hWindow; hWindow = GetWindow(window, GW_HWNDNEXT))
	{
		if (!IsWindowVisible(hWindow))
			continue;

		iTitleLength = GetWindowTextLengthA(hWindow);


		if (iTitleLength == 0)
			continue;

		GetWindowThreadProcessId(hWindow, &dwTmpID);

		if (dwTmpID != this->m_dwPID)
			continue;

		GetWindowTextA(hWindow, szTitle.data(), iTitleLength + 1);

		size_t dash = 0;
		dash = szTitle.find_last_of('-');

		if (dash == std::string::npos) {
			this->CurrentState = STOPPED;
			return { "", "" };
		}

		this->CurrentState = PLAYING;
		//TODO: playing a song with an umlaut (or probably any special charachter really) will fuck up the whole thing
		return { szTitle.substr(0, dash - 1), szTitle.substr(dash + 2, szTitle.size()) };
	}
	return Song_t();
}

void Spotify::Update()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(1500));

	if (!g_Config::get<bool>("misc,spotify,b"))
		return;

	this->CurrentlyPlaying = this->GetCurrentSong();
	switch (this->CurrentState)
	{
	case NOT_RUNNING:
		this->strComplete = "Spotify is not running!";
		break;
	case STOPPED:
		this->strComplete = "Nothing is playing currently!";
		break;
	case PLAYING:
		this->strComplete = std::format("Currently playing {} by {}", this->CurrentlyPlaying.strSongname, this->CurrentlyPlaying.strAuthor);
		break;
	}
}
