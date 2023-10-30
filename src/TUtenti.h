//----------------------------------------------------------------------------
#ifndef TUtentiH
#define TUtentiH
//----------------------------------------------------------------------------
#include <vcl\Buttons.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Graphics.hpp>
#include <vcl\Classes.hpp>
#include <vcl\SysUtils.hpp>
#include <vcl\Windows.hpp>
#include <vcl\System.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <ComCtrls.hpp>
//----------------------------------------------------------------------------
class TUtenti : public TForm
{
__published:
	TButton *OKBtn;
	TButton *CancelBtn;
        TLabel *Label2;
        TMainMenu *MainMenu1;
        TMenuItem *Impo;
        TMenuItem *Aggiungiutente1;
        TMenuItem *Eliminautente1;
        TBevel *Bevel1;
        TImage *Image1;
        TListBox *ListBox1;
        void __fastcall Aggiungiutente1Click(TObject *Sender);
        void __fastcall Eliminautente1Click(TObject *Sender);
        void __fastcall OKBtnClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall ListBox1Click(TObject *Sender);
private:
public:
	virtual __fastcall TUtenti(TComponent* AOwner);
};
//----------------------------------------------------------------------------
extern PACKAGE TUtenti *Utenti;
//----------------------------------------------------------------------------
#endif    
