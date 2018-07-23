//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "MainUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
TMainForm *MainForm;
//---------------------------------------------------------------------------
UI64    ColorTableSize = 0;
TColor *ColorTable     = NULL;
//---------------------------------------------------------------------------
//UI64    FileInfoTableSize = 0;
//CFileInfo **FileInfoTable = NULL;
//---------------------------------------------------------------------------
CFilesList      FilesList;
CDuplicatesList DuplicatesList;
//---------------------------------------------------------------------------
const String AppDir          = APPDIR;
const String TestDir         = AppDir + "\\Test Folder";
const String ScannedFiles    = AppDir + "\\FilesList.txt";
const String DuplicatedFiles = AppDir + "\\DuplicatesList.txt";
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner):TForm(Owner){}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormCreate(TObject *Sender)
{
	#ifdef TESTMODE
	ListBoxPaths->Items->Add(TestDir);
	#endif
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	ClearData();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TMainForm::ClearData()
{
	ClearColorTable();
	FilesList.Clear();
	DuplicatesList.Clear();
	CheckListBoxDuplicatesFiles->Items->Clear();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TMainForm::EnableForm(bool enabled)
{
	ButtonAdd->Enabled    = enabled;
	ButtonDelete->Enabled = enabled;
	ButtonClear->Enabled  = enabled;
	ButtonScan->Enabled   = enabled;
	ListBoxPaths->Enabled = enabled;

	ButtonDeleteSelectedFiles->Enabled   = enabled;
	CheckListBoxDuplicatesFiles->Enabled = enabled;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButtonAddClick(TObject *Sender)
{
	if(OpenDirectoryForm->ShowModal() == mrOk){

		String dir = OpenDirectoryForm->DirectoryListBox->Directory;

		bool found = false;
		for(int i = 0; i < ListBoxPaths->Items->Count; i++){
			if(ListBoxPaths->Items->Strings[i] == dir){
				found = true;
				break;
			}
		}

		switch(found)
		{
		case false: ListBoxPaths->Items->Add(dir);      break;
		case true:  ShowMessage("Path already added."); break;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButtonDeleteClick(TObject *Sender)
{
	int i = 0;
	while(i < ListBoxPaths->Items->Count){
		if(ListBoxPaths->Selected[i]){
			ListBoxPaths->Items->Delete(i);
		} else {
			i++;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButtonDeleteSelectedFilesClick(TObject *Sender)
{
	static const wchar_t newline = '\n';

	int n = CheckListBoxDuplicatesFiles->Items->Count;
	if(n == 0)
		return;

	int NumFilesSelected = 0;
	for(int i = 0; i < n; i++){
		if(CheckListBoxDuplicatesFiles->Selected[i])
			NumFilesSelected++;
	}

	String cap = "Warning";
	String msg = IntToStr(NumFilesSelected) + " are about to be deleted." + String(newline) + "Continue?";
	if(MessageBoxW(Handle, cap.c_str(), msg.c_str(), MB_YESNOCANCEL) != IDYES)
		return;

	int NumFilesDeleted  = 0;
	for(int i = 0; i < n; i++){

		if(CheckListBoxDuplicatesFiles->Selected[i]){

			String fname = CheckListBoxDuplicatesFiles->Items->Strings[i];

			BOOL res = true;
			//BOOL res = DeleteFileW(fname.c_str());

			if(res)
				NumFilesDeleted++;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButtonClearClick(TObject *Sender)
{
	ListBoxPaths->Items->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButtonScanClick(TObject *Sender)
{
	if(ListBoxPaths->Items->Count == 0){
		ShowMessage("No path to scan.");
		return;
	}

	ScanPaths();
	UI64 res = FindDuplicates();
	ShowMessage(IntToStr((__int64)res) + " duplicates found.");
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TMainForm::ScanPaths()
{
	ClearData();
	for(int i = 0; i < ListBoxPaths->Items->Count; i++){
		String dir = ListBoxPaths->Items->Strings[i];
		ScanDir(dir);
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ScanDir(String &dir)
{
	WIN32_FIND_DATAW fd;
	ZeroMemory(&fd, sizeof(fd));

	String PathToScan = dir + "\\*";

	HANDLE h = FindFirstFile(PathToScan.c_str(), &fd);
	if(h == INVALID_HANDLE_VALUE)
		return;

	while(1){

		String fname = fd.cFileName;

		if(fname == "." || fname == ".."){
			if(!FindNextFile(h, &fd)){
				break;
			} else {
				continue;
			}
		}

		bool IsDirectory = (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) > 0;

		if(!IsDirectory){

			String FileName = dir + "\\" + fname;
			UINT64 FileSize = ((UINT64)fd.nFileSizeHigh << 32) | ((UINT64)fd.nFileSizeLow);

			CFileInfo fi;
			fi.FileName   = FileName;
			fi.FileSize   = FileSize;
			fi.Duplicated = FALSE;

			FilesList.Add(&fi);
		} else {
			String NewPath = dir + "\\" + fname;
			ScanDir(NewPath);
		}

		if(!FindNextFile(h, &fd))
			break;
	}

	FindClose(h);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
int __fastcall TMainForm::CalcProgress(UINT64 i, UINT64 n)
{
	return (int)(((double)i / (double)n) * 100.0);
}
//---------------------------------------------------------------------------
UI64 __fastcall TMainForm::FindDuplicates()
{
	UINT64 i = 0;
	UINT64 n = FilesList.GetSize();

	UINT64 id = 0;
	UINT64 NumDuplicatesFound = 0;

	CFilesListNode *n2 = NULL;
	CFilesListNode *n1 = FilesList.GetFirstNode();

	EnableForm(false);
	ScanGauge->Progress = 0;
	Application->ProcessMessages();

	CSimpleTimer Timer;

	while(n1){

		bool duplicated = false;

		if(n1->IsDuplicated())
			goto Next1;

		n2 = n1->GetNext();

		while(n2){

			DWORD ElapsedTime = Timer.Tick();
			if(ElapsedTime >= 50){

				Timer.Reset();
				Application->ProcessMessages();

				/*if((GetAsyncKeyState(VK_ESCAPE) & 0x8000) > 0){
					ShowMessage("Operation aborted.");
					NumDuplicatesFound = -1;
					goto Abort;
				}*/
			}

			CFileInfo *i1 = n1->GetFileInfo();
			CFileInfo *i2 = n2->GetFileInfo();

			if(i2->Duplicated)
				goto Next2;

			if(i1->FileSize == i2->FileSize){

				if(CompareFiles(i1->FileName, i2->FileName, i1->FileSize)){

					duplicated = true;

					if(i1->Duplicated == FALSE){
						NumDuplicatesFound++;
						i1->Duplicated = TRUE;
						DuplicatesList.Add(i1, id);
					}

					NumDuplicatesFound++;
					i2->Duplicated = TRUE;
					DuplicatesList.Add(i2, id);
				}
			}

			Next2:
			n2 = n2->GetNext();
		}

		if(duplicated)
			id++;

		Next1:
		n1 = n1->GetNext();
		i++;

		ScanGauge->Progress = CalcProgress(i, n);
		Application->ProcessMessages();
	}

	BuildColorTable();

	CDuplicatesListNode *node = DuplicatesList.GetFirstNode();
	while(node){
		String fname = node->GetFileInfo()->FileName;
		CheckListBoxDuplicatesFiles->Items->Add(fname);
		node = node->GetNext();
	}

	Abort:
	EnableForm(true);

	return NumDuplicatesFound;
}
//---------------------------------------------------------------------------
bool __fastcall TMainForm::CompareFiles(String fname1, String fname2, UINT64 fsize)
{
	if(fsize == 0)
		return false;

	bool res = true;

	HANDLE h1 = CreateFileW(fname1.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	HANDLE h2 = CreateFileW(fname2.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	const UINT64 FileSize   = fsize;
	const DWORD  BufferSize = 4096;

	BYTE Buffer1[BufferSize];
	BYTE Buffer2[BufferSize];

	DWORD NumRead1 = 0;
	DWORD NumRead2 = 0;

	for(UINT64 i = 0; i < FileSize; i += BufferSize){

		ZeroMemory(&Buffer1[0], BufferSize);
		ZeroMemory(&Buffer2[0], BufferSize);

		ReadFile(h1, &Buffer1[0], BufferSize, &NumRead1, NULL);
		ReadFile(h2, &Buffer2[0], BufferSize, &NumRead2, NULL);

		if(NumRead1 != NumRead2 || NumRead1 == 0 || NumRead2 == 0){
			res = false;
			break;
		}

		if(memcmp(&Buffer1[0], &Buffer2[0], NumRead1) != 0){
			res = false;
			break;
		}
	}

	if(h1 != INVALID_HANDLE_VALUE){CloseHandle(h1);}
	if(h2 != INVALID_HANDLE_VALUE){CloseHandle(h2);}

	return res;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TMainForm::BuildColorTable()
{
	static const TColor Color1 = TColor(0x00FFE0E0);
	static const TColor Color2 = TColor(0x00E0FFE0);

	ClearColorTable();

	ColorTableSize = DuplicatesList.GetSize();
	if(ColorTableSize == 0)
		return;

	ColorTable = new TColor[ColorTableSize];
	ZeroMemory(&ColorTable[0], ColorTableSize * sizeof(TColor));

	CDuplicatesListNode* node = DuplicatesList.GetFirstNode();

	UINT i = 0;
	while(node){
		bool IsOdd = node->GetID() % 2 != 0;
		ColorTable[i++] = IsOdd ? Color1 : Color2;
		node = node->GetNext();
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ClearColorTable()
{
	ColorTableSize = 0;
	SAFE_DELETE(ColorTable);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
/*void __fastcall TMainForm::BuildFileInfoTable()
{
	ClearFileInfoTable();

	FileInfoTableSize = FilesList.GetSize();
	if(FileInfoTableSize == 0)
		return;

	FileInfoTable = new CFileInfo*[FileInfoTableSize];
	ZeroMemory(&(*FileInfoTable[0]), FileInfoTableSize * sizeof(CFileInfo*));

	CFilesListNode* node = FilesList.GetFirstNode();

	UINT i = 0;
	while(node){
		CFileInfo *info = node->GetFileInfo();
		&(*FileInfoTable[i++]) = info;
		node = node->GetNext();
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ClearFileInfoTable()
{
	FileInfoTableSize = 0;
	SAFE_DELETE(*FileInfoTable);
}*/
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TMainForm::CheckListBoxDuplicatesFilesDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State)
{
	TStrings *Items  = CheckListBoxDuplicatesFiles->Items;
	TCanvas  *Canvas = CheckListBoxDuplicatesFiles->Canvas;

	TColor OldPen   = Canvas->Pen->Color;
	TColor OldFont  = Canvas->Font->Color;
	TColor OldBrush = Canvas->Brush->Color;

	String txt = Items->Strings[Index];
	TColor col = ColorTable && Index < (int)ColorTableSize ? ColorTable[Index] : clWindow;

	Canvas->Pen->Color   = col;
	Canvas->Brush->Color = col;
	Canvas->Font->Color  = clWindowText;

	Canvas->Rectangle(Rect);
	Canvas->TextOutW(Rect.Left+1, Rect.Top+1, txt.c_str());
	if(CheckListBoxDuplicatesFiles->State[Index] == odFocused)
		Canvas->DrawFocusRect(Rect);

	Canvas->Pen->Color   = OldPen;
	Canvas->Font->Color  = OldFont;
	Canvas->Brush->Color = OldBrush;
}
//---------------------------------------------------------------------------

