#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <tlhelp32.h>

int main() 
{

    HANDLE hSnap;

    PROCESSENTRY32 pe;


    hSnap=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);

    if (hSnap==INVALID_HANDLE_VALUE)

         return 1;


    pe.dwSize=sizeof(pe);

    if (Process32First(hSnap, &pe))

    do {

          MODULEENTRY32 me;

          HANDLE hMod;


          if (pe.th32ProcessID==0)

     continue;


          hMod=CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,pe.th32ProcessID); 

          if (hMod==INVALID_HANDLE_VALUE) 

     continue;


         me.dwSize = sizeof(me); 

         if (Module32First(hMod, &me))

			 printf("%6d %-15s %s %6d\n",pe.th32ProcessID,me.szModule,me.szExePath, pe.th32ParentProcessID);


         CloseHandle(hMod); 

     } while (Process32Next(hSnap,&pe));


     CloseHandle(hSnap);

     return 0;

}