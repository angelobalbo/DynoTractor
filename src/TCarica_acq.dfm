object Carica_acq: TCarica_acq
  Left = 507
  Top = 77
  Anchors = [akLeft]
  BorderStyle = bsDialog
  Caption = 'Carica_acq'
  ClientHeight = 653
  ClientWidth = 825
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object NxHeaderPanel2: TNxHeaderPanel
    Left = 0
    Top = 0
    Width = 245
    Height = 653
    AdaptiveColors = True
    Align = alLeft
    BorderColor = clBlack
    Caption = 'Percorso:'
    HeaderColor = clHighlight
    HeaderFont.Charset = DEFAULT_CHARSET
    HeaderFont.Color = clWindowText
    HeaderFont.Height = -11
    HeaderFont.Name = 'MS Sans Serif'
    HeaderFont.Style = []
    InnerMargins.Bottom = 4
    InnerMargins.Left = 9
    InnerMargins.Right = 8
    InnerMargins.Top = 5
    Options = []
    TabOrder = 0
    DesignSize = (
      243
      651)
    object DirectoryListBox1: TDirectoryListBox
      Left = 9
      Top = 48
      Width = 226
      Height = 599
      Anchors = [akLeft, akTop, akRight, akBottom]
      Color = clSilver
      FileList = FileListBox1
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ItemHeight = 16
      ParentFont = False
      TabOrder = 0
    end
    object DriveComboBox1: TDriveComboBox
      Left = 9
      Top = 24
      Width = 224
      Height = 22
      DirList = DirectoryListBox1
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 1
      TabStop = False
    end
  end
  object NxHeaderPanel3: TNxHeaderPanel
    Left = 245
    Top = 0
    Width = 236
    Height = 653
    AdaptiveColors = True
    Align = alLeft
    BorderColor = clBlack
    Caption = 'File di acquisizione'
    HeaderColor = clHighlight
    HeaderFont.Charset = DEFAULT_CHARSET
    HeaderFont.Color = clWindowText
    HeaderFont.Height = -11
    HeaderFont.Name = 'MS Sans Serif'
    HeaderFont.Style = []
    InnerMargins.Bottom = 4
    InnerMargins.Left = 9
    InnerMargins.Right = 8
    InnerMargins.Top = 5
    Options = []
    TabOrder = 1
    object FileListBox1: TFileListBox
      Left = 9
      Top = 24
      Width = 217
      Height = 623
      Align = alClient
      Color = clSilver
      FileType = [ftDirectory, ftArchive, ftNormal]
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ItemHeight = 16
      Mask = '*.acq'
      ParentFont = False
      ParentShowHint = False
      ShowGlyphs = True
      ShowHint = True
      TabOrder = 0
      OnClick = FileListBox1Click
    end
  end
  object NxHeaderPanel5: TNxHeaderPanel
    Left = 481
    Top = 0
    Width = 344
    Height = 653
    AdaptiveColors = True
    Align = alClient
    BorderColor = clBlack
    Caption = 'Note:'
    Color = clSilver
    HeaderColor = clHighlight
    HeaderFont.Charset = DEFAULT_CHARSET
    HeaderFont.Color = clWindowText
    HeaderFont.Height = -11
    HeaderFont.Name = 'MS Sans Serif'
    HeaderFont.Style = []
    InnerMargins.Bottom = 4
    InnerMargins.Left = 9
    InnerMargins.Right = 8
    InnerMargins.Top = 5
    Options = []
    TabOrder = 2
    object NxMemo1: TNxMemo
      Left = 9
      Top = 24
      Width = 325
      Height = 257
      Align = alTop
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      MaxLength = 599
      ParentFont = False
      TabOrder = 0
    end
    object NxHeaderPanel4: TNxHeaderPanel
      Left = 9
      Top = 281
      Width = 325
      Height = 366
      AdaptiveColors = True
      Align = alClient
      BorderColor = clBlack
      Caption = 'Dettagli:'
      Color = clSilver
      HeaderColor = clHighlight
      HeaderFont.Charset = DEFAULT_CHARSET
      HeaderFont.Color = clWindowText
      HeaderFont.Height = -11
      HeaderFont.Name = 'MS Sans Serif'
      HeaderFont.Style = []
      InnerMargins.Bottom = 4
      InnerMargins.Left = 9
      InnerMargins.Right = 8
      InnerMargins.Top = 5
      Options = []
      TabOrder = 1
      object NxLabel10: TNxLabel
        Left = 16
        Top = 56
        Width = 101
        Height = 16
        Caption = 'Data/ora  del file:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        HorizontalPosition = hpLeft
        InnerHorizontal = True
        InnerVertical = False
        InnerMargins.Horizontal = 0
        InnerMargins.Vertical = 2
        VerticalPosition = vpTop
      end
      object NxLabel13: TNxLabel
        Left = 168
        Top = 56
        Width = 21
        Height = 16
        Caption = '----'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clRed
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        HorizontalPosition = hpLeft
        InnerHorizontal = True
        InnerVertical = False
        InnerMargins.Horizontal = 0
        InnerMargins.Vertical = 2
        VerticalPosition = vpTop
      end
      object NxLabel14: TNxLabel
        Left = 16
        Top = 88
        Width = 142
        Height = 16
        Caption = 'Tempo di acquisizione: '
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        HorizontalPosition = hpLeft
        InnerHorizontal = True
        InnerVertical = False
        InnerMargins.Horizontal = 0
        InnerMargins.Vertical = 2
        VerticalPosition = vpTop
      end
      object NxLabel15: TNxLabel
        Left = 208
        Top = 88
        Width = 21
        Height = 16
        Caption = '----'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clRed
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        HorizontalPosition = hpLeft
        InnerHorizontal = True
        InnerVertical = False
        InnerMargins.Horizontal = 0
        InnerMargins.Vertical = 2
        VerticalPosition = vpTop
      end
      object NxLabel16: TNxLabel
        Left = 18
        Top = 123
        Width = 161
        Height = 16
        Caption = 'Tempo di campionamento:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        HorizontalPosition = hpLeft
        InnerHorizontal = True
        InnerVertical = False
        InnerMargins.Horizontal = 0
        InnerMargins.Vertical = 2
        VerticalPosition = vpTop
      end
      object NxLabel17: TNxLabel
        Left = 208
        Top = 123
        Width = 21
        Height = 16
        Caption = '----'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clRed
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        HorizontalPosition = hpLeft
        InnerHorizontal = True
        InnerVertical = False
        InnerMargins.Horizontal = 0
        InnerMargins.Vertical = 2
        VerticalPosition = vpTop
      end
      object NxLabel18: TNxLabel
        Left = 18
        Top = 187
        Width = 155
        Height = 16
        Caption = 'Rendimento trasmissione:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        HorizontalPosition = hpLeft
        InnerHorizontal = True
        InnerVertical = False
        InnerMargins.Horizontal = 0
        InnerMargins.Vertical = 2
        VerticalPosition = vpTop
      end
      object NxLabel19: TNxLabel
        Left = 208
        Top = 187
        Width = 21
        Height = 16
        Caption = '----'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clRed
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        HorizontalPosition = hpLeft
        InnerHorizontal = True
        InnerVertical = False
        InnerMargins.Horizontal = 0
        InnerMargins.Vertical = 2
        VerticalPosition = vpTop
      end
      object NxLabel20: TNxLabel
        Left = 18
        Top = 218
        Width = 156
        Height = 16
        Caption = '---------------------------------------'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        Visible = False
        HorizontalPosition = hpLeft
        InnerHorizontal = True
        InnerVertical = False
        InnerMargins.Horizontal = 0
        InnerMargins.Vertical = 2
        VerticalPosition = vpTop
      end
      object NxLabel21: TNxLabel
        Left = 208
        Top = 218
        Width = 21
        Height = 16
        Caption = '----'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clRed
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        Visible = False
        HorizontalPosition = hpLeft
        InnerHorizontal = True
        InnerVertical = False
        InnerMargins.Horizontal = 0
        InnerMargins.Vertical = 2
        VerticalPosition = vpTop
      end
      object NxLabel22: TNxLabel
        Left = 18
        Top = 154
        Width = 153
        Height = 16
        Caption = 'Rapporto di trasmissione:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        HorizontalPosition = hpLeft
        InnerHorizontal = True
        InnerVertical = False
        InnerMargins.Horizontal = 0
        InnerMargins.Vertical = 2
        VerticalPosition = vpTop
      end
      object NxLabel23: TNxLabel
        Left = 208
        Top = 154
        Width = 21
        Height = 16
        Caption = '----'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clRed
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        HorizontalPosition = hpLeft
        InnerHorizontal = True
        InnerVertical = False
        InnerMargins.Horizontal = 0
        InnerMargins.Vertical = 2
        VerticalPosition = vpTop
      end
      object Image5: TImage
        Left = 263
        Top = 288
        Width = 46
        Height = 52
        Picture.Data = {
          07544269746D617096250000424D962500000000000036000000280000002E00
          000034000000010020000000000060250000C40E0000C40E0000000000000000
          0000F5EAE500F5EAE500F4EAE500F5EAE500F4EAE500F5EAE500F4EAE500F5EA
          E500F4EAE500F5EAE500F5EAE500F5EAE500F5EAE500F5EAE500F5EAE500F5EA
          E500F5EAE500F5EAE500F5EAE500F5EAE500F5EAE500F4EAE500F5EAE500F5EA
          E500F5EAE500F4EAE500F5EAE500F5EAE500F5EAE500F5EAE500F5EAE500F5EA
          E500F5EAE500F5EAE500F5EAE500F5EAE500F5EAE500F5EAE500F5EAE500F6EA
          E500F5EAE500F5EAE500F5EAE500F5EAE500F5EAE500F5EAE500F4EAE500F5EA
          E500F5EAE500F5EAE500F4EAE500F5EAE500F5EAE500F5EAE500F5EAE500F5EA
          E500F4EAE500F5EAE500F5EAE500F5EAE500F4EAE500F5EAE500F5EAE500F5EA
          E500F5EAE500F5EAE500F5EAE500F5EAE500F5EAE500F5EAE500F5EAE500F5EA
          E500F5EAE500F5EAE500F5EAE500F5EAE500F5EAE500F5EAE500F5EAE500F5EA
          E500F5EAE500F5EAE500F5EAE500F5EAE500F5EAE500F5EAE500F5EAE500F5EA
          E500F6EAE500F5EAE500F5EAE500F5EAE500F4E9E400F4E9E400F3E9E400F3E9
          E400F4E9E400F4E9E400F4E9E400F4E9E400F3E9E400F4E9E400F4E9E400F4E9
          E400F4E9E400F4E9E400F3E9E400F3E9E400F3E9E400F4E9E400F4E9E400E6D2
          C800D3B3A200BB8B7100A7694800A5674600A96C4D00D2B7AA00F4E9E400F4E9
          E400F4E9E400F4E9E400F4E9E400F4E9E400F4E9E400F4E9E400F4E9E400F4E9
          E400F4E9E400F4E9E400F4E9E400F4E9E400F4E9E400F4E9E400F4E9E400F4E9
          E400F4E9E400F5EAE500F4E9E400F4E9E400F3E9E400F4E9E400F4E9E400F3E9
          E400F4E9E400F4E9E400F3E9E400F4E9E400F4E9E400F4E9E400F3E9E400F4E9
          E400F3E9E400EFE2DB00D9BFB000C7A28D00B5846800AB735300AA715100A96E
          4E00A86C4B00A7694800A56645008C4F3300F4E9E400F4E9E400F3E9E400F4E9
          E400D9D0CC00D8CFCB00F4E9E400F4E9E400F4E9E400F4E9E400F4E9E400F4E9
          E400F4E9E400F4E9E400F4E9E400F4E9E400F4E9E400F4E9E400F4E9E400F5EA
          E500F4E9E400F4E9E400F3E9E400F4E9E400F4E9E400F3E9E400F4E9E400F4E9
          E400F3E9E400F4E9E400F4E9E400F4E9E400E3D0C500D3B6A400BD947A00B07E
          6000AF7C5E00AE7A5B00AD785900AC765600AB735400AA715100A96E4E00A86C
          4B00A669480084442700DBCEC700B8B2B0009692910071707000676767006565
          650084828000DFD6D100F4E9E400F4E9E400F4E9E400F4E9E400F4E9E400F4E9
          E400F4E9E400F4E9E400F4E9E400F4E9E400F4E9E400F5EAE500F4E9E400F4E9
          E400F4E9E400F4E9E400F4E9E400F4E9E400F4E9E400F4E9E400EBDDD500DCC6
          B800C8A89300B88F7300B3876A00B3856800B2836600B1816300B07F6100AF7D
          5E00AE7B5C00AD785900AC765600AB735400AA715100A96E4E00A76B4B008444
          2800B49176008F7E72006D6D6D006C6C6C006A6A6A0068686800616161005B5B
          5A00ACA6A300F4E9E400F4E9E400F4E9E400F4E9E400F4E9E400F4E9E400F4E9
          E400F4E9E400F4E9E400F5E9E400F5EAE500F4E9E400F4E9E400F3E9E400F4E9
          E400F4E9E400E1CEC200D2B8A600BF9C8300B7907400B78F7200B68D7100B68C
          6F00B58A6D00B4886B00B3866900B2846700B1826400B0806200AF7D5F00AE7B
          5C00AD785900AC765600AB735300AA705000A86D4D0085452900BA967A00B994
          7800A689740078736F006D6D6D006B6B6B006363630051515100515151007A77
          7600DFD6D100F4E9E400F4E9E400F4E9E400F4E9E400F4E9E400F4E9E400F4E9
          E400F4E9E400F5EAE500F4E9E400F4E9E400F4E9E400F0E4DD00C5A68F00BA96
          7B00BA957A00B9947900B9937700B8927600B8917400B78F7300B68D7100B58B
          6E00B4896C00B3876A00B2856700B1826500B0806200AF7D5F00AE7B5C00AD78
          5900AC755600AB735300A970500085452900BB987D00BA967B00B9947900B490
          7600707070006E6E6E006666660051515100515151005151510051515100A29D
          9A00F4E9E400F4E9E400F4E9E400F4E9E400F4E9E400F4E9E400F4E9E400F5EA
          E500F3E9E400F4E9E400F4E9E400D1B8A500BC9A7F00BB9A7F00BB997E00BB98
          7D00BA977B00BA957A00B9947800B8927600B7907400B78E7200B68C6F00B58A
          6D00B4876A00B3856700B1826500B0806200AF7D5F00AE7A5B00AD775800AC75
          5500AA72520085462A00BC9B8000BB997E00BA977B00B99579007C7774007171
          7100686868005151510051515100515151005151510051515100706E6D00D5CC
          C800F4E9E400F4E9E400F4E9E400F4E9E400F4E9E400F5EAE500F4E9E400F3E9
          E400F4E9E400CAB19C00BD9E8300BD9D8200BD9C8200BC9B8000BC9A7F00BB99
          7D00BA977C00BA957A00B9937700B8917500B78F7300B68C7000B58A6D00B487
          6A00B0836600B1826400B07F6100AF7C5E00AE7A5B00AC775700AB7454008F51
          3400BD9E8300BC9B8100BB997E00BA977B00877E7700757575006B6B6B005151
          5100515151005151510051515100515151005151510051515100A29D9A00F4E9
          E400F4E9E400F4E9E400F4E9E400F5EAE500F4E9E400F3E9E400F4E9E400CCB4
          9F00BFA18700BFA18600BEA08500BE9F8400BD9D8300BC9C8100BC9A7F00BB98
          7D00BA967B00B9947800BB997E00BD9D8300BFA28700C1A68C009C6E53009963
          4600B1826300B07F6000AF7C5D00AD795900AC75560092563800BEA08600BD9E
          8300BC9B8100BB997E008A817A00787878006D6D6D0051515100515151005151
          5100515151005151510051515100515151005151510084817F00F4E9E400F4E9
          E400F4E9E400F5EAE500F4E9E400F4E9E400F3E9E400CDB6A200C0A58B00C0A4
          8A00C0A38900BFA28800BFA18600BE9F8500C0A58B00C3AA9100C4AF9600C7B3
          9B00C5B09800C4AD9500C3AA9100C1A78E009D6E5400965F4300B2836600B181
          6200AF7D5F00AE7A5C00AD77580093573900BFA38900BEA08600BD9E8300BC9B
          80008C847D007B7B7B006F6F6F00515151005151510051515100515151005151
          51005151510051515100515151007A777600F4E9E400F4E9E400F4E9E400F5EA
          E500F4E9E400F4E9E400F3E9E400CEB9A500C2A88F00C2A88E00C3AA9100C7B4
          9C00CABAA300CCC0A900CBBDA700CABBA400C9B8A000C7B59D00C6B29900C4AF
          9600C3AB9200C2A88F009D6F550096604400B3856800B1826400B07F6100AF7C
          5D00AD795A0093573A00C0A58B00BFA28800BEA08500BD9D82008F8680007E7E
          7E00727272005151510051515100515151005151510051515100515151005151
          5100515151007A777600F4E9E400F4E9E400F4E9E400F5EAE500F3E9E400F4E9
          E400F3E9E400D0BBA800C3AC9300C3AB9200C9B9A100CFC6B100CEC5AF00CDC2
          AC00CCBFA900CABCA500C9B9A200C8B69E00C6B39B00C5AF9700B18F76009D70
          5600864B30009E6B4F00B4876A00B2846600B1816300AF7E5F00AE7A5C009458
          3B00C1A78D00C0A48B00BFA28800BE9F84009189830081818100747474005151
          5100515151005151510051515100515151005151510051515100515151007A77
          7600F4E9E400F4E9E400F4E9E400F5EAE500F4E9E400F4E9E400F4E9E400CBB7
          A100C5AF9700C4AF9600CABCA500D0C9B400CFC7B100CEC4AE00CCC0AA00C1AD
          9500B09179009C6F5600905C4100925C4100A1725700AC816600B78F7300B68C
          6F00B4896C00B3866800B1826400B07F6100AF7C5D0094593C00C2A99000C1A6
          8D00C0A38900BEA18600948C8500848484007D7D7D0051515100515151005151
          51005151510051515100515151005151510051515100706E6D00F4E9E400F4E9
          E400F4E9E400F5EAE500F4E9E400F3E9E400F4E9E400C7B49C00C6B39B00C6B2
          9A00CCBFA800C7B59D00BA987C00A67355009C684C0099694E00A9826700BA9A
          7F00BD9D8300BC9A7F00BA977C00B9947800B8917500B68E7100B58A6D00B387
          6A00B2846600B1806200AF7D5E0094593C00B5957C00C2A88E00C0A58B00BFA2
          8800968F88008787870086868600515151005151510051515100515151005151
          510051515100515151005151510051515100F4E9E400F4E9E400F4E9E400F5EA
          E500F4E9E400F3E9E400F3E9E400C8B7A000C8B69F00C7B59D00C5AF9600B891
          7500BA977C00C4AD9500C3AB9200C2A88F00C0A58B00BFA28800BE9F8500BD9C
          8100BB997D00BA967A00B8927600B78F7300B58C6F00B4886B00B2856700B182
          6300B07E6000945A3D00915C4200B99B8100C1A68D00C0A3890099918B008B8B
          8B00898989005151510051515100515151005151510051515100515151005151
          51005151510051515100F4E9E400F4E9E400F4E9E400F5EAE500F3E9E400F4E9
          E400F4E9E400CABBA400C9BAA300C9B9A100C8B79F00C7B59D00C6B29A00C5B0
          9700C4AD9400C3AA9100C1A78D00C0A48A00BFA18600BD9E8300BC9A7F00BA97
          7B00B9937800B7907400B68D7000B4896C00B3866900B2836500B07F6100945A
          3D00326E9C005A5F700099684E00BB9D83009B948D008E8E8E008C8C8C005151
          5100515151005151510051515100515151005151510051515100515151005151
          5100F4E9E400F4E9E400F4E9E400F5EAE500F3E9E400F4E9E400F3E9E400CBBE
          A700CBBDA600CABCA500C9BAA300C8B7A000C7B59D00C6B29A00C5AF9600C3AC
          9300C2A98F00C1A58C00BFA28800BE9F8400BC9B8000BB987D00B9957900B891
          7500B68E7100B58A6D00B3876900B2836600B0806200794631001289D7004C9C
          DE006D656100613A290090827C00919191008F8F8F0051515100515151005151
          5100515151005151510051515100515151005151510051515100F3E9E400F4E9
          E400F4E9E400F5EAE500F3E9E400F4E9E400F4E9E400CDC2AB00CCC1AA00CCBF
          A800CBBDA600C9BAA300C8B79F00C7B49C00C5B19800C4AD9500C2AA9100C1A7
          8D00C0A38900BEA08600BD9C8200BB997E00BA967A00B8927600B78F7200B58B
          6E00B4886A00AB7A5D0081503A00396E9200128CD8004C9DDE00646464002C2C
          2C007B7B7B009494940092929200515151005151510051515100515151005151
          510051515100515151005151510051515100F4E9E400F4E9E400F4E9E400F5EA
          E500F4E9E400F4E9E400F3E9E400CEC5AF00CEC4AE00CDC2AB00CCBFA800CABC
          A500C9B9A100C7B59D00C6B29A00C4AF9600C3AB9200C2A88E00C0A48A00BFA1
          8600BD9D8300BC9A7F00B7917500AD7E61009F674800766D67005C686F00406E
          8800198FCF001392DA00138FD9004C9EDE00646464002C2C2C00696969009797
          9700959595005151510051515100515151005151510051515100515151005151
          51005151510051515100F4E9E400F4E9E400F4E9E400F5EAE500F4E9E400F4E9
          E400F3E9E400D0C9B300CFC7B100CEC4AE00CCC1AA00CBBDA600C9BAA200C8B6
          9F00C6B39B00C5AF9700C3AC9300BB9C8100B1866900A8725300807A6F00607B
          8100438298002B93BC0015A3DF0014A0DE00149DDD00149BDD001497DC001394
          DB001391DA004C9EDE00676767002C2C2C00646464009A9A9A00989898005151
          5100515151005151510051515100515151005151510051515100515151005151
          5100F4E9E400F4E9E400F4E9E400F5EAE500F3E9E400F3E9E400F4E9E400D1CC
          B600D0C8B300CEC5AF00CDC2AB00CBBEA700C0AA9300B08F77009B7F69007979
          6F0054848A00428C9C002DA0C00017B4E50016B1E40016AFE30016ACE20015A9
          E10015A6E00015A3DF0014A0DE00149DDD00149ADC001397DB001394DA004C9F
          DF008D8D8D002C2C2C007B7B7B009D9D9D009C9C9C0051515100515151005151
          5100515151005151510051515100515151005151510051515100F3E9E400F4E9
          E400F4E9E400F5EAE500F3E9E400F4E9E400F3E9E400E3D6CC00CBB7A600C3AC
          9900767E74004E888D003699AA0024B2D00018C2E90018C0E90018BEE80017BC
          E70017BAE70017B8E60017B5E50016B2E40016AFE30016ADE20015A9E10015A6
          E00015A3DF0014A0DE00149DDD00149ADC001396DB004CA0DF009A9A9A006161
          6100A2A2A200A0A0A0009F9F9F00515151005151510051515100515151005151
          510051515100515151005151510051515100F3E9E400F4E9E400F4E9E400F5EA
          E500F3E9E400F4E9E400F4E9E400F4E9E400F3E9E400F5EEEA0019CBEC0019CA
          EC0019C9EB0019C8EB0018C6EA0018C5EA0018C2E90018C0E90018BEE80017BB
          E70017B8E60017B6E50016B3E40016B0E30016ADE20015A9E10015A6E00015A3
          DF00149FDE00149CDD001499DC004CA0DF00BBBBBB00A7A7A700A5A5A500A4A4
          A400A2A2A2005151510051515100515151005151510051515100515151005151
          51005151510051515100F3E9E400F4E9E400F4E9E400F4EAE500F4E9E400F4E9
          E400F3E9E400F4E9E400F3E9E400F5EEEA0019D0ED0019CFED0019CEED0019CC
          EC0019CBEC0019C9EB0018C6EA0018C4EA0018C1E90018BFE80017BCE70017B9
          E60017B6E50016B3E40016AFE30016ACE20015A9E10015A5E00015A2DF00149E
          DE00149BDD004DA1DF00BDBDBD00AAAAAA00A8A8A800A7A7A700A5A5A5005151
          5100515151005151510051515100515151005151510051515100515151005151
          5100F3E9E400F4E9E400F4E9E400F4EAE500F4E9E400F3E9E400F3E9E400F3E9
          E400F4E9E400F6EEEA001AD4EF001AD3EF001AD2EE001AD0EE0019CFED0019CD
          EC0019CAEC0019C8EB0018C5EA0018C2E90018BFE80017BCE70017B9E60017B6
          E50016B2E40016AFE30016ABE20015A8E10015A4E00015A1DF00149DDD004DA1
          DF00A8999000A67F6B00A59D9A00AAAAAA00A8A8A80051515100515151005151
          5100515151005151510051515100515151005151510051515100F4E9E400F4E9
          E400F4E9E400F5EAE500F4E9E400F4E9E400F4E9E400F3E9E400F3E9E400F5EE
          EA001AD9F0001AD8F0001AD6F0001AD5EF001AD3EE001AD1EE0019CEED0019CB
          EC0019C9EB0018C5EA0018C2E90018BFE80017BCE70017B9E60017B5E50016B1
          E40016AEE30016AAE20015A7E0001498DC0030A1DF0091B9DA008B6D5D00A76A
          49008B533A00ADADAD00ABABAB00515151005151510051515100515151005151
          510051515100515151005151510051515100F4E9E400F4E9E400F3E9E400F5EA
          E500F3E9E400F4E9E400F4E9E400F4E9E400F3E9E400F5EEEA001BDDF2001BDC
          F1001BDBF1001AD9F0001AD7F0001AD4EF001AD2EE0019CFED0019CCEC0019C9
          EB0018C6EA0018C2E90018BFE80016AEE30024ABE2004FB7E6007AC0E900A6D2
          EF00B1CBE200DEDBD900C4B7AF0090746400A96F4F00A86C4C0085452800B0B0
          B000AEAEAE005252520052525200515151005151510051515100515151005151
          51005151510051515100F4E9E400F4E9E400F4E9E400F5EAE500F3E9E400F3E9
          E400F3E9E400F3E9E400F4E9E400F9F3F1001BE2F3001BE1F3001BDFF2001BDD
          F2001BDBF1001AD8F0001AD5EF0018C1E90017B9E60024AFE3007BCBED0097D1
          EF00B0CAE000D1D4D800D0C4BD00C0AB9E00B4998800A37E6700AB7B5F00AD79
          5A00AC775700AB745400AA715100A96F4E0085452900B3B3B300B1B1B1005454
          5400545454005353530053535300525252005252520052525200525252005151
          5100F4E9E400F4E9E400F4E9E400F5EAE500F3E9E400F3E9E400F3E9E400F3E9
          E400F4E9E400F7EFEC002AE8F5001AD7F00019CEED0018C6EB0018C5EA0018C3
          E90018C2E90019D1EE0019C9EB00A5CBED00D1CCC800B2937E00B48D7200B58A
          6D00B4886A00B3856800B2836500B1816200B07E6000AF7C5D00AD795A00AC76
          5700AB745400AA71510085462900B6B6B600B5B5B50057575700575757005656
          5600565656005555550054545400545454005353530053535300F4E9E400F4E9
          E400F4E9E400F5EAE500F4E9E400F4E9E400F3E9E400F3E9E400F3E9E400F0E8
          E400F2F2F20046EDF7001CE8F5001CE5F4001CE3F4001BE0F3001BDDF2001BDA
          F10016ADE200EBF1F700B5A29400B8917500B78F7200B68D7000B58A6D00B488
          6B00B3866800B2836500B1816200B07E6000AE7B5C00AD785900AC765600AB73
          530085462A00B9B9B900B8B8B8005B5B5B005B5B5B005A5A5A00595959005858
          580058585800575757005656560056565600F4E9E400F4E9E400F4E9E400F5EA
          E500F3E9E400F4E9E400F4E9E400F4E9E400F3E9E400F0E9E500E9E9E900A7F5
          F9001CEBF6001CE9F5001CE6F5001CE3F4001BE0F3001BDDF2006BB6E600E5E5
          E500B99A8200B9947800B8927500B78F7300B68D7000B58B6E00B4886B00B385
          6800B2836500B0806200AF7D5F00AE7B5C00AD785800AC75550085472B00BDBD
          BD00BBBBBB00606060005F5F5F005E5E5E005D5D5D005C5C5C005C5C5C005B5B
          5B005A5A5A0059595900F4E9E400F4E9E400F4E9E400F5EAE500F4E9E400F3E9
          E400F3E9E400F3E9E400F4E9E400F0EAE700ECECEC00E8F5F5001DEFF7001CEC
          F6001BE2F30019CAEC0052D5F0005FCEEE00CEE1F200C7BAB000BB997D00BA97
          7B00B9947900B8927600B7907300B68D7000B58A6E00B4886B00B3856800B182
          6400B07F6100AF7C5E00AE7A5B00AC76570086472B00C0C0C000BEBEBE006565
          650064646400636363006262620061616100606060005F5F5F005E5E5E005D5D
          5D00F3E9E400F4E9E400F4E9E400F4EAE500F3E9E400F3E9E400F3E9E400F3E9
          E400F3E9E400F1ECEA00EFEFEF00DFD5CB00CCF1EF00C0D9F000E9EFF500E8E1
          DB00E3DBD200D3C4B700C9B6A600BD9D8300BC9B8100BB997E00BEA08600BEA0
          8600C0A58B00C1A78E00B49279009A664A00B4876A00B2846700B1816300B07E
          6000AE7B5D00AD78590086482C00C3C3C300C1C1C10067676700676767006666
          6600666666006565650065656500646464006363630062626200F4E9E400F3E9
          E400F4E9E400F5EAE500F4E9E400F4E9E400F4E9E400F3E9E400F3E9E400F3EE
          EB00F2F2F200D9CDC000C2A99000C2A88F00C2A88E00C4AE9500C5AF9700C7B5
          9D00C9B8A100CABBA400CCC0AA00CCC1AA00CBBEA800CABCA500C9BAA200C8B7
          9F00B4957D00884D3100B4896C00B3866900B2836500B0806200AF7D5E00AE7A
          5B0086482C00C6C6C600C4C4C4006B6B6B006B6B6B006A6A6A00696969006868
          680068686800676767006666660066666600F4E9E400F4E9E400F3E9E400F5EA
          E500F3E9E400F3E9E400F4E9E400F3E9E400F4E9E400F4EFED00F5F5F500DCD0
          C400C4AC9400C3AC9300C6B19900CEC5AF00CFC7B100D0C8B200CFC8B200CFC7
          B100CEC5AE00CDC2AC00CCC0A900CBBDA600CABAA300C8B8A000B4967D00884D
          3200B58B6E00B4886B00B3856700B1826400B07F6000AE7B5D0086482C00C9C9
          C900C7C7C700707070006F6F6F006E6E6E006D6D6D006D6D6D006C6C6C006B6B
          6B006A6A6A0069696900F3E9E400F4E9E400F3E9E400F5EAE500F3E9E400F4E9
          E400F4E9E400F3E9E400F4E9E400F3EEEC00F5F5F500D7CABB00C5B09800C5AF
          9700C7B49C00CFC7B100D0C9B400D1CBB500D1CBB500D0C9B300CFC6B000CDC3
          AD00CCC1AA00CBBEA700CABBA400C9B8A100AA876E00884E3200B68D7000B58A
          6D00B3866900B2836500B0806200AF7D5E0086492D00CCCCCC00CBCBCB008B8B
          8B007575750074747400737373007272720071717100707070006F6F6F006E6E
          6E00F3E9E400F3E9E400F4E9E400F5EAE500F3E9E400F4E9E400F4E9E400F3E9
          E400F4E9E400F3EEEB00F3F3F300D2C4B200C7B39B00C6B39B00C9B9A200CFC7
          B200D0CAB500D1CDB800C1B09A00BAA58E00A9887000B5998100A57D6400B08D
          7300A0725700AA806500A97E6200B8917500B78E7200B58B6E00B4886B00B285
          6700B1816300B07E600086492D00CFCFCF00CECECE00919191007B7B7B007A7A
          7A00797979007878780077777700757575007474740073737300F3E9E400F3E9
          E400F4E9E400F5EAE500F4E9E400F4E9E400F3E9E400F3E9E400F4E9E400F2ED
          EA00F2F2F200D3C6B400C8B79F00C8B69E00C5B09800BC9D8300AC826600B493
          7900B2907700C3AB9200C2A88F00C1A58C00BFA28800BE9F8500BD9C8200BB99
          7E00BA967B00B9937700B7907300B68C7000B4896C00B3866800B1826500B07F
          610087492D00CDC9C700D1D1D100979797008282820081818100808080007E7E
          7E007D7D7D007C7C7C007A7A7A0079797900F4E9E400F4E9E400F3E9E400F4EA
          E500F3E9E400F4E9E400F3E9E400F4E9E400F4E9E400F2ECEA00F0F0F000D3C8
          B700CABBA300C9BAA200C9B8A100C8B79F00C7B59D00C6B29A00C5B09700C4AD
          9400C3AA9100C1A78E00C0A48A00BFA18700BD9E8300BC9B8000BA977C00B994
          7800B8917500B68E7100B58A6D00B3876900B2836600B0806200874A2E00BEAE
          A700D4D4D4009D9D9D008A8A8A00888888008787870085858500848484008383
          83008181810080808000F3E9E400F4E9E400F4E9E400F5EAE500F3E9E400F3E9
          E400F4E9E400F3E9E400F4E9E400F0EBE900EFEFEF00D4CAB900CBBEA700CBBD
          A600CABCA500C9BAA200C8B7A000C7B59D00C6B29A00C5AF9600C3AC9300C2A9
          9000C1A68C00BFA28800BE9F8500BC9C8100BB997D00BA957A00B8927600B78F
          7200B58B6E00B4886A00B2846700B1816300945B3F00C1B0AA00D7D7D700A3A3
          A30092929200909090008F8F8F008D8D8D008C8C8C008A8A8A00898989008787
          8700F4E9E400F3E9E400F4E9E400F5EAE500F3E9E400F4E9E400F4E9E400F3E9
          E400F3E9E400EFEAE800EDEDED00D5CDBC00CDC1AB00CCC0AA00CBBFA800CABC
          A500C9BAA200C8B79F00C7B49C00C6B19800C4AE9500C3AA9100C1A78D00C0A4
          8A00BEA08600BD9D8200BB9A7E00BA967B00B9937700B78F7300B68C6F00B488
          6B00B3856800B1826400955B3F00C3B3AC00DADADA00A8A8A800979797009696
          960095959500949494009393930092929200919191008F8F8F00F4E9E400F4E9
          E400F4E9E400F5EAE500F3E9E400F4E9E400F3E9E400F3E9E400F3E9E400EFEA
          E700ECECEC00D6CFBE00CEC5AF00CEC4AD00CDC1AB00CBBFA800CABCA500C9B9
          A100C7B59E00C6B29A00C5AF9600C3AB9200C2A88F00C0A58B00BFA18700BD9E
          8300BC9A7F00BA977C00B9937800B7907400B68D7000B4896C00B3866800B182
          640083462A00D2CAC700DDDDDD00B1B1B100A0A0A0009E9E9E009C9C9C009A9A
          9A0098989800979797009696960095959500F4E9E400F4E9E400F3E9E400F4EA
          E500F3E9E400F3E9E400F3E9E400F3E9E400F3E9E400EEE9E600EAEAEA00D6D1
          C100D0C8B200CFC6B000CEC4AD00CCC0AA00CBBDA600C9BAA200C8B69F00C6B3
          9B00C5B09700C4AC9300C2A98F00C1A58C00BFA28800BE9E8400BC9B8000BB97
          7C00B9947800B8907400B68D7100B5896D00B386690099624500AF8E8100E2E2
          E200E0E0E000C0C0C000B2B2B200AEAEAE00AAAAAA00A7A7A700A4A4A400A1A1
          A1009E9E9E009C9C9C00F4E9E400F3E9E400F3E9E400F4EAE500F3E9E400F3E9
          E400F4E9E400F3E9E400F3E9E400EDE8E600E8E8E800DAD7C900D1CBB600D0C8
          B300CEC5AF00CDC1AB00CBBEA700CABBA300C8B7A000C7B49C00C5B09800C4AD
          9400C2A99000B28C7000B1896D00B0866A00BD9A8400BB988300B8958000BE9E
          8B00C3A99B00C2A79900BA9C8E00D4C8C300E7E7E700E5E5E500E4E4E400D5D5
          D500CCCCCC00C6C6C600C1C1C100BCBCBC00B7B7B700B3B3B300AFAFAF00ABAB
          AB00F3E9E400F4E9E400F4E9E400F5EAE500F3E9E400F3E9E400F3E9E400F3E9
          E400F4E9E400EDE7E500E7E7E700E8E8E800DEDCD200CEC3B400C0AB9900CDC0
          B100C7B3A700C7B3A600C7B3A600DCD1CA00D4C2BB00D5C3BC00DDCFCA00F2F2
          F200F3F3F300F4F4F400F5F5F500F6F6F600F5F5F500F3F3F300F1F1F100F0F0
          F000EEEEEE00ECECEC00EAEAEA00E8E8E800E9E9E900F0F0F000ECECEC00E7E7
          E700E0E0E000D9D9D900D3D3D300CDCDCD00C8C8C800C2C2C200F3E9E400F4E9
          E400F4E9E400F5EAE500F3E9E400F3E9E400F3E9E400F3E9E400F4E9E400EBE6
          E300E5E5E500E6E6E600E7E7E700E8E8E800E9E9E900EAEAEA00EBEBEB00EBEB
          EB00ECECEC00EDEDED00EEEEEE00EFEFEF00F0F0F000F1F1F100F2F2F200F2F2
          F200F3F3F300F4F4F400F5F5F500F5F5F500F4F4F400F3F3F300F1F1F100EEEE
          EE00EDEDED00ECECEC00F0F0F000F1F1F100F2F2F200F3F3F300F4F4F400F3F3
          F300F1F1F100EDEDED00E8E8E800E2E2E200F3E9E400F4E9E400F4E9E400F5EA
          E500F3E9E400F3E9E400F3E9E400F3E9E400F3E9E400F0E7E300E1E0E000E3E3
          E300E5E5E500E6E6E600E7E7E700DCDCDC00DDDDDD00DDDDDD00DEDEDE00D9D9
          D900D4D4D400D7D7D700D9D9D900D9D9D900D6D6D600D9D9D900DCDCDC00DFDF
          DF00DFDFDF00E1E1E100E3E3E300E5E5E500E7E7E700E8E8E800E9E9E900EAEA
          EA00EBEBEB00ECECEC00EDEDED00EEEEEE00EFEFEF00F0F0F000F1F1F100F2F2
          F200F3F2F200F4EDEA00F4E9E400F4E9E400F3E9E400F5EAE500F3E9E400F3E9
          E400F4E9E400F3E9E400F3E9E400F3E9E400F3E9E400E4DCD900E5DCD900DCD6
          D300D5D0CE00D5D0CE00C7C4C300C7C4C300BFBEBD00B8B8B800B8B8B800B8B8
          B800B9B9B900BBBBBB00BDBDBD00C0C0C000C3C3C300D1CFCE00D6D4D200D9D7
          D500DCD9D800DEDCDB00E2DFDE00EAE5E300EBE6E400EDE7E500EDE8E500EDE8
          E600EFE9E500F1E9E500F2E9E600F2EAE600F2EAE600F2EAE600F4E9E400F3E9
          E400F4E9E400F4E9E400F3E9E400F4EAE500F4EAE500F4EAE500F4EAE500F4EA
          E500F4EAE500F4EAE500F5EAE500F4EAE500F4EAE500F4EAE500F4EAE500F4EA
          E500F4EAE500F5EAE500F4EAE500F4EAE500F4EAE500F5EAE500F5EAE500F4EA
          E500F5EAE500F5EAE500F5EAE500F5EAE500F5EAE500F5EAE500F5EAE500F4EA
          E500F4EAE500F5EAE500F5EAE500F4EAE500F5EAE500F5EAE500F4EAE500F4EA
          E500F4EAE500F4EAE500F4EAE500F4EAE500F5EAE500F4EAE500F5EAE500F4EA
          E500F5EAE500F5EAE500F4EAE500F4EAE500F4EAE500F4EAE500F4EAE500F4EA
          E500F4EAE500F4EAE500F4EAE500F4EAE500F4EAE500F4EAE500F4EAE500F5EA
          E500F4EAE500F4EAE500F4EAE500F4EAE500F5EAE500F4EAE500F4EAE500F5EA
          E500F4EAE500F4EAE500F5EAE500F4EAE500F4EAE500F4EAE500F4EAE500F5EA
          E500F4EAE500F5EAE500F5EAE500F4EAE500F4EAE500F4EAE500F5EAE500F5EA
          E500F5EAE500F4EAE500F5EAE500F4EAE500F5EAE500F5EAE500F4EAE500F4EA
          E500}
        Stretch = True
      end
      object NxButton6: TNxButton
        Left = 23
        Top = 279
        Width = 84
        Height = 31
        Caption = 'Visualizza...'
        Down = False
        Enabled = False
        TabOrder = 0
        TabStop = True
        Transparent = False
        OnClick = NxButton6Click
      end
    end
  end
end
