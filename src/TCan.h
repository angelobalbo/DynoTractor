//---------------------------------------------------------------------------

#ifndef TCanH
#define TCanH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Mask.hpp>
#include <Buttons.hpp>
#include <Graphics.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <Chart.hpp>
#include <Series.hpp>
#include <TeEngine.hpp>
#include <TeeProcs.hpp>
#include "TeeTools.hpp"
//---------------------------------------------------------------------------
class TfrmCan : public TForm
{
__published:	// IDE-managed Components
  TTimer *tmrRate;
  TGroupBox *gbCanBus;
  TLabel *Label24;
  TLabel *Label26;
  TMaskEdit *meCanRate;
  TMaskEdit *meCanId;
  TBitBtn *btnStart;
  TComboBox *cbCanRate;
  TLabel *Label1;
  TBitBtn *btnStop;
  TBitBtn *BitBtn3;
  TImage *imgStatus;
  TImage *imgStatusStop;
  TImage *imgStatusErr;
  TImage *imgStatusStart;
  TSpeedButton *sbCmd;
  TTimer *tmrCmd;
  TOpenDialog *OpenDialog1;
  TSpeedButton *SpeedButton1;
  TChart *chart;
  TFastLineSeries *srsRpm;
  TFastLineSeries *srsTrq;
  TColorBandTool *ChartTool1;
  TFastLineSeries *srsRpmRt;
  TFastLineSeries *srsTrqRt;
  TLabel *Label2;
  TLabel *lblRes;
  TMaskEdit *MaskEdit1;
  TLabel *Label3;
  TLabel *Label4;
  TMaskEdit *MaskEdit2;
  TLabel *Label5;
  TLabel *Label6;
  TEdit *edRepeat;
  TLabel *Label7;
  TSpeedButton *butSaltaStep;
  void __fastcall meCanIdChange(TObject *Sender);
  void __fastcall meCanRateChange(TObject *Sender);
  void __fastcall BitBtn1Click(TObject *Sender);
  void __fastcall btnStartClick(TObject *Sender);
  void __fastcall tmrRateTimer(TObject *Sender);
  void __fastcall btnStopClick(TObject *Sender);
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
  void __fastcall FormShow(TObject *Sender);
  void __fastcall sbCmdClick(TObject *Sender);
  void __fastcall tmrCmdTimer(TObject *Sender);
  void __fastcall SpeedButton1Click(TObject *Sender);
  void __fastcall MaskEdit1Change(TObject *Sender);
  void __fastcall MaskEdit2Change(TObject *Sender);
  void __fastcall edRepeatChange(TObject *Sender);
  void __fastcall butSaltaStepClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TfrmCan(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmCan *frmCan;
extern long tmrInterval;
extern long trqInterval;
//---------------------------------------------------------------------------
#endif
