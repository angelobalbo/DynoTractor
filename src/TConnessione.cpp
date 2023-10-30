//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TConnessione.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "VrGradient"
#pragma resource "*.dfm"
TConnessione *Connessione;
//---------------------------------------------------------------------------
__fastcall TConnessione::TConnessione(TComponent* Owner)
        : TForm(Owner)
{


}
//---------------------------------------------------------------------------
void __fastcall TConnessione::Timer1Timer(TObject *Sender)
{
switch(Timer1->Tag){
            case 0:
             Image1->Visible=false;
             Image2->Visible=true;
             Timer1->Tag++;
             break;
            case 1:
             Image1->Visible=true;
             Image2->Visible=false;
             Timer1->Tag=0;
            break;
            }
}
//---------------------------------------------------------------------------

void __fastcall TConnessione::FormShow(TObject *Sender)
{
 Timer1->Enabled=true;        
}
//---------------------------------------------------------------------------

void __fastcall TConnessione::FormClose(TObject *Sender,
      TCloseAction &Action)
{
 Timer1->Enabled=false;         
}
//---------------------------------------------------------------------------

