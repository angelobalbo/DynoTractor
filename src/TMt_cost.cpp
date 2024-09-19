//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TMt_cost.h"
#include "TMain.h"
#ifdef _DYNOENGINE
#include "TDebugMode.h"
#endif
#include "CDyn3.h"
#include "Structs.h"
#include<fstream.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMt_cost *Mt_cost;

    extern short sIndice;
    extern unsigned short Ki_prop,Ki_integ;
    extern unsigned short N_cost;
    extern float Val_C_cost,fK_convPes,fK_convC;
    extern float Valore_Cop;
    extern float ValCopVis;
    extern bool OK_N_cost,IO_ref;
    extern unsigned int selPto;
    extern float Rap_tot1;
    //extern float fFact;
    extern float Rend_PTO;
    extern int tipoRapp;
//---------------------------------------------------------------------------
__fastcall TMt_cost::TMt_cost(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TMt_cost::SpeedButton1Click(TObject *Sender)
{
    // calcolo incremento e invio del valore della coppia costante
    // aggiorna nuovo valore
    ValCopVis += Val_C_cost;
    Label2->Caption=FormatFloat("0.#",ValCopVis*fK_convC);
    /// Calcola valore della coppia richiesta al freno
    if(selPto == 0)
      Valore_Cop = (ValCopVis*Rap_tot1/*/fFact*/)*Rend_PTO;
    else
      Valore_Cop = ValCopVis/*/fFact*/;
    IO_ref=true;
}
//---------------------------------------------------------------------------
void __fastcall TMt_cost::BitBtn1Click(TObject *Sender)
{

  OK_N_cost=false;
  //g_brakeControl.CmdStop();
  Main->EndTest();
  Main->SpeedButton21->Enabled=true;
  if(tipoRapp==RAP_RPMFIX)
    Main->SpeedButton1->Enabled=true;
  Main->SpeedButton2->Enabled=true;
  Close();
  //Main->Brake_control(0,0,1); // disattiva il freno
}
//---------------------------------------------------------------------------

void __fastcall TMt_cost::SpeedButton2Click(TObject *Sender)
{
  // calcolo decremento e invio del valore della coppia costante
  // aggiorna nuovo valore
  if(ValCopVis-Val_C_cost>0)
    ValCopVis-=Val_C_cost;
  else
    ValCopVis=0;
  if(ValCopVis==0)
    g_brakeControl.CmdStop();
  Label2->Caption = FormatFloat("0.#",ValCopVis*fK_convC);
  if(selPto == 0)
    Valore_Cop = (ValCopVis * Rap_tot1/* / fFact*/)* Rend_PTO;
  else
   Valore_Cop = ValCopVis/*/fFact*/;

  IO_ref=true;
}
//---------------------------------------------------------------------------


void __fastcall TMt_cost::FormShow(TObject *Sender)
{
  Valore_Cop = 0;
  ValCopVis = 0;
  Label2->Caption=FormatFloat("0.#",Valore_Cop*fK_convC);
  // attiva invio coppia costante al freno.....
  OK_N_cost=true;
}
//---------------------------------------------------------------------------
void __fastcall TMt_cost::FormClose(TObject *Sender, TCloseAction &Action)
{
  OK_N_cost=false;
  IO_ref=false;
  g_brakeControl.CmdStop();
  Main->SpeedButton21->Enabled=true;
  if(tipoRapp==RAP_RPMFIX)
    Main->SpeedButton1->Enabled=true;
  Main->SpeedButton2->Enabled=true;
#ifdef _DYNOENGINE
  frmDebugMode->btnWritePid->Enabled=false;
#endif
}
//---------------------------------------------------------------------------
void __fastcall TMt_cost::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  if(Key=='k')
    SpeedButton1Click(this);
  else if(Key=='m')
    SpeedButton2Click(this);
}
//---------------------------------------------------------------------------

