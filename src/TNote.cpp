//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TNote.h"
#include "TMain.h"
#include "Structs.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TNote *Note;

 int test;
 extern char vNoteTest[8][DIM_MEDIA],Note_acq[DIM_MEDIA];


//---------------------------------------------------------------------------
__fastcall TNote::TNote(TComponent* Owner)
        : TForm(Owner)
{

}
//---------------------------------------------------------------------------

void __fastcall TNote::BitBtn2Click(TObject *Sender)
{
  Memo1->Clear();
}
//---------------------------------------------------------------------------



void __fastcall TNote::Note(int Ntest)
{

  test=Ntest;
  Memo1->Clear();
  if(Ntest<9)
    Memo1->Text=vNoteTest[Ntest-1];
  else
    Memo1->Text=Note_acq;
  // visualizza il form di inserimento dati Note
  ShowModal();
}


void __fastcall TNote::BitBtn1Click(TObject *Sender)
{
  if(test<9)
    Memo1->GetTextBuf(vNoteTest[test-1],DIM_MEDIA);
  else
    Memo1->GetTextBuf(Note_acq,DIM_MEDIA);

  // aggiorna i dati dei test
  Main->updateRichText();

}
//---------------------------------------------------------------------------
void __fastcall TNote::FormShow(TObject *Sender)
{
  Memo1->SetFocus();        
}
//---------------------------------------------------------------------------
