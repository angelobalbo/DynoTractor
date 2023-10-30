//---------------------------------------------------------------------------

#ifndef TSetupH
#define TSetupH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Mask.hpp>
#include <Dialogs.hpp>
#include "VrBorder.hpp"
#include "VrControls.hpp"
#include "VrLabel.hpp"
#include <Graphics.hpp>
#include "NxEdit.hpp"
#include "NxCollection.hpp"
//---------------------------------------------------------------------------
class TSetup : public TForm
{
__published:	// IDE-managed Components
        TBitBtn *BitBtn1;
        TPageControl *PageControl1;
        TTabSheet *TabSheet1;
        TTabSheet *TabSheet2;
        TGroupBox *GroupBox3;
        TGroupBox *GroupBox2;
        TComboBox *ComboBox3;
        TGroupBox *GroupBox4;
        TComboBox *ComboBox4;
        TStaticText *StaticText1;
        TMaskEdit *MaskEdit1;
        TStaticText *StaticText2;
        TMaskEdit *MaskEdit2;
        TStaticText *StaticText3;
        TMaskEdit *MaskEdit3;
        TStaticText *StaticText4;
        TMaskEdit *MaskEdit4;
        TStaticText *StaticText5;
        TMaskEdit *MaskEdit5;
        TStaticText *StaticText6;
        TMaskEdit *MaskEdit6;
        TSpeedButton *SpeedButton1;
        TBevel *Bevel1;
        TImage *Image1;
        TOpenDialog *OpenDialog1;
        TTabSheet *TabSheet3;
        TLabel *Label2;
        TBitBtn *BitBtn2;
        TLabel *Label3;
        TBitBtn *BitBtn3;
        TLabel *Label4;
        TBitBtn *BitBtn4;
        TEdit *Edit2;
        TLabel *Label5;
        TTimer *Timer_celle;
        TLabel *Label6;
        TEdit *Edit3;
        TLabel *Label7;
        TLabel *Label8;
        TLabel *Label9;
        TEdit *Edit4;
        TLabel *Label10;
        TVrBorder *VrBorder1;
        TVrBorder *VrBorder2;
        TVrBorder *VrBorder3;
        TBitBtn *BitBtn6;
        TProgressBar *ProgressBar1;
        TGroupBox *GroupBox1;
        TRadioButton *RadioButton1;
        TRadioButton *RadioButton2;
        TSpeedButton *SpeedButton2;
        TPanel *Panel1;
        TVrLabel *TKLabel;
        TLabel *Label21;
        TLabel *Label22;
        TGroupBox *GroupBox6;
        TStaticText *StaticText7;
        TEdit *Edit1;
        TUpDown *UpDown1;
        TGroupBox *GroupBox7;
  TStaticText *stComDyn3;
        TGroupBox *GroupBox8;
  TStaticText *stComDyra;
        TPanel *Panel3;
        TVrLabel *VrLabel2;
        TPanel *Panel4;
        TVrLabel *VrLabel3;
        TLabel *Label23;
        TImage *Image2;
        TTimer *Timer_Usb;
        TTimer *Timer_Usb2;
        TButton *Button1;
        TButton *Button2;
        TGroupBox *GroupBox9;
        TNxSpinEdit *NxSpinEdit1;
        TNxLabel *NxLabel1;
        TNxButton *NxButton1;
  TTimer *tmrAutoDetect;
  TGroupBox *GroupBox10;
  TNxLabel *NxLabelContaore;
  TNxLabel *NxLabelLanci;
  TNxLabel *NxLabel2;
  TNxLabel *NxLabel3;
  TNxButton *NxButtonAzzeraOre;
  TNxButton *NxButtonAzzeraLanci;
        void __fastcall BitBtn1Click(TObject *Sender);
        void __fastcall ComboBox4Change(TObject *Sender);
        void __fastcall FormActivate(TObject *Sender);
        void __fastcall SpeedButton1Click(TObject *Sender);
        void __fastcall MaskEdit1Change(TObject *Sender);
        void __fastcall MaskEdit2Change(TObject *Sender);
        void __fastcall MaskEdit3Change(TObject *Sender);
        void __fastcall MaskEdit4Change(TObject *Sender);
        void __fastcall MaskEdit5Change(TObject *Sender);
        void __fastcall MaskEdit6Change(TObject *Sender);
        void __fastcall Image1DblClick(TObject *Sender);
        void __fastcall Timer_celleTimer(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall SpeedButton2Click(TObject *Sender);
        void __fastcall RadioButton1Click(TObject *Sender);
        void __fastcall RadioButton2Click(TObject *Sender);
        void __fastcall BitBtn6Click(TObject *Sender);
        void __fastcall BitBtn2Click(TObject *Sender);
        void __fastcall BitBtn3Click(TObject *Sender);
        void __fastcall BitBtn4Click(TObject *Sender);
        void __fastcall BitBtn7Click(TObject *Sender);
        void __fastcall BitBtn8Click(TObject *Sender);
        void __fastcall BitBtn9Click(TObject *Sender);
        void __fastcall ComboBox3Change(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall NxSpinEdit1Change(TObject *Sender);
        void __fastcall NxButton1Click(TObject *Sender);
  void __fastcall FormCreate(TObject *Sender);
  void __fastcall tmrAutoDetectTimer(TObject *Sender);
  void __fastcall Button2Click(TObject *Sender);
  void __fastcall NxButtonAzzeraOreClick(TObject *Sender);
  void __fastcall NxButtonAzzeraLanciClick(TObject *Sender);
private:	// User declarations
  void visualizzaContatori();
//    void NxButtonAzzeraOre();
public:		// User declarations
        __fastcall TSetup(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSetup *Setup;
//---------------------------------------------------------------------------
#endif
