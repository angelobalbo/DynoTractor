//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <IniFiles.hpp>
#include "TTermo.h"
#include "TMain.h"
#include "CUtils.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ANALOGMETERHQLib_OCX"
#pragma resource "*.dfm"
TTermo *Termo;

extern String Profilo;

//---------------------------------------------------------------------------
__fastcall TTermo::TTermo(TComponent* Owner)
        : TForm(Owner)
{
 AnalogMeterHQ1->SetLicense(186111193);
 // apertura del file .ini e acquisizione impostazioni salvate o predefinite
   TIniFile *ini;
   ini = new TIniFile(
   utils.GetProgramDataName(".ini") );
  AnalogMeterHQ1-> MaximumValue = ini-> ReadInteger( "Termocoppia"+Profilo, "Valore_max", 1000 );
  AnalogMeterHQ1->GreenStartValue=ini->ReadInteger( "Termocoppia"+Profilo, "Range_verde", 0 );
  AnalogMeterHQ1->YellowStartValue=ini->ReadInteger( "Termocoppia"+Profilo, "Range_giallo", 750 );
  AnalogMeterHQ1->RedStartValue = ini->ReadInteger ( "Termocoppia"+Profilo, "Range_rosso", 850 );

  delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TTermo::FormResize(TObject *Sender)
{
 // ridimensionamento caratteri
  AnalogMeterHQ1->BottomCoverLabelFont->Size=int(10*Termo->ClientWidth/100);
  AnalogMeterHQ1->SmallScaleFont->Size=int(4*Termo->ClientWidth/100);
  StaticText1->Font->Size=int(4*Termo->ClientWidth/100);
}
//---------------------------------------------------------------------------

void __fastcall TTermo::FormShow(TObject *Sender)
{
 KeyPreview = true; // attiva lettura da tastiera
}
//---------------------------------------------------------------------------

void __fastcall TTermo::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
 Main->Remote(Key);        
}
//---------------------------------------------------------------------------

