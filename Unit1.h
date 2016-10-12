//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

struct APP_PROCINFO
{
	DWORD pid;
	TCHAR szProcessName[MAX_PATH];
	TCHAR szFullPath[MAX_PATH];
//	TCHAR szText[MAX_PATH];
};

//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TButton *Button1;
	TListBox *ListBox1;
	TButton *Button2;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
private:	// User declarations
	void __fastcall PrintProcessNameAndID(DWORD processID, APP_PROCINFO *process);
	wchar_t *__fastcall GetProcessName(DWORD processID);
	bool __fastcall TForm1::ListProcess();
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
