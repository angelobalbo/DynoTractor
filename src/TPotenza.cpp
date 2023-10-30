//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <IniFiles.hpp>
#include "TPotenza.h"
#include "TMain.h"
#include "CUtils.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ANALOGMETERHQLib_OCX"
#pragma resource "*.dfm"
TPotenza *Potenza;
extern String Profilo;
//---------------------------------------------------------------------------
__fastcall TPotenza::TPotenza(TComponent* Owner)
        : TForm(Owner)
{
AnalogMeterHQ1->SetLicense(186111193);
 // apertura del file .ini e acquisizione impostazioni salvate o predefinite
   TIniFile *ini;
   ini = new TIniFile(
   utils.GetProgramDataName(".ini") );
AnalogMeterHQ1->MaximumValue = ini->ReadInteger ( "Potenza"+Profilo, "KW", 200 );
  delete ini;



 Potenza->AnalogMeterHQ1->GreenStartValue=0;
 Potenza->AnalogMeterHQ1->YellowStartValue=AnalogMeterHQ1->MaximumValue;
 Potenza->AnalogMeterHQ1->RedStartValue=AnalogMeterHQ1->MaximumValue;


}
//---------------------------------------------------------------------------
void __fastcall TPotenza::FormResize(TObject *Sender)
{
// ridimensionamento caratteri
  AnalogMeterHQ1->BottomCoverLabelFont->Size=int(10*Potenza->ClientWidth/100);
  AnalogMeterHQ1->SmallScaleFont->Size=int(4*Potenza->ClientWidth/100);
  StaticText1->Font->Size=int(4*Potenza->ClientWidth/100);
}
//---------------------------------------------------------------------------

void __fastcall TPotenza::FormShow(TObject *Sender)
{
 KeyPreview = true; // attiva lettura da tastiera        
}
//---------------------------------------------------------------------------

void __fastcall TPotenza::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
 Main->Remote(Key);        
}
//---------------------------------------------------------------------------

