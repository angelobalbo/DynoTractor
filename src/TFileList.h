//---------------------------------------------------------------------------

#ifndef TFileListH
#define TFileListH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <FileCtrl.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmFileList : public TForm
{
__published:	// IDE-managed Components
  TFileListBox *flb;
  TPanel *Panel1;
  TBitBtn *BitBtn1;
  TBitBtn *BitBtn2;
  void __fastcall BitBtn2Click(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TfrmFileList(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmFileList *frmFileList;
//---------------------------------------------------------------------------
#endif
