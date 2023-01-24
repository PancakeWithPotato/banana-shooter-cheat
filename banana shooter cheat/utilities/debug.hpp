#pragma once
#include "includes.hpp"

#ifdef ERROR
#undef ERROR
#endif

enum eLogs {
	SUCCESS = 1,
	ERROR = 2,
	WARNING = 3
};

struct {
	template <typename ... Args>
	void logState(eLogs level, std::string format, Args const& ... args) {
		switch (level) {
		case eLogs::SUCCESS:
			std::cout << SUCCES << "[+] ";
			break;
		case eLogs::ERROR:
			std::cout << ERR << "[-] ";
			break;
		case eLogs::WARNING:
			std::cout << WAIT << "[*] ";
			break;
		}

		printf(format.c_str(), args ...);
		std::cout << RESET << std::endl;
	}

	void setupConsole(const char* title) {
		AllocConsole();

		freopen_s(reinterpret_cast<_iobuf**>(__acrt_iob_func(0)), "conin$", "r", static_cast<_iobuf*>(__acrt_iob_func(0)));
		freopen_s(reinterpret_cast<_iobuf**>(__acrt_iob_func(1)), "conout$", "w", static_cast<_iobuf*>(__acrt_iob_func(1)));

		SetConsoleTitleA(title);
	}

	void destroyConsole() {
		fclose(static_cast<_iobuf*>(__acrt_iob_func(0)));
		fclose(static_cast<_iobuf*>(__acrt_iob_func(1)));

		FreeConsole();
	}
} g_Debug;
