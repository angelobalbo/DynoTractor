//---------------------------------------------------------------------------

#include <fstream.h>
#include <windows.h>          
#include <shfolder.h>
#pragma hdrstop

#include "CUtils.h"
#include "Log.h"

/// dati esterni per GetSaveDir
extern char Costruttore[30], Modello[30], Allestimento[30], Cliente[30], Targa[30];


//---------------------------------------------------------------------------
CUtils utils;

static char sDiagnostic[][25]=
{
                   "Info",               // 0
                   "Warning",             // 1
                   "Error: division by 0",// 2
                   "Error: NULL pointer", // 3
                   "Error: I/O",          // 4
                   "Error: critical",     // 5
};



bool CUtils::CheckDatFile (const char* name)
{
  bool bFileDat=false;
  char head[6];
  ifstream inFile(name, ios::binary);
  if (inFile.is_open())
  {
    inFile.read(head, 6);
    if (memcmp(head, FILE_FORMAT_TYPE,4)==0)
    {
      if (memcmp(&head[4], FILE_FORMAT_VERS10,2)==0)
      {
          bFileDat=true;
      }
    }
  }
  inFile.close();
  
  return bFileDat;
}

String CUtils::GetSaveDir()
{
  //ritorna la directory per il salvataggio
  AnsiString dir="";
  char cDocPath[MAX_PATH];
  char cPerc[MAX_PATH];
  char cPercPath[MAX_PATH];
  bool f;

  SHGetFolderPath(NULL, CSIDL_PERSONAL,NULL,NULL,cDocPath);
  sprintf(cPerc,"%s\\%s",cDocPath,DYNO_TEST_PATH);
  f=DirectoryExists(cPerc);
  if (!f)
  {
    try
    {
      CreateDirectory(cPerc,NULL);
    }
    catch(Exception &e)
    {
    }
  }
  //ricontrollo se è stato creato
  f=DirectoryExists(cPerc);
  if (f)
    dir=cPerc;
  if ((Costruttore[0]!='\0') && (f))
  {
    //controllo se esiste dir-costr
    sprintf(cPerc,"%s\\%s\\%s",cDocPath,DYNO_TEST_PATH, Costruttore);
    f=DirectoryExists(cPerc);
    if(f)
      dir=cPerc;
    if (Modello[0]!='\0' && f)
    {
      //controllo se esiste dir-costr-mod
      sprintf(cPerc,"%s\\%s\\%s\\%s",cDocPath,DYNO_TEST_PATH, Costruttore,Modello);
      f=DirectoryExists(cPerc);
      if(f)
        dir=cPerc;
      if (Allestimento[0]!='\0' && f)
      {
        //mi posiziono su marca modello e allestimento
        //controllo se esiste il percorso marca-mod-all
        sprintf(cPerc,"%s\\%s\\%s\\%s\\%s",cDocPath,DYNO_TEST_PATH,Costruttore,Modello,Allestimento);
        f=DirectoryExists(cPerc);
        if (f)
          dir=cPerc;
/*
        if (f & Cliente[0]!='\0')
        {
          //metto marca-modello-all
          sprintf(cPerc,"%s\\%s\\%s\\%s\\%s\\%s",cDocPath,DYNO_TEST_PATH,Costruttore,Modello,Allestimento,Cliente);
          f=DirectoryExists(cPerc);
          if (f)
            dir=cPerc;
        }*/
      }
    }
  }

  return dir;
}

String CUtils::CreateSaveDir()
{
  char cDocPath[MAX_PATH];
  SHGetFolderPath(NULL, CSIDL_PERSONAL,NULL,NULL,cDocPath);
  String sPath="";

   try
   {
     //creo la cartella di test
     sPath.cat_printf("%s\\%s",cDocPath,DYNO_TEST_PATH);
     CreateDirectory(sPath.c_str(),NULL);
     //creo la cartella di test\marca
     if(Costruttore[0]!=0)
     {
       sPath.cat_printf("\\%s",Costruttore);
       CreateDirectory(sPath.c_str(),NULL);
     }
     //creo la cartella di test\marca\modello
     if(Modello[0]!=0)
     {
       sPath.cat_printf("\\%s",Modello);
       CreateDirectory(sPath.c_str(),NULL);
     }
     //creo la cartella di test\marca\modello\allestimento
     if(Allestimento[0]!=0)
     {
       sPath.cat_printf("\\%s",Allestimento);
       CreateDirectory(sPath.c_str(),NULL);
     }
     //creo la cartella di test\marca\modello\allestimento\cliente
/*
     if(Cliente[0]!=0)
     {
       sPath.cat_printf("\\%s",Cliente);
       CreateDirectory(sPath.c_str(),NULL);
     }*/
   }
   catch(Exception &e)
   {
   }
  return sPath;
}

void CUtils::SetProgramDataPath()
{
//  String sPath;
  char   cPath[MAX_PATH];
  String sFile;

  sFile=ExtractFileName(Application->ExeName);
//PROGRAMDATA_DYNOBIKE
  SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA,NULL,NULL,cPath);
  sProgramDataName=(String)cPath;
  sProgramDataName.cat_sprintf( "\\%s\\%s\\%s",
                                PROGRAMDATA,
                                sFile.SubString(0,sFile.Length()-4).c_str(),
                                sFile.c_str());
  sProgramDataPath=(String)cPath;
  sProgramDataPath.cat_sprintf("\\%s\\%s\\",
                                PROGRAMDATA,
                                sFile.SubString(0,sFile.Length()-4).c_str());

  SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA,NULL,NULL,cPath);
  sProgramPath = ExtractFilePath(Application->ExeName);
  sprintf (g_cFileLog, "%s", ChangeFileExt(sProgramDataName, ".log").c_str());
  SHGetFolderPath(NULL, CSIDL_PERSONAL,NULL,NULL,cPath);
  strcat(cPath,"\\");
  strcat(cPath,DYNO_TEST_PATH);
  if (!DirectoryExists(cPath))
    CreateDirectory(cPath,NULL);
}

String CUtils::GetProgramPath (const char* cFileName)
{
  return sProgramPath+cFileName;
}

String CUtils::GetProgramDataName(const char* cExt)
{
  return ChangeFileExt(sProgramDataName, cExt);
}

String CUtils::GetProgramDataPath(const char* cFileName)
{

  return sProgramDataPath+(String)cFileName;
}
/// Algoritmo Crypt/Decryt
void CUtils::Encrypt(unsigned char* toEnc, unsigned char* output, int len)
{
  #define KEY_LEN 6
  unsigned char key[KEY_LEN] = {'\21', '\163', '\87','\213', '\27', '\71'}; //Any chars will work
  for (int i = 0; i < len; i++)
    output[i] = toEnc[i] ^ key[i%(sizeof(key)/sizeof(char))];
}


