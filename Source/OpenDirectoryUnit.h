//---------------------------------------------------------------------------
#ifndef OpenDirectoryUnitH
#define OpenDirectoryUnitH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.FileCtrl.hpp>
//---------------------------------------------------------------------------
class TOpenDirectoryForm : public TForm
{
__published:	// IDE-managed Components
	TDirectoryListBox *DirectoryListBox;
	TDriveComboBox *DriveComboBox;
	TButton *ButtonCancel;
	TButton *ButtonOk;
private:	// User declarations
public:		// User declarations
	__fastcall TOpenDirectoryForm(TComponent* Owner);
	String __fastcall GetPath();
};
//---------------------------------------------------------------------------
extern PACKAGE TOpenDirectoryForm *OpenDirectoryForm;
//---------------------------------------------------------------------------
#endif
