//---------------------------------------------------------------------------

#ifndef TSplashH
#define TSplashH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
#include <Graphics.hpp>
//---------------------------------------------------------------------------
class TSplash : public TForm
{
__published:	// IDE-managed Components
        TImage *Image1;
        TTimer *Timer1;
  TImage *Image2;
        void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TSplash(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSplash *Splash;
//---------------------------------------------------------------------------
#endif
