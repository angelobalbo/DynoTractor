//---------------------------------------------------------------------------

#ifndef TCarica_acqH
#define TCarica_acqH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "NxCollection.hpp"
#include "NxEdit.hpp"
#include <ExtCtrls.hpp>
#include <FileCtrl.hpp>
#include <Graphics.hpp>
//---------------------------------------------------------------------------
class TCarica_acq : public TForm
{
__published:	// IDE-managed Components
        TNxHeaderPanel *NxHeaderPanel2;
        TDirectoryListBox *DirectoryListBox1;
        TNxHeaderPanel *NxHeaderPanel3;
        TFileListBox *FileListBox1;
        TNxHeaderPanel *NxHeaderPanel5;
        TNxMemo *NxMemo1;
        TNxHeaderPanel *NxHeaderPanel4;
        TNxLabel *NxLabel10;
        TNxLabel *NxLabel13;
        TNxLabel *NxLabel14;
        TNxLabel *NxLabel15;
        TNxLabel *NxLabel16;
        TNxLabel *NxLabel17;
        TNxLabel *NxLabel18;
        TNxLabel *NxLabel19;
        TNxLabel *NxLabel20;
        TNxLabel *NxLabel21;
        TNxLabel *NxLabel22;
        TNxLabel *NxLabel23;
        TImage *Image5;
        TNxButton *NxButton6;
        TDriveComboBox *DriveComboBox1;
        void __fastcall FileListBox1Click(TObject *Sender);
        void __fastcall NxButton6Click(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TCarica_acq(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TCarica_acq *Carica_acq;
//---------------------------------------------------------------------------
#endif
