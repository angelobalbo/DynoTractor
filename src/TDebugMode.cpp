//---------------------------------------------------------------------------

#include <vcl.h>
#include <IniFiles.hpp>
#pragma hdrstop

#include "TDebugMode.h"
#include "CDyn3.h"
#include "CUtils.h"
#include "Structs.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TeePageNumTool"
#pragma link "TeeTools"
#pragma resource "*.dfm"
TfrmDebugMode *frmDebugMode;

extern unsigned short Ki_prop,Ki_integ; // valore attuali inviati con cmq trq const
extern int Tipo_test;
int rpmMax=10000;
extern struct DYNO_SETS dynoPrm;
unsigned short sPidKp_step; // 0..FFFF x 10
unsigned short sPidKi_step; // 0..FFFF
unsigned short sPidKd_step; // 0..FFFF x 10000
unsigned short sPidKp_trq; // 0..FFFF x 10
unsigned short sPidKi_trq; // 0..FFFF
unsigned short sPidKd_trq; // 0..FFFF x 10000

unsigned short sPidKp_step_ini; // 0..FFFF x 10
unsigned short sPidKi_step_ini; // 0..FFFF
unsigned short sPidKd_step_ini; // 0..FFFF x 10000
unsigned short sPidKp_trq_ini; // 0..FFFF x 10
unsigned short sPidKi_trq_ini; // 0..FFFF
unsigned short sPidKd_trq_ini; // 0..FFFF x 10000

String strPID;
extern float Valore_Cop;
extern int targetStep;
//---------------------------------------------------------------------------
__fastcall TfrmDebugMode::TfrmDebugMode(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmDebugMode::Button1Click(TObject *Sender)
{
  srsP->Clear();
  srsI->Clear();
  srsTarget->Clear();
  srsCurrent->Clear();
  srsPwm->Clear();
  chDebug->BottomAxis->Minimum=0;
  //frmDebugMode->chDebug->BottomAxis->Scroll(0,true);
  frmDebugMode->chDebug->BottomAxis->AdjustMaxMin();
  sbMaxPoints->Position=10;
  sbMaxPoints->Max=100;
  sbMaxPoints->Position=100;
  Label3->Caption=IntToStr(sbMaxPoints->Max);
  sbMaxPoints->Hint=IntToStr(sbMaxPoints->Position);
}
//---------------------------------------------------------------------------

void __fastcall TfrmDebugMode::Button2Click(TObject *Sender)
{
  if(g_brakeControl.GetDebugMode())
  {
     if(MessageDlg("Stop DEBUG mode?",mtConfirmation,TMsgDlgButtons() << mbYes << mbNo,0) == mrYes)
     {
       if(g_brakeControl.CmdDebug(false))
        Button2->Caption="START DEBUG";
     }
  }
  else
  if(MessageDlg("Start DEBUG mode?",mtConfirmation,TMsgDlgButtons() << mbYes << mbNo,0) == mrYes)
  {
       if(g_brakeControl.CmdDebug(true))
       {
        Button2->Caption="STOP DEBUG";

       }
  }

}
//---------------------------------------------------------------------------

void __fastcall TfrmDebugMode::BitBtn1Click(TObject *Sender)
{
  chDebug->PrintLandscape();
}
//---------------------------------------------------------------------------



void __fastcall TfrmDebugMode::sbMaxPointsChange(TObject *Sender)
{
  if(chDebug->BottomAxis->Maximum>sbMaxPoints->Position)
    chDebug->BottomAxis->Minimum=chDebug->BottomAxis->Maximum-sbMaxPoints->Position;

  sbMaxPoints->Hint=IntToStr(sbMaxPoints->Position);
}
//---------------------------------------------------------------------------

void __fastcall TfrmDebugMode::btnWritePidClick(TObject *Sender)
{

  TIniFile *ini;
  int iNewValue;

  if(Tipo_test==TEST_COST_TRQ)
  {
    iNewValue=StrToIntDef(lePtrq->Text,sPidKp_trq);
    if(iNewValue<1 || iNewValue > 65535)
      lePtrq->Text=sPidKp_trq;
    else
      sPidKp_trq=iNewValue;

    iNewValue=StrToIntDef(leItrq->Text,sPidKi_trq);
    if(iNewValue<1 || iNewValue > 65535)
      leItrq->Text=sPidKi_trq;
    else
      sPidKi_trq=iNewValue;

    iNewValue=StrToIntDef(leDtrq->Text,sPidKd_trq);
    if(iNewValue<1 || iNewValue > 65535)
      leDtrq->Text=sPidKd_trq;
    else
      sPidKd_trq=iNewValue;
    if(MessageDlg("Aggiornare il PID sulla scheda con le costanti selezionate?",mtConfirmation,TMsgDlgButtons() << mbYes << mbNo,0) == mrYes)
    {
      g_brakeControl.CmdWritePID(sPidKp_trq,sPidKi_trq,sPidKd_trq);
      do {
        Sleep(300);
        Application->ProcessMessages();
        }
      while(g_brakeControl.IsCmdInProgress());
      g_brakeControl.CmdReadPID();
      do {
        Sleep(300);
        Application->ProcessMessages();
        }
      while(g_brakeControl.IsCmdInProgress());
      g_brakeControl.GetPID(&sPidKp_trq,&sPidKi_trq,&sPidKd_trq);
      lePtrq->Text=sPidKp_trq;
      leItrq->Text=sPidKi_trq;
      leDtrq->Text=sPidKd_trq;

      ini = new TIniFile(utils.GetProgramDataName(".ini"));
      ini->WriteInteger ("PID control trq", "K proporzionale",sPidKp_trq);
      ini->WriteInteger ("PID control trq", "K integrale",sPidKi_trq);
      ini->WriteInteger ("PID control trq", "K derivativo",sPidKd_trq);
      delete ini;//
    }
  }
  else if(Tipo_test==TEST_STEP || Tipo_test==TEST_STEP_TIMED)
  {
    iNewValue=StrToIntDef(lePstep->Text,sPidKp_step);
    if(iNewValue<1 || iNewValue > 65535)
      lePstep->Text=sPidKp_step;
    else
      sPidKp_step=iNewValue;

    iNewValue=StrToIntDef(leIstep->Text,sPidKi_step);
    if(iNewValue<1 || iNewValue > 65535)
      leIstep->Text=sPidKi_step;
    else
      sPidKi_step=iNewValue;

    iNewValue=StrToIntDef(leDstep->Text,sPidKd_step);
    if(iNewValue<1 || iNewValue > 65535)
      leDstep->Text=sPidKd_step;
    else
      sPidKd_step=iNewValue;
    if(sPidKp_step!=sPidKp_step_ini ||
       sPidKi_step!=sPidKi_step_ini ||
       sPidKd_step!=sPidKd_step_ini)
    if(MessageDlg("Aggiornare il PID sulla scheda con le costanti selezionate?",mtConfirmation,TMsgDlgButtons() << mbYes << mbNo,0) == mrYes)
    {
      g_brakeControl.CmdWritePID(sPidKp_step,sPidKi_step,sPidKd_step);
      do
      {
        Sleep(300);
        Application->ProcessMessages();
      }
      while(g_brakeControl.IsCmdInProgress());
      g_brakeControl.CmdReadPID();
      do
      {
        Sleep(300);
        Application->ProcessMessages();
      }
      while(g_brakeControl.IsCmdInProgress());
      g_brakeControl.GetPID(&sPidKp_step,&sPidKi_step,&sPidKd_step);

      lePstep->Text=sPidKp_step;
      leIstep->Text=sPidKi_step;
      leDstep->Text=sPidKd_step;
      ini = new TIniFile(utils.GetProgramDataName(".ini"));
      ini->WriteInteger ("PID control rpm", "K proporzionale",sPidKp_step);
      ini->WriteInteger ("PID control rpm", "K integrale",sPidKi_step);
      ini->WriteInteger ("PID control rpm", "K derivativo",sPidKd_step);
      delete ini;//
    }
  }

}
//---------------------------------------------------------------------------

void __fastcall TfrmDebugMode::btnReadPidClick(TObject *Sender)
{
  bool res;
  res=checkPID();
  if(res==false)
  {
    if(MessageDlg(strPID+" aggiornare il PID sulla scheda con le ultime costanti memorizzate?",mtConfirmation,TMsgDlgButtons() << mbYes << mbNo,0) == mrYes)
    {
      if(Tipo_test==TEST_STEP || Tipo_test==TEST_STEP_TIMED)
      {
        sPidKp_step=sPidKp_step_ini;
        sPidKi_step=sPidKi_step_ini;
        sPidKd_step=sPidKd_step_ini;

        if(g_brakeControl.CmdWritePID(sPidKp_step,sPidKi_step,sPidKd_step))
        {
          lePstep->Text=sPidKp_step;
          leIstep->Text=sPidKi_step;
          leDstep->Text=sPidKd_step;
        }
      }
      else if(Tipo_test==TEST_COST_TRQ)
      {
/*        g_brakeControl.CmdPwm(0,Ki_integ,Ki_prop);
        do
        {
          Application->ProcessMessages();
          Sleep(300);
        }
        while(g_brakeControl.IsCmdInProgress());*/
        sPidKp_trq=sPidKp_trq_ini;
        sPidKi_trq=sPidKi_trq_ini;
        sPidKd_trq=sPidKd_trq_ini;
        if(g_brakeControl.CmdWritePID(sPidKp_trq,sPidKi_trq,sPidKd_trq))
        {
          lePtrq->Text=sPidKp_trq;
          leItrq->Text=sPidKi_trq;
          leDtrq->Text=sPidKd_trq;
        }
      }
    }
  }
}
//---------------------------------------------------------------------------
// checkPID legge i valori dei PID sotto specifiche condizioni, poichè deve
// distinguere il tipo di test in vigore:
// Tipo_test==TEST_COST_TRQ (se OK_N_cost
bool TfrmDebugMode::checkPID()
{
  unsigned short sNewKp,sNewKi,sNewKd;
  unsigned short usEncoder=MIN2USEC/((rpmMax)*dynoPrm.bench.nImpulsiEncoder);
  TIniFile *ini;
  ini = new TIniFile(utils.GetProgramDataName(".ini"));
  lePtrq->Enabled=(Tipo_test==TEST_COST_TRQ);
  leItrq->Enabled=(Tipo_test==TEST_COST_TRQ);
  leDtrq->Enabled=(Tipo_test==TEST_COST_TRQ);

  lePstep->Enabled=(Tipo_test==TEST_STEP || Tipo_test==TEST_STEP_TIMED);
  leIstep->Enabled=(Tipo_test==TEST_STEP || Tipo_test==TEST_STEP_TIMED);
  leDstep->Enabled=(Tipo_test==TEST_STEP || Tipo_test==TEST_STEP_TIMED);

  Button2->Caption=(g_brakeControl.GetDebugMode())?"STOP DEBUG":"START DEBUG";
  if(Tipo_test==TEST_COST_TRQ)
  {
    if(Valore_Cop==0)
    {
      // Imporre una minima azione di controllo pwm affichè il fw selezioni i giusti PID coppia cost
     g_brakeControl.CmdStop();
      do
      {
        Sleep(100);
        Application->ProcessMessages();
      }
      while(g_brakeControl.IsCmdInProgress());
      g_brakeControl.CmdPwm(0,Ki_integ,Ki_prop);
      do
      {
        Sleep(100);
        Application->ProcessMessages();
      }
      while(g_brakeControl.IsCmdInProgress());
      g_brakeControl.CmdStop();
      do
      {
        Sleep(100);
        Application->ProcessMessages();
      }
      while(g_brakeControl.IsCmdInProgress());
    }
    g_brakeControl.CmdReadPID();
    do
    {
      Sleep(100);
      Application->ProcessMessages();
    }
    while(g_brakeControl.IsCmdInProgress());
    g_brakeControl.GetPID(&sPidKp_trq,&sPidKi_trq,&sPidKd_trq);
    g_brakeControl.CmdStop();
      do
      {
        Sleep(100);
        Application->ProcessMessages();
      }
    while(g_brakeControl.IsCmdInProgress());
    lePtrq->Text=sPidKp_trq;
    leItrq->Text=sPidKi_trq;
    leDtrq->Text=sPidKd_trq;
  }
  else if (Tipo_test==TEST_STEP || Tipo_test==TEST_STEP_TIMED)
  {
    if(targetStep==0) // Test non ancora iniziati, invio cmq test a step senza giri
    {
     g_brakeControl.CmdStop();
     do
      {
        Sleep(100);
        Application->ProcessMessages();
      }
      while(g_brakeControl.IsCmdInProgress());
      g_brakeControl.CmdStep(usEncoder,CDyn3::EFrontAxle);
      do
      {
        Sleep(100);
        Application->ProcessMessages();
      }
      while(g_brakeControl.IsCmdInProgress());
      g_brakeControl.CmdStop();
     do
      {
        Sleep(100);
        Application->ProcessMessages();
      }
      while(g_brakeControl.IsCmdInProgress());
    }

    g_brakeControl.CmdReadPID();
    do
    {
      Sleep(100);
      Application->ProcessMessages();
    }
    while(g_brakeControl.IsCmdInProgress());
    g_brakeControl.GetPID(&sPidKp_step,&sPidKi_step,&sPidKd_step);
    g_brakeControl.CmdStop();
      do
      {
        Sleep(100);
        Application->ProcessMessages();
      }
    while(g_brakeControl.IsCmdInProgress());
    lePstep->Text=sPidKp_step;
    leIstep->Text=sPidKi_step;
    leDstep->Text=sPidKd_step;
  }
  sPidKp_step_ini=ini->ReadInteger ("PID control rpm", "K proporzionale",3500);
  sPidKi_step_ini=ini->ReadInteger ("PID control rpm", "K integrale",50);
  sPidKd_step_ini=ini->ReadInteger ("PID control rpm", "K derivativo",0);

  sPidKp_trq_ini=ini->ReadInteger ("PID control trq", "K proporzionale",900);
  sPidKi_trq_ini=ini->ReadInteger ("PID control trq", "K integrale",1800);
  sPidKd_trq_ini=ini->ReadInteger ("PID control trq", "K derivativo",0);
  delete ini;

  if(Tipo_test==TEST_STEP || Tipo_test==TEST_STEP_TIMED)
  {
    strPID=+"RPM step test P="+IntToStr(sPidKp_step_ini)+" I="+IntToStr(sPidKi_step_ini)+" D="+IntToStr(sPidKd_step_ini);
    if( sPidKp_step_ini!=sPidKp_step ||
        sPidKi_step_ini!=sPidKi_step ||
        sPidKd_step_ini!=sPidKd_step)
      return false;
  }
  else if (Tipo_test==TEST_COST_TRQ)
  {
    strPID=+"RPM trq test P="+IntToStr(sPidKp_trq_ini)+" I="+IntToStr(sPidKi_trq_ini)+" D="+IntToStr(sPidKd_trq_ini);
    if( sPidKp_trq_ini!=sPidKp_trq ||
        sPidKi_trq_ini!=sPidKi_trq ||
        sPidKd_trq_ini!=sPidKd_trq)
      return false;
  }

  return true; // solo se costanti pid uguali a quelle memorizzate su ini
}
void __fastcall TfrmDebugMode::FormShow(TObject *Sender)
{
  checkPID();

}
//---------------------------------------------------------------------------

