//---------------------------------------------------------------------------

#ifndef TOil_TempH
#define TOil_TempH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ANALOGMETERHQLib_OCX.h"
#include <DBOleCtl.hpp>
#include <OleCtrls.hpp>
//---------------------------------------------------------------------------
class TOil_Temp : public TForm
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
        __fastcall TOil_Temp(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TOil_Temp *Oil_Temp;
//---------------------------------------------------------------------------
#endif
