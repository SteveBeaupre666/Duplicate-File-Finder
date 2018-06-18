//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "OpenDirectoryUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TOpenDirectoryForm *OpenDirectoryForm;
//---------------------------------------------------------------------------
__fastcall TOpenDirectoryForm::TOpenDirectoryForm(TComponent* Owner):TForm(Owner){}
//---------------------------------------------------------------------------
String __fastcall TOpenDirectoryForm::GetPath()
{
	return DirectoryListBox->Directory;
}
//---------------------------------------------------------------------------

