//---------------------------------------------------------------------------

#ifndef TMisureH
#define TMisureH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TMisure : public TForm
{
__published:	// IDE-managed Components
        TPageControl *PageControl1;
        TTabSheet *TabSheet1;
        TListBox *ListBox1;
        TTabSheet *TabSheet2;
        TListBox *ListBox2;
        TTabSheet *TabSheet5;
        TListBox *ListBox5;
        TBitBtn *BitBtn1;
        TTabSheet *TabSheet3;
        TListBox *ListBox3;
        TTabSheet *TabSheet6;
        TListBox *ListBox6;
        TTabSheet *TabSheet4;
        TListBox *ListBox4;
        void __fastcall BitBtn1Click(TObject *Sender);
        void __fastcall FormActivate(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TMisure(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMisure *Misure;
//---------------------------------------------------------------------------
#endif
