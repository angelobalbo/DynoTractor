object frmFileList: TfrmFileList
  Left = 604
  Top = 299
  ActiveControl = flb
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Vehicle plate'
  ClientHeight = 228
  ClientWidth = 151
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object flb: TFileListBox
    Left = 0
    Top = 0
    Width = 151
    Height = 192
    Align = alClient
    ExtendedSelect = False
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ItemHeight = 14
    Mask = '*.in'
    ParentFont = False
    TabOrder = 0
  end
  object Panel1: TPanel
    Left = 0
    Top = 192
    Width = 151
    Height = 36
    Align = alBottom
    TabOrder = 1
    object BitBtn1: TBitBtn
      Left = 2
      Top = 7
      Width = 71
      Height = 25
      TabOrder = 0
      Kind = bkCancel
    end
    object BitBtn2: TBitBtn
      Left = 77
      Top = 7
      Width = 71
      Height = 25
      TabOrder = 1
      OnClick = BitBtn2Click
      Kind = bkOK
    end
  end
end
