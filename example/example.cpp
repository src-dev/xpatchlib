//Example usage of xipslib.cpp

//Set paths to files
#define IPS_FILE "F:\\example\\patch.ips"
#define SRC_FILE "F:\\example\\default.xbe"
#define BAK_EXT  "bak"
#define LOG_FILE "F:\\example\\debug.log"

#include <stdio.h>
#include <xtl.h>
#include <time.h>
#include "xipslib.h"

bool openLog();
void closeLog();
void logEntry(char ENTRY[]);
void ReturnToDash();
void mountAllDrives();

FILE* lf = NULL;

//Program entry point
VOID __cdecl main() {
	
	mountAllDrives();
	openLog();

	logEntry("Creating backup file..");
	switch (createBak(SRC_FILE, BAK_EXT, true)) {
		case 0:
			logEntry("Backup file created successfully.");
			break;
		case E_FOPEN_SRC: 
			logEntry("Error opening source file!");
			closeLog();
			ReturnToDash();
			return; 
		case E_FOPEN_DST:
			logEntry("Error creating backup file!");
			closeLog();
			ReturnToDash();
			return;
	}

	logEntry("Patching source file..");
	switch(applyIPS(IPS_FILE, SRC_FILE)) {
		case E_NO_ERROR: 
			logEntry("Source file patched successfully.");
			break;
		case E_FOPEN_IPS:
			logEntry("Error opening IPS file!");
			closeLog();
			ReturnToDash();
			return;
		case E_FOPEN_SRC:
			logEntry("Error opening source file!");
			closeLog();
			ReturnToDash();
			return;
		case E_NOT_IPS:
			logEntry("The IPS file is not a valid IPS file!");
			closeLog();
			ReturnToDash();
			return;
	}

	closeLog();
	ReturnToDash();
	return;
}



//Log file functions
bool openLog() {
	if (LOG_FILE != NULL) {
		lf = fopen(LOG_FILE, "a");
		if (!lf) return false;
		logEntry("Log file opened.");
		return true;
	}
}
void closeLog() {
	if (lf != NULL) {
		logEntry("Closing log file..");
		fclose(lf);
	}
}
void logEntry(char ENTRY[]) {
	if (lf != NULL) {
		time_t now = time(NULL);
		struct tm* lt = localtime(&now);
		char buf[80];
		strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", lt);
		fwrite(buf, sizeof(char), strlen(buf), lf);
		fputc(' ', lf);
		fwrite(ENTRY, sizeof(char), strlen((char*)ENTRY), lf);
		fputc('\n', lf);
	}
}

//Return to dashboard
extern "C" VOID __stdcall HalReturnToFirmware(ULONG Routine); // kernel export
static __forceinline void ReturnToDash() {
	// 2 == HalQuickReboot (returns to dash on retail kernels)
	//HalReturnToFirmware(2);
	XLaunchNewImage(NULL, NULL);

	// Fallback (should never return)
	for (;;)
		Sleep(INFINITE);
}

// Helper functions
// Everything below here is used for hard drive access

typedef struct _STRING {
	USHORT	Length;
	USHORT	MaximumLength;
	PSTR	Buffer;
} UNICODE_STRING, * PUNICODE_STRING, ANSI_STRING, * PANSI_STRING;
extern "C" {
	XBOXAPI LONG WINAPI IoCreateSymbolicLink(IN PUNICODE_STRING SymbolicLinkName, IN PUNICODE_STRING DeviceName);
	XBOXAPI LONG WINAPI IoDeleteSymbolicLink(IN PUNICODE_STRING SymbolicLinkName);
}
struct pathconv_s {
	char* DriveLetter;
	char* FullPath;
} pathconv_table[] = {
	{ "DVD:", "\\Device\\Cdrom0" },// Can't use D:
	{ "C:", "\\Device\\Harddisk0\\Partition2" },
	{ "E:", "\\Device\\Harddisk0\\Partition1" },
	{ "F:", "\\Device\\Harddisk0\\Partition6" },
	{ "G:", "\\Device\\Harddisk0\\Partition7" },
	{ "X:", "\\Device\\Harddisk0\\Partition3" },
	{ "Y:", "\\Device\\Harddisk0\\Partition4" },
	{ "Z:", "\\Device\\Harddisk0\\Partition5" },
	{ NULL, NULL }
};
#define DeviceC "\\Device\\Harddisk0\\Partition2"
#define DeviceE "\\Device\\Harddisk0\\Partition1"
#define CdRom "\\Device\\Cdrom0"
#define DeviceX "\\Device\\Harddisk0\\Partition3"
#define DeviceY "\\Device\\Harddisk0\\Partition4"
#define DeviceZ "\\Device\\Harddisk0\\Partition5"
#define DeviceF "\\Device\\Harddisk0\\Partition6"
#define DeviceG "\\Device\\Harddisk0\\Partition7"
#define DriveC "\\??\\C:"
#define DriveD "\\??\\D:"
#define DriveE "\\??\\E:"
#define DriveF "\\??\\F:"
#define DriveG "\\??\\G:"
#define DriveX "\\??\\X:"
#define DriveY "\\??\\Y:"
#define DriveZ "\\??\\Z:"
LONG MountDevice(LPSTR sSymbolicLinkName, char* sDeviceName) {
	UNICODE_STRING 	deviceName;
	deviceName.Buffer = (PSTR)sDeviceName;
	deviceName.Length = (USHORT)strlen(sDeviceName);
	deviceName.MaximumLength = (USHORT)strlen(sDeviceName) + 1;
	UNICODE_STRING 	symbolicLinkName;
	symbolicLinkName.Buffer = sSymbolicLinkName;
	symbolicLinkName.Length = (USHORT)strlen(sSymbolicLinkName);
	symbolicLinkName.MaximumLength = (USHORT)strlen(sSymbolicLinkName) + 1;
	return IoCreateSymbolicLink(&symbolicLinkName, &deviceName);
}
LONG UnMountDevice(LPSTR sSymbolicLinkName) {
	UNICODE_STRING 	symbolicLinkName;
	symbolicLinkName.Buffer = sSymbolicLinkName;
	symbolicLinkName.Length = (USHORT)strlen(sSymbolicLinkName);
	symbolicLinkName.MaximumLength = (USHORT)strlen(sSymbolicLinkName) + 1;
	return IoDeleteSymbolicLink(&symbolicLinkName);
}
void mountAllDrives() {
	UnMountDevice(DriveX);
	UnMountDevice(DriveY);
	UnMountDevice(DriveZ);
	UnMountDevice(DriveC);
	UnMountDevice(DriveE);
	UnMountDevice(DriveF);
	UnMountDevice(DriveG);
	MountDevice(DriveX, DeviceX);
	MountDevice(DriveY, DeviceY);
	MountDevice(DriveZ, DeviceZ);
	MountDevice(DriveC, DeviceC);
	MountDevice(DriveE, DeviceE);
	MountDevice(DriveF, DeviceF);
	MountDevice(DriveG, DeviceG);
}