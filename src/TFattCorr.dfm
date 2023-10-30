object FattCorr: TFattCorr
  Left = 511
  Top = 366
  BorderIcons = []
  BorderStyle = bsSingle
  Caption = 'Fattore di correzione:'
  ClientHeight = 52
  ClientWidth = 224
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
  object NxSpinEditFatt: TNxSpinEdit
    Left = 8
    Top = 16
    Width = 73
    Height = 21
    TabOrder = 0
    Text = '1,000'
    WantTabs = False
    Precision = 3
    Value = 1
    Increment = 0.001
  end
  object ButtonOK: TButton
    Left = 88
    Top = 16
    Width = 57
    Height = 25
    Caption = 'OK'
    TabOrder = 1
    OnClick = ButtonOKClick
  end
  object ButtonCanc: TButton
    Left = 152
    Top = 16
    Width = 57
    Height = 25
    Caption = 'cancel'
    TabOrder = 2
    OnClick = ButtonCancClick
  end
end
