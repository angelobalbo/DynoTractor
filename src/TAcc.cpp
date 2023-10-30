//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TAcc.h"
#include "TMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CGAUGES"
#pragma resource "*.dfm"
TAcc *Acc;


//---------------------------------------------------------------------------
__fastcall TAcc::TAcc(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TAcc::FormActivate(TObject *Sender)
{
//  CGauge1->MinValue=Tens_Ad_fut_min/0.0588;
//  CGauge1->MaxValue=Tens_Ad_fut_max/0.0588;

}
//---------------------------------------------------------------------------
void __fastcall TAcc::FormResize(TObject *Sender)
{
  CGauge1->Font->Size=int(4*Acc->ClientWidth/100);
}
//---------------------------------------------------------------------------

void __fastcall TAcc::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  Main->Remote(Key);
}
//---------------------------------------------------------------------------

void __fastcall TAcc::FormShow(TObject *Sender)
{
 KeyPreview = true; // attiva lettura da tastiera
}
//---------------------------------------------------------------------------

