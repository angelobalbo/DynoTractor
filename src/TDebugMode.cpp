//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TDebugMode.h"
#include "CDyn3.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TeePageNumTool"
#pragma link "TeeTools"
#pragma resource "*.dfm"
TfrmDebugMode *frmDebugMode;
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

