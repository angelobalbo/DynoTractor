object Impostazioni: TImpostazioni
  Left = 269
  Top = 63
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  BorderWidth = 2
  Caption = 'Test Setup'
  ClientHeight = 600
  ClientWidth = 757
  Color = clBtnFace
  Font.Charset = ANSI_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Tahoma'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poMainFormCenter
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 14
  object GroupBox2: TGroupBox
    Left = 8
    Top = 70
    Width = 345
    Height = 224
    Caption = ' Parametri: '
    TabOrder = 0
    object StaticText1: TStaticText
      Left = 24
      Top = 48
      Width = 4
      Height = 4
      TabOrder = 0
    end
    object Test: TPageControl
      Left = 2
      Top = 26
      Width = 341
      Height = 196
      ActivePage = TabSheet3
      Align = alBottom
      Anchors = [akRight, akBottom]
      MultiLine = True
      OwnerDraw = True
      TabOrder = 1
      object TabSheet1: TTabSheet
        Caption = 'TabSheet1'
        TabVisible = False
        DesignSize = (
          333
          186)
        object Label1: TLabel
          Left = 266
          Top = 33
          Width = 40
          Height = 14
          Caption = 'Giri/min'
        end
        object Label2: TLabel
          Left = 265
          Top = 64
          Width = 40
          Height = 14
          Caption = 'Giri/min'
        end
        object Label3: TLabel
          Left = 232
          Top = 95
          Width = 43
          Height = 14
          Caption = 'Secondi'
        end
        object Edit1: TEdit
          Left = 186
          Top = 24
          Width = 73
          Height = 29
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -17
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          OnChange = Edit1Change
        end
        object StaticText2: TStaticText
          Left = 5
          Top = 29
          Width = 157
          Height = 18
          Alignment = taRightJustify
          Anchors = [akTop, akRight]
          Caption = 'Regime di inizio acquisizione:'
          TabOrder = 1
        end
        object Edit2: TEdit
          Left = 186
          Top = 55
          Width = 73
          Height = 29
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -17
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
          OnChange = Edit2Change
        end
        object StaticText3: TStaticText
          Left = 5
          Top = 61
          Width = 152
          Height = 18
          Alignment = taRightJustify
          Anchors = [akTop, akRight]
          Caption = 'Regime di fine acquisizione:'
          TabOrder = 3
        end
        object Edit3: TEdit
          Left = 186
          Top = 87
          Width = 41
          Height = 29
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -17
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 4
          OnChange = Edit3Change
        end
        object StaticText4: TStaticText
          Left = 5
          Top = 94
          Width = 120
          Height = 18
          Alignment = taRightJustify
          Anchors = [akTop, akRight]
          Caption = 'Durata accelerazione:'
          TabOrder = 5
        end
      end
      object TabSheet3: TTabSheet
        Caption = 'TabSheet3'
        ImageIndex = 2
        TabVisible = False
        DesignSize = (
          333
          186)
        object Label9: TLabel
          Left = 252
          Top = 30
          Width = 40
          Height = 14
          Caption = 'Giri/min'
        end
        object Label10: TLabel
          Left = 251
          Top = 64
          Width = 40
          Height = 14
          Caption = 'Giri/min'
        end
        object Label11: TLabel
          Left = 252
          Top = 95
          Width = 40
          Height = 14
          Caption = 'Giri/min'
        end
        object Label12: TLabel
          Left = 220
          Top = 129
          Width = 43
          Height = 14
          Caption = 'Secondi'
        end
        object StaticText10: TStaticText
          Left = 8
          Top = 29
          Width = 90
          Height = 18
          Alignment = taRightJustify
          Anchors = [akTop, akRight]
          Caption = 'Regime minimo:'
          TabOrder = 0
        end
        object StaticText11: TStaticText
          Left = 8
          Top = 61
          Width = 93
          Height = 18
          Alignment = taRightJustify
          Anchors = [akTop, akRight]
          Caption = 'Regime massimo'
          TabOrder = 1
        end
        object StaticText12: TStaticText
          Left = 8
          Top = 92
          Width = 121
          Height = 18
          Alignment = taRightJustify
          Anchors = [akTop, akRight]
          Caption = 'Valore di incremento:'
          TabOrder = 2
        end
        object StaticText13: TStaticText
          Left = 8
          Top = 126
          Width = 143
          Height = 18
          Alignment = taRightJustify
          Anchors = [akTop, akRight]
          Caption = 'Tempo di stazionamento:'
          TabOrder = 3
        end
        object NxNumberEdit3: TNxNumberEdit
          Left = 187
          Top = 22
          Width = 62
          Height = 29
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -17
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 4
          Text = '200'
          WantTabs = False
          OnChange = NxNumberEdit3Change
          Max = 20000
          Min = 200
          Precision = 0
          Value = 200
        end
        object NxNumberEdit4: TNxNumberEdit
          Left = 187
          Top = 55
          Width = 62
          Height = 29
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -17
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 5
          Text = '200'
          WantTabs = False
          OnChange = NxNumberEdit4Change
          Max = 20000
          Min = 200
          Precision = 0
          Value = 200
        end
        object NxNumberEdit5: TNxNumberEdit
          Left = 187
          Top = 88
          Width = 62
          Height = 29
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -17
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 6
          Text = '25'
          WantTabs = False
          OnChange = NxNumberEdit5Change
          Max = 400
          Min = 1
          Precision = 0
          Value = 25
        end
        object NxNumberEdit6: TNxNumberEdit
          Left = 187
          Top = 121
          Width = 30
          Height = 29
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -17
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 7
          Text = '2'
          WantTabs = False
          OnChange = NxNumberEdit6Change
          Max = 60
          Min = 1
          Precision = 0
          Value = 2
        end
      end
      object TabSheet4: TTabSheet
        Caption = 'TabSheet4'
        ImageIndex = 3
        TabVisible = False
        DesignSize = (
          333
          186)
        object Label13: TLabel
          Left = 252
          Top = 32
          Width = 40
          Height = 14
          Caption = 'Giri/min'
        end
        object Label14: TLabel
          Left = 251
          Top = 63
          Width = 40
          Height = 14
          Caption = 'Giri/min'
        end
        object Label15: TLabel
          Left = 235
          Top = 95
          Width = 40
          Height = 14
          Caption = 'Giri/min'
        end
        object StaticText14: TStaticText
          Left = 8
          Top = 29
          Width = 90
          Height = 18
          Alignment = taRightJustify
          Anchors = [akTop, akRight]
          Caption = 'Regime minimo:'
          TabOrder = 0
        end
        object StaticText15: TStaticText
          Left = 8
          Top = 61
          Width = 93
          Height = 18
          Alignment = taRightJustify
          Anchors = [akTop, akRight]
          Caption = 'Regime massimo'
          TabOrder = 1
        end
        object StaticText16: TStaticText
          Left = 8
          Top = 92
          Width = 121
          Height = 18
          Alignment = taRightJustify
          Anchors = [akTop, akRight]
          Caption = 'Valore di incremento:'
          TabOrder = 2
        end
        object NxNumberEdit10: TNxNumberEdit
          Left = 187
          Top = 22
          Width = 62
          Height = 29
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -17
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
          Text = '200'
          WantTabs = False
          OnChange = NxNumberEdit10Change
          Min = 200
          Precision = 0
          Value = 200
        end
        object NxNumberEdit11: TNxNumberEdit
          Left = 187
          Top = 55
          Width = 62
          Height = 29
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -17
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 4
          Text = '200'
          WantTabs = False
          OnChange = NxNumberEdit11Change
          Max = 3000
          Min = 200
          Precision = 0
          Value = 200
        end
        object NxNumberEdit12: TNxNumberEdit
          Left = 187
          Top = 88
          Width = 46
          Height = 29
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -17
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 5
          Text = '25'
          WantTabs = False
          OnChange = NxNumberEdit12Change
          Max = 400
          Min = 25
          Precision = 0
          Value = 25
        end
      end
      object TabSheet8: TTabSheet
        Caption = 'TabSheet8'
        ImageIndex = 4
        TabVisible = False
        DesignSize = (
          333
          186)
        object Label4: TLabel
          Left = 222
          Top = 95
          Width = 43
          Height = 14
          Caption = 'Secondi'
        end
        object Label8: TLabel
          Left = 243
          Top = 63
          Width = 40
          Height = 14
          Caption = 'Giri/min'
        end
        object Label18: TLabel
          Left = 244
          Top = 32
          Width = 40
          Height = 14
          Caption = 'Giri/min'
        end
        object StaticText5: TStaticText
          Left = 4
          Top = 29
          Width = 157
          Height = 18
          Alignment = taRightJustify
          Anchors = [akTop, akRight]
          Caption = 'Regime di inizio acquisizione:'
          TabOrder = 0
        end
        object StaticText9: TStaticText
          Left = 4
          Top = 61
          Width = 152
          Height = 18
          Alignment = taRightJustify
          Anchors = [akTop, akRight]
          Caption = 'Regime di fine acquisizione:'
          TabOrder = 1
        end
        object StaticText17: TStaticText
          Left = 4
          Top = 94
          Width = 122
          Height = 18
          Alignment = taRightJustify
          Anchors = [akTop, akRight]
          Caption = 'Durata decelerazione:'
          TabOrder = 2
        end
        object NxNumberEdit15: TNxNumberEdit
          Left = 179
          Top = 22
          Width = 62
          Height = 29
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -17
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
          Text = '200'
          WantTabs = False
          OnChange = NxNumberEdit15Change
          Max = 12000
          Precision = 0
          Value = 200
        end
        object NxNumberEdit16: TNxNumberEdit
          Left = 179
          Top = 55
          Width = 62
          Height = 29
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -17
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 4
          Text = '200'
          WantTabs = False
          OnChange = NxNumberEdit16Change
          Max = 12000
          Precision = 0
          Value = 200
        end
        object NxNumberEdit17: TNxNumberEdit
          Left = 179
          Top = 88
          Width = 41
          Height = 29
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -17
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 5
          Text = '10'
          WantTabs = False
          OnChange = NxNumberEdit17Change
          Max = 30
          Min = 10
          Precision = 0
          Value = 10
        end
      end
      object TabSheet11: TTabSheet
        Caption = 'TabSheet11'
        ImageIndex = 7
        TabVisible = False
        DesignSize = (
          333
          186)
        object Label19: TLabel
          Left = 111
          Top = 56
          Width = 82
          Height = 14
          Alignment = taRightJustify
          Anchors = [akTop, akRight]
          Caption = 'Step di coppia:'
        end
        object Label29: TLabel
          Left = 252
          Top = 56
          Width = 8
          Height = 14
          Caption = 'N'
        end
        object Label39: TLabel
          Left = 107
          Top = 97
          Width = 86
          Height = 14
          Alignment = taRightJustify
          Anchors = [akTop, akRight]
          Caption = 'Regime minimo:'
        end
        object Label46: TLabel
          Left = 252
          Top = 100
          Width = 40
          Height = 14
          Caption = 'Giri/min'
        end
        object NxNumberEdit13: TNxNumberEdit
          Left = 195
          Top = 48
          Width = 54
          Height = 29
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -17
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          Text = '5,0'
          WantTabs = False
          OnChange = NxNumberEdit13Change
          Max = 1000
          Min = 1
          Precision = 1
          Value = 5
        end
        object NxNumberEdit14: TNxNumberEdit
          Left = 195
          Top = 93
          Width = 54
          Height = 29
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -17
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          Text = '200'
          WantTabs = False
          OnChange = NxNumberEdit14Change
          Max = 3000
          Min = 200
          Precision = 0
          Value = 200
        end
      end
    end
  end
  object GroupBox4: TGroupBox
    Left = 8
    Top = 3
    Width = 345
    Height = 62
    Caption = ' Tipo di test da eseguire: '
    TabOrder = 1
    object ComboBox1: TComboBox
      Left = 8
      Top = 24
      Width = 323
      Height = 24
      Style = csDropDownList
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ItemHeight = 16
      MaxLength = 1
      ParentFont = False
      TabOrder = 0
      OnChange = ComboBox1Change
    end
  end
  object GroupBox1: TGroupBox
    Left = 8
    Top = 302
    Width = 345
    Height = 290
    Caption = ' Calcolo del rapporto '
    TabOrder = 2
    object Rap: TPageControl
      Left = 2
      Top = 66
      Width = 341
      Height = 222
      ActivePage = TabSheet5
      Align = alBottom
      DockSite = True
      MultiLine = True
      TabOrder = 0
      TabStop = False
      object TabSheet5: TTabSheet
        Caption = 'TabSheet1'
        TabVisible = False
        object Label16: TLabel
          Left = 271
          Top = 46
          Width = 21
          Height = 14
          Caption = 'rpm'
        end
        object StaticText21: TStaticText
          Left = 34
          Top = 35
          Width = 164
          Height = 18
          Caption = 'Regime motore di riferimento'
          TabOrder = 0
        end
        object StaticText22: TStaticText
          Left = 34
          Top = 51
          Width = 145
          Height = 18
          Caption = 'per il calcolo del rapporto:'
          TabOrder = 1
        end
        object NxNumberEdit7: TNxNumberEdit
          Left = 203
          Top = 40
          Width = 60
          Height = 29
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -17
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
          Text = '3000'
          WantTabs = False
          OnChange = NxNumberEdit7Change
          Max = 3000
          Min = 1000
          Precision = 0
          Value = 3000
        end
      end
      object TabSheet7: TTabSheet
        Caption = 'TabSheet3'
        ImageIndex = 2
        TabVisible = False
        object NxHeaderPanel1: TNxHeaderPanel
          Left = 0
          Top = 0
          Width = 333
          Height = 212
          AdaptiveColors = True
          Align = alClient
          BorderColor = clBlack
          Caption = 'Calcolo rapporto di trasm. Motore/PTO'
          Color = clMenuBar
          HeaderColor = clHighlight
          HeaderFont.Charset = ANSI_CHARSET
          HeaderFont.Color = clWindowText
          HeaderFont.Height = -12
          HeaderFont.Name = 'Tahoma'
          HeaderFont.Style = []
          InnerMargins.Bottom = 4
          InnerMargins.Left = 9
          InnerMargins.Right = 8
          InnerMargins.Top = 5
          Options = []
          ParentHeaderFont = False
          TabOrder = 0
          object NxLabel1: TNxLabel
            Left = 190
            Top = 61
            Width = 68
            Height = 14
            Caption = 'Giri/min PTO'
            HorizontalPosition = hpLeft
            InnerHorizontal = True
            InnerVertical = False
            InnerMargins.Horizontal = 0
            InnerMargins.Vertical = 2
            VerticalPosition = vpTop
          end
          object NxLabel2: TNxLabel
            Left = 59
            Top = 61
            Width = 83
            Height = 14
            Caption = 'Giri/min Motore'
            HorizontalPosition = hpLeft
            InnerHorizontal = True
            InnerVertical = False
            InnerMargins.Horizontal = 0
            InnerMargins.Vertical = 2
            VerticalPosition = vpTop
          end
          object NxLabel3: TNxLabel
            Left = 152
            Top = 88
            Width = 21
            Height = 14
            Caption = '--->'
            HorizontalPosition = hpLeft
            InnerHorizontal = True
            InnerVertical = False
            InnerMargins.Horizontal = 0
            InnerMargins.Vertical = 2
            VerticalPosition = vpTop
          end
          object NxNumberEdit18: TNxNumberEdit
            Left = 77
            Top = 82
            Width = 49
            Height = 29
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -17
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            Text = '1'
            WantTabs = False
            OnChange = NxNumberEdit18Change
            Max = 4000
            Min = 1
            Precision = 0
            Value = 1
          end
          object NxNumberEdit19: TNxNumberEdit
            Left = 200
            Top = 82
            Width = 49
            Height = 29
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -17
            Font.Name = 'Tahoma'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            Text = '1'
            WantTabs = False
            OnChange = NxNumberEdit19Change
            Max = 2000
            Min = 1
            Precision = 0
            Value = 1
          end
          object StaticText28: TStaticText
            Left = 25
            Top = 149
            Width = 176
            Height = 18
            Caption = 'Rapporto di trasmissione totale:'
            TabOrder = 2
          end
          object NxNumberEdit1: TNxNumberEdit
            Left = 214
            Top = 147
            Width = 49
            Height = 24
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Tahoma'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 3
            Text = '0,00'
            WantTabs = False
            OnChange = NxNumberEdit1Change
          end
        end
      end
    end
    object ComboBox6: TComboBox
      Left = 16
      Top = 26
      Width = 313
      Height = 24
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ItemHeight = 16
      MaxLength = 1
      ParentFont = False
      TabOrder = 1
      OnChange = ComboBox6Change
    end
  end
  object GroupBox8: TGroupBox
    Left = 360
    Top = 192
    Width = 391
    Height = 334
    Caption = ' Dati cliente: '
    TabOrder = 3
    TabStop = True
    object StaticText37: TStaticText
      Left = 24
      Top = 38
      Width = 94
      Height = 25
      Caption = 'Costruttore:'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -17
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
    end
    object StaticText38: TStaticText
      Left = 24
      Top = 99
      Width = 67
      Height = 25
      Caption = 'Modello:'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -17
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
    end
    object StaticText39: TStaticText
      Left = 24
      Top = 161
      Width = 103
      Height = 25
      Caption = 'Allestimento:'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -17
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
    end
    object StaticText42: TStaticText
      Left = 23
      Top = 223
      Width = 53
      Height = 25
      Caption = 'Targa:'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -17
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 3
    end
    object Edit28: TEdit
      Left = 173
      Top = 218
      Width = 196
      Height = 33
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -21
      Font.Name = 'Tahoma'
      Font.Style = []
      MaxLength = 19
      ParentFont = False
      TabOrder = 4
      OnChange = Edit28Change
    end
    object StaticText41: TStaticText
      Left = 24
      Top = 285
      Width = 107
      Height = 25
      Caption = 'Nome cliente:'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -17
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 5
    end
    object Edit27: TEdit
      Left = 173
      Top = 280
      Width = 196
      Height = 33
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -21
      Font.Name = 'Tahoma'
      Font.Style = []
      MaxLength = 29
      ParentFont = False
      TabOrder = 6
      OnChange = Edit27Change
    end
    object ComboBoxCostruttore: TComboBox
      Left = 176
      Top = 41
      Width = 193
      Height = 22
      ItemHeight = 14
      MaxLength = 25
      TabOrder = 7
      Text = 'ComboBoxCostruttore'
      OnChange = ComboBoxCostruttoreChange
    end
    object ComboBoxModello: TComboBox
      Left = 177
      Top = 100
      Width = 192
      Height = 22
      ItemHeight = 14
      MaxLength = 25
      TabOrder = 8
      Text = 'ComboBox1'
      OnChange = ComboBoxModelloChange
    end
    object ComboBoxAllestimento: TComboBox
      Left = 176
      Top = 162
      Width = 193
      Height = 22
      ItemHeight = 14
      MaxLength = 25
      TabOrder = 9
      OnChange = ComboBoxAllestimentoChange
    end
  end
  object GroupBox9: TGroupBox
    Left = 360
    Top = 3
    Width = 191
    Height = 76
    Caption = ' Ventilazione '
    TabOrder = 4
    Visible = False
    object Label27: TLabel
      Left = 56
      Top = 24
      Width = 51
      Height = 14
      Caption = 'Manuale'
      Font.Charset = ANSI_CHARSET
      Font.Color = clGreen
      Font.Height = -12
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label28: TLabel
      Left = 55
      Top = 48
      Width = 71
      Height = 14
      Caption = 'Automatica'
      Font.Charset = ANSI_CHARSET
      Font.Color = clGray
      Font.Height = -12
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object VrRocker1: TVrRocker
      Left = 16
      Top = 25
      Width = 23
      Height = 39
      ShadowColor = clBtnShadow
      ShadowLightColor = clBtnFace
      HighlightColor = clBtnHighlight
      State = stUpperDown
      Style = rsSwitch
      OnUpperClick = VrRocker1UpperClick
      OnLowerClick = VrRocker1LowerClick
      Enabled = False
      TabOrder = 0
    end
  end
  object BitBtn1: TBitBtn
    Left = 672
    Top = 549
    Width = 81
    Height = 41
    Hint = 'Chiude e conferma modifiche'
    ParentShowHint = False
    ShowHint = True
    TabOrder = 5
    OnClick = BitBtn1Click
    Kind = bkOK
  end
  object RadioGroup1: TRadioGroup
    Left = 360
    Top = 83
    Width = 390
    Height = 105
    TabOrder = 6
  end
  object RadioButton1: TRadioButton
    Left = 393
    Top = 111
    Width = 176
    Height = 17
    Caption = 'Giri PTO'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 7
    OnClick = RadioButton1Click
  end
  object RadioButton2: TRadioButton
    Left = 393
    Top = 143
    Width = 119
    Height = 17
    Caption = 'Giri motore'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 8
    OnClick = RadioButton2Click
  end
  object StaticText6: TStaticText
    Left = 570
    Top = 126
    Width = 98
    Height = 18
    Caption = 'Rendimento della'
    TabOrder = 9
  end
  object StaticText7: TStaticText
    Left = 570
    Top = 142
    Width = 105
    Height = 18
    Caption = 'trasmissione  PTO:'
    TabOrder = 10
  end
  object NxNumberEdit2: TNxNumberEdit
    Left = 678
    Top = 139
    Width = 45
    Height = 24
    Enabled = False
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 11
    Text = '0,70'
    WantTabs = False
    OnChange = NxNumberEdit2Change
    Max = 1
    Min = 0.7
    Value = 0.7
  end
end
