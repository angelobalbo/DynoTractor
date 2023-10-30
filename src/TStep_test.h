//---------------------------------------------------------------------------

#ifndef TStep_testH
#define TStep_testH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include "CGAUGES.h"

//---------------------------------------------------------------------------
class TStep_test : public TForm
{
__published:	// IDE-managed Components
        TBitBtn *BitBtn1;
        TBitBtn *BitBtn2;
        TBitBtn *BitBtn3;
        TLabel *Label2;
        TLabel *Label3;
        TBitBtn *BitBtn4;
        TLabel *Label4;
        TBevel *Bevel1;
        TCGauge *CGauge1;
  TBitBtn *PrintStep;
  TBitBtn *ResetTabStep;
  TTimer *tmrStep;
        void __fastcall BitBtn3Click(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall BitBtn1Click(TObject *Sender);
        void __fastcall BitBtn2Click(TObject *Sender);
        void __fastcall BitBtn4Click(TObject *Sender);
        void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
  void __fastcall ResetTabStepClick(TObject *Sender);
  void __fastcall PrintStepClick(TObject *Sender);
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
  void __fastcall tmrStepTimer(TObject *Sender);
private:	// User declarations
  unsigned short m_usEncoder;
public:		// User declarations
        __fastcall TStep_test(TComponent* Owner);
        WideString getStep(void);        
};
//---------------------------------------------------------------------------
extern PACKAGE TStep_test *Step_test;
//---------------------------------------------------------------------------
#endif
