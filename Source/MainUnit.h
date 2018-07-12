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
#include <Vcl.ComCtrls.hpp>
#include <System.Classes.hpp>
//---------------------------------------------------------------------------
#include "OpenDirectoryUnit.h"
//---------------------------------------------------------------------------
#include "TxtFile.h"
#include "FilesList.h"
#include "DuplicatesList.h"
#include "SimpleTimer.h"
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
	TButton *ButtonScan;
	TCheckListBox *CheckListBoxDuplicatesFiles;
	TLabel *Label1;
	TGauge *ScanGauge;
	TMainMenu *MainMenu;
	TMenuItem *File1;
	TMenuItem *Exit1;
	TMenuItem *N1;
	TPopupMenu *PopupMenu;
	TButton *ButtonDeleteFiles;
	TButton *ButtonAdd;
	TButton *ButtonClear;
	TButton *ButtonDelete;
	TListBox *ListBoxPaths;
	TLabel *Label2;
	void __fastcall ButtonScanClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall ButtonAddClick(TObject *Sender);
	void __fastcall ButtonDeleteClick(TObject *Sender);
	void __fastcall ButtonClearClick(TObject *Sender);
	void __fastcall CheckListBoxDuplicatesFilesDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
private:
	void __fastcall ClearData();
	void __fastcall EnableForm(bool enabled);

	bool __fastcall ScanPaths();
	void __fastcall ScanDir(String &dir);

	UI64 __fastcall FindDuplicates();
	bool __fastcall CompareFiles(String fname1, String fname2, UINT64 fsize);

	void __fastcall BuildColorTable();
	void __fastcall ClearColorTable();
public:
	__fastcall TMainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
