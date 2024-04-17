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


#define REDUNDANCY 4

TfrmCan *frmCan;

extern int targetStep,Tipo_test;
extern bool bAvvioPressed;

extern bool OK_N_cost;
extern char g_cFileLog[MAX_PATH];
extern unsigned int selPto;
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
static long timing=0,t=0;//,timingTrq=0;
static long clockPrec,clockPrecTrq;
static int  step=0,stepTrq=0;
static float rpm1=0,rpm0;
float rpm;
static float trq1=0,trq0,trq=0;
long tmrInterval,trqInterval,kInterval=0;
bool jumpStep=false;
bool bRpmTracStarted=false;
bool bRTDataStarted=false;
unsigned char cCanRpmData[8];
bool bCanRpmErr=false;
long lId1,lId2;
#define MAX_TAB_STEP 10000
int nCycle=1;
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

  canStatus canStat;
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


  if(connectCAN(lIdReqCan,lId1,lId2,lTimeRateCan,cbCanRate->ItemIndex))
  {
    s="Connessione Can bus Kvaser";
    LogError(g_cFileLog,LOG_INFO,s.c_str());
    imgStatus->Visible=true;
    imgStatusStart->Visible=false;
    imgStatusStop->Visible=false;
    imgStatusErr->Visible=false;
    tmrRate->Enabled=false;

    //bTimerRateStarted=false;
    //bTimerRpmStarted=false;
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
    bRTDataStarted=true;
    //
//    bTimerRateStarted=false;
    sscanf(meCanId->Text.c_str(), "%x", &lIdReqCan);
    sscanf(MaskEdit1->Text.c_str(), "%x", &lId1);
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
  static bool res;
//  static canStatus canStat;
  tmrRate->Enabled=false;
  if(btnStop->Tag==1)
  {
    statusCan=EConnCAN;
    bRTDataStarted=true;
    btnStop->Tag=0;
    imgStatusStop->Visible=true;
    imgStatusStart->Visible=false;
    imgStatusErr->Visible=false;
    btnStart->Enabled=true;
    btnStop->Enabled=false;
    BitBtn3->Enabled=false;
//    bTimerRateStarted=false;
//    canStat=canObjBufDisable(hChannel,bufIdx1);
//    canStat=canObjBufDisable(hChannel,bufIdx2);
    return;
  }
  if(statusCan!=EDisc)// || statusCan==EReadyCAN)
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
    //if(OK_N_cost) // invia
//    memcpy(msg1,cCanData,sizeof(msg1));
    /*if(*/txCan(lIdReqCan,cCanData);//)
    {
      if(selPto==0)
        torque=(long)(trq*Rap_tot1);
      else
        torque=(long)trq;
      rpm=(long)targetStep;
        cCanData[0]=torque>>8;
        cCanData[1]=(torque)&0xFF;
        if((Tipo_test==TEST_STEP_TIMED && bAvvioPressed) || (Tipo_test==TEST_STEP))
        {
          cCanData[2]=(rpm)>>8;
          cCanData[3]=(rpm)&0xFF;
        }
        else
        {
          cCanData[2]=0;
          cCanData[3]=0;
        }
        cCanData[4]=0xFF;
        cCanData[5]=0xFF;
        cCanData[6]=0xFF;
        cCanData[7]=0xFF;
        //memcpy(msg2,cCanData,sizeof(msg2));
        res=txCan(lId1,cCanData);


    }

    if(res==false && statusCan==EErrCAN)
    {
       imgStatusStart->Visible=false;
       imgStatusErr->Visible=true;
       lblRes->Caption="ERROR CAN";
       btnStart->Enabled=false;
       btnStop->Enabled=false;
       BitBtn3->Enabled=true;
    }

    tmrRate->Enabled=true;
  }

}
//---------------------------------------------------------------------------

void __fastcall TfrmCan::btnStopClick(TObject *Sender)
{
  btnStop->Tag=1;
  //DestroyCan();
}
//---------------------------------------------------------------------------

void __fastcall TfrmCan::FormClose(TObject *Sender, TCloseAction &Action)
{
  if(tmrRate->Enabled==true)
    btnStopClick(this);
  Sleep(100);
  if(sbCmd->Caption=="STOP TEST")
    sbCmdClick(this);
  Sleep(200);
  if(statusCan!=EDisc)
  {

    closeCan();
  }
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
    bRTDataStarted=false;
    bRpmTracStarted=false;
  }
  tmrCmd->Interval=tmrInterval;


}
//---------------------------------------------------------------------------

void __fastcall TfrmCan::sbCmdClick(TObject *Sender)
{
  canStatus canStat;
//
  if(bRpmTracStarted==false)//sbCmd->Caption=="START TEST")
  {
    // ** START TEST **
    Tipo_test=TEST_COST_TRQ;
    srsRpmRt->Clear();
    srsTrqRt->Clear();
    t=0;
    kInterval=0;
    jumpStep=false;
    butSaltaStep->Enabled=true;
    timing=tabStep.time[0]*1000;
//    timingTrq=tabStep.time[0]*1000;
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
    sscanf(MaskEdit2->Text.c_str(), "%x", &lId2);
    memset(cCanRpmData,0,sizeof(cCanRpmData));

    sbCmd->Caption="STOP TEST";
    bRpmTracStarted=true;
    CreateCan(g_cCanLog);
    Valore_Cop=0;
    OK_N_cost=true;
    SpeedButton1->Enabled=false;
    tmrCmd->Enabled=true;
  }
  else // STOP INVIO DATI AL TRACTOR
  {
    tmrCmd->Enabled=false;
    sbCmd->Caption="START TEST";
    bRpmTracStarted=false;
    DestroyCan();
    Main->EndTest();
    Valore_Cop=0;
    trq=0;
    OK_N_cost=false;
    SpeedButton1->Enabled=true;
    butSaltaStep->Enabled=false;
    jumpStep=false;
//    canStat=canObjBufDisable(hChannel,bufIdxRpmTrac);
//    if(canStat!=canOK) LogError(g_cFileLog,LOG_ERR,"canObjBufDisable IdRpmTrac");
  }

}
//---------------------------------------------------------------------------


void __fastcall TfrmCan::tmrCmdTimer(TObject *Sender)
{

  static unsigned char cCanData[8];
  static float deltaRpm;
  static float deltaTrq;

  static float interval;
//  static canStatus canStat;

  tmrCmd->Enabled=false;

  // Condizione di avanzamento step, o step iniziale


  interval=(1000*(clock()-clockPrec)/CLK_TCK);
  timing+=interval;
  clockPrec=clock();

  if((/*(rpm0==rpm1) &&*/ (timing>=(tabStep.time[step])*1000) || jumpStep) /*||
     (rpm1>rpm0 && rpm>=rpm1) ||
     (rpm1<rpm0 && rpm<=rpm1)*/)
  {
    rpm0=tabStep.rpm[step];    // rpm iniziale
    trq0=tabStep.trq[step];
         if(jumpStep)
      {
        jumpStep=false;
        kInterval=(1000*tabStep.time[step])-timing;
        timing+=kInterval;
      }

    //timing=tabStep.time[step]*1000; // tempo iniziale ms

    step++;
    if(step<tabStep.rows)
    {
      rpm1=tabStep.rpm[step];    // rpm finale
      trq1=tabStep.trq[step];
      rpm=rpm0; // Valore attuale da imporre risulta il valore iniziale
      trq=trq0;
      //clockPrec=clock();
    }
    else // condizione di fine prova
    {
      sbCmd->Caption="START TEST";
      bRpmTracStarted=false;
      DestroyCan();
      SpeedButton1->Enabled=true;
      Main->EndTest();
      OK_N_cost=false;
      Valore_Cop=0;
      if(nCycle > 1)
      {
        nCycle--;
        edRepeat->Text=nCycle;
        sbCmdClick(this);
      }

    }

  }
  else
  {

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
     //CASO 1: mantenimento trq1=trq0, tengo il tempo
     if(trq1==trq0)
     {
      trq=trq1;

     }
     //CASO 2: rampa in salita o discesa
     else
     {
        deltaTrq=(trq1-trq)/((float)(tabStep.time[step]*1000-timing+interval)/(float)interval);
        trq+=deltaTrq;
        if      (trq1>trq0 && trq>trq1) trq=trq1;
        else if (trq1>trq0 && trq<trq0) trq=trq0;
        else if (trq1<trq0 && trq<trq1) trq=trq1;
        else if (trq1<trq0 && trq>trq0) trq=trq0;
     }
     //timing=timing+interval;

     //clockPrec=clock();
  }

  if(bRpmTracStarted==true)//sbCmd->Caption=="STOP TEST")
  {

    if(statusCan!=EDisc)// || statusCan==EReadyCAN)
    {
      //      id=0x0C000027;
      if(bCanRpmErr==true)
      {
        Label6->Font->Color=clRed;
        Label6->Caption="Tractor CAN FAILED";
      }
      else
      {
        Label6->Font->Color=clBlack;
        Label6->Caption="Tractor CAN OK";
      }
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
        memcpy(cCanRpmData,cCanData,sizeof(cCanRpmData));

        #if 0
        if(txCan(lId2,cCanRpmData)==false)
          lblRes->Caption="KO";
        else
          lblRes->Caption="OK";
        #endif
      }
    }

    srsRpmRt->AddXY((float)timing/1000,(lRpm*Rap_tot1));
//    srsRpmRt->AddXY((float)timing/1000,(rpm));
    chart->BottomAxis->Title->Caption="T = "+FormatFloat("#.00",(float)timing/1000)+"s";
    if((1000*(clock()-clockPrecTrq)/CLK_TCK)>trqInterval)
    {
      clockPrecTrq=clock();
      if(selPto == 0)
        Valore_Cop=(trq*Rap_tot1/9.81)/fFact;
      else
        Valore_Cop=(trq/9.81)/fFact;

      IO_ref=true;
      srsTrqRt->AddXY((float)timing/1000,Val_coppia_m1 * fK_convC*fFact);
      //srsTrqRt->AddXY((float)timing/*Trq*//1000,trq*fK_convC/9.81);
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
        memset (&tabStep,0,sizeof(TAB_STEP_CAN));
        chart->SubTitle->Caption=ExtractFileName(sTmpFileName);
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
        // Grafico valori tabulati
        srsRpm->Clear();
        srsTrq->Clear();
        srsTrqRt->Clear();
        srsRpmRt->Clear();
        for (i=0;i<tabStep.rows;i++)
        {
          srsRpm->AddXY((float)tabStep.time[i],tabStep.rpm[i]);
          srsTrq->AddXY((float)tabStep.time[i],tabStep.trq[i]);
        }
        if((col<3 && col >0) || i==0)
          MessageBox(NULL,"Table is not correct!","ERROR",MB_OK | MB_ICONERROR);
        else
          sbCmd->Enabled=true;

      } // if infile...
    } // if((sTmpFileName..
  } // if(OpenDialog1->Execute())

}
//---------------------------------------------------------------------------


void __fastcall TfrmCan::MaskEdit1Change(TObject *Sender)
{
  sscanf(MaskEdit1->Text.c_str(), "%x", &lId1);
}
//---------------------------------------------------------------------------

void __fastcall TfrmCan::MaskEdit2Change(TObject *Sender)
{
  sscanf(MaskEdit2->Text.c_str(), "%x", &lId2);
}
//---------------------------------------------------------------------------


void __fastcall TfrmCan::edRepeatChange(TObject *Sender)
{
  nCycle=StrToIntDef(edRepeat->Text,1);
  edRepeat->Text=nCycle;
}
//---------------------------------------------------------------------------

void __fastcall TfrmCan::butSaltaStepClick(TObject *Sender)
{
  jumpStep=true;  
}
//---------------------------------------------------------------------------

