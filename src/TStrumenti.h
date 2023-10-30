//---------------------------------------------------------------------------

#ifndef TStrumentiH
#define TStrumentiH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <FileCtrl.hpp>
#include "NxCollection.hpp"
#include "NxPageControl.hpp"
#include "TeeDragPoint.hpp"
#include "TeeLighting.hpp"
#include "TeeTools.hpp"
#include <Chart.hpp>
#include <Series.hpp>
#include <TeEngine.hpp>
#include <TeeProcs.hpp>
#include <ToolWin.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TStrumenti : public TForm
{
__published:	// IDE-managed Components
        TPageControl *PageControl1;
        TTabSheet *TabSheet1;
        TPageControl *PageControl2;
        TTabSheet *TabSheet3;
        TTabSheet *TabSheet4;
        TGroupBox *GroupBox3;
        TUpDown *UpDown2;
        TEdit *Edit2;
        TEdit *Edit3;
        TUpDown *UpDown3;
        TUpDown *UpDown4;
        TLabeledEdit *LabeledEdit1;
        TGroupBox *GroupBox4;
        TUpDown *UpDown7;
        TLabeledEdit *LabeledEdit2;
        TGroupBox *GroupBox5;
        TUpDown *UpDown10;
        TLabeledEdit *LabeledEdit3;
        TGroupBox *GroupBox6;
        TEdit *Edit8;
        TEdit *Edit9;
        TUpDown *UpDown11;
        TUpDown *UpDown12;
        TUpDown *UpDown13;
        TLabeledEdit *LabeledEdit4;
        TGroupBox *GroupBox7;
        TUpDown *UpDown14;
        TEdit *Edit10;
        TEdit *Edit11;
        TUpDown *UpDown15;
        TLabeledEdit *LabeledEdit5;
        TUpDown *UpDown16;
        TTabSheet *TabSheet5;
        TLabeledEdit *LabeledEdit6;
        TUpDown *UpDown1;
        TUpDown *UpDown5;
        TEdit *Edit1;
        TEdit *Edit5;
        TUpDown *UpDown6;
        TSpeedButton *SpeedButton3;
        TLabel *Label6;
        TLabel *Label7;
        TLabel *Label9;
        TLabel *Label10;
        TTabSheet *TabSheet2;
        TCheckBox *CheckBox6;
        TCheckBox *CheckBox7;
        TCheckBox *CheckBox8;
        TGroupBox *GroupBox2;
        TCheckBox *CheckBox11;
        TBevel *Bevel1;
        TBevel *Bevel2;
        TBevel *Bevel3;
        TBevel *Bevel5;
        TGroupBox *GroupBox8;
        TCheckBox *CheckBox12;
        TGroupBox *GroupBox10;
        TEdit *Edit6;
        TEdit *Edit7;
        TEdit *Edit12;
        TUpDown *UpDown8;
        TUpDown *UpDown9;
        TUpDown *UpDown17;
        TLabeledEdit *LabeledEdit8;
        TLabeledEdit *LabeledEdit9;
        TUpDown *UpDown18;
        TUpDown *UpDown19;
        TLabel *Label11;
        TTimer *Timer1;
        TSaveDialog *SaveDialog1;
        TGroupBox *GroupBox13;
        TComboBox *ComboBox1;
        TCheckBox *CheckBox1;
  TCheckBox *cbTAir;
  TRadioButton *rbPressType0;
  TRadioButton *rbPressType1;
        void __fastcall UpDown4Click(TObject *Sender, TUDBtnType Button);
        void __fastcall UpDown2Click(TObject *Sender, TUDBtnType Button);
        void __fastcall UpDown3Click(TObject *Sender, TUDBtnType Button);
        void __fastcall UpDown7Click(TObject *Sender, TUDBtnType Button);
        void __fastcall UpDown10Click(TObject *Sender, TUDBtnType Button);
        void __fastcall UpDown12Click(TObject *Sender, TUDBtnType Button);
        void __fastcall UpDown16Click(TObject *Sender, TUDBtnType Button);
        void __fastcall UpDown15Click(TObject *Sender, TUDBtnType Button);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall FormActivate(TObject *Sender);
        void __fastcall UpDown1Click(TObject *Sender, TUDBtnType Button);
        void __fastcall UpDown13Click(TObject *Sender, TUDBtnType Button);
        void __fastcall UpDown11Click(TObject *Sender, TUDBtnType Button);
        void __fastcall UpDown5Click(TObject *Sender, TUDBtnType Button);
        void __fastcall UpDown14Click(TObject *Sender, TUDBtnType Button);
        void __fastcall UpDown6Click(TObject *Sender, TUDBtnType Button);
        void __fastcall SpeedButton3Click(TObject *Sender);
        void __fastcall CheckBox6Click(TObject *Sender);
        void __fastcall CheckBox7Click(TObject *Sender);
        void __fastcall CheckBox8Click(TObject *Sender);
        void __fastcall CheckBox11Click(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall CheckBox12Click(TObject *Sender);
        void __fastcall UpDown8Click(TObject *Sender, TUDBtnType Button);
        void __fastcall UpDown9Click(TObject *Sender, TUDBtnType Button);
        void __fastcall UpDown17Click(TObject *Sender, TUDBtnType Button);
        void __fastcall UpDown18Click(TObject *Sender, TUDBtnType Button);
        void __fastcall UpDown19Click(TObject *Sender, TUDBtnType Button);
        void __fastcall ComboBox1Change(TObject *Sender);
        void __fastcall CheckBox1Click(TObject *Sender);
  void __fastcall cbTAirClick(TObject *Sender);
  void __fastcall rbPressType0Click(TObject *Sender);
  void __fastcall rbPressType1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TStrumenti(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TStrumenti *Strumenti;
//---------------------------------------------------------------------------
#endif
