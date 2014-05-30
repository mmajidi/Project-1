#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <tlhelp32.h>


typedef NTSTATUS (NTAPI *_NtQueryInformationProcess)(
    HANDLE ProcessHandle,
    DWORD ProcessInformationClass,
    PVOID ProcessInformation,
    DWORD ProcessInformationLength,
    PDWORD ReturnLength
    );

typedef struct _UNICODE_STRING
{
    USHORT Length;
    USHORT MaximumLength;
    PWSTR Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

typedef struct _PROCESS_BASIC_INFORMATION
{
    LONG ExitStatus;
    PVOID PebBaseAddress;
    ULONG_PTR AffinityMask;
    LONG BasePriority;
    ULONG_PTR UniqueProcessId;
    ULONG_PTR ParentProcessId;
} PROCESS_BASIC_INFORMATION, *PPROCESS_BASIC_INFORMATION;

PVOID GetPebAddress(HANDLE ProcessHandle)
{
    _NtQueryInformationProcess NtQueryInformationProcess =
        (_NtQueryInformationProcess)GetProcAddress(
        GetModuleHandleA("ntdll.dll"), "NtQueryInformationProcess");
    PROCESS_BASIC_INFORMATION pbi;

    NtQueryInformationProcess(ProcessHandle, 0, &pbi, sizeof(pbi), NULL);

    return pbi.PebBaseAddress;
}




int main() 
{

    HANDLE hSnap;

    PROCESSENTRY32 pe;

	/*char buffer;*/
	DWORD dwWritten; // number of bytes written to file
	HANDLE hFile;  //for Writefile func


	int pid;
    HANDLE processHandle;
    PVOID pebAddress;
    PVOID rtlUserProcParamsAddress;
    UNICODE_STRING commandLine;
    WCHAR *commandLineContents;


    hSnap=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);

    if (hSnap==INVALID_HANDLE_VALUE)

         return 1;


    pe.dwSize=sizeof(pe);

    if (Process32First(hSnap, &pe))
	hFile=CreateFile(TEXT("D:\\code\\test.txt"),GENERIC_WRITE|FILE_APPEND_DATA,FILE_SHARE_WRITE,0,OPEN_ALWAYS
	,FILE_ATTRIBUTE_NORMAL,0);
    do {

          MODULEENTRY32 me;

          HANDLE hMod;

		  pid = pe.th32ProcessID;

          if (pe.th32ProcessID==0)

     continue;


          hMod=CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,pe.th32ProcessID); 

          if (hMod==INVALID_HANDLE_VALUE) 

     continue;


		  if ((processHandle = OpenProcess(
        PROCESS_QUERY_INFORMATION | /* required for NtQueryInformationProcess */
        PROCESS_VM_READ, /* required for ReadProcessMemory */
        FALSE, pid)) == 0)
    {
        printf("Could not open process!\n");
        return GetLastError();
    }

    pebAddress = GetPebAddress(processHandle);

    /* get the address of ProcessParameters */
    if (!ReadProcessMemory(processHandle, (PCHAR)pebAddress + 0x10,
        &rtlUserProcParamsAddress, sizeof(PVOID), NULL))
    {
        printf("Could not read the address of ProcessParameters!\n");
        return GetLastError();
    }

    /* read the CommandLine UNICODE_STRING structure */
    if (!ReadProcessMemory(processHandle, (PCHAR)rtlUserProcParamsAddress + 0x40,
        &commandLine, sizeof(commandLine), NULL))
    {
        printf("Could not read CommandLine!\n");
        return GetLastError();
    }

    /* allocate memory to hold the command line */
    commandLineContents = (WCHAR *)malloc(commandLine.Length);

    /* read the command line */
    if (!ReadProcessMemory(processHandle, commandLine.Buffer,
        commandLineContents, commandLine.Length, NULL))
    {
        printf("Could not read the command line string!\n");
        return GetLastError();
    }


         me.dwSize = sizeof(me); 

         Module32First(hMod, &me);


			 
			
		 char buffer[4048]; 
		 memset(buffer, '\0', sizeof(buffer));

		 sprintf(buffer, "\n====================\nPID:%6d szmodule:%-15s szExePath:%s ParentPID:%6d CommandLine:%.*S\n",pe.th32ProcessID,me.szModule,me.szExePath, pe.th32ParentProcessID, commandLine.Length / 2, commandLineContents);
		 WriteFile(hFile,buffer,strlen(buffer),&dwWritten,0);
		 //printf("%s", buffer);

		
		 

		 CloseHandle(processHandle);
		 free(commandLineContents);
         CloseHandle(hMod); 

     } while (Process32Next(hSnap,&pe));

	CloseHandle(hFile);
     CloseHandle(hSnap);

     return 0;

}