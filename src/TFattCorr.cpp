//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TFattCorr.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "NxEdit"
#pragma resource "*.dfm"
TFattCorr *FattCorr;
//---------------------------------------------------------------------------
__fastcall TFattCorr::TFattCorr(TComponent* Owner)
        : TForm(Owner)
{
        fatt=1;
}
//---------------------------------------------------------------------------
void __fastcall TFattCorr::FormShow(TObject *Sender)
{
        //inizializzo l'esito a false
        esit=false;
        //visualizzo il fattore di correzione attuale
        NxSpinEditFatt->Value=fatt;
}
//---------------------------------------------------------------------------
void __fastcall TFattCorr::ButtonOKClick(TObject *Sender)
{
        //salvo il fattore di correzione
        fatt=NxSpinEditFatt->Value;
        //salvo l'OK
        esit=true;
        //chiudo la finestra
        this->Close();
}
//---------------------------------------------------------------------------
void __fastcall TFattCorr::ButtonCancClick(TObject *Sender)
{
        //salvo la scelta
        esit=false;
        //chiudo la finestra
        this->Close();    
}
//---------------------------------------------------------------------------

bool TFattCorr::getEsit()
{
        //ritorno la scelta
        return esit;
}
//---------------------------------------------------------------------------

float TFattCorr::getFatt()
{
        //ritorno il fattore di correzione
        return fatt;
}
//---------------------------------------------------------------------------

void TFattCorr::setFatt(float f)
{
        //imposto il fattore di correzione
        fatt=f;
}
//---------------------------------------------------------------------------
