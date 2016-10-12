//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include <psapi.h>
#pragma comment (lib,"Psapi.lib")

#include <tlhelp32.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::PrintProcessNameAndID(DWORD processID, APP_PROCINFO *process)
{
    // Get a handle to the process.
	HANDLE hProcess = OpenProcess( PROCESS_ALL_ACCESS,
								   TRUE, processID );

	// Get the process name.
	if (NULL != hProcess )
	{
		HMODULE hMod;
		DWORD cbNeeded;

		if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod),
			 &cbNeeded) )
		{
			process->pid = processID;

			GetModuleBaseName( hProcess, hMod, process->szProcessName,
							   sizeof(process->szProcessName)/sizeof(TCHAR) );
			GetModuleFileNameEx( hProcess, hMod, process->szFullPath,
							   sizeof(process->szFullPath)/sizeof(TCHAR) );
		}
	}

	// Release the handle to the process.
	CloseHandle( hProcess );
}

wchar_t *__fastcall TForm1::GetProcessName(DWORD processID)
{
	DWORD NameSize = 0;
	wchar_t lpBaseName[MAX_PATH];
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);

	if (hProcess != NULL) // If the process couldn't be opened, it's probably a system process and we shouldn't terminate it
	{
		// This will tell us the name of the current process
		NameSize = GetModuleFileNameEx(hProcess, NULL, lpBaseName, MAX_PATH);

		// For information only:
		lpBaseName[NameSize] = '\0';

		//cout << "Name: " << lpBaseName << "\tPID:" << pProcessIds[i] << endl;
		CloseHandle(hProcess);
	}

	if (NameSize == 0) {
		return L"Error";
	}
	return lpBaseName;
}

void __fastcall TForm1::Button1Click(TObject *Sender)
{
	ListBox1->Clear();
	ListBox1->Sorted = true;

	// Get the list of process identifiers.
	static DWORD aProcesses[1024];
	DWORD cbNeeded = 0, cProcesses = 0;
	ZeroMemory(aProcesses, sizeof(aProcesses));

	if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
	{
		return;
	}

	// Calculate how many process identifiers were returned.
	cProcesses = cbNeeded / sizeof(DWORD);

	OutputDebugString(L"\n----------------------------");
	// Print the name and process identifier for each process.
	for ( unsigned int i = 0; i < cProcesses; i++ )
	{
		if( aProcesses[i] != 0 )
		{
			static APP_PROCINFO pp;
			ZeroMemory(&pp, sizeof(APP_PROCINFO));
			//PrintProcessNameAndID( aProcesses[i], &pp );
			//if (wcslen(pp.szProcessName))
			//{
			//	ListBox1->Items->Add(pp.szProcessName);
			//}

			wchar_t *procName = GetProcessName(aProcesses[i]);
			if (wcslen(procName))
			{
				ListBox1->Items->Add(procName);
			}
		}
	}
	OutputDebugString(L"\n----------------------------");
}
//---------------------------------------------------------------------------
bool __fastcall TForm1::ListProcess()
{
	ListBox1->Clear();
	ListBox1->Sorted = false;

	HANDLE hsnapprocess;
	HANDLE hprocess;
	PROCESSENTRY32 entry32;
	DWORD Size;

	// 1. 取得系統 process 之 snapshot
	hsnapprocess = CreateToolhelp32Snapshot(
				   TH32CS_SNAPPROCESS, // flag
				   0); // th32ProcessID

	if(hsnapprocess==INVALID_HANDLE_VALUE)
	{
		//printf("CreateToolhelp32Snapshot fail: %d\n",GetLastError());
		return false;
	}

	// 2. 設定 struct 大小
	entry32.dwSize = sizeof(PROCESSENTRY32);

	// 3. 確認正確取得第一個 process
	if(!Process32First(hsnapprocess, &entry32))
	{
		//printf("Process32First fail.\n");
		CloseHandle(hsnapprocess);
		return false;
	}

	// 4. 顯示 process 相關訊息
	do{
		ListBox1->Items->Add(entry32.szExeFile);
		/*
		OutputDebugString(L"\n\n===============================\n");
		OutputDebugString(L"Name      :%s\n", entry32.szExeFile);
		OutputDebugString(L"PID       :%08x\n", entry32.th32ProcessID);
		OutputDebugString(L"parent  ID:%08x\n", entry32.th32ParentProcessID);
		OutputDebugString(L"thread cnt:%u\n", entry32.cntThreads);
		OutputDebugString(L"base   pri:%ld\n", entry32.pcPriClassBase);
		*/
	}while(Process32Next(hsnapprocess, &entry32));
	CloseHandle(hsnapprocess);
	return true;
}


void __fastcall TForm1::Button2Click(TObject *Sender)
{
 	ListProcess();
}
//---------------------------------------------------------------------------

