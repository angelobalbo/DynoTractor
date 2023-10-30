object Giri_Mot: TGiri_Mot
  Left = 541
  Top = 127
  Width = 432
  Height = 398
  BorderIcons = []
  Caption = 'Giri Motore'
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
  object AnalogMeterHQ1: TAnalogMeterHQ
    Left = 0
    Top = 0
    Width = 424
    Height = 340
    DataBindings = <>
    Align = alClient
    TabOrder = 0
    ControlData = {
      00000100D22B00002423000002000000013000000000000000009A9999999999
      F93F333333333333C33F333333333333F33F7B14AE47E17AB43F010001000000
      000005000A000000000000000000000000000088B340000000000000F03F0000
      0000000000000200FF000000FF000000FFFF000000FF000003252E669A999999
      9999C93FFFFFFF0000000000FFFFFF00000352E30B918FCE119DE300AA004BB8
      5101000000BC02A42C0200065461686F6D61000352E30B918FCE119DE300AA00
      4BB85101000000BC02CCC60300065461686F6D619A9999999999F13FC3F5285C
      8FC2ED3F0000000000004E400000000000C052400000000000804640000452E3
      0B918FCE119DE300AA004BB8516C74000000000000}
  end
  object StaticText1: TStaticText
    Left = 0
    Top = 340
    Width = 424
    Height = 24
    Align = alBottom
    BevelInner = bvNone
    Caption = 'G/m'
    Color = clWhite
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentColor = False
    ParentFont = False
    TabOrder = 1
  end
end
