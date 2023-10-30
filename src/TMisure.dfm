object Misure: TMisure
  Left = 751
  Top = 163
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Unit'#224' di misura'
  ClientHeight = 240
  ClientWidth = 289
  Color = clBtnFace
  Font.Charset = ANSI_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Tahoma'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poScreenCenter
  OnActivate = FormActivate
  PixelsPerInch = 96
  TextHeight = 14
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 289
    Height = 196
    ActivePage = TabSheet3
    Align = alTop
    MultiLine = True
    TabIndex = 3
    TabOrder = 0
    TabPosition = tpBottom
    object TabSheet1: TTabSheet
      Caption = 'Coppia'
      object ListBox1: TListBox
        Left = 8
        Top = 16
        Width = 265
        Height = 137
        ItemHeight = 14
        TabOrder = 0
      end
    end
    object TabSheet2: TTabSheet
      Caption = 'Potenza'
      ImageIndex = 1
      object ListBox2: TListBox
        Left = 8
        Top = 16
        Width = 265
        Height = 137
        ItemHeight = 14
        TabOrder = 0
      end
    end
    object TabSheet5: TTabSheet
      Caption = 'Temperatura'
      ImageIndex = 4
      object ListBox5: TListBox
        Left = 8
        Top = 16
        Width = 265
        Height = 137
        ItemHeight = 14
        TabOrder = 0
      end
    end
    object TabSheet3: TTabSheet
      Caption = 'Pressione'
      ImageIndex = 4
      object ListBox3: TListBox
        Left = 8
        Top = 16
        Width = 265
        Height = 137
        ItemHeight = 14
        TabOrder = 0
      end
    end
    object TabSheet6: TTabSheet
      Caption = 'Forza/Peso'
      ImageIndex = 6
      object ListBox6: TListBox
        Left = 8
        Top = 16
        Width = 265
        Height = 137
        ItemHeight = 18
        TabOrder = 0
      end
    end
    object TabSheet4: TTabSheet
      Caption = 'Consumi'
      ImageIndex = 5
      object ListBox4: TListBox
        Left = 8
        Top = 16
        Width = 265
        Height = 137
        ItemHeight = 14
        TabOrder = 0
      end
    end
  end
  object BitBtn1: TBitBtn
    Left = 105
    Top = 205
    Width = 75
    Height = 24
    TabOrder = 1
    OnClick = BitBtn1Click
    Kind = bkOK
  end
end
