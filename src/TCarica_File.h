//---------------------------------------------------------------------------

#ifndef TCarica_FileH
#define TCarica_FileH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <FileCtrl.hpp>
#include <Chart.hpp>
#include <ExtCtrls.hpp>
#include <Series.hpp>
#include <TeEngine.hpp>
#include <TeeProcs.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>
#include "NxCollection.hpp"
//---------------------------------------------------------------------------
class TCarica_File : public TForm
{
__published:	// IDE-managed Components
        TDirectoryListBox *DirectoryListBox1;
        TFileListBox *FileListBox1;
        TDriveComboBox *DriveComboBox1;
        TListBox *ListBox1;
        TBitBtn *BitBtn1;
        TLabel *Label6;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label4;
        TPopupMenu *PopupMenu1;
        TMenuItem *Eliminafile1;
        TMenuItem *Rinominalasessione1;
        TBitBtn *BitBtn2;
        TNxFlipContainer *NxFlipContainer1;
        TChart *Chart1;
        TLineSeries *Series1;
        TLineSeries *Series2;
        void __fastcall FileListBox1Click(TObject *Sender);
        void __fastcall ListBox1Click(TObject *Sender);
        void __fastcall BitBtn1Click(TObject *Sender);
        void __fastcall FileListBox1ContextPopup(TObject *Sender,
          TPoint &MousePos, bool &Handled);
        void __fastcall Eliminafile1Click(TObject *Sender);
        void __fastcall BitBtn2Click(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TCarica_File(TComponent* Owner);


};
//---------------------------------------------------------------------------
extern PACKAGE TCarica_File *Carica_File;
//---------------------------------------------------------------------------
#endif
