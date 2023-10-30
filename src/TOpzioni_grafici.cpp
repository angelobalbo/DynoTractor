//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "TOpzioni_grafici.h"
#include "TMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TOpzioni_grafici *Opzioni_grafici;

 extern bool
          Vis_P_M ,  // potenza motore
          Vis_P_R ,  // potenza ruota
          Vis_C_M ,  // coppia motore
          Vis_C_R ,  // coppia ruota
          Vis_P_D ,  // potenza dissipata
          Vis_Pmap , // pressione aspirazione
          Vis_T_K ,  // temperatura termocoppia K
          Vis_T_N ,  // Temperatura Ntc Olio
          Vis_cons,
          Vis_GV  ;  // visualizza su giri o velocità


 extern short Unit_Vel,sNum_test,Unit_Cons;
 extern int vNumTest[10];//,iUnitConsumi;
 extern String Text202;

//---------------------------------------------------------------------------
__fastcall TOpzioni_grafici::TOpzioni_grafici(TComponent* Owner)
        : TForm(Owner)
{


}
//---------------------------------------------------------------------------




void __fastcall TOpzioni_grafici::CheckBox12Click(TObject *Sender)
{
  Vis_C_R=CheckBox12->Checked;
  Controllo_visual();
}
//---------------------------------------------------------------------------

void __fastcall TOpzioni_grafici::CheckBox10Click(TObject *Sender)
{
  Vis_P_R=CheckBox10->Checked;
  Controllo_visual();
}
//---------------------------------------------------------------------------

void __fastcall TOpzioni_grafici::CheckBox4Click(TObject *Sender)
{
  Vis_Pmap=CheckBox4->Checked;
  Main->Chart1->CustomAxes->Items[0]->Visible=CheckBox4->Checked;
  Controllo_visual();
}
//---------------------------------------------------------------------------


void __fastcall TOpzioni_grafici::CheckBox5Click(TObject *Sender)
{

  Vis_T_K=(CheckBox5->Checked);
  Main->Chart1->CustomAxes->Items[2]->Visible=CheckBox5->Checked;
  Controllo_visual();
}


void __fastcall TOpzioni_grafici::CheckBox8Click(TObject *Sender)
{
  Vis_T_N=CheckBox8->Checked;
  Main->Chart1->CustomAxes->Items[3]->Visible=(CheckBox8->Checked);
  Controllo_visual();
}
//---------------------------------------------------------------------------


void __fastcall TOpzioni_grafici::FormActivate(TObject *Sender)
{
  CheckBox10->Checked=Vis_P_R;
  CheckBox12->Checked=Vis_C_R;
  CheckBox4->Checked=Vis_Pmap;
  CheckBox5->Checked=Vis_T_K;
  CheckBox8->Checked=Vis_T_N;   // Temperatura Ntc Olio
  CheckBoxConsumi->Checked=Vis_cons;
}
//---------------------------------------------------------------------------


void __fastcall TOpzioni_grafici::RadioButton1Click(TObject *Sender)
{
  Vis_GV=false;
  Main->Chart1->BottomAxis->Title->Caption=Text202;//"Giri/min";
}
//---------------------------------------------------------------------------

void __fastcall TOpzioni_grafici::RadioButton2Click(TObject *Sender)
{
  Vis_GV=true;
  if(Unit_Vel ==0) Main->Chart1->BottomAxis->Title->Caption="Km/h";
  if(Unit_Vel ==1) Main->Chart1->BottomAxis->Title->Caption="Mph";
  if(Unit_Vel ==2) Main->Chart1->BottomAxis->Title->Caption="m/s";
}
//---------------------------------------------------------------------------




void __fastcall TOpzioni_grafici::Controllo_visual()
{

    if(Main->CheckBox1->Checked  && Main->CheckBox1->Visible)
    {
        if(Vis_C_R)Main->Series10->Visible=true; // coppia ruota
        if(Vis_P_R)Main->Series9->Visible=true;  // potenza ruota
        if(Vis_Pmap)Main-> Series3->Visible=true;
        if(Vis_T_K) Main-> Series6->Visible=true;
        if(Vis_T_N) Main-> Series7->Visible=true;
        if(Vis_cons) Main-> Series15->Visible=true;
    }
    else
    {
        if(Vis_C_R)Main->Series10->Visible=false;
        if(Vis_P_R)Main->Series9->Visible=false;
        if(Vis_Pmap) Main->Series3->Visible=false;
        if(Vis_T_K) Main-> Series6->Visible=false;
        if(Vis_T_N) Main-> Series7->Visible=false;
        if(Vis_cons) Main-> Series15->Visible=false;
    }
    if(Main->CheckBox2->Checked && Main->CheckBox2->Visible)
    {
        if(Vis_C_R)Main->Series21->Visible=true;  // coppia ruota
        if(Vis_P_R)Main->Series35->Visible=true;  // potenza ruota
        if(Vis_Pmap)Main-> Series60->Visible=true;
        if(Vis_T_K)Main->  Series81->Visible=true;
        if(Vis_T_N) Main-> Series88->Visible=true;
        if(Vis_cons) Main-> Series16->Visible=true;

    }
    else
    {
        if(Vis_C_R)Main->Series21->Visible=false;
        if(Vis_P_R)Main->Series35->Visible=false;
        if(Vis_Pmap)Main-> Series60->Visible=false;
        if(Vis_T_K) Main-> Series81->Visible=false;
        if(Vis_T_N)Main->  Series88->Visible=false;
        if(Vis_cons) Main-> Series16->Visible=false;
    }
    if(Main->CheckBox3->Checked && Main->CheckBox3->Visible)
    {
        if(Vis_C_R)Main->Series22->Visible=true;  // coppia ruota
        if(Vis_P_R)Main->Series36->Visible=true;  // potenza ruota
        if(Vis_Pmap)Main-> Series61->Visible=true;
//        if(Vis_I_A)Main->  Series75->Visible=true;
        if(Vis_T_K)Main->  Series82->Visible=true;
        if(Vis_T_N) Main-> Series89->Visible=true;
        if(Vis_cons) Main-> Series17->Visible=true;
    }
    else
    {
        if(Vis_C_R)Main->Series22->Visible=false;
        if(Vis_P_R)Main->Series36->Visible=false;
        if(Vis_Pmap)Main-> Series61->Visible=false;
//        if(Vis_I_A) Main-> Series75->Visible=false;
        if(Vis_T_K) Main-> Series82->Visible=false;
        if(Vis_T_N) Main-> Series89->Visible=false;
        if(Vis_cons) Main-> Series17->Visible=false;
    }
    if( Main->CheckBox4->Checked && Main->CheckBox4->Visible)
    {
        if(Vis_C_R) Main->Series23->Visible=true;  // coppia ruota
        if(Vis_P_R) Main->Series37->Visible=true;  // potenza ruota
        if(Vis_Pmap) Main-> Series62->Visible=true;
//        if(Vis_I_A)  Main-> Series76->Visible=true;
        if(Vis_T_K) Main->  Series83->Visible=true;
        if(Vis_T_N)  Main-> Series90->Visible=true;
        if(Vis_cons) Main-> Series18->Visible=true;
    }
    else
    {
        if(Vis_C_R) Main->Series23->Visible=false;
        if(Vis_P_R) Main->Series37->Visible=false;
        if(Vis_Pmap)  Main->Series62->Visible=false;
//        if(Vis_I_A)   Main->Series76->Visible=false;
        if(Vis_T_K)  Main-> Series83->Visible=false;
        if(Vis_T_N)  Main-> Series90->Visible=false;
        if(Vis_cons) Main-> Series18->Visible=false;
    }
    if(Main->CheckBox5->Checked && Main->CheckBox5->Visible) //5
    {
        if(Vis_C_R)Main->Series24->Visible=true;  // coppia ruota
        if(Vis_P_R)Main->Series38->Visible=true;  // potenza ruota
        if(Vis_Pmap)Main-> Series63->Visible=true;
//        if(Vis_I_A) Main-> Series77->Visible=true;
        if(Vis_T_K) Main-> Series84->Visible=true;
        if(Vis_T_N) Main-> Series91->Visible=true;
        if(Vis_cons) Main-> Series28->Visible=true;
    }
    else
    {
        if(Vis_C_R)Main->Series24->Visible=false;
        if(Vis_P_R)Main->Series38->Visible=false;
        if(Vis_Pmap)Main-> Series63->Visible=false;
//        if(Vis_I_A) Main-> Series77->Visible=false;
        if(Vis_T_K) Main-> Series84->Visible=false;
        if(Vis_T_N) Main-> Series91->Visible=false;
        if(Vis_cons) Main-> Series28->Visible=false;
    }
    if(Main->CheckBox6->Checked && Main->CheckBox6->Visible)
    {
        if(Vis_C_R)Main->Series25->Visible=true;  // coppia ruota
        if(Vis_P_R)Main->Series39->Visible=true;  // potenza ruota
        if(Vis_Pmap)Main-> Series64->Visible=true;
//        if(Vis_I_A) Main-> Series78->Visible=true;
        if(Vis_T_K) Main-> Series85->Visible=true;
        if(Vis_T_N)Main->  Series92->Visible=true;
        if(Vis_cons) Main-> Series29->Visible=true;
    }
    else
    {
        if(Vis_C_R)Main->Series25->Visible=false;
        if(Vis_P_R)Main->Series39->Visible=false;
        if(Vis_Pmap) Main->Series64->Visible=false;
//        if(Vis_I_A) Main-> Series78->Visible=false;
        if(Vis_T_K) Main-> Series85->Visible=false;
        if(Vis_T_N) Main-> Series92->Visible=false;
        if(Vis_cons) Main-> Series29->Visible=false;
    }
#if 1
    if(Main->CheckBox7->Checked && Main->CheckBox7->Visible)
    {
        if(Vis_C_R)Main->Series26->Visible=true;  // coppia ruota
        if(Vis_P_R)Main->Series40->Visible=true;  // potenza ruota
        if(Vis_Pmap)Main-> Series65->Visible=true;
//        if(Vis_I_A)  Main->Series79->Visible=true;
        if(Vis_T_K)  Main->Series86->Visible=true;
        if(Vis_T_N)  Main->Series93->Visible=true;
        if(Vis_cons) Main-> Series30->Visible=true;
    }
    else
    {
        if(Vis_C_R)Main->Series26->Visible=false;
        if(Vis_P_R)Main->Series40->Visible=false;
        if(Vis_Pmap) Main->Series65->Visible=false;
//        if(Vis_I_A)  Main->Series79->Visible=false;
        if(Vis_T_K) Main-> Series86->Visible=false;
        if(Vis_T_N) Main-> Series93->Visible=false;
        if(Vis_cons) Main-> Series30->Visible=false;
    }
#endif
    if(Main->CheckBox8->Checked && Main->CheckBox8->Visible)
    {
        if(Vis_C_R)Main->Series27->Visible=true;  // coppia ruota
        if(Vis_P_R)Main->Series41->Visible=true;  // potenza ruota
        if(Vis_Pmap) Main->Series66->Visible=true;
//        if(Vis_I_A) Main-> Series80->Visible=true;
        if(Vis_T_K)  Main->Series87->Visible=true;
        if(Vis_T_N) Main-> Series94->Visible=true;
        if(Vis_cons) Main-> Series31->Visible=true;
    }
    else
    {
        if(Vis_C_R)Main->Series27->Visible=false;
        if(Vis_P_R)Main->Series41->Visible=false;
        if(Vis_Pmap) Main->Series66->Visible=false;
        if(Vis_T_K) Main-> Series87->Visible=false;
        if(Vis_T_N) Main-> Series94->Visible=false;
        if(Vis_cons) Main-> Series31->Visible=false;
    }
}

void __fastcall TOpzioni_grafici::BitBtn1Click(TObject *Sender)
{
  Main->PageControl1->ActivePage=Main->TabSheet1;
  Main->carica_unita();
  Main->showAllTests();
  Main->Chart1->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall TOpzioni_grafici::CheckBoxConsumiClick(TObject *Sender)
{
  Vis_cons=CheckBoxConsumi->Checked;
  Main->Chart1->CustomAxes->Items[4]->Visible=CheckBoxConsumi->Checked;
  Controllo_visual();
}
//---------------------------------------------------------------------------





void __fastcall TOpzioni_grafici::FormShow(TObject *Sender)
{
  rbL->Visible=(Unit_Cons!=2);
  rbGKWh->Visible=(Unit_Cons!=2);
}
//---------------------------------------------------------------------------

void __fastcall TOpzioni_grafici::rbGKWhClick(TObject *Sender)
{
  // aggiorna visualizzazione grafici
    Main->Chart1->CustomAxes->Items[4]->Title->Caption="g/kWh";

}
//---------------------------------------------------------------------------

void __fastcall TOpzioni_grafici::rbLClick(TObject *Sender)
{
    switch(Unit_Cons)
    {
      case 0: // litri/ora
        Main->Chart1->CustomAxes->Items[4]->Title->Caption="l/h" ;
      break;
      case 1: // litri/minuto
        Main->Chart1->CustomAxes->Items[4]->Title->Caption="l/min" ;
      break;
      case 2: // grammi/Kwh
        Main->Chart1->CustomAxes->Items[4]->Title->Caption="g/Kwh" ;
      break;
      case 3: // litri
        Main->Chart1->CustomAxes->Items[4]->Title->Caption="l";
    }  
}
//---------------------------------------------------------------------------


void __fastcall TOpzioni_grafici::cbPZeroScaleClick(TObject *Sender)
{
  if(cbPZeroScale->Checked)
  {
    Main->Chart1->LeftAxis->Automatic=false;
    Main->Chart1->LeftAxis->AutomaticMaximum=true;
    Main->Chart1->LeftAxis->AutomaticMinimum=false;
    Main->Chart1->LeftAxis->Minimum=0;
  }
  else
  {
    Main->Chart1->LeftAxis->Automatic=true;
    Main->Chart1->LeftAxis->AutomaticMaximum=true;
    Main->Chart1->LeftAxis->AutomaticMinimum=true;
  }
}
//---------------------------------------------------------------------------

void __fastcall TOpzioni_grafici::cbCZeroScaleClick(TObject *Sender)
{
  if(cbCZeroScale->Checked)
  {
    Main->Chart1->RightAxis->Automatic=false;
    Main->Chart1->RightAxis->AutomaticMaximum=true;
    Main->Chart1->RightAxis->AutomaticMinimum=false;
    Main->Chart1->RightAxis->Minimum=0;
  }
  else
  {
    Main->Chart1->RightAxis->Automatic=true;
    Main->Chart1->RightAxis->AutomaticMaximum=true;
    Main->Chart1->RightAxis->AutomaticMinimum=true;
  }
}
//---------------------------------------------------------------------------

