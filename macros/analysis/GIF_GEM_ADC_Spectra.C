// Copyright (C) Experimental High Energy Physics and Applications Group, VECC, Kolkata
// Author(s): Apar Agarwal

// This macro has been created for the purpose
// analysing various aspects of ADC and Cluster Charge
// of GEM for GIF++ 2023 data

// This Code requires the CbmRoot version DEC 21 or higher
// Recommended on Linux 20.04 --> Fairsoftversion : APR21p2 and FAIRROOTVERSION: v18.6.7 or higher

// Code first tested on 28th September, 2023

void GIF_GEM_ADC_Spectra(Int_t RunId = 740,
                         Double_t Tcorr_Mean = 378.9,  // Time correlation mean in ns
                         Double_t Tcorr_Sigma = 19.23, // Time correlation mean in ns
                         Int_t Number_Of_SD_Cut = 2)   // How many standard deviation cuts is applied
{
    TStopwatch Code_Timer;
    Code_Timer.Start();

    TString InputFileName = "data/Run" + to_string(RunId) + ".digi.root";
    TString GEM_Noise_File_Name = "Run_" + to_string(RunId) + "_GEM_Noise.txt";
    TString Three_Fold_Acceptance_File_Name = "Three_Fold_Area_Config_2.txt";

    Double_t Coincidence_Window_Lower_Bound_mus = 0.001 * (Tcorr_Mean - Number_Of_SD_Cut * Tcorr_Sigma); // Assuming 3 sigma cut
    Double_t Coincidence_Window_Upper_Bound_mus = 0.001 * (Tcorr_Mean + Number_Of_SD_Cut * Tcorr_Sigma);

    if (gSystem->AccessPathName(InputFileName))
    {
        cout << "The file: " << InputFileName << " does not exist!" << endl;
        return;
    }

    if (gSystem->AccessPathName(GEM_Noise_File_Name))
    {
        cout << "The file: " + GEM_Noise_File_Name + " does not exist!" << endl;
        return;
    }

    if (gSystem->AccessPathName(Three_Fold_Acceptance_File_Name))
    {
        cout << "The file: " + Three_Fold_Acceptance_File_Name + " does not exist!" << endl;
        return;
    }

    TFile* fIn = new TFile(InputFileName);
    cout << "Input Run Number:" << RunId << "  File Name: " << InputFileName << endl;

    TTree* Input_Tree = (TTree*)fIn->Get("cbmsim");

    Int_t Three_Fold_Area[23][97] = {0};
    Int_t Three_Fold_Channel, Three_Fold_Sector;

    for (Int_t iChannel = 0; iChannel < 23; iChannel++)
    {
        for (Int_t iSector = 0; iSector < 97; iSector++)
        {
            Three_Fold_Area[iChannel][iSector] = 0;
        }
    }

    ifstream Three_Fold_Area_Infile;

    Three_Fold_Area_Infile.open(Three_Fold_Acceptance_File_Name);

    while (!Three_Fold_Area_Infile.eof())
    {
        Three_Fold_Area_Infile >> Three_Fold_Channel >> Three_Fold_Sector;
        //Three_Fold_Area[Three_Fold_Channel][Three_Fold_Sector]=1;
        Three_Fold_Area[Three_Fold_Channel][Three_Fold_Sector] = 1;
    }

    Three_Fold_Area_Infile.close();

    Int_t Noisy_Channel, Noisy_Sector;
    Int_t GEM_Noise[23][97] = {0};

    ifstream GEM_Noise_Infile;

    GEM_Noise_Infile.open(GEM_Noise_File_Name);

    while (!GEM_Noise_Infile.eof())
    {
        GEM_Noise_Infile >> Noisy_Channel >> Noisy_Sector;
        GEM_Noise[Noisy_Channel][Noisy_Sector] = -1;
    }

    GEM_Noise_Infile.close();

    TString OutputFileName = "data/ADC_Related_Plots/Run" + to_string(RunId) + "_GEM_ADC_Spectra." + to_string(Number_Of_SD_Cut) + "_Sigma_SD_Cut.root";
    TFile* fOut = new TFile(OutputFileName, "recreate");

    //****************************** START: Reading Mapping for mRPC **********************************************************

    Int_t RPCarrayX[512] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1,           //15
                            1, 1, 1, 1, 2, 0, 2, 2, 2, 2, 2, 2, 2, 2, 1, 3,           //31
                            /*Feb 0*/ 3, 2, 3, 3, 3, 3, 3, 3, 0, 3, 2, 3, 4, 4, 4, 1, //47
                            4, 4, 4, 4, 4, 4, 3, 4, 2, 5, 1, 0, 5, 4, 5, 5,           //63
                            5, 5, 5, 5, 5, 5, 3, 5, 3, 2, 2, 4, 0, 1, -3, -3,         //79
                            6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 4, 6, 3, 5, 1, 2,           //95
                            7, 0, 7, 7, 7, 7, 4, 7, 7, 5, 7, 7, 7, 7, 5, 6,           //111
                            0, 1, 3, 2, 8, 6, 8, 8, 8, 8, 3, 8, 8, 4, 8, 8,           //127

                            8, 8, 5, 0, 6, 1, 7, 7, 9, 9, 9, 0, 4, 2, 9, 9,           //143
                            9, 3, 9, 9, 9, 9, 4, 9, 3, 4, 0, 2, 5, 1, 5, 5,           //159
                            /*Feb 1*/ 6, 6, 6, 6, 7, 7, 7, 7, 8, 7, 8, 8, 8, 8, 2, 1, //175
                            4, 3, 6, 5, 0, 7, 8, 1, 9, 9, 9, 2, 3, 8, 5, 4,           //191
                            0, 6, 1, 8, 7, 2, 9, 9, 4, 3, 6, 5, 8, 7, 6, 7,           //207
                            4, 5, 2, 3, 0, 1, 8, 8, 9, 9, 9, 9, 4, 9, 6, 5,           //223
                            8, 7, 9, 9, 8, 9, 2, 3, 7, 1, 6, 4, 9, 5, 8, 0,           //239
                            6, 7, 1, 5, 3, 2, 3, 4, 1, 2, 0, 0, 0, 0, -3, -3,         //255

                            0, 0, 2, 1, 8, 9, 6, 7, 0, 5, 1, 4, 1, 1, 9, 3,           //271
                            2, 7, 5, 6, 2, 3, 2, 3, 0, 1, 8, 4, 9, 4, 7, 8,           //287
                            /*Feb 2*/ 5, 6, 8, 9, 7, 1, 2, 6, 4, 5, 9, 3, 7, 8, 5, 6, //303
                            3, 4, 9, 2, 0, 7, 3, 5, 1, -3, 4, -3, 6, -3, 8, -3,       //319
                            9, 2, 3, 8, 5, 4, 7, 6, 8, 9, 6, 7, 4, 5, 1, 3,           //335
                            9, 2, 0, 8, 6, 7, 4, 5, 2, 3, 8, 9, 5, 6, 4, 1,           //351
                            7, 3, 2, 9, -3, 8, -3, 7, -3, 6, -3, 5, -3, 4, -3, 3,     //367
                            -3, 1, -3, 2, 8, 9, -3, 7, 1, 6, 0, 0, -3, -3, 0, -3,     //383

                            0, 0, 1, 0, 3, 2, 5, 4, 8, 9, 7, 0, 5, 6, 4, 1,                  //399
                            2, 3, 8, 9, 0, 1, 6, 7, 4, 5, 2, 3, 8, 9, 1, 7,                  //415
                            /*Feb 3*/ 5, 6, 3, 4, 9, 2, 7, 8, 1, 6, 2, 5, 4, 3, 8, 9,        //431
                            6, 7, 4, 5, 2, 3, 9, 1, 7, 8, 5, 6, 3, 4, 9, 2,                  //447
                            7, 8, 5, 6, 3, 4, 2, 0, 9, 1, 7, 8, 5, 6, 3, 4,                  //463
                            1, 2, 8, 9, 6, 7, 4, 5, 2, 3, 1, 1, -3, -3, -3, -3,              //479
                            -3, -3, -3, -3, -3, -3, 0, 0, 0, -3, 0, -3, -3, -3, -3, -3,      //495
                            -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3}; //511


    Int_t RPCarrayY[512] = {1, 0, 3, 2, 5, 4, 7, 6, 0, 8, 2, 1, 3, 9, 5, 4,              //15
                            7, 6, 9, 8, 0, 10, 2, 1, 4, 3, 6, 5, 8, 7, 10, 0,            //31
                            /*Feb 0*/ 1, 9, 3, 2, 5, 4, 7, 6, 11, 8, 10, 9, 1, 0, 2, 11, //47
                            4, 3, 6, 5, 8, 7, 10, 9, 11, 0, 12, 12, 1, 10, 3, 2,         //63
                            5, 4, 7, 6, 9, 8, 11, 10, 12, 12, 13, 11, 13, 13, -3, -3,    //79
                            1, 0, 3, 2, 5, 4, 7, 6, 9, 8, 12, 10, 13, 11, 14, 14,        //95
                            0, 14, 2, 1, 4, 3, 13, 5, 6, 12, 8, 7, 10, 9, 13, 11,        //111
                            15, 15, 14, 15, 0, 12, 2, 1, 4, 3, 15, 5, 6, 14, 8, 7,       //127

                            10, 9, 14, 16, 13, 16, 11, 12, 1, 0, 3, 17, 15, 16, 2, 4,                 //143
                            5, 16, 7, 6, 9, 8, 16, 10, 17, 17, 18, 17, 15, 17, 17, 16,                //159
                            /*Feb 1*/ 15, 14, 17, 16, 15, 14, 17, 16, 12, 13, 14, 13, 16, 15, 18, 18, //175
                            18, 18, 18, 18, 19, 18, 11, 19, 12, 11, 13, 19, 19, 17, 19, 19,           //191
                            20, 19, 20, 18, 19, 20, 15, 14, 20, 20, 20, 20, 20, 20, 21, 21,           //207
                            21, 21, 21, 21, 21, 21, 19, 21, 17, 16, 19, 18, 22, 20, 22, 22,           //223
                            22, 22, 22, 21, 23, 23, 22, 22, 23, 22, 23, 23, 24, 23, 24, 22,           //239
                            24, 24, 23, 24, 23, 23, 24, 24, 24, 24, 23, 24, 26, 25, -3, -3,           //255

                            27, 28, 25, 25, 25, 25, 25, 25, 29, 25, 26, 25, 28, 27, 26, 25,           //271
                            26, 26, 26, 26, 27, 26, 28, 27, 30, 29, 26, 26, 27, 27, 27, 27,           //287
                            /*Feb 2*/ 27, 27, 28, 28, 28, 30, 29, 28, 28, 28, 29, 28, 29, 29, 29, 29, //303
                            29, 29, 30, 30, 31, 30, 30, 30, 31, -3, 30, -3, 30, -3, 30, -3,           //319
                            31, 31, 31, 31, 31, 31, 31, 31, 32, 32, 32, 32, 32, 32, 32, 32,           //335
                            33, 32, 32, 33, 33, 33, 33, 33, 33, 33, 34, 34, 34, 34, 34, 33,           //351
                            34, 34, 34, 35, -3, 35, -3, 35, -3, 35, -3, 35, -3, 35, -3, 35,           //367
                            -3, 34, -3, 35, 36, 36, -3, 36, 35, 36, 35, 34, -3, -3, 33, -3,           //383

                            37, 36, 36, 38, 36, 36, 36, 36, 37, 37, 37, 39, 37, 37, 37, 37,           //399
                            37, 37, 38, 38, 40, 38, 38, 38, 38, 38, 38, 38, 39, 39, 39, 39,           //415
                            /*Feb 3*/ 39, 39, 39, 39, 40, 39, 40, 40, 40, 40, 40, 40, 40, 40, 41, 41, //431
                            41, 41, 41, 41, 41, 41, 42, 41, 42, 42, 42, 42, 42, 42, 43, 42,           //447
                            43, 43, 43, 43, 43, 43, 43, 41, 44, 42, 44, 44, 44, 44, 44, 44,           //463
                            43, 44, 45, 45, 45, 45, 45, 45, 45, 45, 44, 45, -3, -3, -3, -3,           //479
                            -3, -3, -3, -3, -3, -3, 44, 45, 43, -3, 42, -3, -3, -3, -3, -3,           //495
                            -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3};          //511

    Int_t RPC_Feb_no_X_Y[10][46] = {0};
    Int_t RPC_Ch_no_X_Y[10][46] = {0};

    for (Int_t j = 0; j < 46; j++)
    {
        for (Int_t i = 0; i < 10; i++)
        {
            RPC_Feb_no_X_Y[i][j] = -1;
            RPC_Ch_no_X_Y[i][j] = -1;
        }
    }

    for (Int_t i = 0; i < 512; i++)
    {
        if (RPCarrayX[i] >= 0)
        {
            Int_t Feb_number = i / 128;
            Int_t Channel_number = i % 128;
            RPC_Feb_no_X_Y[RPCarrayX[i]][RPCarrayY[i]] = Feb_number;
            RPC_Ch_no_X_Y[RPCarrayX[i]][RPCarrayY[i]] = Channel_number;
        }
    }

    //****************************** END: Reading Mapping for mRPC ************************************************************

    TString Input_X_Mapping_GEM = "PadX_GEM.txt";
    TString Input_Y_Mapping_GEM = "PadY_GEM.txt";

    ifstream GEM_Infile_X;

    GEM_Infile_X.open(Input_X_Mapping_GEM);

    Int_t GEM_Coordinate_Feb_Channel[18][128][2] = {0}; //0 for X and 1 for Y coordinate

    for (Int_t i = 0; i < 18; i++)
    {
        for (Int_t j = 0; j < 128; j++)
        {
            // if(i==0 || i==1 || i==3 || i==6 || i==10 || i==17)
            // GEM_Infile_X >> GEM_Coordinate_Feb_Channel[i][127-j][0];
            // else
            GEM_Infile_X >> GEM_Coordinate_Feb_Channel[i][j][0];
        }
    }

    GEM_Infile_X.close();

    ifstream GEM_Infile_Y;

    GEM_Infile_Y.open(Input_Y_Mapping_GEM);

    for (Int_t i = 0; i < 18; i++)
    {
        for (Int_t j = 0; j < 128; j++)
        {
            GEM_Infile_Y >> GEM_Coordinate_Feb_Channel[i][j][1];
        }
    }

    GEM_Infile_Y.close();

    Int_t GEM_Feb_no_X_Y[23][97] = {0};
    Int_t GEM_Ch_no_X_Y[23][97] = {0};

    for (Int_t j = 0; j < 97; j++)
    {
        for (Int_t i = 0; i < 23; i++)
        {
            GEM_Feb_no_X_Y[i][j] = -1;
            GEM_Ch_no_X_Y[i][j] = -1;
        }
    }

    for (Int_t i = 0; i < 18; i++)
    {
        for (Int_t j = 0; j < 128; j++)
        {
            if (GEM_Coordinate_Feb_Channel[i][j][0] >= 0 && GEM_Coordinate_Feb_Channel[i][j][1] >= 0)
            {
                GEM_Feb_no_X_Y[GEM_Coordinate_Feb_Channel[i][j][0]][GEM_Coordinate_Feb_Channel[i][j][1]] = i;
                GEM_Ch_no_X_Y[GEM_Coordinate_Feb_Channel[i][j][0]][GEM_Coordinate_Feb_Channel[i][j][1]] = j;
            }
        }
    }

    //****************************** END: Reading Mapping for Gem ***********************************************************


    cout << "Number of Events in the Tree: " << Input_Tree->GetEntries() << endl;
    Int_t nTimeSlices = Input_Tree->GetEntries();

    TTreeReader Tree_Data_Reader("cbmsim;", fIn); // Both TTree and TTreeReader are not required generally but kept in case some functionalities of TTree are needed

    TTreeReaderArray<CbmMuchDigi> Much_Digi(Tree_Data_Reader, "MuchDigi"); // MuchDigi -> Branch Name  and Much_Digi --> Element to Read Much data
    TTreeReaderValue<CbmTsEventHeader> TimeSlice_Event_Header(Tree_Data_Reader, "EventHeader.");

    Int_t iTreeEntry;
    Int_t FirstDigiFlag = 0;
    Int_t LastDigiFlag = 0;
    uint64_t First_Digi_Time;
    uint64_t Last_Digi_Time;
    TString HistTitle, HistTag;

    HistTag = "GEM_Raw_ADC_Spectra";
    HistTitle = "GEM Raw ADC Spectra for Run " + to_string(RunId);
    TH1I* GEM_Raw_ADC_Spectra = new TH1I(HistTag, HistTitle, 35, -1.5, 33.5);
    GEM_Raw_ADC_Spectra->GetXaxis()->SetTitle("ADC");
    GEM_Raw_ADC_Spectra->GetYaxis()->SetTitle("Counts");

    TH1I* GEM_Febwise_Raw_ADC_Spectra[18];

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTag = "GEM_Raw_ADC_Spectra_Feb_" + to_string(iFeb);
        HistTitle = "GEM Raw ADC Spectra of Feb " + to_string(iFeb) + " for Run " + to_string(RunId);
        GEM_Febwise_Raw_ADC_Spectra[iFeb] = new TH1I(HistTag, HistTitle, 35, -1.5, 33.5);
        GEM_Febwise_Raw_ADC_Spectra[iFeb]->GetXaxis()->SetTitle("ADC");
        GEM_Febwise_Raw_ADC_Spectra[iFeb]->GetYaxis()->SetTitle("Counts");
    }

    HistTag = "GEM_Raw_ADC_Spectra_Febwise_2D";
    HistTitle = "GEM 2D Raw ADC Spectra Febwise for Run " + to_string(RunId);
    TH2F* GEM_Raw_ADC_Spectra_Febwise_2D = new TH2F(HistTag, HistTitle, 20, -1.5, 18.5, 35, -1.5, 33.5);
    GEM_Raw_ADC_Spectra_Febwise_2D->GetXaxis()->SetTitle("FEB Number");
    GEM_Raw_ADC_Spectra_Febwise_2D->GetYaxis()->SetTitle("ADC");

    TH2F* GEM_Channelwise_Raw_ADC_Spectra[18];

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTag = "GEM_Raw_ADC_Spectra_Channelwise_Feb_" + to_string(iFeb);
        HistTitle = "GEM Raw ADC Spectra (Channelwise) of Feb " + to_string(iFeb) + " for Run " + to_string(RunId);
        GEM_Channelwise_Raw_ADC_Spectra[iFeb] = new TH2F(HistTag, HistTitle, 131, -1.5, 129.5, 35, -1.5, 33.5);
        GEM_Channelwise_Raw_ADC_Spectra[iFeb]->GetXaxis()->SetTitle("Channel Number");
        GEM_Channelwise_Raw_ADC_Spectra[iFeb]->GetYaxis()->SetTitle("ADC");
    }

    HistTag = "GEM_ADC_Spectra_Noise_Masked";
    HistTitle = "GEM ADC Spectra with Noise Masking for Run " + to_string(RunId);
    TH1I* GEM_ADC_Spectra_Noise_Masked = new TH1I(HistTag, HistTitle, 35, -1.5, 33.5);
    GEM_ADC_Spectra_Noise_Masked->GetXaxis()->SetTitle("ADC");
    GEM_ADC_Spectra_Noise_Masked->GetYaxis()->SetTitle("Counts");

    TH1I* GEM_Febwise_ADC_Spectra_Noise_Masked[18];

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTag = "GEM_ADC_Spectra_Noise_Masked_Feb_" + to_string(iFeb);
        HistTitle = "GEM ADC Spectra with Noise Masking of Feb " + to_string(iFeb) + " for Run " + to_string(RunId);
        GEM_Febwise_ADC_Spectra_Noise_Masked[iFeb] = new TH1I(HistTag, HistTitle, 35, -1.5, 33.5);
        GEM_Febwise_ADC_Spectra_Noise_Masked[iFeb]->GetXaxis()->SetTitle("ADC");
        GEM_Febwise_ADC_Spectra_Noise_Masked[iFeb]->GetYaxis()->SetTitle("Counts");
    }

    HistTag = "GEM_ADC_Spectra_Noise_Masked_Febwise_2D";
    HistTitle = "GEM 2D ADC Spectra with Noise Masking for Run " + to_string(RunId);
    TH2F* GEM_ADC_Spectra_Noise_Masked_Febwise_2D = new TH2F(HistTag, HistTitle, 20, -1.5, 18.5, 35, -1.5, 33.5);
    GEM_ADC_Spectra_Noise_Masked_Febwise_2D->GetXaxis()->SetTitle("FEB Number");
    GEM_ADC_Spectra_Noise_Masked_Febwise_2D->GetYaxis()->SetTitle("ADC");

    TH2F* GEM_Channelwise_ADC_Spectra_Noise_Masked[18];

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTag = "GEM_Channelwise_ADC_Spectra_Noise_Masked_Feb_" + to_string(iFeb);
        HistTitle = "GEM ADC Spectra (Channelwise) with Noise Masking of Feb " + to_string(iFeb) + " for Run " + to_string(RunId);
        GEM_Channelwise_ADC_Spectra_Noise_Masked[iFeb] = new TH2F(HistTag, HistTitle, 131, -1.5, 129.5, 35, -1.5, 33.5);
        GEM_Channelwise_ADC_Spectra_Noise_Masked[iFeb]->GetXaxis()->SetTitle("Channel Number");
        GEM_Channelwise_ADC_Spectra_Noise_Masked[iFeb]->GetYaxis()->SetTitle("ADC");
    }

    HistTag = "GEM_ADC_Spectra_Noise_Only";
    HistTitle = "GEM ADC Spectra with Noise Only for Run " + to_string(RunId);
    TH1I* GEM_ADC_Spectra_Noise_Only = new TH1I(HistTag, HistTitle, 35, -1.5, 33.5);
    GEM_ADC_Spectra_Noise_Only->GetXaxis()->SetTitle("ADC");
    GEM_ADC_Spectra_Noise_Only->GetYaxis()->SetTitle("Counts");

    TH1I* GEM_Febwise_ADC_Spectra_Noise_Only[18];

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTag = "GEM_ADC_Spectra_Noise_Only_Feb_" + to_string(iFeb);
        HistTitle = "GEM ADC Spectra with Noise Only of Feb " + to_string(iFeb) + " for Run " + to_string(RunId);
        GEM_Febwise_ADC_Spectra_Noise_Only[iFeb] = new TH1I(HistTag, HistTitle, 35, -1.5, 33.5);
        GEM_Febwise_ADC_Spectra_Noise_Only[iFeb]->GetXaxis()->SetTitle("ADC");
        GEM_Febwise_ADC_Spectra_Noise_Only[iFeb]->GetYaxis()->SetTitle("Counts");
    }

    HistTag = "GEM_ADC_Spectra_Noise_Only_Febwise_2D";
    HistTitle = "GEM 2D ADC Spectra with Noise Only for Run " + to_string(RunId);
    TH2F* GEM_ADC_Spectra_Noise_Only_Febwise_2D = new TH2F(HistTag, HistTitle, 20, -1.5, 18.5, 35, -1.5, 33.5);
    GEM_ADC_Spectra_Noise_Only_Febwise_2D->GetXaxis()->SetTitle("FEB Number");
    GEM_ADC_Spectra_Noise_Only_Febwise_2D->GetYaxis()->SetTitle("ADC");

    TH2F* GEM_Channelwise_ADC_Spectra_Noise_Only[18];

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTag = "GEM_Channelwise_ADC_Spectra_Noise_Only_Feb_" + to_string(iFeb);
        HistTitle = "GEM ADC Spectra (Channelwise) with Noise Only of Feb " + to_string(iFeb) + " for Run " + to_string(RunId);
        GEM_Channelwise_ADC_Spectra_Noise_Only[iFeb] = new TH2F(HistTag, HistTitle, 131, -1.5, 129.5, 35, -1.5, 33.5);
        GEM_Channelwise_ADC_Spectra_Noise_Only[iFeb]->GetXaxis()->SetTitle("Channel Number");
        GEM_Channelwise_ADC_Spectra_Noise_Only[iFeb]->GetYaxis()->SetTitle("ADC");
    }

    HistTag = "GEM_2FOLD_Raw_ADC_Spectra";
    HistTitle = "GEM Raw ADC Spectra after 2 Fold Trigger for Run " + to_string(RunId);
    TH1I* GEM_2FOLD_Trigger_Raw_ADC_Spectra = new TH1I(HistTag, HistTitle, 35, -1.5, 33.5);
    GEM_2FOLD_Trigger_Raw_ADC_Spectra->GetXaxis()->SetTitle("ADC");
    GEM_2FOLD_Trigger_Raw_ADC_Spectra->GetYaxis()->SetTitle("Counts");

    TH1I* GEM_Febwise_2FOLD_Trigger_Raw_ADC_Spectra[18];

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTag = "GEM_2FOLD_Trigger_Raw_ADC_Spectra_Feb_" + to_string(iFeb);
        HistTitle = "GEM Raw ADC Spectra after 2 Fold Trigger of Feb " + to_string(iFeb) + " for Run " + to_string(RunId);
        GEM_Febwise_2FOLD_Trigger_Raw_ADC_Spectra[iFeb] = new TH1I(HistTag, HistTitle, 35, -1.5, 33.5);
        GEM_Febwise_2FOLD_Trigger_Raw_ADC_Spectra[iFeb]->GetXaxis()->SetTitle("ADC");
        GEM_Febwise_2FOLD_Trigger_Raw_ADC_Spectra[iFeb]->GetYaxis()->SetTitle("Counts");
    }

    HistTag = "GEM_2FOLD_Raw_ADC_Spectra_Febwise_2D";
    HistTitle = "GEM 2D Raw ADC Spectra after 2 Fold Trigger for Run " + to_string(RunId);
    TH2F* GEM_2FOLD_Raw_ADC_Spectra_Febwise_2D = new TH2F(HistTag, HistTitle, 20, -1.5, 18.5, 35, -1.5, 33.5);
    GEM_2FOLD_Raw_ADC_Spectra_Febwise_2D->GetXaxis()->SetTitle("FEB Number");
    GEM_2FOLD_Raw_ADC_Spectra_Febwise_2D->GetYaxis()->SetTitle("ADC");

    TH2F* GEM_Channelwise_2FOLD_Trigger_Raw_ADC_Spectra[18];

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTag = "GEM_2FOLD_Trigger_Channelwise_Raw_ADC_Spectra_Feb_" + to_string(iFeb);
        HistTitle = "GEM Raw ADC Spectra (Channelwise) after 2 Fold Trigger of Feb " + to_string(iFeb) + " for Run " + to_string(RunId);
        GEM_Channelwise_2FOLD_Trigger_Raw_ADC_Spectra[iFeb] = new TH2F(HistTag, HistTitle, 131, -1.5, 129.5, 35, -1.5, 33.5);
        GEM_Channelwise_2FOLD_Trigger_Raw_ADC_Spectra[iFeb]->GetXaxis()->SetTitle("Channel Number");
        GEM_Channelwise_2FOLD_Trigger_Raw_ADC_Spectra[iFeb]->GetYaxis()->SetTitle("ADC");
    }

    HistTag = "GEM_2FOLD_ADC_Spectra_Noise_Masked";
    HistTitle = "GEM ADC Spectra with Noise Masking after 2 Fold Trigger for Run " + to_string(RunId);
    TH1I* GEM_2FOLD_Trigger_ADC_Spectra_Noise_Masked = new TH1I(HistTag, HistTitle, 35, -1.5, 33.5);
    GEM_2FOLD_Trigger_ADC_Spectra_Noise_Masked->GetXaxis()->SetTitle("ADC");
    GEM_2FOLD_Trigger_ADC_Spectra_Noise_Masked->GetYaxis()->SetTitle("Counts");

    TH1I* GEM_Febwise_2FOLD_Trigger_ADC_Spectra_Noise_Masked[18];

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTag = "GEM_2FOLD_Trigger_ADC_Spectra_Noise_Masked_Feb_" + to_string(iFeb);
        HistTitle = "GEM ADC Spectra with Noise Masking after 2 Fold Trigger of Feb " + to_string(iFeb) + " for Run " + to_string(RunId);
        GEM_Febwise_2FOLD_Trigger_ADC_Spectra_Noise_Masked[iFeb] = new TH1I(HistTag, HistTitle, 35, -1.5, 33.5);
        GEM_Febwise_2FOLD_Trigger_ADC_Spectra_Noise_Masked[iFeb]->GetXaxis()->SetTitle("ADC");
        GEM_Febwise_2FOLD_Trigger_ADC_Spectra_Noise_Masked[iFeb]->GetYaxis()->SetTitle("Counts");
    }

    HistTag = "GEM_2FOLD_ADC_Spectra_Noise_Masked_Febwise_2D";
    HistTitle = "GEM ADC Spectra (Febwise) with Noise Masking after 2 Fold Trigger for Run " + to_string(RunId);
    TH2F* GEM_2FOLD_Trigger_ADC_Spectra_Noise_Masked_Febwise_2D = new TH2F(HistTag, HistTitle, 20, -1.5, 18.5, 35, -1.5, 33.5);
    GEM_2FOLD_Trigger_ADC_Spectra_Noise_Masked_Febwise_2D->GetXaxis()->SetTitle("FEB Number");
    GEM_2FOLD_Trigger_ADC_Spectra_Noise_Masked_Febwise_2D->GetYaxis()->SetTitle("ADC");

    TH2F* GEM_Channelwise_2FOLD_Trigger_ADC_Spectra_Noise_Masked[18];

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTag = "GEM_2FOLD_Trigger_Channelwise_ADC_Spectra_Noise_Masked_Feb_" + to_string(iFeb);
        HistTitle = "GEM ADC Spectra (Channelwise) with Noise Masking after 2 Fold Trigger of Feb " + to_string(iFeb) + " for Run " + to_string(RunId);
        GEM_Channelwise_2FOLD_Trigger_ADC_Spectra_Noise_Masked[iFeb] = new TH2F(HistTag, HistTitle, 131, -1.5, 129.5, 35, -1.5, 33.5);
        GEM_Channelwise_2FOLD_Trigger_ADC_Spectra_Noise_Masked[iFeb]->GetXaxis()->SetTitle("Channel");
        GEM_Channelwise_2FOLD_Trigger_ADC_Spectra_Noise_Masked[iFeb]->GetYaxis()->SetTitle("ADC");
    }

    HistTag = "GEM_2FOLD_Cluster_Charge_Spectra";
    HistTitle = "GEM Cluster Charge Spectra after 2 Fold Trigger for Run " + to_string(RunId);
    TH1I* GEM_2FOLD_Trigger_Cluster_Charge_Spectra = new TH1I(HistTag, HistTitle, 1000, -1.5, 998.5);
    GEM_2FOLD_Trigger_Cluster_Charge_Spectra->GetXaxis()->SetTitle("Cluster Charge (fC)");
    GEM_2FOLD_Trigger_Cluster_Charge_Spectra->GetYaxis()->SetTitle("Counts");

    HistTag = "GEM_2FOLD_Number_Of_Digis_Per_Cluster";
    HistTitle = "GEM Number of Digis Per Cluster after 2 Fold Trigger for Run " + to_string(RunId);
    TH1I* GEM_2FOLD_Number_Of_Digis_Per_Cluster = new TH1I(HistTag, HistTitle, 100, -1.5, 98.5);
    GEM_2FOLD_Number_Of_Digis_Per_Cluster->GetXaxis()->SetTitle("Number of Digis Per Cluster");
    GEM_2FOLD_Number_Of_Digis_Per_Cluster->GetYaxis()->SetTitle("Counts");

    HistTag = "GEM_3FOLD_Raw_ADC_Spectra";
    HistTitle = "GEM Raw ADC Spectra after 3 Fold Trigger for Run " + to_string(RunId);
    TH1I* GEM_3FOLD_Trigger_Raw_ADC_Spectra = new TH1I(HistTag, HistTitle, 35, -1.5, 33.5);
    GEM_3FOLD_Trigger_Raw_ADC_Spectra->GetXaxis()->SetTitle("ADC");
    GEM_3FOLD_Trigger_Raw_ADC_Spectra->GetYaxis()->SetTitle("Counts");

    TH1I* GEM_Febwise_3FOLD_Trigger_Raw_ADC_Spectra[18];

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTag = "GEM_3FOLD_Trigger_Raw_ADC_Spectra_Feb_" + to_string(iFeb);
        HistTitle = "GEM Raw ADC Spectra after 3 Fold Trigger of Feb " + to_string(iFeb) + " for Run " + to_string(RunId);
        GEM_Febwise_3FOLD_Trigger_Raw_ADC_Spectra[iFeb] = new TH1I(HistTag, HistTitle, 35, -1.5, 33.5);
        GEM_Febwise_3FOLD_Trigger_Raw_ADC_Spectra[iFeb]->GetXaxis()->SetTitle("ADC");
        GEM_Febwise_3FOLD_Trigger_Raw_ADC_Spectra[iFeb]->GetYaxis()->SetTitle("Counts");
    }

    HistTag = "GEM_3FOLD_Raw_ADC_Spectra_Febwise_2D";
    HistTitle = "GEM 2D Raw ADC Spectra after 3 Fold Trigger for Run " + to_string(RunId);
    TH2F* GEM_3FOLD_Raw_ADC_Spectra_Febwise_2D = new TH2F(HistTag, HistTitle, 20, -1.5, 18.5, 35, -1.5, 33.5);
    GEM_3FOLD_Raw_ADC_Spectra_Febwise_2D->GetXaxis()->SetTitle("FEB Number");
    GEM_3FOLD_Raw_ADC_Spectra_Febwise_2D->GetYaxis()->SetTitle("ADC");

    TH2F* GEM_Channelwise_3FOLD_Trigger_Raw_ADC_Spectra[18];

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTag = "GEM_3FOLD_Trigger_Channelwise_Raw_ADC_Spectra_Feb_" + to_string(iFeb);
        HistTitle = "GEM Raw ADC Spectra (Channelwise) after 3 Fold Trigger of Feb " + to_string(iFeb) + " for Run " + to_string(RunId);
        GEM_Channelwise_3FOLD_Trigger_Raw_ADC_Spectra[iFeb] = new TH2F(HistTag, HistTitle, 131, -1.5, 129.5, 35, -1.5, 33.5);
        GEM_Channelwise_3FOLD_Trigger_Raw_ADC_Spectra[iFeb]->GetXaxis()->SetTitle("Channel Number");
        GEM_Channelwise_3FOLD_Trigger_Raw_ADC_Spectra[iFeb]->GetYaxis()->SetTitle("ADC");
    }

    HistTag = "GEM_3FOLD_ADC_Spectra_Noise_Masked";
    HistTitle = "GEM ADC Spectra with Noise Masking after 3 Fold Trigger for Run " + to_string(RunId);
    TH1I* GEM_3FOLD_Trigger_ADC_Spectra_Noise_Masked = new TH1I(HistTag, HistTitle, 35, -1.5, 33.5);
    GEM_3FOLD_Trigger_ADC_Spectra_Noise_Masked->GetXaxis()->SetTitle("ADC");
    GEM_3FOLD_Trigger_ADC_Spectra_Noise_Masked->GetYaxis()->SetTitle("Counts");

    TH1I* GEM_Febwise_3FOLD_Trigger_ADC_Spectra_Noise_Masked[18];

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTag = "GEM_3FOLD_Trigger_ADC_Spectra_Noise_Masked_Feb_" + to_string(iFeb);
        HistTitle = "GEM ADC Spectra with Noise Masking after 3 Fold Trigger of Feb " + to_string(iFeb) + " for Run " + to_string(RunId);
        GEM_Febwise_3FOLD_Trigger_ADC_Spectra_Noise_Masked[iFeb] = new TH1I(HistTag, HistTitle, 35, -1.5, 33.5);
        GEM_Febwise_3FOLD_Trigger_ADC_Spectra_Noise_Masked[iFeb]->GetXaxis()->SetTitle("ADC");
        GEM_Febwise_3FOLD_Trigger_ADC_Spectra_Noise_Masked[iFeb]->GetYaxis()->SetTitle("Counts");
    }

    HistTag = "GEM_3FOLD_ADC_Spectra_Noise_Masked_Febwise_2D";
    HistTitle = "GEM ADC Spectra (Febwise) with Noise Masking after 3 Fold Trigger for Run " + to_string(RunId);
    TH2F* GEM_3FOLD_Trigger_ADC_Spectra_Noise_Masked_Febwise_2D = new TH2F(HistTag, HistTitle, 20, -1.5, 18.5, 35, -1.5, 33.5);
    GEM_3FOLD_Trigger_ADC_Spectra_Noise_Masked_Febwise_2D->GetXaxis()->SetTitle("FEB Number");
    GEM_3FOLD_Trigger_ADC_Spectra_Noise_Masked_Febwise_2D->GetYaxis()->SetTitle("ADC");

    TH2F* GEM_Channelwise_3FOLD_Trigger_ADC_Spectra_Noise_Masked[18];

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTag = "GEM_3FOLD_Trigger_Channelwise_ADC_Spectra_Noise_Masked_Feb_" + to_string(iFeb);
        HistTitle = "GEM ADC Spectra (Channelwise) with Noise Masking after 3 Fold Trigger of Feb " + to_string(iFeb) + " for Run " + to_string(RunId);
        GEM_Channelwise_3FOLD_Trigger_ADC_Spectra_Noise_Masked[iFeb] = new TH2F(HistTag, HistTitle, 131, -1.5, 129.5, 35, -1.5, 33.5);
        GEM_Channelwise_3FOLD_Trigger_ADC_Spectra_Noise_Masked[iFeb]->GetXaxis()->SetTitle("Channel");
        GEM_Channelwise_3FOLD_Trigger_ADC_Spectra_Noise_Masked[iFeb]->GetYaxis()->SetTitle("ADC");
    }

    HistTag = "GEM_3FOLD_Cluster_Charge_Spectra";
    HistTitle = "GEM Cluster Charge Spectra after 3 Fold Trigger for Run " + to_string(RunId);
    TH1I* GEM_3FOLD_Trigger_Cluster_Charge_Spectra = new TH1I(HistTag, HistTitle, 1000, -1.5, 998.5);
    GEM_3FOLD_Trigger_Cluster_Charge_Spectra->GetXaxis()->SetTitle("Cluster Charge (fC)");
    GEM_3FOLD_Trigger_Cluster_Charge_Spectra->GetYaxis()->SetTitle("Counts");

    HistTag = "GEM_3FOLD_Number_Of_Digis_Per_Cluster";
    HistTitle = "GEM Number of Digis Per Cluster after 3 Fold Trigger for Run " + to_string(RunId);
    TH1I* GEM_3FOLD_Number_Of_Digis_Per_Cluster = new TH1I(HistTag, HistTitle, 100, -1.5, 98.5);
    GEM_3FOLD_Number_Of_Digis_Per_Cluster->GetXaxis()->SetTitle("Number of Digis Per Cluster");
    GEM_3FOLD_Number_Of_Digis_Per_Cluster->GetYaxis()->SetTitle("Counts");

    Int_t Two_Fold_Lower_Bound = 0;
    Int_t Two_Fold_Upper_Bound = 64;
    Int_t Three_Fold_Lower_Bound = 64;
    Int_t Three_Fold_Upper_Bound = 128;

    cout << "Checking for 2F and 3F Channels" << endl;

    for (iTreeEntry = 0; iTreeEntry < nTimeSlices; iTreeEntry++)
    {
        Tree_Data_Reader.SetEntry(iTreeEntry);
        Int_t iTimeSlice = TimeSlice_Event_Header->GetTsIndex();

        // cout<<"Checking for 2F and 3F Channels:  iTreeEntry: "<<iTreeEntry<<"  TS Index: " << iTimeSlice <<endl;

        uint64_t TimeSlice_Start_Time = TimeSlice_Event_Header->GetTsStartTime();

        if (FirstDigiFlag == 0)
        {
            First_Digi_Time = TimeSlice_Start_Time;
            FirstDigiFlag = 1;
        }

        Int_t Zero_To_Sixty_Three = 0;
        Int_t Sixty_Four_To_One_Hundred_Twenty_Seven = 0;

        for (Int_t iMuchDigi = 0; iMuchDigi < Much_Digi.GetSize(); iMuchDigi++)
        {
            int32_t Station = CbmMuchAddress::GetStationIndex(Much_Digi[iMuchDigi].GetAddress());
            if (Station != 1)
                continue;
            int32_t Channel = CbmMuchAddress::GetChannelIndex(Much_Digi[iMuchDigi].GetAddress());
            if (Channel < 64)
                Zero_To_Sixty_Three++;
            else
                Sixty_Four_To_One_Hundred_Twenty_Seven++;
        }

        if (Zero_To_Sixty_Three > 100 && Sixty_Four_To_One_Hundred_Twenty_Seven > 100)
        {
            if (Zero_To_Sixty_Three < Sixty_Four_To_One_Hundred_Twenty_Seven)
            {
                Two_Fold_Lower_Bound = 64;
                Two_Fold_Upper_Bound = 128;
                Three_Fold_Lower_Bound = 0;
                Three_Fold_Upper_Bound = 64;
                cout << "Found! 0-63: 3F   and    64-127: 2F at Timeslice: " << iTreeEntry << endl;
                break;
            }
            else
            {
                Two_Fold_Lower_Bound = 0;
                Two_Fold_Upper_Bound = 64;
                Three_Fold_Lower_Bound = 64;
                Three_Fold_Upper_Bound = 128;
                cout << "Found! 0-63: 2F   and    64-127: 3F at Timeslice: " << iTreeEntry << endl;
                break;
            }
        }
    }

    // for(iTreeEntry = 85; iTreeEntry < 200 ;iTreeEntry++)
    for (iTreeEntry = 0; iTreeEntry < nTimeSlices; iTreeEntry++)
    {
        Tree_Data_Reader.SetEntry(iTreeEntry);
        uint64_t TimeSlice_Start_Time = TimeSlice_Event_Header->GetTsStartTime();

        Int_t iTimeSlice = TimeSlice_Event_Header->GetTsIndex();

        if (RunId == 693 && iTimeSlice == 1091)
            continue; // These Particular Runs have transient noise
        if (RunId == 699)
        {
            if (iTimeSlice > 1290 && iTimeSlice < 1298)
                continue; // These Particular Runs have transient noise
            if (iTimeSlice > 1851 && iTimeSlice < 1857)
                continue; // These Particular Runs have transient noise
        }

        // if(iTreeEntry%100 == 0)
        cout << "iTreeEntry: " << iTreeEntry << "  TS Index: " << iTimeSlice << endl;

        Int_t GEM_Digis = 0;
        Int_t iMuchStartDigi = 0;

        Bool_t Trigger_Flag = false;

        iMuchStartDigi = 0;
        Int_t Threefold_hits = 0;
        Int_t Fourfold_hits = 0;

        Float_t Cluster_Charge = 0;
        Int_t nDigis_Per_Trigger = 0;

        std::vector<Int_t> Much2F(Much_Digi.GetSize());
        std::vector<Int_t> Much3F(Much_Digi.GetSize());

        for (Int_t iDigi = 0; iDigi < Much_Digi.GetSize(); iDigi++)
        {
            Much2F.push_back(0);
            Much3F.push_back(0);
        }

        for (Int_t iMuchDigi = 0; iMuchDigi < Much_Digi.GetSize(); iMuchDigi++, Trigger_Flag = false)
        // for(Int_t iMuchDigi=iMuchStartDigi;iMuchDigi1 < Much_Digi.GetSize();iMuchDigi1++,TriggerFlag=false)
        {
            int32_t Station = CbmMuchAddress::GetStationIndex(Much_Digi[iMuchDigi].GetAddress());

            if (Station == 0)
            {
                int32_t Channel = CbmMuchAddress::GetChannelIndex(Much_Digi[iMuchDigi].GetAddress());
                int32_t Sector = CbmMuchAddress::GetSectorIndex(Much_Digi[iMuchDigi].GetAddress());
                if (Channel > 22 || Sector > 96)
                    continue;
                Int_t FEB_GEM = GEM_Feb_no_X_Y[Channel][Sector];
                Int_t FEB_CH = GEM_Ch_no_X_Y[Channel][Sector];
                Int_t ADC_Value = Much_Digi[iMuchDigi].GetAdc();

                if (FEB_GEM == 4 || FEB_GEM == 9 || FEB_GEM == 10 || FEB_GEM == 13)
                {
                    if (FEB_CH % 2 == 0)
                    {
                        FEB_CH = (FEB_CH + 1);
                        Sector = GEM_Coordinate_Feb_Channel[FEB_GEM][FEB_CH][1];
                        Channel = GEM_Coordinate_Feb_Channel[FEB_GEM][FEB_CH][0];
                    }
                    else
                    {
                        FEB_CH = (FEB_CH - 1);
                        Sector = GEM_Coordinate_Feb_Channel[FEB_GEM][FEB_CH][1];
                        Channel = GEM_Coordinate_Feb_Channel[FEB_GEM][FEB_CH][0];
                    }
                }

                if (Sector < 0)
                    continue;
                if (Channel < 0)
                    continue;
                if (Channel > 22 || Sector > 96)
                    continue;

                GEM_Raw_ADC_Spectra->Fill(ADC_Value);
                GEM_Febwise_Raw_ADC_Spectra[FEB_GEM]->Fill(ADC_Value);
                GEM_Raw_ADC_Spectra_Febwise_2D->Fill(FEB_GEM, ADC_Value);
                GEM_Channelwise_Raw_ADC_Spectra[FEB_GEM]->Fill(FEB_CH, ADC_Value);

                if (GEM_Noise[CbmMuchAddress::GetChannelIndex(Much_Digi[iMuchDigi].GetAddress())][CbmMuchAddress::GetChannelIndex(Much_Digi[iMuchDigi].GetAddress())] != -1)
                {
                    GEM_ADC_Spectra_Noise_Masked->Fill(ADC_Value);
                    GEM_Febwise_ADC_Spectra_Noise_Masked[FEB_GEM]->Fill(ADC_Value);
                    GEM_ADC_Spectra_Noise_Masked_Febwise_2D->Fill(FEB_GEM, ADC_Value);
                    GEM_Channelwise_ADC_Spectra_Noise_Masked[FEB_GEM]->Fill(FEB_CH, ADC_Value);
                }
                else
                {
                    GEM_ADC_Spectra_Noise_Only->Fill(ADC_Value);
                    GEM_Febwise_ADC_Spectra_Noise_Only[FEB_GEM]->Fill(ADC_Value);
                    GEM_ADC_Spectra_Noise_Only_Febwise_2D->Fill(FEB_GEM, ADC_Value);
                    GEM_Channelwise_ADC_Spectra_Noise_Only[FEB_GEM]->Fill(FEB_CH, ADC_Value);
                }
            }

            if (Station != 1)
                continue; // Select only Trigger

            int32_t Channel = CbmMuchAddress::GetChannelIndex(Much_Digi[iMuchDigi].GetAddress());
            if (Channel != 25 && Channel != 125)
                continue; // Single Channel from either 2 Fold or 3 Fold
            // if(Channel!=125) continue; // 3-FOLD Coincidence only
            // if(Channel>=64) continue; // 2-FOLD Coincidence only

            Int_t Trigger_Digi = 0;
            Bool_t Trigger_Flag = false;
            Int_t iLastDigi = iMuchStartDigi;
            Int_t Tolerance = (Int_t)TMath::Ceil(Much_Digi.GetSize() / (1280.0)); // 1 Ts = ~128ms --> 1/(1280) = ~100 #mus

            Int_t iMuchStartDigi = iMuchDigi - Tolerance;
            if (iMuchStartDigi < 0)
                iMuchStartDigi = 0;

            Int_t iMuchLastDigi = iMuchDigi + Tolerance;
            if (iMuchLastDigi > Much_Digi.GetSize() - 1)
                iMuchLastDigi = Much_Digi.GetSize() - 1;

            for (Int_t iMuchDigi1 = iMuchStartDigi; iMuchDigi1 < iMuchLastDigi; iMuchDigi1++)
            {
                if (iMuchDigi == iMuchDigi1)
                    continue;
                Cluster_Charge = 0;

                // if(Much2F[iMuchDigi1]==1) continue;
                // if(Much3F[iMuchDigi1]==1) continue;

                int32_t Station1 = CbmMuchAddress::GetStationIndex(Much_Digi[iMuchDigi1].GetAddress());
                if (Station1 != 0)
                    continue;

                Double_t First_Time_difference = ((Double_t)Much_Digi[iMuchDigi1].GetTime() - (Double_t)Much_Digi[0].GetTime()) / 1000.0;

                //cout<<"Sector: "<<Sector1<<"  Channel: "<<Channel1<<"  ADC: "<<Much_Digi[iMuchDigi].GetAdc()<<" Timestamp: "<<(uint64_t)Much_Digi[iMuchDigi1].GetTime()<<endl;

                Double_t Time_difference = ((Double_t)Much_Digi[iMuchDigi].GetTime() - (Double_t)Much_Digi[iMuchDigi1].GetTime()) / 1000.0;

                if (First_Time_difference > 0.0)
                {
                    // cout<<Time_difference<<endl;

                    if (Time_difference > Coincidence_Window_Upper_Bound_mus)
                    {
                        continue;
                    }
                    else if (Time_difference < Coincidence_Window_Lower_Bound_mus)
                    {
                        break;
                    }
                    else
                    {
                        int32_t Sector1 = CbmMuchAddress::GetSectorIndex(Much_Digi[iMuchDigi1].GetAddress());
                        int32_t Channel1 = CbmMuchAddress::GetChannelIndex(Much_Digi[iMuchDigi1].GetAddress());

                        //cout<<"StepA"<<endl;

                        if (Sector1 > 96 || Channel1 > 22)
                            continue;
                        Int_t FEB_GEM = GEM_Feb_no_X_Y[Channel1][Sector1];
                        Int_t FEB_CH = GEM_Ch_no_X_Y[Channel1][Sector1];
                        Int_t ADC_Value = Much_Digi[iMuchDigi1].GetAdc();


                        if (FEB_GEM == 4 || FEB_GEM == 9 || FEB_GEM == 10 || FEB_GEM == 13)
                        {

                            if (FEB_CH % 2 == 0)
                            {
                                FEB_CH = (FEB_CH + 1);
                                Sector1 = GEM_Coordinate_Feb_Channel[FEB_GEM][FEB_CH][1];
                                Channel1 = GEM_Coordinate_Feb_Channel[FEB_GEM][FEB_CH][0];
                            }
                            else
                            {
                                FEB_CH = (FEB_CH - 1);
                                Sector1 = GEM_Coordinate_Feb_Channel[FEB_GEM][FEB_CH][1];
                                Channel1 = GEM_Coordinate_Feb_Channel[FEB_GEM][FEB_CH][0];
                            }
                        }

                        if (Sector1 < 0)
                            continue;
                        if (Channel1 < 0)
                            continue;
                        //if(Sector1>40) continue;
                        //cout<<"StepB"<<endl;

                        // if(GEM_Noise[Channel1][Sector1]==-1) continue;
                        //cout<<iMuchDigi1<<"  Sector: "<<Sector1<<"  Channel: "<<Channel1<<"  ADC: "<<Much_Digi[iMuchDigi1].GetAdc()<<" Timestamp: "<<(uint64_t)Much_Digi[iMuchDigi1].GetTime()<<endl;
                        // cout<<iMuchDigi<<"  FEB: "<<FEB_GEM<<"  Channel: "<<FEB_CH<<"  ADC: "<<Much_Digi[iMuchDigi].GetAdc()<<" Timestamp: "<<(uint64_t)Much_Digi[iMuchDigi1].GetTime()<<endl;

                        if (Channel >= Two_Fold_Lower_Bound && Channel < Two_Fold_Upper_Bound)
                        {
                            GEM_2FOLD_Trigger_Raw_ADC_Spectra->Fill(ADC_Value);
                            GEM_Febwise_2FOLD_Trigger_Raw_ADC_Spectra[FEB_GEM]->Fill(ADC_Value);
                            GEM_2FOLD_Raw_ADC_Spectra_Febwise_2D->Fill(FEB_GEM, ADC_Value);
                            GEM_Channelwise_2FOLD_Trigger_Raw_ADC_Spectra[FEB_GEM]->Fill(FEB_CH, ADC_Value);

                            if (GEM_Noise[CbmMuchAddress::GetChannelIndex(Much_Digi[iMuchDigi1].GetAddress())][CbmMuchAddress::GetSectorIndex(Much_Digi[iMuchDigi1].GetAddress())] != -1)
                            {
                                GEM_2FOLD_Trigger_ADC_Spectra_Noise_Masked->Fill(ADC_Value);
                                GEM_Febwise_2FOLD_Trigger_ADC_Spectra_Noise_Masked[FEB_GEM]->Fill(ADC_Value);
                                GEM_2FOLD_Trigger_ADC_Spectra_Noise_Masked_Febwise_2D->Fill(FEB_GEM, ADC_Value);
                                GEM_Channelwise_2FOLD_Trigger_ADC_Spectra_Noise_Masked[FEB_GEM]->Fill(FEB_CH, ADC_Value);
                            }

                            if (Much2F[iMuchDigi1] == 1)
                                continue;
                            if (GEM_Noise[CbmMuchAddress::GetChannelIndex(Much_Digi[iMuchDigi1].GetAddress())][CbmMuchAddress::GetSectorIndex(Much_Digi[iMuchDigi1].GetAddress())] == -1)
                                continue;
                            // if(FEB_GEM==14 && ADC_Value<6) continue;  // Low ADC Noise injection due to ringing in certain febs
                            // if(FEB_GEM==9 && ADC_Value<10) continue;
                            // if(FEB_GEM==10 && ADC_Value<4) continue;
                            // if(FEB_GEM==13 && ADC_Value<4) continue;

                            //cout<<"Step C"<<endl;

                            if (GEM_Noise[CbmMuchAddress::GetChannelIndex(Much_Digi[iMuchDigi1].GetAddress())][CbmMuchAddress::GetSectorIndex(Much_Digi[iMuchDigi1].GetAddress())] != -1) // Redundant step but kept to save time during quality checks
                            {
                                //cout<<iMuchDigi1<<"  Sector: "<<Sector1<<"  Channel: "<<Channel1<<"  ADC: "<<Much_Digi[iMuchDigi1].GetAdc()<<endl;
                                Cluster_Charge += 6.0 + 2.5 * Much_Digi[iMuchDigi1].GetAdc();
                                nDigis_Per_Trigger++;
                            }

                            Much2F[iMuchDigi1] = 1;

                            for (Int_t iMuchDigi2 = iMuchDigi1 + 1; iMuchDigi2 < Much_Digi.GetSize(); iMuchDigi2++)
                            {
                                if (((Double_t)Much_Digi[iMuchDigi].GetTime() - (Double_t)Much_Digi[iMuchDigi2].GetTime()) / 1000.0 < Coincidence_Window_Lower_Bound_mus)
                                    break;

                                //cout<<"StepD"<<endl;
                                int32_t Station2 = CbmMuchAddress::GetStationIndex(Much_Digi[iMuchDigi2].GetAddress());
                                if (Station2 != 0)
                                    continue;
                                int32_t Sector2 = CbmMuchAddress::GetSectorIndex(Much_Digi[iMuchDigi2].GetAddress());
                                int32_t Channel2 = CbmMuchAddress::GetChannelIndex(Much_Digi[iMuchDigi2].GetAddress());

                                if (Sector2 < 0)
                                    continue;
                                if (Channel2 < 0)
                                    continue;

                                if (Sector2 > 96 || Channel2 > 22)
                                {
                                    Much2F[iMuchDigi2] = 1;
                                    continue;
                                }

                                Int_t FEB_GEM2 = GEM_Feb_no_X_Y[Channel2][Sector2];
                                Int_t FEB_CH2 = GEM_Ch_no_X_Y[Channel2][Sector2];


                                if (FEB_GEM2 == 4 || FEB_GEM2 == 9 || FEB_GEM2 == 10 || FEB_GEM2 == 13)
                                {
                                    if (FEB_CH2 % 2 == 0)
                                    {
                                        FEB_CH2 = (FEB_CH2 + 1);
                                        Sector2 = GEM_Coordinate_Feb_Channel[FEB_GEM2][FEB_CH2][1];
                                        Channel2 = GEM_Coordinate_Feb_Channel[FEB_GEM2][FEB_CH2][0];
                                    }
                                    else
                                    {
                                        FEB_CH2 = (FEB_CH2 - 1);
                                        Sector2 = GEM_Coordinate_Feb_Channel[FEB_GEM2][FEB_CH2][1];
                                        Channel2 = GEM_Coordinate_Feb_Channel[FEB_GEM2][FEB_CH2][0];
                                    }
                                }

                                if (Sector2 < 0)
                                    continue;
                                if (Channel2 < 0)
                                    continue;

                                //cout<<"StepE"<<endl;

                                if (GEM_Noise[CbmMuchAddress::GetChannelIndex(Much_Digi[iMuchDigi2].GetAddress())][CbmMuchAddress::GetSectorIndex(Much_Digi[iMuchDigi2].GetAddress())] == -1)
                                {
                                    Much2F[iMuchDigi2] = 1;
                                    continue;
                                }

                                if (Much2F[iMuchDigi2] == 1)
                                    continue;

                                //cout<<iMuchDigi2<<"  Sector: "<<Sector2<<"  Channel: "<<Channel2<<"  ADC: "<<Much_Digi[iMuchDigi2].GetAdc()<<endl;

                                //cout<<"Step3"<<endl;

                                if (GEM_Noise[CbmMuchAddress::GetChannelIndex(Much_Digi[iMuchDigi2].GetAddress())][CbmMuchAddress::GetSectorIndex(Much_Digi[iMuchDigi2].GetAddress())] != -1 && TMath::Sqrt((Sector2 - Sector1) * (Sector2 - Sector1) + (Channel2 - Channel1) * (Channel2 - Channel1)) < 2.85) // sqrt(8) = 2.8282 which is max distance 2 pads diagonally
                                {
                                    Cluster_Charge += 6.0 + 2.5 * Much_Digi[iMuchDigi2].GetAdc();
                                    nDigis_Per_Trigger++;
                                    Much2F[iMuchDigi2] = 1;
                                }
                            }

                            //cout<<"Step4"<<endl;

                            if (Cluster_Charge != 0)
                            {
                                GEM_2FOLD_Trigger_Cluster_Charge_Spectra->Fill(Cluster_Charge);
                                GEM_2FOLD_Number_Of_Digis_Per_Cluster->Fill(nDigis_Per_Trigger);
                            }

                            Cluster_Charge = 0;
                            nDigis_Per_Trigger = 0;
                        }
                        else if (Channel >= Three_Fold_Lower_Bound && Channel < Three_Fold_Upper_Bound)
                        {
                            GEM_3FOLD_Trigger_Raw_ADC_Spectra->Fill(ADC_Value);
                            GEM_Febwise_3FOLD_Trigger_Raw_ADC_Spectra[FEB_GEM]->Fill(ADC_Value);
                            GEM_3FOLD_Raw_ADC_Spectra_Febwise_2D->Fill(FEB_GEM, ADC_Value);
                            GEM_Channelwise_3FOLD_Trigger_Raw_ADC_Spectra[FEB_GEM]->Fill(FEB_CH, ADC_Value);

                            //cout<<"Step5"<<endl;

                            if (GEM_Noise[CbmMuchAddress::GetChannelIndex(Much_Digi[iMuchDigi1].GetAddress())][CbmMuchAddress::GetSectorIndex(Much_Digi[iMuchDigi1].GetAddress())] != -1)
                            {
                                GEM_3FOLD_Trigger_ADC_Spectra_Noise_Masked->Fill(ADC_Value);
                                GEM_Febwise_3FOLD_Trigger_ADC_Spectra_Noise_Masked[FEB_GEM]->Fill(ADC_Value);
                                GEM_3FOLD_Trigger_ADC_Spectra_Noise_Masked_Febwise_2D->Fill(FEB_GEM, ADC_Value);
                                GEM_Channelwise_3FOLD_Trigger_ADC_Spectra_Noise_Masked[FEB_GEM]->Fill(FEB_CH, ADC_Value);
                            }

                            if (Much3F[iMuchDigi1] == 1)
                                continue;
                            if (GEM_Noise[CbmMuchAddress::GetChannelIndex(Much_Digi[iMuchDigi1].GetAddress())][CbmMuchAddress::GetSectorIndex(Much_Digi[iMuchDigi1].GetAddress())] == -1)
                                continue;
                            if (Three_Fold_Area[Channel1][Sector1] == 0)
                                continue;
                            // if(FEB_GEM==14 && ADC_Value<6 ) continue;  // Low ADC Noise injection in certain FEBs due to ringing effect
                            // if(FEB_GEM==13 && ADC_Value<4 ) continue;
                            // if(FEB_GEM==10 && ADC_Value<4 ) continue;
                            // if(FEB_GEM==9  && ADC_Value<10) continue;

                            //cout<<"Step6"<<endl;

                            if (GEM_Noise[CbmMuchAddress::GetChannelIndex(Much_Digi[iMuchDigi1].GetAddress())][CbmMuchAddress::GetSectorIndex(Much_Digi[iMuchDigi1].GetAddress())] != -1 && Three_Fold_Area[Channel1][Sector1] == 1) // Redundancy kept on purpose
                            {
                                //cout<<iMuchDigi1<<"  Sector: "<<Sector1<<"  Channel: "<<Channel1<<"  ADC: "<<Much_Digi[iMuchDigi1].GetAdc()<<endl;
                                Cluster_Charge += 6.0 + 2.5 * Much_Digi[iMuchDigi1].GetAdc();
                                nDigis_Per_Trigger++;
                            }

                            Much3F[iMuchDigi1] = 1;

                            //cout<<"Step7"<<endl;

                            for (Int_t iMuchDigi2 = iMuchDigi1 + 1; iMuchDigi2 < Much_Digi.GetSize(); iMuchDigi2++)
                            {
                                if (((Double_t)Much_Digi[iMuchDigi].GetTime() - (Double_t)Much_Digi[iMuchDigi2].GetTime()) / 1000.0 < Coincidence_Window_Lower_Bound_mus)
                                    break;
                                int32_t Station2 = CbmMuchAddress::GetStationIndex(Much_Digi[iMuchDigi2].GetAddress());
                                if (Station2 != 0)
                                    continue;
                                int32_t Sector2 = CbmMuchAddress::GetSectorIndex(Much_Digi[iMuchDigi2].GetAddress());
                                int32_t Channel2 = CbmMuchAddress::GetChannelIndex(Much_Digi[iMuchDigi2].GetAddress());

                                if (Sector2 < 0)
                                    continue;
                                if (Channel2 < 0)
                                    continue;

                                if (Sector2 > 96 || Channel2 > 22)
                                {
                                    Much3F[iMuchDigi2] = 1;
                                    continue;
                                }

                                //cout<<"Step8"<<endl;
                                Int_t FEB_GEM2 = GEM_Feb_no_X_Y[Channel2][Sector2];
                                Int_t FEB_CH2 = GEM_Ch_no_X_Y[Channel2][Sector2];

                                if (FEB_GEM2 == 4 || FEB_GEM2 == 9 || FEB_GEM2 == 10 || FEB_GEM2 == 13)
                                {
                                    if (FEB_CH2 % 2 == 0)
                                    {
                                        FEB_CH2 = (FEB_CH2 + 1);
                                        Sector2 = GEM_Coordinate_Feb_Channel[FEB_GEM2][FEB_CH2][1];
                                        Channel2 = GEM_Coordinate_Feb_Channel[FEB_GEM2][FEB_CH2][0];
                                    }
                                    else
                                    {
                                        FEB_CH2 = (FEB_CH2 - 1);
                                        Sector2 = GEM_Coordinate_Feb_Channel[FEB_GEM2][FEB_CH2][1];
                                        Channel2 = GEM_Coordinate_Feb_Channel[FEB_GEM2][FEB_CH2][0];
                                    }
                                }

                                //cout<<iMuchDigi2<<"  Sector: "<<Sector2<<"  Channel: "<<Channel2<<"  ADC: "<<Much_Digi[iMuchDigi2].GetAdc()<<endl;

                                if (Sector2 < 0)
                                    continue;
                                if (Channel2 < 0)
                                    continue;

                                if (Three_Fold_Area[Channel2][Sector2] == 0 || GEM_Noise[CbmMuchAddress::GetChannelIndex(Much_Digi[iMuchDigi2].GetAddress())][CbmMuchAddress::GetSectorIndex(Much_Digi[iMuchDigi2].GetAddress())] == -1)
                                {
                                    Much3F[iMuchDigi2] = 1;
                                    continue;
                                }

                                if (Much3F[iMuchDigi2] == 1)
                                    continue;

                                //cout<<"Step1"<<endl;
                                if (GEM_Noise[CbmMuchAddress::GetChannelIndex(Much_Digi[iMuchDigi2].GetAddress())][CbmMuchAddress::GetSectorIndex(Much_Digi[iMuchDigi2].GetAddress())] != -1 && Three_Fold_Area[Channel2][Sector2] == 1 && TMath::Sqrt((Sector2 - Sector1) * (Sector2 - Sector1) + (Channel2 - Channel1) * (Channel2 - Channel1)) < 2.85) // sqrt(8) = 2.8282 which is max distance 2 pads diagonally
                                {
                                    //cout<<"  "<<iMuchDigi2<<"  Time diff: "<< ((Double_t)Much_Digi[iMuchDigi2].GetTime()-(Double_t)Much_Digi[iMuchDigi1].GetTime() )/ 1000.0 <<"  Sector: "<<Sector2<<"  Channel: "<<Channel2<<"  ADC: "<<Much_Digi[iMuchDigi2].GetAdc()<<endl;
                                    Cluster_Charge += 6.0 + 2.5 * Much_Digi[iMuchDigi2].GetAdc();
                                    nDigis_Per_Trigger++;
                                    Much3F[iMuchDigi2] = 1;
                                }

                                //cout<<"Step2"<<endl;
                            }

                            if (Cluster_Charge != 0)
                            {
                                GEM_3FOLD_Trigger_Cluster_Charge_Spectra->Fill(Cluster_Charge);
                                GEM_3FOLD_Number_Of_Digis_Per_Cluster->Fill(nDigis_Per_Trigger);
                            }

                            Cluster_Charge = 0;
                            nDigis_Per_Trigger = 0;
                        }
                    }
                }
                else
                {
                    if (Time_difference < 0.0)
                        break;
                }
            }
        }

        Much2F.clear();
        Much3F.clear();
    }

    fOut->mkdir("ADC Related Plots");
    fOut->mkdir("ADC Related Plots/RAW");
    fOut->mkdir("ADC Related Plots/RAW/GEM");
    fOut->mkdir("ADC Related Plots/RAW/GEM/No Trigger");
    fOut->mkdir("ADC Related Plots/RAW/GEM/Two Fold");
    fOut->mkdir("ADC Related Plots/RAW/GEM/Three Fold");
    fOut->mkdir("ADC Related Plots/RAW/Febwise");
    fOut->mkdir("ADC Related Plots/RAW/Febwise/No Trigger");
    fOut->mkdir("ADC Related Plots/RAW/Febwise/No Trigger/1D");
    fOut->mkdir("ADC Related Plots/RAW/Febwise/No Trigger/2D");
    fOut->mkdir("ADC Related Plots/RAW/Febwise/No Trigger/Channelwise");
    fOut->mkdir("ADC Related Plots/RAW/Febwise/Two Fold");
    fOut->mkdir("ADC Related Plots/RAW/Febwise/Two Fold/1D");
    fOut->mkdir("ADC Related Plots/RAW/Febwise/Two Fold/2D");
    fOut->mkdir("ADC Related Plots/RAW/Febwise/Two Fold/Channelwise");
    fOut->mkdir("ADC Related Plots/RAW/Febwise/Three Fold");
    fOut->mkdir("ADC Related Plots/RAW/Febwise/Three Fold/1D");
    fOut->mkdir("ADC Related Plots/RAW/Febwise/Three Fold/2D");
    fOut->mkdir("ADC Related Plots/RAW/Febwise/Three Fold/Channelwise");
    fOut->mkdir("ADC Related Plots/Noise Masked");
    fOut->mkdir("ADC Related Plots/Noise Masked/GEM");
    fOut->mkdir("ADC Related Plots/Noise Masked/GEM/No Trigger");
    fOut->mkdir("ADC Related Plots/Noise Masked/GEM/Two Fold");
    fOut->mkdir("ADC Related Plots/Noise Masked/GEM/Three Fold");
    fOut->mkdir("ADC Related Plots/Noise Masked/Febwise");
    fOut->mkdir("ADC Related Plots/Noise Masked/Febwise/No Trigger");
    fOut->mkdir("ADC Related Plots/Noise Masked/Febwise/No Trigger/1D");
    fOut->mkdir("ADC Related Plots/Noise Masked/Febwise/No Trigger/2D");
    fOut->mkdir("ADC Related Plots/Noise Masked/Febwise/No Trigger/Channelwise");
    fOut->mkdir("ADC Related Plots/Noise Masked/Febwise/Two Fold");
    fOut->mkdir("ADC Related Plots/Noise Masked/Febwise/Two Fold/1D");
    fOut->mkdir("ADC Related Plots/Noise Masked/Febwise/Two Fold/2D");
    fOut->mkdir("ADC Related Plots/Noise Masked/Febwise/Two Fold/Channelwise");
    fOut->mkdir("ADC Related Plots/Noise Masked/Febwise/Three Fold");
    fOut->mkdir("ADC Related Plots/Noise Masked/Febwise/Three Fold/1D");
    fOut->mkdir("ADC Related Plots/Noise Masked/Febwise/Three Fold/2D");
    fOut->mkdir("ADC Related Plots/Noise Masked/Febwise/Three Fold/Channelwise");
    fOut->mkdir("ADC Related Plots/Noise Only");
    fOut->mkdir("ADC Related Plots/Noise Only/GEM");
    fOut->mkdir("ADC Related Plots/Noise Only/Febwise");
    fOut->mkdir("ADC Related Plots/Noise Only/Febwise/1D");
    fOut->mkdir("ADC Related Plots/Noise Only/Febwise/2D");
    fOut->mkdir("ADC Related Plots/Noise Only/Febwise/Channelwise");
    fOut->mkdir("ADC Related Plots/Cluster Charge");
    fOut->mkdir("ADC Related Plots/Cluster Charge/Two Fold");
    fOut->mkdir("ADC Related Plots/Cluster Charge/Three Fold");

    fOut->cd("ADC Related Plots/RAW/GEM/No Trigger");
    GEM_Raw_ADC_Spectra->Write();

    fOut->cd("ADC Related Plots/RAW/Febwise/No Trigger/1D");
    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
        GEM_Febwise_Raw_ADC_Spectra[iFeb]->Write();

    fOut->cd("ADC Related Plots/RAW/Febwise/No Trigger/2D");
    GEM_Raw_ADC_Spectra_Febwise_2D->Write();

    fOut->cd("ADC Related Plots/RAW/Febwise/No Trigger/Channelwise");
    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
        GEM_Channelwise_Raw_ADC_Spectra[iFeb]->Write();

    fOut->cd("ADC Related Plots/Noise Masked/GEM/No Trigger");
    GEM_ADC_Spectra_Noise_Masked->Write();

    fOut->cd("ADC Related Plots/Noise Masked/Febwise/No Trigger/1D");
    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
        GEM_Febwise_ADC_Spectra_Noise_Masked[iFeb]->Write();

    fOut->cd("ADC Related Plots/Noise Masked/Febwise/No Trigger/2D");
    GEM_ADC_Spectra_Noise_Masked_Febwise_2D->Write();

    fOut->cd("ADC Related Plots/Noise Masked/Febwise/No Trigger/Channelwise");
    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
        GEM_Channelwise_ADC_Spectra_Noise_Masked[iFeb]->Write();

    fOut->cd("ADC Related Plots/Noise Only/GEM");
    GEM_ADC_Spectra_Noise_Only->Write();

    fOut->cd("ADC Related Plots/Noise Only/Febwise/1D");
    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
        GEM_Febwise_ADC_Spectra_Noise_Only[iFeb]->Write();

    fOut->cd("ADC Related Plots/Noise Only/Febwise/2D");
    GEM_ADC_Spectra_Noise_Only_Febwise_2D->Write();

    fOut->cd("ADC Related Plots/Noise Only/Febwise/Channelwise");
    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
        GEM_Channelwise_ADC_Spectra_Noise_Only[iFeb]->Write();

    fOut->cd("ADC Related Plots/RAW/GEM/Two Fold");
    GEM_2FOLD_Trigger_Raw_ADC_Spectra->Write();

    fOut->cd("ADC Related Plots/RAW/Febwise/Two Fold/1D");
    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
        GEM_Febwise_2FOLD_Trigger_Raw_ADC_Spectra[iFeb]->Write();

    fOut->cd("ADC Related Plots/RAW/Febwise/Two Fold/2D");
    GEM_2FOLD_Raw_ADC_Spectra_Febwise_2D->Write();

    fOut->cd("ADC Related Plots/RAW/Febwise/Two Fold/Channelwise");
    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
        GEM_Channelwise_2FOLD_Trigger_Raw_ADC_Spectra[iFeb]->Write();

    fOut->cd("ADC Related Plots/Noise Masked/GEM/Two Fold");
    GEM_2FOLD_Trigger_ADC_Spectra_Noise_Masked->Write();

    fOut->cd("ADC Related Plots/Noise Masked/Febwise/Two Fold/1D");
    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
        GEM_Febwise_2FOLD_Trigger_ADC_Spectra_Noise_Masked[iFeb]->Write();

    fOut->cd("ADC Related Plots/Noise Masked/Febwise/Two Fold/2D");
    GEM_2FOLD_Trigger_ADC_Spectra_Noise_Masked_Febwise_2D->Write();

    fOut->cd("ADC Related Plots/Noise Masked/Febwise/Two Fold/Channelwise");
    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
        GEM_Channelwise_2FOLD_Trigger_ADC_Spectra_Noise_Masked[iFeb]->Write();

    fOut->cd("ADC Related Plots/Cluster Charge/Two Fold");
    GEM_2FOLD_Trigger_Cluster_Charge_Spectra->Write();
    GEM_2FOLD_Number_Of_Digis_Per_Cluster->Write();

    fOut->cd("ADC Related Plots/RAW/GEM/Three Fold");
    GEM_3FOLD_Trigger_Raw_ADC_Spectra->Write();

    fOut->cd("ADC Related Plots/RAW/Febwise/Three Fold/1D");
    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
        GEM_Febwise_3FOLD_Trigger_Raw_ADC_Spectra[iFeb]->Write();

    fOut->cd("ADC Related Plots/RAW/Febwise/Three Fold/2D");
    GEM_3FOLD_Raw_ADC_Spectra_Febwise_2D->Write();

    fOut->cd("ADC Related Plots/RAW/Febwise/Three Fold/Channelwise");
    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
        GEM_Channelwise_3FOLD_Trigger_Raw_ADC_Spectra[iFeb]->Write();

    fOut->cd("ADC Related Plots/Noise Masked/GEM/Three Fold");
    GEM_3FOLD_Trigger_ADC_Spectra_Noise_Masked->Write();

    fOut->cd("ADC Related Plots/Noise Masked/Febwise/Three Fold/1D");
    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
        GEM_Febwise_3FOLD_Trigger_ADC_Spectra_Noise_Masked[iFeb]->Write();

    fOut->cd("ADC Related Plots/Noise Masked/Febwise/Three Fold/2D");
    GEM_3FOLD_Trigger_ADC_Spectra_Noise_Masked_Febwise_2D->Write();

    fOut->cd("ADC Related Plots/Noise Masked/Febwise/Three Fold/Channelwise");
    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
        GEM_Channelwise_3FOLD_Trigger_ADC_Spectra_Noise_Masked[iFeb]->Write();

    fOut->cd("ADC Related Plots/Cluster Charge/Three Fold");
    GEM_3FOLD_Trigger_Cluster_Charge_Spectra->Write();
    GEM_3FOLD_Number_Of_Digis_Per_Cluster->Write();

    fIn->Close();
    fOut->Close();

    cout << "Output File Name: " << OutputFileName << endl;

    Code_Timer.Stop();

    cout << "_______________________Code Executed Successfully!________________________" << endl;
    cout << "Real time: " << Code_Timer.RealTime() / 60.0 << " m, CPU time: " << Code_Timer.CpuTime() / 60.0 << " m" << endl;
}
