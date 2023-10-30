//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TSetup.h"
#include "TConnessione.h"
#include "TMain.h"
#include<fstream.h>
#include "math.h"
#include "CUtils.h"
#include "Structs.h"
#include "Log.h"
#include "CDyra.h"
#include "CDyn3.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "VrBorder"
#pragma link "VrControls"
#pragma link "VrLabel"
#pragma link "NxEdit"
#pragma link "NxCollection"
#pragma resource "*.dfm"

#define MAX_NUM_PORTE 100
TSetup *Setup;
  // definisce le variabili esterne
  extern bool cellError;
  extern struct DYNO_SETS dynoPrm;
  extern struct CELL_CALIB_PARAMS  cellPrm;/**< Set dei parametri cella analogo a TARA_CELL*/
  extern short tipoBanco,
              Lingua,
              sIndice;
  extern unsigned int okProcessData;
  extern char  Buffer[100],Errori[][50];
  extern String   Percorso,Ragione,Indirizzo,Profilo,
                 Telefono,Fax,Internet,EMail,Text56,Text59,Text60,Text61,Text62,Text63,Text91;

 extern float Val_cella_m1,Vcel1;

 extern int iOffset_patm,iOffset_temp;
 extern String txtLabelContaore, txtLabelLanci;
   //**********
 int iComDyra,iComDypt;
  String COMM;

 bool Ok_read_front_fase_1 = false,
      Ok_read_front_fase_2 = false,
      Ok_read_front_fase_3 = false,


      Ok_read_rear_fase_1  = false,
      Ok_read_rear_fase_2  = false,
      Ok_read_rear_fase_3  = false,
      OK_Lang = false;



 float fVal_med_F_fase_1 =0,
       fVal_med_F_fase_2 =0,
       fVal_med_F_fase_3 =0,

       fVal_med_R_fase_1 =0,
       fVal_med_R_fase_2 =0,
       fVal_med_R_fase_3 =0;

 int C_Usb=0,C_Usb2=0,Com_num[MAX_NUM_PORTE];
 short Mod_setup=0;


 unsigned char connDypt = 0,connDyra = 0;

  extern bool Connect_Dyra,Connect_Dypt;
  TCHAR lpTargetPath[1000];

//---------------------------------------------------------------------------
__fastcall TSetup::TSetup(TComponent* Owner)
        : TForm(Owner)
{

}


void __fastcall TSetup::BitBtn1Click(TObject *Sender)
{
  // aggiorna impostazioni banco in uso
  tipoBanco=ComboBox3->ItemIndex;
  // avviso di riavvio
  if(Mod_setup!=tipoBanco)
  {
    MessageBeep(MB_ICONASTERISK);
    ShowMessage(Text91);
  }
  Timer_celle->Enabled=false;

  // richiama funzione solo se la lingua è stata cambiata
  if(OK_Lang)Main->Idioma();

}
//---------------------------------------------------------------------------


void __fastcall TSetup::ComboBox4Change(TObject *Sender)
{
 Lingua=ComboBox4->ItemIndex+1;

 OK_Lang=true;

}
//---------------------------------------------------------------------------

void __fastcall TSetup::FormActivate(TObject *Sender)
{
	 if(g_dyraControl.isOn())
  {
    iComDyra=g_dyraControl.GetPortIndex();
    stComDyra->Caption=g_dyraControl.GetPort();
    stComDyra->Color=clGreen;
  }
  else
  {
    stComDyra->Caption="";
    stComDyra->Color=clRed;
  }
  if(g_brakeControl.isOn())
  {
    iComDypt=g_brakeControl.GetPortIndex();
    stComDyn3->Caption=g_brakeControl.GetPort();
    stComDyn3->Color=clGreen;
  }
  else
  {
    stComDyn3->Caption="";
    stComDyn3->Color=clRed;
  }

 // visualizza impostazioni del banco in uso

 ComboBox3->ItemIndex=tipoBanco;

 // visualizza impostazioni della lingua
 if(Lingua)
  ComboBox4->ItemIndex=Lingua-1;
 else
  ComboBox4->ItemIndex=0;
 MaskEdit1->Text=Ragione;
 MaskEdit2->Text=Indirizzo;
 MaskEdit3->Text=Telefono;
 MaskEdit4->Text=Fax;
 MaskEdit5->Text=Internet;
 MaskEdit6->Text=EMail;
 OpenDialog1->InitialDir=Percorso;
 if(FileExists(Percorso))
   Image1->Picture->LoadFromFile(Percorso);
 else
   Image1->Picture->Bitmap->LoadFromResourceName((int)HInstance, "Logo1");

  PageControl1->ActivePage=TabSheet1;

  // visualizza lunghezza braccio freno anteriore (mt)  
  Edit4->Text=FormatFloat("0.###",dynoPrm.bench.braccioReazCellaAnte);
  // Date delle tarature correnti.......
  Label21->Caption = DateToStr(cellPrm.dataCalibA);
}
//---------------------------------------------------------------------------

void __fastcall TSetup::SpeedButton1Click(TObject *Sender)
{
  String S;
  if(OpenDialog1->Execute())
  {
    S=OpenDialog1->FileName;
    Image1->Picture->LoadFromFile(S);
    Percorso=OpenDialog1->FileName;
  }
}
//---------------------------------------------------------------------------

void __fastcall TSetup::MaskEdit1Change(TObject *Sender)
{
  Ragione=MaskEdit1->Text;
}
//---------------------------------------------------------------------------

void __fastcall TSetup::MaskEdit2Change(TObject *Sender)
{
 Indirizzo=MaskEdit2->Text;
}
//---------------------------------------------------------------------------

void __fastcall TSetup::MaskEdit3Change(TObject *Sender)
{
Telefono=MaskEdit3->Text;
}
//---------------------------------------------------------------------------

void __fastcall TSetup::MaskEdit4Change(TObject *Sender)
{
Fax=MaskEdit4->Text;
}
//---------------------------------------------------------------------------

void __fastcall TSetup::MaskEdit5Change(TObject *Sender)
{
 Internet=MaskEdit5->Text;
}
//---------------------------------------------------------------------------

void __fastcall TSetup::MaskEdit6Change(TObject *Sender)
{
 EMail=MaskEdit6->Text;
}
//---------------------------------------------------------------------------

void __fastcall TSetup::Image1DblClick(TObject *Sender)
{
if(Percorso!="")
  {

  if(MessageDlg(Text59,mtConfirmation,TMsgDlgButtons() << mbYes << mbNo,0)== mrYes)
     {
   Beep();
   Percorso="";
   Image1->Picture=0;
     }

  }
}
//---------------------------------------------------------------------------

void __fastcall TSetup::Timer_celleTimer(TObject *Sender)
{
  // Vcc
  Edit2->Text=FormatFloat("0.#####",Vcel1*1000);
  // Kg
  Edit3->Text=FormatFloat("0.##",Val_cella_m1);
  
  /// Macchina a stati per la procedura di calibrazione
  // --------- legge valore medio cella anteriore con barra vuota fase 1------
  if(Ok_read_front_fase_1 == true)
  {
    ProgressBar1->Position=Timer_celle->Tag;
    fVal_med_F_fase_1 += Val_cella_m1;
    if(Timer_celle->Tag == 10)
    {
      fVal_med_F_fase_1/=10;Timer_celle->Tag=0;
      VrBorder2->Visible=true;
      Label2->Enabled=false;
      Label3->Enabled=true;
      BitBtn2->Enabled=false;
      BitBtn3->Enabled=true;
      ProgressBar1->Position=0;
      Ok_read_front_fase_1 = false;
    }
    Timer_celle->Tag++;
  }
  // ---------------------------------------------------------------------

  // --------- legge valore medio cella anteriore con barra + peso fase 2---
  if(Ok_read_front_fase_2 == true)
  {
    ProgressBar1->Position=Timer_celle->Tag;
    fVal_med_F_fase_2 += Val_cella_m1;
    if(Timer_celle->Tag == 10)
    {
      fVal_med_F_fase_2/=10;Timer_celle->Tag=0;
      VrBorder3->Visible=true;
      Label3->Enabled=false;
      Label4->Enabled=true;
      BitBtn4->Enabled=true;
      BitBtn3->Enabled=false;
      ProgressBar1->Position=0;
      Ok_read_front_fase_2 = false;
      cellPrm.deltaFase21A=fVal_med_F_fase_2-fVal_med_F_fase_1;
      cellPrm.pesoCampioneA=dynoPrm.bench.pesoCampione;
      cellPrm.braccioA=dynoPrm.bench.KbarraTar;
    }
    Timer_celle->Tag++;
  }
  // --------- legge valore medio cella senza barra fase 3 Tara ------------
  if(Ok_read_front_fase_3 == true)
  {
    ProgressBar1->Position=Timer_celle->Tag;
    fVal_med_F_fase_3 += Val_cella_m1;
    if(Timer_celle->Tag == 10)
    {
      fVal_med_F_fase_3/=10;Timer_celle->Tag=0;
      BitBtn6->Enabled=true;
      SpeedButton2->Enabled=true;
      ProgressBar1->Position=0;
      Ok_read_front_fase_3 = false;
    }
    Timer_celle->Tag++;
  }
}
//---------------------------------------------------------------------------

void __fastcall TSetup::FormShow(TObject *Sender)
{
  char cPort[10];
  int t=0;
  visualizzaContatori();
  NxButtonAzzeraOre->Caption="Reset "+txtLabelContaore;
  NxButtonAzzeraLanci->Caption="Reset "+txtLabelLanci;  
  Mod_setup=tipoBanco; // acquisisce impostazione del tipo di banco
  OK_Lang = false;
  Timer_celle->Enabled=true;
  RadioButton1->Checked=true;

  if(cellPrm.taraManuA==0)
    RadioButton1->Checked=true;

  if(cellPrm.taraManuA==1)
    RadioButton2->Checked=true;

  COMM="";
  
  for (int Nu=1;Nu<=MAX_NUM_PORTE;Nu++)
  {
    sprintf(cPort,"COM%d",Nu);
    if (QueryDosDevice(cPort,(LPSTR)lpTargetPath,1000))
    {
      COMM = COMM + (cPort)+ " ";
      Com_num[t]=Nu;
      t++;
    }
  }
  Label23->Caption = Text56 + " " + COMM;
  NxSpinEdit1->Value=iOffset_patm;
}
//---------------------------------------------------------------------------










void __fastcall TSetup::FormClose(TObject *Sender, TCloseAction &Action)
{
 Label2->Enabled=false;
 Label3->Enabled=false;
 Label4->Enabled=false;
 RadioButton1->Enabled=true;
 RadioButton2->Enabled=true;

}
//---------------------------------------------------------------------------

void __fastcall TSetup::SpeedButton2Click(TObject *Sender)
{
  MessageBeep(MB_ICONEXCLAMATION);
  if(MessageDlg(Text60,mtConfirmation,TMsgDlgButtons() << mbYes << mbNo,0) == mrYes)
  {
    // azzera le variabili
    fVal_med_F_fase_1=0;
    fVal_med_F_fase_2=0;
    fVal_med_F_fase_3=0;
    cellPrm.pesoCampioneA=1;
    cellPrm.deltaFase21A=1;
    cellPrm.offsetA=0;
    cellPrm.braccioA=1;
    RadioButton1->Enabled=false;
    RadioButton2->Enabled=false;
    Label4->Enabled=false;
    Label7->Caption="Kg";

    if(RadioButton2->Checked) // taratura manuale
    {
      VrBorder1->Visible=true;
      Label2->Enabled=true;
      SpeedButton2->Enabled=false;
      BitBtn2->Enabled=true;
      BitBtn1->Enabled=false;
      BitBtn3->Enabled=false;
      BitBtn4->Enabled=false;
      ProgressBar1->Visible=true;
      ProgressBar1->Position=0;
    }
    if(RadioButton1->Checked) // usa caratteristica cella
    {
      SpeedButton2->Enabled=false;
      VrBorder3->Visible=true;
      Label4->Enabled=true;
      BitBtn4->Enabled=true;
      BitBtn1->Enabled=false;
      BitBtn3->Enabled=false;
      ProgressBar1->Visible=true;
      ProgressBar1->Position=0;
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TSetup::RadioButton1Click(TObject *Sender)
{
  if(RadioButton1->Checked)
    cellPrm.taraManuA=0;
}
//---------------------------------------------------------------------------

void __fastcall TSetup::RadioButton2Click(TObject *Sender)
{
  if(RadioButton2->Checked)
    cellPrm.taraManuA=1;
}
//---------------------------------------------------------------------------


void __fastcall TSetup::BitBtn6Click(TObject *Sender)
{

 // memorizzazione taratura...
 BitBtn6->Enabled=false;


                 // [0] = Data ultima taratura cella Front
                 // [1] = Valore in Kg del peso campione passato da DATI_VARI[19]
                 // [2] = vuoto
                 // [3] = valore lettura Front fase 1 (solo barra)
                 // [4] = valore lettura Front fase 2 (barra + peso noto)
                 // [5] = valore lettura Front fase 3 (senza nulla Tara)
                 // [6] = Delta Front tra lettura fase 2 e fase 1
                 // [7] = Offset di azzeramento valore a riposo Front
                 // [8] = valore costante del braccio Barra di taratura passato da DATI_VARI[18]
                 // [9] = 0-> usa caratt. elett. della cella   1-> taratura manuale
                 // [10] = vuoto

                 // [11] = Data ultima taratura cella Post
                 // [12] = Valore in Kg del peso campione passato da DATI_VARI
                 // [13] = vuoto
                 // [14] = valore lettura Post fase 1 (solo barra)
                 // [15] = valore lettura Post fase 2 (barra + peso noto)
                 // [16] = valore lettura Post fase 3 (senza nulla Tara)
                 // [17] = Delta Post tra lettura fase 2 e fase 1
                 // [18] = Offset di azzeramento valore a riposo Post
                 // [19] = valore costante del braccio Barra di taratura
                 // [20] = 0-> usa caratt. elett. della cella   1-> taratura manuale
                 // [21] = vuoto

   cellPrm.dataCalibA=int(Date());
   cellPrm.null2=0;
   cellPrm.readFase1A=fVal_med_F_fase_1;
   cellPrm.readFase2A=fVal_med_F_fase_2;
   cellPrm.readFase3A=fVal_med_F_fase_3;
   cellPrm.offsetA=cellPrm.readFase3A;
   if(RadioButton1->Checked)
   {
      cellPrm.taraManuA=0;
      cellPrm.braccioA=1;
   }
   if(RadioButton2->Checked)
   {
      cellPrm.taraManuA=1;
      // costante della barra
      cellPrm.braccioA=dynoPrm.bench.KbarraTar;
   }

   // scrittura su file dei dati di taratura.........

 ofstream out(utils.GetProgramDataPath("file_tara.cel").c_str(), ios::out | ios::binary);  // salva il nuovo file di taratura

 if(!out)
 {
   ShowMessage(Text61);
 }

 else
 {
   out.write((unsigned char*)&cellPrm,sizeof (cellPrm));
 }

 out.close();

 RadioButton1->Enabled=true;
 RadioButton2->Enabled=true;
 Label4->Enabled=false;
 BitBtn1->Enabled=true;

  ShowMessage(Text63);

  Label21->Caption = DateToStr(cellPrm.dataCalibA);
  Label7->Caption="Kg";

  cellError=false;
}
//---------------------------------------------------------------------------


void __fastcall TSetup::BitBtn2Click(TObject *Sender)
{

VrBorder1->Visible=false;
BitBtn2->Enabled=false;
Ok_read_front_fase_1 = true; // abilita lettura campionamenti cella fase 1        
}
//---------------------------------------------------------------------------

void __fastcall TSetup::BitBtn3Click(TObject *Sender)
{

VrBorder2->Visible=false;
//BitBtn3->Enabled=false;
Ok_read_front_fase_2 = true; // abilita lettura campionamenti cella fase 2
}
//---------------------------------------------------------------------------

void __fastcall TSetup::BitBtn4Click(TObject *Sender)
{

VrBorder3->Visible=false;
BitBtn4->Enabled=false;
Ok_read_front_fase_3 = true; // abilita lettura campionamenti cella fase 3
}
//---------------------------------------------------------------------------


void __fastcall TSetup::BitBtn7Click(TObject *Sender)
{
  Ok_read_rear_fase_1 = true; // abilita lettura campionamenti cella post fase 1
}
//---------------------------------------------------------------------------

void __fastcall TSetup::BitBtn8Click(TObject *Sender)
{

  Ok_read_rear_fase_2 = true; // abilita lettura campionamenti cella post fase 2
}
//---------------------------------------------------------------------------

void __fastcall TSetup::BitBtn9Click(TObject *Sender)
{
  Ok_read_rear_fase_3 = true; // abilita lettura campionamenti cella post fase 3
}
//---------------------------------------------------------------------------


void __fastcall TSetup::ComboBox3Change(TObject *Sender)
{
  tipoBanco = ComboBox3->ItemIndex;
  Main->GroupBox4->Visible=(tipoBanco==TRACTOR_TWIN);

  String nomefile;
  if(okProcessData==1)
  {
    if(tipoBanco==2)//Trailer
      nomefile="File_impo_trailer.dmt";
    else
      nomefile="File_impo_trac.dmt";
    ifstream infile(utils.GetProgramDataPath(nomefile.c_str()).c_str(), ios::binary);  // carica impostazioni
    if(infile.is_open())
    {
      infile.read((unsigned char*)&dynoPrm,sizeof (dynoPrm));
      infile.close();
      #ifdef _DEBUG
        LogError(g_cFileLog,LOG_INFO,"Caricato il file_impo da Setup");
      #endif
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TSetup::Button1Click(TObject *Sender)
{
    int t=0;
  COMM="";
  char cPort[6];
  for (int Nu=1;Nu<=MAX_NUM_PORTE;Nu++)
  {
    sprintf(cPort,"COM%d",Nu);
    if (QueryDosDevice(cPort,(LPSTR)lpTargetPath,1000))
    {
      COMM = COMM + (cPort)+ " ";
      Com_num[t]=Nu;
      t++;
    }
  }
  Label23->Caption = Text56 + " " + COMM;

  Button1->Enabled=false;
  stComDyra->Caption="";
  Label23->Caption = Text56 + " " + COMM;
  C_Usb=0;

  g_dyraControl.Disconnect();
  Sleep(200);
  /*
  while(g_dyraControl.GetDyraStatus()!=CDyra::EDyraNotAvail &&
        g_dyraControl.GetDyraStatus()!=CDyra::EDyraNotDefined)*/
    Application->ProcessMessages();
  g_dyraControl.autoDetect();
  stComDyra->Color=clYellow;
  tmrAutoDetect->Enabled=true;
 }
//---------------------------------------------------------------------------

void __fastcall TSetup::NxSpinEdit1Change(TObject *Sender)
{
 iOffset_temp = NxSpinEdit1->Value;
}
//---------------------------------------------------------------------------

void __fastcall TSetup::NxButton1Click(TObject *Sender)
{

  MessageBeep(MB_ICONEXCLAMATION);

if(MessageDlg(Text60,mtConfirmation,TMsgDlgButtons() << mbYes << mbNo,0) == mrYes)
{

 iOffset_patm=iOffset_temp;

  TIniFile *ini;
  ini = new TIniFile(utils.GetProgramDataName(".INI"));
  ini->WriteInteger ("Offset"+Profilo, "Patmo",iOffset_patm);
  delete ini;

}
 else  {iOffset_temp=iOffset_patm;
       NxSpinEdit1->Value=iOffset_patm;}


}
//---------------------------------------------------------------------------

void __fastcall TSetup::FormCreate(TObject *Sender)
{
//  Height=600;
//  Width =500;
}
//---------------------------------------------------------------------------


void __fastcall TSetup::tmrAutoDetectTimer(TObject *Sender)
{
  /// Problema all uscita da  questo if!
  if( (/*g_dyraControl.isOn() &&*/ (!g_dyraControl.isAutoDetect()) ) &&
      (/*g_brakeControl.isOn() && */(!g_brakeControl.isAutoDetect())))
    tmrAutoDetect->Enabled=false;
  stComDyra->Caption=g_dyraControl.GetPort();
  if(g_dyraControl.isAutoDetect())
    stComDyra->Color=clYellow;
  else
  {
    Button1->Enabled=true;
    if(g_dyraControl.isOn())
    {
      stComDyra->Color=clGreen;
      Main->sComDyra=(String)g_dyraControl.GetPort();
    }
    else
    {
      stComDyra->Color=clRed;
      stComDyra->Caption="";
    }
  }

  stComDyn3->Caption=g_brakeControl.GetPort();

  if(g_brakeControl.isAutoDetect())
    stComDyn3->Color=clYellow;
  else
  {
    Button2->Enabled=true;
    if(g_brakeControl.isOn())
    {
      stComDyn3->Color=clGreen;
      Main->sComDyn3=(String)g_brakeControl.GetPort();
    }
    else
    {
      stComDyn3->Color=clRed;
       stComDyn3->Caption="";
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TSetup::Button2Click(TObject *Sender)
{
  int t=0;
  COMM="";
  char cPort[6];
  Button2->Enabled=false;
  for (int Nu=1;Nu<=MAX_NUM_PORTE;Nu++)
  {
    sprintf(cPort,"COM%d",Nu);
    if (QueryDosDevice(cPort,(LPSTR)lpTargetPath,1000))
    {
      COMM = COMM + (cPort)+ " ";
      Com_num[t]=Nu;
      t++;
    }
  }
  Label23->Caption = Text56 + " " + COMM;

  stComDyn3->Caption="";
  C_Usb2=0;
  stComDyn3->Color=clYellow;
  g_brakeControl.Disconnect();
  while(g_brakeControl.GetDyn3Status()!=CDyn3::EDyn3NotAvail &&
    g_brakeControl.GetDyn3Status()!=CDyn3::EDyn3NotDefined)
  Application->ProcessMessages();
  g_brakeControl.autoDetect();
  tmrAutoDetect->Enabled=true;
}
//---------------------------------------------------------------------------

void TSetup::visualizzaContatori()
{
        char Buffer[32];
        //carico i contatori
        TDateTime azzCont  = Main->dataResetCountMinPar;
        TDateTime azzLanc  = Main->dataResetLanci;
        float ore=Main->nCountMinTotRpmRulli+Main->contaoreMinuti;
        long lanci=Main->nLanciTotali;
        NxLabel2->Caption=azzCont;
        NxLabel3->Caption=azzLanc;
        ore/=60;
        sprintf(Buffer,"%s: %d",txtLabelLanci.c_str(),lanci);
        NxLabelLanci->Caption= Buffer;
        sprintf(Buffer,"%s: %1.2f",txtLabelContaore.c_str(),ore);
        NxLabelContaore->Caption=Buffer;
}

void __fastcall TSetup::NxButtonAzzeraOreClick(TObject *Sender)
{
  //sommo quello fatto fino ad adesso
  Main->sommaContaore();
  //azzero le ore di funzionamento
  Main->nCountMinTotRpmRulli=0;
  TDateTime adesso  = Now();
  float tmp = adesso.operator double();

  Main->dataResetCountMinPar=tmp;
  //scrivo il file
  Main->scriviFileImpo();
  //ricarico i dati
  visualizzaContatori();
}
//---------------------------------------------------------------------------

void __fastcall TSetup::NxButtonAzzeraLanciClick(TObject *Sender)
{
        //azzero i lanci parziali
        Main->nLanciTotali=0;        //parziali avviati        
        dynoPrm.count.nLanciTermPar=0;        //parziali terminati
        TDateTime adesso  = Now();
        float tmp = adesso.operator double();
        Main->dataResetLanci=tmp;
        //scrivo il file
        Main->scriviFileImpo();
        //ricarico i dati
        visualizzaContatori();
}
//---------------------------------------------------------------------------

