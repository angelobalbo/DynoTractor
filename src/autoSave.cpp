//---------------------------------------------------------------------------


#pragma hdrstop
#include <windows.h>
#include <fstream.h>
#include <shfolder.h>

#include "CUtils.h"
#include "autoSave.h"


// globale
DAT_TYPE g_dynoTest;
char cArchivePath[MAX_PATH],cZipName[MAX_PATH];
static TZip* s_pZip;
static HANDLE hThreadSave;
void PrepareArchive(TZip* pZip)
{
  char cDocPath[MAX_PATH];
  s_pZip=pZip;
  SHGetFolderPath(NULL, CSIDL_PERSONAL,NULL,NULL,cDocPath);

    sprintf(cArchivePath,"%s\\%s",cDocPath,DYNO_TEST_PATH);
  if(!DirectoryExists((String)cArchivePath))
    CreateDirectory(cArchivePath,NULL);

  sprintf(cArchivePath,"%s\\%s\\archive",cDocPath,DYNO_TEST_PATH);
  if(!DirectoryExists((String)cArchivePath))
    CreateDirectory(cArchivePath,NULL);

  sprintf(cArchivePath,"%s\\%s\\archive\\%s",cDocPath,DYNO_TEST_PATH,Now().FormatString("YYYY"));
  if(!DirectoryExists((String)cArchivePath))
    CreateDirectory(cArchivePath,NULL);
  sprintf(cZipName,"%s\\%s.dyno.zip",cArchivePath,Now().FormatString("MM"));
  s_pZip->Reset();
  s_pZip->AppendFiles();
  s_pZip->ArchiveFile=cZipName;
  s_pZip->ExtractToPath=cArchivePath;
  s_pZip->ExtractAll();
  s_pZip->Reset();
  sprintf(cDocPath,"%s\\%s",cArchivePath,Now().FormatString("MM"));
  strcpy(cArchivePath,cDocPath);
  wchar_t *str = new wchar_t[((String)cArchivePath).WideCharBufSize()];
  //s_pZip->IncludeFiles(((String)cArchivePath).WideChar(str, ((String)cArchivePath).WideCharBufSize()));
  //s_pZip->Compress();
  delete[] str;


  if(!DirectoryExists((String)cArchivePath))
    CreateDirectory(cArchivePath,NULL);
}

DWORD WINAPI ThreadSave(LPVOID lpParam)
{
  //int *pNSerial=(int*)lpParam;
  char cFileName[MAX_PATH];
  TDateTime* data;

  data=(TDateTime*)&g_dynoTest.testPrms.dataL;
  sprintf(cFileName,"%s\\#%d%s.dyno",cArchivePath,(int)(g_dynoTest.testPrms.nLancio), data->FormatString("_YYYY-MM-DD_HH.NN.SS"));
  // salva il banco e tutti dati di tutte le prove effettuate
  ofstream out(cFileName, ios::out | ios::binary);
  memset (g_dynoTest.version,0,sizeof(g_dynoTest.version));
  strcpy(g_dynoTest.version,TRACTOR_VER_1);
  out.write((unsigned char *)&g_dynoTest,(sizeof(DAT_TYPE)-sizeof(CDyra::DataIn1)*(MAX_SERIAL-g_dynoTest.testPrms.nPunti)));
  out.close();
  s_pZip->AppendFiles();
  s_pZip->ArchiveFile=cZipName;
  wchar_t *str = new wchar_t[((String)cArchivePath).WideCharBufSize()];
  s_pZip->IncludeFiles(((String)cArchivePath).WideChar(str, ((String)cArchivePath).WideCharBufSize()));
  delete[] str;
  s_pZip->Compress();
  s_pZip->Reset();
  return 0;

}

int LoadDyno(const char* cName)
{
  char cFileName[MAX_PATH];
  char cFileVer [10];
  int  iNSerial=0;

  // Carica da file il banco e tutti dati di tutte le prove effettuate
  ifstream infile(cName, ios::binary);
  memset(&g_dynoTest,0,sizeof(g_dynoTest));
  if(infile.is_open() && (!infile.eof()))
  {
    infile.seekg (0, ios::beg);
    infile.read((unsigned char*)cFileVer,sizeof(cFileVer));
    infile.seekg (0, ios::beg);
    if(strcmp(cFileVer,TRACTOR_VER_1)==0)
    {
      infile.read((unsigned char*)&g_dynoTest,sizeof(g_dynoTest));
      infile.close();
      return g_dynoTest.testPrms.nPunti;
    }
  }

  infile.close();
  return 0;
}

void CloseArchive()
{
  //svuoto il contento della cartella
  TSearchRec fsr;
  AnsiString sNome,sPercorsoNome;

  if (FindFirst((String)cArchivePath+ "\\*.*",faAnyFile,fsr)==0)
  {
    do
    {
      //controllo che non siano . o ..
      sNome=AnsiString(fsr.Name);
      if ((sNome!=".") && (sNome!=".."))
      {
        //controllo che sia un file
        if (fsr.Attr==32)
        {
          //creo il percorso completo
          sPercorsoNome=(String)cArchivePath+"\\"+sNome;
          //cancello il file
          try
          {
            DeleteFile(sPercorsoNome);
          }
          catch(Exception &e)
          {
          }
        }
      }
    }
    while (FindNext(fsr) == 0);
    FindClose(fsr);
  }
  Application->ProcessMessages();
  RemoveDir(cArchivePath);
}

bool SaveDyno()
{
  hThreadSave=CreateThread( NULL,0,ThreadSave, NULL, 0, NULL);
  if(hThreadSave>0)
    return true;
  else
    return false;
}


//---------------------------------------------------------------------------

#pragma package(smart_init)
