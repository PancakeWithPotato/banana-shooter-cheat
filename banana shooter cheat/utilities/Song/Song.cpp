#include "Song.hpp"

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
	//this method prob sucks but it works so yeah

	static int iTitlelenght = 0;
	static char title[300];
	static DWORD tmpID = 0;
	//get the process id of spotify
	this->m_dwPID = this->GetProcessID();
	//spotify isnt even running
	if (this->m_dwPID == 0)
	{
		this->CurrentState = NOT_RUNNING;
		return{ "", "" };
	}
	//loop through windows
	for (HWND window = GetTopWindow(0); window; window = GetWindow(window, GW_HWNDNEXT))
	{
		//skip window if isnt visible
		if (!IsWindowVisible(window))
			continue;
		iTitlelenght = GetWindowTextLengthA(window);

		//skip if no title
		if (iTitlelenght == 0)
			continue;

		//get process id of window
		GetWindowThreadProcessId(window, &tmpID);

		if (tmpID != this->m_dwPID)
			continue;

		//its spotify
		GetWindowTextA(window, title, iTitlelenght + 1);

		std::string strTitle{ title };
		size_t dash = 0;
		dash = strTitle.find_last_of('-');

		//its running, no song playing tho
		if (dash == std::string::npos)  //changed the magic number
		{
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
	Sleep(1500);
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
		//std::cout << std::format("Currently playing {} by {}\n", this->CurrentlyPlaying.strSongname, this->CurrentlyPlaying.strAuthor);
		break;
	}
}
