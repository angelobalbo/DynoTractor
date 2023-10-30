//--------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <stdconvs.hpp>
#include<fstream.h>
#include <shfolder.h>
#include "TCarica_File.h"
#include "TMain.h"
#include "CUtils.h"
#include "structs.h"
#include "TImpostazioni.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "NxCollection"
#pragma resource "*.dfm"


TCarica_File *Carica_File;


 extern char  Buffer[100],     // buffer di scambio stringhe di testo usate
                         // per la lingua in uso
        VERS[10],     // contiene la versione del file di impostazioni

        Cliente[30],
        Targa[20],
        NOTE[500],
        Costruttore[30],
        Modello[30],
        Allestimento[30],
        nomeTest[100],
        Note_test[500];

  extern short sNum_test,Unit_Tem;
  extern unsigned char cDynVersion;
  extern struct TEST_SETS          testPrm;/**< Set dei parametri del test analogo a DAT */

  extern bool bSavedTest;

  static int  vGiri          [8][DIM_MEDIA];
  static char vNoteTest      [8][DIM_MEDIA];
  static char vNomeTest      [8][100];
  static float vTermoK        [8][DIM_MEDIA];
  static float vTermoOil      [8][DIM_MEDIA];
  static float vPresMAP       [8][DIM_MEDIA];
  static float vConsTot       [8][DIM_MEDIA];
  static float vCoppiaRuota   [8][DIM_MEDIA];
  static float vPotenzaRuota  [8][DIM_MEDIA];
  static float vCons          [8][DIM_MEDIA];
  static struct TEST_SETS          datPrm[8];
  
  int iNum_Test_Bis[10];

  extern int Giri[500],
          Giri_Acq[500],
          Cont_Acq,
          Cont_Acq2,
          vNumTest[10];

  extern    float
      vettCons[500],
      fnum_val[3],
      fTermo_K[500],          // contiene valori acquisiti mediati della termocoppia 0-1000°C
      fTermo_Oil[500],        // contiene valori acquisiti mediati della termp olio 0-150°C
      fPres_MAP[500],         // contiene valori acquisiti mediati della pressione d'aspirazione
      fvettConsTot[500],           // contiene valori acquisiti mediati della lambda 0-1V  0-5V
      fAD_conv_15v[500],      // contiene valori acquisiti mediati del AD converter 0-15Vcc
      fRap_tot_A[500],      // contiene curva di rapporto totale asse anteriore
      fRap_tot_P[500],      // contiene curva di rapporto totale asse posteriore

  fK_convC,
  fK_convP,

  Coppia_ruota[500], // coppia risultante da spline alla ruota
  Potenza_ruota[500],// potenza risultante alla ruota da coppia ruota
  Potenza_dissip[500],// potenza dissipata asse anteriore
  Coppia_motore[500], // coppia risultante da somma tra coppia ruota e cop dissipata
  Potenza_motore[500];// potenza risultante da somma tra pot ruota e pot dissipata



 extern bool OKF,Avvio_OK;

 extern String SS; // nome file selezionato
 String SSTmp;
 extern String Text42,Text1,Text2,Text3,Text4,Text5,Text6,Text7,Text8,Text9,Text10,
 Text11,Text12,Text13,Text14,Text15,Text16,Text17,Text36,Str17,Str19,Str21,Str25,Text68,
 Text69,Text70;

//---------------------------------------------------------------------------
__fastcall TCarica_File::TCarica_File(TComponent* Owner)
        : TForm(Owner)
{

}
//---------------------------------------------------------------------------

TDateTime dataFile;
static TDateTime dataFileTmp=0;
void __fastcall TCarica_File::FileListBox1Click(TObject *Sender)
{
  BitBtn2->Enabled=true;

  SSTmp = ExtractFileName(FileListBox1->FileName);
  Caption = ExtractFileName(FileListBox1->FileName);

  int G=FileAge(SSTmp);
  dataFileTmp = FileDateToDateTime(G);

  if(Avvio_OK==false)
  {
    AnsiString NewFileName = utils.GetProgramDataPath("temp.dyn");
    SSTmp=NewFileName;
  }
  else
  {
    Label4->Caption = Text42 + " " + FileDateToDateTime(G);
    FileListBox1->Hint= FileDateToDateTime(G);
  }
  ifstream infile(SSTmp.c_str(), ios::binary);  // carica la prova...
  infile.seekg (0, infile.end);
  int length = infile.tellg();
  infile.seekg (0, infile.beg);
  // carica il test 1
  infile.read((char *)Costruttore, sizeof (Costruttore));
  infile.read((char *)Modello, sizeof (Modello));
  infile.read((char *)Allestimento, sizeof (Allestimento));
  for(int i=0;i<8;i++)
  {
    infile.read((char *)vNomeTest[i], sizeof (vNomeTest[i]));
    if(i==0)
      infile.read((unsigned char *)iNum_Test_Bis , sizeof (iNum_Test_Bis));
    infile.read((unsigned char *)&datPrm[i],sizeof (datPrm[i]));
    infile.read((unsigned char *)vGiri[i] , sizeof (vGiri[i]));
    infile.read((unsigned char *)vCoppiaRuota[i] , sizeof (vCoppiaRuota[i]));
    infile.read((unsigned char *)vCoppiaRuota[i] , sizeof (vCoppiaRuota[i]));
    infile.read((unsigned char *)vPotenzaRuota[i], sizeof (vPotenzaRuota[i]));
    infile.seekg(sizeof(vPotenzaRuota[i])*4,infile.cur);
    infile.read((unsigned char *)vTermoK[i], sizeof (vTermoK[i]));
    infile.read((unsigned char *)vTermoOil[i],sizeof (vTermoOil[i]));
    infile.read((unsigned char *)vPresMAP[i],sizeof(vPresMAP[i]));
    infile.read((unsigned char *)vConsTot[i],sizeof(vConsTot[i]));
    infile.seekg(sizeof(vPotenzaRuota[i]),infile.cur);
    infile.read((char *)vNoteTest[i], sizeof (vNoteTest[i]));
    infile.read((unsigned char *)vCons[i],sizeof(vCons[i]));
  }

  if(length>233730)
  {
     infile.read((unsigned char *)Targa, sizeof (Targa));
     infile.read((unsigned char *)Cliente, sizeof (Cliente));
     cDynVersion=0x01;
#if 0
     if(length>234000)
     {
       cDynVersion=0x02;
        out.read((unsigned char*)tabStepGiri,sizeof(tabStepGiri));
        out.read((unsigned char*)tabStepCoppia,sizeof(tabStepCoppia));
        out.read((unsigned char*)tabStepPot,sizeof(tabStepPot));
        out.read((unsigned char*)tabStepCons,sizeof(tabStepCons));
        out.read((unsigned char*)tabStepLamb,sizeof(tabStepLamb));
        out.read((unsigned char*)tabStepMap,sizeof(tabStepMap));
        out.read((unsigned char*)tabStepTk,sizeof(tabStepTk));
     }
#endif
  }
  else
  {
    Targa[0]='\0';
    Cliente[0]='\0';
    cDynVersion=0x00;
  }
  infile.close();

  if(cDynVersion!=0x1)
  {
    for(int i=0;i<500;i++)
      for(int j=0;j<8;j++)
        if(vPotenzaRuota[j][i] && vCons[j][i])
          vCons[j][i]=vPotenzaRuota[j][i]*vCons[j][i]/(1.36*860);          
  }

  // visualizza i test contenuti nella sessione

  ListBox1->Items->Clear();
  if(iNum_Test_Bis[1])  ListBox1->Items->Add(vNomeTest[0]);
  if(iNum_Test_Bis[2])  ListBox1->Items->Add(vNomeTest[1]);
  if(iNum_Test_Bis[3])  ListBox1->Items->Add(vNomeTest[2]);
  if(iNum_Test_Bis[4])  ListBox1->Items->Add(vNomeTest[3]);
  if(iNum_Test_Bis[5])  ListBox1->Items->Add(vNomeTest[4]);
  if(iNum_Test_Bis[6])  ListBox1->Items->Add(vNomeTest[5]);
  if(iNum_Test_Bis[7])  ListBox1->Items->Add(vNomeTest[6]);
  if(iNum_Test_Bis[8])  ListBox1->Items->Add(vNomeTest[7]);

  ListBox1->ItemIndex=0;
  ListBox1Click(this);
}

//---------------------------------------------------------------------------
void __fastcall TCarica_File::ListBox1Click(TObject *Sender)
{
  char szBuffer[100];
  int i,j;

  BitBtn1->Enabled=true;
  BitBtn2->Enabled=true;
  j=-1;

  for (i=1;i<=9;i++)
  {
    if(iNum_Test_Bis[i])
    {
      j++;
      if(ListBox1->ItemIndex==j)
        break;
    }
  }

  memcpy(nomeTest,vNomeTest[i-1],sizeof(nomeTest));
  memcpy(Giri,vGiri[i-1],sizeof(Giri));
  memcpy(Note_test,vNoteTest[i-1],sizeof(Note_test));
  memcpy(fTermo_K,vTermoK[i-1],sizeof(fTermo_K));
  memcpy(fTermo_Oil,vTermoOil[i-1],sizeof(fTermo_Oil));
  memcpy(fPres_MAP,vPresMAP[i-1],sizeof(fPres_MAP));
  memcpy(fvettConsTot,vConsTot[i-1],sizeof(fvettConsTot));
  memcpy(Coppia_ruota,vCoppiaRuota[i-1],sizeof(Coppia_ruota));
  memcpy(Potenza_ruota,vPotenzaRuota[i-1],sizeof(Potenza_ruota));
  memcpy(vettCons,vCons[i-1],sizeof(vettCons));
  memcpy(&testPrm,&datPrm[i-1],sizeof(testPrm));

  // Anteprima grafici in finestra Caricamento file
  Series1->Clear();
  Series2->Clear();
  for(int t=0;t<500;t++)
  {
    if(Giri[t])
      Series2->AddXY(Giri[t],Coppia_ruota[t]*fK_convC,"",clTeeColor);
    if(Giri[t])
      Series1->AddXY(Giri[t],Potenza_ruota[t]*fK_convP,"", clTeeColor);
  }
  Series1->Visible=true;
  Series2->Visible=true;

}
//---------------------------------------------------------------------------



void __fastcall TCarica_File::BitBtn1Click(TObject *Sender)
{
    SS=SSTmp;
    dataFile=dataFileTmp;
    Main->Salvamisurazione1->Enabled=true;

//    Impostazioni->Edit23->Text=Costruttore;
    Impostazioni->ComboBoxCostruttore->Text=Costruttore;
//    Impostazioni->Edit24->Text=Modello;
    Impostazioni->ComboBoxModello->Text=Modello;
//    Impostazioni->Edit26->Text=Allestimento;
    Impostazioni->ComboBoxAllestimento->Text=Allestimento;
    Impostazioni->Costr=Costruttore;
    Impostazioni->Mod=Modello;
    Impostazioni->All=Allestimento;
    Main->Stampadeigrafici1->Enabled=true;
    Main->Stampadatideitest1->Enabled=true;

    // attiva visualizz. delle serie di test sul pannello
    Main->Panel33->Visible=true;
    Main->showPerformance(Main->addTest());
    BitBtn2->Enabled=false;
    BitBtn2->Enabled=false;
    Main->TabSheet5->TabVisible=true;
    Main->PageControl1->ActivePage=Main->TabSheet1;
    Close();

}
//---------------------------------------------------------------------------


void __fastcall TCarica_File::FileListBox1ContextPopup(TObject *Sender,
      TPoint &MousePos, bool &Handled)
{
  PopupMenu=PopupMenu1;
}
//---------------------------------------------------------------------------

void __fastcall TCarica_File::Eliminafile1Click(TObject *Sender)
{


 if(MessageDlg(Str25, mtWarning, TMsgDlgButtons() << mbYes << mbNo ,0)== mrYes)
 {
   DeleteFile(ExtractFileName(FileListBox1->FileName));
   DirectoryListBox1->Update();
   FileListBox1->Update();
  }

}
//---------------------------------------------------------------------------

void __fastcall TCarica_File::BitBtn2Click(TObject *Sender)
{
  int D=0;
  SS=SSTmp;
  dataFile=dataFileTmp;
  Main->Salvamisurazione1->Enabled=true;

  //Impostazioni->Edit23->Text=Costruttore;
  Impostazioni->ComboBoxCostruttore->Text=Costruttore;
  Impostazioni->ComboBoxModello->Text=Modello;
  Impostazioni->ComboBoxAllestimento->Text=Allestimento;


  Main->Stampadeigrafici1->Enabled=true;
  Main->Stampadatideitest1->Enabled=true;
  // controlla presenza di test in memoria
  bool Check=false;
  for(int t=1; t<=8; t++)
  if(vNumTest[t]) Check = true;

  if(Check==true)
    Main->Nuovasessioneditest1Click(this); // lancia nuova sessione di test

  D = ListBox1->Items->Capacity;
  for( t=0; t<D; t++)
  {
    ListBox1->ItemIndex=t;
    ListBox1Click(this);   // carica un test alla volta...
    Main->showPerformance(Main->addTest());
  }

  Main->Panel33->Visible=true;
  BitBtn2->Enabled=false;
  BitBtn2->Enabled=false;
  Main->TabSheet5->TabVisible=true;
  Main->PageControl1->ActivePage=Main->TabSheet1;
  Close();
}
//---------------------------------------------------------------------------


void __fastcall TCarica_File::FormShow(TObject *Sender)
{
  //carico il percorso predefinito
  char *cDocPath=new char[160];
  char *p=new char[200];
  SHGetFolderPath(NULL, CSIDL_PERSONAL,NULL,NULL,cDocPath);

  DirectoryListBox1->Drive=cDocPath[0];
  sprintf(p,"%s\\DynoTractorTest",cDocPath);
  DirectoryListBox1->Directory= p;
  FileListBox1->Update();
  delete [] p;
  delete [] cDocPath;
}
//---------------------------------------------------------------------------



