//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <IniFiles.hpp>
#include "TCoppia.h"
#include "TMain.h"
#include "CUtils.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ANALOGMETERHQLib_OCX"
#pragma resource "*.dfm"
TCoppia *Coppia;

extern String Profilo;

//---------------------------------------------------------------------------
__fastcall TCoppia::TCoppia(TComponent* Owner)
        : TForm(Owner)
{
 AnalogMeterHQ1->SetLicense(186111193);

 // apertura del file .ini e acquisizione impostazioni salvate o predefinite
   TIniFile *ini;
   ini = new TIniFile(utils.GetProgramDataName(".ini") );
   AnalogMeterHQ1->MaximumValue = ini->ReadInteger ( "Coppia"+Profilo, "Kgfm", 40 );
   delete ini;

  AnalogMeterHQ1->GreenStartValue=0;
  AnalogMeterHQ1->YellowStartValue=AnalogMeterHQ1->MaximumValue;
  AnalogMeterHQ1->RedStartValue=AnalogMeterHQ1->MaximumValue;




}
//---------------------------------------------------------------------------
void __fastcall TCoppia::FormResize(TObject *Sender)
{
  // ridimensionamento caratteri
  AnalogMeterHQ1->BottomCoverLabelFont->Size=int(10*Coppia->ClientWidth/100);
  AnalogMeterHQ1->SmallScaleFont->Size=int(4*Coppia->ClientWidth/100);
  StaticText1->Font->Size=int(4*Coppia->ClientWidth/100);
}
//---------------------------------------------------------------------------

void __fastcall TCoppia::FormShow(TObject *Sender)
{
 KeyPreview = true; // attiva lettura da tastiera        
}
//---------------------------------------------------------------------------

void __fastcall TCoppia::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  Main->Remote(Key);        
}
//---------------------------------------------------------------------------

