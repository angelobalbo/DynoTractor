//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <IniFiles.hpp>
#include "TGiri_Mot.h"
#include "TMain.h"
#include "CUtils.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ANALOGMETERHQLib_OCX"

#pragma resource "*.dfm"
TGiri_Mot *Giri_Mot;

extern String Profilo;
extern int  Reg_iniz_acc;
extern String Str0;

//---------------------------------------------------------------------------
__fastcall TGiri_Mot::TGiri_Mot(TComponent* Owner)
        : TForm(Owner)
{
AnalogMeterHQ1->SetLicense(186111193);


 // apertura del file .ini e acquisizione impostazioni salvate o predefinite
   TIniFile *ini;
   ini = new TIniFile(utils.GetProgramDataName(".ini") );
   AnalogMeterHQ1-> MaximumValue = ini-> ReadInteger( "Regime_max"+Profilo, "Giri/min", 3000 );
   AnalogMeterHQ1->YellowStartValue=ini->ReadInteger( "Regime_max"+Profilo, "Giri_giallo", 2000 );
   AnalogMeterHQ1->RedStartValue = ini->ReadInteger ( "Regime_max"+Profilo, "Giri_rosso", 2500 );
   delete ini;




}
//---------------------------------------------------------------------------
void __fastcall TGiri_Mot::FormActivate(TObject *Sender)
{
 AnalogMeterHQ1->GreenStartValue=Reg_iniz_acc;
 StaticText1->Caption = "rpm";//Str0;
}
//---------------------------------------------------------------------------

void __fastcall TGiri_Mot::FormResize(TObject *Sender)
{

  // ridimensionamento caratteri
  AnalogMeterHQ1->BottomCoverLabelFont->Size=int(10*Giri_Mot->ClientWidth/100);
  AnalogMeterHQ1->SmallScaleFont->Size=int(4*Giri_Mot->ClientWidth/100);
  StaticText1->Font->Size=int(4*Giri_Mot->ClientWidth/100);
}
//---------------------------------------------------------------------------

void __fastcall TGiri_Mot::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  Main->Remote(Key);        
}
//---------------------------------------------------------------------------

void __fastcall TGiri_Mot::FormShow(TObject *Sender)
{
 KeyPreview = true; // attiva lettura da tastiera
}
//---------------------------------------------------------------------------

