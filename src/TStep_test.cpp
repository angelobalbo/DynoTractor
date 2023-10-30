//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TStep_test.h"
#include "TImpostazioni.h"
#include "TMain.h"
#include "CDyn3.h"
#include "Log.h"
#include "Structs.h"
#pragma package(smart_init)
#pragma link "CGAUGES"
#pragma resource "*.dfm"

  TStep_test *Step_test;

  extern bool bFileTrace;
  extern char g_cDyn3Log[MAX_PATH];
  static char sLog[100];
  extern struct DYNO_SETS dynoPrm;
  extern int  Contatore,
              Tipo_test,
              Delta_reg,
              Reg_Stp,
              Tempo_staz,
              rpmStart,
              rpmStop,
              tipoRapp,
              iMaxStepDistance; // 0-> vuoto 1-> Test presente in memoria

  extern short Tstep,num_pass_step;
  extern bool OK_TEST_STEP;
  // Tab step
//  extern unsigned int primaVoltaTabStep;
  extern int tabStepGiri[500];
  extern float tabStepCoppia[500],tabStepPot[500],tabStepCons[500],tabStepLamb[500],tabStepMap[500],tabStepTk[500],
         Val_cop_medio,
       Val_reg_medio,
       Val_map_medio,
       Val_oil_medio,
       Val_k_medio;
  extern float Rap_tot1;
  int Reg_Stp_1 = 0;
  extern bool gbStepTestCancel;
  extern bool bNotifyRTChart;
  extern long lRpm;
  bool bAvvioPressed=false;
  extern String Str0;
 //---------------------------------------------------------------------------
__fastcall TStep_test::TStep_test(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TStep_test::BitBtn3Click(TObject *Sender)
{
  CGauge1->Progress=0;
  CGauge1->Refresh();

  Main->Series105->Clear();
  Main->Series106->Clear();
  Main->Series107->Clear();
  Main->StaticText2->Visible=false;
  BitBtn4->Enabled=true;
  //gbStepTestCancel=true;
  //g_brakeControl.CmdStop();
  Main->EndTest();
  Main->SpeedButton2->Enabled=true;
  bNotifyRTChart=false;
  OK_TEST_STEP=false;
  Tstep=0;
  Val_cop_medio=0;
  Val_reg_medio=0;
  Val_oil_medio=0;
  Val_map_medio=0;
  Val_k_medio=0;
  bAvvioPressed=false;
  
  if(rpmStart > rpmStop && Reg_Stp < rpmStop || Reg_Stp==rpmStart)
  {
      gbStepTestCancel=true;
      Main->clearDat();
  }
  else if(Contatore==0)
  {
    OK_TEST_STEP=false;
    Main->clearDat();
  }
  else if(MessageDlg("Confirm test?",mtConfirmation,TMsgDlgButtons() << mbYes << mbNo,0)==mrYes)
  {
    gbStepTestCancel=true;
  }
  else
  {
    Contatore=0;
    OK_TEST_STEP=false;
    Main->clearDat();
  }


  Close();
}
//---------------------------------------------------------------------------
void __fastcall TStep_test::FormShow(TObject *Sender)
{

  Reg_Stp=rpmStart;
  // attiva lettura da tastiera
  KeyPreview = true;
  Label2->Caption=IntToStr(Reg_Stp)+" "+Str0;
  bAvvioPressed=false;

  // visualizza tipo di test in esecuzione
  gbStepTestCancel=false;
  //Width=CGauge1->Left+CGauge1->Width+BitBtn4->Left;

  switch (Tipo_test)
  {
    case TEST_STEP_TIMED:
      Caption=Impostazioni->ComboBox1->Items->Strings[0];
      Label4->Visible=true;
      BitBtn1->Visible=false;
      BitBtn2->Visible=false;
      BitBtn3->Enabled=true;
      BitBtn4->Visible=true;
      BitBtn3->Left=BitBtn4->Left+BitBtn4->Width+20;

      Label4->Visible=true;
      CGauge1->Visible=true;
      //PrintStep->Visible=true;
      //ResetTabStep->Visible=true;

      //Width=CGauge1->Left+CGauge1->Width+BitBtn4->Left;
      //Label2->Width=110;
      //Label3->Width=95;
    break;
    case TEST_STEP:
      Label4->Visible=false;
      BitBtn1->Visible=true;
      BitBtn2->Visible=true;
      BitBtn3->Visible=true;
      BitBtn4->Visible=false;
      BitBtn3->Left=BitBtn1->Left+(BitBtn2->Left+BitBtn2->Width-BitBtn1->Left)/2-BitBtn3->Width/2;
      //Width=BitBtn1->Left+BitBtn2->Left+BitBtn2->Width;

      //Label2->Width=110;
      //Label3->Width=95;
      Label4->Visible=false;
      CGauge1->Visible=false;
      //PrintStep->Visible=false;
      //ResetTabStep->Visible=false;
      // comando d'impostazione...
      m_usEncoder=MIN2USEC/((Reg_Stp/Rap_tot1)*dynoPrm.bench.nImpulsiEncoder);
     // comando step rpm
      if(!(Main->getStatusDat()==START_SIMUL_DAT || Main->getStatusDat()==RUN_SIMUL_DAT)) g_brakeControl.CmdStep(m_usEncoder,CDyn3::EFrontAxle);
      Caption=Impostazioni->ComboBox1->Items->Strings[1];
      if(bFileTrace)
      {
        sprintf(sLog,"Cmd Step RPM=%d",Reg_Stp);
        LogError(g_cDyn3Log,LOG_INFO,sLog);
      }
    break;
    case 2:
      Caption=Impostazioni->ComboBox1->Items->Strings[4];
    break;
    case 4:
      Caption=Impostazioni->ComboBox1->Items->Strings[2];
    break;
    case 5:
      Caption=Impostazioni->ComboBox1->Items->Strings[3];
    break;
    default:
    break;
  }



  Main->SpeedButton8->Enabled=false;
  Main->SpeedButton4->Enabled=false;
  Main->SpeedButton1->Enabled=false;
  Main->SpeedButton2->Enabled=false;
  /// In caso di simulazione DAT viene inviato comando AVVIO dal Main
  Main->pressStartDat();
  
}
//---------------------------------------------------------------------------
void __fastcall TStep_test::BitBtn1Click(TObject *Sender)
{
  Reg_Stp += Delta_reg; // incrementa i giri
  if( rpmStop > rpmStart)
  {
    if(Reg_Stp > rpmStop) Reg_Stp = rpmStop;
    if(Reg_Stp < rpmStart) Reg_Stp = rpmStart;
  }

  if( rpmStop < rpmStart)
  {
    if(Reg_Stp < rpmStop) Reg_Stp = rpmStop;
    if(Reg_Stp > rpmStart) Reg_Stp = rpmStart;
  }
  Label2->Caption=IntToStr(Reg_Stp)+" "+Str0; // aggiorna valore visualizzato
  m_usEncoder=MIN2USEC/((Reg_Stp/Rap_tot1)*dynoPrm.bench.nImpulsiEncoder);
  if(!(Main->getStatusDat()==START_SIMUL_DAT || Main->getStatusDat()==RUN_SIMUL_DAT)) g_brakeControl.CmdStep(m_usEncoder,CDyn3::EFrontAxle);
  if(bFileTrace)
  {
    sprintf(sLog,"Cmd Step RPM=%d",Reg_Stp);
    LogError(g_cDyn3Log,LOG_INFO,sLog);
  }
}
//---------------------------------------------------------------------------

void __fastcall TStep_test::BitBtn2Click(TObject *Sender)
{
  Reg_Stp -= Delta_reg; // decrementa i giri

  if( rpmStop > rpmStart)
  {
    if(Reg_Stp > rpmStop) Reg_Stp = rpmStop;
    if(Reg_Stp < rpmStart) Reg_Stp = rpmStart;
  }
  if( rpmStop < rpmStart)
  {
    if(Reg_Stp < rpmStop) Reg_Stp = rpmStop;
    if(Reg_Stp > rpmStart) Reg_Stp = rpmStart;
  }
  Label2->Caption=IntToStr(Reg_Stp)+" "+Str0; // aggiorna valore visualizzato
  // comando step rpm
  m_usEncoder=MIN2USEC/((Reg_Stp/Rap_tot1)*dynoPrm.bench.nImpulsiEncoder);
  if(!(Main->getStatusDat()==START_SIMUL_DAT || Main->getStatusDat()==RUN_SIMUL_DAT)) g_brakeControl.CmdStep(m_usEncoder,CDyn3::EFrontAxle);
  if(bFileTrace)
  {
    sprintf(sLog,"Cmd Step RPM=%d",Reg_Stp);
    LogError(g_cDyn3Log,LOG_INFO,sLog);
  }
}
//---------------------------------------------------------------------------

void __fastcall TStep_test::BitBtn4Click(TObject *Sender)
{
  Contatore=0;
  num_pass_step=0;
  Tstep=0;
  Val_cop_medio=0;
  Val_reg_medio=0;
  Val_oil_medio=0;
  Val_map_medio=0;
  Val_k_medio=0;
  gbStepTestCancel=false;
  BitBtn4->Enabled=true;
  BitBtn3->Enabled=true;
  CGauge1->Progress=0;
  Main->Series105->Clear();
  Main->Series106->Clear();
  Main->Series107->Clear();

  Main->ResetTabAcq();
  //primaVoltaTabStep=1;
  //rowTabStep=1;
  CGauge1->MaxValue=int(Tempo_staz/0.025);
  Main->StaticText2->Visible=true;
  Main->Panel33->Visible=true;
  Main->Azzera_var();

  if(rpmStart < rpmStop && Reg_Stp >= rpmStop) Reg_Stp=rpmStart;
  if(rpmStart > rpmStop && Reg_Stp <= rpmStop) Reg_Stp=rpmStart;
//  OK_TEST_STEP=true; // attiva test a step determinato
  // comando d'impostazione...
  //m_usEncoder=MIN2USEC/((Reg_Stp/Rap_tot1)*dynoPrm.bench.nImpulsiEncoder);
  //g_brakeControl.CmdStep(m_usEncoder,CDyn3::EFrontAxle);

  //tmrStep->Enabled=true;
  bAvvioPressed=true;
  Main->createDat();
  tmrStepTimer(this);
  Label2->Caption=IntToStr(Reg_Stp)+" "+Str0;
  BitBtn4->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall TStep_test::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  if(Tipo_test==TEST_STEP)
  {
    if(Key=='k')
      BitBtn1Click(this);
    else if(Key=='m')
      BitBtn2Click(this);
  }
  //Main->Remote(Key);
}
//---------------------------------------------------------------------------


void __fastcall TStep_test::ResetTabStepClick(TObject *Sender)
{
  Main->ResetTabAcq();
}
//---------------------------------------------------------------------------

void __fastcall TStep_test::PrintStepClick(TObject *Sender)
{
    Main->TabStepPrint->Enabled = true;
    Main->AddTabAcq();
}
//---------------------------------------------------------------------------
WideString TStep_test::getStep(void)
{
  return Reg_Stp;
}

void __fastcall TStep_test::FormClose(TObject *Sender,
      TCloseAction &Action)
{

  OK_TEST_STEP=false;
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

void __fastcall TStep_test::tmrStepTimer(TObject *Sender)
{
  static int iTarget=0;
  tmrStep->Enabled=false;


  if(lRpm*Rap_tot1-Reg_Stp>iMaxStepDistance && (!gbStepTestCancel) && bAvvioPressed)
  {
    if(iTarget==0 || abs((int)(lRpm*Rap_tot1)-iTarget)<15)
    {
      iTarget=(int)(lRpm*Rap_tot1-iMaxStepDistance);
      m_usEncoder=MIN2USEC/(((lRpm*Rap_tot1-iMaxStepDistance)/Rap_tot1)*dynoPrm.bench.nImpulsiEncoder);
      if(!(Main->getStatusDat()==START_SIMUL_DAT || Main->getStatusDat()==RUN_SIMUL_DAT)) g_brakeControl.CmdStep(m_usEncoder,CDyn3::EFrontAxle);
      if(bFileTrace)
      {
        sprintf(sLog,"Cmd Step RPM=%d",(int)(lRpm*Rap_tot1-iMaxStepDistance));
        LogError(g_cDyn3Log,LOG_INFO,sLog);
      }
    }
    tmrStep->Enabled=true;
  }
  else if(!gbStepTestCancel && bAvvioPressed)
  {
    m_usEncoder=MIN2USEC/((Reg_Stp/Rap_tot1)*dynoPrm.bench.nImpulsiEncoder);
    if(!(Main->getStatusDat()==START_SIMUL_DAT || Main->getStatusDat()==RUN_SIMUL_DAT)) g_brakeControl.CmdStep(m_usEncoder,CDyn3::EFrontAxle);
    if(bFileTrace)
    {
      sprintf(sLog,"Cmd Step RPM=%d",Reg_Stp);
      LogError(g_cDyn3Log,LOG_INFO,sLog);
    }
    OK_TEST_STEP=true; // attiva test a step determinato
    bAvvioPressed=false;
    iTarget=0;
  }
  else
  {
    if(!(Main->getStatusDat()==START_SIMUL_DAT || Main->getStatusDat()==RUN_SIMUL_DAT)) g_brakeControl.CmdStop();
    iTarget=0;
    if(bFileTrace)            
    {
      LogError(g_cDyn3Log,LOG_INFO,"Cmd Stop");
    }
    bAvvioPressed=false;
    //Main->stopAndSaveDat();
  }

}
//---------------------------------------------------------------------------

