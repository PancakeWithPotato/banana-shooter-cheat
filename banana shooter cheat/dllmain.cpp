#include "utilities/includes.hpp"
#include "core/hack.hpp"

DWORD WINAPI DllEntry( LPVOID lpThreadParameter )
{
	if (!g_Hack->Setup())
		g_Debug.logState(::error, "Failed to setup!");

	while (!GetAsyncKeyState(VK_END))
		std::this_thread::sleep_for(std::chrono::milliseconds(500));

	g_Hack->shouldUnload = true;
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	FreeLibraryAndExitThread(static_cast< HMODULE > ( lpThreadParameter ), 0); 
}

// pasted from https://learn.microsoft.com/en-us/windows/win32/dlls/dllmain
BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,  // handle to DLL module
    DWORD fdwReason,     // reason for calling function
    LPVOID lpvReserved )  // reserved
{
    // Perform actions based on the reason for calling.
    switch( fdwReason ) 
    { 
        case DLL_PROCESS_ATTACH:
            // Initialize once for each new process.
            // Return FALSE to fail DLL load.
            DisableThreadLibraryCalls( hinstDLL );

	    if ( HANDLE handle = CreateThread( nullptr, NULL, DllEntry, hinstDLL, NULL, nullptr ) )
	    {
		CloseHandle( handle );
            }
            break;

        case DLL_THREAD_ATTACH:
            // Do thread-specific initialization.
            break;

        case DLL_THREAD_DETACH:
            // Do thread-specific cleanup.
            break;

        case DLL_PROCESS_DETACH:
        
            if (lpvReserved != nullptr)
            {
                break; // do not do cleanup if process termination scenario
            }
            
            // Perform any necessary cleanup.
	    g_Hack->Destroy();
	    delete g_Hack;
            break;
    }
	
    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}
