object Start: TStart
  Left = 542
  Top = 299
  Anchors = []
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Start'
  ClientHeight = 211
  ClientWidth = 340
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
    340
    211)
  PixelsPerInch = 96
  TextHeight = 23
  object Label1: TLabel
    Left = 20
    Top = 19
    Width = 111
    Height = 23
    Alignment = taRightJustify
    Anchors = [akTop, akRight]
    Caption = 'Inizio test :'
  end
  object Label2: TLabel
    Left = 134
    Top = 10
    Width = 105
    Height = 40
    Alignment = taRightJustify
    Anchors = [akTop, akRight]
    Caption = '00000'
    Font.Charset = ANSI_CHARSET
    Font.Color = clGreen
    Font.Height = -33
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label3: TLabel
    Left = 243
    Top = 20
    Width = 59
    Height = 21
    Caption = 'Giri/min'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -17
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label4: TLabel
    Left = 16
    Top = 117
    Width = 36
    Height = 25
    Caption = '----'
    Color = clBlue
    Font.Charset = ANSI_CHARSET
    Font.Color = clBlue
    Font.Height = -21
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentColor = False
    ParentFont = False
    Transparent = True
  end
  object Label5: TLabel
    Left = 16
    Top = 72
    Width = 221
    Height = 25
    Caption = 'Accelerare a fondo !!!'
    Font.Charset = ANSI_CHARSET
    Font.Color = clRed
    Font.Height = -21
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object BitBtn1: TBitBtn
    Left = 9
    Top = 155
    Width = 158
    Height = 50
    Caption = 'OK'
    Default = True
    TabOrder = 0
    OnClick = BitBtn1Click
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
  object BitBtn2: TBitBtn
    Left = 175
    Top = 155
    Width = 158
    Height = 50
    Caption = 'Cancella'
    TabOrder = 1
    OnClick = BitBtn2Click
    Kind = bkCancel
  end
  object Timer1: TTimer
    Enabled = False
    Interval = 500
    OnTimer = Timer1Timer
    Left = 280
    Top = 64
  end
  object Timer2: TTimer
    Enabled = False
    Interval = 250
    OnTimer = Timer2Timer
    Left = 280
    Top = 104
  end
end
