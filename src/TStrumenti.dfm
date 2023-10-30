object Strumenti: TStrumenti
  Left = 347
  Top = 471
  ActiveControl = PageControl1
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  BorderWidth = 2
  Caption = 'Impostazioni degli strumenti...'
  ClientHeight = 435
  ClientWidth = 299
  Color = clBtnFace
  Font.Charset = ANSI_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'Tahoma'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poScreenCenter
  OnActivate = FormActivate
  OnClose = FormClose
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 16
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 299
    Height = 435
    ActivePage = TabSheet1
    Align = alClient
    TabIndex = 0
    TabOrder = 0
    TabPosition = tpBottom
    object TabSheet1: TTabSheet
      Caption = 'Impostazioni'
      object PageControl2: TPageControl
        Left = 0
        Top = 0
        Width = 291
        Height = 406
        ActivePage = TabSheet2
        Align = alClient
        MultiLine = True
        TabIndex = 3
        TabOrder = 0
        object TabSheet3: TTabSheet
          Caption = 'Giri-Coppia-Potenza'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          object Bevel2: TBevel
            Left = 8
            Top = 8
            Width = 264
            Height = 311
          end
          object GroupBox3: TGroupBox
            Left = 61
            Top = 16
            Width = 165
            Height = 85
            Caption = ' Giri Motore '
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
            object UpDown2: TUpDown
              Left = 59
              Top = 24
              Width = 15
              Height = 21
              Associate = Edit2
              Min = 1000
              Max = 18000
              Increment = 100
              Position = 1000
              TabOrder = 0
              Wrap = False
              OnClick = UpDown2Click
            end
            object Edit2: TEdit
              Left = 11
              Top = 24
              Width = 48
              Height = 21
              Color = clYellow
              ReadOnly = True
              TabOrder = 1
              Text = '1.000'
            end
            object Edit3: TEdit
              Left = 11
              Top = 50
              Width = 48
              Height = 21
              Color = clRed
              ReadOnly = True
              TabOrder = 2
              Text = '2.000'
            end
            object UpDown3: TUpDown
              Left = 59
              Top = 50
              Width = 15
              Height = 21
              Associate = Edit3
              Min = 2000
              Max = 19000
              Increment = 100
              Position = 2000
              TabOrder = 3
              Wrap = False
              OnClick = UpDown3Click
            end
            object UpDown4: TUpDown
              Left = 137
              Top = 37
              Width = 15
              Height = 21
              Associate = LabeledEdit1
              Min = 3000
              Max = 19000
              Increment = 500
              Position = 3000
              TabOrder = 4
              Wrap = False
              OnClick = UpDown4Click
            end
            object LabeledEdit1: TLabeledEdit
              Left = 88
              Top = 37
              Width = 49
              Height = 21
              EditLabel.Width = 33
              EditLabel.Height = 13
              EditLabel.Caption = '   Max'
              LabelPosition = lpAbove
              LabelSpacing = 3
              ReadOnly = True
              TabOrder = 5
              Text = '3.000'
            end
          end
          object GroupBox4: TGroupBox
            Left = 61
            Top = 120
            Width = 165
            Height = 85
            Caption = 'Coppia lorda '
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 1
            object Label6: TLabel
              Left = 16
              Top = 40
              Width = 28
              Height = 13
              Caption = 'Lb/ft'
            end
            object UpDown7: TUpDown
              Left = 105
              Top = 37
              Width = 15
              Height = 21
              Associate = LabeledEdit2
              Min = 5
              Max = 2000
              Increment = 5
              Position = 5
              TabOrder = 0
              Wrap = False
              OnClick = UpDown7Click
            end
            object LabeledEdit2: TLabeledEdit
              Left = 56
              Top = 37
              Width = 49
              Height = 21
              EditLabel.Width = 33
              EditLabel.Height = 13
              EditLabel.Caption = '   Max'
              LabelPosition = lpAbove
              LabelSpacing = 3
              ReadOnly = True
              TabOrder = 1
              Text = '5'
            end
          end
          object GroupBox5: TGroupBox
            Left = 61
            Top = 224
            Width = 165
            Height = 85
            Caption = ' Potenza lorda '
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 2
            object Label7: TLabel
              Left = 20
              Top = 40
              Width = 14
              Height = 13
              Caption = 'Cv'
            end
            object UpDown10: TUpDown
              Left = 105
              Top = 37
              Width = 15
              Height = 21
              Associate = LabeledEdit3
              Min = 5
              Max = 510
              Increment = 10
              Position = 5
              TabOrder = 0
              Wrap = False
              OnClick = UpDown10Click
            end
            object LabeledEdit3: TLabeledEdit
              Left = 56
              Top = 37
              Width = 49
              Height = 21
              EditLabel.Width = 33
              EditLabel.Height = 13
              EditLabel.Caption = '   Max'
              LabelPosition = lpAbove
              LabelSpacing = 3
              ReadOnly = True
              TabOrder = 1
              Text = '5'
            end
          end
        end
        object TabSheet4: TTabSheet
          Caption = 'Manifold - Sonda K '
          ImageIndex = 1
          object Bevel5: TBevel
            Left = 8
            Top = 8
            Width = 264
            Height = 321
          end
          object GroupBox6: TGroupBox
            Left = 48
            Top = 16
            Width = 178
            Height = 169
            Caption = ' Map '
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
            object SpeedButton3: TSpeedButton
              Left = 88
              Top = 125
              Width = 69
              Height = 25
              Caption = 'Offset'
              Glyph.Data = {
                76010000424D7601000000000000760000002800000020000000100000000100
                04000000000000010000120B0000120B00001000000000000000000000000000
                800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
                FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00337333733373
                3373337F3F7F3F7F3F7F33737373737373733F7F7F7F7F7F7F7F770000000000
                00007777777777777777330333333C333333337FFF3337F3333F370993333C33
                3399377773F337F33377330339333C3339333F7FF7FFF7FFF7FF770777977C77
                97777777777777777777330333933C339333337F3373F7F37333370333393C39
                3333377F333737F7333333033333999333333F7FFFFF777FFFFF770777777C77
                77777777777777777777330333333C333333337F333337F33333370333333C33
                3333377F333337F33333330333333C3333333F7FFFFFF7FFFFFF770777777777
                7777777777777777777733333333333333333333333333333333}
              NumGlyphs = 2
              OnClick = SpeedButton3Click
            end
            object Label9: TLabel
              Left = 24
              Top = 65
              Width = 30
              Height = 13
              Caption = 'mbar'
            end
            object Edit8: TEdit
              Left = 11
              Top = 112
              Width = 48
              Height = 21
              Color = clYellow
              ReadOnly = True
              TabOrder = 0
              Text = '0'
            end
            object Edit9: TEdit
              Left = 11
              Top = 141
              Width = 48
              Height = 21
              Color = clRed
              ReadOnly = True
              TabOrder = 1
              Text = '0'
            end
            object UpDown11: TUpDown
              Left = 59
              Top = 141
              Width = 22
              Height = 21
              Associate = Edit9
              Min = -400
              Max = 3000
              Increment = 50
              Position = 0
              TabOrder = 2
              Wrap = False
              OnClick = UpDown11Click
            end
            object UpDown12: TUpDown
              Left = 59
              Top = 112
              Width = 22
              Height = 21
              Associate = Edit8
              Min = -600
              Max = 3000
              Increment = 50
              Position = 0
              TabOrder = 3
              Wrap = False
              OnClick = UpDown12Click
            end
            object UpDown13: TUpDown
              Left = 141
              Top = 96
              Width = 15
              Height = 21
              Associate = LabeledEdit4
              Min = 500
              Max = 3000
              Increment = 100
              Position = 500
              TabOrder = 4
              Wrap = False
              OnClick = UpDown13Click
            end
            object LabeledEdit4: TLabeledEdit
              Left = 92
              Top = 96
              Width = 49
              Height = 21
              EditLabel.Width = 33
              EditLabel.Height = 13
              EditLabel.Caption = '   Max'
              LabelPosition = lpAbove
              LabelSpacing = 3
              ReadOnly = True
              TabOrder = 5
              Text = '500'
            end
            object LabeledEdit6: TLabeledEdit
              Left = 92
              Top = 55
              Width = 49
              Height = 21
              EditLabel.Width = 29
              EditLabel.Height = 13
              EditLabel.Caption = '   Min'
              LabelPosition = lpAbove
              LabelSpacing = 3
              ReadOnly = True
              TabOrder = 6
              Text = '0'
            end
            object UpDown1: TUpDown
              Left = 141
              Top = 55
              Width = 15
              Height = 21
              Associate = LabeledEdit6
              Min = -1000
              Max = 0
              Increment = 100
              Position = 0
              TabOrder = 7
              Wrap = False
              OnClick = UpDown1Click
            end
            object UpDown5: TUpDown
              Left = 59
              Top = 85
              Width = 22
              Height = 21
              Associate = Edit1
              Min = -1000
              Max = 3000
              Increment = 50
              Position = 0
              TabOrder = 8
              Wrap = False
              OnClick = UpDown5Click
            end
            object Edit1: TEdit
              Left = 11
              Top = 85
              Width = 48
              Height = 21
              Color = clLime
              ReadOnly = True
              TabOrder = 9
              Text = '0'
            end
            object rbPressType0: TRadioButton
              Left = 8
              Top = 16
              Width = 113
              Height = 17
              Caption = '-1....+2 bar'
              Font.Charset = ANSI_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentFont = False
              TabOrder = 10
              OnClick = rbPressType0Click
            end
            object rbPressType1: TRadioButton
              Left = 8
              Top = 40
              Width = 81
              Height = 17
              Caption = '-1....+6 bar'
              Font.Charset = ANSI_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = []
              ParentFont = False
              TabOrder = 11
              OnClick = rbPressType1Click
            end
          end
          object GroupBox7: TGroupBox
            Left = 48
            Top = 192
            Width = 178
            Height = 125
            Caption = ' Termocoppia K '
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 1
            object Label10: TLabel
              Left = 30
              Top = 29
              Width = 13
              Height = 13
              Caption = #176'C'
            end
            object UpDown14: TUpDown
              Left = 59
              Top = 72
              Width = 15
              Height = 21
              Associate = Edit10
              Min = 0
              Max = 1000
              Increment = 10
              Position = 0
              TabOrder = 0
              Wrap = False
              OnClick = UpDown14Click
            end
            object Edit10: TEdit
              Left = 11
              Top = 72
              Width = 48
              Height = 21
              Color = clYellow
              ReadOnly = True
              TabOrder = 1
              Text = '0'
            end
            object Edit11: TEdit
              Left = 11
              Top = 98
              Width = 48
              Height = 21
              Color = clRed
              ReadOnly = True
              TabOrder = 2
              Text = '0'
            end
            object UpDown15: TUpDown
              Left = 59
              Top = 98
              Width = 15
              Height = 21
              Associate = Edit11
              Min = 0
              Max = 1000
              Increment = 10
              Position = 0
              TabOrder = 3
              Wrap = False
              OnClick = UpDown15Click
            end
            object LabeledEdit5: TLabeledEdit
              Left = 88
              Top = 47
              Width = 49
              Height = 21
              EditLabel.Width = 33
              EditLabel.Height = 13
              EditLabel.Caption = '   Max'
              LabelPosition = lpAbove
              LabelSpacing = 3
              ReadOnly = True
              TabOrder = 4
              Text = '20'
            end
            object UpDown16: TUpDown
              Left = 137
              Top = 47
              Width = 15
              Height = 21
              Associate = LabeledEdit5
              Min = 20
              Max = 1000
              Increment = 10
              Position = 20
              TabOrder = 5
              Wrap = False
              OnClick = UpDown16Click
            end
            object Edit5: TEdit
              Left = 11
              Top = 47
              Width = 48
              Height = 21
              Color = clLime
              ReadOnly = True
              TabOrder = 6
              Text = '0'
            end
            object UpDown6: TUpDown
              Left = 59
              Top = 47
              Width = 15
              Height = 21
              Associate = Edit5
              Min = 0
              Max = 1000
              Increment = 10
              Position = 0
              TabOrder = 7
              Wrap = False
              OnClick = UpDown6Click
            end
          end
        end
        object TabSheet5: TTabSheet
          Caption = 'Velocit'#224' - T. Olio'
          ImageIndex = 2
          object Bevel3: TBevel
            Left = 8
            Top = 8
            Width = 264
            Height = 311
          end
          object GroupBox10: TGroupBox
            Left = 61
            Top = 40
            Width = 165
            Height = 137
            Caption = 'Temperatura Olio'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
            object Label11: TLabel
              Left = 28
              Top = 29
              Width = 13
              Height = 13
              Caption = #176'C'
            end
            object Edit6: TEdit
              Left = 12
              Top = 45
              Width = 48
              Height = 21
              Color = clLime
              ReadOnly = True
              TabOrder = 0
              Text = '0'
            end
            object Edit7: TEdit
              Left = 12
              Top = 70
              Width = 48
              Height = 21
              Color = clYellow
              ReadOnly = True
              TabOrder = 1
              Text = '0'
            end
            object Edit12: TEdit
              Left = 12
              Top = 95
              Width = 48
              Height = 21
              Color = clRed
              ReadOnly = True
              TabOrder = 2
              Text = '0'
            end
            object UpDown8: TUpDown
              Left = 60
              Top = 45
              Width = 16
              Height = 21
              Associate = Edit6
              Min = 0
              Max = 320
              Increment = 5
              Position = 0
              TabOrder = 3
              Wrap = False
              OnClick = UpDown8Click
            end
            object UpDown9: TUpDown
              Left = 60
              Top = 70
              Width = 16
              Height = 21
              Associate = Edit7
              Min = 0
              Max = 320
              Increment = 5
              Position = 0
              TabOrder = 4
              Wrap = False
              OnClick = UpDown9Click
            end
            object UpDown17: TUpDown
              Left = 60
              Top = 95
              Width = 16
              Height = 21
              Associate = Edit12
              Min = 0
              Max = 320
              Increment = 5
              Position = 0
              TabOrder = 5
              Wrap = False
              OnClick = UpDown17Click
            end
            object LabeledEdit8: TLabeledEdit
              Left = 88
              Top = 47
              Width = 49
              Height = 21
              EditLabel.Width = 29
              EditLabel.Height = 13
              EditLabel.Caption = '   Min'
              LabelPosition = lpAbove
              LabelSpacing = 3
              ReadOnly = True
              TabOrder = 6
              Text = '0'
            end
            object LabeledEdit9: TLabeledEdit
              Left = 88
              Top = 93
              Width = 49
              Height = 21
              EditLabel.Width = 33
              EditLabel.Height = 13
              EditLabel.Caption = '   Max'
              LabelPosition = lpAbove
              LabelSpacing = 3
              ReadOnly = True
              TabOrder = 7
              Text = '0'
            end
            object UpDown18: TUpDown
              Left = 137
              Top = 47
              Width = 16
              Height = 21
              Associate = LabeledEdit8
              Min = 0
              Increment = 5
              Position = 0
              TabOrder = 8
              Wrap = False
              OnClick = UpDown18Click
            end
            object UpDown19: TUpDown
              Left = 137
              Top = 93
              Width = 16
              Height = 21
              Associate = LabeledEdit9
              Min = 0
              Max = 300
              Increment = 5
              Position = 0
              TabOrder = 9
              Wrap = False
              OnClick = UpDown19Click
            end
          end
        end
        object TabSheet2: TTabSheet
          Caption = 'Indicatori'
          ImageIndex = 4
          object Bevel1: TBevel
            Left = 8
            Top = 8
            Width = 264
            Height = 318
          end
          object CheckBox6: TCheckBox
            Left = 40
            Top = 29
            Width = 115
            Height = 17
            Caption = 'Manifold'
            Checked = True
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = [fsBold]
            ParentFont = False
            State = cbChecked
            TabOrder = 0
            OnClick = CheckBox6Click
          end
          object CheckBox7: TCheckBox
            Left = 40
            Top = 87
            Width = 103
            Height = 17
            Caption = 'Temp. K'
            Checked = True
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = [fsBold]
            ParentFont = False
            State = cbChecked
            TabOrder = 2
            OnClick = CheckBox7Click
          end
          object CheckBox8: TCheckBox
            Left = 40
            Top = 59
            Width = 115
            Height = 17
            Caption = 'Temp. Olio'
            Checked = True
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = [fsBold]
            ParentFont = False
            State = cbChecked
            TabOrder = 1
            OnClick = CheckBox8Click
          end
          object GroupBox2: TGroupBox
            Left = 17
            Top = 167
            Width = 248
            Height = 50
            Caption = 'Barra degli strumenti analogici'
            TabOrder = 4
            object CheckBox11: TCheckBox
              Left = 25
              Top = 24
              Width = 120
              Height = 17
              Caption = 'nascondi'
              Font.Charset = ANSI_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'Tahoma'
              Font.Style = [fsBold]
              ParentFont = False
              TabOrder = 0
              OnClick = CheckBox11Click
            end
          end
          object GroupBox8: TGroupBox
            Left = 17
            Top = 216
            Width = 248
            Height = 50
            Caption = ' Allarmi visivi '
            TabOrder = 5
            object CheckBox12: TCheckBox
              Left = 24
              Top = 24
              Width = 81
              Height = 17
              Caption = 'Attivi'
              TabOrder = 0
              OnClick = CheckBox12Click
            end
          end
          object GroupBox13: TGroupBox
            Left = 17
            Top = 267
            Width = 248
            Height = 55
            Caption = 'Indica massimo valore a fine test di:'
            TabOrder = 6
            object ComboBox1: TComboBox
              Left = 36
              Top = 22
              Width = 177
              Height = 24
              AutoComplete = False
              Color = clBtnFace
              ItemHeight = 16
              TabOrder = 0
              OnChange = ComboBox1Change
            end
          end
          object CheckBox1: TCheckBox
            Left = 40
            Top = 140
            Width = 103
            Height = 17
            Caption = 'Consumi'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 3
            OnClick = CheckBox1Click
          end
          object cbTAir: TCheckBox
            Left = 40
            Top = 114
            Width = 103
            Height = 17
            Caption = 'Temp. Aria'
            Checked = True
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Style = [fsBold]
            ParentFont = False
            State = cbChecked
            TabOrder = 7
            OnClick = cbTAirClick
          end
        end
      end
    end
  end
  object Timer1: TTimer
    Enabled = False
    Interval = 200
    Left = 88
    Top = 368
  end
  object SaveDialog1: TSaveDialog
    DefaultExt = 'trc'
    Filter = '|*.trc'
    Options = [ofOverwritePrompt, ofHideReadOnly, ofEnableSizing]
    Left = 32
    Top = 368
  end
end
