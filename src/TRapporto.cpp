//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TRapporto.h"
#include "TStart.h"
#include "TMain.h"
#include "TStep_test.h"
#include "TImpostazioni.h"
#include "Structs.h"
#pragma package(smart_init)
#pragma link "CGAUGES"
#pragma resource "*.dfm"
TRapporto *Rapporto;

 extern int Reg_calc_rap,
            tipoRapp,
            Tipo_test;

 extern float Rap_tot1;

 extern long lRpm;

 extern bool Flag_ok_rap;

 extern String Text45,Text46;



//---------------------------------------------------------------------------
__fastcall TRapporto::TRapporto(TComponent* Owner)
        : TForm(Owner)
{



}

//---------------------------------------------------------------------------
void __fastcall TRapporto::BitBtn1Click(TObject *Sender)
{
  if(lRpm)
  {
    Rap_tot1=Reg_calc_rap/float(lRpm);
    Impostazioni->NxNumberEdit1->Value=Rap_tot1;
    Close();
  }
  else
    ShowMessage(Text46);
}

//---------------------------------------------------------------------------
void __fastcall TRapporto::BitBtn2Click(TObject *Sender)
{
 Main-> SpeedButton2->Enabled=true;
 Close();
}
//---------------------------------------------------------------------------

void __fastcall TRapporto::FormActivate(TObject *Sender)
{
  // contiene il tipo di calcolo del rapporto totale
  // 0 -> Inserimento regime di calcolo del rapporto
  // 2 -> Inserimento manuale del rapporto
  if(tipoRapp==0)
  {
    Label2->Caption=Reg_calc_rap;
    BitBtn1->Visible=true;
    Label1->Caption=Text45;
    Label3->Visible=false;
  }
}
//---------------------------------------------------------------------------



void __fastcall TRapporto::Timer1Timer(TObject *Sender)
{
  switch(Tipo_test)
  {
    case TEST_STEP_TIMED:
    case TEST_STEP:
    case TEST_DECEL:
      Step_test->Show();
    break;
  }
  Timer1->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall TRapporto::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  Main->Remote(Key);
}
//---------------------------------------------------------------------------

void __fastcall TRapporto::FormShow(TObject *Sender)
{

KeyPreview = true; // attiva lettura da tastiera
//BitBtn1->SetFocus(); 
Main->SpeedButton8->Enabled=false;
Main->SpeedButton15->Enabled=false;
Main->SpeedButton17->Enabled=false;
Main->SpeedButton18->Enabled=false;
Main->SpeedButton4->Enabled=false;
Main->SpeedButton1->Enabled=false;
Main->SpeedButton2->Enabled=false;
Main->SpeedButton21->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall TRapporto::FormClose(TObject *Sender, TCloseAction &Action)
{
Main->SpeedButton2->Enabled=true;
Main->SpeedButton8->Enabled=true;
Main->SpeedButton15->Enabled=true;
Main->SpeedButton17->Enabled=true;
Main->SpeedButton18->Enabled=true;
Main->SpeedButton4->Enabled=true;
Main->SpeedButton1->Enabled=true;
Main->SpeedButton21->Enabled=true;
}
//---------------------------------------------------------------------------

