// Copyright (C) Experimental High Energy Physics and Applications Group, VECC, Kolkata
// Author(s): Apar Agarwal

// This macro has been created for the purpose
// analysing various aspects of Time correlations
// of GEM with Trigger for GIF++ 2023 data

// This Code requires the CbmRoot version DEC 21 or higher
// Recommended on Linux 20.04 --> Fairsoftversion : APR21p2 and FAIRROOTVERSION: v18.6.7 or higher

// Code first tested on 1st October, 2023

void GIF_GEM_Trigger_Timecorrelations(Int_t RunId = 966)
{
    TStopwatch Code_Timer;
    Code_Timer.Start();

    TString InputFileName = "Run" + to_string(RunId) + "_Noise_Removed_Ringing_Walk_Corrected.digi.root";
    TString GEM_Noise_File_Name = "Run_" + to_string(RunId) + "_GEM_Noise.txt";

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

    TFile* fIn = new TFile(InputFileName);
    cout << "Input Run Number:" << RunId << "  File Name: " << InputFileName << endl;

    TTree* Input_Tree = (TTree*)fIn->Get("cbmsim");

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

    TString OutputFileName = "data/Time_Correlations/Run" + to_string(RunId) + "_GEM_Trigger_Time_Correlations_corrected.root";
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
            // if(i==0 || i==1 || i==3 || i==6 || i==10 || i==17)
            // GEM_Infile_Y >> GEM_Coordinate_Feb_Channel[i][127-j][1];
            // else
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

    HistTag = "GEM_Trigger_Two_Fold_1D_Raw_All_Febs_Inclusive";
    HistTitle = "GEM vs. 2 Fold Trigger Time Correlation Spectra (Raw) for Run " + to_string(RunId);
    TH1I* GEM_Trigger_Two_Fold_1D_Raw_All_Febs_Inclusive = new TH1I(HistTag, HistTitle, 5000, -7780, 7780);
    GEM_Trigger_Two_Fold_1D_Raw_All_Febs_Inclusive->GetXaxis()->SetTitle("Time difference (ns)");
    GEM_Trigger_Two_Fold_1D_Raw_All_Febs_Inclusive->GetYaxis()->SetTitle("Counts");

    TH1I* GEM_Trigger_Two_Fold_1D_Raw_Feb[18];

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTag = "GEM_Trigger_Two_Fold_1D_Raw_Feb_" + to_string(iFeb);
        HistTitle = "GEM Feb " + to_string(iFeb) + " vs. 2 Fold Trigger Time Correlation Spectra (Raw) for Run " + to_string(RunId);
        GEM_Trigger_Two_Fold_1D_Raw_Feb[iFeb] = new TH1I(HistTag, HistTitle, 5000, -7780, 7780);
        GEM_Trigger_Two_Fold_1D_Raw_Feb[iFeb]->GetXaxis()->SetTitle("Time difference (ns)");
        GEM_Trigger_Two_Fold_1D_Raw_Feb[iFeb]->GetYaxis()->SetTitle("Counts");
    }

    HistTag = "GEM_Trigger_Two_Fold_1D_Noise_Masked_All_Febs_Inclusive";
    HistTitle = "GEM vs. 2 Fold Trigger Time Correlation Spectra (Noise Masked) for Run " + to_string(RunId);
    TH1I* GEM_Trigger_Two_Fold_1D_Noise_Masked_All_Febs_Inclusive = new TH1I(HistTag, HistTitle, 5000, -7780, 7780);
    GEM_Trigger_Two_Fold_1D_Noise_Masked_All_Febs_Inclusive->GetXaxis()->SetTitle("Time difference (ns)");
    GEM_Trigger_Two_Fold_1D_Noise_Masked_All_Febs_Inclusive->GetYaxis()->SetTitle("Counts");

    TH1I* GEM_Trigger_Two_Fold_1D_Noise_Masked_Feb[18];

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTag = "GEM_Trigger_Two_Fold_1D_Noise_Masked_Feb_" + to_string(iFeb);
        HistTitle = "GEM Feb " + to_string(iFeb) + " vs. 2 Fold Trigger Time Correlation Spectra (Noise Masked) for Run " + to_string(RunId);
        GEM_Trigger_Two_Fold_1D_Noise_Masked_Feb[iFeb] = new TH1I(HistTag, HistTitle, 5000, -7780, 7780);
        GEM_Trigger_Two_Fold_1D_Noise_Masked_Feb[iFeb]->GetXaxis()->SetTitle("Time difference (ns)");
        GEM_Trigger_Two_Fold_1D_Noise_Masked_Feb[iFeb]->GetYaxis()->SetTitle("Counts");
    }

    HistTag = "GEM_Trigger_Two_Fold_1D_Noise_Only_All_Febs_Inclusive";
    HistTitle = "GEM vs. 2 Fold Trigger Time Correlation Spectra (Noise Only) for Run " + to_string(RunId);
    TH1I* GEM_Trigger_Two_Fold_1D_Noise_Only_All_Febs_Inclusive = new TH1I(HistTag, HistTitle, 5000, -7780, 7780);
    GEM_Trigger_Two_Fold_1D_Noise_Only_All_Febs_Inclusive->GetXaxis()->SetTitle("Time difference (ns)");
    GEM_Trigger_Two_Fold_1D_Noise_Only_All_Febs_Inclusive->GetYaxis()->SetTitle("Counts");

    TH1I* GEM_Trigger_Two_Fold_1D_Noise_Only_Feb[18];

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTag = "GEM_Trigger_Two_Fold_1D_Noise_Only_Feb_" + to_string(iFeb);
        HistTitle = "GEM Feb " + to_string(iFeb) + " vs. 2 Fold Trigger Time Correlation Spectra (Noise Only) for Run " + to_string(RunId);
        GEM_Trigger_Two_Fold_1D_Noise_Only_Feb[iFeb] = new TH1I(HistTag, HistTitle, 5000, -7780, 7780);
        GEM_Trigger_Two_Fold_1D_Noise_Only_Feb[iFeb]->GetXaxis()->SetTitle("Time difference (ns)");
        GEM_Trigger_Two_Fold_1D_Noise_Only_Feb[iFeb]->GetYaxis()->SetTitle("Counts");
    }

    HistTag = "GEM_Trigger_Two_Fold_2D_vs_Timeslice_Raw_All_Febs_Inclusive";
    HistTitle = "GEM vs. 2 Fold Trigger Time Correlation Spectra (Raw) with Timeslice for Run " + to_string(RunId);
    TH2F* GEM_Trigger_Two_Fold_2D_vs_Timeslice_Raw_All_Febs_Inclusive = new TH2F(HistTag, HistTitle, nTimeSlices + 3, -1.5, nTimeSlices + 1.5, 5000, -7780, 7780);
    GEM_Trigger_Two_Fold_2D_vs_Timeslice_Raw_All_Febs_Inclusive->GetXaxis()->SetTitle("Timeslice number");
    GEM_Trigger_Two_Fold_2D_vs_Timeslice_Raw_All_Febs_Inclusive->GetYaxis()->SetTitle("Time difference (ns)");

    TH2F* GEM_Trigger_Two_Fold_2D_vs_Timeslice_Raw_Feb[18];

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTag = "GEM_Trigger_Two_Fold_2D_vs_Timeslice_Raw_Feb_" + to_string(iFeb);
        HistTitle = "GEM Feb " + to_string(iFeb) + " vs. 2 Fold Trigger Time Correlation Spectra (Raw) with Timeslice for Run " + to_string(RunId);
        GEM_Trigger_Two_Fold_2D_vs_Timeslice_Raw_Feb[iFeb] = new TH2F(HistTag, HistTitle, nTimeSlices + 3, -1.5, nTimeSlices + 1.5, 5000, -7780, 7780);
        GEM_Trigger_Two_Fold_2D_vs_Timeslice_Raw_Feb[iFeb]->GetXaxis()->SetTitle("Timeslice number");
        GEM_Trigger_Two_Fold_2D_vs_Timeslice_Raw_Feb[iFeb]->GetYaxis()->SetTitle("Time difference (ns)");
    }

    HistTag = "GEM_Trigger_Two_Fold_2D_vs_Timeslice_Noise_Masked_All_Febs_Inclusive";
    HistTitle = "GEM vs. 2 Fold Trigger Time Correlation Spectra (Noise Masked) with Timeslice for Run " + to_string(RunId);
    TH2F* GEM_Trigger_Two_Fold_2D_vs_Timeslice_Noise_Masked_All_Febs_Inclusive = new TH2F(HistTag, HistTitle, nTimeSlices + 3, -1.5, nTimeSlices + 1.5, 5000, -7780, 7780);
    GEM_Trigger_Two_Fold_2D_vs_Timeslice_Noise_Masked_All_Febs_Inclusive->GetXaxis()->SetTitle("Timeslice number");
    GEM_Trigger_Two_Fold_2D_vs_Timeslice_Noise_Masked_All_Febs_Inclusive->GetYaxis()->SetTitle("Time difference (ns)");

    TH2F* GEM_Trigger_Two_Fold_2D_vs_Timeslice_Noise_Masked_Feb[18];

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTag = "GEM_Trigger_Two_Fold_2D_vs_Timeslice_Noise_Masked_Feb_" + to_string(iFeb);
        HistTitle = "GEM Feb " + to_string(iFeb) + " vs. 2 Fold Trigger Time Correlation Spectra (Noise Masked) with Timeslice for Run " + to_string(RunId);
        GEM_Trigger_Two_Fold_2D_vs_Timeslice_Noise_Masked_Feb[iFeb] = new TH2F(HistTag, HistTitle, nTimeSlices + 3, -1.5, nTimeSlices + 1.5, 5000, -7780, 7780);
        GEM_Trigger_Two_Fold_2D_vs_Timeslice_Noise_Masked_Feb[iFeb]->GetXaxis()->SetTitle("Timeslice number");
        GEM_Trigger_Two_Fold_2D_vs_Timeslice_Noise_Masked_Feb[iFeb]->GetYaxis()->SetTitle("Time difference (ns)");
    }

    HistTag = "GEM_Trigger_Two_Fold_2D_vs_Timeslice_Noise_Only_All_Febs_Inclusive";
    HistTitle = "GEM vs. 2 Fold Trigger Time Correlation Spectra (Noise Only) with Timeslice for Run " + to_string(RunId);
    TH2F* GEM_Trigger_Two_Fold_2D_vs_Timeslice_Noise_Only_All_Febs_Inclusive = new TH2F(HistTag, HistTitle, nTimeSlices + 3, -1.5, nTimeSlices + 1.5, 5000, -7780, 7780);
    GEM_Trigger_Two_Fold_2D_vs_Timeslice_Noise_Only_All_Febs_Inclusive->GetXaxis()->SetTitle("Timeslice number");
    GEM_Trigger_Two_Fold_2D_vs_Timeslice_Noise_Only_All_Febs_Inclusive->GetYaxis()->SetTitle("Time difference (ns)");

    TH2F* GEM_Trigger_Two_Fold_2D_vs_Timeslice_Noise_Only_Feb[18];

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTag = "GEM_Trigger_Two_Fold_2D_vs_Timeslice_Noise_Only_Feb_" + to_string(iFeb);
        HistTitle = "GEM Feb " + to_string(iFeb) + " vs. 2 Fold Trigger Time Correlation Spectra (Noise Only) with Timeslice for Run " + to_string(RunId);
        GEM_Trigger_Two_Fold_2D_vs_Timeslice_Noise_Only_Feb[iFeb] = new TH2F(HistTag, HistTitle, nTimeSlices + 3, -1.5, nTimeSlices + 1.5, 5000, -7780, 7780);
        GEM_Trigger_Two_Fold_2D_vs_Timeslice_Noise_Only_Feb[iFeb]->GetXaxis()->SetTitle("Timeslice number");
        GEM_Trigger_Two_Fold_2D_vs_Timeslice_Noise_Only_Feb[iFeb]->GetYaxis()->SetTitle("Time difference (ns)");
    }

    TH2F* GEM_Trigger_Two_Fold_2D_vs_ADC_Raw_Feb[18];

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTag = "GEM_Trigger_Two_Fold_2D_vs_ADC_Raw_Feb_" + to_string(iFeb);
        HistTitle = "GEM Feb " + to_string(iFeb) + " vs. 2 Fold Trigger Time Correlation Spectra (Raw) with ADC for Run " + to_string(RunId);
        GEM_Trigger_Two_Fold_2D_vs_ADC_Raw_Feb[iFeb] = new TH2F(HistTag, HistTitle, 35, -1.5, 33.5, 5000, -7780, 7780);
        GEM_Trigger_Two_Fold_2D_vs_ADC_Raw_Feb[iFeb]->GetXaxis()->SetTitle("ADC");
        GEM_Trigger_Two_Fold_2D_vs_ADC_Raw_Feb[iFeb]->GetYaxis()->SetTitle("Time difference (ns)");
    }

    TH2F* GEM_Trigger_Two_Fold_2D_vs_ADC_Noise_Masked_Feb[18];

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTag = "GEM_Trigger_Two_Fold_2D_vs_ADC_Noise_Masked_Feb_" + to_string(iFeb);
        HistTitle = "GEM Feb " + to_string(iFeb) + " vs. 2 Fold Trigger Time Correlation Spectra (Noise Masked) with ADC for Run " + to_string(RunId);
        GEM_Trigger_Two_Fold_2D_vs_ADC_Noise_Masked_Feb[iFeb] = new TH2F(HistTag, HistTitle, 35, -1.5, 33.5, 5000, -7780, 7780);
        GEM_Trigger_Two_Fold_2D_vs_ADC_Noise_Masked_Feb[iFeb]->GetXaxis()->SetTitle("ADC");
        GEM_Trigger_Two_Fold_2D_vs_ADC_Noise_Masked_Feb[iFeb]->GetYaxis()->SetTitle("Time difference (ns)");
    }

    TH2F* GEM_Trigger_Two_Fold_2D_vs_ADC_Noise_Only_Feb[18];

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTag = "GEM_Trigger_Two_Fold_2D_vs_ADC_Noise_Only_Feb_" + to_string(iFeb);
        HistTitle = "GEM Feb " + to_string(iFeb) + " vs. 2 Fold Trigger Time Correlation Spectra (Noise Only) with ADC for Run " + to_string(RunId);
        GEM_Trigger_Two_Fold_2D_vs_ADC_Noise_Only_Feb[iFeb] = new TH2F(HistTag, HistTitle, 35, -1.5, 33.5, 5000, -7780, 7780);
        GEM_Trigger_Two_Fold_2D_vs_ADC_Noise_Only_Feb[iFeb]->GetXaxis()->SetTitle("ADC");
        GEM_Trigger_Two_Fold_2D_vs_ADC_Noise_Only_Feb[iFeb]->GetYaxis()->SetTitle("Time difference (ns)");
    }

    HistTag = "GEM_Trigger_Two_Fold_2D_vs_Feb_Raw";
    HistTitle = "GEM vs. 2 Fold Trigger Time Correlation Spectra (Raw) with Feb number for Run " + to_string(RunId);
    TH2F* GEM_Trigger_Two_Fold_2D_vs_Feb_Raw = new TH2F(HistTag, HistTitle, 21, -1.5, 19.5, 5000, -7780, 7780);
    GEM_Trigger_Two_Fold_2D_vs_Feb_Raw->GetXaxis()->SetTitle("FEB number");
    GEM_Trigger_Two_Fold_2D_vs_Feb_Raw->GetYaxis()->SetTitle("Time difference (ns)");

    HistTag = "GEM_Trigger_Two_Fold_2D_vs_Feb_Noise_Masked";
    HistTitle = "GEM vs. 2 Fold Trigger Time Correlation Spectra (Noise Masked) with Feb number for Run " + to_string(RunId);
    TH2F* GEM_Trigger_Two_Fold_2D_vs_Feb_Noise_Masked = new TH2F(HistTag, HistTitle, 21, -1.5, 19.5, 5000, -7780, 7780);
    GEM_Trigger_Two_Fold_2D_vs_Feb_Noise_Masked->GetXaxis()->SetTitle("FEB number");
    GEM_Trigger_Two_Fold_2D_vs_Feb_Noise_Masked->GetYaxis()->SetTitle("Time difference (ns)");

    HistTag = "GEM_Trigger_Two_Fold_2D_vs_Feb_Noise_Only";
    HistTitle = "GEM vs. 2 Fold Trigger Time Correlation Spectra (Noise Only) with Feb number for Run " + to_string(RunId);
    TH2F* GEM_Trigger_Two_Fold_2D_vs_Feb_Noise_Only = new TH2F(HistTag, HistTitle, 21, -1.5, 19.5, 5000, -7780, 7780);
    GEM_Trigger_Two_Fold_2D_vs_Feb_Noise_Only->GetXaxis()->SetTitle("FEB number");
    GEM_Trigger_Two_Fold_2D_vs_Feb_Noise_Only->GetYaxis()->SetTitle("Time difference (ns)");

    TH2F* GEM_Trigger_Two_Fold_2D_vs_GEM_Channel_Raw_Feb[18];

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTag = "GEM_Trigger_Two_Fold_2D_vs_GEM_Channel_Raw_Feb_" + to_string(iFeb);
        HistTitle = "GEM Feb " + to_string(iFeb) + " vs. 2 Fold Trigger Time Correlation Spectra (Raw) with GEM Channel number for Run " + to_string(RunId);
        GEM_Trigger_Two_Fold_2D_vs_GEM_Channel_Raw_Feb[iFeb] = new TH2F(HistTag, HistTitle, 131, -1.5, 129.5, 5000, -7780, 7780);
        GEM_Trigger_Two_Fold_2D_vs_GEM_Channel_Raw_Feb[iFeb]->GetXaxis()->SetTitle("Channel number");
        GEM_Trigger_Two_Fold_2D_vs_GEM_Channel_Raw_Feb[iFeb]->GetYaxis()->SetTitle("Time difference (ns)");
    }

    TH2F* GEM_Trigger_Two_Fold_2D_vs_GEM_Channel_Noise_Masked_Feb[18];

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTag = "GEM_Trigger_Two_Fold_2D_vs_GEM_Channel_Noise_Masked_Feb_" + to_string(iFeb);
        HistTitle = "GEM Feb " + to_string(iFeb) + " vs. 2 Fold Trigger Time Correlation Spectra (Noise Masked) with GEM Channel number for Run " + to_string(RunId);
        GEM_Trigger_Two_Fold_2D_vs_GEM_Channel_Noise_Masked_Feb[iFeb] = new TH2F(HistTag, HistTitle, 131, -1.5, 129.5, 5000, -7780, 7780);
        GEM_Trigger_Two_Fold_2D_vs_GEM_Channel_Noise_Masked_Feb[iFeb]->GetXaxis()->SetTitle("Channel number");
        GEM_Trigger_Two_Fold_2D_vs_GEM_Channel_Noise_Masked_Feb[iFeb]->GetYaxis()->SetTitle("Time difference (ns)");
    }

    TH2F* GEM_Trigger_Two_Fold_2D_vs_GEM_Channel_Noise_Only_Feb[18];

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTag = "GEM_Trigger_Two_Fold_2D_vs_GEM_Channel_Noise_Only_Feb_" + to_string(iFeb);
        HistTitle = "GEM Feb " + to_string(iFeb) + " vs. 2 Fold Trigger Time Correlation Spectra (Noise Only) with GEM Channel number for Run " + to_string(RunId);
        GEM_Trigger_Two_Fold_2D_vs_GEM_Channel_Noise_Only_Feb[iFeb] = new TH2F(HistTag, HistTitle, 131, -1.5, 129.5, 5000, -7780, 7780);
        GEM_Trigger_Two_Fold_2D_vs_GEM_Channel_Noise_Only_Feb[iFeb]->GetXaxis()->SetTitle("Channel number");
        GEM_Trigger_Two_Fold_2D_vs_GEM_Channel_Noise_Only_Feb[iFeb]->GetYaxis()->SetTitle("Time difference (ns)");
    }

    HistTag = "GEM_Trigger_Three_Fold_1D_Raw_All_Febs_Inclusive";
    HistTitle = "GEM vs. 3 Fold Trigger Time Correlation Spectra (Raw) for Run " + to_string(RunId);
    TH1I* GEM_Trigger_Three_Fold_1D_Raw_All_Febs_Inclusive = new TH1I(HistTag, HistTitle, 5000, -7780, 7780);
    GEM_Trigger_Three_Fold_1D_Raw_All_Febs_Inclusive->GetXaxis()->SetTitle("Time difference (ns)");
    GEM_Trigger_Three_Fold_1D_Raw_All_Febs_Inclusive->GetYaxis()->SetTitle("Counts");

    TH1I* GEM_Trigger_Three_Fold_1D_Raw_Feb[18];

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTag = "GEM_Trigger_Three_Fold_1D_Raw_Feb_" + to_string(iFeb);
        HistTitle = "GEM Feb " + to_string(iFeb) + " vs. 3 Fold Trigger Time Correlation Spectra (Raw) for Run " + to_string(RunId);
        GEM_Trigger_Three_Fold_1D_Raw_Feb[iFeb] = new TH1I(HistTag, HistTitle, 5000, -7780, 7780);
        GEM_Trigger_Three_Fold_1D_Raw_Feb[iFeb]->GetXaxis()->SetTitle("Time difference (ns)");
        GEM_Trigger_Three_Fold_1D_Raw_Feb[iFeb]->GetYaxis()->SetTitle("Counts");
    }

    HistTag = "GEM_Trigger_Three_Fold_1D_Noise_Masked_All_Febs_Inclusive";
    HistTitle = "GEM vs. 3 Fold Trigger Time Correlation Spectra (Noise Masked) for Run " + to_string(RunId);
    TH1I* GEM_Trigger_Three_Fold_1D_Noise_Masked_All_Febs_Inclusive = new TH1I(HistTag, HistTitle, 5000, -7780, 7780);
    GEM_Trigger_Three_Fold_1D_Noise_Masked_All_Febs_Inclusive->GetXaxis()->SetTitle("Time difference (ns)");
    GEM_Trigger_Three_Fold_1D_Noise_Masked_All_Febs_Inclusive->GetYaxis()->SetTitle("Counts");

    TH1I* GEM_Trigger_Three_Fold_1D_Noise_Masked_Feb[18];

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTag = "GEM_Trigger_Three_Fold_1D_Noise_Masked_Feb_" + to_string(iFeb);
        HistTitle = "GEM Feb " + to_string(iFeb) + " vs. 3 Fold Trigger Time Correlation Spectra (Noise Masked) for Run " + to_string(RunId);
        GEM_Trigger_Three_Fold_1D_Noise_Masked_Feb[iFeb] = new TH1I(HistTag, HistTitle, 5000, -7780, 7780);
        GEM_Trigger_Three_Fold_1D_Noise_Masked_Feb[iFeb]->GetXaxis()->SetTitle("Time difference (ns)");
        GEM_Trigger_Three_Fold_1D_Noise_Masked_Feb[iFeb]->GetYaxis()->SetTitle("Counts");
    }

    HistTag = "GEM_Trigger_Three_Fold_1D_Noise_Only_All_Febs_Inclusive";
    HistTitle = "GEM vs. 3 Fold Trigger Time Correlation Spectra (Noise Only) for Run " + to_string(RunId);
    TH1I* GEM_Trigger_Three_Fold_1D_Noise_Only_All_Febs_Inclusive = new TH1I(HistTag, HistTitle, 5000, -7780, 7780);
    GEM_Trigger_Three_Fold_1D_Noise_Only_All_Febs_Inclusive->GetXaxis()->SetTitle("Time difference (ns)");
    GEM_Trigger_Three_Fold_1D_Noise_Only_All_Febs_Inclusive->GetYaxis()->SetTitle("Counts");

    TH1I* GEM_Trigger_Three_Fold_1D_Noise_Only_Feb[18];

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTag = "GEM_Trigger_Three_Fold_1D_Noise_Only_Feb_" + to_string(iFeb);
        HistTitle = "GEM Feb " + to_string(iFeb) + " vs. 3 Fold Trigger Time Correlation Spectra (Noise Only) for Run " + to_string(RunId);
        GEM_Trigger_Three_Fold_1D_Noise_Only_Feb[iFeb] = new TH1I(HistTag, HistTitle, 5000, -7780, 7780);
        GEM_Trigger_Three_Fold_1D_Noise_Only_Feb[iFeb]->GetXaxis()->SetTitle("Time difference (ns)");
        GEM_Trigger_Three_Fold_1D_Noise_Only_Feb[iFeb]->GetYaxis()->SetTitle("Counts");
    }

    HistTag = "GEM_Trigger_Three_Fold_2D_vs_Timeslice_Raw_All_Febs_Inclusive";
    HistTitle = "GEM vs. 3 Fold Trigger Time Correlation Spectra (Raw) with Timeslice for Run " + to_string(RunId);
    TH2F* GEM_Trigger_Three_Fold_2D_vs_Timeslice_Raw_All_Febs_Inclusive = new TH2F(HistTag, HistTitle, nTimeSlices + 3, -1.5, nTimeSlices + 1.5, 5000, -7780, 7780);
    GEM_Trigger_Three_Fold_2D_vs_Timeslice_Raw_All_Febs_Inclusive->GetXaxis()->SetTitle("Timeslice number");
    GEM_Trigger_Three_Fold_2D_vs_Timeslice_Raw_All_Febs_Inclusive->GetYaxis()->SetTitle("Time difference (ns)");

    TH2F* GEM_Trigger_Three_Fold_2D_vs_Timeslice_Raw_Feb[18];

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTag = "GEM_Trigger_Three_Fold_2D_vs_Timeslice_Raw_Feb_" + to_string(iFeb);
        HistTitle = "GEM Feb " + to_string(iFeb) + " vs. 3 Fold Trigger Time Correlation Spectra (Raw) with Timeslice for Run " + to_string(RunId);
        GEM_Trigger_Three_Fold_2D_vs_Timeslice_Raw_Feb[iFeb] = new TH2F(HistTag, HistTitle, nTimeSlices + 3, -1.5, nTimeSlices + 1.5, 5000, -7780, 7780);
        GEM_Trigger_Three_Fold_2D_vs_Timeslice_Raw_Feb[iFeb]->GetXaxis()->SetTitle("Timeslice number");
        GEM_Trigger_Three_Fold_2D_vs_Timeslice_Raw_Feb[iFeb]->GetYaxis()->SetTitle("Time difference (ns)");
    }

    HistTag = "GEM_Trigger_Three_Fold_2D_vs_Timeslice_Noise_Masked_All_Febs_Inclusive";
    HistTitle = "GEM vs. 3 Fold Trigger Time Correlation Spectra (Noise Masked) with Timeslice for Run " + to_string(RunId);
    TH2F* GEM_Trigger_Three_Fold_2D_vs_Timeslice_Noise_Masked_All_Febs_Inclusive = new TH2F(HistTag, HistTitle, nTimeSlices + 3, -1.5, nTimeSlices + 1.5, 5000, -7780, 7780);
    GEM_Trigger_Three_Fold_2D_vs_Timeslice_Noise_Masked_All_Febs_Inclusive->GetXaxis()->SetTitle("Timeslice number");
    GEM_Trigger_Three_Fold_2D_vs_Timeslice_Noise_Masked_All_Febs_Inclusive->GetYaxis()->SetTitle("Time difference (ns)");

    TH2F* GEM_Trigger_Three_Fold_2D_vs_Timeslice_Noise_Masked_Feb[18];

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTag = "GEM_Trigger_Three_Fold_2D_vs_Timeslice_Noise_Masked_Feb_" + to_string(iFeb);
        HistTitle = "GEM Feb " + to_string(iFeb) + " vs. 3 Fold Trigger Time Correlation Spectra (Noise Masked) with Timeslice for Run " + to_string(RunId);
        GEM_Trigger_Three_Fold_2D_vs_Timeslice_Noise_Masked_Feb[iFeb] = new TH2F(HistTag, HistTitle, nTimeSlices + 3, -1.5, nTimeSlices + 1.5, 5000, -7780, 7780);
        GEM_Trigger_Three_Fold_2D_vs_Timeslice_Noise_Masked_Feb[iFeb]->GetXaxis()->SetTitle("Timeslice number");
        GEM_Trigger_Three_Fold_2D_vs_Timeslice_Noise_Masked_Feb[iFeb]->GetYaxis()->SetTitle("Time difference (ns)");
    }

    HistTag = "GEM_Trigger_Three_Fold_2D_vs_Timeslice_Noise_Only_All_Febs_Inclusive";
    HistTitle = "GEM vs. 3 Fold Trigger Time Correlation Spectra (Noise Only) with Timeslice for Run " + to_string(RunId);
    TH2F* GEM_Trigger_Three_Fold_2D_vs_Timeslice_Noise_Only_All_Febs_Inclusive = new TH2F(HistTag, HistTitle, nTimeSlices + 3, -1.5, nTimeSlices + 1.5, 5000, -7780, 7780);
    GEM_Trigger_Three_Fold_2D_vs_Timeslice_Noise_Only_All_Febs_Inclusive->GetXaxis()->SetTitle("Timeslice number");
    GEM_Trigger_Three_Fold_2D_vs_Timeslice_Noise_Only_All_Febs_Inclusive->GetYaxis()->SetTitle("Time difference (ns)");

    TH2F* GEM_Trigger_Three_Fold_2D_vs_Timeslice_Noise_Only_Feb[18];

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTag = "GEM_Trigger_Three_Fold_2D_vs_Timeslice_Noise_Only_Feb_" + to_string(iFeb);
        HistTitle = "GEM Feb " + to_string(iFeb) + " vs. 3 Fold Trigger Time Correlation Spectra (Noise Only) with Timeslice for Run " + to_string(RunId);
        GEM_Trigger_Three_Fold_2D_vs_Timeslice_Noise_Only_Feb[iFeb] = new TH2F(HistTag, HistTitle, nTimeSlices + 3, -1.5, nTimeSlices + 1.5, 5000, -7780, 7780);
        GEM_Trigger_Three_Fold_2D_vs_Timeslice_Noise_Only_Feb[iFeb]->GetXaxis()->SetTitle("Timeslice number");
        GEM_Trigger_Three_Fold_2D_vs_Timeslice_Noise_Only_Feb[iFeb]->GetYaxis()->SetTitle("Time difference (ns)");
    }

    TH2F* GEM_Trigger_Three_Fold_2D_vs_ADC_Raw_Feb[18];

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTag = "GEM_Trigger_Three_Fold_2D_vs_ADC_Raw_Feb_" + to_string(iFeb);
        HistTitle = "GEM Feb " + to_string(iFeb) + " vs. 3 Fold Trigger Time Correlation Spectra (Raw) with ADC for Run " + to_string(RunId);
        GEM_Trigger_Three_Fold_2D_vs_ADC_Raw_Feb[iFeb] = new TH2F(HistTag, HistTitle, 35, -1.5, 33.5, 5000, -7780, 7780);
        GEM_Trigger_Three_Fold_2D_vs_ADC_Raw_Feb[iFeb]->GetXaxis()->SetTitle("ADC");
        GEM_Trigger_Three_Fold_2D_vs_ADC_Raw_Feb[iFeb]->GetYaxis()->SetTitle("Time difference (ns)");
    }

    TH2F* GEM_Trigger_Three_Fold_2D_vs_ADC_Noise_Masked_Feb[18];

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTag = "GEM_Trigger_Three_Fold_2D_vs_ADC_Noise_Masked_Feb_" + to_string(iFeb);
        HistTitle = "GEM Feb " + to_string(iFeb) + " vs. 3 Fold Trigger Time Correlation Spectra (Noise Masked) with ADC for Run " + to_string(RunId);
        GEM_Trigger_Three_Fold_2D_vs_ADC_Noise_Masked_Feb[iFeb] = new TH2F(HistTag, HistTitle, 35, -1.5, 33.5, 5000, -7780, 7780);
        GEM_Trigger_Three_Fold_2D_vs_ADC_Noise_Masked_Feb[iFeb]->GetXaxis()->SetTitle("ADC");
        GEM_Trigger_Three_Fold_2D_vs_ADC_Noise_Masked_Feb[iFeb]->GetYaxis()->SetTitle("Time difference (ns)");
    }

    TH2F* GEM_Trigger_Three_Fold_2D_vs_ADC_Noise_Only_Feb[18];

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTag = "GEM_Trigger_Three_Fold_2D_vs_ADC_Noise_Only_Feb_" + to_string(iFeb);
        HistTitle = "GEM Feb " + to_string(iFeb) + " vs. 3 Fold Trigger Time Correlation Spectra (Noise Only) with ADC for Run " + to_string(RunId);
        GEM_Trigger_Three_Fold_2D_vs_ADC_Noise_Only_Feb[iFeb] = new TH2F(HistTag, HistTitle, 35, -1.5, 33.5, 5000, -7780, 7780);
        GEM_Trigger_Three_Fold_2D_vs_ADC_Noise_Only_Feb[iFeb]->GetXaxis()->SetTitle("ADC");
        GEM_Trigger_Three_Fold_2D_vs_ADC_Noise_Only_Feb[iFeb]->GetYaxis()->SetTitle("Time difference (ns)");
    }

    HistTag = "GEM_Trigger_Three_Fold_2D_vs_Feb_Raw";
    HistTitle = "GEM vs. 3 Fold Trigger Time Correlation Spectra (Raw) with Feb number for Run " + to_string(RunId);
    TH2F* GEM_Trigger_Three_Fold_2D_vs_Feb_Raw = new TH2F(HistTag, HistTitle, 21, -1.5, 19.5, 5000, -7780, 7780);
    GEM_Trigger_Three_Fold_2D_vs_Feb_Raw->GetXaxis()->SetTitle("FEB number");
    GEM_Trigger_Three_Fold_2D_vs_Feb_Raw->GetYaxis()->SetTitle("Time difference (ns)");

    HistTag = "GEM_Trigger_Three_Fold_2D_vs_Feb_Noise_Masked";
    HistTitle = "GEM vs. 3 Fold Trigger Time Correlation Spectra (Noise Masked) with Feb number for Run " + to_string(RunId);
    TH2F* GEM_Trigger_Three_Fold_2D_vs_Feb_Noise_Masked = new TH2F(HistTag, HistTitle, 21, -1.5, 19.5, 5000, -7780, 7780);
    GEM_Trigger_Three_Fold_2D_vs_Feb_Noise_Masked->GetXaxis()->SetTitle("FEB number");
    GEM_Trigger_Three_Fold_2D_vs_Feb_Noise_Masked->GetYaxis()->SetTitle("Time difference (ns)");

    HistTag = "GEM_Trigger_Three_Fold_2D_vs_Feb_Noise_Only";
    HistTitle = "GEM vs. 3 Fold Trigger Time Correlation Spectra (Noise Only) with Feb number for Run " + to_string(RunId);
    TH2F* GEM_Trigger_Three_Fold_2D_vs_Feb_Noise_Only = new TH2F(HistTag, HistTitle, 21, -1.5, 19.5, 5000, -7780, 7780);
    GEM_Trigger_Three_Fold_2D_vs_Feb_Noise_Only->GetXaxis()->SetTitle("FEB number");
    GEM_Trigger_Three_Fold_2D_vs_Feb_Noise_Only->GetYaxis()->SetTitle("Time difference (ns)");

    TH2F* GEM_Trigger_Three_Fold_2D_vs_GEM_Channel_Raw_Feb[18];

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTag = "GEM_Trigger_Three_Fold_2D_vs_GEM_Channel_Raw_Feb_" + to_string(iFeb);
        HistTitle = "GEM Feb " + to_string(iFeb) + " vs. 3 Fold Trigger Time Correlation Spectra (Raw) with GEM Channel number for Run " + to_string(RunId);
        GEM_Trigger_Three_Fold_2D_vs_GEM_Channel_Raw_Feb[iFeb] = new TH2F(HistTag, HistTitle, 131, -1.5, 129.5, 5000, -7780, 7780);
        GEM_Trigger_Three_Fold_2D_vs_GEM_Channel_Raw_Feb[iFeb]->GetXaxis()->SetTitle("Channel number");
        GEM_Trigger_Three_Fold_2D_vs_GEM_Channel_Raw_Feb[iFeb]->GetYaxis()->SetTitle("Time difference (ns)");
    }

    TH2F* GEM_Trigger_Three_Fold_2D_vs_GEM_Channel_Noise_Masked_Feb[18];

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTag = "GEM_Trigger_Three_Fold_2D_vs_GEM_Channel_Noise_Masked_Feb_" + to_string(iFeb);
        HistTitle = "GEM Feb " + to_string(iFeb) + " vs. 3 Fold Trigger Time Correlation Spectra (Noise Masked) with GEM Channel number for Run " + to_string(RunId);
        GEM_Trigger_Three_Fold_2D_vs_GEM_Channel_Noise_Masked_Feb[iFeb] = new TH2F(HistTag, HistTitle, 131, -1.5, 129.5, 5000, -7780, 7780);
        GEM_Trigger_Three_Fold_2D_vs_GEM_Channel_Noise_Masked_Feb[iFeb]->GetXaxis()->SetTitle("Channel number");
        GEM_Trigger_Three_Fold_2D_vs_GEM_Channel_Noise_Masked_Feb[iFeb]->GetYaxis()->SetTitle("Time difference (ns)");
    }

    TH2F* GEM_Trigger_Three_Fold_2D_vs_GEM_Channel_Noise_Only_Feb[18];

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTag = "GEM_Trigger_Three_Fold_2D_vs_GEM_Channel_Noise_Only_Feb_" + to_string(iFeb);
        HistTitle = "GEM Feb " + to_string(iFeb) + " vs. 3 Fold Trigger Time Correlation Spectra (Noise Only) with GEM Channel number for Run " + to_string(RunId);
        GEM_Trigger_Three_Fold_2D_vs_GEM_Channel_Noise_Only_Feb[iFeb] = new TH2F(HistTag, HistTitle, 131, -1.5, 129.5, 5000, -7780, 7780);
        GEM_Trigger_Three_Fold_2D_vs_GEM_Channel_Noise_Only_Feb[iFeb]->GetXaxis()->SetTitle("Channel number");
        GEM_Trigger_Three_Fold_2D_vs_GEM_Channel_Noise_Only_Feb[iFeb]->GetYaxis()->SetTitle("Time difference (ns)");
    }

    Int_t Two_Fold_Lower_Bound = 0;
    Int_t Two_Fold_Upper_Bound = 64;
    Int_t Three_Fold_Lower_Bound = 64;
    Int_t Three_Fold_Upper_Bound = 128;

    for (iTreeEntry = 0; iTreeEntry < nTimeSlices; iTreeEntry++)
    {
        Tree_Data_Reader.SetEntry(iTreeEntry);
        Int_t iTimeSlice = TimeSlice_Event_Header->GetTsIndex();

        cout << "Checking for 2F and 3F Channels:  iTreeEntry: " << iTreeEntry << "  TS Index: " << iTimeSlice << endl;

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
                cout << "Found! 0-63: 3F   and    64-127: 2F" << endl;
                break;
            }
            else
            {
                Two_Fold_Lower_Bound = 0;
                Two_Fold_Upper_Bound = 64;
                Three_Fold_Lower_Bound = 64;
                Three_Fold_Upper_Bound = 128;
                cout << "Found! 0-63: 2F   and    64-127: 3F" << endl;
                break;
            }
        }
    }


    for (iTreeEntry = 0; iTreeEntry < nTimeSlices; iTreeEntry++)
    {
        Tree_Data_Reader.SetEntry(iTreeEntry);
        uint64_t TimeSlice_Start_Time = TimeSlice_Event_Header->GetTsStartTime();

        Int_t iTimeSlice = TimeSlice_Event_Header->GetTsIndex();

        // if(iTreeEntry%100 == 0)
        cout << "iTreeEntry: " << iTreeEntry << "  TS Index: " << iTimeSlice << endl;


        for (Int_t iMuchDigi = 0; iMuchDigi < Much_Digi.GetSize(); iMuchDigi++)
        // for(Int_t iMuchDigi=iMuchStartDigi;iMuchDigi1 < Much_Digi.GetSize();iMuchDigi1++,TriggerFlag=false)
        {
            int32_t Channel = CbmMuchAddress::GetChannelIndex(Much_Digi[iMuchDigi].GetAddress());
            if (Channel != 125 && Channel != 5)
                continue; // Single Channel from both 2 Fold or 3 Fold Coincidence
            // if(Channel!=125) continue; // 3-FOLD Coincidence only
            // if(Channel!=5) continue; // 2-FOLD Coincidence only
            int32_t Station = CbmMuchAddress::GetStationIndex(Much_Digi[iMuchDigi].GetAddress());
            if (Station != 1)
                continue; // Selecting Only Trigger Hits

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

                int32_t Station1 = CbmMuchAddress::GetStationIndex(Much_Digi[iMuchDigi1].GetAddress());
                if (Station1 != 0)
                    continue; // Selecting Only GEM Hits

                Double_t First_Time_difference = ((Double_t)Much_Digi[iMuchDigi1].GetTime() - (Double_t)Much_Digi[0].GetTime()) / 1000.0;

                Double_t Time_difference = ((Double_t)Much_Digi[iMuchDigi].GetTime() - (Double_t)Much_Digi[iMuchDigi1].GetTime()) / 1000.0;

                if (First_Time_difference > 0.0)
                {
                    if (Time_difference > 9.000)
                        continue;
                    else if (Time_difference < -9.000)
                        break;
                    else
                    {
                        int32_t Sector1 = CbmMuchAddress::GetSectorIndex(Much_Digi[iMuchDigi1].GetAddress());
                        int32_t Channel1 = CbmMuchAddress::GetChannelIndex(Much_Digi[iMuchDigi1].GetAddress());

                        if (Sector1 > 96 || Channel1 > 22)
                            continue;
                        Double_t Time_difference_ns = ((Double_t)Much_Digi[iMuchDigi].GetTime() - (Double_t)Much_Digi[iMuchDigi1].GetTime());

                        Int_t FEB_GEM = GEM_Feb_no_X_Y[Channel1][Sector1];
                        Int_t FEB_CH = GEM_Ch_no_X_Y[Channel1][Sector1];
                        Int_t ADC_Value = Much_Digi[iMuchDigi1].GetAdc();

                        if (Channel >= Two_Fold_Lower_Bound && Channel < Two_Fold_Upper_Bound)
                        {
                            GEM_Trigger_Two_Fold_1D_Raw_All_Febs_Inclusive->Fill(Time_difference_ns);
                            GEM_Trigger_Two_Fold_1D_Raw_Feb[FEB_GEM]->Fill(Time_difference_ns);

                            GEM_Trigger_Two_Fold_2D_vs_Timeslice_Raw_All_Febs_Inclusive->Fill(iTimeSlice, Time_difference_ns);
                            GEM_Trigger_Two_Fold_2D_vs_Timeslice_Raw_Feb[FEB_GEM]->Fill(iTimeSlice, Time_difference_ns);

                            GEM_Trigger_Two_Fold_2D_vs_ADC_Raw_Feb[FEB_GEM]->Fill(ADC_Value, Time_difference_ns);
                            GEM_Trigger_Two_Fold_2D_vs_Feb_Raw->Fill(FEB_GEM, Time_difference_ns);
                            GEM_Trigger_Two_Fold_2D_vs_GEM_Channel_Raw_Feb[FEB_GEM]->Fill(FEB_CH, Time_difference_ns);

                            if (GEM_Noise[Channel1][Sector1] != -1)
                            {
                                GEM_Trigger_Two_Fold_1D_Noise_Masked_All_Febs_Inclusive->Fill(Time_difference_ns);
                                GEM_Trigger_Two_Fold_1D_Noise_Masked_Feb[FEB_GEM]->Fill(Time_difference_ns);

                                GEM_Trigger_Two_Fold_2D_vs_Timeslice_Noise_Masked_All_Febs_Inclusive->Fill(iTimeSlice, Time_difference_ns);
                                GEM_Trigger_Two_Fold_2D_vs_Timeslice_Noise_Masked_Feb[FEB_GEM]->Fill(iTimeSlice, Time_difference_ns);

                                GEM_Trigger_Two_Fold_2D_vs_ADC_Noise_Masked_Feb[FEB_GEM]->Fill(ADC_Value, Time_difference_ns);
                                GEM_Trigger_Two_Fold_2D_vs_Feb_Noise_Masked->Fill(FEB_GEM, Time_difference_ns);
                                GEM_Trigger_Two_Fold_2D_vs_GEM_Channel_Noise_Masked_Feb[FEB_GEM]->Fill(FEB_CH, Time_difference_ns);
                            }
                            else
                            {
                                GEM_Trigger_Two_Fold_1D_Noise_Only_All_Febs_Inclusive->Fill(Time_difference_ns);
                                GEM_Trigger_Two_Fold_1D_Noise_Only_Feb[FEB_GEM]->Fill(Time_difference_ns);

                                GEM_Trigger_Two_Fold_2D_vs_Timeslice_Noise_Only_All_Febs_Inclusive->Fill(iTimeSlice, Time_difference_ns);
                                GEM_Trigger_Two_Fold_2D_vs_Timeslice_Noise_Only_Feb[FEB_GEM]->Fill(iTimeSlice, Time_difference_ns);

                                GEM_Trigger_Two_Fold_2D_vs_ADC_Noise_Only_Feb[FEB_GEM]->Fill(ADC_Value, Time_difference_ns);
                                GEM_Trigger_Two_Fold_2D_vs_Feb_Noise_Only->Fill(FEB_GEM, Time_difference_ns);
                                GEM_Trigger_Two_Fold_2D_vs_GEM_Channel_Noise_Only_Feb[FEB_GEM]->Fill(FEB_CH, Time_difference_ns);
                            }
                        }
                        else if (Channel >= Three_Fold_Lower_Bound && Channel < Three_Fold_Upper_Bound)
                        {
                            GEM_Trigger_Three_Fold_1D_Raw_All_Febs_Inclusive->Fill(Time_difference_ns);
                            GEM_Trigger_Three_Fold_1D_Raw_Feb[FEB_GEM]->Fill(Time_difference_ns);

                            GEM_Trigger_Three_Fold_2D_vs_Timeslice_Raw_All_Febs_Inclusive->Fill(iTimeSlice, Time_difference_ns);
                            GEM_Trigger_Three_Fold_2D_vs_Timeslice_Raw_Feb[FEB_GEM]->Fill(iTimeSlice, Time_difference_ns);

                            GEM_Trigger_Three_Fold_2D_vs_ADC_Raw_Feb[FEB_GEM]->Fill(ADC_Value, Time_difference_ns);
                            GEM_Trigger_Three_Fold_2D_vs_Feb_Raw->Fill(FEB_GEM, Time_difference_ns);
                            GEM_Trigger_Three_Fold_2D_vs_GEM_Channel_Raw_Feb[FEB_GEM]->Fill(FEB_CH, Time_difference_ns);

                            if (GEM_Noise[Channel1][Sector1] != -1)
                            {
                                GEM_Trigger_Three_Fold_1D_Noise_Masked_All_Febs_Inclusive->Fill(Time_difference_ns);
                                GEM_Trigger_Three_Fold_1D_Noise_Masked_Feb[FEB_GEM]->Fill(Time_difference_ns);

                                GEM_Trigger_Three_Fold_2D_vs_Timeslice_Noise_Masked_All_Febs_Inclusive->Fill(iTimeSlice, Time_difference_ns);
                                GEM_Trigger_Three_Fold_2D_vs_Timeslice_Noise_Masked_Feb[FEB_GEM]->Fill(iTimeSlice, Time_difference_ns);

                                GEM_Trigger_Three_Fold_2D_vs_ADC_Noise_Masked_Feb[FEB_GEM]->Fill(ADC_Value, Time_difference_ns);
                                GEM_Trigger_Three_Fold_2D_vs_Feb_Noise_Masked->Fill(FEB_GEM, Time_difference_ns);
                                GEM_Trigger_Three_Fold_2D_vs_GEM_Channel_Noise_Masked_Feb[FEB_GEM]->Fill(FEB_CH, Time_difference_ns);
                            }
                            else
                            {
                                GEM_Trigger_Three_Fold_1D_Noise_Only_All_Febs_Inclusive->Fill(Time_difference_ns);
                                GEM_Trigger_Three_Fold_1D_Noise_Only_Feb[FEB_GEM]->Fill(Time_difference_ns);

                                GEM_Trigger_Three_Fold_2D_vs_Timeslice_Noise_Only_All_Febs_Inclusive->Fill(iTimeSlice, Time_difference_ns);
                                GEM_Trigger_Three_Fold_2D_vs_Timeslice_Noise_Only_Feb[FEB_GEM]->Fill(iTimeSlice, Time_difference_ns);

                                GEM_Trigger_Three_Fold_2D_vs_ADC_Noise_Only_Feb[FEB_GEM]->Fill(ADC_Value, Time_difference_ns);
                                GEM_Trigger_Three_Fold_2D_vs_Feb_Noise_Only->Fill(FEB_GEM, Time_difference_ns);
                                GEM_Trigger_Three_Fold_2D_vs_GEM_Channel_Noise_Only_Feb[FEB_GEM]->Fill(FEB_CH, Time_difference_ns);
                            }
                        }
                    }
                }
                else
                {
                    if (Time_difference < -12.0)
                        break;
                }
            }
        }
    }

    fOut->mkdir("Time Correlations");
    fOut->mkdir("Time Correlations/GEM-Trigger");
    fOut->mkdir("Time Correlations/GEM-Trigger/Two Fold");
    fOut->mkdir("Time Correlations/GEM-Trigger/Two Fold/1D");
    fOut->mkdir("Time Correlations/GEM-Trigger/Two Fold/1D/Raw");
    fOut->mkdir("Time Correlations/GEM-Trigger/Two Fold/1D/Raw/All Febs Inclusive");
    fOut->mkdir("Time Correlations/GEM-Trigger/Two Fold/1D/Raw/Febwise");
    fOut->mkdir("Time Correlations/GEM-Trigger/Two Fold/1D/Noise Masked");
    fOut->mkdir("Time Correlations/GEM-Trigger/Two Fold/1D/Noise Masked/All Febs Inclusive");
    fOut->mkdir("Time Correlations/GEM-Trigger/Two Fold/1D/Noise Masked/Febwise");
    fOut->mkdir("Time Correlations/GEM-Trigger/Two Fold/1D/Noise Only");
    fOut->mkdir("Time Correlations/GEM-Trigger/Two Fold/1D/Noise Only/All Febs Inclusive");
    fOut->mkdir("Time Correlations/GEM-Trigger/Two Fold/1D/Noise Only/Febwise");
    fOut->mkdir("Time Correlations/GEM-Trigger/Two Fold/2D");
    fOut->mkdir("Time Correlations/GEM-Trigger/Two Fold/2D/Versus Timeslice");
    fOut->mkdir("Time Correlations/GEM-Trigger/Two Fold/2D/Versus Timeslice/Raw");
    fOut->mkdir("Time Correlations/GEM-Trigger/Two Fold/2D/Versus Timeslice/Raw/All Febs Inclusive");
    fOut->mkdir("Time Correlations/GEM-Trigger/Two Fold/2D/Versus Timeslice/Raw/Febwise");
    fOut->mkdir("Time Correlations/GEM-Trigger/Two Fold/2D/Versus Timeslice/Noise Masked");
    fOut->mkdir("Time Correlations/GEM-Trigger/Two Fold/2D/Versus Timeslice/Noise Masked/All Febs Inclusive");
    fOut->mkdir("Time Correlations/GEM-Trigger/Two Fold/2D/Versus Timeslice/Noise Masked/Febwise");
    fOut->mkdir("Time Correlations/GEM-Trigger/Two Fold/2D/Versus Timeslice/Noise Only");
    fOut->mkdir("Time Correlations/GEM-Trigger/Two Fold/2D/Versus Timeslice/Noise Only/All Febs Inclusive");
    fOut->mkdir("Time Correlations/GEM-Trigger/Two Fold/2D/Versus Timeslice/Noise Only/Febwise");
    fOut->mkdir("Time Correlations/GEM-Trigger/Two Fold/2D/Versus ADC");
    fOut->mkdir("Time Correlations/GEM-Trigger/Two Fold/2D/Versus ADC/Raw");
    fOut->mkdir("Time Correlations/GEM-Trigger/Two Fold/2D/Versus ADC/Raw/Febwise");
    fOut->mkdir("Time Correlations/GEM-Trigger/Two Fold/2D/Versus ADC/Noise Masked");
    fOut->mkdir("Time Correlations/GEM-Trigger/Two Fold/2D/Versus ADC/Noise Masked/Febwise");
    fOut->mkdir("Time Correlations/GEM-Trigger/Two Fold/2D/Versus ADC/Noise Only");
    fOut->mkdir("Time Correlations/GEM-Trigger/Two Fold/2D/Versus ADC/Noise Only/Febwise");
    fOut->mkdir("Time Correlations/GEM-Trigger/Two Fold/2D/Versus Feb Number");
    fOut->mkdir("Time Correlations/GEM-Trigger/Two Fold/2D/Versus Feb Number/Raw");
    fOut->mkdir("Time Correlations/GEM-Trigger/Two Fold/2D/Versus Feb Number/Noise Masked");
    fOut->mkdir("Time Correlations/GEM-Trigger/Two Fold/2D/Versus Feb Number/Noise Only");
    fOut->mkdir("Time Correlations/GEM-Trigger/Two Fold/2D/Versus Channel Number");
    fOut->mkdir("Time Correlations/GEM-Trigger/Two Fold/2D/Versus Channel Number/Raw");
    fOut->mkdir("Time Correlations/GEM-Trigger/Two Fold/2D/Versus Channel Number/Raw/Febwise");
    fOut->mkdir("Time Correlations/GEM-Trigger/Two Fold/2D/Versus Channel Number/Noise Masked");
    fOut->mkdir("Time Correlations/GEM-Trigger/Two Fold/2D/Versus Channel Number/Noise Masked/Febwise");
    fOut->mkdir("Time Correlations/GEM-Trigger/Two Fold/2D/Versus Channel Number/Noise Only");
    fOut->mkdir("Time Correlations/GEM-Trigger/Two Fold/2D/Versus Channel Number/Noise Only/Febwise");
    fOut->mkdir("Time Correlations/GEM-Trigger/Three Fold");
    fOut->mkdir("Time Correlations/GEM-Trigger/Three Fold/1D");
    fOut->mkdir("Time Correlations/GEM-Trigger/Three Fold/1D/Raw");
    fOut->mkdir("Time Correlations/GEM-Trigger/Three Fold/1D/Raw/All Febs Inclusive");
    fOut->mkdir("Time Correlations/GEM-Trigger/Three Fold/1D/Raw/Febwise");
    fOut->mkdir("Time Correlations/GEM-Trigger/Three Fold/1D/Noise Masked");
    fOut->mkdir("Time Correlations/GEM-Trigger/Three Fold/1D/Noise Masked/All Febs Inclusive");
    fOut->mkdir("Time Correlations/GEM-Trigger/Three Fold/1D/Noise Masked/Febwise");
    fOut->mkdir("Time Correlations/GEM-Trigger/Three Fold/1D/Noise Only");
    fOut->mkdir("Time Correlations/GEM-Trigger/Three Fold/1D/Noise Only/All Febs Inclusive");
    fOut->mkdir("Time Correlations/GEM-Trigger/Three Fold/1D/Noise Only/Febwise");
    fOut->mkdir("Time Correlations/GEM-Trigger/Three Fold/2D");
    fOut->mkdir("Time Correlations/GEM-Trigger/Three Fold/2D/Versus Timeslice");
    fOut->mkdir("Time Correlations/GEM-Trigger/Three Fold/2D/Versus Timeslice/Raw");
    fOut->mkdir("Time Correlations/GEM-Trigger/Three Fold/2D/Versus Timeslice/Raw/All Febs Inclusive");
    fOut->mkdir("Time Correlations/GEM-Trigger/Three Fold/2D/Versus Timeslice/Raw/Febwise");
    fOut->mkdir("Time Correlations/GEM-Trigger/Three Fold/2D/Versus Timeslice/Noise Masked");
    fOut->mkdir("Time Correlations/GEM-Trigger/Three Fold/2D/Versus Timeslice/Noise Masked/All Febs Inclusive");
    fOut->mkdir("Time Correlations/GEM-Trigger/Three Fold/2D/Versus Timeslice/Noise Masked/Febwise");
    fOut->mkdir("Time Correlations/GEM-Trigger/Three Fold/2D/Versus Timeslice/Noise Only");
    fOut->mkdir("Time Correlations/GEM-Trigger/Three Fold/2D/Versus Timeslice/Noise Only/All Febs Inclusive");
    fOut->mkdir("Time Correlations/GEM-Trigger/Three Fold/2D/Versus Timeslice/Noise Only/Febwise");
    fOut->mkdir("Time Correlations/GEM-Trigger/Three Fold/2D/Versus ADC");
    fOut->mkdir("Time Correlations/GEM-Trigger/Three Fold/2D/Versus ADC/Raw");
    fOut->mkdir("Time Correlations/GEM-Trigger/Three Fold/2D/Versus ADC/Raw/Febwise");
    fOut->mkdir("Time Correlations/GEM-Trigger/Three Fold/2D/Versus ADC/Noise Masked");
    fOut->mkdir("Time Correlations/GEM-Trigger/Three Fold/2D/Versus ADC/Noise Masked/Febwise");
    fOut->mkdir("Time Correlations/GEM-Trigger/Three Fold/2D/Versus ADC/Noise Only");
    fOut->mkdir("Time Correlations/GEM-Trigger/Three Fold/2D/Versus ADC/Noise Only/Febwise");
    fOut->mkdir("Time Correlations/GEM-Trigger/Three Fold/2D/Versus Feb Number");
    fOut->mkdir("Time Correlations/GEM-Trigger/Three Fold/2D/Versus Feb Number/Raw");
    fOut->mkdir("Time Correlations/GEM-Trigger/Three Fold/2D/Versus Feb Number/Noise Masked");
    fOut->mkdir("Time Correlations/GEM-Trigger/Three Fold/2D/Versus Feb Number/Noise Only");
    fOut->mkdir("Time Correlations/GEM-Trigger/Three Fold/2D/Versus Channel Number");
    fOut->mkdir("Time Correlations/GEM-Trigger/Three Fold/2D/Versus Channel Number/Raw");
    fOut->mkdir("Time Correlations/GEM-Trigger/Three Fold/2D/Versus Channel Number/Raw/Febwise");
    fOut->mkdir("Time Correlations/GEM-Trigger/Three Fold/2D/Versus Channel Number/Noise Masked");
    fOut->mkdir("Time Correlations/GEM-Trigger/Three Fold/2D/Versus Channel Number/Noise Masked/Febwise");
    fOut->mkdir("Time Correlations/GEM-Trigger/Three Fold/2D/Versus Channel Number/Noise Only");
    fOut->mkdir("Time Correlations/GEM-Trigger/Three Fold/2D/Versus Channel Number/Noise Only/Febwise");

    fOut->cd("Time Correlations/GEM-Trigger/Two Fold/1D/Raw/All Febs Inclusive");
    GEM_Trigger_Two_Fold_1D_Raw_All_Febs_Inclusive->Write();

    fOut->cd("Time Correlations/GEM-Trigger/Two Fold/1D/Raw/Febwise");
    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
        GEM_Trigger_Two_Fold_1D_Raw_Feb[iFeb]->Write();

    fOut->cd("Time Correlations/GEM-Trigger/Two Fold/1D/Noise Masked/All Febs Inclusive");
    GEM_Trigger_Two_Fold_1D_Noise_Masked_All_Febs_Inclusive->Write();

    fOut->cd("Time Correlations/GEM-Trigger/Two Fold/1D/Noise Masked/Febwise");
    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
        GEM_Trigger_Two_Fold_1D_Noise_Masked_Feb[iFeb]->Write();

    fOut->cd("Time Correlations/GEM-Trigger/Two Fold/1D/Noise Only/All Febs Inclusive");
    GEM_Trigger_Two_Fold_1D_Noise_Only_All_Febs_Inclusive->Write();

    fOut->cd("Time Correlations/GEM-Trigger/Two Fold/1D/Noise Only/Febwise");
    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
        GEM_Trigger_Two_Fold_1D_Noise_Only_Feb[iFeb]->Write();

    fOut->cd("Time Correlations/GEM-Trigger/Two Fold/2D/Versus Timeslice/Raw/All Febs Inclusive");
    GEM_Trigger_Two_Fold_2D_vs_Timeslice_Raw_All_Febs_Inclusive->Write();

    fOut->cd("Time Correlations/GEM-Trigger/Two Fold/2D/Versus Timeslice/Raw/Febwise");
    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
        GEM_Trigger_Two_Fold_2D_vs_Timeslice_Raw_Feb[iFeb]->Write();

    fOut->cd("Time Correlations/GEM-Trigger/Two Fold/2D/Versus Timeslice/Noise Masked/All Febs Inclusive");
    GEM_Trigger_Two_Fold_2D_vs_Timeslice_Noise_Masked_All_Febs_Inclusive->Write();

    fOut->cd("Time Correlations/GEM-Trigger/Two Fold/2D/Versus Timeslice/Noise Masked/Febwise");
    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
        GEM_Trigger_Two_Fold_2D_vs_Timeslice_Noise_Masked_Feb[iFeb]->Write();

    fOut->cd("Time Correlations/GEM-Trigger/Two Fold/2D/Versus Timeslice/Noise Only/All Febs Inclusive");
    GEM_Trigger_Two_Fold_2D_vs_Timeslice_Noise_Only_All_Febs_Inclusive->Write();

    fOut->cd("Time Correlations/GEM-Trigger/Two Fold/2D/Versus Timeslice/Noise Only/Febwise");
    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
        GEM_Trigger_Two_Fold_2D_vs_Timeslice_Noise_Only_Feb[iFeb]->Write();

    fOut->cd("Time Correlations/GEM-Trigger/Two Fold/2D/Versus ADC/Raw/Febwise");
    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
        GEM_Trigger_Two_Fold_2D_vs_ADC_Raw_Feb[iFeb]->Write();

    fOut->cd("Time Correlations/GEM-Trigger/Two Fold/2D/Versus ADC/Noise Masked/Febwise");
    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
        GEM_Trigger_Two_Fold_2D_vs_ADC_Noise_Masked_Feb[iFeb]->Write();

    fOut->cd("Time Correlations/GEM-Trigger/Two Fold/2D/Versus ADC/Noise Only/Febwise");
    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
        GEM_Trigger_Two_Fold_2D_vs_ADC_Noise_Only_Feb[iFeb]->Write();

    fOut->cd("Time Correlations/GEM-Trigger/Two Fold/2D/Versus Feb Number/Raw");
    GEM_Trigger_Two_Fold_2D_vs_Feb_Raw->Write();

    fOut->cd("Time Correlations/GEM-Trigger/Two Fold/2D/Versus Feb Number/Noise Masked");
    GEM_Trigger_Two_Fold_2D_vs_Feb_Noise_Masked->Write();

    fOut->cd("Time Correlations/GEM-Trigger/Two Fold/2D/Versus Feb Number/Noise Only");
    GEM_Trigger_Two_Fold_2D_vs_Feb_Noise_Only->Write();

    fOut->cd("Time Correlations/GEM-Trigger/Two Fold/2D/Versus Channel Number/Raw/Febwise");
    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
        GEM_Trigger_Two_Fold_2D_vs_GEM_Channel_Raw_Feb[iFeb]->Write();

    fOut->cd("Time Correlations/GEM-Trigger/Two Fold/2D/Versus Channel Number/Noise Masked/Febwise");
    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
        GEM_Trigger_Two_Fold_2D_vs_GEM_Channel_Noise_Masked_Feb[iFeb]->Write();

    fOut->cd("Time Correlations/GEM-Trigger/Two Fold/2D/Versus Channel Number/Noise Only/Febwise");
    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
        GEM_Trigger_Two_Fold_2D_vs_GEM_Channel_Noise_Only_Feb[iFeb]->Write();

    fOut->cd("Time Correlations/GEM-Trigger/Three Fold/1D/Raw/All Febs Inclusive");
    GEM_Trigger_Three_Fold_1D_Raw_All_Febs_Inclusive->Write();

    fOut->cd("Time Correlations/GEM-Trigger/Three Fold/1D/Raw/Febwise");
    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
        GEM_Trigger_Three_Fold_1D_Raw_Feb[iFeb]->Write();

    fOut->cd("Time Correlations/GEM-Trigger/Three Fold/1D/Noise Masked/All Febs Inclusive");
    GEM_Trigger_Three_Fold_1D_Noise_Masked_All_Febs_Inclusive->Write();

    fOut->cd("Time Correlations/GEM-Trigger/Three Fold/1D/Noise Masked/Febwise");
    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
        GEM_Trigger_Three_Fold_1D_Noise_Masked_Feb[iFeb]->Write();

    fOut->cd("Time Correlations/GEM-Trigger/Three Fold/1D/Noise Only/All Febs Inclusive");
    GEM_Trigger_Three_Fold_1D_Noise_Only_All_Febs_Inclusive->Write();

    fOut->cd("Time Correlations/GEM-Trigger/Three Fold/1D/Noise Only/Febwise");
    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
        GEM_Trigger_Three_Fold_1D_Noise_Only_Feb[iFeb]->Write();

    fOut->cd("Time Correlations/GEM-Trigger/Three Fold/2D/Versus Timeslice/Raw/All Febs Inclusive");
    GEM_Trigger_Three_Fold_2D_vs_Timeslice_Raw_All_Febs_Inclusive->Write();

    fOut->cd("Time Correlations/GEM-Trigger/Three Fold/2D/Versus Timeslice/Raw/Febwise");
    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
        GEM_Trigger_Three_Fold_2D_vs_Timeslice_Raw_Feb[iFeb]->Write();

    fOut->cd("Time Correlations/GEM-Trigger/Three Fold/2D/Versus Timeslice/Noise Masked/All Febs Inclusive");
    GEM_Trigger_Three_Fold_2D_vs_Timeslice_Noise_Masked_All_Febs_Inclusive->Write();

    fOut->cd("Time Correlations/GEM-Trigger/Three Fold/2D/Versus Timeslice/Noise Masked/Febwise");
    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
        GEM_Trigger_Three_Fold_2D_vs_Timeslice_Noise_Masked_Feb[iFeb]->Write();

    fOut->cd("Time Correlations/GEM-Trigger/Three Fold/2D/Versus Timeslice/Noise Only/All Febs Inclusive");
    GEM_Trigger_Three_Fold_2D_vs_Timeslice_Noise_Only_All_Febs_Inclusive->Write();

    fOut->cd("Time Correlations/GEM-Trigger/Three Fold/2D/Versus Timeslice/Noise Only/Febwise");
    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
        GEM_Trigger_Three_Fold_2D_vs_Timeslice_Noise_Only_Feb[iFeb]->Write();

    fOut->cd("Time Correlations/GEM-Trigger/Three Fold/2D/Versus ADC/Raw/Febwise");
    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
        GEM_Trigger_Three_Fold_2D_vs_ADC_Raw_Feb[iFeb]->Write();

    fOut->cd("Time Correlations/GEM-Trigger/Three Fold/2D/Versus ADC/Noise Masked/Febwise");
    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
        GEM_Trigger_Three_Fold_2D_vs_ADC_Noise_Masked_Feb[iFeb]->Write();

    fOut->cd("Time Correlations/GEM-Trigger/Three Fold/2D/Versus ADC/Noise Only/Febwise");
    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
        GEM_Trigger_Three_Fold_2D_vs_ADC_Noise_Only_Feb[iFeb]->Write();

    fOut->cd("Time Correlations/GEM-Trigger/Three Fold/2D/Versus Feb Number/Raw");
    GEM_Trigger_Three_Fold_2D_vs_Feb_Raw->Write();

    fOut->cd("Time Correlations/GEM-Trigger/Three Fold/2D/Versus Feb Number/Noise Masked");
    GEM_Trigger_Three_Fold_2D_vs_Feb_Noise_Masked->Write();

    fOut->cd("Time Correlations/GEM-Trigger/Three Fold/2D/Versus Feb Number/Noise Only");
    GEM_Trigger_Three_Fold_2D_vs_Feb_Noise_Only->Write();

    fOut->cd("Time Correlations/GEM-Trigger/Three Fold/2D/Versus Channel Number/Raw/Febwise");
    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
        GEM_Trigger_Three_Fold_2D_vs_GEM_Channel_Raw_Feb[iFeb]->Write();

    fOut->cd("Time Correlations/GEM-Trigger/Three Fold/2D/Versus Channel Number/Noise Masked/Febwise");
    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
        GEM_Trigger_Three_Fold_2D_vs_GEM_Channel_Noise_Masked_Feb[iFeb]->Write();

    fOut->cd("Time Correlations/GEM-Trigger/Three Fold/2D/Versus Channel Number/Noise Only/Febwise");
    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
        GEM_Trigger_Three_Fold_2D_vs_GEM_Channel_Noise_Only_Feb[iFeb]->Write();

    fIn->Close();
    fOut->Close();

    cout << "Output File Name: " << OutputFileName << endl;

    Code_Timer.Stop();

    cout << "_______________________Code Executed Successfully!________________________" << endl;
    cout << "Real time: " << Code_Timer.RealTime() / 60.0 << " m, CPU time: " << Code_Timer.CpuTime() / 60.0 << " m" << endl;
}
