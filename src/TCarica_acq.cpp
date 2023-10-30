//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include<fstream.h>
#include <shfolder.h>
#include "TCarica_acq.h"
#include "TMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "NxCollection"
#pragma link "NxEdit"
#pragma resource "*.dfm"
TCarica_acq *Carica_acq;

  int byte_letti=0;

 extern char Note_acq[500],Buffer[100];

 extern int Pos_vector;

 bool bFileHasCons=false;

 extern float
       Giri_moto[7200],
       Giri_ifut[7200],
       Giri_toil[7200],
       Giri_terk[7200],
       Giri_pmap[7200],
       Giri_copp[7200],
       Giri_pote[7200],
       Giri_cons[7200],         //aggiunto dalla versione 12
       Acq_dati[100];

//---------------------------------------------------------------------------
__fastcall TCarica_acq::TCarica_acq(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------


void __fastcall TCarica_acq::FileListBox1Click(TObject *Sender)
{
   String SS;
  SS = ExtractFileName(FileListBox1->FileName);
  int G=FileAge(SS);

 ifstream in(SS.c_str(), ios::out | ios::binary);  // salva la prova...

 in.read((char *) Note_acq, sizeof (Note_acq));
 in.read((unsigned char *) Acq_dati, sizeof (Acq_dati));
 in.read((unsigned char *) Giri_moto, sizeof (Giri_moto));
 in.read((unsigned char *) Giri_copp, sizeof (Giri_copp));
 in.read((unsigned char *) Giri_pote, sizeof (Giri_pote));
 in.read((unsigned char *) Giri_pmap, sizeof (Giri_pmap));
 in.read((unsigned char *) Giri_terk, sizeof (Giri_terk));
 in.read((unsigned char *) Giri_toil, sizeof (Giri_toil));
 in.read((unsigned char *) Giri_ifut, sizeof (Giri_ifut));

 try
 {
        if (!in.eof())
        {
                in.read((unsigned char *) Giri_cons, sizeof (Giri_cons));
                bFileHasCons=true;
        }
 }
 catch(Exception &ex)
 {
        //i file vecchi non hanno la parte di consumi
        bFileHasCons=false;
 }
 in.close();


 NxMemo1->Text=Note_acq;
 NxLabel13->Caption = FileDateToDateTime(G);


 NxButton6->Enabled=true;

   // Visualizza i dati del file salvato...


  // Tempo di acquisizione
  sprintf(Buffer,"%-1.1f",((Acq_dati[1]/1000)*Acq_dati[0])/60 );
  NxLabel15->Caption = Buffer + String(" Min.");


  // Tempo di campionamento
  sprintf(Buffer,"%.f",Acq_dati[1]);
  NxLabel17->Caption = Buffer + String(" ms");

   // Rapporto di trasmissione
  sprintf(Buffer,"%-1.2f",Acq_dati[2]);
  NxLabel23->Caption = Buffer;

   // Rendimento della trasmissione
  sprintf(Buffer,"%-1.2f",Acq_dati[3]);
  NxLabel19->Caption = Buffer;



  /*

  // misura pneumatico
  char Buffer2[30],Buffer3[30];
  sprintf(Buffer,"%.f",Dat_ATB[1]);
  sprintf(Buffer2,"%.f",Dat_ATB[2]);
  sprintf(Buffer3,"%-1.1f",Dat_ATB[4]);
  NxLabel17->Caption = Buffer + String("/") + Buffer2 + String("/") + Buffer3;

  // Diametro ruota mm
  sprintf(Buffer,"%.f",Dat_ATB[3]);
  NxLabel23->Caption = Buffer;


  // Pigone
  sprintf(Buffer,"%.f",Dat_ATB[5]);
  NxLabel19->Caption = Buffer;

  // Corona
  sprintf(Buffer,"%.f",Dat_ATB[6]);
  NxLabel21->Caption = Buffer;
      */





}
//---------------------------------------------------------------------------

void __fastcall TCarica_acq::NxButton6Click(TObject *Sender)
{
// disattiva timer connessione

NxButton6->Enabled=false;
Main->Series1->Clear();
Main->Series2->Clear();
Main->Series98->Clear();
//Main->Series99->Clear();
Main->Series101->Clear();
Main->Series102->Clear();
Main->Series103->Clear();

 for(int t=0; t<Acq_dati[0]; t++)
 {

  Main->Series98->AddXY(t,Giri_moto[t],"",clTeeColor);  // temperatura del motore


  Main->Series1->AddXY(t,Giri_copp[t],"",clTeeColor);  // temperatura dei gas


  Main->Series2->AddXY(t,Giri_pote[t],"",clTeeColor); // giri motore


  Main->Series103->AddXY(t,Giri_pmap[t],"",clTeeColor); // velocità



  Main->Series102->AddXY(t,Giri_terk[t],"",clTeeColor); // velocità


  Main->Series101->AddXY(t,Giri_toil[t],"",clTeeColor); // velocità


//  Main->Series99->AddXY(t,Giri_ifut[t],"",clTeeColor); // velocità

  if (bFileHasCons)
  {
        //inserisco anche i cunsumi
        Main->Series8->AddXY(t,Giri_cons[t],"",clTeeColor); // consumi
  }



 }

 Main->NxButton1->Enabled=false; // disattiva pulsante start
 Main->NxButton8->Enabled=true; // attiva pulsante cancella
 Close();


}
//---------------------------------------------------------------------------

void __fastcall TCarica_acq::FormShow(TObject *Sender)
{
  char cDocPath[MAX_PATH];
  char cPath[MAX_PATH];

  SHGetFolderPath(NULL, CSIDL_PERSONAL,NULL,NULL,cDocPath);

  DirectoryListBox1->Drive=cDocPath[0];
  sprintf(cPath,"%s\\DynoTractorTest",cDocPath);
  DirectoryListBox1->Directory= cPath;
  FileListBox1->Update();

}
//---------------------------------------------------------------------------

