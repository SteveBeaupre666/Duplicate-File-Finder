//---------------------------------------------------------------------------
#ifndef MainUnitH
#define MainUnitH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
//---------------------------------------------------------------------------
#include <Vcl.Forms.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.CheckLst.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Samples.Gauges.hpp>
//---------------------------------------------------------------------------
#include "OpenDirectoryUnit.h"
//---------------------------------------------------------------------------
#include "TxtFile.h"
#include "LinkedList.h"
//---------------------------------------------------------------------------
#define UI64   unsigned __int64
#define UINT64 unsigned __int64
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GroupBoxSearchPaths;
	TListBox *ListBoxPaths;
	TButton *ButtonAdd;
	TButton *ButtonDelete;
	TButton *ButtonClear;
	TGroupBox *GroupBoxScanPaths;
	TButton *ButtonScan;
	TGauge *ScanPathsGauge;
	TGroupBox *GroupBoxDuplicates;
	TCheckListBox *CheckListBoxDuplicatesFiles;
	void __fastcall ButtonScanClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall ButtonAddClick(TObject *Sender);
	void __fastcall ButtonDeleteClick(TObject *Sender);
	void __fastcall ButtonClearClick(TObject *Sender);
private:	// User declarations
	//void __fastcall SaveFilesList(char *fname);
	void __fastcall ScanFolders();
	void __fastcall Scan(String path);
	UI64 __fastcall FindDuplicates();
	bool __fastcall CompareFiles(String fname1, String fname2, UINT64 fsize);
public:		// User declarations
	__fastcall TMainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
