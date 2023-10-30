//---------------------------------------------------------------------------

#ifndef TCoppiaH
#define TCoppiaH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ANALOGMETERHQLib_OCX.h"
#include <DBOleCtl.hpp>
#include <OleCtrls.hpp>
//---------------------------------------------------------------------------
class TCoppia : public TForm
{
__published:	// IDE-managed Components
        TAnalogMeterHQ *AnalogMeterHQ1;
        TStaticText *StaticText1;
        void __fastcall FormResize(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
private:	// User declarations
public:		// User declarations
        __fastcall TCoppia(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TCoppia *Coppia;
//---------------------------------------------------------------------------
#endif
