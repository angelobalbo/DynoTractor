//---------------------------------------------------------------------------

#ifndef TConnessioneH
#define TConnessioneH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "VrControls.hpp"
#include "VrGradient.hpp"
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include "VrLabel.hpp"
//---------------------------------------------------------------------------
class TConnessione : public TForm
{
__published:	// IDE-managed Components
        TVrGradient *VrGradient1;
        TImage *Image1;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TImage *Image2;
        TTimer *Timer1;
        void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
        __fastcall TConnessione(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TConnessione *Connessione;
//---------------------------------------------------------------------------
#endif
