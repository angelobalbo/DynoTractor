//---------------------------------------------------------------------------

#ifndef TRapportoH
#define TRapportoH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include "CGAUGES.h"
//---------------------------------------------------------------------------
class TRapporto : public TForm
{
__published:	// IDE-managed Components
        TBitBtn *BitBtn1;
        TBitBtn *BitBtn2;
        TImage *Image1;
        TLabel *Label1;
        TLabel *Label2;
        TBevel *Bevel1;
        TTimer *Timer1;
        TLabel *Label3;
        void __fastcall BitBtn1Click(TObject *Sender);
        void __fastcall BitBtn2Click(TObject *Sender);
        void __fastcall FormActivate(TObject *Sender);
        void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
        __fastcall TRapporto(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TRapporto *Rapporto;
//---------------------------------------------------------------------------
#endif
