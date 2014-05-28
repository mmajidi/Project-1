#include <windows.h>
#include <stdio.h>
#include <time.h>

 
int main()
{

char buffer[]="Write this text to file";
DWORD dwWritten; // number of bytes written to file
HANDLE hFile;  

hFile=CreateFile(TEXT("C:\\test.txt"),GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
if(hFile==INVALID_HANDLE_VALUE)
  {
    
    return 1;
  }
WriteFile(hFile,buffer,sizeof(buffer),&dwWritten,0);
CloseHandle(hFile);


return 0;
}

