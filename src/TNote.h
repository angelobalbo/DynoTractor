//---------------------------------------------------------------------------

#ifndef TNoteH
#define TNoteH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TNote : public TForm
{
__published:	// IDE-managed Components
        TMemo *Memo1;
        TBitBtn *BitBtn1;
        TBitBtn *BitBtn2;
        TBitBtn *BitBtn3;
        void __fastcall BitBtn2Click(TObject *Sender);
        void __fastcall BitBtn1Click(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations

        __fastcall TNote(TComponent* Owner);

        void __fastcall Note(int Ntest);




};
//---------------------------------------------------------------------------
extern PACKAGE TNote *Note;
//---------------------------------------------------------------------------
#endif
