// Copyright (C) Experimental High Energy Physics and Applications Group, VECC, Kolkata
// Author(s): Apar Agarwal

// This macro has been created for the purpose
// analysing various kinds of spill structures
// of GEM with trigger for GIF++ 2023 data

// This Code requires the CbmRoot version DEC 21 or higher
// Recommended on Linux 20.04 --> Fairsoftversion : APR21p2 and FAIRROOTVERSION: v18.6.7 or higher

// Code first tested on 4th October, 2023

void GIF_GEM_With_Trigger_Spillstructures(Int_t RunId = 966)
{
    TString InputFileName = "Run" + to_string(RunId) + "_0001.digi.root";
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

    TString OutputFileName = "data/Run" + to_string(RunId) + "_GEM_Without_Trigger_Spillstructures.root";
    TFile* fOut = new TFile(OutputFileName, "recreate");

    TString Input_X_Mapping_GEM = "PadX_GEM.txt";
    TString Input_Y_Mapping_GEM = "PadY_GEM.txt";

    ifstream GEM_Infile_X;

    GEM_Infile_X.open(Input_X_Mapping_GEM);

    Int_t GEM_Coordinate_Feb_Channel[18][128][2] = {0}; //0 for X and 1 for Y coordinate

    for (Int_t i = 0; i < 18; i++)
    {
        for (Int_t j = 0; j < 128; j++)
        {
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

    //****************************** END: Reading Mapping for GEM ***********************************************************


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

    HistTag = "GEM_No_Trigger_Raw_Time_Based_1D_All_Febs_Inclusive";
    HistTitle = "GEM 1D Raw Spillstructure without trigger [Time based](All Febs inclusive)";
    TH1I* GEM_No_Trigger_Raw_Time_Based_1D_All_Febs_Inclusive = new TH1I(HistTag, HistTitle, 128 * (nTimeSlices + 11), -128.0, 128.0 * (nTimeSlices + 10.0));
    GEM_No_Trigger_Raw_Time_Based_1D_All_Febs_Inclusive->GetXaxis()->SetTitle("Time (ms)");
    GEM_No_Trigger_Raw_Time_Based_1D_All_Febs_Inclusive->GetYaxis()->SetTitle("Counts");

    TH1I* GEM_No_Trigger_Raw_Time_Based_1D_Feb[18];

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTag = "GEM_No_Trigger_Raw_Time_Based_1D_Feb_" + to_string(iFeb);
        HistTitle = "GEM Feb " + to_string(iFeb) + " 1D Raw Spillstructure without trigger [Time Based]";
        GEM_No_Trigger_Raw_Time_Based_1D_Feb[iFeb] = new TH1I(HistTag, HistTitle, 128 * (nTimeSlices + 11), -128.0, 128.0 * (nTimeSlices + 10.0));
        GEM_No_Trigger_Raw_Time_Based_1D_Feb[iFeb]->GetXaxis()->SetTitle("Time (ms)");
        GEM_No_Trigger_Raw_Time_Based_1D_Feb[iFeb]->GetYaxis()->SetTitle("Counts");
    }

    HistTag = "GEM_No_Trigger_Raw_Timeslice_Based_1D_All_Febs_Inclusive";
    HistTitle = "GEM 1D Raw Spillstructure without trigger [Time slice based] (All Febs inclusive)";
    TH1I* GEM_No_Trigger_Raw_Timeslice_Based_1D_All_Febs_Inclusive = new TH1I(HistTag, HistTitle, nTimeSlices + 10, -1.5, nTimeSlices + 8.5);
    GEM_No_Trigger_Raw_Timeslice_Based_1D_All_Febs_Inclusive->GetXaxis()->SetTitle("Timeslice number");
    GEM_No_Trigger_Raw_Timeslice_Based_1D_All_Febs_Inclusive->GetYaxis()->SetTitle("Counts");

    TH1I* GEM_No_Trigger_Raw_Timeslice_Based_1D_Feb[18];

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTag = "GEM_No_Trigger_Raw_Timeslice_Based_1D_Feb_" + to_string(iFeb);
        HistTitle = "GEM Feb " + to_string(iFeb) + " 1D Raw Spillstructure without trigger [Time slice Based]";
        GEM_No_Trigger_Raw_Timeslice_Based_1D_Feb[iFeb] = new TH1I(HistTag, HistTitle, nTimeSlices + 10, -1.5, nTimeSlices + 8.5);
        GEM_No_Trigger_Raw_Timeslice_Based_1D_Feb[iFeb]->GetXaxis()->SetTitle("Timeslice number");
        GEM_No_Trigger_Raw_Timeslice_Based_1D_Feb[iFeb]->GetYaxis()->SetTitle("Counts");
    }

    TH2F* GEM_No_Trigger_Raw_Timeslice_Based_2D_vs_ADC_Feb[18];

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTag = "GEM_No_Trigger_Raw_Timeslice_Based_2D_vs_ADC_Feb_" + to_string(iFeb);
        HistTitle = "GEM Feb " + to_string(iFeb) + " 2D Raw Spillstructure vs. ADC without trigger [Time slice Based]";
        GEM_No_Trigger_Raw_Timeslice_Based_2D_vs_ADC_Feb[iFeb] = new TH2F(HistTag, HistTitle, nTimeSlices + 10, -1.5, nTimeSlices + 8.5, 35, -1.5, 33.5);
        GEM_No_Trigger_Raw_Timeslice_Based_2D_vs_ADC_Feb[iFeb]->GetXaxis()->SetTitle("Timeslice number");
        GEM_No_Trigger_Raw_Timeslice_Based_2D_vs_ADC_Feb[iFeb]->GetYaxis()->SetTitle("ADC");
    }

    TH2F* GEM_No_Trigger_Raw_Timeslice_Based_2D_vs_Channel_Feb[18];

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTag = "GEM_No_Trigger_Raw_Timeslice_Based_2D_vs_Channel_Feb_" + to_string(iFeb);
        HistTitle = "GEM Feb " + to_string(iFeb) + " 2D Raw Spillstructure vs. Channel without trigger [Time slice Based]";
        GEM_No_Trigger_Raw_Timeslice_Based_2D_vs_Channel_Feb[iFeb] = new TH2F(HistTag, HistTitle, nTimeSlices + 10, -1.5, nTimeSlices + 8.5, 131, -1.5, 129.5);
        GEM_No_Trigger_Raw_Timeslice_Based_2D_vs_Channel_Feb[iFeb]->GetXaxis()->SetTitle("Timeslice number");
        GEM_No_Trigger_Raw_Timeslice_Based_2D_vs_Channel_Feb[iFeb]->GetYaxis()->SetTitle("Channel");
    }

    HistTag = "GEM_No_Trigger_Raw_Timeslice_Based_2D_vs_Feb_Number";
    HistTitle = "GEM 2D Raw Spillstructure vs. Feb Number without trigger [Time slice based] (All Febs inclusive)";
    TH2F* GEM_No_Trigger_Raw_Timeslice_Based_2D_vs_Feb_Number = new TH2F(HistTag, HistTitle, nTimeSlices + 10, -1.5, nTimeSlices + 8.5, 21, -1.5, 19.5);
    GEM_No_Trigger_Raw_Timeslice_Based_2D_vs_Feb_Number->GetXaxis()->SetTitle("Timeslice number");
    GEM_No_Trigger_Raw_Timeslice_Based_2D_vs_Feb_Number->GetYaxis()->SetTitle("Feb number");


    HistTag = "GEM_No_Trigger_Noise_Masked_Time_Based_1D_All_Febs_Inclusive";
    HistTitle = "GEM 1D Noise Masked Spillstructure without trigger [Time based](All Febs inclusive)";
    TH1I* GEM_No_Trigger_Noise_Masked_Time_Based_1D_All_Febs_Inclusive = new TH1I(HistTag, HistTitle, 128 * (nTimeSlices + 11), -128.0, 128.0 * (nTimeSlices + 10.0));
    GEM_No_Trigger_Noise_Masked_Time_Based_1D_All_Febs_Inclusive->GetXaxis()->SetTitle("Time (ms)");
    GEM_No_Trigger_Noise_Masked_Time_Based_1D_All_Febs_Inclusive->GetYaxis()->SetTitle("Counts");

    TH1I* GEM_No_Trigger_Noise_Masked_Time_Based_1D_Feb[18];

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTag = "GEM_No_Trigger_Noise_Masked_Time_Based_1D_Feb_" + to_string(iFeb);
        HistTitle = "GEM Feb " + to_string(iFeb) + " 1D Noise Masked Spillstructure without trigger [Time Based]";
        GEM_No_Trigger_Noise_Masked_Time_Based_1D_Feb[iFeb] = new TH1I(HistTag, HistTitle, 128 * (nTimeSlices + 11), -128.0, 128.0 * (nTimeSlices + 10.0));
        GEM_No_Trigger_Noise_Masked_Time_Based_1D_Feb[iFeb]->GetXaxis()->SetTitle("Time (ms)");
        GEM_No_Trigger_Noise_Masked_Time_Based_1D_Feb[iFeb]->GetYaxis()->SetTitle("Counts");
    }

    HistTag = "GEM_No_Trigger_Noise_Masked_Timeslice_Based_1D_All_Febs_Inclusive";
    HistTitle = "GEM 1D Noise Masked Spillstructure without trigger [Time slice based] (All Febs inclusive)";
    TH1I* GEM_No_Trigger_Noise_Masked_Timeslice_Based_1D_All_Febs_Inclusive = new TH1I(HistTag, HistTitle, nTimeSlices + 10, -1.5, nTimeSlices + 8.5);
    GEM_No_Trigger_Noise_Masked_Timeslice_Based_1D_All_Febs_Inclusive->GetXaxis()->SetTitle("Timeslice number");
    GEM_No_Trigger_Noise_Masked_Timeslice_Based_1D_All_Febs_Inclusive->GetYaxis()->SetTitle("Counts");

    TH1I* GEM_No_Trigger_Noise_Masked_Timeslice_Based_1D_Feb[18];

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTag = "GEM_No_Trigger_Noise_Masked_Timeslice_Based_1D_Feb_" + to_string(iFeb);
        HistTitle = "GEM Feb " + to_string(iFeb) + " 1D Noise Masked Spillstructure without trigger [Time slice Based]";
        GEM_No_Trigger_Noise_Masked_Timeslice_Based_1D_Feb[iFeb] = new TH1I(HistTag, HistTitle, nTimeSlices + 10, -1.5, nTimeSlices + 8.5);
        GEM_No_Trigger_Noise_Masked_Timeslice_Based_1D_Feb[iFeb]->GetXaxis()->SetTitle("Timeslice number");
        GEM_No_Trigger_Noise_Masked_Timeslice_Based_1D_Feb[iFeb]->GetYaxis()->SetTitle("Counts");
    }

    TH2F* GEM_No_Trigger_Noise_Masked_Timeslice_Based_2D_vs_ADC_Feb[18];

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTag = "GEM_No_Trigger_Noise_Masked_Timeslice_Based_2D_vs_ADC_Feb_" + to_string(iFeb);
        HistTitle = "GEM Feb " + to_string(iFeb) + " 2D Noise Masked Spillstructure vs. ADC without trigger [Time slice Based]";
        GEM_No_Trigger_Noise_Masked_Timeslice_Based_2D_vs_ADC_Feb[iFeb] = new TH2F(HistTag, HistTitle, nTimeSlices + 10, -1.5, nTimeSlices + 8.5, 35, -1.5, 33.5);
        GEM_No_Trigger_Noise_Masked_Timeslice_Based_2D_vs_ADC_Feb[iFeb]->GetXaxis()->SetTitle("Timeslice number");
        GEM_No_Trigger_Noise_Masked_Timeslice_Based_2D_vs_ADC_Feb[iFeb]->GetYaxis()->SetTitle("ADC");
    }

    TH2F* GEM_No_Trigger_Noise_Masked_Timeslice_Based_2D_vs_Channel_Feb[18];

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTag = "GEM_No_Trigger_Noise_Masked_Timeslice_Based_2D_vs_Channel_Feb_" + to_string(iFeb);
        HistTitle = "GEM Feb " + to_string(iFeb) + " 2D Noise Masked Spillstructure vs. Channel without trigger [Time slice Based]";
        GEM_No_Trigger_Noise_Masked_Timeslice_Based_2D_vs_Channel_Feb[iFeb] = new TH2F(HistTag, HistTitle, nTimeSlices + 10, -1.5, nTimeSlices + 8.5, 131, -1.5, 129.5);
        GEM_No_Trigger_Noise_Masked_Timeslice_Based_2D_vs_Channel_Feb[iFeb]->GetXaxis()->SetTitle("Timeslice number");
        GEM_No_Trigger_Noise_Masked_Timeslice_Based_2D_vs_Channel_Feb[iFeb]->GetYaxis()->SetTitle("Channel");
    }

    HistTag = "GEM_No_Trigger_Noise_Masked_Timeslice_Based_2D_vs_Feb_Number";
    HistTitle = "GEM 2D Noise Masked Spillstructure vs. Feb Number without trigger [Time slice based] (All Febs inclusive)";
    TH2F* GEM_No_Trigger_Noise_Masked_Timeslice_Based_2D_vs_Feb_Number = new TH2F(HistTag, HistTitle, nTimeSlices + 10, -1.5, nTimeSlices + 8.5, 21, -1.5, 19.5);
    GEM_No_Trigger_Noise_Masked_Timeslice_Based_2D_vs_Feb_Number->GetXaxis()->SetTitle("Timeslice number");
    GEM_No_Trigger_Noise_Masked_Timeslice_Based_2D_vs_Feb_Number->GetYaxis()->SetTitle("Feb number");


    HistTag = "GEM_No_Trigger_Noise_Only_Time_Based_1D_All_Febs_Inclusive";
    HistTitle = "GEM 1D Noise Only Spillstructure without trigger [Time based](All Febs inclusive)";
    TH1I* GEM_No_Trigger_Noise_Only_Time_Based_1D_All_Febs_Inclusive = new TH1I(HistTag, HistTitle, 128 * (nTimeSlices + 11), -128.0, 128.0 * (nTimeSlices + 10.0));
    GEM_No_Trigger_Noise_Only_Time_Based_1D_All_Febs_Inclusive->GetXaxis()->SetTitle("Time (ms)");
    GEM_No_Trigger_Noise_Only_Time_Based_1D_All_Febs_Inclusive->GetYaxis()->SetTitle("Counts");

    TH1I* GEM_No_Trigger_Noise_Only_Time_Based_1D_Feb[18];

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTag = "GEM_No_Trigger_Noise_Only_Time_Based_1D_Feb_" + to_string(iFeb);
        HistTitle = "GEM Feb " + to_string(iFeb) + " 1D Noise Only Spillstructure without trigger [Time Based]";
        GEM_No_Trigger_Noise_Only_Time_Based_1D_Feb[iFeb] = new TH1I(HistTag, HistTitle, 128 * (nTimeSlices + 11), -128.0, 128.0 * (nTimeSlices + 10.0));
        GEM_No_Trigger_Noise_Only_Time_Based_1D_Feb[iFeb]->GetXaxis()->SetTitle("Time (ms)");
        GEM_No_Trigger_Noise_Only_Time_Based_1D_Feb[iFeb]->GetYaxis()->SetTitle("Counts");
    }

    HistTag = "GEM_No_Trigger_Noise_Only_Timeslice_Based_1D_All_Febs_Inclusive";
    HistTitle = "GEM 1D Noise Only Spillstructure without trigger [Time slice based] (All Febs inclusive)";
    TH1I* GEM_No_Trigger_Noise_Only_Timeslice_Based_1D_All_Febs_Inclusive = new TH1I(HistTag, HistTitle, nTimeSlices + 10, -1.5, nTimeSlices + 8.5);
    GEM_No_Trigger_Noise_Only_Timeslice_Based_1D_All_Febs_Inclusive->GetXaxis()->SetTitle("Timeslice number");
    GEM_No_Trigger_Noise_Only_Timeslice_Based_1D_All_Febs_Inclusive->GetYaxis()->SetTitle("Counts");

    TH1I* GEM_No_Trigger_Noise_Only_Timeslice_Based_1D_Feb[18];

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTag = "GEM_No_Trigger_Noise_Only_Timeslice_Based_1D_Feb_" + to_string(iFeb);
        HistTitle = "GEM Feb " + to_string(iFeb) + " 1D Noise Only Spillstructure without trigger [Time slice Based]";
        GEM_No_Trigger_Noise_Only_Timeslice_Based_1D_Feb[iFeb] = new TH1I(HistTag, HistTitle, nTimeSlices + 10, -1.5, nTimeSlices + 8.5);
        GEM_No_Trigger_Noise_Only_Timeslice_Based_1D_Feb[iFeb]->GetXaxis()->SetTitle("Timeslice number");
        GEM_No_Trigger_Noise_Only_Timeslice_Based_1D_Feb[iFeb]->GetYaxis()->SetTitle("Counts");
    }

    TH2F* GEM_No_Trigger_Noise_Only_Timeslice_Based_2D_vs_ADC_Feb[18];

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTag = "GEM_No_Trigger_Noise_Only_Timeslice_Based_2D_vs_ADC_Feb_" + to_string(iFeb);
        HistTitle = "GEM Feb " + to_string(iFeb) + " 2D Noise Only Spillstructure vs. ADC without trigger [Time slice Based]";
        GEM_No_Trigger_Noise_Only_Timeslice_Based_2D_vs_ADC_Feb[iFeb] = new TH2F(HistTag, HistTitle, nTimeSlices + 10, -1.5, nTimeSlices + 8.5, 35, -1.5, 33.5);
        GEM_No_Trigger_Noise_Only_Timeslice_Based_2D_vs_ADC_Feb[iFeb]->GetXaxis()->SetTitle("Timeslice number");
        GEM_No_Trigger_Noise_Only_Timeslice_Based_2D_vs_ADC_Feb[iFeb]->GetYaxis()->SetTitle("ADC");
    }

    TH2F* GEM_No_Trigger_Noise_Only_Timeslice_Based_2D_vs_Channel_Feb[18];

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTag = "GEM_No_Trigger_Noise_Only_Timeslice_Based_2D_vs_Channel_Feb_" + to_string(iFeb);
        HistTitle = "GEM Feb " + to_string(iFeb) + " 2D Noise Only Spillstructure vs. Channel without trigger [Time slice Based]";
        GEM_No_Trigger_Noise_Only_Timeslice_Based_2D_vs_Channel_Feb[iFeb] = new TH2F(HistTag, HistTitle, nTimeSlices + 10, -1.5, nTimeSlices + 8.5, 131, -1.5, 129.5);
        GEM_No_Trigger_Noise_Only_Timeslice_Based_2D_vs_Channel_Feb[iFeb]->GetXaxis()->SetTitle("Timeslice number");
        GEM_No_Trigger_Noise_Only_Timeslice_Based_2D_vs_Channel_Feb[iFeb]->GetYaxis()->SetTitle("Channel");
    }

    HistTag = "GEM_No_Trigger_Noise_Only_Timeslice_Based_2D_vs_Feb_Number";
    HistTitle = "GEM 2D Noise Only Spillstructure vs. Feb Number without trigger [Time slice based] (All Febs inclusive)";
    TH2F* GEM_No_Trigger_Noise_Only_Timeslice_Based_2D_vs_Feb_Number = new TH2F(HistTag, HistTitle, nTimeSlices + 10, -1.5, nTimeSlices + 8.5, 21, -1.5, 19.5);
    GEM_No_Trigger_Noise_Only_Timeslice_Based_2D_vs_Feb_Number->GetXaxis()->SetTitle("Timeslice number");
    GEM_No_Trigger_Noise_Only_Timeslice_Based_2D_vs_Feb_Number->GetYaxis()->SetTitle("Feb number");


    for (iTreeEntry = 0; iTreeEntry < 100; iTreeEntry++)
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

    for (iTreeEntry = 0; iTreeEntry < 1; iTreeEntry++)
    {
        Tree_Data_Reader.SetEntry(iTreeEntry);
        uint64_t TimeSlice_Start_Time = TimeSlice_Event_Header->GetTsStartTime();

        if (FirstDigiFlag == 0)
        {
            First_Digi_Time = TimeSlice_Start_Time;
            FirstDigiFlag = 1;
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

        Int_t GEM_Digis = 0;
        Int_t iMuchStartDigi = 0;

        for (Int_t iMuchDigi = 0; iMuchDigi < Much_Digi.GetSize(); iMuchDigi++, Trigger_Flag = false)
        // for(Int_t iMuchDigi=iMuchStartDigi;iMuchDigi1 < Much_Digi.GetSize();iMuchDigi1++,TriggerFlag=false)
        {
            int32_t Station = CbmMuchAddress::GetStationIndex(Much_Digi[iMuchDigi].GetAddress());
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
            Int_t Tolerance = (Int_t)TMath::Ceil(Much_Digi.GetSize() / (1280 * 4.0)); // 1 Ts = ~128ms --> 1/(4*1280) = ~25 #mus

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
                    continue;

                Double_t First_Time_difference = ((Double_t)Much_Digi[iMuchDigi1].GetTime() - (Double_t)Much_Digi[0].GetTime()) / 1000.0;

                // cout<<"Sector: "<<Sector1<<"  Channel: "<<Channel1<<"  ADC: "<<Much_Digi[iMuchDigi].GetAdc()<<" Timestamp: "<<(uint64_t)Much_Digi[iMuchDigi1].GetTime()<<endl;

                Double_t Time_difference = ((Double_t)Much_Digi[iMuchDigi].GetTime() - (Double_t)Much_Digi[iMuchDigi1].GetTime()) / 1000.0;

                if (First_Time_difference > 0.0)
                {
                    // cout<<Time_difference<<endl;

                    if (Time_difference > 0.500)
                    {
                        continue;
                    }
                    else if (Time_difference < 0.200)
                    {
                        break;
                    }
                    else
                    {
                        int32_t Sector1 = CbmMuchAddress::GetSectorIndex(Much_Digi[iMuchDigi1].GetAddress());
                        int32_t Channel1 = CbmMuchAddress::GetChannelIndex(Much_Digi[iMuchDigi1].GetAddress());

                        if (Sector1 > 96 || Channel1 > 22)
                            continue;
                        Int_t FEB_GEM = GEM_Feb_no_X_Y[Channel1][Sector1];
                        Int_t FEB_CH = GEM_Ch_no_X_Y[Channel1][Sector1];
                        Int_t ADC_Value = Much_Digi[iMuchDigi1].GetAdc();

                        // if(GEM_Noise[Channel1][Sector1]==-1) continue;
                        // cout<<iMuchDigi<<"  Sector: "<<Sector1<<"  Channel: "<<Channel1<<"  ADC: "<<Much_Digi[iMuchDigi].GetAdc()<<" Timestamp: "<<(uint64_t)Much_Digi[iMuchDigi1].GetTime()<<endl;
                        // cout<<iMuchDigi<<"  FEB: "<<FEB_GEM<<"  Channel: "<<FEB_CH<<"  ADC: "<<Much_Digi[iMuchDigi].GetAdc()<<" Timestamp: "<<(uint64_t)Much_Digi[iMuchDigi1].GetTime()<<endl;
                        if (GEM_Noise[Channel1][Sector1] != -1)
                        {
                            Cluster_Charge += 6.0 + 2.5 * Much_Digi[iMuchDigi1].GetAdc();
                            nDigis_Per_Trigger++;
                        }

                        if (Channel >= Two_Fold_Lower_Bound && Channel < Two_Fold_Upper_Bound)
                        {
                            GEM_No_Trigger_Raw_Time_Based_1D_All_Febs_Inclusive->Fill(Much_Digi_Time_ms);
                            GEM_No_Trigger_Raw_Time_Based_1D_Feb[FEB_GEM]->Fill(Much_Digi_Time_ms);
                            GEM_No_Trigger_Raw_Timeslice_Based_1D_All_Febs_Inclusive->Fill(iTimeSlice);
                            GEM_No_Trigger_Raw_Timeslice_Based_1D_Feb[FEB_GEM]->Fill(iTimeSlice);
                            GEM_No_Trigger_Raw_Timeslice_Based_2D_vs_ADC_Feb[FEB_GEM]->Fill(iTimeSlice, ADC_Value);
                            GEM_No_Trigger_Raw_Timeslice_Based_2D_vs_Channel_Feb[FEB_GEM]->Fill(iTimeSlice, FEB_CH);
                            GEM_No_Trigger_Raw_Timeslice_Based_2D_vs_Feb_Number->Fill(iTimeSlice, FEB_GEM);

                            if (GEM_Noise[Channel][Sector] != -1)
                            {
                                GEM_No_Trigger_Noise_Masked_Time_Based_1D_All_Febs_Inclusive->Fill(Much_Digi_Time_ms);
                                GEM_No_Trigger_Noise_Masked_Time_Based_1D_Feb[FEB_GEM]->Fill(Much_Digi_Time_ms);
                                GEM_No_Trigger_Noise_Masked_Timeslice_Based_1D_All_Febs_Inclusive->Fill(iTimeSlice);
                                GEM_No_Trigger_Noise_Masked_Timeslice_Based_1D_Feb[FEB_GEM]->Fill(iTimeSlice);
                                GEM_No_Trigger_Noise_Masked_Timeslice_Based_2D_vs_ADC_Feb[FEB_GEM]->Fill(iTimeSlice, ADC_Value);
                                GEM_No_Trigger_Noise_Masked_Timeslice_Based_2D_vs_Channel_Feb[FEB_GEM]->Fill(iTimeSlice, FEB_CH);
                                GEM_No_Trigger_Noise_Masked_Timeslice_Based_2D_vs_Feb_Number->Fill(iTimeSlice, FEB_GEM);
                            }
                            else
                            {
                                GEM_No_Trigger_Noise_Only_Time_Based_1D_All_Febs_Inclusive->Fill(Much_Digi_Time_ms);
                                GEM_No_Trigger_Noise_Only_Time_Based_1D_Feb[FEB_GEM]->Fill(Much_Digi_Time_ms);
                                GEM_No_Trigger_Noise_Only_Timeslice_Based_1D_All_Febs_Inclusive->Fill(iTimeSlice);
                                GEM_No_Trigger_Noise_Only_Timeslice_Based_1D_Feb[FEB_GEM]->Fill(iTimeSlice);
                                GEM_No_Trigger_Noise_Only_Timeslice_Based_2D_vs_ADC_Feb[FEB_GEM]->Fill(iTimeSlice, ADC_Value);
                                GEM_No_Trigger_Noise_Only_Timeslice_Based_2D_vs_Channel_Feb[FEB_GEM]->Fill(iTimeSlice, FEB_CH);
                                GEM_No_Trigger_Noise_Only_Timeslice_Based_2D_vs_Feb_Number->Fill(iTimeSlice, FEB_GEM);
                            }
                        }
                        else if (Channel >= Three_Fold_Lower_Bound && Channel < Three_Fold_Upper_Bound)
                        {
                            GEM_No_Trigger_Raw_Time_Based_1D_All_Febs_Inclusive->Fill(Much_Digi_Time_ms);
                            GEM_No_Trigger_Raw_Time_Based_1D_Feb[FEB_GEM]->Fill(Much_Digi_Time_ms);
                            GEM_No_Trigger_Raw_Timeslice_Based_1D_All_Febs_Inclusive->Fill(iTimeSlice);
                            GEM_No_Trigger_Raw_Timeslice_Based_1D_Feb[FEB_GEM]->Fill(iTimeSlice);
                            GEM_No_Trigger_Raw_Timeslice_Based_2D_vs_ADC_Feb[FEB_GEM]->Fill(iTimeSlice, ADC_Value);
                            GEM_No_Trigger_Raw_Timeslice_Based_2D_vs_Channel_Feb[FEB_GEM]->Fill(iTimeSlice, FEB_CH);
                            GEM_No_Trigger_Raw_Timeslice_Based_2D_vs_Feb_Number->Fill(iTimeSlice, FEB_GEM);

                            if (GEM_Noise[Channel][Sector] != -1)
                            {
                                GEM_No_Trigger_Noise_Masked_Time_Based_1D_All_Febs_Inclusive->Fill(Much_Digi_Time_ms);
                                GEM_No_Trigger_Noise_Masked_Time_Based_1D_Feb[FEB_GEM]->Fill(Much_Digi_Time_ms);
                                GEM_No_Trigger_Noise_Masked_Timeslice_Based_1D_All_Febs_Inclusive->Fill(iTimeSlice);
                                GEM_No_Trigger_Noise_Masked_Timeslice_Based_1D_Feb[FEB_GEM]->Fill(iTimeSlice);
                                GEM_No_Trigger_Noise_Masked_Timeslice_Based_2D_vs_ADC_Feb[FEB_GEM]->Fill(iTimeSlice, ADC_Value);
                                GEM_No_Trigger_Noise_Masked_Timeslice_Based_2D_vs_Channel_Feb[FEB_GEM]->Fill(iTimeSlice, FEB_CH);
                                GEM_No_Trigger_Noise_Masked_Timeslice_Based_2D_vs_Feb_Number->Fill(iTimeSlice, FEB_GEM);
                            }
                            else
                            {
                                GEM_No_Trigger_Noise_Only_Time_Based_1D_All_Febs_Inclusive->Fill(Much_Digi_Time_ms);
                                GEM_No_Trigger_Noise_Only_Time_Based_1D_Feb[FEB_GEM]->Fill(Much_Digi_Time_ms);
                                GEM_No_Trigger_Noise_Only_Timeslice_Based_1D_All_Febs_Inclusive->Fill(iTimeSlice);
                                GEM_No_Trigger_Noise_Only_Timeslice_Based_1D_Feb[FEB_GEM]->Fill(iTimeSlice);
                                GEM_No_Trigger_Noise_Only_Timeslice_Based_2D_vs_ADC_Feb[FEB_GEM]->Fill(iTimeSlice, ADC_Value);
                                GEM_No_Trigger_Noise_Only_Timeslice_Based_2D_vs_Channel_Feb[FEB_GEM]->Fill(iTimeSlice, FEB_CH);
                                GEM_No_Trigger_Noise_Only_Timeslice_Based_2D_vs_Feb_Number->Fill(iTimeSlice, FEB_GEM);
                            }
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
    }

    fOut->mkdir("Spill Structures");
    fOut->mkdir("Spill Structures/GEM");
    fOut->mkdir("Spill Structures/GEM/Without Trigger");
    fOut->mkdir("Spill Structures/GEM/Without Trigger/Raw");
    fOut->mkdir("Spill Structures/GEM/Without Trigger/Raw/Time Based");
    fOut->mkdir("Spill Structures/GEM/Without Trigger/Raw/Time Based/1D");
    fOut->mkdir("Spill Structures/GEM/Without Trigger/Raw/Time Based/1D/All Febs Inclusive");
    fOut->mkdir("Spill Structures/GEM/Without Trigger/Raw/Time Based/1D/Febwise");
    fOut->mkdir("Spill Structures/GEM/Without Trigger/Raw/Timeslice Based");
    fOut->mkdir("Spill Structures/GEM/Without Trigger/Raw/Timeslice Based/1D");
    fOut->mkdir("Spill Structures/GEM/Without Trigger/Raw/Timeslice Based/1D/All Febs Inclusive");
    fOut->mkdir("Spill Structures/GEM/Without Trigger/Raw/Timeslice Based/1D/Febwise");
    fOut->mkdir("Spill Structures/GEM/Without Trigger/Raw/Timeslice Based/2D");
    fOut->mkdir("Spill Structures/GEM/Without Trigger/Raw/Timeslice Based/2D/vs. ADC");
    fOut->mkdir("Spill Structures/GEM/Without Trigger/Raw/Timeslice Based/2D/vs. Channel Number");
    fOut->mkdir("Spill Structures/GEM/Without Trigger/Raw/Timeslice Based/2D/vs. Feb Number");
    fOut->mkdir("Spill Structures/GEM/Without Trigger/Noise Masked");
    fOut->mkdir("Spill Structures/GEM/Without Trigger/Noise Masked/Time Based");
    fOut->mkdir("Spill Structures/GEM/Without Trigger/Noise Masked/Time Based/1D");
    fOut->mkdir("Spill Structures/GEM/Without Trigger/Noise Masked/Time Based/1D/All Febs Inclusive");
    fOut->mkdir("Spill Structures/GEM/Without Trigger/Noise Masked/Time Based/1D/Febwise");
    fOut->mkdir("Spill Structures/GEM/Without Trigger/Noise Masked/Timeslice Based");
    fOut->mkdir("Spill Structures/GEM/Without Trigger/Noise Masked/Timeslice Based/1D");
    fOut->mkdir("Spill Structures/GEM/Without Trigger/Noise Masked/Timeslice Based/1D/All Febs Inclusive");
    fOut->mkdir("Spill Structures/GEM/Without Trigger/Noise Masked/Timeslice Based/1D/Febwise");
    fOut->mkdir("Spill Structures/GEM/Without Trigger/Noise Masked/Timeslice Based/2D");
    fOut->mkdir("Spill Structures/GEM/Without Trigger/Noise Masked/Timeslice Based/2D/vs. ADC");
    fOut->mkdir("Spill Structures/GEM/Without Trigger/Noise Masked/Timeslice Based/2D/vs. Channel Number");
    fOut->mkdir("Spill Structures/GEM/Without Trigger/Noise Masked/Timeslice Based/2D/vs. Feb Number");
    fOut->mkdir("Spill Structures/GEM/Without Trigger/Noise Only");
    fOut->mkdir("Spill Structures/GEM/Without Trigger/Noise Only/Time Based");
    fOut->mkdir("Spill Structures/GEM/Without Trigger/Noise Only/Time Based/1D");
    fOut->mkdir("Spill Structures/GEM/Without Trigger/Noise Only/Time Based/1D/All Febs Inclusive");
    fOut->mkdir("Spill Structures/GEM/Without Trigger/Noise Only/Time Based/1D/Febwise");
    fOut->mkdir("Spill Structures/GEM/Without Trigger/Noise Only/Timeslice Based");
    fOut->mkdir("Spill Structures/GEM/Without Trigger/Noise Only/Timeslice Based/1D");
    fOut->mkdir("Spill Structures/GEM/Without Trigger/Noise Only/Timeslice Based/1D/All Febs Inclusive");
    fOut->mkdir("Spill Structures/GEM/Without Trigger/Noise Only/Timeslice Based/1D/Febwise");
    fOut->mkdir("Spill Structures/GEM/Without Trigger/Noise Only/Timeslice Based/2D");
    fOut->mkdir("Spill Structures/GEM/Without Trigger/Noise Only/Timeslice Based/2D/vs. ADC");
    fOut->mkdir("Spill Structures/GEM/Without Trigger/Noise Only/Timeslice Based/2D/vs. Channel Number");
    fOut->mkdir("Spill Structures/GEM/Without Trigger/Noise Only/Timeslice Based/2D/vs. Feb Number");


    fOut->cd("Spill Structures/GEM/Without Trigger/Raw/Time Based/1D/All Febs Inclusive");
    GEM_No_Trigger_Raw_Time_Based_1D_All_Febs_Inclusive->Write();

    fOut->cd("Spill Structures/GEM/Without Trigger/Raw/Time Based/1D/Febwise");
    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
        GEM_No_Trigger_Raw_Time_Based_1D_Feb[iFeb]->Write();

    fOut->cd("Spill Structures/GEM/Without Trigger/Raw/Timeslice Based/1D/All Febs Inclusive");
    GEM_No_Trigger_Raw_Timeslice_Based_1D_All_Febs_Inclusive->Write();

    fOut->cd("Spill Structures/GEM/Without Trigger/Raw/Timeslice Based/1D/Febwise");
    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
        GEM_No_Trigger_Raw_Timeslice_Based_1D_Feb[iFeb]->Write();

    fOut->cd("Spill Structures/GEM/Without Trigger/Raw/Timeslice Based/2D/vs. ADC");
    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
        GEM_No_Trigger_Raw_Timeslice_Based_2D_vs_ADC_Feb[iFeb]->Write();

    fOut->cd("Spill Structures/GEM/Without Trigger/Raw/Timeslice Based/2D/vs. Channel Number");
    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
        GEM_No_Trigger_Raw_Timeslice_Based_2D_vs_Channel_Feb[iFeb]->Write();

    fOut->cd("Spill Structures/GEM/Without Trigger/Raw/Timeslice Based/2D/vs. Feb Number");
    GEM_No_Trigger_Raw_Timeslice_Based_2D_vs_Feb_Number->Write();


    fOut->cd("Spill Structures/GEM/Without Trigger/Noise Masked/Time Based/1D/All Febs Inclusive");
    GEM_No_Trigger_Noise_Masked_Time_Based_1D_All_Febs_Inclusive->Write();

    fOut->cd("Spill Structures/GEM/Without Trigger/Noise Masked/Time Based/1D/Febwise");
    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
        GEM_No_Trigger_Noise_Masked_Time_Based_1D_Feb[iFeb]->Write();

    fOut->cd("Spill Structures/GEM/Without Trigger/Noise Masked/Timeslice Based/1D/All Febs Inclusive");
    GEM_No_Trigger_Noise_Masked_Timeslice_Based_1D_All_Febs_Inclusive->Write();

    fOut->cd("Spill Structures/GEM/Without Trigger/Noise Masked/Timeslice Based/1D/Febwise");
    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
        GEM_No_Trigger_Noise_Masked_Timeslice_Based_1D_Feb[iFeb]->Write();

    fOut->cd("Spill Structures/GEM/Without Trigger/Noise Masked/Timeslice Based/2D/vs. ADC");
    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
        GEM_No_Trigger_Noise_Masked_Timeslice_Based_2D_vs_ADC_Feb[iFeb]->Write();

    fOut->cd("Spill Structures/GEM/Without Trigger/Noise Masked/Timeslice Based/2D/vs. Channel Number");
    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
        GEM_No_Trigger_Noise_Masked_Timeslice_Based_2D_vs_Channel_Feb[iFeb]->Write();

    fOut->cd("Spill Structures/GEM/Without Trigger/Noise Masked/Timeslice Based/2D/vs. Feb Number");
    GEM_No_Trigger_Noise_Masked_Timeslice_Based_2D_vs_Feb_Number->Write();


    fOut->cd("Spill Structures/GEM/Without Trigger/Noise Only/Time Based/1D/All Febs Inclusive");
    GEM_No_Trigger_Noise_Only_Time_Based_1D_All_Febs_Inclusive->Write();

    fOut->cd("Spill Structures/GEM/Without Trigger/Noise Only/Time Based/1D/Febwise");
    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
        GEM_No_Trigger_Noise_Only_Time_Based_1D_Feb[iFeb]->Write();

    fOut->cd("Spill Structures/GEM/Without Trigger/Noise Only/Timeslice Based/1D/All Febs Inclusive");
    GEM_No_Trigger_Noise_Only_Timeslice_Based_1D_All_Febs_Inclusive->Write();

    fOut->cd("Spill Structures/GEM/Without Trigger/Noise Only/Timeslice Based/1D/Febwise");
    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
        GEM_No_Trigger_Noise_Only_Timeslice_Based_1D_Feb[iFeb]->Write();

    fOut->cd("Spill Structures/GEM/Without Trigger/Noise Only/Timeslice Based/2D/vs. ADC");
    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
        GEM_No_Trigger_Noise_Only_Timeslice_Based_2D_vs_ADC_Feb[iFeb]->Write();

    fOut->cd("Spill Structures/GEM/Without Trigger/Noise Only/Timeslice Based/2D/vs. Channel Number");
    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
        GEM_No_Trigger_Noise_Only_Timeslice_Based_2D_vs_Channel_Feb[iFeb]->Write();

    fOut->cd("Spill Structures/GEM/Without Trigger/Noise Only/Timeslice Based/2D/vs. Feb Number");
    GEM_No_Trigger_Noise_Only_Timeslice_Based_2D_vs_Feb_Number->Write();

    fIn->Close();
    fOut->Close();
}
