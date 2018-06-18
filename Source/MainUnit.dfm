object MainForm: TMainForm
  Left = 0
  Top = 0
  BorderStyle = bsSingle
  Caption = 'Duplicate File Finder'
  ClientHeight = 369
  ClientWidth = 955
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
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBoxSearchPaths: TGroupBox
    Left = 8
    Top = 8
    Width = 481
    Height = 241
    Caption = 'Search Paths'
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
  object GroupBoxScanPaths: TGroupBox
    Left = 8
    Top = 255
    Width = 481
    Height = 106
    Caption = 'Scan Paths'
    TabOrder = 1
    DesignSize = (
      481
      106)
    object ScanPathsGauge: TGauge
      Left = 8
      Top = 79
      Width = 465
      Height = 18
      Anchors = [akLeft, akRight, akBottom]
      Progress = 0
      ExplicitWidth = 657
    end
    object ButtonScan: TButton
      Left = 8
      Top = 22
      Width = 465
      Height = 51
      Anchors = [akLeft, akTop, akRight, akBottom]
      Caption = 'Scan'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -24
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      OnClick = ButtonScanClick
    end
  end
  object GroupBoxDuplicates: TGroupBox
    Left = 495
    Top = 8
    Width = 452
    Height = 353
    Caption = 'Duplicates Files'
    TabOrder = 2
    DesignSize = (
      452
      353)
    object CheckListBoxDuplicatesFiles: TCheckListBox
      Left = 8
      Top = 16
      Width = 436
      Height = 328
      Anchors = [akLeft, akTop, akRight, akBottom]
      ItemHeight = 13
      TabOrder = 0
    end
  end
end
