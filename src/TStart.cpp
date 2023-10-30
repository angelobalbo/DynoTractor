//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "stdio.h"                  
#include "TStart.h"
#include "TMain.h"
#include "TImpostazioni.h"
#include "Structs.h"
#include "CDyn3.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
#define KReg 65
#define KVel 5
#define Ac_targ 0.15
TStart *Start;

  extern struct DYNO_SETS dynoPrm;
  extern float  C_Time,
              Rap_tot1,
              velocita,
              OLD_REG,
              Val_ACC,
              fTermo_Oil[500],
              fTermo_K[500],
              fPres_MAP[500];


  extern int
              Vel_iniz,
              Tempo_dec,
              Reg_iniz_acc,
              Reg_fine_dec,
              tipoRapp,
              Tipo_test,
              Reg_iniz_dec,
              Reg_fine_dec,
              Tempo_acc,
              regime2;

extern short  Unit_Vel,sNum_test;

extern bool Start_acq,OK_TEST_AUTO,Friz;

extern String Str0,Text86;

extern long lRpm;
extern char Buffer[100];

//---------------------------------------------------------------------------
__fastcall TStart::TStart(TComponent* Owner)
        : TForm(Owner)
{

}
//---------------------------------------------------------------------------
void __fastcall TStart::BitBtn2Click(TObject *Sender)
{
 Main->Series105->Clear();
 Main->Series106->Clear();
 Main->Series107->Clear();
 Main->StaticText2->Visible=false;
 Start_acq=false; // Stop ok inizio acquisizione dati di test
 Main->clearDat();
 Main->EndTest();
 Close();
}
//---------------------------------------------------------------------------


extern float RPM_IST[7500],C_IST[7500];
extern int cntrIst ;
void __fastcall TStart::BitBtn1Click(TObject *Sender)
{
  Main->Panel33->Visible=true;
  Main->StaticText2->Visible=true;
  Main->Series105->Clear();
  Main->Series106->Clear();
  Main->Series107->Clear();
  OLD_REG=0;
  Friz=true;

  // richiama funzione azzeramento di tutte le variabili
  Main->Azzera_var();
  cntrIst = 0;
  Start_acq=true; // ok per inizio acquisizione dati di test
  Main->createDat();
  memset(C_IST,0,sizeof(C_IST));
  memset(RPM_IST,0,sizeof(RPM_IST));
  memset(fTermo_Oil,0,sizeof(fTermo_Oil));
  memset(fTermo_K,0,sizeof(fTermo_K));
  memset(fPres_MAP,0,sizeof(fPres_MAP));
  if(!(Main->getStatusDat()==START_SIMUL_DAT || Main->getStatusDat()==RUN_SIMUL_DAT)) g_brakeControl.CmdStart(1); // Start prova decelerativa

  Timer2->Enabled=false;
  BitBtn1->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall TStart::FormShow(TObject *Sender)
{
  unsigned int uiEncoderStart,uiEncoderStop,uiAccel;
  Label5->Caption=Text86;
  KeyPreview = true; // attiva lettura da tastiera
  Caption=Impostazioni->ComboBox1->Items->Strings[4];
  Label4->Caption="";
  // comando d'impostazione...
  uiEncoderStart=60000000/((Reg_iniz_dec/Rap_tot1)*dynoPrm.bench.nImpulsiEncoder);
  uiEncoderStop=60000000/((Reg_fine_dec/Rap_tot1)*dynoPrm.bench.nImpulsiEncoder);
  uiAccel=((Reg_iniz_dec-Reg_fine_dec)/Tempo_dec/Rap_tot1)*C_Time;
  if(!(Main->getStatusDat()==START_SIMUL_DAT || Main->getStatusDat()==RUN_SIMUL_DAT)) g_brakeControl.CmdSetup(uiAccel,uiEncoderStart,uiEncoderStop,CDyn3::EFrontAxle);
  Label3->Caption=Str0;
  Label2->Caption=Reg_iniz_dec;

  BitBtn1->Enabled=true;
  BitBtn1->SetFocus();

  Main->SpeedButton8->Enabled=false;
  Main->SpeedButton15->Enabled=false;
  Main->SpeedButton17->Enabled=false;
  Main->SpeedButton18->Enabled=false;
  Main->SpeedButton4->Enabled=false;
  Main->SpeedButton1->Enabled=false;
  Main->SpeedButton2->Enabled=false;
  Main->SpeedButton21->Enabled=false;

  // avvia il timer di controllo inizio test automatico
 if(OK_TEST_AUTO)
 {
        Timer2->Enabled=true;
        BitBtn1->Enabled=false;
 }
 Main->pressStartDat();
}
//---------------------------------------------------------------------------

void __fastcall TStart::FormClose(TObject *Sender, TCloseAction &Action)
{


   BitBtn1->Enabled=true;// riattiva tasto inizio campionamento

  Label5->Visible=true;

  Timer1->Enabled=false;


Main->SpeedButton8->Enabled=true;
Main->SpeedButton15->Enabled=true;
Main->SpeedButton17->Enabled=true;
Main->SpeedButton18->Enabled=true;
Main->SpeedButton4->Enabled=true;
Main->SpeedButton21->Enabled=true;
  if(tipoRapp==RAP_RPMFIX)
    Main->SpeedButton1->Enabled=true;
  Main->SpeedButton2->Enabled=true;
}
//---------------------------------------------------------------------------




void __fastcall TStart::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
 Main->Remote(Key);
}
//---------------------------------------------------------------------------



void __fastcall TStart::Timer1Timer(TObject *Sender)
{


 //   Label5->Visible=false;

 if(Timer1->Tag==0)
     {
     Timer1->Tag=1;Label4->Visible=false;
     }


 else
  {
   Timer1->Tag=0;
   Label4->Visible=true;
     }


}
//---------------------------------------------------------------------------

void __fastcall TStart::Timer2Timer(TObject *Sender)
{

  // avvio automatico del test...
  switch(Tipo_test)
  {
    case 0:   // test su base giri
      if(  Val_ACC > -Ac_targ  && Val_ACC <= Ac_targ  && lRpm*Rap_tot1 > Reg_iniz_acc-KReg && lRpm*Rap_tot1 < Reg_iniz_acc+KReg )
      {
        BitBtn1Click(this);
        Timer2->Enabled=false;
      }
    break;
    case 1:  // test su base velocità  ant/post/4wd
      if( Val_ACC > -Ac_targ && Val_ACC <= Ac_targ && velocita > Vel_iniz-KVel && velocita < Vel_iniz+KVel )
        BitBtn1Click(this);Timer2->Enabled=false;
      break;
  } // switch

}
//---------------------------------------------------------------------------
