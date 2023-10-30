//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include<fstream.h>

#include "TPilot_Freno.h"
#include "TMain.h"
#include "Structs.h"
#include "CDyn3.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "VrControls"
#pragma link "VrWheel"
#pragma link "VrProgressBar"
#pragma link "VrTrackBar"
#pragma link "ANALOGMETERHQLib_OCX"
#pragma resource "*.dfm"
TPilot_Freno *Pilot_Freno;
unsigned short Temp_ValPwm=0;

extern unsigned short usPwmWrite,usPwmAvg;

extern int tipoRapp;
//---------------------------------------------------------------------------
__fastcall TPilot_Freno::TPilot_Freno(TComponent* Owner)
        : TForm(Owner)
{
  AnalogMeterHQ1->SetLicense(186111193);
}
//---------------------------------------------------------------------------
void __fastcall TPilot_Freno::VrTrackBar1Change(TObject *Sender)
{
  usPwmWrite=VrTrackBar1->Position;
  g_brakeControl.CmdBrakeMan(usPwmWrite,CDyn3::EFrontAxle);
  if(g_brakeControl.GetFwVer()<5)
  {
    AnalogMeterHQ1->Value=((float)usPwmWrite/254)*100;
    AnalogMeterHQ1->Caption=FormatFloat("0.#",((float)usPwmWrite/254)*100);
  }
  else
  {
    AnalogMeterHQ1->Value=((float)usPwmWrite/4095)*100;
    AnalogMeterHQ1->Caption=FormatFloat("0.#",((float)usPwmWrite/4095)*100);
  }

  BitBtn2->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TPilot_Freno::FormShow(TObject *Sender)
{
  KeyPreview = true; // attiva lettura da tastiera
  usPwmWrite=0;
  VrTrackBar1->MaxValue=(g_brakeControl.GetFwVer()<5)?254:4095;
  VrTrackBar1->Position=0;
  AnalogMeterHQ1->Value=0;
  BitBtn1->Enabled=false;
  BitBtn2->Enabled=false;
  Main->SpeedButton8->Enabled=false;
  Main->SpeedButton15->Enabled=false;
  Main->SpeedButton17->Enabled=false;
  Main->SpeedButton18->Enabled=false;
  Main->SpeedButton4->Enabled=false;
  Main->SpeedButton1->Enabled=false;
  Main->SpeedButton2->Enabled=false;
  Main->SpeedButton21->Enabled=false;
  Main->Sequenzaparametriditest1->Enabled=false;
  VrTrackBar1->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TPilot_Freno::BitBtn2Click(TObject *Sender)
{
  Temp_ValPwm=VrTrackBar1->Position;
  usPwmWrite=0;
  VrTrackBar1->Position=0;
  AnalogMeterHQ1->Value=0;
  g_brakeControl.CmdStop();
  BitBtn1->Enabled=true;
  BitBtn2->Enabled=false;
  BitBtn1->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TPilot_Freno::FormClose(TObject *Sender,
      TCloseAction &Action)
{

  usPwmAvg=usPwmWrite;
  Main->EndTest();
  //g_brakeControl.CmdStop();
  Main->SpeedButton2->Enabled=true;
  Main->SpeedButton8->Enabled=true;
  Main->SpeedButton15->Enabled=true;
  Main->SpeedButton17->Enabled=true;
  Main->SpeedButton18->Enabled=true;
  Main->SpeedButton4->Enabled=true;
  Main->SpeedButton21->Enabled=true;
  Main->Sequenzaparametriditest1->Enabled=true;
  if(tipoRapp==RAP_RPMFIX)
    Main->SpeedButton1->Enabled=true;
  Main->SpeedButton2->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TPilot_Freno::BitBtn1Click(TObject *Sender)
{
  usPwmWrite=Temp_ValPwm;
  if(g_brakeControl.GetFwVer()<5)
  {
    AnalogMeterHQ1->Value=((float)usPwmWrite/254)*100;
    AnalogMeterHQ1->Caption=FormatFloat("0.#",((float)usPwmWrite/254)*100);
  }
  else
  {
    AnalogMeterHQ1->Value=((float)usPwmWrite/4095)*100;
    AnalogMeterHQ1->Caption=FormatFloat("0.#",((float)usPwmWrite/4095)*100);
  }
  VrTrackBar1->Position=usPwmWrite;
  g_brakeControl.CmdBrakeMan(usPwmWrite,CDyn3::EFrontAxle);
}
//---------------------------------------------------------------------------


void __fastcall TPilot_Freno::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  Main->Remote(Key);
}
//---------------------------------------------------------------------------

void __fastcall TPilot_Freno::PrintStepClick(TObject *Sender)
{
    Main->TabStepPrint->Enabled = true;
    Main->AddTabAcq();
}
//---------------------------------------------------------------------------

void __fastcall TPilot_Freno::ResetTabStepClick(TObject *Sender)
{
  Main->ResetTabAcq();
}
//---------------------------------------------------------------------------

