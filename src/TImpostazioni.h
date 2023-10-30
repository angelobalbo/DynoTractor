//---------------------------------------------------------------------------

#ifndef TImpostazioniH
#define TImpostazioniH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <AxCtrls.hpp>
#include <OleCtrls.hpp>
#include <VCF1.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <TabNotBk.hpp>
#include <Buttons.hpp>
#include <Mask.hpp>
#include "VrControls.hpp"
#include "VrRocker.hpp"
#include "VrLabel.hpp"
#include "NxCollection.hpp"
#include "NxEdit.hpp"
//---------------------------------------------------------------------------
class TImpostazioni : public TForm
{
__published:	// IDE-managed Components
        TGroupBox *GroupBox2;
        TGroupBox *GroupBox4;
        TStaticText *StaticText1;
        TPageControl *Test;
        TTabSheet *TabSheet1;
        TTabSheet *TabSheet3;
        TTabSheet *TabSheet4;
        TEdit *Edit1;
        TLabel *Label1;
        TStaticText *StaticText2;
        TEdit *Edit2;
        TLabel *Label2;
        TStaticText *StaticText3;
        TEdit *Edit3;
        TLabel *Label3;
        TStaticText *StaticText4;
        TStaticText *StaticText10;
        TLabel *Label9;
        TStaticText *StaticText11;
        TLabel *Label10;
        TStaticText *StaticText12;
        TLabel *Label11;
        TStaticText *StaticText13;
        TLabel *Label12;
        TStaticText *StaticText14;
        TLabel *Label13;
        TLabel *Label14;
        TStaticText *StaticText15;
        TStaticText *StaticText16;
        TLabel *Label15;
        TComboBox *ComboBox1;
        TGroupBox *GroupBox1;
        TPageControl *Rap;
        TTabSheet *TabSheet5;
        TLabel *Label16;
        TStaticText *StaticText21;
        TStaticText *StaticText22;
        TTabSheet *TabSheet7;
        TComboBox *ComboBox6;
        TStaticText *StaticText5;
        TStaticText *StaticText9;
        TStaticText *StaticText17;
        TLabel *Label4;
        TLabel *Label8;
        TLabel *Label18;
        TGroupBox *GroupBox8;
        TStaticText *StaticText37;
        TStaticText *StaticText38;
        TStaticText *StaticText39;
        TStaticText *StaticText42;
        TEdit *Edit28;
        TStaticText *StaticText41;
        TEdit *Edit27;
        TGroupBox *GroupBox9;
        TVrRocker *VrRocker1;
        TLabel *Label27;
        TLabel *Label28;
        TBitBtn *BitBtn1;
        TTabSheet *TabSheet11;
        TLabel *Label19;
        TLabel *Label29;
        TLabel *Label39;
        TLabel *Label46;
        TRadioGroup *RadioGroup1;
        TRadioButton *RadioButton1;
        TRadioButton *RadioButton2;
        TStaticText *StaticText6;
        TStaticText *StaticText7;
        TNxNumberEdit *NxNumberEdit2;
        TNxNumberEdit *NxNumberEdit3;
        TNxNumberEdit *NxNumberEdit4;
        TNxNumberEdit *NxNumberEdit5;
        TNxNumberEdit *NxNumberEdit6;
        TNxNumberEdit *NxNumberEdit7;
        TNxNumberEdit *NxNumberEdit10;
        TNxNumberEdit *NxNumberEdit11;
        TNxNumberEdit *NxNumberEdit12;
        TNxNumberEdit *NxNumberEdit13;
        TNxNumberEdit *NxNumberEdit14;
        TNxNumberEdit *NxNumberEdit15;
        TNxNumberEdit *NxNumberEdit16;
        TNxNumberEdit *NxNumberEdit17;
        TNxHeaderPanel *NxHeaderPanel1;
        TNxNumberEdit *NxNumberEdit18;
        TNxNumberEdit *NxNumberEdit19;
        TStaticText *StaticText28;
        TNxLabel *NxLabel1;
        TNxLabel *NxLabel2;
        TNxLabel *NxLabel3;
        TNxNumberEdit *NxNumberEdit1;
  TComboBox *ComboBoxCostruttore;
  TComboBox *ComboBoxModello;
  TComboBox *ComboBoxAllestimento;
        void __fastcall ComboBox1Change(TObject *Sender);
        void __fastcall Edit3Change(TObject *Sender);
        void __fastcall ComboBox6Change(TObject *Sender);
        void __fastcall Edit1Change(TObject *Sender);
        void __fastcall Edit2Change(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall Edit28Change(TObject *Sender);
        void __fastcall Edit27Change(TObject *Sender);
        void __fastcall VrRocker1LowerClick(TObject *Sender);
        void __fastcall VrRocker1UpperClick(TObject *Sender);
        void __fastcall NxNumberEdit1Change(TObject *Sender);
        void __fastcall RadioButton1Click(TObject *Sender);
        void __fastcall RadioButton2Click(TObject *Sender);
        void __fastcall NxNumberEdit3Change(TObject *Sender);
        void __fastcall NxNumberEdit4Change(TObject *Sender);
        void __fastcall NxNumberEdit5Change(TObject *Sender);
        void __fastcall NxNumberEdit6Change(TObject *Sender);
        void __fastcall NxNumberEdit7Change(TObject *Sender);
        void __fastcall NxNumberEdit10Change(TObject *Sender);
        void __fastcall NxNumberEdit11Change(TObject *Sender);
        void __fastcall NxNumberEdit12Change(TObject *Sender);
        void __fastcall NxNumberEdit13Change(TObject *Sender);
        void __fastcall NxNumberEdit14Change(TObject *Sender);
        void __fastcall NxNumberEdit15Change(TObject *Sender);
        void __fastcall NxNumberEdit16Change(TObject *Sender);
        void __fastcall NxNumberEdit17Change(TObject *Sender);
        void __fastcall NxNumberEdit18Change(TObject *Sender);
        void __fastcall NxNumberEdit19Change(TObject *Sender);
        void __fastcall NxNumberEdit2Change(TObject *Sender);
  void __fastcall BitBtn1Click(TObject *Sender);
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
  void __fastcall ComboBoxCostruttoreChange(TObject *Sender);
  void __fastcall ComboBoxModelloChange(TObject *Sender);
  void __fastcall ComboBoxAllestimentoChange(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TImpostazioni(TComponent* Owner);
        AnsiString Costr;
        AnsiString Mod;
        AnsiString All;        
};
//---------------------------------------------------------------------------
extern PACKAGE TImpostazioni *Impostazioni;
//---------------------------------------------------------------------------
#endif
