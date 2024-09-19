//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TSplash.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSplash *Splash;
//---------------------------------------------------------------------------
__fastcall TSplash::TSplash(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TSplash::Timer1Timer(TObject *Sender)
{
 Timer1->Enabled=false;
 Close();

}
//---------------------------------------------------------------------------
void __fastcall TSplash::FormShow(TObject *Sender)
{
#ifdef _DYNOENGINE
  Image2->Visible=true;
#else
  Image1->Visible=true;
#endif
 Timer1->Enabled=true;

}
//---------------------------------------------------------------------------



