//----------------------------------------------------------------------------
#ifndef TInfoH
#define TInfoH
//----------------------------------------------------------------------------
#include <vcl\System.hpp>
#include <vcl\Windows.hpp>
#include <vcl\SysUtils.hpp>
#include <vcl\Classes.hpp>
#include <vcl\Graphics.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <jpeg.hpp>
//----------------------------------------------------------------------------
class TInfo : public TForm
{
__published:        
	TButton *OKBtn;
        TBevel *Bevel2;
        TPanel *Panel1;
        TImage *Image1;
        TLabel *Label2;
        TLabel *Label3;
        TLabel *Label4;
        TLabel *Label5;
        TLabel *Label8;
        TLabel *Label9;
        TTimer *Timer1;
        TLabel *Label10;
  TLabel *lblFw;
  TImage *Image2;
        void __fastcall Label4Click(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
private:
public:
	virtual __fastcall TInfo(TComponent* AOwner);
};
//----------------------------------------------------------------------------
extern PACKAGE TInfo *Info;
//----------------------------------------------------------------------------
#endif    
