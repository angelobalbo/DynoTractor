//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop


#include <stdio.h>
#include <fstream.h>
#include <shfolder.h>
#include <time.h>
#include "TCan.h"
#include "TMain.h"
#include "CUtils.h"
#include "CanBus.h"
#include "log.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TeeTools"
#pragma resource "*.dfm"
TfrmCan *frmCan;
extern bool OK_N_cost;

extern char g_cFileLog[MAX_PATH];

extern bool IO_ref;
extern float  Val_potenza_m1,
              fK_convP,
              fFact,
              Val_coppia_m1,
              fK_convC,
              Rap_tot1,
              TAir,
              Val_coppia_m1,
              fFact,
              fK_convC,
              Rap_tot1;
extern long lRpm;
extern float Valore_Cop;
static long timing=0,t=0,timingTrq=0;
static long clockPrec,clockPrecTrq;
static int  step=0,stepTrq=0;
static float rpm1=0,rpm0,rpm;
static float trq1=0,trq0,trq;
long tmrInterval,trqInterval;

#define MAX_TAB_STEP 50
typedef struct
{
  float time [MAX_TAB_STEP];
  float timestamp[MAX_TAB_STEP];
  float rpm  [MAX_TAB_STEP];
  float trq  [MAX_TAB_STEP];
  int rows;
} TAB_STEP_CAN;

TAB_STEP_CAN tabStep;
//---------------------------------------------------------------------------
__fastcall TfrmCan::TfrmCan(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TfrmCan::meCanIdChange(TObject *Sender)
{
  sscanf(meCanId->Text.c_str(), "%x", &lIdReqCan);
}
//---------------------------------------------------------------------------
void __fastcall TfrmCan::meCanRateChange(TObject *Sender)
{
  lTimeRateCan=StrToIntDef(meCanRate->Text, 50);
}
//---------------------------------------------------------------------------
void __fastcall TfrmCan::BitBtn1Click(TObject *Sender)
{

  lTimeRateCan=StrToIntDef(meCanRate->Text, 50);
  sscanf(meCanId->Text.c_str(), "%x", &lIdReqCan);
  tmrRate->Interval=lTimeRateCan;
  if(statusCan==EDisc)
    if(openCan()==false)
    {
      LogError(g_cFileLog,LOG_ERR,"Apertura Kvaser fallita!");
      return;
    }

  //if(!mycanCreate())
  String s;


  if(connectCAN(lIdReqCan,lTimeRateCan,cbCanRate->ItemIndex))
  {
    s="Connessione Can bus Kvaser";
    LogError(g_cFileLog,LOG_INFO,s.c_str());
    imgStatus->Visible=true;
    imgStatusStart->Visible=false;
    imgStatusStop->Visible=false;
    imgStatusErr->Visible=false;
    tmrRate->Enabled=false;
    btnStart->Enabled=true;
    btnStop->Enabled=false;
    BitBtn3->Enabled=false;
  }
  else
  {
    s="Connessione Kvaser FALLITA!";
    LogError(g_cFileLog,LOG_INFO,s.c_str());
    imgStatus->Visible=false;
    imgStatusStart->Visible=false;
    imgStatusStop->Visible=false;
    imgStatusErr->Visible=true;
  }
}
//---------------------------------------------------------------------------



void __fastcall TfrmCan::btnStartClick(TObject *Sender)
{
  if(statusCan==EConnCAN)
  {
    imgStatus->Visible=false;
    imgStatusStart->Visible=true;
    imgStatusStop->Visible=false;
    imgStatusErr->Visible=false;
    tmrRate->Enabled=true;
    btnStart->Enabled=false;
    btnStop->Enabled=true;
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmCan::tmrRateTimer(TObject *Sender)
{
  //
  static long rpm, torque, power,temp;
  static unsigned char cCanData[8];
  tmrRate->Enabled=false;
  if(btnStop->Tag==1)
  {
    statusCan=EConnCAN;
    btnStop->Tag=0;
    imgStatusStop->Visible=true;
    imgStatusStart->Visible=false;
    imgStatusErr->Visible=false;
    btnStart->Enabled=true;
    btnStop->Enabled=false;
    BitBtn3->Enabled=false;
    return;
  }
  if(statusCan==EConnCAN || statusCan==EReadyCAN)
  {
    // RPM motore giri/min
    rpm=(long)((float)lRpm*Rap_tot1);

    // Coppia
    torque=(long)(Val_coppia_m1*fK_convC*fFact*10);

    // Potenza
    power=(long)(Val_potenza_m1*fK_convP*fFact*10);

    // °C Temperatura atmosferica
    temp=(long)(TAir+40);
    cCanData[0]=rpm>>8;
    cCanData[1]=(rpm)&0xFF;

    cCanData[2]=(torque)>>8;
    cCanData[3]=(torque)&0xFF;

    cCanData[4]=(power)>>8;
    cCanData[5]=(power)&0xFF;
    cCanData[6]=(temp)&0xFF;

    if(txCan(lIdReqCan,cCanData))
      tmrRate->Enabled=true;
    else if(statusCan==EErrCAN)
    {
       imgStatusStart->Visible=false;
       imgStatusErr->Visible=true;
       btnStart->Enabled=false;
       btnStop->Enabled=false;
       BitBtn3->Enabled=true;
    }

  }

}
//---------------------------------------------------------------------------

void __fastcall TfrmCan::btnStopClick(TObject *Sender)
{
  btnStop->Tag=1;

}
//---------------------------------------------------------------------------

void __fastcall TfrmCan::FormClose(TObject *Sender, TCloseAction &Action)
{
  if(statusCan!=EDisc)
    closeCan();
}
//---------------------------------------------------------------------------

void __fastcall TfrmCan::FormShow(TObject *Sender)
{
  //
  if(statusCan==EDisc)
  {
    BitBtn3->Enabled=true;
    btnStart->Enabled=false;
    btnStop->Enabled=false;
    imgStatusStop->Visible=false;
    imgStatusStart->Visible=false;
    imgStatusErr->Visible=false;
    imgStatus->Visible=false;
  }
  tmrCmd->Interval=tmrInterval;
  tmrTrq->Interval=trqInterval;

}
//---------------------------------------------------------------------------

void __fastcall TfrmCan::sbCmdClick(TObject *Sender)
{
//
  if(sbCmd->Caption=="START")
  {
    srsRpm->Clear();
    srsTrq->Clear();
    srsRpmRt->Clear();
    srsTrqRt->Clear();
    t=0;
    timing=tabStep.time[0]*1000;
    timingTrq=tabStep.time[0]*1000;
    step=1;
    stepTrq=1;
    clockPrec=clock();
    clockPrecTrq=clock();
    rpm0=tabStep.rpm[0];
    rpm1=tabStep.rpm[1];
    trq0=tabStep.trq[0];
    trq1=tabStep.trq[1];
    rpm=rpm0;
    trq=trq0;
    tmrCmd->Enabled=true;
    tmrTrq->Enabled=true;
    sbCmd->Caption="STOP";
    Valore_Cop=0;
    OK_N_cost=true;

  }
  else
  {
    tmrCmd->Enabled=false;
    tmrTrq->Enabled=false;
    sbCmd->Caption="START";
    Main->EndTest();
    Valore_Cop=0;
    OK_N_cost=false;
  }

}
//---------------------------------------------------------------------------


void __fastcall TfrmCan::tmrCmdTimer(TObject *Sender)
{

  static long id;
  static unsigned char cCanData[8];
  static float deltaRpm;

  static float interval;

  tmrCmd->Enabled=false;

  // Condizione di avanzamento step, o step iniziale


  interval=(1000*(clock()-clockPrec)/CLK_TCK);
  timing+=interval;
  clockPrec=clock();
  if(((rpm0==rpm1) && (timing>=(tabStep.time[step])*1000)) ||
     (rpm1>rpm0 && rpm>=rpm1) ||
     (rpm1<rpm0 && rpm<=rpm1))
  {
    rpm0=tabStep.rpm[step];    // rpm iniziale
    //timing=tabStep.time[step]*1000; // tempo iniziale ms


    step++;
    if(step<tabStep.rows)
    {
      rpm1=tabStep.rpm[step];    // rpm finale
      rpm=rpm0; // Valore attuale da imporre risulta il valore iniziale
      //clockPrec=clock();
    }
    else // condizione di fine prova
    {
      sbCmd->Caption="START";
      if(OK_N_cost==true)
      {
        OK_N_cost=false;
        Main->EndTest();
        Valore_Cop=0;
        tmrTrq->Enabled=false;
      }
    }

  }
  else
  {
    //interval=(1000*(clock()-clockPrec)/CLK_TCK);
     //CASO 1: mantenimento rpm1=rpm0, tengo il tempo
     if(rpm1==rpm0)
      rpm=rpm1;
     //CASO 2: rampa in salita o discesa
     else
     {

        deltaRpm=(rpm1-rpm)/((float)(tabStep.time[step]*1000-timing+interval)/(float)interval);
        rpm+=deltaRpm;
        if      (rpm1>rpm0 && rpm>rpm1) rpm=rpm1;
        else if (rpm1<rpm0 && rpm<rpm1) rpm=rpm1;
     }

     //timing=timing+interval;

     //clockPrec=clock();
  }

  if(sbCmd->Caption=="STOP")
  {
    srsRpm->AddXY(timing/1000, rpm);
    srsRpmRt->AddXY(timing/1000,(lRpm*Rap_tot1));
    chart->BottomAxis->Title->Caption="T = "+FormatFloat("#.##",(float)timing/1000)+"s";
    if(statusCan==EConnCAN || statusCan==EReadyCAN)
    {
      id=0x0C000027;

      if(rpm>0)
      {
        cCanData[0]=0x01;
        cCanData[1]=(int)(rpm*8)&0xFF;
        cCanData[2]=(int)(rpm*8)>>8;
        cCanData[3]=0xFF;
        cCanData[4]=0xFF;
        cCanData[5]=0xFF;
        cCanData[6]=0xFF;
        cCanData[7]=0xFF;
        if(txCan(id,cCanData)==false)
          lblRes->Caption="KO";
        else
          lblRes->Caption="OK";
      }
      //tmrCmd->Enabled=true;
    }
    tmrCmd->Enabled=true;
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmCan::SpeedButton1Click(TObject *Sender)
{
  String sTmpFileName,sStrBuf;
  int i,col;
  char sBuffer[200];
  //ritorna la directory per il salvataggio
  AnsiString dir="";
  char cDocPath[MAX_PATH];
  char cPerc[MAX_PATH];
  char cPercPath[MAX_PATH];
  bool f;

  SHGetFolderPath(NULL, CSIDL_PERSONAL,NULL,NULL,cDocPath);
  sprintf(cPerc,"%s\\%s",cDocPath,DYNO_TEST_PATH);
  f=DirectoryExists(cPerc);
  if (!f)
  {
    try
    {
      CreateDirectory(cPerc,NULL);
    }
    catch(Exception &e)
    {
    }
  }
  //ricontrollo se è stato creato
  f=DirectoryExists(cPerc);
  if (f)
    dir=cPerc;

  memset (&tabStep,0,sizeof(TAB_STEP_CAN));
  OpenDialog1->InitialDir=dir+"\\StepTest";
  OpenDialog1->DefaultExt="*.csv";
  if(OpenDialog1->Execute())
  {
    sTmpFileName=OpenDialog1->FileName;
    if((sTmpFileName.SubString(sTmpFileName.Length()-3,4).UpperCase()==".CSV"))
    {
      ifstream infile(sTmpFileName.c_str(), ios::binary);
      if(infile.is_open())
      {
        infile.seekg (0, ios::beg);
        /// Aggiornamento dati di interstazione
        infile.getline(sBuffer,sizeof(sBuffer));
        i=0;
        tabStep.rows=0;
        while(!infile.eof() && i<MAX_TAB_STEP)
        {
          memset(sBuffer,0,sizeof(sBuffer));
          infile.getline(sBuffer,sizeof(sBuffer));
          sStrBuf=sBuffer;
          if(sStrBuf.IsEmpty()==false)
          {
          if(sStrBuf.AnsiPos("\r")>0)
          {
            sBuffer[sStrBuf.AnsiPos("\r")-1]=';';
            sStrBuf=sBuffer;
          }
          col=0;
          if(sStrBuf.AnsiPos(";")>0)
          {
            tabStep.time[i]=StrToFloatDef(sStrBuf.SubString(0,sStrBuf.AnsiPos(";")-1),0);
            sStrBuf=sStrBuf.SubString(sStrBuf.AnsiPos(";")+1,sStrBuf.Length());
            col++;
          }

          if(sStrBuf.AnsiPos(";")>0)
          {
            tabStep.rpm[i]=StrToFloatDef(sStrBuf.SubString(0,sStrBuf.AnsiPos(";")-1),0);
            sStrBuf=sStrBuf.SubString(sStrBuf.AnsiPos(";")+1,sStrBuf.Length());
            col++;
          }
          if(sStrBuf.AnsiPos(";")>0)
          {
            tabStep.trq[i]=StrToFloatDef(sStrBuf.SubString(0,sStrBuf.AnsiPos(";")-1),0);
            sStrBuf=sStrBuf.SubString(sStrBuf.AnsiPos(";")+1,sStrBuf.Length());
            col++;
          }
          if (col<3) break;
          tabStep.rows++;
          i++;
          }
        } // while
        infile.close();
        if((col<3 && col >0) || i==0)
          MessageBox(NULL,"Table is not correct!","ERROR",MB_OK | MB_ICONERROR);
        else
          sbCmd->Enabled=true;

      } // if infile...
    } // if((sTmpFileName..
  } // if(OpenDialog1->Execute())

}
//---------------------------------------------------------------------------

void __fastcall TfrmCan::tmrTrqTimer(TObject *Sender)
{
  static float deltaTrq;
  static long interval;

  tmrTrq->Enabled=false;

  interval=(1000*(clock()-clockPrecTrq)/CLK_TCK);
  timingTrq+=interval;
  clockPrecTrq=clock();


  // Condizione di avanzamento step, o step iniziale
  if(sbCmd->Caption=="START") return;
  if(((trq0==trq1) && (timingTrq>=(tabStep.time[stepTrq])*1000)) ||
     (trq1>trq0 && trq>=trq1) ||
     (trq1<trq0 && trq<=trq1))
  {
    trq0=tabStep.trq[stepTrq];    // trq iniziale
    stepTrq++;
    if(stepTrq<tabStep.rows)
    {
      trq1=tabStep.trq[stepTrq];    // trq finale
      trq=trq0;
    }
    else // condizione di fine prova
    {
      sbCmd->Caption="START";
      Main->EndTest();
      OK_N_cost=false;
      Valore_Cop=0;
    }

  }
  else
  {
     //CASO 1: mantenimento trq1=trq0, tengo il tempo
     if(trq1==trq0)
      trq=trq1;
     //CASO 2: rampa in salita o discesa
     else
     {
        deltaTrq=(trq1-trq)/((float)(tabStep.time[stepTrq]*1000-timingTrq+interval)/(float)interval);
        trq+=deltaTrq;
        if      (trq1>trq0 && trq>trq1) trq=trq1;
        else if (trq1<trq0 && trq<trq1) trq=trq1;
     }
  }

  if(sbCmd->Caption=="STOP")
  {
    srsTrq->AddXY(timingTrq/1000, trq);
    srsTrqRt->AddXY(timingTrq/1000,Val_coppia_m1 * fK_convC*fFact);
    Valore_Cop=(trq/9.81)/fFact;
    IO_ref=true;
    // Inserire comando coppia costante dyn3
    tmrTrq->Enabled=true;
  }
}
//---------------------------------------------------------------------------

