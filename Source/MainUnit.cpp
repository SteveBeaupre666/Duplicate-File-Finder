//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "MainUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
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
	ClearColorTable();
	FilesList.Clear();
	DuplicatesList.Clear();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButtonAddClick(TObject *Sender)
{
	TOpenDirectoryForm *pOpenDirDlg = new TOpenDirectoryForm(this);

	try {
		if(pOpenDirDlg && pOpenDirDlg->ShowModal() == mrOk){
			String dir = pOpenDirDlg->DirectoryListBox->Directory;
			ListBoxPaths->Items->Add(dir);
		}
	} __finally {
		SAFE_DELETE(pOpenDirDlg);
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
TColor __fastcall TMainForm::GetColor(BYTE r, BYTE g, BYTE b, BYTE a)
{
	DWORD col = ((DWORD)r) | ((DWORD)g << 8) | ((DWORD)g << 16) | ((DWORD)a << 24);
	return TColor(col);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TMainForm::CheckListBoxDuplicatesFilesDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State)
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
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TMainForm::ScanPaths()
{
	FilesList.Clear();
	int n = ListBoxPaths->Items->Count;

	for(int i = 0; i < n; i++){
		String path = ListBoxPaths->Items->Strings[i];
		ScanDir(path);
	}

	HWND h = this->Handle;
	if(MessageBoxA(h, "Warning!", "Save the list of scanned files?", MB_YESNO) == IDOK)
		SaveFilesList(ScannedFiles);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
bool __fastcall TMainForm::SaveFilesList(const String &fname)
{
	CTxtFile f;
	if(f.Create(fname.c_str())){

		CFilesListNode *node = FilesList.GetFirstNode();

		while(node){

			int res = f.WriteLine(fname.c_str());

			if(!res){
				f.Close();
				return false;
			}

			node = node->GetNext();
		}

		f.Close();
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
			FindNextFile(h, &fd);
			continue;
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
UI64 __fastcall TMainForm::FindDuplicates()
{
	//static const wchar_t newline = '\n';

	DuplicatesList.Clear();
	CheckListBoxDuplicatesFiles->Items->Clear();

	UINT64 id = 0;
	UINT64 NumDuplicatesFound = 0;

	CFilesListNode *n2 = NULL;
	CFilesListNode *n1 = FilesList.GetFirstNode();

	while(n1){

		bool duplicated = false;

		if(n1->IsDuplicated())
			goto Next1;

		n2 = n1->GetNext();

		while(n2){

			CFileInfo *i1 = n1->GetFileInfo();
			CFileInfo *i2 = n2->GetFileInfo();

			if(i2->Duplicated)
				goto Next2;

			if(i1->FileSize == i2->FileSize){

				if(CompareFiles(i1->FileName, i2->FileName, i1->FileSize)){

					duplicated = true;

					String FileName1 = i1->FileName;
					String FileName2 = i2->FileName;

					wchar_t *fname1 = FileName1.c_str();
					wchar_t *fname2 = FileName2.c_str();

					if(i1->Duplicated == FALSE){

						NumDuplicatesFound++;
						i1->Duplicated = TRUE;

						DuplicatesList.Add(i1, id);
						CheckListBoxDuplicatesFiles->Items->Add(fname1);
					}

					NumDuplicatesFound++;
					i2->Duplicated = TRUE;

					DuplicatesList.Add(i2, id);
					CheckListBoxDuplicatesFiles->Items->Add(fname2);
				}
			}

			Next2:
			n2 = n2->GetNext();
		}

		if(duplicated)
			id++;

		Next1:
		n1 = n1->GetNext();
	}

	BuildColorTable();

	return NumDuplicatesFound;
}
//---------------------------------------------------------------------------
bool __fastcall TMainForm::CompareFiles(String fname1, String fname2, UINT64 fsize)
{
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

