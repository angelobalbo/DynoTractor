//---------------------------------------------------------------------------

#ifndef TMt_costH
#define TMt_costH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TMt_cost : public TForm
{
__published:	// IDE-managed Components
        TSpeedButton *SpeedButton1;
        TSpeedButton *SpeedButton2;
        TBitBtn *BitBtn1;
        TLabel *Label3;
        TLabel *Label2;
        TLabel *Label1;
        void __fastcall SpeedButton1Click(TObject *Sender);
        void __fastcall BitBtn1Click(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall SpeedButton2Click(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
  void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
private:	// User declarations
public:		// User declarations
        __fastcall TMt_cost(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMt_cost *Mt_cost;
//---------------------------------------------------------------------------
#endif
