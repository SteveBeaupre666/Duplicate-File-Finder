object MainForm: TMainForm
  Left = 0
  Top = 0
  Caption = 'Duplicate File Finder'
  ClientHeight = 305
  ClientWidth = 962
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  OnCreate = FormCreate
  DesignSize = (
    962
    305)
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 487
    Top = 8
    Width = 74
    Height = 13
    Caption = 'Duplicated Files'
  end
  object GroupBoxSearchPaths: TGroupBox
    Left = 8
    Top = 8
    Width = 481
    Height = 241
    Caption = 'Paths to scan'
    TabOrder = 0
    DesignSize = (
      481
      241)
    object ListBoxPaths: TListBox
      Left = 8
      Top = 16
      Width = 465
      Height = 185
      Anchors = [akLeft, akTop, akRight, akBottom]
      ItemHeight = 13
      TabOrder = 0
    end
    object ButtonAdd: TButton
      Left = 8
      Top = 207
      Width = 105
      Height = 25
      Anchors = [akLeft, akBottom]
      Caption = 'Add...'
      TabOrder = 1
      OnClick = ButtonAddClick
    end
    object ButtonDelete: TButton
      Left = 119
      Top = 207
      Width = 105
      Height = 25
      Anchors = [akLeft, akBottom]
      Caption = 'Delete'
      TabOrder = 2
      OnClick = ButtonDeleteClick
    end
    object ButtonClear: TButton
      Left = 230
      Top = 207
      Width = 105
      Height = 25
      Anchors = [akLeft, akBottom]
      Caption = 'Clear'
      TabOrder = 3
      OnClick = ButtonClearClick
    end
  end
  object ButtonScan: TButton
    Left = 8
    Top = 246
    Width = 473
    Height = 51
    Caption = 'Scan Paths...'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -24
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    OnClick = ButtonScanClick
  end
  object CheckListBoxDuplicatesFiles: TCheckListBox
    Left = 495
    Top = 24
    Width = 458
    Height = 273
    Anchors = [akLeft, akTop, akRight, akBottom]
    ItemHeight = 13
    Style = lbOwnerDrawFixed
    TabOrder = 2
    OnDrawItem = CheckListBoxDuplicatesFilesDrawItem
  end
end
