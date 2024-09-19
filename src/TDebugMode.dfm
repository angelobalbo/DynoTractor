object frmDebugMode: TfrmDebugMode
  Left = 533
  Top = 65
  Width = 732
  Height = 605
  Caption = 'DEBUG MODE'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poMainFormCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object chDebug: TChart
    Left = 0
    Top = 49
    Width = 724
    Height = 522
    AllowPanning = pmNone
    LeftWall.Visible = False
    Legend.Bevel = bvLowered
    Legend.Font.Charset = ANSI_CHARSET
    Legend.Font.Height = -15
    Legend.Font.Name = 'Arial Black'
    Legend.Font.Style = [fsBold]
    Legend.LegendStyle = lsSeries
    Legend.ShapeStyle = fosRoundRectangle
    Legend.TextStyle = ltsPlain
    Legend.TopPos = 25
    Legend.Transparent = True
    Legend.VertSpacing = 25
    MarginBottom = 10
    MarginLeft = 10
    MarginRight = 10
    MarginTop = 10
    PrintProportional = False
    SubTitle.Font.Charset = ANSI_CHARSET
    SubTitle.Font.Height = -15
    Title.Alignment = taLeftJustify
    Title.Font.Charset = ANSI_CHARSET
    Title.Font.Color = clBlack
    Title.Font.Height = -16
    Title.Font.Name = 'Tahoma'
    Title.Font.Style = [fsBold]
    Title.Shadow.Smooth = False
    Title.Text.Strings = (
      'PID CONTROL  PARAMETERS ')
    BottomAxis.Automatic = False
    BottomAxis.AutomaticMinimum = False
    BottomAxis.Grid.Color = clSilver
    CustomAxes = <
      item
        Axis.Color = clBlue
        Axis.Width = 3
        AxisValuesFormat = '#,##0'
        Grid.Color = clSilver
        Grid.SmallDots = True
        Horizontal = False
        OtherSide = False
        StartPosition = 50
        EndPosition = 74
        PositionPercent = -1
        Title.Angle = 90
        Title.Caption = 'Trg / Cur'
        Title.Font.Charset = ANSI_CHARSET
        Title.Font.Height = -13
        Title.Font.Name = 'Arial Black'
        Title.Font.Style = [fsBold]
      end
      item
        Axis.Color = clGray
        Axis.Width = 3
        AxisValuesFormat = '#,#00.#'
        Grid.Color = clSilver
        Grid.SmallDots = True
        Horizontal = False
        OtherSide = False
        StartPosition = 75
        PositionPercent = -1
        Title.Angle = 90
        Title.Caption = 'pwm  %'
        Title.Font.Charset = ANSI_CHARSET
        Title.Font.Height = -13
        Title.Font.Name = 'Arial Black'
        Title.Font.Style = [fsBold]
      end
      item
        Axis.Color = clGreen
        Axis.Width = 3
        Grid.Color = clSilver
        Grid.SmallDots = True
        Horizontal = False
        OtherSide = False
        StartPosition = 25
        EndPosition = 49
        PositionPercent = -1
        Title.Angle = 90
        Title.Caption = 'I term'
        Title.Font.Charset = ANSI_CHARSET
        Title.Font.Height = -13
        Title.Font.Name = 'Arial Black'
        Title.Font.Style = [fsBold]
      end>
    LeftAxis.Axis.Color = clRed
    LeftAxis.Axis.Width = 3
    LeftAxis.Grid.Color = clSilver
    LeftAxis.Grid.SmallDots = True
    LeftAxis.EndPosition = 24
    LeftAxis.PositionPercent = -1
    LeftAxis.TickLength = 3
    LeftAxis.Title.Caption = 'P term'
    LeftAxis.Title.Font.Charset = ANSI_CHARSET
    LeftAxis.Title.Font.Height = -13
    LeftAxis.Title.Font.Name = 'Arial Black'
    LeftAxis.Title.Font.Style = [fsBold]
    RightAxis.Axis.Color = clGreen
    RightAxis.Axis.Width = 4
    RightAxis.StartPosition = 25
    RightAxis.EndPosition = 49
    RightAxis.Title.Caption = 'I term'
    RightAxis.Title.Font.Charset = ANSI_CHARSET
    RightAxis.Title.Font.Height = -13
    RightAxis.Title.Font.Name = 'Arial Black'
    RightAxis.Title.Font.Style = [fsBold]
    View3D = False
    Align = alClient
    BevelOuter = bvLowered
    Color = clWhite
    TabOrder = 0
    DesignSize = (
      724
      522)
    PrintMargins = (
      0
      0
      0
      0)
    object Label1: TLabel
      Left = 40
      Top = 484
      Width = 176
      Height = 16
      Anchors = [akLeft, akBottom]
      Caption = 'Zoom chart (number of points)'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label2: TLabel
      Left = 5
      Top = 484
      Width = 17
      Height = 16
      Anchors = [akLeft, akBottom]
      Caption = '10'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label3: TLabel
      Left = 687
      Top = 483
      Width = 25
      Height = 16
      Alignment = taRightJustify
      Anchors = [akRight, akBottom]
      Caption = '100'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Button1: TButton
      Left = 510
      Top = 8
      Width = 89
      Height = 25
      Anchors = [akTop, akRight]
      Caption = 'CLEAR'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Arial Black'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 0
      OnClick = Button1Click
    end
    object cbRefresh: TCheckBox
      Left = 280
      Top = 11
      Width = 97
      Height = 17
      Caption = 'REFRESH'
      Checked = True
      State = cbChecked
      TabOrder = 1
    end
    object Button2: TButton
      Left = 386
      Top = 7
      Width = 117
      Height = 26
      Anchors = [akTop, akRight]
      Caption = 'STOP DEBUG'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Arial Black'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 2
      OnClick = Button2Click
    end
    object BitBtn1: TBitBtn
      Left = 607
      Top = 4
      Width = 105
      Height = 28
      Anchors = [akTop, akRight]
      TabOrder = 3
      OnClick = BitBtn1Click
      Glyph.Data = {
        F6000000424DF600000000000000760000002800000010000000100000000100
        04000000000080000000C40E0000C40E00001000000000000000000000000000
        800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00300000000000
        00030888888888888880088888888888888000000000000000000F8F8F8F8F8F
        8F8008F8F8F8F8F8F9F00F8F8F8F8F8F8F8000000000000000003330FFFFFFFF
        03333330F0000F0F03333330FFFFFFFF03333330F00F000003333330FFFF0FF0
        33333330F08F0F0333333330FFFF003333333330000003333333}
    end
    object sbMaxPoints: TScrollBar
      Left = 1
      Top = 504
      Width = 722
      Height = 17
      Align = alBottom
      Min = 10
      PageSize = 0
      ParentShowHint = False
      Position = 100
      ShowHint = True
      TabOrder = 4
      OnChange = sbMaxPointsChange
    end
    object btnReadPid: TBitBtn
      Left = 8
      Top = 8
      Width = 89
      Height = 25
      Caption = 'READ PID'
      TabOrder = 5
      OnClick = btnReadPidClick
      Kind = bkRetry
    end
    object btnWritePid: TBitBtn
      Left = 112
      Top = 8
      Width = 89
      Height = 25
      Caption = 'WRITE PID'
      Enabled = False
      TabOrder = 6
      OnClick = btnWritePidClick
      Kind = bkAll
    end
    object srsP: TFastLineSeries
      Marks.Callout.Brush.Color = clBlack
      Marks.Visible = False
      Title = 'P'
      LinePen.Color = clRed
      LinePen.Width = 2
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
    end
    object srsI: TFastLineSeries
      Marks.Callout.Brush.Color = clBlack
      Marks.DrawEvery = 4
      Marks.Visible = False
      Title = 'I'
      VertAxis = aCustomVertAxis
      LinePen.Color = clGreen
      LinePen.Width = 2
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
      CustomVertAxis = 2
    end
    object srsCurrent: TFastLineSeries
      Marks.Callout.Brush.Color = clBlack
      Marks.Visible = False
      SeriesColor = 33023
      Title = 'Current'
      VertAxis = aCustomVertAxis
      LinePen.Color = 33023
      LinePen.Width = 2
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
      CustomVertAxis = 0
    end
    object srsTarget: TFastLineSeries
      Marks.Callout.Brush.Color = clBlack
      Marks.Visible = False
      Title = 'Target'
      VertAxis = aCustomVertAxis
      LinePen.Color = clBlue
      LinePen.Width = 2
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
      CustomVertAxis = 0
    end
    object srsPwm: TFastLineSeries
      Marks.Callout.Brush.Color = clBlack
      Marks.Visible = False
      Title = 'PWM %'
      VertAxis = aCustomVertAxis
      LinePen.Color = clGray
      LinePen.Width = 2
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
      CustomVertAxis = 1
    end
    object ChartTool1: TPageNumTool
      Active = False
      Callout.Brush.Color = clBlack
      Callout.Arrow.Visible = False
      Text = 'Page %d of %d'
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 724
    Height = 49
    Align = alTop
    TabOrder = 1
    object GroupBox1: TGroupBox
      Left = 1
      Top = 1
      Width = 350
      Height = 47
      Align = alLeft
      Caption = 'STEP RPM'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 0
      object lePstep: TLabeledEdit
        Left = 16
        Top = 17
        Width = 100
        Height = 21
        EditLabel.Width = 9
        EditLabel.Height = 13
        EditLabel.Caption = 'P'
        LabelPosition = lpLeft
        LabelSpacing = 3
        TabOrder = 0
      end
      object leIstep: TLabeledEdit
        Left = 136
        Top = 17
        Width = 97
        Height = 21
        EditLabel.Width = 5
        EditLabel.Height = 13
        EditLabel.Caption = 'I'
        LabelPosition = lpLeft
        LabelSpacing = 3
        TabOrder = 1
      end
      object leDstep: TLabeledEdit
        Left = 256
        Top = 17
        Width = 89
        Height = 21
        EditLabel.Width = 10
        EditLabel.Height = 13
        EditLabel.Caption = 'D'
        LabelPosition = lpLeft
        LabelSpacing = 3
        TabOrder = 2
      end
    end
    object GroupBox2: TGroupBox
      Left = 351
      Top = 1
      Width = 372
      Height = 47
      Align = alClient
      Caption = 'CONSTANT TORQUE'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 1
      object lePtrq: TLabeledEdit
        Left = 19
        Top = 17
        Width = 100
        Height = 21
        EditLabel.Width = 9
        EditLabel.Height = 13
        EditLabel.Caption = 'P'
        LabelPosition = lpLeft
        LabelSpacing = 3
        TabOrder = 0
      end
      object leItrq: TLabeledEdit
        Left = 141
        Top = 17
        Width = 97
        Height = 21
        EditLabel.Width = 5
        EditLabel.Height = 13
        EditLabel.Caption = 'I'
        LabelPosition = lpLeft
        LabelSpacing = 3
        TabOrder = 1
      end
      object leDtrq: TLabeledEdit
        Left = 261
        Top = 17
        Width = 89
        Height = 21
        EditLabel.Width = 10
        EditLabel.Height = 13
        EditLabel.Caption = 'D'
        LabelPosition = lpLeft
        LabelSpacing = 3
        TabOrder = 2
      end
    end
  end
end
