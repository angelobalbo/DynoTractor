//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TOil_Temp.h"
#include "TMain.h"
#include "CUtils.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ANALOGMETERHQLib_OCX"
#include <IniFiles.hpp>
#pragma resource "*.dfm"
TOil_Temp *Oil_Temp;

  extern String Profilo;

//---------------------------------------------------------------------------
__fastcall TOil_Temp::TOil_Temp(TComponent* Owner)
        : TForm(Owner)
{
 AnalogMeterHQ1->SetLicense(186111193);
  // apertura del file .ini e acquisizione impostazioni salvate o predefinite
   TIniFile *ini;
   ini = new TIniFile(utils.GetProgramDataName(".ini"));
  AnalogMeterHQ1-> MinimumValue = ini-> ReadInteger( "Oil"+Profilo, "Temp_min", 0 );
  AnalogMeterHQ1-> MaximumValue = ini-> ReadInteger( "Oil"+Profilo, "Temp_max", 150 );
  AnalogMeterHQ1->GreenStartValue=ini->ReadInteger( "Oil"+Profilo, "Temp_verde", 0 );
  AnalogMeterHQ1->YellowStartValue=ini->ReadInteger( "Oil"+Profilo, "Temp_giallo", 95 );
  AnalogMeterHQ1->RedStartValue = ini->ReadInteger ( "Oil"+Profilo, "Temp_rosso", 110 );
  delete ini;
}
//---------------------------------------------------------------------------

void __fastcall TOil_Temp::FormResize(TObject *Sender)
{
// ridimensionamento caratteri
  AnalogMeterHQ1->BottomCoverLabelFont->Size=int(10*Oil_Temp->ClientWidth/100);
  AnalogMeterHQ1->SmallScaleFont->Size=int(4*Oil_Temp->ClientWidth/100);
  StaticText1->Font->Size=int(4*Oil_Temp->ClientWidth/100);
}
//---------------------------------------------------------------------------
void __fastcall TOil_Temp::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
 Main->Remote(Key);
}
//---------------------------------------------------------------------------
void __fastcall TOil_Temp::FormShow(TObject *Sender)
{
 KeyPreview = true; // attiva lettura da tastiera
}
//---------------------------------------------------------------------------
