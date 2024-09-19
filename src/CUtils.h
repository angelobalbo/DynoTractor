//---------------------------------------------------------------------------

#ifndef CUtilsH
#define CUtilsH

#include <vcl.h>

#ifdef _DYNOBIKE
#define FILE_FORMAT_TYPE    "DSDB"
#define FILE_FORMAT_VERS10  "10"
#define DYNO_TEST_PATH      "DynoBikeTest"
#define FILE_FORMAT_DAT     {'D','S','D','R','1','0','\0','\1'}
#define FILE_FORMAT_DYN     {'D','S','D','R','1','0','\0','\0'}
#define FILE_IMPO           "file_impo.dmt"
#define FILE_TARA           "file_tara.cel"
#endif
#ifdef _DYNORACE
#define FILE_FORMAT_TYPE    "DSDR"
#define FILE_FORMAT_VERS10  "10"
#define DYNO_TEST_PATH      "DynoRaceTest"
#define FILE_FORMAT_DAT     {'D','S','D','R','1','0','\1','\1'}
#define FILE_FORMAT_DYN     {'D','S','D','R','1','0','\1','\0'}
#define FILE_IMPO           "file_impo.dmt"
#define FILE_TARA           "file_tara.cel"
#endif
#ifdef _DYNOTRACTOR
#define FILE_FORMAT_TYPE    "DSDB"
#define FILE_FORMAT_VERS10  "10"
#ifdef _DYNOENGINE
#define DYNO_TEST_PATH      "DynoEngineTest"
#else
#define DYNO_TEST_PATH      "DynoTractorTest"
#endif
#define FILE_FORMAT_DAT     {'D','S','D','R','1','0','\2','\1'}
#define FILE_FORMAT_DYN     {'D','S','D','R','1','0','\2','\0'}
#define FILE_IMPO           "file_impo_trac.dmt"
#define FILE_TARA           "file_tara.cel"
#endif
#ifdef _DYNOTRUCK
#define FILE_FORMAT_TYPE    "DSDB"
#define FILE_FORMAT_VERS10  "10"
#define DYNO_TEST_PATH      "DynoTruckTest"
#define FILE_FORMAT_DAT     {'D','S','D','R','1','0','\3','\1'}
#define FILE_FORMAT_DYN     {'D','S','D','R','1','0','\3','\0'}
#endif

#define FILE_TYPE_DAT 1
#define FILE_TYPE_DYN 2
#define FILE_TYPE_ERR 0

#ifdef _VAMAG
#define PROGRAMDATA_DYNORACE  "\\Vamag\\Dynorace\\"
#else
#define PROGRAMDATA_DYNORACE  "\\Dimtech\\Dynorace\\"
#endif

#ifdef _VAMAG
#define PROGRAMDATA  "Vamag"
#else
#define PROGRAMDATA  "Dimtech"
#endif

//---------------------------------------------------------------------------
struct DAT_FILE_FORMAT
{
  char type[4];
  char ver [2];
  unsigned char header[6];
  unsigned char data [4000];
};

class CUtils
{
  char m_cFileType[4];
  char m_cFileVer[2];
  char m_cFileHead[6];
  unsigned char m_cFileHeadType;
  unsigned int  m_iDataLen;
  struct DAT_FILE_FORMAT m_datFile;
  String sProgramDataPath;
  String sProgramDataName;
  String sProgramPath;

public:
  void Encrypt(unsigned char* toEnc, unsigned char* output, int len);
  bool CheckDatFile (const char*);
  String GetSaveDir();
  CUtils()
  {
    String s;
     memcpy (m_cFileType, FILE_FORMAT_TYPE, sizeof(m_cFileType));
     m_cFileHeadType=FILE_TYPE_DAT;
     m_iDataLen=1;
     SetProgramDataPath();

  }


  void SetProgramDataPath();
  String GetProgramDataName(const char* cExt);
  String GetProgramDataPath(const char* cFileName);
  String GetProgramPath (const char* cFileName);
  String CreateSaveDir();
};

extern CUtils utils;
extern char g_cFileLog[MAX_PATH];
#endif
