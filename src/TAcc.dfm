object Acc: TAcc
  Left = 223
  Top = 230
  Width = 555
  Height = 91
  BorderIcons = []
  Caption = 'Apertura acceleratore'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  OnActivate = FormActivate
  OnKeyDown = FormKeyDown
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object CGauge1: TCGauge
    Left = 0
    Top = 0
    Width = 547
    Height = 57
    Align = alClient
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ForeColor = clBlue
    ParentFont = False
    Progress = 12
  end
end
