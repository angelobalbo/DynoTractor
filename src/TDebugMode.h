//---------------------------------------------------------------------------

#ifndef TDebugModeH
#define TDebugModeH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Chart.hpp>
#include <ExtCtrls.hpp>
#include <Series.hpp>
#include <TeEngine.hpp>
#include <TeeProcs.hpp>
#include <Buttons.hpp>
#include "TeePageNumTool.hpp"
#include "TeeTools.hpp"
//---------------------------------------------------------------------------
class TfrmDebugMode : public TForm
{
__published:	// IDE-managed Components
  TChart *chDebug;
  TFastLineSeries *srsP;
  TFastLineSeries *srsI;
  TFastLineSeries *srsCurrent;
  TFastLineSeries *srsTarget;
  TFastLineSeries *srsPwm;
  TButton *Button1;
  TCheckBox *cbRefresh;
  TButton *Button2;
  TBitBtn *BitBtn1;
  TPageNumTool *ChartTool1;
  TScrollBar *sbMaxPoints;
  TLabel *Label1;
  TLabel *Label2;
  TLabel *Label3;
  void __fastcall Button1Click(TObject *Sender);
  void __fastcall Button2Click(TObject *Sender);
  void __fastcall BitBtn1Click(TObject *Sender);
  void __fastcall sbMaxPointsChange(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TfrmDebugMode(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmDebugMode *frmDebugMode;
//---------------------------------------------------------------------------
#endif
