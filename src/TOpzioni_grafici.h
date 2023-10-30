//---------------------------------------------------------------------------

#ifndef TOpzioni_graficiH
#define TOpzioni_graficiH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TOpzioni_grafici : public TForm
{
__published:	// IDE-managed Components
        TBitBtn *BitBtn1;
        TGroupBox *GroupBox1;
        TCheckBox *CheckBox10;
        TBevel *Bevel1;
        TGroupBox *GroupBox2;
        TGroupBox *GroupBox4;
        TCheckBox *CheckBox4;
        TCheckBox *CheckBox5;
        TCheckBox *CheckBox8;
        TCheckBox *CheckBox12;
    TCheckBox *CheckBoxConsumi;
  TRadioButton *rbGKWh;
  TRadioButton *rbL;
  TCheckBox *cbPZeroScale;
  TCheckBox *cbCZeroScale;
        void __fastcall CheckBox12Click(TObject *Sender);
        void __fastcall CheckBox10Click(TObject *Sender);
        void __fastcall CheckBox4Click(TObject *Sender);
        void __fastcall CheckBox5Click(TObject *Sender);
        void __fastcall CheckBox8Click(TObject *Sender);
        void __fastcall FormActivate(TObject *Sender);
        void __fastcall RadioButton1Click(TObject *Sender);
        void __fastcall RadioButton2Click(TObject *Sender);
        void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall CheckBoxConsumiClick(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);
  void __fastcall rbGKWhClick(TObject *Sender);
  void __fastcall rbLClick(TObject *Sender);
  void __fastcall cbPZeroScaleClick(TObject *Sender);
  void __fastcall cbCZeroScaleClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TOpzioni_grafici(TComponent* Owner);

        void __fastcall Controllo_visual(); // controllo visualizzazione test attivi


};
//---------------------------------------------------------------------------
extern PACKAGE TOpzioni_grafici *Opzioni_grafici;
//---------------------------------------------------------------------------
#endif
