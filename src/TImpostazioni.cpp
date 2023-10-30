//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TImpostazioni.h"
#include <iostream.h>
#include <shfolder.h>
#include "TMain.h"
#include "TGiri_Mot.h"
#include "TSetup.h"
#include "TOpzioni_grafici.h"
#include "CUtils.h"
#include "Structs.h"
#include "CDyra.h"
#include "log.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "VrControls"
#pragma link "VrRocker"
#pragma link "VrLabel"
#pragma link "NxCollection"
#pragma link "NxEdit"
#pragma resource "*.dfm"
TImpostazioni *Impostazioni;

  extern bool bOnInit; 
 extern short tipoBanco,Unit_Vel,
              sD_ruota_A,sL,sH,sR,Tipo_cambio,
              Marcia_test,Vel_ini_tac,Step_vel_tac,
              sVentola,sIndice,sCammes;


 extern int  Reg_iniz_acc, Reg_fine_acc, Tempo_acc,
             Reg_iniz_dec, Reg_fine_dec, Tempo_dec,Tempo_staz,
             Passiva_calc,Reg_staz, Vel_iniz, Vel_fine,
             Vel_staz, rpmStart,  rpmStop,
             Delta_reg,Tipo_test,
             tipoRapp ,Reg_calc_rap ,
             N_Cilindri,N_Tempi,
             Toll_rap ,OK_calc,Num_l,OK_calc_,Num_ll,
             Filtro_Rpm,KV_Int_calc,Tipo_norm;

 extern float Rap_tot1,Val_C_cost,DATI_VARI[500],fK_convV,N_mot_rap,N_pto_rap,Rend_PTO, fK_convC;

 extern double fCx,fS,fPendenza,fKrotolamento,Soglia_min,fPeso_veicolo,fTemp_aria,
             fPres_aria,fUmidita;


 extern char  Buffer[100],
              Cliente[30],
              Targa[20],
              NOTE[500],
              Costruttore[30],
              Modello[30],
              Allestimento[30];



 extern bool Mod_imp,OK_TEST_AUTO,OK_POT_rend_mot;
 extern String Str11,Text200,Text201,Text37,Text38,Str1,Str2,Text206,Text207;
 extern unsigned char  flagConsumi,statusRelaisConfig;



 extern unsigned char           flagConsumi,consumiGiaAttivo;
 extern int                     Pos_consumi;
 extern short unsigned int      uiPrecImpulsiCons;
 extern float                   litri_h,litri_tot;

 extern unsigned int selPto;




//---------------------------------------------------------------------------
__fastcall TImpostazioni::TImpostazioni(TComponent* Owner)
        : TForm(Owner)
{

}
//---------------------------------------------------------------------------
void __fastcall TImpostazioni::ComboBox1Change(TObject *Sender)
{
  if(bOnInit) return;
 // stabilisce il tipo di test da eseguire
  Mod_imp=true; // ok per salvataggio

  switch(ComboBox1->ItemIndex)
  {
    case 0:
     Tipo_test=0;
    break;
    case 1:
     Tipo_test=1;
    break;
    case 2:
     Tipo_test=4;
    break;
    case 3:
     Tipo_test=5;
    break;
    case 4:
     Tipo_test=2;
    break;
    default:
    break;
  }

 //Test->ActivePageIndex=ComboBox1->ItemIndex;

  Main->SpeedButton4->Down=false;
  Main->SpeedButton8->Down=false;
  Main->SpeedButton21->Down=false;
  Main->SpeedButton17->Down=false;
  Main->SpeedButton15->Down=false;


  Main->Caption=ComboBox1->Text;

  //ComboBox6->Enabled=true;
  Rap->Visible=true;
  switch(Tipo_test)
  {
            case 0:
            Main->SpeedButton4->Down=true;
            Test->ActivePageIndex=1;
            break;
            case 1:
            Test->ActivePageIndex=2;
            Main->SpeedButton8->Down=true;
            break;
            case 2:
            Test->ActivePageIndex=3;
            Main->SpeedButton15->Down=true;
            break;
            case 4:
            Test->ActivePageIndex=4;
            Main->SpeedButton21->Down=true;
            break;
            case 5:
            Test->ActivePageIndex=5;
            Main->SpeedButton17->Down=true;
            //ComboBox6->Enabled=false;
            Rap->Visible=false;
            break;
            default:
            break;
            }
}
//---------------------------------------------------------------------------











void __fastcall TImpostazioni::Edit3Change(TObject *Sender)
{
  if(bOnInit) return;
  Mod_imp=true; // ok per salvataggio
  Tempo_acc = StrToIntDef(Edit3->Text,Tempo_acc);
  if(Tempo_acc < 1) Edit3->Text=1;
}
//---------------------------------------------------------------------------

void __fastcall TImpostazioni::ComboBox6Change(TObject *Sender)
{
  if(bOnInit) return;
 Mod_imp=true; // ok per salvataggio
 Rap->ActivePageIndex=ComboBox6->ItemIndex;
 tipoRapp=(ComboBox6->ItemIndex==0)?RAP_RPMFIX:RAP_MANUAL;

 if(tipoRapp==RAP_MANUAL)
 {
  Main->SpeedButton1->Enabled=false;
   if( N_pto_rap)
  Rap_tot1 = N_mot_rap / N_pto_rap;

 }

 else Main->SpeedButton1->Enabled=true;

}
//--------------------------------------------------------------------------

void __fastcall TImpostazioni::Edit1Change(TObject *Sender)
{
  if(bOnInit) return;
 Mod_imp=true; // ok per salvataggio
Reg_iniz_acc = StrToIntDef(Edit1->Text,Reg_iniz_acc);
Giri_Mot->AnalogMeterHQ1->GreenStartValue=Reg_iniz_acc;


}
//---------------------------------------------------------------------------

void __fastcall TImpostazioni::Edit2Change(TObject *Sender)
{
  if(bOnInit) return;
 Mod_imp=true; // ok per salvataggio
Reg_fine_acc = StrToIntDef(Edit2->Text,Reg_fine_acc);

}
//---------------------------------------------------------------------------












void __fastcall TImpostazioni::FormShow(TObject *Sender)
{

  char fil[MAX_PATH];
  String path;
  int tot;
  int ind;
  bool f;

  path=utils.GetProgramDataPath("");
  sprintf(fil,"%sDB\\producers.txt",utils.GetProgramDataPath("").c_str());
  //carico il db delle marche
  //controllo se è gia stato creato
  f=FileExists(fil);
  //carico il file contente l'elenco delle marche
  ComboBoxCostruttore->Clear();
  if (f) ComboBoxCostruttore->Items->LoadFromFile(fil);
  //carico i dati (se presenti)
  if (Costruttore!="" && (f))
  {
    ind=ComboBoxCostruttore->Items->IndexOf(Costruttore);
    if(ind>-1)
      ComboBoxCostruttore->ItemIndex=ind;
    else
    {
      ComboBoxCostruttore->Items->Add(Costruttore);
      ind=ComboBoxCostruttore->Items->IndexOf(Costruttore);
      ComboBoxCostruttore->ItemIndex=ind;
    }
  }
  //per caricare il db dei modelli
  //Application->ProcessMessages(); // delete 2.3.6.2
  //carico il file contenente i modelli della marca selezionata
  if (Costruttore!="")
  {
    sprintf(fil,"%sDB\\M_%s.txt",path,Costruttore);
    //controllo se è gia stato creato il file con i modelli della marca selezionata
    f=FileExists(fil);
  }
  else
    f=false;

  //carico il file con l'elenco dei modelli

  if (f) ComboBoxModello->Items->LoadFromFile(fil);
  if (Modello!="" && (f))
  {
    ComboBoxModello->Clear();
    ind=ComboBoxModello->Items->IndexOf(Modello);
     if(ind>-1)
      ComboBoxModello->ItemIndex=ind;
    else
    {
      ComboBoxModello->Items->Add(Modello);
      ind=ComboBoxModello->Items->IndexOf(Modello);
      ComboBoxModello->ItemIndex=ind;
    }
  }

  if ((Modello!="") && (Costruttore!=""))
  {
    sprintf(fil,"%sDB\\M_%s_%s.txt",path,Costruttore,Modello);
    //controllo se è gia stato creato il file con i modelli della marca selezionata
    f=FileExists(fil);
  }
  else
    f=false;

  //carico il file con l'elenco dei modelli

  if (f) ComboBoxAllestimento->Items->LoadFromFile(fil);
  if (Allestimento!="" && (f))
  {
    ComboBoxAllestimento->Clear();
    ind=ComboBoxAllestimento->Items->IndexOf(Allestimento);
    if(ind>-1)
      ComboBoxAllestimento->ItemIndex=ind;
    else
    {
      ComboBoxAllestimento->Items->Add(Allestimento);
      ind=ComboBoxAllestimento->Items->IndexOf(Allestimento);
      ComboBoxAllestimento->ItemIndex=ind;
    }
  }

  ComboBox6->Enabled=(selPto==0);

  if(selPto==1)
  {
    RadioButton1->Checked=true;
  }
  else
  {
    RadioButton2->Checked=true;
  }
  NxNumberEdit2->Value=Rend_PTO;

  Rap->Visible=true;
  switch (Tipo_test)
  {
    case TEST_STEP_TIMED:
      ComboBox1->ItemIndex=0;
      Test->ActivePageIndex=1;
    break;
    case TEST_STEP:
      ComboBox1->ItemIndex=1;
      Test->ActivePageIndex=2;
    break;
    case TEST_DECEL:
      Test->ActivePageIndex=3;
      ComboBox1->ItemIndex=4;
    break;
    case TEST_COST_TRQ:
      ComboBox1->ItemIndex=2;
      Test->ActivePageIndex=4;
  break;
  case TEST_BRAKE_MAN:
    Test->ActivePageIndex=-1;
    ComboBox1->ItemIndex=3;
    Rap->Visible=false;
  break;
  default:
  break;
 }

 Edit1->Text=Reg_iniz_acc;
 Edit2->Text=Reg_fine_acc;
 Edit3->Text=Tempo_acc;

 NxNumberEdit15->Text=Reg_iniz_dec;
 NxNumberEdit16->Text=Reg_fine_dec;
 NxNumberEdit17->Text=Tempo_dec;

 NxNumberEdit18->Text=N_mot_rap;
 NxNumberEdit19->Text=N_pto_rap;

 NxNumberEdit3->Text=rpmStart;
 NxNumberEdit4->Text=rpmStop;
 NxNumberEdit5->Text=Delta_reg;
 NxNumberEdit6->Text=Tempo_staz;
 NxNumberEdit13->Text=Val_C_cost*fK_convC;


 NxNumberEdit10->Text=rpmStart;
 NxNumberEdit11->Text=rpmStop;
 NxNumberEdit12->Text=Delta_reg;

 Rap->ActivePageIndex=(tipoRapp==RAP_RPMFIX)?0:1;

 ComboBox6->ItemIndex=Rap->ActivePageIndex;

 NxNumberEdit7->Text=Reg_calc_rap;

 NxNumberEdit14->Text= Soglia_min;
  if(sVentola)
  {
  Label28->Font->Color=clGreen;
  Label27->Font->Color=clGray;
  VrRocker1->State=stLowerDown;
  }
  else
  {
  VrRocker1->State=stUpperDown;
  Label28->Font->Color=clGray;
  Label27->Font->Color=clGreen;
  }

//  Edit23->Text=Costruttore;
//  Edit24->Text=Modello;
//  Edit26->Text=Allestimento;
  Edit28->Text=Targa;
  Edit27->Text=Cliente;
  NxNumberEdit1->Value=Rap_tot1;

}
//---------------------------------------------------------------------------







void __fastcall TImpostazioni::Edit28Change(TObject *Sender)
{
   int Size = Edit28->GetTextLen();
  if(bOnInit) return;
  Size++;
  char *Buffer = new char[Size];
  Edit28->GetTextBuf(Buffer,Size);
  for(int t=0;t<=Size;t++)  Targa[t]=Buffer[t];
  delete Buffer;

}
//---------------------------------------------------------------------------

void __fastcall TImpostazioni::Edit27Change(TObject *Sender)
{
  int Size = Edit27->GetTextLen();
  if(bOnInit) return;
  Size++;
  char *Buffer = new char[Size];
  Edit27->GetTextBuf(Buffer,Size);
  for(int t=0;t<=Size;t++)  Cliente[t]=Buffer[t];
  delete Buffer;

}
//---------------------------------------------------------------------------

void __fastcall TImpostazioni::VrRocker1LowerClick(TObject *Sender)
{
  if(bOnInit) return;
  try
  {
    Label28->Font->Color=clGreen;
    Label27->Font->Color=clGray;
    // Stop Giri rullo
    g_dyraControl.CmdStop();
    statusRelaisConfig=statusRelaisConfig|0x08;
    g_dyraControl.CmdRele(statusRelaisConfig);
    g_dyraControl.CmdStart();
    sVentola=1;
  } // try...
  catch (EAPDException& E) // Eccezione della seriale se non presente o altro
  {
    MessageBeep(MB_ICONEXCLAMATION);
    if(MessageDlg(Str11,mtConfirmation,TMsgDlgButtons() << mbYes << mbNo,0) == mrYes)
      Setup->ShowModal();
  }
}
//---------------------------------------------------------------------------

void __fastcall TImpostazioni::VrRocker1UpperClick(TObject *Sender)
{
  if(bOnInit) return;
  try
  {
    Label28->Font->Color=clGray;
    Label27->Font->Color=clGreen;
    g_dyraControl.CmdStop();
    statusRelaisConfig=statusRelaisConfig&0xF7;
    // Stop ventola e impostazione manuale
    g_dyraControl.CmdRele(statusRelaisConfig);
    g_dyraControl.CmdStart();
    sVentola=0;
  } // try...
  catch (EAPDException& E) // Eccezione della seriale se non presente o altro
  {
    MessageBeep(MB_ICONEXCLAMATION);
    if(MessageDlg(Str11,mtConfirmation,TMsgDlgButtons() << mbYes << mbNo,0) == mrYes)
      Setup->ShowModal();
  }
}
//---------------------------------------------------------------------------

void __fastcall TImpostazioni::NxNumberEdit1Change(TObject *Sender)
{
  if(bOnInit) return;
  if (selPto==0)
    Rap_tot1=NxNumberEdit1->Value;
}
//---------------------------------------------------------------------------


void __fastcall TImpostazioni::RadioButton1Click(TObject *Sender)
{

  // GIRI SU PTO rapporto=1
  ComboBox6->ItemIndex=0;
  ComboBox6->Enabled=false;
  tipoRapp=RAP_RPMFIX;
  Rap->ActivePageIndex=ComboBox6->ItemIndex;
  ComboBox6->Enabled=false;
  NxNumberEdit2->Enabled=false;
  Main->SpeedButton1->Enabled=false;
  Main->Panel5->Caption=Text201;
  //PTO_CONN = 1;
  Rap_tot1=1; // Rapporto diretto
  NxNumberEdit1->Value=Rap_tot1;
  OK_POT_rend_mot=false;
  NxNumberEdit2->Enabled=false;
  Rend_PTO=1;
  NxNumberEdit2->Value=Rend_PTO;
  selPto = 1;
  Opzioni_grafici->CheckBox10->Caption = Text206;
  Opzioni_grafici->CheckBox12->Caption = Text206;
}
//---------------------------------------------------------------------------

void __fastcall TImpostazioni::RadioButton2Click(TObject *Sender)
{

  ComboBox6->Enabled=true;
  NxNumberEdit2->Enabled=true;
  Main->SpeedButton1->Enabled=true;
  Main->Panel5->Caption=Text200;
  if(N_pto_rap && N_mot_rap && tipoRapp==RAP_MANUAL)
    Rap_tot1=N_mot_rap/N_pto_rap;
  NxNumberEdit1->Text= Rap_tot1;
  OK_POT_rend_mot=true;
  NxNumberEdit2->Enabled=true;
  Rend_PTO= NxNumberEdit2->Value;
  selPto = 0;
  Opzioni_grafici->CheckBox10->Caption = Text207;
  Opzioni_grafici->CheckBox12->Caption = Text207;
}
//---------------------------------------------------------------------------

void __fastcall TImpostazioni::NxNumberEdit3Change(TObject *Sender)
{
  if(bOnInit) return;
  Mod_imp=true; // ok per salvataggio
  rpmStart = NxNumberEdit3->Value;
}
//---------------------------------------------------------------------------

void __fastcall TImpostazioni::NxNumberEdit4Change(TObject *Sender)
{
  if(bOnInit) return;
  Mod_imp=true; // ok per salvataggio
  rpmStop = NxNumberEdit4->Value;
}
//---------------------------------------------------------------------------

void __fastcall TImpostazioni::NxNumberEdit5Change(TObject *Sender)
{
  if(bOnInit) return;
  Mod_imp=true; // ok per salvataggio
  Delta_reg = NxNumberEdit5->Value;
}
//---------------------------------------------------------------------------


void __fastcall TImpostazioni::NxNumberEdit6Change(TObject *Sender)
{
  if(bOnInit) return;
  Mod_imp=true; // ok per salvataggio
  Tempo_staz = NxNumberEdit6->Value;
  Main->ResetConsumi();
}
//---------------------------------------------------------------------------

void __fastcall TImpostazioni::NxNumberEdit7Change(TObject *Sender)
{
  if(bOnInit) return;
  Reg_calc_rap=NxNumberEdit7->Value;
}
//---------------------------------------------------------------------------

void __fastcall TImpostazioni::NxNumberEdit10Change(TObject *Sender)
{
  if(bOnInit) return;
  Mod_imp=true; // ok per salvataggio
  rpmStart = NxNumberEdit10->Value;
}
//---------------------------------------------------------------------------

void __fastcall TImpostazioni::NxNumberEdit11Change(TObject *Sender)
{
  if(bOnInit) return;
  Mod_imp=true; // ok per salvataggio
  rpmStop=NxNumberEdit11->Value;
}
//---------------------------------------------------------------------------

void __fastcall TImpostazioni::NxNumberEdit12Change(TObject *Sender)
{
  if(bOnInit) return;
  Mod_imp=true; // ok per salvataggio
  Delta_reg = NxNumberEdit12->Value;
}
//---------------------------------------------------------------------------


void __fastcall TImpostazioni::NxNumberEdit13Change(TObject *Sender)
{
  if(bOnInit) return;
  Val_C_cost = float(NxNumberEdit13->Value)/fK_convC;
}
//---------------------------------------------------------------------------

void __fastcall TImpostazioni::NxNumberEdit14Change(TObject *Sender)
{
  if(bOnInit) return;
  Soglia_min = NxNumberEdit14->Value;
}
//---------------------------------------------------------------------------

void __fastcall TImpostazioni::NxNumberEdit15Change(TObject *Sender)
{
  if(bOnInit) return;
  Mod_imp=true; // ok per salvataggio
  Reg_iniz_dec = NxNumberEdit15->Value;
}
//---------------------------------------------------------------------------

void __fastcall TImpostazioni::NxNumberEdit16Change(TObject *Sender)
{
  if(bOnInit) return;
  Mod_imp=true; // ok per salvataggio
  Reg_fine_dec = NxNumberEdit16->Value;
}
//---------------------------------------------------------------------------

void __fastcall TImpostazioni::NxNumberEdit17Change(TObject *Sender)
{
  if(bOnInit) return;
  Mod_imp=true; // ok per salvataggio
  Tempo_dec = NxNumberEdit17->Value;
}
//---------------------------------------------------------------------------

void __fastcall TImpostazioni::NxNumberEdit18Change(TObject *Sender)
{
  if(bOnInit) return;
  N_mot_rap = NxNumberEdit18->Value;
  if(N_pto_rap && N_mot_rap && selPto==0)
    Rap_tot1=N_mot_rap/N_pto_rap;
  NxNumberEdit1->Value= Rap_tot1;
}
//---------------------------------------------------------------------------

void __fastcall TImpostazioni::NxNumberEdit19Change(TObject *Sender)
{
  if(bOnInit) return;
  N_pto_rap = NxNumberEdit19->Value;
  if(N_pto_rap && N_mot_rap && selPto==0)
    Rap_tot1=N_mot_rap/N_pto_rap;
  NxNumberEdit1->Value= Rap_tot1;
}
//---------------------------------------------------------------------------

void __fastcall TImpostazioni::NxNumberEdit2Change(TObject *Sender)
{
  if(bOnInit) return;
  Rend_PTO = NxNumberEdit2->Value;
  if(!Rend_PTO )Rend_PTO=1;
}
//---------------------------------------------------------------------------

void __fastcall TImpostazioni::BitBtn1Click(TObject *Sender)
{
  char fil[MAX_PATH];

  if (ComboBoxCostruttore->Text!="")
        {
                int ind=ComboBoxCostruttore->Items->IndexOf(ComboBoxCostruttore->Text);
                if (ind==-1)
                {
                        //se non è presente lo inserisco
                        ComboBoxCostruttore->Items->Add(ComboBoxCostruttore->Text);
                }
        }

        //cerco se è gia presente il modello
        if (ComboBoxModello->Text!=""){
                int ind=ComboBoxModello->Items->IndexOf(ComboBoxModello->Text);
                if (ind==-1) {
                        //se non è presente lo inserisco
                        ComboBoxModello->Items->Add(ComboBoxModello->Text);
                }
        }
        //cerco se è gia presente l'allestimento
        if (ComboBoxAllestimento->Text!="")
        {
           int ind=ComboBoxAllestimento->Items->IndexOf(ComboBoxAllestimento->Text);
             if (ind==-1)
             {
               //se non è presente lo inserisco
               ComboBoxAllestimento->Items->Add(ComboBoxAllestimento->Text);
             }
        }

        //salvo il database dei costruttori
        String path=utils.GetProgramDataPath("");
        sprintf(fil,"%sDB\\producers.txt",path);
        try
        {
           ComboBoxCostruttore->Items->SaveToFile(fil);
        }
        catch(Exception &e)
        {
        }
        //salvo il DB dei modelli riferiti al costruttore

        sprintf(fil,"%sDB\\M_%s.txt",path,ComboBoxCostruttore->Text.c_str());
        //posso salvarlo solo se è inserito un costruttore
        try
        {
          if (Costruttore!="") ComboBoxModello->Items->SaveToFile(fil);
        }
        catch(Exception &e)
        {
        }
        //salvo il DB degli allestimenti riferiti al modello
        sprintf(fil,"%sDB\\M_%s_%s.txt",path,ComboBoxCostruttore->Text.c_str(),ComboBoxModello->Text.c_str());
        //posso salvarlo solo se sono inseriti marca e modello
        try
        {
          if ((Costruttore!="") && (Modello!="")) ComboBoxAllestimento->Items->SaveToFile(fil);
        }
        catch(Exception &e)
        {
        }
  utils.CreateSaveDir();
  Main->SpeedButton18->Enabled=true;  
}
//---------------------------------------------------------------------------



void __fastcall TImpostazioni::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  Main->SpeedButton18->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TImpostazioni::ComboBoxCostruttoreChange(TObject *Sender)
{
  char fil[MAX_PATH];
   if(bOnInit) return;
  //modificato il costruttore
  //prelevo la stringa immessa o selezionata
  Costr=ComboBoxCostruttore->Text;   //dovrebbe essere troncato a 25

  memset(Costruttore,0,sizeof(Costruttore));
  memcpy(Costruttore,Costr.c_str(),sizeof(Costruttore)-1);

  //carico il file contenente i modelli della marca selezionata
  try
  {
        String path=utils.GetProgramDataPath("");
        sprintf(fil,"%sDB\\M_%s.txt",path,Costr);
        //controllo se è gia stato creato il file con i modelli della marca selezionata
        bool f=FileExists(fil);
        //carico il file con l'elenco dei modelli
        ComboBoxModello->Clear();
        ComboBoxAllestimento->Clear();
        if (f) ComboBoxModello->Items->LoadFromFile(fil);
  }
  catch(Exception &e)
  {
  }
}
//---------------------------------------------------------------------------

void __fastcall TImpostazioni::ComboBoxModelloChange(TObject *Sender)
{
  char fil[MAX_PATH];
   if(bOnInit) return;
  //modificato il modello
  //prelevo la stringa immessa o selezionata
  Mod=ComboBoxModello->Text;  //dovrebbe essere troncato a 25
  memset(Modello,0,sizeof(Modello));
  memcpy(Modello,Mod.c_str(),sizeof(Modello)-1);
  //carico il file contenente gli allestimenti del modello selezionato

  try
  {
        String path=utils.GetProgramDataPath("");
        sprintf(fil,"%sDB\\M_%s_%s.txt",path,Costr,Mod);
        //controllo se è gia stato creato il file con i modelli della marca selezionata
        bool f=FileExists(fil);
        //carico il file con l'elenco dei modelli
        ComboBoxAllestimento->Clear();
        if (f) ComboBoxAllestimento->Items->LoadFromFile(fil);
  }
  catch(Exception &e)
  {
  }

}
//---------------------------------------------------------------------------

void __fastcall TImpostazioni::ComboBoxAllestimentoChange(TObject *Sender)
{
//modifica allestimento
//prelevo la stringa immessa o selezionata
   if(bOnInit) return;
  All=ComboBoxAllestimento->Text;
  memset(Allestimento,0,sizeof(Allestimento));
  memcpy(Allestimento,All.c_str(),sizeof(Allestimento)-1);
}
//---------------------------------------------------------------------------

