//---------------------------------------------------------------------------

#ifndef TGiri_MotH
#define TGiri_MotH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include <DBOleCtl.hpp>
#include <OleCtrls.hpp>
#include "ANALOGMETERHQLib_OCX.h"
//---------------------------------------------------------------------------
class TGiri_Mot : public TForm
{
__published:	// IDE-managed Components
        TAnalogMeterHQ *AnalogMeterHQ1;
        TStaticText *StaticText1;
        void __fastcall FormActivate(TObject *Sender);
        void __fastcall FormResize(TObject *Sender);
        void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TGiri_Mot(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TGiri_Mot *Giri_Mot;
//---------------------------------------------------------------------------
#endif
