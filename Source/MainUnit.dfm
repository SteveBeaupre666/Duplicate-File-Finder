object MainForm: TMainForm
  Left = 0
  Top = 0
  Caption = 'Duplicate File Finder'
  ClientHeight = 350
  ClientWidth = 990
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = MainMenu
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object ScanGauge: TGauge
    Left = 8
    Top = 312
    Width = 968
    Height = 26
    Progress = 0
  end
  object Label1: TLabel
    Left = 511
    Top = 8
    Width = 74
    Height = 13
    Caption = 'Duplicated Files'
  end
  object Label2: TLabel
    Left = 8
    Top = 8
    Width = 69
    Height = 13
    Caption = 'Paths to scan:'
  end
  object ButtonScan: TButton
    Left = 8
    Top = 249
    Width = 465
    Height = 51
    Caption = 'Scan Paths...'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -21
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    OnClick = ButtonScanClick
  end
  object CheckListBoxDuplicatesFiles: TCheckListBox
    Left = 511
    Top = 27
    Width = 465
    Height = 216
    ItemHeight = 13
    PopupMenu = PopupMenu
    Style = lbOwnerDrawFixed
    TabOrder = 1
    OnDrawItem = CheckListBoxDuplicatesFilesDrawItem
  end
  object ButtonDeleteFiles: TButton
    Left = 511
    Top = 249
    Width = 465
    Height = 51
    Caption = 'Delete Files...'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -21
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    OnClick = ButtonScanClick
  end
  object ButtonAdd: TButton
    Left = 8
    Top = 218
    Width = 150
    Height = 25
    Caption = 'Add...'
    TabOrder = 3
    OnClick = ButtonAddClick
  end
  object ButtonClear: TButton
    Left = 323
    Top = 218
    Width = 150
    Height = 25
    Caption = 'Clear'
    TabOrder = 4
    OnClick = ButtonClearClick
  end
  object ButtonDelete: TButton
    Left = 165
    Top = 218
    Width = 150
    Height = 25
    Caption = 'Delete'
    TabOrder = 5
    OnClick = ButtonDeleteClick
  end
  object ListBoxPaths: TListBox
    Left = 8
    Top = 27
    Width = 465
    Height = 185
    ItemHeight = 13
    TabOrder = 6
  end
  object MainMenu: TMainMenu
    Left = 32
    Top = 40
    object File1: TMenuItem
      Caption = '&File'
      object N1: TMenuItem
        Caption = '-'
      end
      object Exit1: TMenuItem
        Caption = '&Exit'
      end
    end
  end
  object PopupMenu: TPopupMenu
    Left = 920
    Top = 40
  end
end
