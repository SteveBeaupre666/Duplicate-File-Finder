//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "MainUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMainForm *MainForm;
//---------------------------------------------------------------------------
struct CFileData {
	UINT64 FileSize;
	String FileName;
	bool   Duplicated;
};
//---------------------------------------------------------------------------
LinkedList<CFileData> FilesList;
//---------------------------------------------------------------------------
LPCSTR FilesListName = "C:\\New Programming Folder\\Programs\\Duplicate File Finder\\FilesList.txt";
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner):TForm(Owner){}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormCreate(TObject *Sender)
{
	ListBoxPaths->Items->Add("C:\\New Programming Folder\\Programs\\Duplicate File Finder\\Test Folder");
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	FilesList.Clear();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButtonScanClick(TObject *Sender)
{
	FilesList.Clear();
	ScanFolders();
	//SaveFilesList((char*)FilesListName);

	FindDuplicates();
	//UINT64 NumDups = FindDuplicates();x
	//ShowMessage("Dupliates Found: " + IntToStr((__int64)NumDups) + ".");
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButtonAddClick(TObject *Sender)
{
	TOpenDirectoryForm *pOpenDirDlg = new TOpenDirectoryForm(this);

	if(pOpenDirDlg){
		if(pOpenDirDlg->ShowModal() == mrOk){
			String dir = pOpenDirDlg->DirectoryListBox->Directory;
			ListBoxPaths->Items->Add(dir);
		}

		delete pOpenDirDlg;
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButtonDeleteClick(TObject *Sender)
{
	int indx = ListBoxPaths->ItemIndex;
	if(indx >= 0)
		ListBoxPaths->Items->Delete(indx);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButtonClearClick(TObject *Sender)
{
	ListBoxPaths->Items->Clear();
}
//---------------------------------------------------------------------------
// TODO: rewrite this with the new unicode TxtFile class
//---------------------------------------------------------------------------
/*void __fastcall TMainForm::SaveFilesList(char *fname)
{
	CTxtFile f;
	if(f.Create(fname)){

		LinkedListNode<CFileData> *node = FilesList.GetFirstNode();

		while(node){

			AnsiString s = AnsiString(node->item.FileName);

			f.WriteArgs("%s\n", s.c_str());

			node = node->next;
		}

		f.Close();
	}
}*/
//---------------------------------------------------------------------------
void __fastcall TMainForm::ScanFolders()
{
	int NumPaths = ListBoxPaths->Items->Count;

	for(int i = 0; i < NumPaths; i++){
		String path = ListBoxPaths->Items->Strings[i];
		Scan(path);
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::Scan(String path)
{
	WIN32_FIND_DATAW fd;
	ZeroMemory(&fd, sizeof(fd));

	String PathToScan = path + "\\*";

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
			CFileData fi;
			ZeroMemory(&fi, sizeof(fi));

			String FileName = path + "\\" + fname;
			UINT64 FileSize = ((UINT64)fd.nFileSizeHigh << 32) | ((UINT64)fd.nFileSizeLow);

			fi.FileName = FileName;
			fi.FileSize = FileSize;
			fi.Duplicated = false;

			FilesList.Push(&fi);
		} else {
			String NewPath = path + "\\" + fname;
			Scan(NewPath);
		}


		if(!FindNextFile(h, &fd))
			break;
	}

	FindClose(h);
}
//---------------------------------------------------------------------------
UI64 __fastcall TMainForm::FindDuplicates()
{
	static const wchar_t newline = '\n';
	static const String fname = "C:\\New Programming Folder\\Programs\\Duplicate File Finder\\Duplicates.txt";

	CheckListBoxDuplicatesFiles->Items->Clear();

	CTxtFile f;
	if(!f.Create(fname.c_str()))
		return 0;

	UINT64 NumDuplicatesFound = 0;

	LinkedListNode<CFileData> *n2 = NULL;
	LinkedListNode<CFileData> *n1 = FilesList.GetFirstNode();

	while(n1){

		bool duplicated = false;

		if(n1->item.Duplicated)
			goto Next1;

		n2 = n1->next;

		while(n2){

			CFileData *i1 = &n1->item;
			CFileData *i2 = &n2->item;

			if(i2->Duplicated)
				goto Next2;

			if(i1->FileSize == i2->FileSize){

				if(CompareFiles(i1->FileName, i2->FileName, i1->FileSize)){

					wchar_t *fname1 = i1->FileName.c_str();
					wchar_t *fname2 = i2->FileName.c_str();

					if(i1->Duplicated == false){
						f.WriteLine(fname1);
						CheckListBoxDuplicatesFiles->Items->Add(fname1);
						NumDuplicatesFound++;
					}

					f.WriteLine(fname2);
					CheckListBoxDuplicatesFiles->Items->Add(fname2);
					NumDuplicatesFound++;

					i1->Duplicated = true;
					i2->Duplicated = true;

					duplicated = true;
				}
			}

			Next2:
			n2 = n2->next;
		}

		// This should be at the start of the first loop somehow...
		if(duplicated)
			f.WriteChar(newline);

		Next1:
		n1 = n1->next;
	}

	f.Close();

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

