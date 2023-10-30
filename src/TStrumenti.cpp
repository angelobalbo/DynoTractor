//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "TStrumenti.h"
#include<stdio.h>
#include<fstream.h>
#include "TGiri_Mot.h"
#include "TCoppia.h"
#include "TPotenza.h"
#include "TMap.h"
#include "TTermo.h"
#include "TAcc.h"
#include "TMain.h"
#include "TOil_Temp.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "NxCollection"
#pragma link "NxPageControl"
#pragma link "TeeDragPoint"
#pragma link "TeeLighting"
#pragma link "TeeTools"
#pragma resource "*.dfm"
TStrumenti *Strumenti;

  extern   struct UM_LIMIT limit;
 extern short Intero_strum,Chec_singola,Unit_Tem,Imp_Ifut,sTipo_dato;
 extern float Tens_Ad_fut,Tens_Ad_fut_min,Tens_Ad_fut_max,Pmap,K_R,fK_convPr;
 extern char  Buffer[100],Y_TAB[20];
 extern bool Allarme, bMapType;
// extern double CUSTOM_TAB[256];

  double   /*CUSTOM_TAB_temp[256],*/  // tabelle temporanee
           dval_step0=0,    // valore contenuto in step (0)
           dval_step1=0,    // valore contenuto in step (255)
           dval_m    =0;    // coefficiente della retta  (m)





     String SSA;     //nome file sett. selezionato

   unsigned char uvalue[20];
   int  iStep = 1;

  bool OK_mod=false; 
  extern String Str25,Str1,Str2,Str3,Str4,Text26,Text28,Text29,Text30;

  //---------------------------------------------------------------------------
__fastcall TStrumenti::TStrumenti(TComponent* Owner)
        : TForm(Owner)
{




}
//---------------------------------------------------------------------------




void __fastcall TStrumenti::UpDown4Click(TObject *Sender,
      TUDBtnType Button)
{
Giri_Mot->AnalogMeterHQ1->MaximumValue=UpDown4->Position;
}
//---------------------------------------------------------------------------

void __fastcall TStrumenti::UpDown2Click(TObject *Sender,
      TUDBtnType Button)
{
Giri_Mot->AnalogMeterHQ1->YellowStartValue=UpDown2->Position;
}
//---------------------------------------------------------------------------

void __fastcall TStrumenti::UpDown3Click(TObject *Sender,
      TUDBtnType Button)
{
Giri_Mot->AnalogMeterHQ1->RedStartValue=UpDown3->Position;
}
//---------------------------------------------------------------------------



void __fastcall TStrumenti::UpDown7Click(TObject *Sender,
      TUDBtnType Button)
{
if(UpDown7->Position==5)Coppia->AnalogMeterHQ1->NumberOfDivisions=2;
else Coppia->AnalogMeterHQ1->NumberOfDivisions=6;

Coppia->AnalogMeterHQ1->MaximumValue=UpDown7->Position;
Coppia->AnalogMeterHQ1->RedStartValue=UpDown7->Position;
Coppia->AnalogMeterHQ1->GreenStartValue=0;
Coppia->AnalogMeterHQ1->YellowStartValue=UpDown7->Position;

}
//---------------------------------------------------------------------------

void __fastcall TStrumenti::UpDown10Click(TObject *Sender,
      TUDBtnType Button)
{
if(UpDown10->Position==5)Potenza->AnalogMeterHQ1->NumberOfDivisions=5;
else Potenza->AnalogMeterHQ1->NumberOfDivisions=5;

Potenza->AnalogMeterHQ1->MaximumValue=UpDown10->Position;
Potenza->AnalogMeterHQ1->RedStartValue=UpDown10->Position;
Potenza->AnalogMeterHQ1->YellowStartValue=UpDown10->Position;
Potenza->AnalogMeterHQ1->GreenStartValue=0;
}
//---------------------------------------------------------------------------


void __fastcall TStrumenti::UpDown12Click(TObject *Sender,
      TUDBtnType Button)
{
  limit.fYellowPress=(float)UpDown12->Position/fK_convPr;

}
//---------------------------------------------------------------------------


void __fastcall TStrumenti::UpDown16Click(TObject *Sender,
      TUDBtnType Button)
{
 Termo->AnalogMeterHQ1->MaximumValue=UpDown16->Position;
}
//---------------------------------------------------------------------------


void __fastcall TStrumenti::UpDown15Click(TObject *Sender,
      TUDBtnType Button)
{
 Termo->AnalogMeterHQ1->RedStartValue=UpDown15->Position;
}
//---------------------------------------------------------------------------





void __fastcall TStrumenti::FormCreate(TObject *Sender)
{

   // assegnazione dei settaggi strumenti ai menu opzione
  Strumenti->Align=alNone;
  //Strumenti->Height=485;
  //Strumenti->Width=310;

  // strumento regime motore
  UpDown4->Position = Giri_Mot->AnalogMeterHQ1->MaximumValue;
  UpDown2->Position = Giri_Mot->AnalogMeterHQ1->YellowStartValue;
  UpDown3->Position = Giri_Mot->AnalogMeterHQ1->RedStartValue;

  // strumento coppia lorda
  UpDown7->Position = Coppia->AnalogMeterHQ1->MaximumValue;


  // strumento potenza lorda
  UpDown10->Position = Potenza->AnalogMeterHQ1->MaximumValue;

  // strumento Map
  UpDown1->Position = Map->AnalogMeterHQ1->MinimumValue;
  UpDown13->Position = Map->AnalogMeterHQ1->MaximumValue;
  UpDown5->Position = Map->AnalogMeterHQ1->GreenStartValue;
  UpDown12->Position = Map->AnalogMeterHQ1->YellowStartValue;
  UpDown11->Position = Map->AnalogMeterHQ1->RedStartValue;


   // strumento termocoppia di tipo K
  UpDown16->Position = Termo->AnalogMeterHQ1->MaximumValue;
  UpDown6->Position = Termo->AnalogMeterHQ1->GreenStartValue;
  UpDown14->Position = Termo->AnalogMeterHQ1->YellowStartValue;
  UpDown15->Position = Termo->AnalogMeterHQ1->RedStartValue;


 
   // strumento olio motore

  UpDown18->Position = Oil_Temp->AnalogMeterHQ1->MinimumValue;
  UpDown19->Position = Oil_Temp->AnalogMeterHQ1->MaximumValue;
  UpDown8->Position = Oil_Temp->AnalogMeterHQ1->GreenStartValue;
  UpDown9->Position = Oil_Temp->AnalogMeterHQ1->YellowStartValue;
  UpDown17->Position = Oil_Temp->AnalogMeterHQ1->RedStartValue;

  KeyPreview = true;

}
//---------------------------------------------------------------------------





void __fastcall TStrumenti::FormClose(TObject *Sender,
      TCloseAction &Action)
{
 Timer1->Enabled=false; // disattiva time di visualizzazione valore Ifut
  Main->carica_unita();
 Close();
}
//---------------------------------------------------------------------------




void __fastcall TStrumenti::FormActivate(TObject *Sender)
{
//a.balbo delete
// sprintf(Buffer,"%-1.2f",Tens_Ad_fut_min);
// Label4->Caption=String(Buffer);
// sprintf(Buffer,"%-1.2f",Tens_Ad_fut_max);
// Label5->Caption=String(Buffer);

  UpDown5->Increment=50*fK_convPr;
  UpDown5->Min=-1000*fK_convPr;
  if(bMapType==false)
  {
    UpDown5->Max=3000*fK_convPr;
    UpDown12->Max=3000*fK_convPr;
    UpDown11->Max=3000*fK_convPr;
    UpDown13->Max=3000*fK_convPr;
    rbPressType0->Checked=true;
  }
  else
  {
    UpDown5->Max=6000*fK_convPr;
    UpDown12->Max=6000*fK_convPr;
    UpDown11->Max=6000*fK_convPr;
    UpDown13->Max=6000*fK_convPr;
    rbPressType1->Checked=true;
  }
  //giallo
  UpDown12->Increment=50*fK_convPr;
  UpDown12->Min=-600*fK_convPr;

  //rosso
  UpDown11->Increment=50*fK_convPr;
  UpDown11->Min=-400*fK_convPr;

  //min
  UpDown1->Increment=100*fK_convPr;
  UpDown1->Min=-1000*fK_convPr;
  UpDown1->Max=0;
  //max
  UpDown13->Increment=100*fK_convPr;
  UpDown13->Min=500*fK_convPr;


  //valori
  UpDown5->Position=(int)(limit.fGreenPress*fK_convPr);
  UpDown12->Position=(int)(limit.fYellowPress*fK_convPr);
  UpDown11->Position=(int)(limit.fRedPress*fK_convPr);
  UpDown1->Position=(int)(limit.fMinPress*fK_convPr);
  UpDown13->Position=(int)(limit.fMaxPress*fK_convPr);



}
//---------------------------------------------------------------------------





void __fastcall TStrumenti::UpDown1Click(TObject *Sender,
      TUDBtnType Button)
{
  limit.fMinPress=UpDown1->Position/fK_convPr;
}
//---------------------------------------------------------------------------

void __fastcall TStrumenti::UpDown13Click(TObject *Sender,
      TUDBtnType Button)
{
  limit.fMaxPress=(float)UpDown13->Position/fK_convPr;
}
//---------------------------------------------------------------------------

void __fastcall TStrumenti::UpDown11Click(TObject *Sender,
      TUDBtnType Button)
{
  limit.fRedPress=(float)UpDown11->Position/fK_convPr;
}
//---------------------------------------------------------------------------

void __fastcall TStrumenti::UpDown5Click(TObject *Sender,
      TUDBtnType Button)
{
 limit.fGreenPress=(float)UpDown5->Position/fK_convPr;
}
//---------------------------------------------------------------------------

void __fastcall TStrumenti::UpDown14Click(TObject *Sender,
      TUDBtnType Button)
{
 Termo->AnalogMeterHQ1->YellowStartValue=UpDown14->Position;
}
//---------------------------------------------------------------------------

void __fastcall TStrumenti::UpDown6Click(TObject *Sender,
      TUDBtnType Button)
{
 Termo->AnalogMeterHQ1->GreenStartValue=UpDown6->Position;        
}
//---------------------------------------------------------------------------


void __fastcall TStrumenti::SpeedButton3Click(TObject *Sender)
{
 K_R+=Pmap;
}
//---------------------------------------------------------------------------








void __fastcall TStrumenti::CheckBox7Click(TObject *Sender)
{

  if(CheckBox7->Checked)
  {
    Main->Panel20->Visible=true;
    Main->Panel18->Align=alLeft;
    Main->Splitter11->Align=alLeft;
    Main->Panel18->Width=184;
  }
  else
  {
    Main->Splitter11->Align=alRight;
    Main->Panel20->Visible=false;
    Main->Panel18->Align=alClient;
  }
  Main->OilLabelDblClick(this);
}
//---------------------------------------------------------------------------

void __fastcall TStrumenti::CheckBox8Click(TObject *Sender)
{


  if(CheckBox8->Checked)
  {
    Main->Panel18->Visible=true;
    Main->Splitter2->Align=alLeft;
  }
  else
  {
    Main->Panel18->Visible=false;
    Main->Panel20->Align=alClient;
    Main->Splitter2->Align=alRight;
  }
  Main->OilLabelDblClick(this);
}
//---------------------------------------------------------------------------


void __fastcall TStrumenti::CheckBox6Click(TObject *Sender)
{

  if(CheckBox6->Checked)
  {
    Main->Panel14->Visible=true;
    Main->Panel24->Align=alLeft;
    Main->Splitter9->Align=alLeft;
  }
  else
  {
    Main->Panel14->Visible=false;
    Main->Splitter9->Align=alRight;
  }
}
//---------------------------------------------------------------------------





void __fastcall TStrumenti::CheckBox1Click(TObject *Sender)
{
 if(CheckBox1->Checked)
 {
        Main->Splitter9->Align=alRight;
        Main->Panel24->Visible=false;
        Main->Panel6->Visible=true;
        Main->Panel14->Align=alLeft;
        Main->Splitter9->Align=alLeft;
        Main->Panel14->Width=184;

 }
 else
 {
        Main->Splitter9->Align=alRight;
        Main->Panel6->Visible=false;
        Main->Panel14->Align=alClient;
 }
}
//---------------------------------------------------------------------------



void __fastcall TStrumenti::CheckBox11Click(TObject *Sender)
{
 if(CheckBox11->Checked) Main->ToolBar5->Visible=false;

 else Main->ToolBar5->Visible=true;

}
//---------------------------------------------------------------------------

void __fastcall TStrumenti::FormShow(TObject *Sender)
{
  if(Main->ToolBar5->Visible) CheckBox11->Checked=false;
  else CheckBox11->Checked=true;


  CheckBox12->Checked = Allarme;

  Timer1->Enabled=true; // attiva time di visualizzazione valore Ifut

   ComboBox1->Items->Clear();
   ComboBox1->Items->Add(Str3);
   ComboBox1->Items->Add(Str4);
   ComboBox1->Items->Add(Str1);
   ComboBox1->Items->Add(Str2);
   ComboBox1->Items->Add(Text26);
   ComboBox1->Items->Add(Text29);
   ComboBox1->Items->Add(Text28);
   ComboBox1->Items->Add(Text30);

   ComboBox1->ItemIndex = sTipo_dato;


}
//---------------------------------------------------------------------------



void __fastcall TStrumenti::CheckBox12Click(TObject *Sender)
{
 if(CheckBox12->Checked)
 {
  Main->Timer_Allarm->Enabled=true;
  Allarme = true;
 }

      else
      {
       Main->Timer_Allarm->Enabled=false;
       Main->GiriLabel->Color=clWhite;
       Main->MapLabel->Color=clWhite;
       Main->TKLabel->Color=clWhite;
       Main->OilLabel->Color=clWhite;
       Allarme = false;
      }


}
//---------------------------------------------------------------------------

void __fastcall TStrumenti::UpDown8Click(TObject *Sender,
      TUDBtnType Button)
{
 Oil_Temp->AnalogMeterHQ1->GreenStartValue=UpDown8->Position;
}
//---------------------------------------------------------------------------

void __fastcall TStrumenti::UpDown9Click(TObject *Sender,
      TUDBtnType Button)
{
 Oil_Temp->AnalogMeterHQ1->YellowStartValue=UpDown9->Position;          
}
//---------------------------------------------------------------------------

void __fastcall TStrumenti::UpDown17Click(TObject *Sender,
      TUDBtnType Button)
{
 Oil_Temp->AnalogMeterHQ1->RedStartValue=UpDown17->Position;
}
//---------------------------------------------------------------------------

void __fastcall TStrumenti::UpDown18Click(TObject *Sender,
      TUDBtnType Button)
{


if(  Unit_Tem == 0)
   {
   UpDown19->Max=100;
   UpDown19->Min=0;
   }

      if( Unit_Tem == 1)
      {
      UpDown19->Min=32;
      UpDown19->Max=212;
      }

 Oil_Temp->AnalogMeterHQ1->MinimumValue=UpDown18->Position;
}
//---------------------------------------------------------------------------

void __fastcall TStrumenti::UpDown19Click(TObject *Sender,
      TUDBtnType Button)
{

 if(  Unit_Tem == 0)
 {
 UpDown19->Max=150;
 UpDown19->Min=100;
 }

    if(  Unit_Tem == 1)
      {
      UpDown19->Max=302;
      UpDown19->Min=212;
      }

 Oil_Temp->AnalogMeterHQ1->MaximumValue=UpDown19->Position;




 }
//---------------------------------------------------------------------------


void __fastcall TStrumenti::ComboBox1Change(TObject *Sender)
{
  sTipo_dato=ComboBox1->ItemIndex;
}
//---------------------------------------------------------------------------


void __fastcall TStrumenti::cbTAirClick(TObject *Sender)
{
  Main->Panel12->Visible=(cbTAir->Checked);
  Main->Splitter11->Align=(cbTAir->Checked)?alLeft:alRight;
  Main->OilLabelDblClick(this);
}
//---------------------------------------------------------------------------


void __fastcall TStrumenti::rbPressType0Click(TObject *Sender)
{
  if(bMapType==true)
  {
    bMapType=false;
    limit.fMaxPress=3000;
    UpDown13->Max=3000*fK_convPr;
    UpDown11->Max=3000*fK_convPr;
    UpDown5->Max=3000*fK_convPr;
    Map->AnalogMeterHQ1->MaximumValue=ceil(3000*fK_convPr);
  }


}
//---------------------------------------------------------------------------

void __fastcall TStrumenti::rbPressType1Click(TObject *Sender)
{

  if(bMapType==false)
  {
    bMapType=true;
    limit.fMaxPress=6000;
    UpDown13->Max=(6000*fK_convPr);

    UpDown5->Max =(6000*fK_convPr);
    UpDown11->Max=(6000*fK_convPr);
    Map->AnalogMeterHQ1->MaximumValue=(6000*fK_convPr);
  }
    UpDown13->Position=(int)(limit.fMaxPress*fK_convPr);
}
//---------------------------------------------------------------------------


