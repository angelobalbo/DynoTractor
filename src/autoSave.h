//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
#include "Ziplib_OCX.h"
#include "structs.h"
#include "CDyra.h"

//#include "structs.h"
#define TRACTOR_VER_1 "TRAC01"


#define MAX_SERIAL 20000
  typedef struct
  {
    char version[10];
    char nomeTest [100];
    char costruttore[30];
    char modello[30];
    char allestimento[30];
    char cliente [30];
    char targa[30];
    struct TEST_SETS testPrms;
    struct DYNO_SETS dynoPrms;
    CDyra::DataIn1 serialDat[MAX_SERIAL];
  } DAT_TYPE;
extern DAT_TYPE g_dynoTest;
extern char cArchivePath[MAX_PATH];
bool SaveDyno();
int  LoadDyno(const char* cName);
void PrepareArchive(TZip* pZip);
void CloseArchive();


//---------------------------------------------------------------------------
#endif
