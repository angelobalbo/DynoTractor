object Step_test: TStep_test
  Left = 88
  Top = 264
  BorderIcons = []
  BorderStyle = bsDialog
  BorderWidth = 1
  Caption = 'Step_test'
  ClientHeight = 215
  ClientWidth = 438
  Color = clBtnFace
  Font.Charset = ANSI_CHARSET
  Font.Color = clWindowText
  Font.Height = -19
  Font.Name = 'Tahoma'
  Font.Style = [fsBold]
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poMainFormCenter
  OnClose = FormClose
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  DesignSize = (
    438
    215)
  PixelsPerInch = 96
  TextHeight = 23
  object Bevel1: TBevel
    Left = 0
    Top = 0
    Width = 438
    Height = 215
    Align = alClient
  end
  object Label2: TLabel
    Left = 16
    Top = 12
    Width = 209
    Height = 41
    AutoSize = False
    Caption = '0000'
    Font.Charset = ANSI_CHARSET
    Font.Color = clGreen
    Font.Height = -29
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label3: TLabel
    Left = 159
    Top = -2
    Width = 88
    Height = 23
    Anchors = [akLeft, akTop, akRight]
    AutoSize = False
    Caption = 'G/min'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -17
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    Visible = False
  end
  object Label4: TLabel
    Left = 216
    Top = 22
    Width = 105
    Height = 23
    Alignment = taRightJustify
    Anchors = [akTop, akRight]
    AutoSize = False
    Caption = 'Tempo [s]'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -17
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object CGauge1: TCGauge
    Left = 325
    Top = 11
    Width = 99
    Height = 42
    ForeColor = clBlue
  end
  object BitBtn1: TBitBtn
    Left = 16
    Top = 64
    Width = 135
    Height = 57
    Caption = '+'
    Default = True
    Font.Charset = ANSI_CHARSET
    Font.Color = clRed
    Font.Height = -37
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 0
    OnClick = BitBtn1Click
  end
  object BitBtn2: TBitBtn
    Left = 171
    Top = 64
    Width = 134
    Height = 57
    Caption = '-'
    Default = True
    Font.Charset = ANSI_CHARSET
    Font.Color = clGreen
    Font.Height = -45
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 1
    OnClick = BitBtn2Click
  end
  object BitBtn3: TBitBtn
    Left = 172
    Top = 135
    Width = 133
    Height = 57
    Caption = 'Annulla'
    TabOrder = 3
    OnClick = BitBtn3Click
    Kind = bkCancel
  end
  object BitBtn4: TBitBtn
    Left = 16
    Top = 135
    Width = 134
    Height = 57
    Caption = 'Avvia'
    Default = True
    TabOrder = 2
    OnClick = BitBtn4Click
    Glyph.Data = {
      DE010000424DDE01000000000000760000002800000024000000120000000100
      0400000000006801000000000000000000001000000000000000000000000000
      80000080000000808000800000008000800080800000C0C0C000808080000000
      FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
      3333333333333333333333330000333333333333333333333333F33333333333
      00003333344333333333333333388F3333333333000033334224333333333333
      338338F3333333330000333422224333333333333833338F3333333300003342
      222224333333333383333338F3333333000034222A22224333333338F338F333
      8F33333300003222A3A2224333333338F3838F338F33333300003A2A333A2224
      33333338F83338F338F33333000033A33333A222433333338333338F338F3333
      0000333333333A222433333333333338F338F33300003333333333A222433333
      333333338F338F33000033333333333A222433333333333338F338F300003333
      33333333A222433333333333338F338F00003333333333333A22433333333333
      3338F38F000033333333333333A223333333333333338F830000333333333333
      333A333333333333333338330000333333333333333333333333333333333333
      0000}
    NumGlyphs = 2
  end
  object PrintStep: TBitBtn
    Left = 328
    Top = 80
    Width = 89
    Height = 40
    BiDiMode = bdLeftToRight
    Caption = 'ACQ     '
    Default = True
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Times New Roman'
    Font.Style = [fsBold]
    ParentBiDiMode = False
    ParentFont = False
    TabOrder = 4
    OnClick = PrintStepClick
    Glyph.Data = {
      FE000000424DFE00000000000000760000002800000010000000110000000100
      04000000000088000000C40E0000C40E00001000000000000000000000000000
      800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
      FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00FFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFF00000000000000000FFFFFFFFFFFFFF00F00F00F00F0
      00F00FFFFFFFFFFFFFF00F00F00F00F000F00FFFFFFFFFFFFFF00F00F00F00F0
      00F00FFFFFFFFFFFFFF00F00F00F00F000F00FFFFFFFFFFFFFF00CCCCCCCCCCC
      CCC0088CCCCCCCCCC8800000000000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFF}
  end
  object ResetTabStep: TBitBtn
    Left = 328
    Top = 134
    Width = 86
    Height = 40
    Caption = 'RESET'
    Default = True
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Times New Roman'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 5
    OnClick = ResetTabStepClick
    Glyph.Data = {
      FE000000424DFE00000000000000760000002800000010000000110000000100
      04000000000088000000C40E0000C40E00001000000000000000000000000000
      800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
      FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00FFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFF00000000000000000FFFFFFFFFFFFFF00F00F00F00F0
      00F00FFFFFFFFFFFFFF00F00F00F00F000F00FFFFFFFFFFFFFF00F00F00F00F0
      00F00FFFFFFFFFFFFFF00F00F00F00F000F00FFFFFFFFFFFFFF00CCCCCCCCCCC
      CCC0088CCCCCCCCCC8800000000000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFF}
  end
  object tmrStep: TTimer
    Enabled = False
    Interval = 500
    OnTimer = tmrStepTimer
    Left = 304
    Top = 192
  end
end
