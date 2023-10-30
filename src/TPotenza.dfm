object Potenza: TPotenza
  Left = 374
  Top = 191
  Width = 388
  Height = 241
  BorderIcons = []
  Caption = 'Potenza'
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
    Width = 380
    Height = 183
    DataBindings = <>
    Align = alClient
    TabOrder = 0
    ControlData = {
      0000010046270000EA12000002000000013000000000000000009A9999999999
      F93F9A9999999999C93F333333333333F33F7B14AE47E17AB43F010001000000
      00000600060000000000000000000000000000005940000000000000F03F0000
      0000000000000200FF000000FF000000FFFF000000FF000003252E669A999999
      9999C93FFFFFFF0000000000FFFFFF00000352E30B918FCE119DE300AA004BB8
      5101000000BC02DC7C0100065461686F6D61000352E30B918FCE119DE300AA00
      4BB85101000000BC02CCC60300065461686F6D619A9999999999F13FC3F5285C
      8FC2ED3F0000000000004E400000000000C052400000000000805640000452E3
      0B918FCE119DE300AA004BB8516C74000000000000}
  end
  object StaticText1: TStaticText
    Left = 0
    Top = 183
    Width = 380
    Height = 24
    Align = alBottom
    BevelInner = bvNone
    Caption = '-------'
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
