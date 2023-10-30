//---------------------------------------------------------------------------

#ifndef TStartH
#define TStartH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include "VrControls.hpp"
#include "VrDigit.hpp"
#include "VrScrollText.hpp"
#include "VrPieGraph.hpp"
#include "pies.h"
#include "CGAUGES.h"
//---------------------------------------------------------------------------
class TStart : public TForm
{
__published:	// IDE-managed Components
        TBitBtn *BitBtn1;
        TBitBtn *BitBtn2;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TLabel *Label4;
        TLabel *Label5;
        TTimer *Timer1;
        TTimer *Timer2;
        void __fastcall BitBtn2Click(TObject *Sender);
        void __fastcall BitBtn1Click(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall Timer2Timer(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TStart(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TStart *Start;
//---------------------------------------------------------------------------
#endif
