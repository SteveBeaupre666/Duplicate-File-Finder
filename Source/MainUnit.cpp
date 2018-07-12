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
UI64    TableSize  = 0;
TColor *ColorTable = NULL;
//---------------------------------------------------------------------------
CFilesList      FilesList;
CDuplicatesList DuplicatesList;
//---------------------------------------------------------------------------
const String AppDir          = APPDIR;
const String TestDir         = AppDir + "\\Test Folder";
const String ScannedFiles    = AppDir + "\\FilesList.txt";
const String DuplicatedFiles = AppDir + "\\DuplicatesList.txt";
//---------------------------------------------------------------------------
//const wchar_t newline = '\n';
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner):TForm(Owner){}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormCreate(TObject *Sender)
{
	#ifdef TESTMODE
	//ListBoxPaths->Items->Add(TestDir);
	ListBoxPaths->Items->Add("C:\\Temp\\Tounes du Telephone");
	#endif

//	CheckListBoxDuplicatesFiles->Style = lbStandard;
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
	CheckListBoxDuplicatesFiles->Items->Clear();
	ClearColorTable();
	FilesList.Clear();
	DuplicatesList.Clear();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TMainForm::EnableForm(bool enabled)
{
	ButtonAdd->Enabled = enabled;
	ButtonDelete->Enabled = enabled;
	ButtonClear->Enabled = enabled;
	ButtonScan->Enabled = enabled;
	ButtonDeleteFiles->Enabled = enabled;
	ListBoxPaths->Enabled = enabled;
	CheckListBoxDuplicatesFiles->Enabled = enabled;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButtonAddClick(TObject *Sender)
{
	if(OpenDirectoryForm->ShowModal() == mrOk){
		String dir = OpenDirectoryForm->DirectoryListBox->Directory;
		ListBoxPaths->Items->Add(dir);
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButtonDeleteClick(TObject *Sender)
{
	int i = 0;
	while(i < ListBoxPaths->Items->Count){

		if(ListBoxPaths->Selected[i]){
			i++;
			continue;
		}

		ListBoxPaths->Items->Delete(i);
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
	ScanPaths();
	FindDuplicates();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
bool __fastcall TMainForm::ScanPaths()
{
	ClearData();

	int n = ListBoxPaths->Items->Count;
	if(n == 0)
		return false;

	for(int i = 0; i < n; i++){
		String path = ListBoxPaths->Items->Strings[i];
		ScanDir(path);
	}

	return true;
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
			if(fname == "Mix"){
				_asm {
					nop
				}
			}
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

		int progress = (int)(((float)i / (float)n) * 100.0f);
		ScanGauge->Progress = progress;
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

	DWORD NumBytesRead1 = 0;
	DWORD NumBytesRead2 = 0;

	for(UINT64 i = 0; i < FileSize; i += BufferSize){

		ZeroMemory(&Buffer1[0], BufferSize);
		ZeroMemory(&Buffer2[0], BufferSize);

		ReadFile(h1, &Buffer1[0], BufferSize, &NumBytesRead1, NULL);
		ReadFile(h2, &Buffer2[0], BufferSize, &NumBytesRead2, NULL);

		if(NumBytesRead1 != NumBytesRead2 || NumBytesRead1 == 0 || NumBytesRead2 == 0){
			res = false;
			break;
		}

		if(memcmp(&Buffer1[0], &Buffer2[0], NumBytesRead1) != 0){
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

	TableSize = DuplicatesList.GetSize();
	if(TableSize == 0)
		return;

	ColorTable = new TColor[TableSize];
	ZeroMemory(&ColorTable[0], TableSize * sizeof(TColor));

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
	TableSize = 0;
	SAFE_DELETE(ColorTable);
}
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
	TColor col = ColorTable && Index < (int)TableSize ? ColorTable[Index] : clWindow;

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
/*void __fastcall TMainForm::CheckListBoxDuplicatesFilesDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State)
{
	TCheckListBox* CheckListBox = CheckListBoxDuplicatesFiles;
	TCanvas *Canvas = CheckListBox->Canvas;

	// Save pen, brush and fonts settings
	TColor OldPen   = Canvas->Pen->Color;
	TColor OldBrush = Canvas->Brush->Color;
	TColor OldFont  = Canvas->Font->Color;

	// Paint the background
	Canvas->Brush->Color = clWindow;
	Canvas->Font->Color  = clWindowText;
	Canvas->FillRect(Rect);

	// Store text settings...
	int l = Rect.Left;
	int w = Rect.Right - Rect.Left;
	int h = CheckListBox->ItemHeight;
	int n = CheckListBox->Items->Count;

	//int k = 0;
	for(int i = 0; i < n; i++){

		int x = 17;
		int y = h * i;

		TColor col = clWhite;
		if(ColorTable && i < (int)TableSize)
			col = ColorTable[i];

		Canvas->Pen->Color   = col;
		Canvas->Brush->Color = col;

		TRect r(l, y, l+w, y+h);
		Canvas->Rectangle(r);

		String txt = CheckListBox->Items->Strings[i];
		Canvas->TextOutW(x,y, txt);

		if(CheckListBox->State[i] == odFocused)
			Canvas->DrawFocusRect(Rect);
	}

	// Restore pen, brush and fonts settings
	Canvas->Pen->Color   = OldPen;
	Canvas->Brush->Color = OldBrush;
	Canvas->Font->Color  = OldFont;
}*/
//---------------------------------------------------------------------------

