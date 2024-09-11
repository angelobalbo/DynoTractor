object frmDebugMode: TfrmDebugMode
  Left = 868
  Top = 51
  Width = 709
  Height = 472
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
  PixelsPerInch = 96
  TextHeight = 13
  object chDebug: TChart
    Left = 0
    Top = 0
    Width = 701
    Height = 438
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
      701
      438)
    PrintMargins = (
      0
      0
      0
      0)
    object Label1: TLabel
      Left = 40
      Top = 400
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
      Top = 400
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
      Left = 664
      Top = 399
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
      Left = 487
      Top = 16
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
      Left = 16
      Top = 8
      Width = 97
      Height = 17
      Caption = 'REFRESH'
      Checked = True
      State = cbChecked
      TabOrder = 1
    end
    object Button2: TButton
      Left = 375
      Top = 15
      Width = 105
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
      Left = 584
      Top = 12
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
      Top = 420
      Width = 699
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
end
