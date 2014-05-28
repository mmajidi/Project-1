#include <windows.h>
#include <stdio.h>
#include <time.h>

 
int main(void)
{
HANDLE hFile;
HANDLE hAppend;
DWORD dwBytesRead, dwBytesWritten, dwPos;
// Make sure the files are there...
LPCWSTR fname = L"c:\\mytestfile.txt";
LPCWSTR fname2 = L"c:\\mytestfiletwo.txt";
char buff[4096];
 
// Open the existing file.
hFile = CreateFile(fname, // open testfile.txt
GENERIC_READ, // open for reading
0, // do not share
NULL, // default security
OPEN_EXISTING, // existing file only
FILE_ATTRIBUTE_NORMAL, // normal file
NULL); // no attribute template
 
if(hFile == INVALID_HANDLE_VALUE)
printf("Could not open %S lol!.\n", fname);
else
printf("%S opened successfully.\n", fname);
 
// Open the existing file, or if the file does not exist,
// create a new file.
hAppend = CreateFile(fname2, // open mytestfiletwo.txt
GENERIC_WRITE, // open for writing
0, // do not share
NULL, // default security
OPEN_ALWAYS, // open or create
FILE_ATTRIBUTE_NORMAL, // normal file
NULL); // no attribute template
 
if(hAppend == INVALID_HANDLE_VALUE)
printf("Could not open %S lol!.\n", fname2);
else
{
printf("%S opened/created successfully.\n", fname2);
printf("\nAppending %S\'s content to %S\'s content\n", fname, fname2);
printf("Check the %S content lol!\n\n", fname2);
}
 
// Append the first file to the end of the second file.
// Lock the second file to prevent another process from
// accessing it while writing to it. Unlock the file when writing is finished.
do
{
if(ReadFile(hFile, buff, 4096, &dwBytesRead, NULL))
{
dwPos = SetFilePointer(hAppend, 0, NULL, FILE_END);
 
WriteFile(hAppend, buff, dwBytesRead, &dwBytesWritten, NULL);
 
}
} while (dwBytesRead == 4096);
 
// Close both files.
if(CloseHandle(hFile) != 0)
printf("\n%S file handle closed successfully!\n", fname);
if(CloseHandle(hAppend) != 0)
printf("%S file handle closed successfully!\n", fname2);
return 0;
}
 