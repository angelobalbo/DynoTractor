//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "log.h"
//---------------------------------------------------------------------------
USEFORM("TMain.cpp", Main);
USEFORM("TGiri_Mot.cpp", Giri_Mot);
USEFORM("TCoppia.cpp", Coppia);
USEFORM("TPotenza.cpp", Potenza);
USEFORM("TMap.cpp", Map);
USEFORM("TTermo.cpp", Termo);
USEFORM("TImpostazioni.cpp", Impostazioni);
USEFORM("TInfo.cpp", Info);
USEFORM("TStrumenti.cpp", Strumenti);
USEFORM("TMisure.cpp", Misure);
USEFORM("TUtenti.cpp", Utenti);
USEFORM("TRapporto.cpp", Rapporto);
USEFORM("TStart.cpp", Start);
USEFORM("TAcc.cpp", Acc);
USEFORM("TOpzioni_grafici.cpp", Opzioni_grafici);
USEFORM("TSetup.cpp", Setup);
USEFORM("TStep_test.cpp", Step_test);
USEFORM("TPilot_Freno.cpp", Pilot_Freno);
USEFORM("TCarica_File.cpp", Carica_File);
USEFORM("TConnessione.cpp", Connessione);
USEFORM("TSplash.cpp", Splash);
USEFORM("TNote.cpp", Note);
USEFORM("TMt_cost.cpp", Mt_cost);
USEFORM("TOil_Temp.cpp", Oil_Temp);
USEFORM("TFattCorr.cpp", FattCorr);
USEFORM("TFileList.cpp", frmFileList);
USEFORM("TCan.cpp", frmCan);
USEFORM("TDebugMode.cpp", frmDebugMode);
//---------------------------------------------------------------------------
extern char g_cFileLog[MAX_PATH];
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

     


  try
  {
     Application->Initialize();
     Application->Title = "Dyno Tractor";
     #ifdef _DEBUG
        LogError(g_cFileLog,LOG_INFO,"Inizializzazione Dynotractor");
    #endif
     Application->CreateForm(__classid(TMain), &Main);
     Application->CreateForm(__classid(TGiri_Mot), &Giri_Mot);
     Application->CreateForm(__classid(TCoppia), &Coppia);
     Application->CreateForm(__classid(TImpostazioni), &Impostazioni);
     Application->CreateForm(__classid(TPotenza), &Potenza);
     Application->CreateForm(__classid(TMap), &Map);
     Application->CreateForm(__classid(TTermo), &Termo);
     Application->CreateForm(__classid(TOil_Temp), &Oil_Temp);
     Application->CreateForm(__classid(TStrumenti), &Strumenti);
     Application->CreateForm(__classid(TMisure), &Misure);
     Application->CreateForm(__classid(TUtenti), &Utenti);
     Application->CreateForm(__classid(TRapporto), &Rapporto);
     Application->CreateForm(__classid(TStart), &Start);
     Application->CreateForm(__classid(TAcc), &Acc);
     Application->CreateForm(__classid(TOpzioni_grafici), &Opzioni_grafici);
     Application->CreateForm(__classid(TSetup), &Setup);
     Application->CreateForm(__classid(TStep_test), &Step_test);
     Application->CreateForm(__classid(TPilot_Freno), &Pilot_Freno);
     Application->CreateForm(__classid(TCarica_File), &Carica_File);
     Application->CreateForm(__classid(TConnessione), &Connessione);
     Application->CreateForm(__classid(TNote), &Note);
     Application->CreateForm(__classid(TMt_cost), &Mt_cost);
     Application->CreateForm(__classid(TFattCorr), &FattCorr);
     Application->CreateForm(__classid(TfrmFileList), &frmFileList);
     Application->CreateForm(__classid(TfrmCan), &frmCan);
     Application->CreateForm(__classid(TfrmDebugMode), &frmDebugMode);
     #ifdef _DEBUG
      LogError(g_cFileLog,LOG_INFO,"Termine CreateForm");
     #endif
     Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        catch (...)
        {
                 try
                 {
                         throw Exception("");
                 }
                 catch (Exception &exception)
                 {
                         Application->ShowException(&exception);
                 }
        }
        return 0;
}
//---------------------------------------------------------------------------
