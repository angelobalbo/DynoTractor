//---------------------------------------------------------------------------

#ifndef TAccH
#define TAccH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "CGAUGES.h"
//---------------------------------------------------------------------------
class TAcc : public TForm
{
__published:	// IDE-managed Components
        TCGauge *CGauge1;
        void __fastcall FormActivate(TObject *Sender);
        void __fastcall FormResize(TObject *Sender);
        void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TAcc(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TAcc *Acc;
//---------------------------------------------------------------------------
#endif
