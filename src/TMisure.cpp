//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "TMisure.h"
#include "TMain.h"
#include "TCoppia.h"
#include "TPotenza.h"
#include "TTermo.h"
//#include "Unit16.h"
#include "TStrumenti.h"
#include "TMap.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMisure *Misure;



  extern short   Unit_Cop,Unit_Pot,Unit_Tem,Unit_Vel,
                Unit_Map,Vis_Air_Fuel,Unit_Pes,Unit_Sup,Unit_Cons,sTest_antep;
  extern int iPos;
  extern bool Out_dat,bMapType;


//---------------------------------------------------------------------------
__fastcall TMisure::TMisure(TComponent* Owner)
        : TForm(Owner)
{

 


}
//---------------------------------------------------------------------------







void __fastcall TMisure::BitBtn1Click(TObject *Sender)
{




 /*
    Unit_Cop = 0,     // variabile contenente unità di misura della coppia
                         // 0 = Newton.metro
                         // 1 = Kilogrammo forza.metro
                         // 2 = Pound-force.piede

     Unit_Pot = 0,     // variabile contenente unità di misura della potenza
                         // 0 = Kilowatt
                         // 1 = Cavallo Potenza (metric)
                         // 2 = Hors Power

       Unit_Tem = 0,     // variabile contenente unità di misura delle temperature
                         // 0 = Gradi Celsius
                         // 1 = Fahrenheit


       Unit_Vel = 0,     // variabile contenente unità di misura della velocità
                         // 0 = Kilometri per ora
                         // 1 = miglio per ora
                         // 2 = metri/secondo

       Unit_Map = 0,     // variabile contenente unità di misura della pres. manifold
                         // 0 = mbar
                         // 1 = Psi


 */

  // aggiornamento dello strumento di Coppia:
  if(Unit_Cop==0 && ListBox1->ItemIndex==1)
  {
  Coppia->AnalogMeterHQ1->MinimumValue *=0.102;
  Coppia->AnalogMeterHQ1->MaximumValue *=0.102;
  Coppia->AnalogMeterHQ1->RedStartValue=Coppia->AnalogMeterHQ1->MaximumValue;
  Strumenti->UpDown7->Position=Coppia->AnalogMeterHQ1->MaximumValue;
  }

  if(Unit_Cop==1 && ListBox1->ItemIndex==0)
  {
   Coppia->AnalogMeterHQ1->MinimumValue *=9.81;
   Coppia->AnalogMeterHQ1->MaximumValue *=9.81;
   Coppia->AnalogMeterHQ1->RedStartValue=Coppia->AnalogMeterHQ1->MaximumValue;
   Strumenti->UpDown7->Position=Coppia->AnalogMeterHQ1->MaximumValue;
  }
  if(Unit_Cop==0 && ListBox1->ItemIndex==2)
  {
   Coppia->AnalogMeterHQ1->MinimumValue *=0.737;
   Coppia->AnalogMeterHQ1->MaximumValue *=0.737;
   Coppia->AnalogMeterHQ1->RedStartValue=Coppia->AnalogMeterHQ1->MaximumValue;
   Strumenti->UpDown7->Position=Coppia->AnalogMeterHQ1->MaximumValue;
  }
  if(Unit_Cop==1 && ListBox1->ItemIndex==2)
  {
   Coppia->AnalogMeterHQ1->MinimumValue *=7.233;
   Coppia->AnalogMeterHQ1->MaximumValue *=7.233;
   Coppia->AnalogMeterHQ1->RedStartValue=Coppia->AnalogMeterHQ1->MaximumValue;
   Strumenti->UpDown7->Position=Coppia->AnalogMeterHQ1->MaximumValue;
  }

  if(Unit_Cop==2 && ListBox1->ItemIndex==0)
  {
   Coppia->AnalogMeterHQ1->MinimumValue *=1.355;
   Coppia->AnalogMeterHQ1->MaximumValue *=1.355;
   Coppia->AnalogMeterHQ1->RedStartValue=Coppia->AnalogMeterHQ1->MaximumValue;
   Strumenti->UpDown7->Position=Coppia->AnalogMeterHQ1->MaximumValue;
  }
  if(Unit_Cop==2 && ListBox1->ItemIndex==1)
  {
   Coppia->AnalogMeterHQ1->MinimumValue *=0.138;
   Coppia->AnalogMeterHQ1->MaximumValue *=0.138;
   Coppia->AnalogMeterHQ1->RedStartValue=Coppia->AnalogMeterHQ1->MaximumValue;
   Strumenti->UpDown7->Position=Coppia->AnalogMeterHQ1->MaximumValue;
  }



 // aggiornamento dello strumento Potenza
  if(Unit_Pot==0 && ListBox2->ItemIndex==1)
  {
   Potenza->AnalogMeterHQ1->MinimumValue *=1.36;
   Potenza->AnalogMeterHQ1->MaximumValue *=1.36;
   Potenza->AnalogMeterHQ1->RedStartValue=Potenza->AnalogMeterHQ1->MaximumValue;
  }
  if(Unit_Pot==1 && ListBox2->ItemIndex==0)
  {
   Potenza->AnalogMeterHQ1->MinimumValue *=0.735;
   Potenza->AnalogMeterHQ1->MaximumValue *=0.735;
   Potenza->AnalogMeterHQ1->RedStartValue=Potenza->AnalogMeterHQ1->MaximumValue;
  }
  if(Unit_Pot==0 && ListBox2->ItemIndex==2)
  {
   Potenza->AnalogMeterHQ1->MinimumValue *=1.341;
   Potenza->AnalogMeterHQ1->MaximumValue *=1.341;
   Potenza->AnalogMeterHQ1->RedStartValue=Potenza->AnalogMeterHQ1->MaximumValue;
  }
  if(Unit_Pot==1 && ListBox2->ItemIndex==2)
  {
   Potenza->AnalogMeterHQ1->MinimumValue *=0.986;
   Potenza->AnalogMeterHQ1->MaximumValue *=0.986;
   Potenza->AnalogMeterHQ1->RedStartValue=Potenza->AnalogMeterHQ1->MaximumValue;
  }
  if(Unit_Pot==2 && ListBox2->ItemIndex==0)
  {
   Potenza->AnalogMeterHQ1->MinimumValue *=0.745;
   Potenza->AnalogMeterHQ1->MaximumValue *=0.745;
   Potenza->AnalogMeterHQ1->RedStartValue=Potenza->AnalogMeterHQ1->MaximumValue;
  }
  if(Unit_Pot==2 && ListBox2->ItemIndex==1)
  {
   Potenza->AnalogMeterHQ1->MinimumValue *=1.013;
   Potenza->AnalogMeterHQ1->MaximumValue *=1.013;
   Potenza->AnalogMeterHQ1->RedStartValue=Potenza->AnalogMeterHQ1->MaximumValue;
  }




     // aggiornamento dello strumenti Temperatura
  if(Unit_Tem==0 && ListBox5->ItemIndex==1)
  {
   Termo->AnalogMeterHQ1->MinimumValue =0;
   Termo->AnalogMeterHQ1->MaximumValue = (1.8*Termo->AnalogMeterHQ1->MaximumValue)+32;
   Termo->AnalogMeterHQ1->GreenStartValue=int((Strumenti->UpDown6->Position*1.8)+32);
   Termo->AnalogMeterHQ1->YellowStartValue=int((Strumenti->UpDown14->Position*1.8)+32);
   Termo->AnalogMeterHQ1->RedStartValue=int((Strumenti->UpDown15->Position*1.8)+32);
   Strumenti->UpDown6->Position=Termo->AnalogMeterHQ1->GreenStartValue;
   Strumenti->UpDown14->Position=Termo->AnalogMeterHQ1->YellowStartValue;
   Strumenti->UpDown15->Position=Termo->AnalogMeterHQ1->RedStartValue;
   Strumenti->UpDown16->Position=Termo->AnalogMeterHQ1->MaximumValue;
  }

   if(Unit_Tem==1 && ListBox5->ItemIndex==0)
  {
   Termo->AnalogMeterHQ1->MinimumValue =0;
   Termo->AnalogMeterHQ1->MaximumValue =(Termo->AnalogMeterHQ1->MaximumValue-32)*0.555;
   Termo->AnalogMeterHQ1->GreenStartValue=int((Strumenti->UpDown6->Position-32)*0.555);
   Termo->AnalogMeterHQ1->YellowStartValue=int((Strumenti->UpDown14->Position-32)*0.555);
   Termo->AnalogMeterHQ1->RedStartValue=int((Strumenti->UpDown15->Position-32)*0.555);
   Strumenti->UpDown6->Position=Termo->AnalogMeterHQ1->GreenStartValue;
   Strumenti->UpDown14->Position=Termo->AnalogMeterHQ1->YellowStartValue;
   Strumenti->UpDown15->Position=Termo->AnalogMeterHQ1->RedStartValue;
   Strumenti->UpDown16->Position=Termo->AnalogMeterHQ1->MaximumValue;

  }





  // aggiornamento dello strumento pressione Manifold

  if(Unit_Map==0 && ListBox3->ItemIndex==1)  // se Bar e opzione Psi
  {
     Map->AnalogMeterHQ1->MinimumValue *= 0.0145;
        Map->AnalogMeterHQ1->MaximumValue *= 0.0145;
        Map->AnalogMeterHQ1->GreenStartValue=Strumenti->UpDown5->Position * 0.0145;
        Map->AnalogMeterHQ1->YellowStartValue=Strumenti->UpDown12->Position * 0.0145;
        Map->AnalogMeterHQ1->RedStartValue=Strumenti->UpDown11->Position * 0.0145;
        Strumenti->UpDown5->Position=Map->AnalogMeterHQ1->GreenStartValue;
        Strumenti->UpDown12->Position=Map->AnalogMeterHQ1->YellowStartValue;
        Strumenti->UpDown11->Position=Map->AnalogMeterHQ1->RedStartValue;
        Strumenti->UpDown13->Position=Map->AnalogMeterHQ1->MaximumValue;
  /*
   if(bMapType==false)
   {
     Strumenti->UpDown13->Max=3000*0.0145;
     Strumenti->UpDown11->Max=3000*0.0145;
     Strumenti->UpDown5->Max=3000*0.0145;
   }
   else
   {
     Strumenti->UpDown13->Max=6000*0.0145;
     Strumenti->UpDown11->Max=6000*0.0145;
     Strumenti->UpDown5->Max=6000*0.0145;
   }
   Map->AnalogMeterHQ1->MinimumValue = ceil((float)Map->AnalogMeterHQ1->MinimumValue*0.0145);
   Map->AnalogMeterHQ1->MaximumValue = ceil((float)Map->AnalogMeterHQ1->MaximumValue*0.0145);
   Map->AnalogMeterHQ1->GreenStartValue=ceil((float)Map->AnalogMeterHQ1->GreenStartValue* 0.0145);
   Map->AnalogMeterHQ1->YellowStartValue=ceil((float)Map->AnalogMeterHQ1->YellowStartValue*0.0145);
   Map->AnalogMeterHQ1->RedStartValue=ceil((float)Map->AnalogMeterHQ1->RedStartValue*0.0145);
   Strumenti->UpDown5->Position=Map->AnalogMeterHQ1->GreenStartValue;
   Strumenti->UpDown12->Position=Map->AnalogMeterHQ1->YellowStartValue;
   Strumenti->UpDown11->Position=Map->AnalogMeterHQ1->RedStartValue;
   Strumenti->UpDown13->Position=Map->AnalogMeterHQ1->MaximumValue;
   Strumenti->UpDown1->Position=Map->AnalogMeterHQ1->MinimumValue;
   */

  }


  if(Unit_Map==1 && ListBox3->ItemIndex==0)  // se Psi e opzione Bar
  {
        Map->AnalogMeterHQ1->MinimumValue *= 69;
        Map->AnalogMeterHQ1->MaximumValue *= 69;
        Map->AnalogMeterHQ1->GreenStartValue=Strumenti->UpDown5->Position * 69;
        Map->AnalogMeterHQ1->YellowStartValue=Strumenti->UpDown12->Position * 69;
        Map->AnalogMeterHQ1->RedStartValue=Strumenti->UpDown11->Position * 69;
        Strumenti->UpDown5->Position=Map->AnalogMeterHQ1->GreenStartValue;
        Strumenti->UpDown12->Position=Map->AnalogMeterHQ1->YellowStartValue;
        Strumenti->UpDown11->Position=Map->AnalogMeterHQ1->RedStartValue;
        Strumenti->UpDown13->Position=Map->AnalogMeterHQ1->MaximumValue;
  /*
   if(bMapType==false)
   {
     Strumenti->UpDown13->Max=3000;
     Strumenti->UpDown11->Max=3000;
     Strumenti->UpDown5->Max=3000;
   }
   else
   {
     Strumenti->UpDown13->Max=6000;
     Strumenti->UpDown11->Max=6000;
     Strumenti->UpDown5->Max=6000;
   }
   Map->AnalogMeterHQ1->MinimumValue = (int)ceil((float)Map->AnalogMeterHQ1->MinimumValue/0.0145);
   Map->AnalogMeterHQ1->MaximumValue = (int)ceil((float)Map->AnalogMeterHQ1->MaximumValue/0.0145);
   Map->AnalogMeterHQ1->GreenStartValue=(int)ceil((float)Map->AnalogMeterHQ1->GreenStartValue/ 0.0145);
   Map->AnalogMeterHQ1->YellowStartValue=(int)ceil((float)Map->AnalogMeterHQ1->YellowStartValue/0.0145);//* 69;
   Map->AnalogMeterHQ1->RedStartValue=(int)ceil((float)Map->AnalogMeterHQ1->RedStartValue/0.0145);//* 69;

   Strumenti->UpDown5->Position=Map->AnalogMeterHQ1->GreenStartValue;
   Strumenti->UpDown12->Position=Map->AnalogMeterHQ1->YellowStartValue;
   Strumenti->UpDown11->Position=Map->AnalogMeterHQ1->RedStartValue;
   Strumenti->UpDown13->Position=Map->AnalogMeterHQ1->MaximumValue;
   Strumenti->UpDown1->Position=Map->AnalogMeterHQ1->MinimumValue;
   */
  }


   // aggiornamento delle unità impostate:
 Unit_Cop=ListBox1->ItemIndex;
 Unit_Pot=ListBox2->ItemIndex;
 Unit_Map=ListBox3->ItemIndex;
 Unit_Tem=ListBox5->ItemIndex;
 Unit_Pes=ListBox6->ItemIndex;
 Unit_Cons=ListBox4->ItemIndex;



  Main->updateUnits();
  // aggiornamento dei coefficienti di conversione e strumenti....
  Main->carica_unita();
  // aggiorna visualizzazione grafici
  Main->showAllTests();

  if(Out_dat==false)
    Main->Visualizza_dat_test_strumenti(sTest_antep,iPos);

}
//---------------------------------------------------------------------------






void __fastcall TMisure::FormActivate(TObject *Sender)
{
 // focalizza le selezioni correnti salvate su disco...
  ListBox1->ItemIndex=Unit_Cop;
  ListBox2->ItemIndex=Unit_Pot;
  ListBox3->ItemIndex=Unit_Map;
  ListBox5->ItemIndex=Unit_Tem;
  ListBox6->ItemIndex=Unit_Pes;
  ListBox4->ItemIndex=Unit_Cons;

}
//---------------------------------------------------------------------------




