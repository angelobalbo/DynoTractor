//---------------------------------------------------------------------------

#ifndef TPilot_FrenoH
#define TPilot_FrenoH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "VrControls.hpp"
#include "VrWheel.hpp"
#include "VrProgressBar.hpp"
#include "VrTrackBar.hpp"
#include <Buttons.hpp>
#include "ANALOGMETERHQLib_OCX.h"
#include <DBOleCtl.hpp>
#include <OleCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TPilot_Freno : public TForm
{
__published:	// IDE-managed Components
        TVrTrackBar *VrTrackBar1;
        TAnalogMeterHQ *AnalogMeterHQ1;
        TPanel *Panel1;
        TBitBtn *BitBtn2;
        TBitBtn *BitBtn1;
  TBitBtn *PrintStep;
  TBitBtn *ResetTabStep;
        void __fastcall VrTrackBar1Change(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall BitBtn2Click(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall BitBtn1Click(TObject *Sender);
        void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
  void __fastcall PrintStepClick(TObject *Sender);
  void __fastcall ResetTabStepClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TPilot_Freno(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TPilot_Freno *Pilot_Freno;
//---------------------------------------------------------------------------
#endif
