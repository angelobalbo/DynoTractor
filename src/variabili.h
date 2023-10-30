
/*******************************************************************************
 Data: 15/10/2007
 File contenente tutte le variabili e le costanti utilizzate nel programma
 Dyno Race II versione Trattore


*******************************************************************************/
#include "Structs.h"

/// Tipo di banco


 String    Produttore="Dimsport s.r.l",
            Link ="www.dimsport.it",
            Versione_EXE="1.0.3";
            int Ver_sw=1000;

short                                                 
  // il tipo di banco selezionato:
  // 0=TRACTOR  1 brake, 2 cells -> PesoMax=1000
  // 1=TWIN     2 brakes, 2 cells -> PesoMax=1000
  // 2=TRAILER  1 brake, 1 cell   -> PesoMax=2000
  tipoBanco=TRACTOR,  
                    


       Vis_Air_Fuel= 0,  // variabile contenente il tipo di visualizzazione
                         // scelta per il rapporto stechiometrico
                         // 0 = 600 -1400
                         // 1 = 9.70 - 18.00 rapporto A/F

       Unit_Cop = 0,     // variabile contenente unità di misura della coppia
                         // 0 = Newton.metro
                         // 1 = Kilogrammo forza.metro
                         // 2 = Pound-force.piede

       Unit_Pot = 0,     // variabile contenente unità di misura della potenza
                         // 0 = Kilowatt
                         // 1 = Cavallo Potenza (metric)
                         // 2 = Hors Power

       Unit_Tem = 0,     // variabile contenente unità di misura delle temperature
                         // 0 = Gradi Celsius
                         // 1 = Fahrenheit


       Unit_Vel = 0,     // variabile contenente unità di misura della velocità
                         // 0 = Kilometri per ora
                         // 1 = miglio per ora
                         // 2 = metri/secondo

       Unit_Map = 0,     // variabile contenente unità di misura della pres. manifold
                         // 0 = mbar
                         // 1 = Psi


       Unit_Pes = 0,     //  variabile contenente unità di misura della forza/peso
                         // 0 = N
                         // 1 = Kg
                         // 2 = lb

       Unit_Cons = 0,     //  variabile contenente unità di misura della forza/peso
                         // 0 = l/h
                         // 1 = l/min
                         // 2 = l/Km
        Unit_Sup = 0,     //  variabile contenente unità di misura della superficie frontale
                         // 0 = m²
                         // 1 = ft²

        Imp_Ifut = 0,    // varabile d'impostazione uso ingresso analogico 0-15Vcc
                         // 0 -> Acceleratore
                         // 1 -> 0-15Vcc
                         // 2 -> Caratteristica custom

       sTipo_dato =0,    // tipo di grandezza(max valore) da visualizzare a fine test

       Vel_ini_tac  = 0,    // contiene velocità d'inizio per test tachimetro
       Step_vel_tac = 0,    // contiene lo step progressivo per test tachimetro

       num_val_cel=1,     // contiene il numero di letture valide delle celle di carico
       num_pass_step=0, //contiene il numero di acuisizioni dal cambiamento di step al raggiungimento del set point
       num_sec_attesa=0, //contiene il tempo di attesa ne test a step per avviare comunque l'acquisizione
       Lingua=0,         // variabile contenente il codice di lingua in uso
                         // 0 -> Italiano
                         // 1000 -> Inglese
                         // 2000 -> Tedesco
                         // 3000 -> Fracese
                         // 4000 -> Spagnolo
                         // 5000 -> eccetra...

                         // 0 -> Motore
       OK_Ctrl_error=3,  // contatore cicli
       count_com_Dypt=0, // contatore reinvii comandi Dypt

       sIndice =0,       // indice di puntamento array DATI impostazioni generali

       sNum_test=0,      // indice numero dei test di prova eseguiti

       sTest_antep=0,   // indice numero del test in anteprima dati...

       sCammes = 1,      // moltiplicatore giri albero a cammes

       sVentola=0,       // 0-> attivazione ventola manuale  1-> attivazione automatica (con giri rullo)

       Tstep=0,          // contatore di valori letti test a step

       C_Fail=0;         // contatore errori Diagnosi consecutivi

  unsigned short  usPwmAvg,
                  usPwmWrite,
                  usPwm=0, N_cost=0, // contiene il valore della coppia costante da inviare
                  N_cost_4p=0; //  contiene il valore della coppia costante solo asse post 4wd

  int  canPresent=0,
       Reg_iniz_acc = 0 ,    // contiene il regime di inizio test prova accelerativa
       Reg_fine_acc = 0 ,    // contiene il regime di fine test test prova accelerativa
       Tempo_acc = 0 ,       // contiene il tempo di durata accelerativa di test
       Contatore = 0,        // contatore posizione array valori acquisiti in tempo reale
       CCC=0,
       Perc=0,               // ifut settato in acceleratore
       iPos = 0,             // contiene posizione di riferimento array dati del test per scorrimento barra dati
       Reg_iniz_dec = 0 ,    // contiene il regime di inizio test prova decelerativa
       Reg_fine_dec = 0 ,    // contiene il regime di fine test test prova decelerativa
       Tempo_dec = 0 ,       // contiene il tempo di durata decelerativa di test

       Vel_iniz = 0 ,    // contiene la velocità di inizio test
       Vel_fine = 0 ,    // contiene la velocità di fine test

       rpmStart = 0 ,     // contiene il regime minimo di inizio step test
       rpmStop = 0 ,     // contiene il regime massimo di fine step test
       Delta_reg = 0 ,   // contiene il valore di incremento tra uno step e l'altro
       Reg_Stp   = 0 ,   // contiene valore giri/min stazionamento da inviare a freno/i
       Tempo_staz = 0 ,  // contiene il tempo di stazionamento a regime fisso
       Tipo_test = 0 ,   // contiene modalità del test da eseguire:
                         // 0 -> Test con stazionamento a tempo con step programmabili
                         // 1 -> Test con stazionamento a step fissi programmabili
                         // 2 -> Test decelerativo a seconda del regime motore
                         // 3 -> Simulazine Ciclo di prova
                         // 4 -> Trazione costante
                         // 5 -> Frenatura manuale del regime motore

       Tipo_ciclo = 0 ,  // contiene il tipo di motore
                         // 0 -> Otto
                         // 1 -> Diesel

       Tipo_Alim = 0 ,   // contiene il tipo di alimentazione motore:
                         // 0 -> Aspirato
                         // 1 -> Turbo
                         // 2 -> Sovralimentato (compressore)

       Tipo_norm = 0 ,   // contiene tipo norma di correzione:
                         // 0 -> DIN 70020
                         // 1 -> EWG 80/1269
                         // 2 -> ISO 1585
                         // 3 -> SAE J1349
                         // 4 -> JIS D1001
                         // 5 -> NESSUNA

    tipoRapp = RAP_RPMFIX ,  // contiene il tipo di calcolo del rapporto totale
                         // 0 -> Inserimento regime di calcolo del rapporto
                         // 1 -> Kit RPM per rilevamento automatico
                         // 2 -> Inserimento manuale del rapporto

    Reg_calc_rap = 0,    // contiene il regime scelto di calcolo del rapporto totate

    N_Cilindri = 0 ,     // contiene il numero di cilindri in uso

    N_Tempi = 0 ,        // contiene il numero di tempi del motore in esame
    Cont_letture_acc = 0,  // contatore di letture valori validi regimi per calcolo accelerazione istantanea
            num = 1,     // posizione in array buffer di scambio per dati seriali
       Cont_acq = 0,     // posizione in array buffer dati seriali

            l_m = 0,     // posizione in array tabelle lambda
           cont = 0,     // numero di campionamenti effettuato
         C_rap  = 0,     // usato per il calcolo del rapporto di trasmissione
         OK_calc = 1,    // se = 0 nessun calcolo automatico del rapporto ant
                         // se = 1 avvia il calcolo automatico del rapporto ant
         OK_calc_= 1,    // se = 0 nessun calcolo automatico del rapporto post
                         // se = 1 avvia il calcolo automatico del rapporto post
          Num_l  = 1,    // contatore per calcolo rapporto anteriore
          Num_ll = 1,    // contatore per calcolo rapporto posteriore
          Cont_Acq =0,   // contatore progressivo asse X per visualiz. grandezze Acquisite   Dyra
          Cont_Acq2 =0,   // contatore progressivo asse X per visualiz. grandezze Acquisite  Dyn3

          cicli_acc=1,

          KV_Int_calc = 150, // coeff variabile di calcolo intervallo giri motore

          iOffset_patm = 0, // offeset di aggiustamento valore pressione atmosferica
          iOffset_temp = 0, // offset di aggiustamento valore pressione atm. temporaneo

          Giri[500], // contiene i giri di riferimento interpolati dei grafici di coppia e potenza

         tabStepGiri[4][500],
         Giri_Acq[500], // contiene i giri di riferimento dei grafici coppia potenza


           imax_reg_A = 0,   // contiene regime massimo asse anteriore
           imax_reg_P = 0,   // contiene regime massimo asse posteriore


           iVA=0, // 1 valore comando inviato

           vNumTest[10]; // 0-> vuoto 1-> Test presente in memoria

  int  vGiri          [8][DIM_MEDIA];
  char vNoteTest      [8][DIM_MEDIA];
  char vNomeTest      [8][100];
  float vTermoK        [8][DIM_MEDIA];
  float vTermoOil      [8][DIM_MEDIA];
  float vPresMAP       [8][DIM_MEDIA];
  float vConsTot       [8][DIM_MEDIA];
  float vCoppiaRuota   [8][DIM_MEDIA];
  float vPotenzaRuota  [8][DIM_MEDIA];
  float vCons          [8][DIM_MEDIA];
  
  unsigned int uiVB=0,          // 2° parte del comando
               Risp_com_Dypt;   // risposta al comando da Dypt

  char

        Buffer[100],     // buffer di scambio stringhe di testo usate  per la lingua in uso
        Buffer_[160],      // buffer2 di scambio stringhe di testo usate  per la lingua in uso

        Note_acq[500],  // note del file di acquisizione

        Cliente[30],
        Targa[20],
        NOTE[500],
        Costruttore[30],
        Modello[30],
        Allestimento[30],

        nomeTest[100],
        Note_test[500],

           tabStepNGiri[500],
           tabStepNCoppia[500],
           tabStepNPot[500],
           tabStepNCons[500],
           tabStepNLamb[500],
           tabStepNMap[500],
           tabStepNTk[500],

        buf_reg[100],     // buffer di scambio per dati seriali


        Name_TAB[30], // identificativo sensore caratt. da cliente
        Y_TAB[20],   // ident. asse y


          Errori[][50]={
                            "Nessuno",
                            "Divisione per Zero",                        // 1
                            "Errore proveniente da porta seriale",      //  2
                            "File d'impostazione non presente",         //  3
                            "File di tarature celle non presente",       // 4
                            "File DynoRace.IDI non presente",            // 5
                            "Elettronica di gestione",                   // 6
                            "Importato file di taratura celle",          // 7
                            "Esportato file di taratura celle",          // 8
                            "Eseguita taratura cella anteriore modalità:", // 9
                            "Eseguita taratura cella posteriore modalità:",// 10
                                                                           // 11
                        };



  unsigned char ucFwType,
                ucFwVer, /*rpm[28],*/ // contiene i valori seriali
                CKS,     // contiene il checksum della stringa dati seriali
               Defect_code=0, // contiene il codice difetto dei freni
               ucVC=0, // 3° parte del comando
               Old_Defect=0; // contiene il vecchio difetto

  String    Profilo="User 1",  // contiene il nome del profilo in uso
            Percorso, // contiene il nome e percorso del file logo... (stampa)
            Ragione,// contiene il nome della ragione sociale
            Indirizzo,// indirizzo officina
            Telefono,// telefono officina
            Fax, // fax officina
            Internet, // Sito Internet
            EMail,  // indirizzo posta elettronica
            SNome_File, // nome file caratterizzazione custom ingresso IFut
                // stringhe di testo contenenti messaggi e titoli vari
            Str0,Str1,Str2,Str3,Str4,Str5,Str6,Str7,Str8,Str9,Str10,
            Str11,Str12,Str13,Str14,Str15,Str16,Str17,Str18,Str19,
            Str21,Str22,Str23,Str24,Str25,Str26,Str27,Str28,Str29,
            Str30,Text0,Text1,Text2,Text3,Text4,Text5,Text6,
            Text7,Text8,Text9,Text10,Text11,Text12,Text13,
            Text14,Text15,Text16,Text17,Text22,Text23,Text24,Text25,
            Text26,Text27,Text28,Text29,Text30,Text31,Text32,Text33,
            Text34,Text35,Text36,Text37,Text38,Text39,Text40,Text41,
            Text42,Text43,Text44,Text45,Text46,Text47,Text48,Text49,Text50,
            Text51,Text52,Text53,Text54,Text55,Text56,Text57,Text58,Text59,
            Text60,Text61,Text62,Text63,Text64,Text65,Text66,Text67,Text68,
            Text69,Text70,Text71,Text72,Text73,Text74,Text75,Text76,Text77,Text78,
            Text79,Text80,Text81,Text82,Text83,Text84,Text85,Text86,Text87,Text88,
            Text89,Text90,Text91,SS,Text200,Text201,Text202,Text203,Text204,Text205,
            Text206,Text207,normSAE,normISO,normCE,normECE,normCEE,normDIN,normOCDE,
            textSaveAcq,textCancAll,textAdd,textSameAcq,TextPowerData,TextEnvironmentData,txtLabelContaore,txtLabelLanci,
            TextTractorData,TextAmbPresOilHum,TextConsSpec,TextRPMminmax,TextRapporto,TextCons,txtGraphTitle;





// Istanze dei parametri generali di banco
  struct DYNO_SETS          dynoPrm;/**< Set dei parametri banco analogo a DATI_VARI*/
  struct TEST_SETS          testPrm;/**< Set dei parametri del test analogo a DAT */
  struct TEST_SETS          datPrm[8];/**< Set dei parametri del test analogo a DAT_1..DAT_8*/
  struct CELL_CALIB_PARAMS  cellPrm;/**< Set dei parametri cella analogo a TARA_CELL*/            

 /*

 Organizz. DATI_VARI[500]:

 ****  Blocco 1  [AUTO]       da    1 -> 100  ****
 ****  Blocco 2  [TIR]        da  101 -> 200  ****
 ****  Blocco 3  [BIKE]       da  201 -> 300  ****
 ****  Blocco 4  [TRATTORE]   da  301 -> 400  ****
 ****  Blocco 5  [MOTORE]     da  401 -> 500  ****



                            // [0] =  Non utilizzato



            Impostazioni 1 - 24

                            // [1] =  Revisione file impostazione
                            // [2] =  diametro dei rulli (mm)
                            // [3] =  Inerzia del bancata anteriore Kg/m^2
                            // [4] =  Inerzia del bancata posteriore Kg/m^2
                            // [5] =  Inerzia del freno anteriore  Kg/m^2
                            // [6] =  Inerzia del freno posteriore  Kg/m^2
                            // [7] =  lunghezza del braccio di reaz. cella Ant(mt)
                            // [8] =  lunghezza del braccio di reaz. cella Post(mt)
                            // [9] =  Carico max cella anteriore Kg
                            // [10] = Carico max cella posteriore Kg
                            // [11] = Caratteristica elettrica mV/V della cella anteriore
                            // [12] = Caratteristica elettrica mV/V della cella posteriore
                            // [13] = Tensione di alimentazione  cella anteriore (Volt):
                            // [14] = Tensione di alimentazione  cella posteriore (Volt):
                            // [15] = Tempo di letture msec
                            // [16] = Numero impulsi giro encoder
                            // [17] = Coefficiente di taratura termocoppia di tipo K 0-1000°C
                            // [18] = Costante della barra di taratura
                            // [19] = Peso campione (Kg)
                            // [20] = Inerzia del volano anteriore  Kg/m^2
                            // [21] = Inerzia del volano anteriore  Kg/m^2
                            // [22] =
                            // [23] =
                            // [24] =


             Filtraggi 25 - 49

                            // [25] =  Costante di filtraggio coppia inerziale ist e potenza real time
                            // [26] =  coeff. di saturazione filtro giri rullo
                            // [27] =  Costante di filtraggio strumento Lambda
                            // [28] =  Costante di filtraggio strumento Map
                            // [29] =  Costante di filtraggio Temp. Atmosf. (Ntc2)
                            // [30] =  Costante di filtraggio Temp. olio (Ntc1)
                            // [31] =  Costante di filtraggio pressione atmosferica
                            // [32] =  Costante di filtraggio umidità relativa
                            // [33] =  Numero di letture valide mediate celle di carico
                            // [34] =  Costante di filtraggio strumenti Ad-Fut
                            // [35] =  Costante di filtraggio accelerazione veicolo
                            // [36] =  Costante fi filtraggio picchi su RPM
                            // [37] =  Costante fi filtraggio campioni su RPM
                            // [38] =  Costante fi filtraggio benza su RPM
                            // [39] =  Costante fi filtraggio picchi su cella
                            // [40] =  Costante fi filtraggio campioni su cella
                            // [41] =  Costante fi filtraggio benza su cella
                            // [42] =



             Soglie 50 - 99


                            // [50] = Regime minimo di calcolo del grafico G/m:
                            // [51] = Intervallo di interpolazione grafica G/m:
                            // [52] = numero di campioni per calcolo grandezze
                            // [53] = Costante di calcolo prestazioni G/m (motore)
                            // [54] = Regime massimo di calcolo G/m
                            // [55] = Numero di campioni medio di calcolo Lam/K/Map
                            // [56] = Intervallo di calcolo prestazioni G/m (Rullo x passiva)
                            // [57] = Numero di letture valide calcolo accelerazione veicolo m/s^2
                            // [58] = Costante d'incremento prestazioni asse Anteriore (test 4WD) (Cv)


 Fine DATI_VARI[500]  */
 float
 //TARA_CELL[50],  // contiene dati di taratura delle celle di carico

                 // [0] = Data ultima taratura cella Front
                 // [1] = Valore in Kg del peso campione passato da DATI_VARI[19]
                 // [2] = vuoto
                 // [3] = valore lettura Front fase 1 (solo barra)
                 // [4] = valore lettura Front fase 2 (barra + peso noto)
                 // [5] = valore lettura Front fase 3 (senza nulla Tara)
                 // [6] = Delta Front tra lettura fase 2 e fase 1
                 // [7] = Offset di azzeramento valore a riposo Front
                 // [8] = valore costante del braccio Barra di taratura passato da DATI_VARI[18]
                 // [9] = 0-> usa caratt. elett. della cella   1-> taratura manuale
                 // [10] = vuoto

                 // [11] = Data ultima taratura cella Post
                 // [12] = Valore in Kg del peso campione passato da DATI_VARI
                 // [13] = vuoto
                 // [14] = valore lettura Post fase 1 (solo barra)
                 // [15] = valore lettura Post fase 2 (barra + peso noto)
                 // [16] = valore lettura Post fase 3 (senza nulla Tara)
                 // [17] = Delta Post tra lettura fase 2 e fase 1
                 // [18] = Offset di azzeramento valore a riposo Post
                 // [19] = valore costante del braccio Barra di taratura
                 // [20] = 0-> usa caratt. elett. della cella   1-> taratura manuale
                 // [21] = vuoto


  Coeff_fm =  0.3, // contiene fattore medio di correzione delle prestazioni
            fFact =  0,  // fattore di correzione curve di potenza
         Fattore_c = 0, // fattore di correzione se impostazione manuale
          Rap_tot1 =  0,  // Rapporto totale asse anteriore
          Valore_Cop = 0,  // valore di coppia costante
          ValCopVis = 0,
          K_Cop = 0,      // coefficente di conversione della coppia motore
          K_Pot = 0,      // coefficente di conversione della potenza motore
          K_Vel = 0,      // coefficente di conversione della velocità veicolo
          TAir=0,TAir0=0,
          V_T,
          TOil0=0,TOil=0,
          TOIL  = 0,      // contiene il valore della temperatura olio
       velocita = 0,      // contiene il valore della velocità veicolo
           Pmap = 0,Pmap0=0,      // contiene il valore di pressione assoluta mainfold
          Patmo = 0,      // contiene il valore della pressione atmosferica ATTUALE
            K_R = 0,      // contiene il valore della costante di taratura mainfold
   Val_cella_1 = 0,      // contiene il valore della cella di carico 1
   Val_cella_2 = 0,      // contiene il valore della cella di carico 2
   Val_cella_m1 = 0,     // contiene il valore medio della cella di carico 1
   Val_coppia_m1 =0,     // contiene il valore di coppia istantanea media cella 1
   Val_coppia_iner=0,
   Val_potenza_m1 =0,     // contiene il valore di potenza istantanea media
   Val_potenza_m2 =0,     // contiene il valore di potenza istantanea media
   Val_potenza_m3 =0,     // contiene il valore di potenza ant+post istantanea media
   Vcel1          =0,     // tensione cella anteriore
    Tens_Ad_fut = 0,      // contiene il valore di tensione AD Fut 0- 15 Vcc istantaneo
Tens_Ad_fut_min = 0,      // contiene il valore di tensione AD fut minimo scritto su file di taratura
Tens_Ad_fut_max =15,      // contiene il valore di tensione AD fut massimo scritto su file di taratura

       Val_cop_medio=0,   // contiene coppia media singolo punto test a step
       Val_reg_medio=0,   // contiene regime medio singolo punto test a step
       Val_map_medio=0,   // contiene pressione media singolo punto test a step
       Val_oil_medio=0,   // contiene olio media singolo punto test a step
       Val_k_medio=0,    // contiene termocoppia media singolo punto test a step

       Giri_moto[7200],
       Giri_ifut[7200],
       Giri_toil[7200],
       Giri_terk[7200],
       Giri_pmap[7200],
       Giri_copp[7200],
       Giri_pote[7200],
       Giri_cons[7200],         //aggiunti dalla versione 12
       Acq_dati[100],

       Val_pat   = 0,      // usata per il filtraggio valore pressione atmosferica
       Val_Hr    = 0,      // usata per il filtraggio valore umidità relativa
       Val_C_I   = 0,      // usata per il filtraggio valore coppia inerziale asse anteriore
       Val_C_IP  = 0,      // usata per il filtraggio valore coppia inerziale asse posteriore
       Val_Lambda   = 0,   // usata per il filtraggio valore Lambda
       Val_ACC      =0,    // usata per accelerazione veicolo
       Val_FRENO    = 0,   // usato per filrare il valore di cella inviato al freno
       fK_convP   = 0,      // contiene il coeff. di conversione per la potenza
       fK_convC   = 0,      // contiene il coeff. di conversione per la coppia
       fK_convV   = 0,      // contiene il coeff. di conversione per la velocità
       fK_convPr   = 0,     // contiene il coeff. di conversione per la pressione manifold
       fK_convPes  = 0,     // contiene il coeff. di conversione per la forza peso
       fK_convSup  = 0,     // contiene il coeff. di conversione per la superficie
       fK_convCons = 0,     // contiene il coeff. di conversione per i consumi
       fK_isteresi = 0,     // contiene il valore di velocità d'isteresi d'arresto test corrisp. a circa 5 Km/h

       Som_rap = 0,            // contiene la somma parziale dei rapporti di trasmissione calcolati
       Som_rap2= 0,            // contiene la somma parziale dei rapporti di trasmissione post calcolati (4wd)
       vettCons[500],
       fTermo_K[500],          // contiene valori acquisiti mediati della termocoppia 0-1000°C
       fTermo_Oil[500],        // contiene valori acquisiti mediati della termp olio 0-150°C
       fPres_MAP[500],         // contiene valori acquisiti mediati della pressione d'aspirazione
       fvettConsTot[500],           // contiene valori acquisiti mediati della lambda 0-1V  0-5V
       fAD_conv_15v[500],      // contiene valori acquisiti mediati del AD converter 0-15Vcc
       fRap_tot_A[500],      // contiene curva di rapporto totale asse anteriore
       fRap_tot_P[500],      // contiene curva di rapporto totale asse posteriore
       RPM_M[7500],         // giri acquisiti in tempo reale mediata
       C_M[7500],           // Coppi motrice acquisita in tempo reale mediata
       RPM_IST[7500],         // giri acquisiti in tempo reale mediata
       C_IST[7500],           // Coppi motrice acquisita in tempo reale mediata
       vSerTermoK[7500],
       vSerTermoOil[7500],
       vSerCons[7500],
       vSerConsTot[7500],
       vSerPresMap[7500],
       Tempo_accel = 0,      // contiene tempo di esecuzione del test parte accelerativa/dec
       Tempo_acc_tot = 0,    // contiene tempo totale di esecuzione del test

       C_Inerz_A=0,            // contiene coppia inerziale asse anteriore istantanea
       Old_reg_A=0,            // contiene regime old asse anteriore per calcolo coppia istantanea
       C_Inerz_P=0,            // contiene coppia inerziale asse posteriore istantanea
       Old_reg_P=0,            // contiene regime old asse posteriore per calcolo coppia istantanea
       Val_C_cost=0,           // contiene il valore di coppia costante per test (7)
       Old_vel=0,              // contiene il vecchio valore di velocità
       OLD_REG=0,              // contiene il regime vecchio per real time
       Percorso_Tot = 0,       // contiene il percorso totale del veicolo

       N_mot_rap = 0,          // giri di riferimento motore per calcolo rapporto
       N_pto_rap = 0,         // giri di riferimento pto per calcolo rapporto

  Facc_tmp=0,
  Facc_=0,

  Rend_PTO = 0.9,            // rendimento tipico della trasmissione PTO

  DAT[150],          // contiene tutti i dati del test

       /*
          DAT[0] = Rap_tot1 (rapporto di trasmissione totale asse anteriore)
          DAT[1] = Rap_tot2 (rapporto di trasmissione totale asse posteriore)
          DAT[2] = vuoto
          DAT[3] = vuoto
          DAT[4] = Tipo_traz (tipo di trazione asse in uso vedi sopra)
          DAT[5] = Val_t (temperatura atmosferica di esecuzione del test °C)
          DAT[6] = Val_pat (pressione atmosferica di esecuzione del test mbar)
          DAT[7] = Val_Hr  (umidità relativa di esecuzione del test %)
          DAT[8] = fFact (fattore di correzione prestazioni calcolato)
          DAT[9] = tipoBanco (tipo di banco utilizzato)
          DAT[10]= Tipo_test (tipo di test utilizzato per eseguire la prova)
          DAT[11]= Tipo_ciclo (tipo di ciclo del motore)
          DAT[12]= Tipo_Alim (alimentazione B/D)
          DAT[13]= Tipo_norm (tipo di normativa utilizzata
          DAT[14]= tipoRapp (tipo di sistema scelto per calcolo rapporto)
          DAT[15]= Reg_calc_rap (regime scelto per calcolo rapporto)
          DAT[16]= Marcia_test (marcia di prova)
          DAT[17]= Tipo_cambio (tipo di cambio)
          DAT[18]= sD_ruota_A (contiene il diametro di rotolamento della ruota)
          DAT[19]= sL (larghezza impronta a terra)
          DAT[20]= sH (spalla)
          DAT[21]= sR (raggio di rotolamento)
          DAT[22]=Tipo_carico
          DAT[23]=Passiva_calc
          DAT[24]=Vis_Air_Fuel
          DAT[25]=Unit_Cop
          DAT[26]=Unit_Pot
          DAT[27]=Unit_Tem
          DAT[28]=Unit_Vel
          DAT[29]=Unit_Map
          DAT[30]=Vel_ini_tac
          DAT[31]=Step_vel_tac
          DAT[32]=Lingua
          DAT[33]=Reg_iniz_acc
          DAT[34]=Reg_fine_acc
          DAT[35]=Tempo_acc
          DAT[36]=Reg_iniz_dec
          DAT[37]=Reg_fine_dec
          DAT[38]=Tempo_dec
          DAT[39]=Vel_iniz
          DAT[40]=Vel_fine
          DAT[41]=rpmStart
          DAT[42]=rpmStop
          DAT[43]=Delta_reg
          DAT[44]=Reg_Stp
          DAT[45]=Tempo_staz
          DAT[46]= Coeff_fm
          DAT[47]= TARA_CELL[0]
          DAT[48]= TARA_CELL[1]
          DAT[49]= TARA_CELL[2]
          DAT[50]= TARA_CELL[3]
          DAT[51]= TARA_CELL[4]
          DAT[52]= TARA_CELL[5]
          DAT[53] = DATI_VARI[1+sIndice]   Revisione file impostazione
          DAT[54] = DATI_VARI[2+sIndice]  diametro dei rulli (mm)
          DAT[55] = DATI_VARI[3+sIndice]  Inerzia del bancata anteriore Kg/m^2
          DAT[56] = DATI_VARI[4+sIndice]  Inerzia del bancata posteriore Kg/m^2
          DAT[57] = DATI_VARI[5+sIndice]  Inerzia del freno anteriore  Kg/m^2
          DAT[58] = DATI_VARI[6+sIndice]  Inerzia del freno posteriore  Kg/m^2
          DAT[59] = DATI_VARI[7+sIndice]  lunghezza del braccio di reaz. cella Ant(mm)
          DAT[60] = DATI_VARI[8+sIndice]  lunghezza del braccio di reaz. cella Post(mm)
          DAT[61] = DATI_VARI[9+sIndice]  Carico max cella anteriore Kg
          DAT[62] = DATI_VARI[10+sIndice] Carico max cella posteriore Kg
          DAT[63] = DATI_VARI[11+sIndice] Caratteristica elettrica mV/V della cella anteriore
          DAT[64] = DATI_VARI[12+sIndice] Caratteristica elettrica mV/V della cella posteriore
          DAT[65] = DATI_VARI[13+sIndice] Tensione di alimentazione  cella anteriore (Volt):
          DAT[66] = DATI_VARI[14+sIndice] Tensione di alimentazione  cella posteriore (Volt):
          DAT[67] = DATI_VARI[15+sIndice] Tempo di letture msec
          DAT[68] = DATI_VARI[16+sIndice] Numero impulsi giro encoder
          DAT[69] = DATI_VARI[17+sIndice] Coefficiente di taratura termocoppia di tipo K 0-1000°C
          DAT[70] = DATI_VARI[18+sIndice] Costante della barra di taratura
          DAT[71] = DATI_VARI[19+sIndice] Peso campione (Kg)
          DAT[72] = DATI_VARI[25+sIndice]  Costante di filtraggio giri rullo acquisiti
          DAT[73] = Regime_motore2         Regime_motore2 alla max coppia motore
          DAT[74] = DATI_VARI[27+sIndice]  Costante di filtraggio strumento Lambda
          DAT[75] = DATI_VARI[28+sIndice]  Costante di filtraggio strumento Map
          DAT[76] = DATI_VARI[29+sIndice]  Costante di filtraggio Temp. Atmosf. (Ntc2)
          DAT[77] = DATI_VARI[30+sIndice] Costante di filtraggio Temp. olio (Ntc1)
          DAT[78] = DATI_VARI[31+sIndice]  Costante di filtraggio pressione atmosferica
          DAT[79] = DATI_VARI[32+sIndice]  Costante di filtraggio umidità relativa
          DAT[80] = DATI_VARI[33+sIndice]  Numero di letture valide mediate celle di carico
          DAT[81] = DATI_VARI[50+sIndice] Regime minimo di calcolo del grafico G/m:
          DAT[82] = DATI_VARI[51+sIndice] Intervallo di interpolazione grafica G/m:
          DAT[83] = DATI_VARI[52+sIndice] numero di campioni per calcolo grandezze
          DAT[84] = DATI_VARI[53+sIndice] Intervallo di calcolo prestazioni G/m:
          DAT[85] = DATI_VARI[54+sIndice] Regime massimo di calcolo G/m
          DAT[86] = DATI_VARI[55+sIndice] Numero di campioni medio di calcolo Lam/K/Map
          DAT[87] = Tempo_accel  (contiene tempo di esecuzione del test parte accelerativa/dec)
          DAT[88] = Tempo_acc_tot (contiene tempo totale di esecuzione del test)
          DAT[89] = max_reg (contiene il massimo regime asse anteriore)
          DAT[90] = max_reg (contiene il massimo regime asse posteriore)
          DAT[91] = Max_pot_motore DIN
          DAT[92] = Max_pot_motore ECE
          DAT[93] = Max_pot_ruota; //
          DAT[94] = Max_cop_ruota; //
          DAT[95] = Max_pot_motore ECE
          DAT[96] = Regime_motore;
          DAT[97] = Max_manifol;
          DAT[98] = Max_termocoppia;
          DAT[99] = Max_temp_olio;
          DAT[100] = Max_lambda;
          DAT[101] = Max_AD;
          DAT[102] = Intervallo di calcolo prestazioni G/m rullo x passiva
          DAT[103] = Ver_sw;
          DAT[104] = selPto;

          DAT[110] = consumo @MaxPower
          DAT[111] = consumo @MaxCoppia
          DAT[112] = coppia @MaxRpm

       */



  Coppia_ruota[500], // coppia risultante da spline alla ruota
  Potenza_ruota[500],// potenza risultante alla ruota da coppia ruota
  Potenza_dissip[500],// potenza dissipata
  Coppia_dissip[500],  // coppia dissipata
  Coppia_motore[500], // coppia risultante da somma tra coppia ruota e cop dissipata
  Potenza_motore[500],// potenza risultante da somma tra pot ruota e pot dissipata

  tabStepCoppia[4][500],
  tabStepPot[4][500],
  tabStepCons[4][500],
  tabStepLamb[4][500],
  tabStepMap[4][500],
  tabStepTk[4][500],



       Max_pot_motore=0,  // massima potenza raggiunta del motore
       Max_pot_ruota=0,  // massima potenza raggiunta alla ruota
       Max_cop_motore=0, // massima coppia motore
       Max_cop_ruota=0,  // massima coppia ruota
       Max_pot_dissipata=0,  // massima potenza dissipata al regime della max potenza motore
       Max_manifol=0,  // massima pressione aspirazione raggiunta a max pot. mot.
       Max_termocoppia=0,  // massima temperatura K raggiunta a max pot. mot.
       Max_temp_olio=0,  // massima temperatura olio raggiunta a max pot. mot.
       Max_lambda=0,  // massima lambda raggiunta a max pot. mot.
       Max_AD=0,  // massima tensione AD 0-15Vcc raggiunta a max pot. mot.
       Regime_motore=0, // regime del motore di riferimento di tutti i valori massimi
       Regime_motore2=0, // regime del motore alla max coppia
       consMaxP,        // consumo alla massima potenza
       consMaxC,        // consumo alla max coppia
       coppiaMaxPow,    // coppia alla max potenza
       fnum_val[3]; // contatore di valori validi per visualizzazione dati debug


   double
    // variabili usate per la simulazione stradale
       fCx = 0,               // coefficente di penetrazione all'aria
       fS  = 0,               // sezione maestra del veicolo m^2
       fPendenza     = 0,     // pendenza del piano stradale in %
       fKrotolamento = 0, // coefficente di rotolamento
       Soglia_min = 0,   // contiene il valore di Vel/giri minimo per controllo coppia costante 
       frho= 0,               // massa volumica dell'aria Kg/m^3
       fPeso_veicolo = 0, // peso del veicolo in Kg
       fTemp_aria    = 0, // temperatura aria in °C
       fPres_aria    = 0, // pressione atmosferica in mbar
       fUmidita      = 0, // umidità relativa per calcolo massa volumica aria
       CUSTOM_TAB[256]; // tabella custom caratterizzata dal cliente



   long   lRpm=0,  // contiene i giri rulli ingresso 1
          lRpmPrec;  // contiene i giri rulli ingresso 2


   bool   // visualizza grandezze su grafici 0-> No  -  1-> Si
          Vis_P_M = 1,  // potenza motore
          Vis_P_R = 0,  // potenza ruota
          Vis_C_M = 1,  // coppia motore
          Vis_C_R = 0,  // coppia ruota
          Vis_P_D = 0,  // potenza dissipata

          Vis_Pmap = 0, // pressione aspirazione
          Vis_R_AB = 0, // rapporto aria/benzina
          Vis_cons = 0,
          Vis_T_K = 0,  // temperatura termocoppia K
          Vis_I_A = 0,  // Ingresso analogico 0-15Vcc
          Vis_T_N = 0,  // Temperatura Ntc Olio
          Vis_GV = 0,  // Visualizza su 0->giri  1->Velocità


          // stabilisce se visualizza strumento oppure no  0->No 1->Si

          Vis_Rpm=1,  // giri motore
          Vis_Pot=0,  // potenza
          Vis_Cop=0,  // coppia
         Vis_Pres=0,  // map
          Vis_TemK=0,  // temperatura K
          Vis_TemN=0,  // temperatura Ntc oil
          Vis_AirB=0,  // Lambda
           Vis_Acc=0,  // ingresso A/D 0-15Vcc
           Vis_Oil=0,  // tempreatura olio motore
           Allarme=0,  // gestione allarme visivo per superamento valori impostati

           bCalc_frho = true, // true=calcola automaticamente massa volumica dell'aria
                              // false=inserimento manuale dei dati di temp. e press. atmosferica

           Mod_imp=0,  // 0-> nessuna modifica 1-> impostazioni utente modificate
           OKF=false,  // controlla opzione di sovrascrittura file in memoria

           Out_dat=true, // stabilisce output dati rilevati 0= dati da file 1=real time

           Avvio_OK = true, // controlla stato chiusura programma
           Connect_Dyra = false, // stato connessione Dyra al banco
           Connect_Dypt = false, // stato connessione Dypt banco
           OK_W_Err     = true,  // ok per visualizzazione form errore di comunicazione
           OK_N_cost = false,        // inizio test simulazione
           Friz=true,             // controllo pedale frizione
           OK_TEST_AUTO = false, // stato di attivazione inizio test automatico 0-> man 1-> auto
           OK_TACHI= false, // stato di esecuzione test tachimetro           
           OK_TEST_STEP=false,   // per acquisizione test a step
           OK_ACQ_TEST_STEP=false,
           IO_ref=true,          // per controllo invio coppia costante
           OK_POT_rend_mot=true, // 0->coppia PTO 1->coppia motore
           Start_acq = false; // stabilisce inizio acquisizione dati durante test
                              // false -> nessuna acquisizione
                              // true -> acquisizione on



