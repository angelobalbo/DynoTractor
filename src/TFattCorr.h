//---------------------------------------------------------------------------

#ifndef TFattCorrH
#define TFattCorrH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "NxEdit.hpp"
//---------------------------------------------------------------------------
class TFattCorr : public TForm
{
__published:	// IDE-managed Components
        TNxSpinEdit *NxSpinEditFatt;
        TButton *ButtonOK;
        TButton *ButtonCanc;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall ButtonOKClick(TObject *Sender);
        void __fastcall ButtonCancClick(TObject *Sender);
private:	// User declarations
        bool esit;
        float fatt;
public:		// User declarations
        bool getEsit();
        float getFatt();
        void setFatt(float f);
        __fastcall TFattCorr(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFattCorr *FattCorr;
//---------------------------------------------------------------------------
#endif
