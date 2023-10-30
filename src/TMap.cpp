//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <IniFiles.hpp>
#include "TMap.h"
#include "TMain.h"
#include "CUtils.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ANALOGMETERHQLib_OCX"
#pragma resource "*.dfm"
TMap *Map;

 extern String Profilo;


//---------------------------------------------------------------------------
__fastcall TMap::TMap(TComponent* Owner)
        : TForm(Owner)
{
AnalogMeterHQ1->SetLicense(186111193);
 // apertura del file .ini e acquisizione impostazioni salvate o predefinite
  TIniFile *ini;

  ini = new TIniFile(utils.GetProgramDataName(".ini"));
  AnalogMeterHQ1-> MinimumValue = ini-> ReadInteger( "Map"+Profilo, "Press_min", 0 );
  AnalogMeterHQ1-> MaximumValue = ini-> ReadInteger( "Map"+Profilo, "Press_max", 1800 );
  AnalogMeterHQ1->GreenStartValue=ini->ReadInteger( "Map"+Profilo, "Press_verde", 0 );
  AnalogMeterHQ1->YellowStartValue=ini->ReadInteger( "Map"+Profilo, "Press_giallo", 900 );
  AnalogMeterHQ1->RedStartValue = ini->ReadInteger ( "Map"+Profilo, "Press_rosso", 1400 );
  delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TMap::FormResize(TObject *Sender)
{
// ridimensionamento caratteri
  AnalogMeterHQ1->BottomCoverLabelFont->Size=int(10*Map->ClientWidth/100);
  AnalogMeterHQ1->SmallScaleFont->Size=int(4*Map->ClientWidth/100);
  StaticText1->Font->Size=int(4*Map->ClientWidth/100);
}
//---------------------------------------------------------------------------

void __fastcall TMap::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
 Main->Remote(Key);        
}
//---------------------------------------------------------------------------

void __fastcall TMap::FormShow(TObject *Sender)
{
 KeyPreview = true; // attiva lettura da tastiera
}
//---------------------------------------------------------------------------

