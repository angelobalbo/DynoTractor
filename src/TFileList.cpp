//---------------------------------------------------------------------------

#include <vcl.h>
#include <fstream.h>
#pragma hdrstop

#include "TFileList.h"
#include "TMain.h"
#include "TImpostazioni.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

 extern char
              Cliente[30],
              Targa[20],
              Costruttore[30],
              Modello[30],
              Allestimento[30];

TfrmFileList *frmFileList;

//---------------------------------------------------------------------------
__fastcall TfrmFileList::TfrmFileList(TComponent* Owner)
  : TForm(Owner)
{
}                                  
//---------------------------------------------------------------------------




void __fastcall TfrmFileList::BitBtn2Click(TObject *Sender)
{
 char line[MAX_PATH]; /*< lettura linea da file ingresso*/
  String sRow; /*<riga in formato AnsiString da interpretare*/
  String sField; /*< campo da inserire nei dati impostazioni prova*/
  ofstream outFile;
  String sTitle;
  if(flb->FileName=="")
  {
    sTitle="File not selected";
    MessageDlg(sTitle,mtWarning,TMsgDlgButtons() << mbOK,0);
    return;
  }


  Main->sFileImported=flb->FileName;

  ifstream inFile(flb->FileName.c_str(), ios::binary);  // carica impostazioni
  if(!inFile.is_open())
  {
    ShowMessage("File not found!");
  }
  else
  {
    if(Main->sFileImported!="")
    {
      Main->sFileImported=UpperCase(Main->sFileImported);
      if(Main->sOutPath=="")
        Main->sOutPath="C:";
      Main->sFileExported=Main->sOutPath+"\\"+
                          ExtractFileName(Main->sFileImported);
      Main->sFileExported=Main->sFileExported.SubString(0,Main->sFileExported.Length()-2);
      Main->sFileExported+="tmp";
      outFile.open(Main->sFileExported.c_str(),ios_base::out | ios::binary);
      if(outFile.is_open())
      {
        while(inFile.eof()==false)
        {
          inFile.getline(line,MAX_PATH);
          sRow=line;
          sField="";
          if(sRow.AnsiPos("=")>0)
          {
            sField=sRow.SubString(sRow.AnsiPos("=")+1,sRow.Length()-sRow.AnsiPos("="));
            sField=Trim(sField);
            if(sField.Length()>29)
              sField=sField.SubString(0,29);
          }
          sRow=StringReplace(sRow,"=",";",TReplaceFlags() << rfReplaceAll);
          outFile << sRow.c_str() << endl;
          sRow=LowerCase(sRow);
          if(sRow.AnsiPos("client name"))
          {
            strcpy(Cliente,sField.c_str());
            sTitle=Impostazioni->StaticText41->Caption+" "+sField;
          }
          else if(sRow.AnsiPos("vehicle plate"))
          {
            strcpy(Targa,sField.c_str());
            sTitle+="\n\n"+Impostazioni->StaticText42->Caption+" "+sField;
          }
          else if(sRow.AnsiPos("manufacturer"))
            strcpy(Costruttore,sField.c_str());
          else if(sRow.AnsiPos("model"))

            strcpy(Modello,sField.c_str());
          else if(sRow.AnsiPos("type"))
            strcpy(Allestimento,sField.c_str());
        }
        MessageDlg(sTitle,mtInformation,TMsgDlgButtons() << mbOK,0);
        outFile.close();
        Main->Savetest1->Enabled=true;
        //Main->sbDataIn->Enabled=false;
        //Main->sbDataOut->Enabled=true;
      }
      inFile.close();

      Impostazioni->ComboBoxCostruttore->Text=Costruttore;
      Impostazioni->ComboBoxModello->Text=Modello;
      Impostazioni->ComboBoxAllestimento->Text=Allestimento;

      Impostazioni->Edit28->Text=Targa;
      Impostazioni->Edit27->Text=Cliente;
      //Main->Savetest1->Enabled=false;
      Close();

    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmFileList::FormShow(TObject *Sender)
{
  flb->FileName="";

}
//---------------------------------------------------------------------------

