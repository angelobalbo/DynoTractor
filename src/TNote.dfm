object Note: TNote
  Left = 379
  Top = 180
  BorderIcons = []
  BorderStyle = bsDialog
  BorderWidth = 1
  Caption = 'Inserisci note del test'
  ClientHeight = 316
  ClientWidth = 499
  Color = clBtnFace
  Font.Charset = ANSI_CHARSET
  Font.Color = clWindowText
  Font.Height = -16
  Font.Name = 'Tahoma'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 120
  TextHeight = 19
  object Memo1: TMemo
    Left = 13
    Top = 18
    Width = 475
    Height = 239
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -17
    Font.Name = 'Tahoma'
    Font.Style = []
    MaxLength = 499
    ParentFont = False
    ScrollBars = ssVertical
    TabOrder = 0
  end
  object BitBtn1: TBitBtn
    Left = 399
    Top = 276
    Width = 89
    Height = 29
    TabOrder = 1
    OnClick = BitBtn1Click
    Kind = bkOK
  end
  object BitBtn2: TBitBtn
    Left = 19
    Top = 276
    Width = 156
    Height = 29
    Caption = 'Cancella tutto'
    TabOrder = 2
    OnClick = BitBtn2Click
    Glyph.Data = {
      76010000424D7601000000000000760000002800000020000000100000000100
      04000000000000010000120B0000120B00001000000000000000000000000000
      800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
      FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00500005000555
      555557777F777555F55500000000555055557777777755F75555005500055055
      555577F5777F57555555005550055555555577FF577F5FF55555500550050055
      5555577FF77577FF555555005050110555555577F757777FF555555505099910
      555555FF75777777FF555005550999910555577F5F77777775F5500505509990
      3055577F75F77777575F55005055090B030555775755777575755555555550B0
      B03055555F555757575755550555550B0B335555755555757555555555555550
      BBB35555F55555575F555550555555550BBB55575555555575F5555555555555
      50BB555555555555575F555555555555550B5555555555555575}
    NumGlyphs = 2
  end
  object BitBtn3: TBitBtn
    Left = 257
    Top = 276
    Width = 89
    Height = 29
    Caption = 'Annulla'
    TabOrder = 3
    Kind = bkCancel
  end
end
