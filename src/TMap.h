//---------------------------------------------------------------------------

#ifndef TMapH
#define TMapH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ANALOGMETERHQLib_OCX.h"
#include <DBOleCtl.hpp>
#include <OleCtrls.hpp>
//---------------------------------------------------------------------------
class TMap : public TForm
{
__published:	// IDE-managed Components
        TAnalogMeterHQ *AnalogMeterHQ1;
        TStaticText *StaticText1;
        void __fastcall FormResize(TObject *Sender);
        void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TMap(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMap *Map;
//---------------------------------------------------------------------------
#endif
