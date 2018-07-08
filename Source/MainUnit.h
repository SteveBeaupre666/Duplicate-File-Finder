//---------------------------------------------------------------------------
#ifndef MainUnitH
#define MainUnitH
//---------------------------------------------------------------------------
#include <Vcl.Forms.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.CheckLst.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Samples.Gauges.hpp>
#include <System.Classes.hpp>
//---------------------------------------------------------------------------
#include "OpenDirectoryUnit.h"
//---------------------------------------------------------------------------
#include "TxtFile.h"
#include "FilesList.h"
#include "DuplicatesList.h"
//#include "LinkedList.h"
//---------------------------------------------------------------------------
#define UI64   unsigned __int64
#define UINT64 unsigned __int64
//---------------------------------------------------------------------------
#define SAFE_DELETE(x)  if(x){delete x;x=NULL;}
//---------------------------------------------------------------------------
#define APPDIR "C:\\New Programming Folder\\Programs\\Duplicate File Finder"
//---------------------------------------------------------------------------
#define TESTMODE
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GroupBoxSearchPaths;
	TListBox *ListBoxPaths;
	TButton *ButtonAdd;
	TButton *ButtonDelete;
	TButton *ButtonClear;
	TButton *ButtonScan;
	TCheckListBox *CheckListBoxDuplicatesFiles;
	TLabel *Label1;
	void __fastcall ButtonScanClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall ButtonAddClick(TObject *Sender);
	void __fastcall ButtonDeleteClick(TObject *Sender);
	void __fastcall ButtonClearClick(TObject *Sender);
	void __fastcall CheckListBoxDuplicatesFilesDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
private:	// User declarations
	TColor __fastcall GetColor(BYTE r, BYTE g, BYTE b, BYTE a = 0);
	String __fastcall MakePaths(String s1, String s2);

	void __fastcall ScanPaths();
	void __fastcall ScanDir(String &dir);
	bool __fastcall SaveFilesList(const String &fname);

	UI64 __fastcall FindDuplicates();
	bool __fastcall CompareFiles(String fname1, String fname2, UINT64 fsize);

	void __fastcall BuildColorTable();
	void __fastcall ClearColorTable();
public:		// User declarations
	__fastcall TMainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
