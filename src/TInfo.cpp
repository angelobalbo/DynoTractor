//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "TInfo.h"
#include<iostream.h>
#include "TMain.h"
#include "winver.h"
#include "structs.h"
#include "CDyn3.h"
#include "CDyra.h"

//---------------------------------------------------------------------
#pragma resource "*.dfm"
TInfo *Info;

 extern struct DYNO_SETS dynoPrm;
 extern char Buffer[100];
 extern float DATI_VARI[500];
 extern String Text71,Text72,Text73,Text74,Link,Produttore;
 extern int Ver_sw;
 extern String Versione_EXE;
 extern  char swNbr[20];
//---------------------------------------------------------------------
__fastcall TInfo::TInfo(TComponent* AOwner)
	: TForm(AOwner)
{
 Label8->Caption=Text71;
 Label9->Caption=Text72 + " " + FormatFloat("0.#",dynoPrm.bench.revFile);
 Label3->Caption=Text74;
 Label2->Caption=Text73+" "+(String)swNbr;
}
//---------------------------------------------------------------------



void __fastcall TInfo::Label4Click(TObject *Sender)
{
  String St = "http://"+Label4->Caption;
  ShellExecute(Handle,"open",St.c_str(),0,0,SW_SHOW);
}
//---------------------------------------------------------------------------


void __fastcall TInfo::FormShow(TObject *Sender)
{
  unsigned char fwVer, fwType;
  char cTmp[100],cDyra[20],cDyn3[20];
// Dati finestra informazioni
 Label10->Caption= Produttore;
 Label4->Caption= Link;
#ifdef _VAMAG
  Image2->Visible=true;
#else
  Image1->Visible=true;
#endif
  cDyra[0]=0;
  cDyn3[0]=0;
  cTmp[0]=0;
  if(g_dyraControl.GetFwVer())
    sprintf(cDyra,"dyra 0x%02X",g_dyraControl.GetFwVer());
  if(g_brakeControl.GetFwVer())
    sprintf(cDyn3,"dyn %c 0x%02X",g_brakeControl.GetFwType(),g_brakeControl.GetFwVer());
  if(cDyn3[0] || cDyra[0])
    sprintf(cTmp,"Firmware  %s    %s",cDyra,cDyn3);
  lblFw->Caption=(String)cTmp;
}
//---------------------------------------------------------------------------

