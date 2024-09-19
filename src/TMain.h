//---------------------------------------------------------------------------

#ifndef TMainH
#define TMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <ExtCtrls.hpp>
#include <Chart.hpp>
#include <Series.hpp>
#include <TeEngine.hpp>
#include <TeeProcs.hpp>
#include "AdPort.hpp"
#include "OoMisc.hpp"
#include <ImgList.hpp>
#include "TeeTools.hpp"
#include "TeeGauges.hpp"
#include "TeeEdit.hpp"
#include "VrDigit.hpp"
#include "VrGradient.hpp"
#include "VrLabel.hpp"
#include "VrRocker.hpp"
#include "VrBlinkLed.hpp"
#include "VrButtons.hpp"
#include <Dialogs.hpp>
#include "TeeSelectorTool.hpp"
#include "TeeSeriesBandTool.hpp"
#include "TeeSpline.hpp"
#include <Graphics.hpp>
#include <Grids.hpp>
#include "TeeSeriesTextEd.hpp"
#include "TeeURL.hpp"
#include <TabNotBk.hpp>
#include <ValEdit.hpp>
#include "ErrorBar.hpp"
#include "MyPoint.hpp"
#include "CurvFitt.hpp"
#include "StatChar.hpp"
#include "TeeExtraLegendTool.hpp"
#include "TeeDragPoint.hpp"
#include "NxCollection.hpp"
#include "NxCustomGrid.hpp"
#include "NxCustomGridControl.hpp"
#include "NxGrid.hpp"
#include "NxScrollControl.hpp"
#include "NxColumnClasses.hpp"
#include "NxColumns.hpp"
#include "NxGridPrint.hpp"
#include "VrControls.hpp"
#include "CGAUGES.h"
#include "TeeGanttTool.hpp"
#include "VrScrollText.hpp"
#include "NxToolBox.hpp"
#include "NxLinkMenu.hpp"
#include "NxEdit.hpp"
#include "VrBorder.hpp"
#include "CDyra.h"
#include "Structs.h"
#include "ZIPLib_OCX.h"
#include <OleCtrls.hpp>
//#include "CDyra.h"
#define COLONNE_TABSTEP_VUOTE   1
/// Per serial DAT

typedef enum
{
  NO_DAT=0,
  CREATE_DAT,
  SAMPLING_DAT,
  SAVE_DAT,
  LOAD_DAT,
  F10_SIMUL_DAT,
  START_SIMUL_DAT,
  RUN_SIMUL_DAT,
  TERMINATE_DAT,
} STATUS_DAT;




//---------------------------------------------------------------------------
class TMain : public TForm
{
__published:	// IDE-managed Components

        TMainMenu *MainMenu1;
        TMenuItem *test1;
        TMenuItem *Aprimisurazioni1;
        TMenuItem *N5;
        TMenuItem *Salvamisurazione1;
        TMenuItem *N1;
        TMenuItem *Stampatest1;
        TMenuItem *Setupdistampa1;
        TMenuItem *N3;
        TMenuItem *Esci1;
        TMenuItem *Installazioni1;
        TMenuItem *Setupsistema1;
        TMenuItem *Unitdimisura1;
        TMenuItem *N2;
        TMenuItem *Impostazionistrumenti1;
        TMenuItem *N4;
        TMenuItem *Sequenzaparametriditest1;
        TMenuItem *Utenti1;
        TMenuItem *Cambiautente1;
        TMenuItem *Info1;
        TMenuItem *Informazionisu1;
  TMenuItem *bAssistance;
        TToolBar *ToolBar1;
        TPanel *Panel1;
        TPageControl *PageControl1;
        TTabSheet *TabSheet1;
        TImageList *ImageList1;
        TMenuItem *Impostazionigrafici1;
        TPanel *Panel3;
        TPageControl *PageControl2;
        TTabSheet *TabSheet2;
        TTabSheet *TabSheet3;
        TChart *Chart2;
        TPanel *Panel30;
        TFastLineSeries *Series11;
        TTimer *Timer_Atm;
        TGroupBox *GroupBox1;
        TVrRocker *VrRocker1;
        TVrBlinkLed *VrBlinkLed1;
        TLabel *Label1;
        TLabel *Label2;
        TAxisArrowTool *ChartTool1;
        TFastLineSeries *Series12;
        TFastLineSeries *Series13;
        TFastLineSeries *Series14;
        TGroupBox *GroupBox2;
        TPanel *Panel23;
        TPanel *Panel22;
        TPanel *Panel26;
        TPanel *Panel27;
        TPanel *Panel28;
        TPanel *Panel29;
        TPanel *Panel31;
        TPanel *Panel32;
        TVrLabel *TatmLabel;
        TVrLabel *PatmLabel;
        TVrLabel *HrLabel;
        TVrLabel *FcLabel;
        TRadioGroup *RadioGroup1;
        TStaticText *StaticText20;
        TComboBox *ComboBox5;
        TOpenDialog *OpenDia_tara;
        TMenuItem *Esportafileditaratura1;
        TMenuItem *Esportafileprofiloutenti1;
        TMenuItem *Setupsistema2;
        TMenuItem *Fileditaraturacelle1;
        TMenuItem *Fileditaraturacelle2;
        TMenuItem *N6;
        TSaveDialog *SaveDia_tara;
        TPopupMenu *PopupMenu1;
        TMenuItem *Eliminadefinitivamenteiltest1;
        TMenuItem *Rinominailtest1;
        TPanel *Panel33;
        TSplitter *Splitter4;
        TPopupMenu *PopupMenu2;
        TMenuItem *MenuItem1;
        TMenuItem *MenuItem2;
        TPopupMenu *PopupMenu3;
        TMenuItem *Eliminadefinitivamenteiltest2;
        TMenuItem *Rinominailtest2;
        TPopupMenu *PopupMenu4;
        TMenuItem *Eliminadefinitivamenteiltest3;
        TMenuItem *Rinominailtest3;
        TPopupMenu *PopupMenu5;
        TMenuItem *Eliminadefinitivamenteiltest4;
        TMenuItem *Rinominailtest4;
        TPopupMenu *PopupMenu6;
        TMenuItem *Eliminadefinitivamenteiltest5;
        TMenuItem *Rinominailtest5;
        TPopupMenu *PopupMenu7;
        TMenuItem *Eliminadefinitivamenteiltest6;
        TMenuItem *Rinominailtest6;
        TPopupMenu *PopupMenu8;
        TMenuItem *Eliminadefinitivamenteiltest7;
        TMenuItem *Rinominailtest7;
        TMenuItem *Nuovasessioneditest1;
        TTimer *Timer1;
        TSaveDialog *SaveDialog1;
        TOpenDialog *OpenDialog1;
        TToolBar *ToolBar3;
        TMarksTipTool *ChartTool5;
        TStatusBar *StatusBar1;
        TTabSheet *TabSheet4;
        TRichEdit *RichEdit1;
        TMenuItem *Stampadeigrafici1;
        TMenuItem *Stampadelletabelle1;
        TMenuItem *Stampadatideitest1;
        TPrintDialog *PrintDialog1;
        TTabSheet *TabSheet5;
        TPageControl *PageControl3;
        TTabSheet *Tabella_test_1;
        TTabSheet *Tabella_test_2;
        TTabSheet *Tabella_test_3;
        TTabSheet *Tabella_test_4;
        TTabSheet *Tabella_test_5;
        TTabSheet *Tabella_test_6;
        TTabSheet *Tabella_test_7;
        TTabSheet *Tabella_test_8;
        TPrinterSetupDialog *PrinterSetupDialog1;
        TMenuItem *Aggiunginote1;
        TMenuItem *Aggiungimodificanotedeltest1;
        TMenuItem *Aggiungimodificanotedeltest2;
        TMenuItem *Aggiungimodificanotedeltest3;
        TMenuItem *Aggiungimodificanotedeltest4;
        TMenuItem *Aggiungimodificanotedeltest6;
        TMenuItem *Aggiungimodificanotedeltest7;
        TMenuItem *Aggiungimodificanotedeltest8;
        TMenuItem *Taco1;
        TVrGradient *VrGradient1;
        TVrGradient *VrGradient2;
        TBevel *Bevel12;
        TCheckBox *CheckBox8;
        TCheckBox *CheckBox7;
        TCheckBox *CheckBox6;
        TCheckBox *CheckBox5;
        TCheckBox *CheckBox4;
        TCheckBox *CheckBox3;
        TCheckBox *CheckBox2;
        TCheckBox *CheckBox1;
        TPanel *Panel34;
        TPanel *Panel4;
        TVrLabel *GiriLabel;
        TPanel *Panel5;
        TSplitter *Splitter3;
        TPanel *Panel43;
        TPanel *Panel8;
        TVrLabel *CopLabel;
        TPanel *Panel9;
        TPanel *Panel44;
        TPanel *Panel10;
        TVrLabel *PotLabel;
        TPanel *Panel11;
        TSplitter *Splitter7;
        TSplitter *Splitter8;
        TPanel *Panel45;
        TSplitter *Splitter9;
        TSplitter *Splitter10;
        TPanel *Panel46;
        TSplitter *Splitter11;
        TToolBar *ToolBar5;
        TSpeedButton *SpeedButton5;
        TSpeedButton *SpeedButton7;
        TSpeedButton *SpeedButton10;
        TSpeedButton *SpeedButton11;
        TSpeedButton *SpeedButton12;
        TBevel *Bevel8;
        TBevel *Bevel7;
        TBevel *Bevel5;
        TBevel *Bevel10;
        TBevel *Bevel6;
        TBevel *Bevel4;
        TBevel *Bevel3;
        TPanel *Panel14;
        TVrLabel *MapLabel;
        TPanel *Panel15;
        TPanel *Panel18;
        TVrLabel *OilLabel;
        TPanel *Panel19;
        TPanel *Panel24;
        TVrLabel *VccLabel;
        TPanel *Panel25;
        TTrackBar *TrackBar1;
        TBevel *Bevel2;
        TStaticText *StaticText1;
        TTimer *Timer_Allarm;
        TSpeedButton *SpeedButton26;
        TBevel *Bevel13;
        TTabSheet *TabSheet9;
        TPanel *Panel47;
        TGroupBox *GroupBox11;
        TLabel *Label28;
        TVrLabel *VrLabel8;
        TLabel *Label29;
        TVrLabel *VrLabel10;
        TPanel *Panel48;
        TVrLabel *VrLabel13;
        TTimer *Timer_freni;
        TPanel *Panel49;
        TImage *Image1;
        TImage *Image2;
        TImage *Image3;
        TVrLabel *VrLabel14;
        TLabel *Label3;
        TSaveDialog *SaveDialog2;
        TNxSplitter *NxSplitter1;
        TNxGridPrint *NxGridPrint1;
        TNextGrid *NextGrid1;
        TNxTextColumn *NxTextColumn1;
        TNxTextColumn *NxTextColumn2;
        TNxTextColumn *NxTextColumn3;
        TNxTextColumn *NxTextColumn4;
        TNxTextColumn *NxTextColumn5;
        TNxTextColumn *NxTextColumn6;
        TNextGrid *NextGrid2;
        TNxTextColumn *NxTextColumn13;
        TNxTextColumn *NxTextColumn14;
        TNxTextColumn *NxTextColumn15;
        TNxTextColumn *NxTextColumn16;
        TNxTextColumn *NxTextColumn17;
        TNxTextColumn *NxTextColumn18;
        TNextGrid *NextGrid3;
        TNxTextColumn *NxTextColumn25;
        TNxTextColumn *NxTextColumn26;
        TNxTextColumn *NxTextColumn27;
        TNxTextColumn *NxTextColumn28;
        TNxTextColumn *NxTextColumn29;
        TNxTextColumn *NxTextColumn30;
        TNextGrid *NextGrid4;
        TNxTextColumn *NxTextColumn37;
        TNxTextColumn *NxTextColumn38;
        TNxTextColumn *NxTextColumn39;
        TNxTextColumn *NxTextColumn40;
        TNxTextColumn *NxTextColumn41;
        TNxTextColumn *NxTextColumn42;
        TNextGrid *NextGrid5;
        TNxTextColumn *NxTextColumn49;
        TNxTextColumn *NxTextColumn50;
        TNxTextColumn *NxTextColumn51;
        TNxTextColumn *NxTextColumn52;
        TNxTextColumn *NxTextColumn53;
        TNxTextColumn *NxTextColumn54;
        TNextGrid *NextGrid6;
        TNxTextColumn *NxTextColumn61;
        TNxTextColumn *NxTextColumn62;
        TNxTextColumn *NxTextColumn63;
        TNxTextColumn *NxTextColumn64;
        TNxTextColumn *NxTextColumn65;
        TNxTextColumn *NxTextColumn66;
        TNextGrid *NextGrid7;
        TNxTextColumn *NxTextColumn73;
        TNxTextColumn *NxTextColumn74;
        TNxTextColumn *NxTextColumn75;
        TNxTextColumn *NxTextColumn76;
        TNxTextColumn *NxTextColumn77;
        TNxTextColumn *NxTextColumn78;
        TNextGrid *NextGrid8;
        TNxTextColumn *NxTextColumn85;
        TNxTextColumn *NxTextColumn86;
        TNxTextColumn *NxTextColumn87;
        TNxTextColumn *NxTextColumn88;
        TNxTextColumn *NxTextColumn89;
        TNxTextColumn *NxTextColumn90;
        TStaticText *StaticText2;
        TNxButton *NxButton3;
        TToolButton *ToolButton5;
        TToolButton *ToolButton6;
        TMenuItem *Anteprimadati1;
        TMenuItem *Anteprimadati2;
        TMenuItem *AnteprimaDati3;
        TMenuItem *Anteprimadati4;
        TMenuItem *Anteprimadati5;
        TMenuItem *Anteprimadati6;
        TMenuItem *Anteprimadati7;
        TMenuItem *Anteprimadati8;
        TChart *Chart1;
        TFastLineSeries *Series3;
        TFastLineSeries *Series6;
        TFastLineSeries *Series7;
        TFastLineSeries *Series9;
        TFastLineSeries *Series10;
        TSmoothingFunction *TeeFunction1;
        TFastLineSeries *Series21;
        TFastLineSeries *Series22;
        TFastLineSeries *Series23;
        TFastLineSeries *Series24;
        TFastLineSeries *Series25;
        TFastLineSeries *Series26;
        TFastLineSeries *Series27;
        TFastLineSeries *Series35;
        TFastLineSeries *Series36;
        TFastLineSeries *Series37;
        TFastLineSeries *Series38;
        TFastLineSeries *Series39;
        TFastLineSeries *Series40;
        TFastLineSeries *Series41;
        TFastLineSeries *Series60;
        TFastLineSeries *Series61;
        TFastLineSeries *Series62;
        TFastLineSeries *Series63;
        TFastLineSeries *Series64;
        TFastLineSeries *Series65;
        TFastLineSeries *Series66;
        TFastLineSeries *Series67;
        TFastLineSeries *Series81;
        TFastLineSeries *Series82;
        TFastLineSeries *Series83;
        TFastLineSeries *Series84;
        TFastLineSeries *Series85;
        TFastLineSeries *Series86;
        TFastLineSeries *Series87;
        TFastLineSeries *Series88;
        TFastLineSeries *Series89;
        TFastLineSeries *Series90;
        TFastLineSeries *Series91;
        TFastLineSeries *Series92;
        TFastLineSeries *Series93;
        TFastLineSeries *Series94;
        TCursorTool *ChartTool4;
        TMarksTipTool *ChartTool3;
        TColorLineTool *ChartTool9;
        TCGauge *CGauge2;
        TToolButton *ToolButton7;
        TNxLabel *NxLabel8;
        TSaveDialog *SaveDialog3;
        TNxButton *SpeedButton4;
        TNxButton *SpeedButton8;
        TNxButton *SpeedButton21;
        TNxButton *SpeedButton17;
        TNxButton *SpeedButton15;
        TNxButton *SpeedButton18;
        TNxButton *SpeedButton1;
  TNxButton *sbDataIn;
        TGroupBox *GroupBox3;
        TLabel *Label4;
        TLabel *Label5;
        TVrLabel *VrLabel1;
        TVrLabel *VrLabel2;
        TLabel *Label6;
        TVrLabel *VrLabel3;
        TStringGrid *StringGrid1;
        TTabSheet *TabSheet6;
        TChart *Chart3;
        TFastLineSeries *Series57;
        TFastLineSeries *Series20;
        TFastLineSeries *Series19;
        TFastLineSeries *Series56;
        TFastLineSeries *Series58;
        TFastLineSeries *Series59;
        TMarksTipTool *ChartTool2;
        TToolBar *ToolBar2;
        TButton *Button1;
        TButton *Button2;
        TButton *Button3;
        TFastLineSeries *Series4;
        TPanel *Panel6;
        TVrLabel *ConsLabel;
        TPanel *Panel7;
    TTimer *TimerStopPwm;
    TFastLineSeries *Series15;
    TFastLineSeries *Series16;
    TFastLineSeries *Series17;
    TFastLineSeries *Series18;
    TFastLineSeries *Series28;
    TFastLineSeries *Series29;
    TFastLineSeries *Series30;
    TFastLineSeries *Series31;
    TCGauge *CGauge3;
    TNxLabel *NxLabel11;
  TTabSheet *TabSheet11;
  TNextGrid *NextGrid9;
  TNxTextColumn *NxTextColumn109;
  TNxTextColumn *NxTextColumn110;
  TNxTextColumn *NxTextColumn111;
  TNxTextColumn *NxTextColumn112;
  TNxTextColumn *NxTextColumn113;
  TNxTextColumn *NxTextColumn114;
  TNxTextColumn *NxTextColumn115;
  TMenuItem *TabStepPrint;
  TMenuItem *exportTables;
  TLabel *Label7;
  TEdit *Edit1;
  TSpeedButton *SpeedButton3;
  TImage *Image4;
  TVrLabel *ConsLabel2;
  TSplitter *Splitter1;
  TSplitter *Splitter2;
  TPanel *Panel12;
  TVrLabel *TAirLabel;
  TPanel *Panel13;
  TPanel *Panel20;
  TVrLabel *TKLabel;
  TPanel *Panel17;
  TNxTextColumn *NxTextColumn7;
  TNxTextColumn *NxTextColumn8;
  TNxTextColumn *NxTextColumn9;
  TNxTextColumn *NxTextColumn10;
  TNxTextColumn *NxTextColumn11;
  TNxTextColumn *NxTextColumn12;
  TNxTextColumn *NxTextColumn19;
  TNxTextColumn *NxTextColumn20;
  TNxTextColumn *NxTextColumn21;
  TNxTextColumn *NxTextColumn22;
  TNxTextColumn *NxTextColumn23;
  TNxTextColumn *NxTextColumn24;
  TNxTextColumn *NxTextColumn31;
  TNxTextColumn *NxTextColumn32;
  TNxTextColumn *NxTextColumn33;
  TNxTextColumn *NxTextColumn34;
  TTimer *tmrProcessData;
  TFastLineSeries *Series105;
  TFastLineSeries *Series106;
  TFastLineSeries *Series107;
  TGroupBox *GroupBox4;
  TLabel *Label8;
  TLabel *Label9;
  TVrLabel *VrLabel4;
  TVrLabel *VrLabel5;
  TLabel *Label10;
  TVrLabel *VrLabel6;
  TTimer *tmrContaOre;
  TMenuItem *CaricaImpostazioni2;
  TMenuItem *CaricaImpostazioni8;
  TMenuItem *Fattoredicorrezione8;
  TMenuItem *CaricaImpostazioni7;
  TMenuItem *Fattoredicorrezione7;
  TMenuItem *CaricaImpostazioni6;
  TMenuItem *Fattoredicorrezione6;
  TMenuItem *CaricaImpostazioni5;
  TMenuItem *Fattoredicorrezione5;
  TMenuItem *CaricaImpostazioni4;
  TMenuItem *Fattoredicorrezione4;
  TMenuItem *CaricaImpostazioni3;
  TMenuItem *Fattoredicorrezione3;
  TMenuItem *Fattoredicorrezione2;
  TMenuItem *CaricaImpostazioni1;
  TMenuItem *Fattoredicorrezione1;
  TLabel *lblPID;
  TButton *btnPID;
  TButton *btnRfr;
  TMenuItem *ApriDatSeriale;
  TZip *pZip;
  TTabSheet *TabSheet7;
  TNextGrid *NextGrid11;
  TNxTextColumn *NxTextColumn35;
  TNxTextColumn *NxTextColumn36;
  TNxTextColumn *NxTextColumn43;
  TNxTextColumn *NxTextColumn44;
  TNxTextColumn *NxTextColumn45;
  TNxTextColumn *NxTextColumn46;
  TNxTextColumn *NxTextColumn47;
  TTabSheet *TabSheet8;
  TNextGrid *NextGrid12;
  TNxTextColumn *NxTextColumn48;
  TNxTextColumn *NxTextColumn55;
  TNxTextColumn *NxTextColumn56;
  TNxTextColumn *NxTextColumn57;
  TNxTextColumn *NxTextColumn58;
  TNxTextColumn *NxTextColumn59;
  TNxTextColumn *NxTextColumn60;
  TMenuItem *ExportData1;
  TMenuItem *Savetest1;
  TMenuItem *Setpaths1;
  TMenuItem *t1;
  TMenuItem *t2;
  TMenuItem *t3;
  TMenuItem *t4;
  TMenuItem *t5;
  TMenuItem *t6;
  TMenuItem *t7;
  TMenuItem *t8;
  TMenuItem *In1;
  TMenuItem *Out1;
  TNxButton *SpeedButton19;
  TNxButton *sbDataOut;
  TNxButton *SpeedButton2;
  TBitBtn *btnCan;
  TEdit *edP;
  TEdit *edI;
  TEdit *edD;
  TLabel *lblP;
  TLabel *lblI;
  TLabel *lblD;
  TMenuItem *mnuDebugMode;
  
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

        void __fastcall SpeedButton7Click(TObject *Sender);
        void __fastcall SpeedButton7Click2(TObject *Sender);

        void __fastcall SpeedButton10Click(TObject *Sender);
        void __fastcall SpeedButton10Click2(TObject *Sender);



        void __fastcall SpeedButton11Click(TObject *Sender);
        void __fastcall SpeedButton11Click2(TObject *Sender);

        void __fastcall SpeedButton12Click(TObject *Sender);
        void __fastcall SpeedButton12Click2(TObject *Sender);

        void __fastcall Impostazionistrumenti1Click(TObject *Sender);
        void __fastcall SpeedButton2Click(TObject *Sender);
        void __fastcall Informazionisu1Click(TObject *Sender);
        void __fastcall Sequenzaparametriditest1Click(TObject *Sender);
        void __fastcall Esci1Click(TObject *Sender);
        void __fastcall Unitdimisura1Click(TObject *Sender);
        void __fastcall Cambiautente1Click(TObject *Sender);
        void __fastcall SpeedButton1Click(TObject *Sender);

        void __fastcall SpeedButton5Click(TObject *Sender);   //On
        void __fastcall SpeedButton5Click2(TObject *Sender); // Off


        void __fastcall SpeedButton26Click(TObject *Sender); // olio
        void __fastcall SpeedButton26Click2(TObject *Sender);


        void __fastcall Impostazionigrafici1Click(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall VrRocker1LowerClick(TObject *Sender);
        void __fastcall VrRocker1UpperClick(TObject *Sender);
        void __fastcall Timer_AtmTimer(TObject *Sender);
        void __fastcall ComboBox5Change(TObject *Sender);
        void __fastcall Setupsistema2Click(TObject *Sender);
        void __fastcall Fileditaraturacelle1Click(TObject *Sender);
        void __fastcall Fileditaraturacelle2Click(TObject *Sender);
        void __fastcall SpeedButton8Click(TObject *Sender);
        void __fastcall SpeedButton4Click(TObject *Sender);
        void __fastcall SpeedButton15Click(TObject *Sender);
        void __fastcall SpeedButton17Click(TObject *Sender);
        void __fastcall SpeedButton18Click(TObject *Sender);
        void __fastcall SpeedButton19Click(TObject *Sender);
        void __fastcall SpeedButton19Click2(TObject *Sender);
        void __fastcall CheckBox1ContextPopup(TObject *Sender,
          TPoint &MousePos, bool &Handled);
        void __fastcall Panel33CanResize(TObject *Sender, int &NewWidth,
          int &NewHeight, bool &Resize);
        void __fastcall CheckBox1Click(TObject *Sender);
        void __fastcall CheckBox2Click(TObject *Sender);
        void __fastcall CheckBox3Click(TObject *Sender);
        void __fastcall CheckBox4Click(TObject *Sender);
        void __fastcall CheckBox5Click(TObject *Sender);
        void __fastcall CheckBox6Click(TObject *Sender);
        void __fastcall CheckBox7Click(TObject *Sender);
        void __fastcall CheckBox8Click(TObject *Sender);
        void __fastcall Rinominailtest1Click(TObject *Sender);
        void __fastcall MenuItem2Click(TObject *Sender);
        void __fastcall CheckBox2ContextPopup(TObject *Sender,
          TPoint &MousePos, bool &Handled);
        void __fastcall Rinominailtest3Click(TObject *Sender);
        void __fastcall Rinominailtest4Click(TObject *Sender);
        void __fastcall Rinominailtest5Click(TObject *Sender);
        void __fastcall Rinominailtest6Click(TObject *Sender);
        void __fastcall Rinominailtest7Click(TObject *Sender);
        void __fastcall CheckBox3ContextPopup(TObject *Sender,
          TPoint &MousePos, bool &Handled);
        void __fastcall CheckBox4ContextPopup(TObject *Sender,
          TPoint &MousePos, bool &Handled);
        void __fastcall CheckBox5ContextPopup(TObject *Sender,
          TPoint &MousePos, bool &Handled);
        void __fastcall CheckBox6ContextPopup(TObject *Sender,
          TPoint &MousePos, bool &Handled);
        void __fastcall CheckBox7ContextPopup(TObject *Sender,
          TPoint &MousePos, bool &Handled);
        void __fastcall CheckBox8ContextPopup(TObject *Sender,
          TPoint &MousePos, bool &Handled);
        void __fastcall Eliminadefinitivamenteiltest1Click(
          TObject *Sender);
        void __fastcall Rinominailtest2Click(TObject *Sender);
        void __fastcall MenuItem1Click(TObject *Sender);
        void __fastcall Eliminadefinitivamenteiltest2Click(
          TObject *Sender);
        void __fastcall Eliminadefinitivamenteiltest3Click(
          TObject *Sender);
        void __fastcall Eliminadefinitivamenteiltest4Click(
          TObject *Sender);
        void __fastcall Eliminadefinitivamenteiltest5Click(
          TObject *Sender);
        void __fastcall Eliminadefinitivamenteiltest6Click(
          TObject *Sender);
        void __fastcall Eliminadefinitivamenteiltest7Click(
          TObject *Sender);
        void __fastcall Nuovasessioneditest1Click(TObject *Sender);
        void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall Salvamisurazione1Click(TObject *Sender); // attiva ventola
        void __fastcall Aprimisurazioni1Click(TObject *Sender);
        void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall HrLabelDblClick(TObject *Sender);
        void __fastcall Stampadatideitest1Click(TObject *Sender);
        void __fastcall Stampadeigrafici1Click(TObject *Sender);
        void __fastcall Stampadelletabelle1Click(TObject *Sender);
        void __fastcall Setupdistampa1Click(TObject *Sender);
        void __fastcall Aggiunginote1Click(TObject *Sender);
        void __fastcall Aggiungimodificanotedeltest1Click(TObject *Sender);
        void __fastcall Aggiungimodificanotedeltest2Click(TObject *Sender);
        void __fastcall Aggiungimodificanotedeltest3Click(TObject *Sender);
        void __fastcall Aggiungimodificanotedeltest4Click(TObject *Sender);
        void __fastcall Aggiungimodificanotedeltest6Click(TObject *Sender);
        void __fastcall Aggiungimodificanotedeltest7Click(TObject *Sender);
        void __fastcall Aggiungimodificanotedeltest8Click(TObject *Sender);
        void __fastcall Taco1Click(TObject *Sender);
        void __fastcall SpeedButton21Click(TObject *Sender);
        void __fastcall Panel8Resize(TObject *Sender);
        void __fastcall Panel10Resize(TObject *Sender);
        void __fastcall Panel14Resize(TObject *Sender);
        void __fastcall Panel18Resize(TObject *Sender);
        void __fastcall Panel20Resize(TObject *Sender);
        void __fastcall TrackBar1Change(TObject *Sender);
        void __fastcall Timer_AllarmTimer(TObject *Sender);
        void __fastcall Timer_freniTimer(TObject *Sender);
        void __fastcall Panel4Resize(TObject *Sender);
        void __fastcall NxButton3Click(TObject *Sender);
        void __fastcall Anteprimadati1Click(TObject *Sender);
        void __fastcall Anteprimadati2Click(TObject *Sender);
        void __fastcall AnteprimaDati3Click(TObject *Sender);
        void __fastcall Anteprimadati4Click(TObject *Sender);
        void __fastcall Anteprimadati5Click(TObject *Sender);
        void __fastcall Anteprimadati6Click(TObject *Sender);
        void __fastcall Anteprimadati7Click(TObject *Sender);
        void __fastcall Anteprimadati8Click(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall Button3Click(TObject *Sender);
        void __fastcall TimerStopPwmTimer(TObject *Sender);
  void __fastcall TabStepPrintClick(TObject *Sender);
  void __fastcall exportTablesClick(TObject *Sender);
  void __fastcall MapLabelDblClick(TObject *Sender);
  void __fastcall ConsLabelDblClick(TObject *Sender);
  void __fastcall OilLabelDblClick(TObject *Sender);
  void __fastcall TKLabelDblClick(TObject *Sender);
  void __fastcall Panel6Resize(TObject *Sender);
  void __fastcall PageControl2Resize(TObject *Sender);
  void __fastcall Panel45Resize(TObject *Sender);
  void __fastcall Edit1Change(TObject *Sender);
  void __fastcall SpeedButton3Click(TObject *Sender);
  void __fastcall Series15GetMarkText(TChartSeries *Sender, int ValueIndex,
          AnsiString &MarkText);
  void __fastcall Series16GetMarkText(TChartSeries *Sender, int ValueIndex,
          AnsiString &MarkText);
  void __fastcall Series17GetMarkText(TChartSeries *Sender, int ValueIndex,
          AnsiString &MarkText);
  void __fastcall Series18GetMarkText(TChartSeries *Sender, int ValueIndex,
          AnsiString &MarkText);
  void __fastcall Series28GetMarkText(TChartSeries *Sender, int ValueIndex,
          AnsiString &MarkText);
  void __fastcall Series29GetMarkText(TChartSeries *Sender, int ValueIndex,
          AnsiString &MarkText);
  void __fastcall Series31GetMarkText(TChartSeries *Sender, int ValueIndex,
          AnsiString &MarkText);
  void __fastcall Series30GetMarkText(TChartSeries *Sender, int ValueIndex,
          AnsiString &MarkText);
  void __fastcall Panel12Resize(TObject *Sender);
  void __fastcall tmrProcessDataTimer(TObject *Sender);
  void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
  void __fastcall tmrContaOreTimer(TObject *Sender);
  void __fastcall CaricaImpostazioni8Click(TObject *Sender);
  void __fastcall CaricaImpostazioni7Click(TObject *Sender);
  void __fastcall CaricaImpostazioni6Click(TObject *Sender);
  void __fastcall CaricaImpostazioni5Click(TObject *Sender);
  void __fastcall CaricaImpostazioni4Click(TObject *Sender);
  void __fastcall CaricaImpostazioni3Click(TObject *Sender);
  void __fastcall CaricaImpostazioni2Click(TObject *Sender);
  void __fastcall CaricaImpostazioni1Click(TObject *Sender);
  void __fastcall Fattoredicorrezione1Click(TObject *Sender);
  void __fastcall Fattoredicorrezione2Click(TObject *Sender);
  void __fastcall Fattoredicorrezione3Click(TObject *Sender);
  void __fastcall Fattoredicorrezione4Click(TObject *Sender);
  void __fastcall Fattoredicorrezione5Click(TObject *Sender);
  void __fastcall Fattoredicorrezione6Click(TObject *Sender);
  void __fastcall Fattoredicorrezione7Click(TObject *Sender);
  void __fastcall Fattoredicorrezione8Click(TObject *Sender);
  void __fastcall ApriDatSerialeClick(TObject *Sender);
  void __fastcall bAssistanceClick(TObject *Sender);
  void __fastcall sbDataInClick(TObject *Sender);
  void __fastcall sbDataOutClick(TObject *Sender);
  void __fastcall t1Click(TObject *Sender);
  void __fastcall t2Click(TObject *Sender);
  void __fastcall t3Click(TObject *Sender);
  void __fastcall t4Click(TObject *Sender);
  void __fastcall t5Click(TObject *Sender);
  void __fastcall t6Click(TObject *Sender);
  void __fastcall t7Click(TObject *Sender);
  void __fastcall t8Click(TObject *Sender);
  void __fastcall In1Click(TObject *Sender);
  void __fastcall Out1Click(TObject *Sender);
  void __fastcall btnCanClick(TObject *Sender);
  void __fastcall mnuDebugModeClick(TObject *Sender);



private:
      
        STATUS_DAT statusDat;
        int iNTest;
        //bool bDatEnabled;
        //bool bDatLoad;
        int  iDatIndex;
        bool bTestToExport; // 0=nessun test effettuato 1=Test effettuato da salvare 2=Test salvato
        static TColor clTestColor[9];
        bool graphEnabled[8];
        void LoadTestPrms(struct TEST_SETS* datPrm);
        float m_minV_T;
        void aggiornaINI(void);
        bool DirectoryExists( const char* pzPath ) ;
        void controllaAndamentoRPMdec();
        void CaricaImpoDAT(float*);
        void CaricaDatiSeriali();
        void SalvaDatiSeriali(int);
        void ProcessData();
        void notifyDyra2Main();
        void salvaImpoDat(struct TEST_SETS*);
        void azzeraPannelli();
        void caricaImpoTest(int i);
        void saveTest(int i);
public:		// User declarations

#ifdef CAN_BUS
        bool bCanInUse;
        int  iCanRate;
        long lCanIdMsg;
        void PopulateCANComboBox(TComboBox* );
        void ClearCANComboBox();
        void PopulateCANIndex();
        long lIdCanSimul;        
#endif


        inline STATUS_DAT getStatusDat()
        {
          return statusDat;
        }
        
        String sInPath;
        String sOutPath;
        String sFileImported;        
        String sFileExported;
        bool pressStartDat();
        bool runDat();
        bool loadDat();
        bool createDat();
        void addSerialDat(CDyra::DataIn1* row);
        void getSerialDat(CDyra::DataIn1* row);
        bool stopAndSaveDat();
        void clearDat();
        String sComDyra;
        String sComDyn3;
        void sommaContaore();
        float nCountMinTotRpmRulli;
        float dataResetCountMinPar;
        float dataResetLanci;
        float nLanciTotali;
        int contaoreMinuti;
        int contaoreSecondi;
        bool scriviFileImpo();
        void caricaBanco();
        void __fastcall carica_imp(); // carica le impostazioni del programma

        void __fastcall salva_imp(); // salva le impostazioni del programma

        void __fastcall carica_unita(); // aggiorna unità di misura selezionate

        void __fastcall Brake_control(int Param,unsigned int Step,unsigned char Stp); // comando al freno/i

        void __fastcall Azzera_var(); // azzera tutte le variabili utilizzate per il test

        int addTest();
        void __fastcall PrepareShowPerformance();        
        void __fastcall showPerformance(int); // visualizza le grandezze Coppia/Potenza dopo calcoli
        void showAllTests();

        void __fastcall Remote(int Codice_Tasto); // Funzione di controllo telecomando

        
        void updateUnits();
        void updateGrids();
        void updateRichText();
        void __fastcall Def_rapporto(); // seleziona il rapporto di trasmissione corrente

        void __fastcall Max_value_test(int); // Verifica dei massimi valori raggiunti durante il test

        void __fastcall CalcPerformance(int iSamples); // Calcolo prestazioni asse Anteriore


        void __fastcall Idioma(); // routine di configurazione lingua in uso

        void __fastcall Log_Error(int Errore , String Action, unsigned char Difetto);  // gestione aggiornamento file log error

        void __fastcall Salvataggio_autom( void ); // salva test automaticamente

        void __fastcall Visualizza_dat_test_strumenti(int,int); // visualizza negli strumenti i dati
                                                         // relativi alla posizione della barra di scorrimento (grafici)
                                                         // parametri: n°test,posizione array
        void __fastcall ResetConsumi(void);

        void  AddTabAcq();
        void  LoadTabAcq();
        void  ResetTabAcq();

        __fastcall TMain(TComponent* Owner);
        
       void EndTest();


};


//---------------------------------------------------------------------------
extern PACKAGE TMain *Main;
//---------------------------------------------------------------------------
#endif
