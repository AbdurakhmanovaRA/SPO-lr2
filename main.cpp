#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <locale.h>
#include <iomanip>
#include <string>

using namespace std;

#define BUF_SIZE 256
TCHAR szName[] = TEXT("Global\\MyFileMappingObject");
TCHAR Msg[100];

int main()
{
    setlocale(LC_ALL, "RUS");
    //1
    cout << "Enter mapping name: \n";
    cin >> szName;
    HANDLE hMapFile;
    LPCTSTR pBuf;
    DWORD errcode;
    //2
    hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE,szName);

    errcode = GetLastError();

    if (hMapFile == NULL)
    {
        cout << "Could not open file mapping object. Error " << errcode << endl;
        cout << "Creating new mapping " << szName << endl;
        hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, BUF_SIZE, szName);
    }
    else
    {
        cout << "Connecting to " << szName << endl;
    }


    //3
    pBuf = (LPTSTR) MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, BUF_SIZE);
    if (pBuf == NULL)
   {
      cout <<"Could not map view of file. Error " << GetLastError() << endl;
      CloseHandle(hMapFile);
      return 1;
   }
    //4

    int n_act=0;

    while (n_act < 3)
    {
        cout << "Enter 1 for writing, 2 for reading, 3 for exit: \n";
        cin >> n_act;
        switch (n_act){
            case 1:
                cout << "Enter the message: ";
                cin >> Msg;
                CopyMemory((PVOID)pBuf, Msg, (_tcslen(Msg) + 1) * sizeof(TCHAR));
                break;

            case 2:
                cout << "Message: " << pBuf << endl;
                break;

            case 3:
                UnmapViewOfFile(pBuf);
                CloseHandle(hMapFile);
                break;
        }
    }

    return 0;

}


