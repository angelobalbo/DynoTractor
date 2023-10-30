object Map: TMap
  Left = 1086
  Top = 344
  Width = 266
  Height = 216
  BorderIcons = []
  Caption = 'Map'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  OnKeyDown = FormKeyDown
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object AnalogMeterHQ1: TAnalogMeterHQ
    Left = 0
    Top = 0
    Width = 258
    Height = 158
    DataBindings = <>
    Align = alClient
    ParentShowHint = False
    TabOrder = 0
    ControlData = {
      00000100AA1A00005410000002000000013000000000000000009A9999999999
      F93F9A9999999999C93F333333333333F33F7B14AE47E17AB43F010001000000
      000006000A000000000000408FC00000000000005940000000000000F03F0000
      000000408FC00200FF000000FF000000FFFF000000FF000003252E669A999999
      9999C93FFFFFFF0000000000FFFFFF00000352E30B918FCE119DE300AA004BB8
      5101000000BC02DC7C0100065461686F6D61000352E30B918FCE119DE300AA00
      4BB85101000000BC02CCC60300065461686F6D619A9999999999F13FC3F5285C
      8FC2ED3F000000000000594000000000000059400000000000805640000452E3
      0B918FCE119DE300AA004BB8516C74000000000000}
  end
  object StaticText1: TStaticText
    Left = 0
    Top = 158
    Width = 258
    Height = 24
    Align = alBottom
    BevelInner = bvNone
    Caption = '--------'
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
