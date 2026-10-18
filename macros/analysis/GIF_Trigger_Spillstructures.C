// Copyright (C) Experimental High Energy Physics and Applications Group, VECC, Kolkata
// Author(s): Apar Agarwal

// This macro has been created for the purpose
// analysing various kinds of spill structures
// of GEM without trigger for GIF++ 2023 data

// This Code requires the CbmRoot version DEC 21 or higher
// Recommended on Linux 20.04 --> Fairsoftversion : APR21p2 and FAIRROOTVERSION: v18.6.7 or higher

// Code first tested on 5th October, 2023

void GIF_Trigger_Spillstructures(Int_t RunId = 966)
{
    TString InputFileName = "Run" + to_string(RunId) + "_Noise_Removed_Ringing_Walk_Corrected.digi.root";

    if (gSystem->AccessPathName(InputFileName))
    {
        cout << "The file: " << InputFileName << " does not exist!" << endl;
        return;
    }

    TFile* fIn = new TFile(InputFileName);
    TTree* Input_Tree = (TTree*)fIn->Get("cbmsim");

    TString OutputFileName = "data/Spill_Structures/Run" + to_string(RunId) + "_Trigger_Spillstructures_corrected.root";
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
    Int_t Two_Fold_Lower_Bound = 0;
    Int_t Two_Fold_Upper_Bound = 64;
    Int_t Three_Fold_Lower_Bound = 64;
    Int_t Three_Fold_Upper_Bound = 128;
    uint64_t First_Digi_Time;
    uint64_t Last_Digi_Time;
    TString HistTitle, HistTag;

    HistTag = "Trigger_Two_Fold_Time_Based_1D";
    HistTitle = "Two Fold Trigger 1D Spillstructure [Time based] for Run " + to_string(RunId);
    TH1I* Trigger_Two_Fold_Time_Based_1D = new TH1I(HistTag, HistTitle, 128 * (nTimeSlices + 11), -128.0, 128.0 * (nTimeSlices + 10.0));
    Trigger_Two_Fold_Time_Based_1D->GetXaxis()->SetTitle("Time (ms)");
    Trigger_Two_Fold_Time_Based_1D->GetYaxis()->SetTitle("Counts");

    HistTag = "Trigger_Two_Fold_Timeslice_Based_1D";
    HistTitle = "Two Fold Trigger 1D Spillstructure [Timeslice based] for Run " + to_string(RunId);
    TH1I* Trigger_Two_Fold_Timeslice_Based_1D = new TH1I(HistTag, HistTitle, nTimeSlices + 10, -1.5, nTimeSlices + 8.5);
    Trigger_Two_Fold_Timeslice_Based_1D->GetXaxis()->SetTitle("Timeslice number");
    Trigger_Two_Fold_Timeslice_Based_1D->GetYaxis()->SetTitle("Counts");

    HistTag = "Trigger_Two_Fold_Timeslice_Based_2D_vs_ADC";
    HistTitle = "Two Fold Trigger vs ADC 2D Spillstructure [Timeslice based] for Run " + to_string(RunId);
    TH2F* Trigger_Two_Fold_Timeslice_Based_2D_vs_ADC = new TH2F(HistTag, HistTitle, nTimeSlices + 10, -1.5, nTimeSlices + 8.5, 35, -1.5, 33.5);
    Trigger_Two_Fold_Timeslice_Based_2D_vs_ADC->GetXaxis()->SetTitle("Timeslice number");
    Trigger_Two_Fold_Timeslice_Based_2D_vs_ADC->GetYaxis()->SetTitle("ADC");

    HistTag = "Trigger_Two_Fold_Timeslice_Based_2D_vs_Channel";
    HistTitle = "Two Fold Trigger vs Channel 2D Spillstructure [Timeslice based] for Run " + to_string(RunId);
    TH2F* Trigger_Two_Fold_Timeslice_Based_2D_vs_Channel = new TH2F(HistTag, HistTitle, nTimeSlices + 10, -1.5, nTimeSlices + 8.5, 131, -1.5, 129.5);
    Trigger_Two_Fold_Timeslice_Based_2D_vs_Channel->GetXaxis()->SetTitle("Timeslice number");
    Trigger_Two_Fold_Timeslice_Based_2D_vs_Channel->GetYaxis()->SetTitle("Channel number");

    HistTag = "Trigger_Three_Fold_Time_Based_1D";
    HistTitle = "Three Fold Trigger 1D Spillstructure [Time based] for Run " + to_string(RunId);
    TH1I* Trigger_Three_Fold_Time_Based_1D = new TH1I(HistTag, HistTitle, 128 * (nTimeSlices + 11), -128.0, 128.0 * (nTimeSlices + 10.0));
    Trigger_Three_Fold_Time_Based_1D->GetXaxis()->SetTitle("Time (ms)");
    Trigger_Three_Fold_Time_Based_1D->GetYaxis()->SetTitle("Counts");

    HistTag = "Trigger_Three_Fold_Timeslice_Based_1D";
    HistTitle = "Three Fold Trigger 1D Spillstructure [Timeslice based] for Run " + to_string(RunId);
    TH1I* Trigger_Three_Fold_Timeslice_Based_1D = new TH1I(HistTag, HistTitle, nTimeSlices + 10, -1.5, nTimeSlices + 8.5);
    Trigger_Three_Fold_Timeslice_Based_1D->GetXaxis()->SetTitle("Timeslice number");
    Trigger_Three_Fold_Timeslice_Based_1D->GetYaxis()->SetTitle("Counts");

    HistTag = "Trigger_Three_Fold_Timeslice_Based_2D_vs_ADC";
    HistTitle = "Three Fold Trigger vs ADC 2D Spillstructure [Timeslice based] for Run " + to_string(RunId);
    TH2F* Trigger_Three_Fold_Timeslice_Based_2D_vs_ADC = new TH2F(HistTag, HistTitle, nTimeSlices + 10, -1.5, nTimeSlices + 8.5, 35, -1.5, 33.5);
    Trigger_Three_Fold_Timeslice_Based_2D_vs_ADC->GetXaxis()->SetTitle("Timeslice number");
    Trigger_Three_Fold_Timeslice_Based_2D_vs_ADC->GetYaxis()->SetTitle("ADC");

    HistTag = "Trigger_Three_Fold_Timeslice_Based_2D_vs_Channel";
    HistTitle = "Three Fold Trigger vs Channel 2D Spillstructure [Timeslice based] for Run " + to_string(RunId);
    TH2F* Trigger_Three_Fold_Timeslice_Based_2D_vs_Channel = new TH2F(HistTag, HistTitle, nTimeSlices + 10, -1.5, nTimeSlices + 8.5, 131, -1.5, 129.5);
    Trigger_Three_Fold_Timeslice_Based_2D_vs_Channel->GetXaxis()->SetTitle("Timeslice number");
    Trigger_Three_Fold_Timeslice_Based_2D_vs_Channel->GetYaxis()->SetTitle("Channel number");


    HistTag = "All_Trigger_Time_Based_1D";
    HistTitle = "All Trigger 1D Spillstructure [Time based] for Run " + to_string(RunId);
    TH1I* All_Trigger_Time_Based_1D = new TH1I(HistTag, HistTitle, 128 * (nTimeSlices + 11), -128.0, 128.0 * (nTimeSlices + 10.0));
    All_Trigger_Time_Based_1D->GetXaxis()->SetTitle("Time (ms)");
    All_Trigger_Time_Based_1D->GetYaxis()->SetTitle("Counts");

    HistTag = "All_Trigger_Timeslice_Based_1D";
    HistTitle = "All Trigger 1D Spillstructure [Timeslice based] for Run " + to_string(RunId);
    TH1I* All_Trigger_Timeslice_Based_1D = new TH1I(HistTag, HistTitle, nTimeSlices + 10, -1.5, nTimeSlices + 8.5);
    All_Trigger_Timeslice_Based_1D->GetXaxis()->SetTitle("Timeslice number");
    All_Trigger_Timeslice_Based_1D->GetYaxis()->SetTitle("Counts");

    HistTag = "All_Trigger_Timeslice_Based_2D_vs_ADC";
    HistTitle = "All Trigger vs ADC 2D Spillstructure [Timeslice based] for Run " + to_string(RunId);
    TH2F* All_Trigger_Timeslice_Based_2D_vs_ADC = new TH2F(HistTag, HistTitle, nTimeSlices + 10, -1.5, nTimeSlices + 8.5, 35, -1.5, 33.5);
    All_Trigger_Timeslice_Based_2D_vs_ADC->GetXaxis()->SetTitle("Timeslice number");
    All_Trigger_Timeslice_Based_2D_vs_ADC->GetYaxis()->SetTitle("ADC");

    HistTag = "All_Trigger_Timeslice_Based_2D_vs_Channel";
    HistTitle = "All Trigger vs Channel 2D Spillstructure [Timeslice based] for Run " + to_string(RunId);
    TH2F* All_Trigger_Timeslice_Based_2D_vs_Channel = new TH2F(HistTag, HistTitle, nTimeSlices + 10, -1.5, nTimeSlices + 8.5, 131, -1.5, 129.5);
    All_Trigger_Timeslice_Based_2D_vs_Channel->GetXaxis()->SetTitle("Timeslice number");
    All_Trigger_Timeslice_Based_2D_vs_Channel->GetYaxis()->SetTitle("Channel number");

    for (iTreeEntry = 0; iTreeEntry < 200; iTreeEntry++)
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

    // for(iTreeEntry = 0; iTreeEntry < 100 ;iTreeEntry++)
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
            int32_t Station = CbmMuchAddress::GetStationIndex(Much_Digi[iMuchDigi].GetAddress());
            if (Station != 1)
                continue; // Selecting Only Trigger
            int32_t Channel = CbmMuchAddress::GetChannelIndex(Much_Digi[iMuchDigi].GetAddress());

            uint64_t Trigger_Digi_Time = TimeSlice_Start_Time + (uint64_t)Much_Digi[iMuchDigi].GetTime() - First_Digi_Time;

            Double_t Trigger_Digi_Time_ms = Trigger_Digi_Time / 1000000.0;
            Int_t ADC_Value = Much_Digi[iMuchDigi].GetAdc();

            All_Trigger_Time_Based_1D->Fill(Trigger_Digi_Time_ms);
            All_Trigger_Timeslice_Based_1D->Fill(iTimeSlice);
            All_Trigger_Timeslice_Based_2D_vs_ADC->Fill(iTimeSlice, ADC_Value);
            All_Trigger_Timeslice_Based_2D_vs_Channel->Fill(iTimeSlice, Channel);

            if (Channel >= Two_Fold_Lower_Bound && Channel < Two_Fold_Upper_Bound) //2-FOLD Coincidence only
            {
                Trigger_Two_Fold_Time_Based_1D->Fill(Trigger_Digi_Time_ms);
                Trigger_Two_Fold_Timeslice_Based_1D->Fill(iTimeSlice);
                Trigger_Two_Fold_Timeslice_Based_2D_vs_ADC->Fill(iTimeSlice, ADC_Value);
                Trigger_Two_Fold_Timeslice_Based_2D_vs_Channel->Fill(iTimeSlice, Channel);
            }
            else if (Channel >= Three_Fold_Lower_Bound && Channel < Three_Fold_Upper_Bound) //3-FOLD Coincidence only
            {
                Trigger_Three_Fold_Time_Based_1D->Fill(Trigger_Digi_Time_ms);
                Trigger_Three_Fold_Timeslice_Based_1D->Fill(iTimeSlice);
                Trigger_Three_Fold_Timeslice_Based_2D_vs_ADC->Fill(iTimeSlice, ADC_Value);
                Trigger_Three_Fold_Timeslice_Based_2D_vs_Channel->Fill(iTimeSlice, Channel);
            }
        }
    }

    fOut->mkdir("Spill Structures");
    fOut->mkdir("Spill Structures/Trigger");
    fOut->mkdir("Spill Structures/Trigger/Two Fold");
    fOut->mkdir("Spill Structures/Trigger/Two Fold/Time Based");
    fOut->mkdir("Spill Structures/Trigger/Two Fold/Time Based/1D");
    fOut->mkdir("Spill Structures/Trigger/Two Fold/Timeslice Based");
    fOut->mkdir("Spill Structures/Trigger/Two Fold/Timeslice Based/1D");
    fOut->mkdir("Spill Structures/Trigger/Two Fold/Timeslice Based/2D");
    fOut->mkdir("Spill Structures/Trigger/Two Fold/Timeslice Based/2D/vs ADC");
    fOut->mkdir("Spill Structures/Trigger/Two Fold/Timeslice Based/2D/vs Channel");
    fOut->mkdir("Spill Structures/Trigger/Three Fold");
    fOut->mkdir("Spill Structures/Trigger/Three Fold/Time Based");
    fOut->mkdir("Spill Structures/Trigger/Three Fold/Time Based/1D");
    fOut->mkdir("Spill Structures/Trigger/Three Fold/Timeslice Based");
    fOut->mkdir("Spill Structures/Trigger/Three Fold/Timeslice Based/1D");
    fOut->mkdir("Spill Structures/Trigger/Three Fold/Timeslice Based/2D");
    fOut->mkdir("Spill Structures/Trigger/Three Fold/Timeslice Based/2D/vs ADC");
    fOut->mkdir("Spill Structures/Trigger/Three Fold/Timeslice Based/2D/vs Channel");
    fOut->mkdir("Spill Structures/Trigger/All");
    fOut->mkdir("Spill Structures/Trigger/All/Time Based");
    fOut->mkdir("Spill Structures/Trigger/All/Time Based/1D");
    fOut->mkdir("Spill Structures/Trigger/All/Timeslice Based");
    fOut->mkdir("Spill Structures/Trigger/All/Timeslice Based/1D");
    fOut->mkdir("Spill Structures/Trigger/All/Timeslice Based/2D");
    fOut->mkdir("Spill Structures/Trigger/All/Timeslice Based/2D/vs ADC");
    fOut->mkdir("Spill Structures/Trigger/All/Timeslice Based/2D/vs Channel");

    fOut->cd("Spill Structures/Trigger/Two Fold/Time Based/1D");
    Trigger_Two_Fold_Time_Based_1D->Write();

    fOut->cd("Spill Structures/Trigger/Two Fold/Timeslice Based/1D");
    Trigger_Two_Fold_Timeslice_Based_1D->Write();

    fOut->cd("Spill Structures/Trigger/Two Fold/Timeslice Based/2D/vs ADC");
    Trigger_Two_Fold_Timeslice_Based_2D_vs_ADC->Write();

    fOut->cd("Spill Structures/Trigger/Two Fold/Timeslice Based/2D/vs Channel");
    Trigger_Two_Fold_Timeslice_Based_2D_vs_Channel->Write();

    fOut->cd("Spill Structures/Trigger/Three Fold/Time Based/1D");
    Trigger_Three_Fold_Time_Based_1D->Write();

    fOut->cd("Spill Structures/Trigger/Three Fold/Timeslice Based/1D");
    Trigger_Three_Fold_Timeslice_Based_1D->Write();

    fOut->cd("Spill Structures/Trigger/Three Fold/Timeslice Based/2D/vs ADC");
    Trigger_Three_Fold_Timeslice_Based_2D_vs_ADC->Write();

    fOut->cd("Spill Structures/Trigger/Three Fold/Timeslice Based/2D/vs Channel");
    Trigger_Three_Fold_Timeslice_Based_2D_vs_Channel->Write();

    fOut->cd("Spill Structures/Trigger/All/Time Based/1D");
    All_Trigger_Time_Based_1D->Write();

    fOut->cd("Spill Structures/Trigger/All/Timeslice Based/1D");
    All_Trigger_Timeslice_Based_1D->Write();

    fOut->cd("Spill Structures/Trigger/All/Timeslice Based/2D/vs ADC");
    All_Trigger_Timeslice_Based_2D_vs_ADC->Write();

    fOut->cd("Spill Structures/Trigger/All/Timeslice Based/2D/vs Channel");
    All_Trigger_Timeslice_Based_2D_vs_Channel->Write();

    fIn->Close();
    fOut->Close();
}
