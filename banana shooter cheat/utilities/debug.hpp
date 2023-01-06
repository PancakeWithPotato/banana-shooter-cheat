#pragma once
#include "includes.hpp"

enum eLogs {
	success = 1,
	error = 2, // pancake: lowercase enum names are generally bad practice, but wingdi.h already defines ERROR to 0
	warning = 3
};

struct {
	template <typename ... Args>
	void logState(eLogs level, std::string format, Args const& ... args) {
		switch (level) {
		case eLogs::success:
			std::cout << SUCCES << "[+] ";
			break;
		case eLogs::error:
			std::cout << ERR << "[-] ";
			break;
		case eLogs::warning:
			std::cout << WAIT << "[*] ";
			break;
		}

		printf(format.c_str(), args ...); // std::cout doesnt like args ... wtf !!
		std::cout << RESET << std::endl;
	}

	void setupConsole(const char* title) {
		AllocConsole();

		freopen_s(reinterpret_cast<_iobuf**>(__acrt_iob_func(0)), "conin$", "r", static_cast<_iobuf*>(__acrt_iob_func(0)));
		freopen_s(reinterpret_cast<_iobuf**>(__acrt_iob_func(1)), "conout$", "w", static_cast<_iobuf*>(__acrt_iob_func(1)));
		freopen_s(reinterpret_cast<_iobuf**>(__acrt_iob_func(2)), "conout$", "w", static_cast<_iobuf*>(__acrt_iob_func(2)));

		SetConsoleTitleA(title);
	}

	void DestroyConsole() {
		fclose(static_cast<_iobuf*>(__acrt_iob_func(0)));
		fclose(static_cast<_iobuf*>(__acrt_iob_func(1)));
		fclose(static_cast<_iobuf*>(__acrt_iob_func(2)));

		FreeConsole();
	}
} g_Debug;