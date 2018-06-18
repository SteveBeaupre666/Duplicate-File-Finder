object OpenDirectoryForm: TOpenDirectoryForm
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'Select Folder...'
  ClientHeight = 328
  ClientWidth = 263
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object DirectoryListBox: TDirectoryListBox
    Left = 8
    Top = 8
    Width = 249
    Height = 257
    TabOrder = 0
  end
  object DriveComboBox: TDriveComboBox
    Left = 8
    Top = 271
    Width = 249
    Height = 19
    DirList = DirectoryListBox
    TabOrder = 1
  end
  object ButtonCancel: TButton
    Left = 182
    Top = 296
    Width = 75
    Height = 25
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 2
  end
  object ButtonOk: TButton
    Left = 101
    Top = 296
    Width = 75
    Height = 25
    Caption = 'Ok'
    ModalResult = 1
    TabOrder = 3
  end
end
