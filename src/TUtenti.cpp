//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TMain.h"
#include "CDyn3.h"
#include "CDyra.h"
#include "CUtils.h"
#include "new.h"
#include "TGiri_Mot.h"
#include "TMap.h"
#include "TCoppia.h"
#include "TPotenza.h"
#include "TTermo.h"
#include "TAcc.h"
#include "TUtenti.h"
#include <IniFiles.hpp>
#include "TOil_Temp.h"
//---------------------------------------------------------------------
#pragma resource "*.dfm"
TUtenti *Utenti;



  bool          Debugs_Status;
  extern short  Prof_strum,
                Vis_Air_Fuel;
  extern bool bFileTrace;
  extern bool bPidControl;  
  extern String Profilo,Str10,Text64,Text65,Text66,Text67;
  extern char Buffer[100];
  extern unsigned short sPidKp; // 0..FFFF x 10
  extern unsigned short sPidKi; // 0..FFFF
  extern unsigned short sPidKd; // 0..FFFF x 10000



  extern bool   // visualizza grandezze su grafici 0-> No  -  1-> Si

          Vis_P_M ,  // potenza motore
          Vis_P_R ,  // potenza ruota
          Vis_C_M ,  // coppia motore
          Vis_C_R ,  // coppia ruota
          Vis_P_D ,  // potenza dissipata
          Vis_Pmap , // pressione aspirazione
          Vis_R_AB , // rapporto aria/benzina
          Vis_T_K ,  // temperatura termocoppia K
          Vis_T_N ,  // Temperatura Ntc Olio
          Vis_GV ,  // Visualizza su 0->giri  1->Velocità


          // stabilisce se visualizza strumento oppure no  0->No 1->Si

          Vis_Rpm ,  // giri motore
          Vis_Pot,  // potenza
          Vis_Cop,  // coppia
         Vis_Pres,  // map
          Vis_TemK,  // temperatura K
          Vis_TemN,  // temperatura Ntc oil
          Vis_AirB,  // Lambda
           Vis_Acc;  // ingresso A/D 0-15Vcc

  
//---------------------------------------------------------------------
__fastcall TUtenti::TUtenti(TComponent* AOwner)
	: TForm(AOwner)
{


}
//---------------------------------------------------------------------


void __fastcall TUtenti::Aggiungiutente1Click(TObject *Sender)
{
  TIniFile *ini;

  MessageBeep(MB_ICONEXCLAMATION);
  if(MessageDlg(Str10,mtConfirmation,TMsgDlgButtons() << mbYes << mbNo,0) == mrYes)
    Main->salva_imp(); // Se SI salva funzione di lettura impostazioni

  AnsiString InputString = InputBox(Text65, Text64, "").LowerCase();
  if(InputString=="maintenance mode")
  {
    bPidControl=false;
    MessageDlg("Trace Dyn3, Dyra activated",mtConfirmation,TMsgDlgButtons() << mbOK,0);
    bFileTrace=true;
    g_dyraControl.ActiveLog(bFileTrace);
    g_brakeControl.ActiveLog(bFileTrace);
    return;
  }
  else if(InputString=="pidcontrol")
  {
    bPidControl=true;
  }
  else
  {
    bPidControl=false;
    ListBox1->Items->Add(InputString);
    // conteggia e predispone la visualizzazione dell'ultimo utente inserito
    for(int i=0; i<ListBox1->Items->Capacity; i++) ListBox1->ItemIndex=i;
    ini = new TIniFile(utils.GetProgramDataName(".ini") );
    for( i=0; i<ListBox1->Items->Capacity; i++)
    {
      ListBox1->ItemIndex=i;
      ini->WriteString( "Profili", ListBox1->ItemIndex, ListBox1->Items->Strings[i] );
    }
    delete ini;
          Vis_P_M = 1;
          Vis_P_R = 0;
          Vis_C_M = 1;
          Vis_C_R = 0;
          Vis_P_D = 0;
          Vis_Pmap = 0;
          Vis_R_AB = 0;
          Vis_T_K = 0;

          Vis_T_N = 0;
          Vis_GV  = 0;

            Vis_Rpm=1;  // giri motore
          Vis_Pot=0;  // potenza
          Vis_Cop=0;  // coppia
         Vis_Pres=0;  // map
          Vis_TemK=0;  // temperatura K
          Vis_TemN=0;  // temperatura Ntc oil
          Vis_AirB=0;  // Lambda
           Vis_Acc=0;  // ingresso A/D 0-15Vcc
   Main->salva_imp(); // Se SI salva funzione di lettura impostazioni
   OKBtn->Enabled=true;
  }


  if(bPidControl)
  {
    Main->Panel33->Visible=true;  
    Main->tbProp->Visible=true;
    Main->tbInte->Visible=true;
    Main->tbDeri->Visible=true;
    Main->lblPID->Visible=true;
    Main->lblP->Visible=true;
    Main->lblI->Visible=true;
    Main->lblD->Visible=true;
    Main->btnPID->Visible=true;
    Main->btnRfr->Visible=true;
    g_brakeControl.CmdReadPID();
    do
      Application->ProcessMessages();
    while(g_brakeControl.IsCmdInProgress());
    g_brakeControl.GetPID(&sPidKp,&sPidKi,&sPidKd);
  }
  else
  {
    Main->tbProp->Visible=false;
    Main->tbProp->Visible=false;
    Main->tbProp->Visible=false;
    Main->lblPID->Visible=false;
    Main->lblP->Visible=false;
    Main->lblI->Visible=false;
    Main->lblD->Visible=false;
    Main->btnPID->Visible=false;
    Main->btnRfr->Visible=false;    
  }





  // Salva su disco nuovo profilo standard con nome scelto da utente...
  Profilo=InputString;

}
//---------------------------------------------------------------------------

void __fastcall TUtenti::Eliminautente1Click(TObject *Sender)
{

if(ListBox1->Items->Capacity > 0 && MessageDlg(Text66, mtWarning, TMsgDlgButtons() << mbYes << mbNo, 0)== mrYes)
 {

  TIniFile *ini;
   ini = new TIniFile(utils.GetProgramDataName(".ini") );
   ini->DeleteKey("Profili",ListBox1->ItemIndex);
   delete ini;

  ListBox1->Items->Delete(ListBox1->ItemIndex);
  for(int i=0; i<ListBox1->Items->Capacity; i++) ListBox1->ItemIndex=0;



 }

}
//---------------------------------------------------------------------------


void __fastcall TUtenti::OKBtnClick(TObject *Sender)
{

 Giri_Mot->Hide();
 Potenza->Hide();
 Coppia->Hide();
 Map->Hide();
 Termo->Hide();
 Acc->Hide();
 Oil_Temp->Hide();

   Profilo=ListBox1->Items->Strings[ListBox1->ItemIndex];//    GetText();



   Main->carica_imp(); // richiama funzione di lettura impostazioni




}
//---------------------------------------------------------------------------










void __fastcall TUtenti::FormShow(TObject *Sender)
{
  Label2->Caption=Text67;
  ListBox1->Clear();

 int Pos=0;    

  TIniFile *ini;
   ini = new TIniFile(utils.GetProgramDataName(".ini") );
   for(int i=0; i<10; i++)
     {
     if(ini->ReadString( "Profili", i, "")!="")
     ListBox1->Items->Add(ini->ReadString( "Profili", i, ""));
     if(ini->ReadString( "Profili", i, "")==Profilo)Pos=i;
     }
    delete ini;

  // visualizza utente in uso...
  if(ListBox1->Items->Capacity==0) ListBox1->Items->Add("Default");

   ListBox1->ItemIndex=Pos;

   OKBtn->Enabled=false;



}
//---------------------------------------------------------------------------

void __fastcall TUtenti::ListBox1Click(TObject *Sender)
{
  OKBtn->Enabled=true;
}
//---------------------------------------------------------------------------



