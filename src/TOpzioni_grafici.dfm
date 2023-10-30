object Opzioni_grafici: TOpzioni_grafici
  Left = 697
  Top = -13
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  BorderWidth = 1
  Caption = 'Opzioni di visualizzazione dei grafici'
  ClientHeight = 338
  ClientWidth = 337
  Color = clBtnFace
  Font.Charset = ANSI_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'Tahoma'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poMainFormCenter
  OnActivate = FormActivate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 16
  object Bevel1: TBevel
    Left = -3
    Top = 112
    Width = 344
    Height = 5
  end
  object BitBtn1: TBitBtn
    Left = 254
    Top = 288
    Width = 75
    Height = 41
    TabOrder = 0
    OnClick = BitBtn1Click
    Kind = bkOK
  end
  object GroupBox1: TGroupBox
    Left = 8
    Top = 8
    Width = 156
    Height = 97
    Caption = ' Potenza '
    Font.Charset = ANSI_CHARSET
    Font.Color = clBlack
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    object CheckBox10: TCheckBox
      Left = 16
      Top = 32
      Width = 129
      Height = 17
      Cursor = crHandPoint
      Caption = 'Ruota'
      Font.Charset = ANSI_CHARSET
      Font.Color = clBlue
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      OnClick = CheckBox10Click
    end
    object cbPZeroScale: TCheckBox
      Left = 16
      Top = 64
      Width = 129
      Height = 17
      Cursor = crHandPoint
      Caption = 'Mod. Y scale'
      Font.Charset = ANSI_CHARSET
      Font.Color = clBlue
      Font.Height = -15
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      OnClick = cbPZeroScaleClick
    end
  end
  object GroupBox2: TGroupBox
    Left = 173
    Top = 9
    Width = 157
    Height = 96
    Caption = ' Coppia '
    TabOrder = 2
    object CheckBox12: TCheckBox
      Left = 10
      Top = 31
      Width = 127
      Height = 17
      Cursor = crHandPoint
      Caption = 'Ruota'
      Font.Charset = ANSI_CHARSET
      Font.Color = clBlue
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      OnClick = CheckBox12Click
    end
    object cbCZeroScale: TCheckBox
      Left = 10
      Top = 63
      Width = 127
      Height = 17
      Cursor = crHandPoint
      Caption = 'Mod. Y scale'
      Font.Charset = ANSI_CHARSET
      Font.Color = clBlue
      Font.Height = -15
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      OnClick = cbCZeroScaleClick
    end
  end
  object GroupBox4: TGroupBox
    Left = 8
    Top = 113
    Width = 321
    Height = 136
    Caption = 'Grafici'
    TabOrder = 3
    object CheckBox4: TCheckBox
      Left = 8
      Top = 24
      Width = 297
      Height = 17
      Cursor = crHandPoint
      BiDiMode = bdLeftToRight
      Caption = 'Pressione aspirazione (manifold)'
      ParentBiDiMode = False
      TabOrder = 0
      OnClick = CheckBox4Click
    end
    object CheckBox5: TCheckBox
      Left = 8
      Top = 52
      Width = 297
      Height = 17
      Cursor = crHandPoint
      Caption = 'Termocoppia (K)  Alta temperatura'
      TabOrder = 1
      OnClick = CheckBox5Click
    end
    object CheckBox8: TCheckBox
      Left = 8
      Top = 81
      Width = 298
      Height = 17
      Cursor = crHandPoint
      Caption = 'Temperatura Olio motore'
      TabOrder = 2
      OnClick = CheckBox8Click
    end
    object CheckBoxConsumi: TCheckBox
      Left = 8
      Top = 108
      Width = 105
      Height = 17
      Cursor = crHandPoint
      Caption = 'Consumi'
      TabOrder = 3
      OnClick = CheckBoxConsumiClick
    end
    object rbGKWh: TRadioButton
      Left = 183
      Top = 112
      Width = 58
      Height = 14
      Caption = 'g/kWh'
      Checked = True
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 4
      TabStop = True
      OnClick = rbGKWhClick
    end
    object rbL: TRadioButton
      Left = 255
      Top = 110
      Width = 58
      Height = 16
      Caption = 'l/h'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 5
      OnClick = rbLClick
    end
  end
end
