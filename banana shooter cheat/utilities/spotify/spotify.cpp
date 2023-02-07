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
	static int iTitlelenght = 0;
	static char title[300];
	static DWORD tmpID = 0;
	this->m_dwPID = this->GetProcessID();

	if (this->m_dwPID == 0)
	{
		this->CurrentState = NOT_RUNNING;
		return{ "", "" };
	}

	for (HWND window = GetTopWindow(0); window; window = GetWindow(window, GW_HWNDNEXT))
	{
		if (!IsWindowVisible(window))
			continue;

		iTitlelenght = GetWindowTextLengthA(window);


		if (iTitlelenght == 0)
			continue;

		GetWindowThreadProcessId(window, &tmpID);

		if (tmpID != this->m_dwPID)
			continue;

		GetWindowTextA(window, title, iTitlelenght + 1);

		std::string strTitle{ title };
		size_t dash = 0;
		dash = strTitle.find_last_of('-');

		if (dash == std::string::npos) {
			this->CurrentState = STOPPED;
			return { "", "" };
		}

		this->CurrentState = PLAYING;
		return { strTitle.substr(0, dash - 1), strTitle.substr(dash + 2, strTitle.size()) };
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
