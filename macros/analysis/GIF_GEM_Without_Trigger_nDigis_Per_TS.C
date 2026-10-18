// Copyright (C) Experimental High Energy Physics and Applications Group, VECC, Kolkata
// Author(s): Apar Agarwal

// This macro has been created for the purpose
// analysing various aspects of nDigis Per TS
// of GEM for GIF++ 2023 data

// This Code requires the CbmRoot version DEC 21 or higher
// Recommended on Linux 20.04 --> Fairsoftversion : APR21p2 and FAIRROOTVERSION: v18.6.7 or higher

// Code first tested on 13th November, 2023

void GIF_GEM_Without_Trigger_nDigis_Per_TS(Int_t RunId = 893)
{
    TStopwatch Code_Timer;
    Code_Timer.Start();

    TString InputFileName = "Run" + to_string(RunId) + ".digi.root";
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

    TString OutputFileName = "data/nDigis_Per_TS_Distributions/Run" + to_string(RunId) + "_GEM_Without_Trigger_nDigis_Per_TS.root";
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
    const Int_t nTimeSlices = Input_Tree->GetEntries();

    TTreeReader Tree_Data_Reader("cbmsim;", fIn); // Both TTree and TTreeReader are not required generally but kept in case some functionalities of TTree are needed

    TTreeReaderArray<CbmMuchDigi> Much_Digi(Tree_Data_Reader, "MuchDigi"); // MuchDigi -> Branch Name  and Much_Digi --> Element to Read Much data
    TTreeReaderValue<CbmTsEventHeader> TimeSlice_Event_Header(Tree_Data_Reader, "EventHeader.");

    Int_t iTreeEntry;
    Int_t FirstDigiFlag = 0;
    Int_t LastDigiFlag = 0;
    uint64_t First_Digi_Time;
    uint64_t Last_Digi_Time;
    TString HistTitle, HistTag;

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

    Int_t GEM_Digis_Without_Trigger_Raw_No_ADC_Cut[1500] = {0};
    Int_t Highest_GEM_Digis_Without_Trigger_Raw_No_ADC_Cut = 0;
    Int_t GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Single[31][1500] = {0};
    Int_t Highest_GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Single[31] = {0};
    Int_t Highest_GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Single_Overall = 0;
    Int_t GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Cumulative[31][1500] = {0};
    Int_t Highest_GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Cumulative[31] = {0};
    Int_t Highest_GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Cumulative_Overall = 0;
    Int_t GEM_Digis_Without_Trigger_Noise_Masked_No_ADC_Cut[1500] = {0};
    Int_t Highest_GEM_Digis_Without_Trigger_Noise_Masked_No_ADC_Cut = 0;
    Int_t GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Single[31][1500] = {0};
    Int_t Highest_GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Single[31] = {0};
    Int_t Highest_GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Single_Overall = 0;
    Int_t GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Cumulative[31][1500] = {0};
    Int_t Highest_GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Cumulative[31] = {0};
    Int_t Highest_GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Cumulative_Overall = 0;
    Int_t GEM_Digis_Without_Trigger_Noise_Only_No_ADC_Cut[1500] = {0};
    Int_t Highest_GEM_Digis_Without_Trigger_Noise_Only_No_ADC_Cut = 0;
    Int_t GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Single[31][1500] = {0};
    Int_t Highest_GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Single[31] = {0};
    Int_t Highest_GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Single_Overall = 0;
    Int_t GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Cumulative[31][1500] = {0};
    Int_t Highest_GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Cumulative[31] = {0};
    Int_t Highest_GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Cumulative_Overall = 0;

    Int_t GEM_Digis_Without_Trigger_Raw_No_ADC_Cut_vs_FEB[18][1500] = {0};
    Int_t Highest_GEM_Digis_Without_Trigger_Raw_No_ADC_Cut_vs_FEB[18] = {0};
    Int_t Highest_GEM_Digis_Without_Trigger_Raw_No_ADC_Cut_vs_FEB_Overall = 0;


    // Int_t GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Single_vs_FEB[31][18][1500]={0};
    // Int_t Highest_GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Single_vs_FEB[31][18]={0};
    // Int_t Highest_GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Single_vs_FEB_Overall[31]={0};

    // Int_t GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Cumulative_vs_FEB[31][18][1500]={0};
    // Int_t Highest_GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Cumulative_vs_FEB[31][18]={0};
    // Int_t Highest_GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Cumulative_vs_FEB_Overall[31]={0};

    Int_t GEM_Digis_Without_Trigger_Noise_Masked_No_ADC_Cut_vs_FEB[18][1500] = {0};
    Int_t Highest_GEM_Digis_Without_Trigger_Noise_Masked_No_ADC_Cut_vs_FEB[18] = {0};
    Int_t Highest_GEM_Digis_Without_Trigger_Noise_Masked_No_ADC_Cut_vs_FEB_Overall = 0;


    // Int_t GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Single_vs_FEB[31][18][1500]={0};
    // Int_t Highest_GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Single_vs_FEB[31][18]={0};
    // Int_t Highest_GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Single_vs_FEB_Overall[31]={0};

    // Int_t GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Cumulative_vs_FEB[31][18][1500]={0};
    // Int_t Highest_GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Cumulative_vs_FEB[31][18]={0};
    // Int_t Highest_GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Cumulative_vs_FEB_Overall[31]={0};

    Int_t GEM_Digis_Without_Trigger_Noise_Only_No_ADC_Cut_vs_FEB[18][1500] = {0};
    Int_t Highest_GEM_Digis_Without_Trigger_Noise_Only_No_ADC_Cut_vs_FEB[18] = {0};
    Int_t Highest_GEM_Digis_Without_Trigger_Noise_Only_No_ADC_Cut_vs_FEB_Overall = 0;

    // Int_t GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Single_vs_FEB[31][18][1500]={0};
    // Int_t Highest_GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Single_vs_FEB[31][18]={0};
    // Int_t Highest_GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Single_vs_FEB_Overall[31]={0};

    // Int_t GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Cumulative_vs_FEB[31][18][1500]={0};
    // Int_t Highest_GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Cumulative_vs_FEB[31][18]={0};
    // Int_t Highest_GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Cumulative_vs_FEB_Overall[31]={0};

    Int_t GEM_Digis_Without_Trigger_Raw_No_ADC_Cut_vs_ADC[31][1500] = {0};
    Int_t Highest_GEM_Digis_Without_Trigger_Raw_No_ADC_Cut_vs_ADC[31] = {0};
    Int_t Highest_GEM_Digis_Without_Trigger_Raw_No_ADC_Cut_vs_ADC_Overall = 0;

    Int_t GEM_Digis_Without_Trigger_Noise_Masked_No_ADC_Cut_vs_ADC[31][1500] = {0};
    Int_t Highest_GEM_Digis_Without_Trigger_Noise_Masked_No_ADC_Cut_vs_ADC[31] = {0};
    Int_t Highest_GEM_Digis_Without_Trigger_Noise_Masked_No_ADC_Cut_vs_ADC_Overall = 0;

    Int_t GEM_Digis_Without_Trigger_Noise_Only_No_ADC_Cut_vs_ADC[31][1500] = {0};
    Int_t Highest_GEM_Digis_Without_Trigger_Noise_Only_No_ADC_Cut_vs_ADC[31] = {0};
    Int_t Highest_GEM_Digis_Without_Trigger_Noise_Only_No_ADC_Cut_vs_ADC_Overall = 0;

    // for(iTreeEntry = 0; iTreeEntry < 200 ;iTreeEntry++)
    for (iTreeEntry = 0; iTreeEntry < nTimeSlices; iTreeEntry++)
    {
        Tree_Data_Reader.SetEntry(iTreeEntry);
        uint64_t TimeSlice_Start_Time = TimeSlice_Event_Header->GetTsStartTime();

        Int_t iTimeSlice = TimeSlice_Event_Header->GetTsIndex();

        // if(iTreeEntry%100 == 0)
        cout << "iTreeEntry: " << iTreeEntry << "  TS Index: " << iTimeSlice << endl;

        for (Int_t iMuchDigi = 0; iMuchDigi < Much_Digi.GetSize(); iMuchDigi++)
        {
            int32_t Station = CbmMuchAddress::GetStationIndex(Much_Digi[iMuchDigi].GetAddress());
            if (Station != 0)
                continue; // Selecting GEM Only
            int32_t Sector = CbmMuchAddress::GetSectorIndex(Much_Digi[iMuchDigi].GetAddress());
            int32_t Channel = CbmMuchAddress::GetChannelIndex(Much_Digi[iMuchDigi].GetAddress());
            if (Sector > 96 || Channel > 22)
                continue;
            Int_t GEM_FEB = GEM_Feb_no_X_Y[Channel][Sector];
            Int_t GEM_CH = GEM_Ch_no_X_Y[Channel][Sector];
            Int_t ADC_Value = Much_Digi[iMuchDigi].GetAdc() - 1; // One subtracted since ADC start from 1 while array starts from 0

            GEM_Digis_Without_Trigger_Raw_No_ADC_Cut[iTimeSlice]++;
            if (GEM_Digis_Without_Trigger_Raw_No_ADC_Cut[iTimeSlice] > Highest_GEM_Digis_Without_Trigger_Raw_No_ADC_Cut)
                Highest_GEM_Digis_Without_Trigger_Raw_No_ADC_Cut = GEM_Digis_Without_Trigger_Raw_No_ADC_Cut[iTimeSlice];

            GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Single[ADC_Value][iTimeSlice]++;
            if (GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Single[ADC_Value][iTimeSlice] > Highest_GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Single[ADC_Value])
                Highest_GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Single[ADC_Value] = GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Single[ADC_Value][iTimeSlice];

            for (Int_t iADC = ADC_Value + 1; iADC < 30; iADC++)
                GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Cumulative[iADC + 1][iTimeSlice]++;
            if (ADC_Value != 30)
            {
                if (GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Cumulative[ADC_Value + 1][iTimeSlice] > Highest_GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Cumulative[ADC_Value + 1])
                    Highest_GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Cumulative[ADC_Value + 1] = GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Cumulative[ADC_Value + 1][iTimeSlice];
            }

            GEM_Digis_Without_Trigger_Raw_No_ADC_Cut_vs_FEB[GEM_FEB][iTimeSlice]++;
            if (GEM_Digis_Without_Trigger_Raw_No_ADC_Cut_vs_FEB[GEM_FEB][iTimeSlice] > Highest_GEM_Digis_Without_Trigger_Raw_No_ADC_Cut_vs_FEB[GEM_FEB])
                Highest_GEM_Digis_Without_Trigger_Raw_No_ADC_Cut_vs_FEB[GEM_FEB] = GEM_Digis_Without_Trigger_Raw_No_ADC_Cut_vs_FEB[GEM_FEB][iTimeSlice];

            // GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Single_vs_FEB[ADC_Value][GEM_FEB][iTimeSlice]++;
            // if(GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Single_vs_FEB[ADC_Value][GEM_FEB][iTimeSlice] > Highest_GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Single_vs_FEB[ADC_Value][GEM_FEB])
            //     Highest_GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Single_vs_FEB[ADC_Value][GEM_FEB] = GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Single_vs_FEB[ADC_Value][GEM_FEB][iTimeSlice];

            // for(Int_t iADC=ADC_Value+1;iADC<30;iADC++) GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Cumulative_vs_FEB[iADC][GEM_FEB][iTimeSlice]++;
            // if(ADC_Value!=30)
            // {
            //     if(GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Cumulative_vs_FEB[ADC_Value+1][GEM_FEB][iTimeSlice] > Highest_GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Cumulative_vs_FEB[ADC_Value+1][GEM_FEB])
            //     {
            //         Highest_GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Cumulative_vs_FEB[ADC_Value+1][GEM_FEB] = GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Cumulative_vs_FEB[ADC_Value+1][GEM_FEB][iTimeSlice];
            //     }
            // }

            GEM_Digis_Without_Trigger_Raw_No_ADC_Cut_vs_ADC[ADC_Value][iTimeSlice]++;
            if (GEM_Digis_Without_Trigger_Raw_No_ADC_Cut_vs_ADC[ADC_Value][iTimeSlice] > Highest_GEM_Digis_Without_Trigger_Raw_No_ADC_Cut_vs_ADC[ADC_Value])
                Highest_GEM_Digis_Without_Trigger_Raw_No_ADC_Cut_vs_ADC[ADC_Value] = GEM_Digis_Without_Trigger_Raw_No_ADC_Cut_vs_ADC[ADC_Value][iTimeSlice];

            if (GEM_Noise[Channel][Sector] != -1)
            {
                GEM_Digis_Without_Trigger_Noise_Masked_No_ADC_Cut[iTimeSlice]++;
                if (GEM_Digis_Without_Trigger_Noise_Masked_No_ADC_Cut[iTimeSlice] > Highest_GEM_Digis_Without_Trigger_Noise_Masked_No_ADC_Cut)
                    Highest_GEM_Digis_Without_Trigger_Noise_Masked_No_ADC_Cut = GEM_Digis_Without_Trigger_Noise_Masked_No_ADC_Cut[iTimeSlice];

                GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Single[ADC_Value][iTimeSlice]++;
                if (GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Single[ADC_Value][iTimeSlice] > Highest_GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Single[ADC_Value])
                    Highest_GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Single[ADC_Value] = GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Single[ADC_Value][iTimeSlice];

                for (Int_t iADC = ADC_Value + 1; iADC < 30; iADC++)
                    GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Cumulative[iADC + 1][iTimeSlice]++;
                if (ADC_Value != 30)
                {
                    if (GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Cumulative[ADC_Value + 1][iTimeSlice] > Highest_GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Cumulative[ADC_Value + 1])
                        Highest_GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Cumulative[ADC_Value + 1] = GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Cumulative[ADC_Value + 1][iTimeSlice];
                }

                GEM_Digis_Without_Trigger_Noise_Masked_No_ADC_Cut_vs_FEB[GEM_FEB][iTimeSlice]++;
                if (GEM_Digis_Without_Trigger_Noise_Masked_No_ADC_Cut_vs_FEB[GEM_FEB][iTimeSlice] > Highest_GEM_Digis_Without_Trigger_Noise_Masked_No_ADC_Cut_vs_FEB[GEM_FEB])
                    Highest_GEM_Digis_Without_Trigger_Noise_Masked_No_ADC_Cut_vs_FEB[GEM_FEB] = GEM_Digis_Without_Trigger_Noise_Masked_No_ADC_Cut_vs_FEB[GEM_FEB][iTimeSlice];

                // GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Single_vs_FEB[ADC_Value][GEM_FEB][iTimeSlice]++;
                // if(GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Single_vs_FEB[ADC_Value][GEM_FEB][iTimeSlice] > Highest_GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Single_vs_FEB[ADC_Value][GEM_FEB])
                //     Highest_GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Single_vs_FEB[ADC_Value][GEM_FEB] = GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Single_vs_FEB[ADC_Value][GEM_FEB][iTimeSlice];

                // for(Int_t iADC=ADC_Value+1;iADC<30;iADC++) GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Cumulative_vs_FEB[iADC][GEM_FEB][iTimeSlice]++;
                // if(ADC_Value!=30)
                // {
                //     if(GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Cumulative_vs_FEB[ADC_Value+1][GEM_FEB][iTimeSlice] > Highest_GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Cumulative_vs_FEB[ADC_Value+1][GEM_FEB])
                //     {
                //         Highest_GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Cumulative_vs_FEB[ADC_Value+1][GEM_FEB] = GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Cumulative_vs_FEB[ADC_Value+1][GEM_FEB][iTimeSlice];
                //     }
                // }

                GEM_Digis_Without_Trigger_Noise_Masked_No_ADC_Cut_vs_ADC[ADC_Value][iTimeSlice]++;
                if (GEM_Digis_Without_Trigger_Noise_Masked_No_ADC_Cut_vs_ADC[ADC_Value][iTimeSlice] > Highest_GEM_Digis_Without_Trigger_Noise_Masked_No_ADC_Cut_vs_ADC[ADC_Value])
                    Highest_GEM_Digis_Without_Trigger_Noise_Masked_No_ADC_Cut_vs_ADC[ADC_Value] = GEM_Digis_Without_Trigger_Noise_Masked_No_ADC_Cut_vs_ADC[ADC_Value][iTimeSlice];
            }
            else
            {
                GEM_Digis_Without_Trigger_Noise_Only_No_ADC_Cut[iTimeSlice]++;
                if (GEM_Digis_Without_Trigger_Noise_Only_No_ADC_Cut[iTimeSlice] > Highest_GEM_Digis_Without_Trigger_Noise_Only_No_ADC_Cut)
                    Highest_GEM_Digis_Without_Trigger_Noise_Only_No_ADC_Cut = GEM_Digis_Without_Trigger_Noise_Only_No_ADC_Cut[iTimeSlice];

                // GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Single[ADC_Value][iTimeSlice]++;
                // if(GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Single[ADC_Value][iTimeSlice] > Highest_GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Single[ADC_Value])
                //     Highest_GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Single[ADC_Value] = GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Single[ADC_Value][iTimeSlice];

                // for(Int_t iADC=ADC_Value+1;iADC<30;iADC++) GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Cumulative[iADC+1][iTimeSlice]++;
                // if(ADC_Value!=30)
                // {
                //     if(GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Cumulative[ADC_Value+1][iTimeSlice] > Highest_GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Cumulative[ADC_Value+1])
                //     Highest_GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Cumulative[ADC_Value+1] = GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Cumulative[ADC_Value+1][iTimeSlice];
                // }

                GEM_Digis_Without_Trigger_Noise_Only_No_ADC_Cut_vs_FEB[GEM_FEB][iTimeSlice]++;
                if (GEM_Digis_Without_Trigger_Noise_Only_No_ADC_Cut_vs_FEB[GEM_FEB][iTimeSlice] > Highest_GEM_Digis_Without_Trigger_Noise_Only_No_ADC_Cut_vs_FEB[GEM_FEB])
                    Highest_GEM_Digis_Without_Trigger_Noise_Only_No_ADC_Cut_vs_FEB[GEM_FEB] = GEM_Digis_Without_Trigger_Noise_Only_No_ADC_Cut_vs_FEB[GEM_FEB][iTimeSlice];

                // GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Single_vs_FEB[ADC_Value][GEM_FEB][iTimeSlice]++;
                // if(GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Single_vs_FEB[ADC_Value][GEM_FEB][iTimeSlice] > Highest_GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Single_vs_FEB[ADC_Value][GEM_FEB])
                //     Highest_GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Single_vs_FEB[ADC_Value][GEM_FEB] = GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Single_vs_FEB[ADC_Value][GEM_FEB][iTimeSlice];

                // for(Int_t iADC=ADC_Value+1;iADC<30;iADC++) GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Cumulative_vs_FEB[iADC][GEM_FEB][iTimeSlice]++;
                // if(ADC_Value!=30)
                // {
                //     if(GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Cumulative_vs_FEB[ADC_Value+1][GEM_FEB][iTimeSlice] > Highest_GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Cumulative_vs_FEB[ADC_Value+1][GEM_FEB])
                //     {
                //         Highest_GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Cumulative_vs_FEB[ADC_Value+1][GEM_FEB] = GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Cumulative_vs_FEB[ADC_Value+1][GEM_FEB][iTimeSlice];
                //     }
                // }

                GEM_Digis_Without_Trigger_Noise_Only_No_ADC_Cut_vs_ADC[ADC_Value][iTimeSlice]++;
                if (GEM_Digis_Without_Trigger_Noise_Only_No_ADC_Cut_vs_ADC[ADC_Value][iTimeSlice] > Highest_GEM_Digis_Without_Trigger_Noise_Only_No_ADC_Cut_vs_ADC[ADC_Value])
                    Highest_GEM_Digis_Without_Trigger_Noise_Only_No_ADC_Cut_vs_ADC[ADC_Value] = GEM_Digis_Without_Trigger_Noise_Only_No_ADC_Cut_vs_ADC[ADC_Value][iTimeSlice];
            }
        }
    }

    HistTag = "GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Raw_No_ADC_Cut";
    HistTitle = "GEM-1D Raw nDigis Per TS distribution (No ADC Cut) for Run " + to_string(RunId);
    TH1D* GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Raw_No_ADC_Cut = new TH1D(HistTag, HistTitle, Highest_GEM_Digis_Without_Trigger_Raw_No_ADC_Cut + 101, -1, Highest_GEM_Digis_Without_Trigger_Raw_No_ADC_Cut + 100);
    GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Raw_No_ADC_Cut->GetXaxis()->SetTitle("nDigis/TS");
    GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Raw_No_ADC_Cut->GetYaxis()->SetTitle("Counts");

    TH1D* GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Raw_With_ADC_Cut_Single[31];

    for (Int_t iADC = 0; iADC < 31; iADC++)
    {
        HistTag = "GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Raw_With_ADC_Cut_Single_" + to_string(iADC + 1);
        HistTitle = "GEM-1D Raw nDigis Per TS distribution (Only ADC= " + to_string(iADC + 1) + ") for Run " + to_string(RunId);
        GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Raw_With_ADC_Cut_Single[iADC] = new TH1D(HistTag, HistTitle, Highest_GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Single[iADC] + 101, -1, Highest_GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Single[iADC] + 100);
        GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Raw_With_ADC_Cut_Single[iADC]->GetXaxis()->SetTitle("nDigis/TS");
        GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Raw_With_ADC_Cut_Single[iADC]->GetYaxis()->SetTitle("Counts");
    }

    TH1D* GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Raw_With_ADC_Cut_Cumulative[30];

    for (Int_t iADC = 0; iADC < 30; iADC++)
    {
        HistTag = "GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Raw_With_ADC_Cut_Cumulative_" + to_string(iADC + 1);
        HistTitle = "GEM-1D Raw nDigis Per TS distribution (ADC<= " + to_string(iADC + 1) + " Masked) for Run " + to_string(RunId);
        GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Raw_With_ADC_Cut_Cumulative[iADC] = new TH1D(HistTag, HistTitle, Highest_GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Cumulative[iADC + 1] + 101, -1, Highest_GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Cumulative[iADC + 1] + 100);
        GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Raw_With_ADC_Cut_Cumulative[iADC]->GetXaxis()->SetTitle("nDigis/TS");
        GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Raw_With_ADC_Cut_Cumulative[iADC]->GetYaxis()->SetTitle("Counts");
    }

    HistTag = "GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Noise_Masked_No_ADC_Cut";
    HistTitle = "GEM-1D Noise Masked nDigis Per TS distribution (No ADC Cut) for Run " + to_string(RunId);
    TH1D* GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Noise_Masked_No_ADC_Cut = new TH1D(HistTag, HistTitle, Highest_GEM_Digis_Without_Trigger_Noise_Masked_No_ADC_Cut + 101, -1, Highest_GEM_Digis_Without_Trigger_Noise_Masked_No_ADC_Cut + 100);
    GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Noise_Masked_No_ADC_Cut->GetXaxis()->SetTitle("nDigis/TS");
    GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Noise_Masked_No_ADC_Cut->GetYaxis()->SetTitle("Counts");

    TH1D* GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Noise_Masked_With_ADC_Cut_Single[31];

    for (Int_t iADC = 0; iADC < 31; iADC++)
    {
        HistTag = "GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Noise_Masked_With_ADC_Cut_Single_" + to_string(iADC + 1);
        HistTitle = "GEM-1D Noise Masked nDigis Per TS distribution (Only ADC= " + to_string(iADC + 1) + ") for Run " + to_string(RunId);
        GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Noise_Masked_With_ADC_Cut_Single[iADC] = new TH1D(HistTag, HistTitle, Highest_GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Single[iADC] + 101, -1, Highest_GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Single[iADC] + 100);
        GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Noise_Masked_With_ADC_Cut_Single[iADC]->GetXaxis()->SetTitle("nDigis/TS");
        GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Noise_Masked_With_ADC_Cut_Single[iADC]->GetYaxis()->SetTitle("Counts");
    }

    TH1D* GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Noise_Masked_With_ADC_Cut_Cumulative[30];

    for (Int_t iADC = 0; iADC < 30; iADC++)
    {
        HistTag = "GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Noise_Masked_With_ADC_Cut_Cumulative_" + to_string(iADC + 1);
        HistTitle = "GEM-1D Noise Masked nDigis Per TS distribution (ADC<= " + to_string(iADC + 1) + " Masked) for Run " + to_string(RunId);
        GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Noise_Masked_With_ADC_Cut_Cumulative[iADC] = new TH1D(HistTag, HistTitle, Highest_GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Cumulative[iADC + 1] + 101, -1, Highest_GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Cumulative[iADC + 1] + 100);
        GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Noise_Masked_With_ADC_Cut_Cumulative[iADC]->GetXaxis()->SetTitle("nDigis/TS");
        GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Noise_Masked_With_ADC_Cut_Cumulative[iADC]->GetYaxis()->SetTitle("Counts");
    }

    HistTag = "GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Noise_Only_No_ADC_Cut";
    HistTitle = "GEM-1D Noise Only nDigis Per TS distribution (No ADC Cut) for Run " + to_string(RunId);
    TH1D* GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Noise_Only_No_ADC_Cut = new TH1D(HistTag, HistTitle, Highest_GEM_Digis_Without_Trigger_Noise_Only_No_ADC_Cut + 101, -1, Highest_GEM_Digis_Without_Trigger_Noise_Only_No_ADC_Cut + 100);
    GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Noise_Only_No_ADC_Cut->GetXaxis()->SetTitle("nDigis/TS");
    GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Noise_Only_No_ADC_Cut->GetYaxis()->SetTitle("Counts");

    TH1D* GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Noise_Only_With_ADC_Cut_Single[31];

    for (Int_t iADC = 0; iADC < 31; iADC++)
    {
        HistTag = "GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Noise_Only_With_ADC_Cut_Single_" + to_string(iADC + 1);
        HistTitle = "GEM-1D Noise Only nDigis Per TS distribution (Only ADC= " + to_string(iADC + 1) + ") for Run " + to_string(RunId);
        GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Noise_Only_With_ADC_Cut_Single[iADC] = new TH1D(HistTag, HistTitle, Highest_GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Single[iADC] + 101, -1, Highest_GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Single[iADC] + 100);
        GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Noise_Only_With_ADC_Cut_Single[iADC]->GetXaxis()->SetTitle("nDigis/TS");
        GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Noise_Only_With_ADC_Cut_Single[iADC]->GetYaxis()->SetTitle("Counts");
    }

    TH1D* GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Noise_Only_With_ADC_Cut_Cumulative[30];

    for (Int_t iADC = 0; iADC < 30; iADC++)
    {
        HistTag = "GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Noise_Only_With_ADC_Cut_Cumulative_" + to_string(iADC + 1);
        HistTitle = "GEM-1D Noise Only nDigis Per TS distribution (ADC<= " + to_string(iADC + 1) + " Only) for Run " + to_string(RunId);
        GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Noise_Only_With_ADC_Cut_Cumulative[iADC] = new TH1D(HistTag, HistTitle, Highest_GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Cumulative[iADC + 1] + 101, -1, Highest_GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Cumulative[iADC + 1] + 100);
        GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Noise_Only_With_ADC_Cut_Cumulative[iADC]->GetXaxis()->SetTitle("nDigis/TS");
        GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Noise_Only_With_ADC_Cut_Cumulative[iADC]->GetYaxis()->SetTitle("Counts");
    }

    for (Int_t iADC = 0; iADC < 31; iADC++) // Find Overall Highest in ADC Plots
    {
        if (Highest_GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Single_Overall < Highest_GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Single[iADC])
            Highest_GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Single_Overall = Highest_GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Single[iADC];

        if (Highest_GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Cumulative_Overall < Highest_GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Cumulative[iADC])
            Highest_GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Cumulative_Overall = Highest_GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Cumulative[iADC];

        if (Highest_GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Single_Overall < Highest_GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Single[iADC])
            Highest_GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Single_Overall = Highest_GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Single[iADC];

        if (Highest_GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Cumulative_Overall < Highest_GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Cumulative[iADC])
            Highest_GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Cumulative_Overall = Highest_GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Cumulative[iADC];

        if (Highest_GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Single_Overall < Highest_GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Single[iADC])
            Highest_GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Single_Overall = Highest_GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Single[iADC];

        if (Highest_GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Cumulative_Overall < Highest_GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Cumulative[iADC])
            Highest_GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Cumulative_Overall = Highest_GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Cumulative[iADC];

        if (Highest_GEM_Digis_Without_Trigger_Raw_No_ADC_Cut_vs_ADC_Overall < Highest_GEM_Digis_Without_Trigger_Raw_No_ADC_Cut_vs_ADC[iADC])
            Highest_GEM_Digis_Without_Trigger_Raw_No_ADC_Cut_vs_ADC_Overall = Highest_GEM_Digis_Without_Trigger_Raw_No_ADC_Cut_vs_ADC[iADC];

        if (Highest_GEM_Digis_Without_Trigger_Noise_Masked_No_ADC_Cut_vs_ADC_Overall < Highest_GEM_Digis_Without_Trigger_Noise_Masked_No_ADC_Cut_vs_ADC[iADC])
            Highest_GEM_Digis_Without_Trigger_Noise_Masked_No_ADC_Cut_vs_ADC_Overall = Highest_GEM_Digis_Without_Trigger_Noise_Masked_No_ADC_Cut_vs_ADC[iADC];

        if (Highest_GEM_Digis_Without_Trigger_Noise_Only_No_ADC_Cut_vs_ADC_Overall < Highest_GEM_Digis_Without_Trigger_Noise_Only_No_ADC_Cut_vs_ADC[iADC])
            Highest_GEM_Digis_Without_Trigger_Noise_Only_No_ADC_Cut_vs_ADC_Overall = Highest_GEM_Digis_Without_Trigger_Noise_Only_No_ADC_Cut_vs_ADC[iADC];
    }

    for (Int_t iFeb = 0; iFeb < 18; iFeb++) // Find Overall Highest in Feb Plots
    {
        if (Highest_GEM_Digis_Without_Trigger_Raw_No_ADC_Cut_vs_FEB_Overall < Highest_GEM_Digis_Without_Trigger_Raw_No_ADC_Cut_vs_FEB[iFeb])
            Highest_GEM_Digis_Without_Trigger_Raw_No_ADC_Cut_vs_FEB_Overall = Highest_GEM_Digis_Without_Trigger_Raw_No_ADC_Cut_vs_FEB[iFeb];

        if (Highest_GEM_Digis_Without_Trigger_Noise_Masked_No_ADC_Cut_vs_FEB_Overall < Highest_GEM_Digis_Without_Trigger_Noise_Masked_No_ADC_Cut_vs_FEB[iFeb])
            Highest_GEM_Digis_Without_Trigger_Noise_Masked_No_ADC_Cut_vs_FEB_Overall = Highest_GEM_Digis_Without_Trigger_Noise_Masked_No_ADC_Cut_vs_FEB[iFeb];

        if (Highest_GEM_Digis_Without_Trigger_Noise_Only_No_ADC_Cut_vs_FEB_Overall < Highest_GEM_Digis_Without_Trigger_Noise_Only_No_ADC_Cut_vs_FEB[iFeb])
            Highest_GEM_Digis_Without_Trigger_Noise_Only_No_ADC_Cut_vs_FEB_Overall = Highest_GEM_Digis_Without_Trigger_Noise_Only_No_ADC_Cut_vs_FEB[iFeb];

        // for(Int_t iADC=0;iADC<31;iADC++)
        // {
        //     if(Highest_GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Single_vs_FEB_Overall[iADC] > Highest_GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Single_vs_FEB[iADC][iFeb])
        //         Highest_GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Single_vs_FEB_Overall[iADC] = Highest_GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Single_vs_FEB[iADC][iFeb];

        //     if(Highest_GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Cumulative_vs_FEB_Overall[iADC] > Highest_GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Cumulative_vs_FEB[iADC][iFeb])
        //         Highest_GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Cumulative_vs_FEB_Overall[iADC] = Highest_GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Cumulative_vs_FEB[iADC][iFeb];

        //     if(Highest_GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Single_vs_FEB_Overall[iADC] > Highest_GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Single_vs_FEB[iADC][iFeb])
        //         Highest_GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Single_vs_FEB_Overall[iADC] = Highest_GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Single_vs_FEB[iADC][iFeb];

        //     if(Highest_GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Cumulative_vs_FEB_Overall[iADC] > Highest_GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Cumulative_vs_FEB[iADC][iFeb])
        //         Highest_GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Cumulative_vs_FEB_Overall[iADC] = Highest_GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Cumulative_vs_FEB[iADC][iFeb];

        //     if(Highest_GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Single_vs_FEB_Overall[iADC] > Highest_GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Single_vs_FEB[iADC][iFeb])
        //         Highest_GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Single_vs_FEB_Overall[iADC] = Highest_GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Single_vs_FEB[iADC][iFeb];

        //     if(Highest_GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Cumulative_vs_FEB_Overall[iADC] > Highest_GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Cumulative_vs_FEB[iADC][iFeb])
        //         Highest_GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Cumulative_vs_FEB_Overall[iADC] = Highest_GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Cumulative_vs_FEB[iADC][iFeb];
        // }
    }

    HistTag = "GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Raw_No_ADC_Cut";
    HistTitle = "GEM-2D Raw nDigis Per TS distribution (No ADC Cut) vs. Feb number for Run " + to_string(RunId);
    TH2D* GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Raw_No_ADC_Cut = new TH2D(HistTag, HistTitle, Highest_GEM_Digis_Without_Trigger_Raw_No_ADC_Cut_vs_FEB_Overall + 101, -1, Highest_GEM_Digis_Without_Trigger_Raw_No_ADC_Cut_vs_FEB_Overall + 100, 21, -1.5, 19.5);
    GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Raw_No_ADC_Cut->GetXaxis()->SetTitle("nDigis/TS");
    GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Raw_No_ADC_Cut->GetYaxis()->SetTitle("Feb number");

    TH2D* GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Raw_With_ADC_Cut_Single[31];

    for (Int_t iADC = 0; iADC < 31; iADC++)
    {
        HistTag = "GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Raw_With_ADC_Cut_Single_" + to_string(iADC + 1);
        HistTitle = "GEM-2D Raw nDigis Per TS distribution (Only ADC= " + to_string(iADC + 1) + ") vs. Feb number for Run " + to_string(RunId);
        GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Raw_With_ADC_Cut_Single[iADC] = new TH2D(HistTag, HistTitle, Highest_GEM_Digis_Without_Trigger_Raw_No_ADC_Cut_vs_FEB_Overall + 101, -1, Highest_GEM_Digis_Without_Trigger_Raw_No_ADC_Cut_vs_FEB_Overall + 100, 21, -1.5, 19.5);
        GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Raw_With_ADC_Cut_Single[iADC]->GetXaxis()->SetTitle("nDigis/TS");
        GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Raw_With_ADC_Cut_Single[iADC]->GetYaxis()->SetTitle("Feb number");
    }

    TH2D* GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Raw_With_ADC_Cut_Cumulative[30];

    for (Int_t iADC = 0; iADC < 30; iADC++)
    {
        HistTag = "GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Raw_With_ADC_Cut_Cumulative_" + to_string(iADC + 1);
        HistTitle = "GEM-2D Raw nDigis Per TS distribution (ADC<= " + to_string(iADC + 1) + " Only) vs. Feb number for Run " + to_string(RunId);
        GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Raw_With_ADC_Cut_Cumulative[iADC] = new TH2D(HistTag, HistTitle, Highest_GEM_Digis_Without_Trigger_Raw_No_ADC_Cut_vs_FEB_Overall + 101, -1, Highest_GEM_Digis_Without_Trigger_Raw_No_ADC_Cut_vs_FEB_Overall + 100, 21, -1.5, 19.5);
        GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Raw_With_ADC_Cut_Cumulative[iADC]->GetXaxis()->SetTitle("nDigis/TS");
        GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Raw_With_ADC_Cut_Cumulative[iADC]->GetYaxis()->SetTitle("Feb number");
    }

    // TH2D* GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Raw_With_ADC_Cut_Single[31];

    // for(Int_t iADC=0;iADC<31;iADC++)
    // {
    //     HistTag = "GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Raw_With_ADC_Cut_Single_"+to_string(iADC+1);
    //     HistTitle = "GEM-2D Raw nDigis Per TS distribution (Only ADC= "+ to_string(iADC+1) +") vs. Feb number for Run "+to_string(RunId);
    //     GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Raw_With_ADC_Cut_Single[iADC] = new TH2D(HistTag,HistTitle,Highest_GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Single_vs_FEB_Overall[iADC]+101,-1,Highest_GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Single_vs_FEB_Overall[iADC]+100,21,-1.5,19.5);
    //     GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Raw_With_ADC_Cut_Single[iADC]->GetXaxis()->SetTitle("nDigis/TS");
    //     GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Raw_With_ADC_Cut_Single[iADC]->GetYaxis()->SetTitle("Feb number");
    // }

    // TH2D* GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Raw_With_ADC_Cut_Cumulative[30];

    // for(Int_t iADC=0;iADC<30;iADC++)
    // {
    //     HistTag = "GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Raw_With_ADC_Cut_Cumulative_"+to_string(iADC+1);
    //     HistTitle = "GEM-2D Raw nDigis Per TS distribution (ADC<= "+ to_string(iADC+1) +" Only) vs. Feb number for Run "+to_string(RunId);
    //     GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Raw_With_ADC_Cut_Cumulative[iADC] = new TH2D(HistTag,HistTitle,Highest_GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Cumulative_vs_FEB_Overall[iADC]+101,-1,Highest_GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Cumulative_vs_FEB_Overall[iADC]+100,21,-1.5,19.5);
    //     GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Raw_With_ADC_Cut_Cumulative[iADC]->GetXaxis()->SetTitle("nDigis/TS");
    //     GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Raw_With_ADC_Cut_Cumulative[iADC]->GetYaxis()->SetTitle("Feb number");
    // }

    HistTag = "GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Masked_No_ADC_Cut";
    HistTitle = "GEM-2D Noise Masked nDigis Per TS distribution (No ADC Cut) vs. Feb number for Run " + to_string(RunId);
    TH2D* GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Masked_No_ADC_Cut = new TH2D(HistTag, HistTitle, Highest_GEM_Digis_Without_Trigger_Noise_Masked_No_ADC_Cut_vs_FEB_Overall + 101, -1, Highest_GEM_Digis_Without_Trigger_Noise_Masked_No_ADC_Cut_vs_FEB_Overall + 100, 21, -1.5, 19.5);
    GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Masked_No_ADC_Cut->GetXaxis()->SetTitle("nDigis/TS");
    GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Masked_No_ADC_Cut->GetYaxis()->SetTitle("Feb number");

    TH2D* GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Masked_With_ADC_Cut_Single[31];

    for (Int_t iADC = 0; iADC < 31; iADC++)
    {
        HistTag = "GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Masked_With_ADC_Cut_Single_" + to_string(iADC + 1);
        HistTitle = "GEM-2D Noise Masked nDigis Per TS distribution (Only ADC= " + to_string(iADC + 1) + ") vs. Feb number for Run " + to_string(RunId);
        GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Masked_With_ADC_Cut_Single[iADC] = new TH2D(HistTag, HistTitle, Highest_GEM_Digis_Without_Trigger_Noise_Masked_No_ADC_Cut_vs_FEB_Overall + 101, -1, Highest_GEM_Digis_Without_Trigger_Noise_Masked_No_ADC_Cut_vs_FEB_Overall + 100, 21, -1.5, 19.5);
        GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Masked_With_ADC_Cut_Single[iADC]->GetXaxis()->SetTitle("nDigis/TS");
        GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Masked_With_ADC_Cut_Single[iADC]->GetYaxis()->SetTitle("Feb number");
    }

    TH2D* GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Masked_With_ADC_Cut_Cumulative[30];

    for (Int_t iADC = 0; iADC < 30; iADC++)
    {
        HistTag = "GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Masked_With_ADC_Cut_Cumulative_" + to_string(iADC + 1);
        HistTitle = "GEM-2D Noise Masked nDigis Per TS distribution (ADC<= " + to_string(iADC + 1) + " Only) vs. Feb number for Run " + to_string(RunId);
        GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Masked_With_ADC_Cut_Cumulative[iADC] = new TH2D(HistTag, HistTitle, Highest_GEM_Digis_Without_Trigger_Noise_Masked_No_ADC_Cut_vs_FEB_Overall + 101, -1, Highest_GEM_Digis_Without_Trigger_Noise_Masked_No_ADC_Cut_vs_FEB_Overall + 100, 21, -1.5, 19.5);
        GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Masked_With_ADC_Cut_Cumulative[iADC]->GetXaxis()->SetTitle("nDigis/TS");
        GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Masked_With_ADC_Cut_Cumulative[iADC]->GetYaxis()->SetTitle("Feb number");
    }


    // TH2D* GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Masked_With_ADC_Cut_Single[31];

    // for(Int_t iADC=0;iADC<31;iADC++)
    // {
    //     HistTag = "GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Masked_With_ADC_Cut_Single_"+to_string(iADC+1);
    //     HistTitle = "GEM-2D Noise Masked nDigis Per TS distribution (Only ADC= "+ to_string(iADC+1) +") vs. Feb number for Run "+to_string(RunId);
    //     GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Masked_With_ADC_Cut_Single[iADC] = new TH2D(HistTag,HistTitle,Highest_GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Single_vs_FEB_Overall[iADC]+101,-1,Highest_GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Single_vs_FEB_Overall[iADC]+100,21,-1.5,19.5);
    //     GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Masked_With_ADC_Cut_Single[iADC]->GetXaxis()->SetTitle("nDigis/TS");
    //     GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Masked_With_ADC_Cut_Single[iADC]->GetYaxis()->SetTitle("Feb number");
    // }

    // TH2D* GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Masked_With_ADC_Cut_Cumulative[30];

    // for(Int_t iADC=0;iADC<30;iADC++)
    // {
    //     HistTag = "GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Masked_With_ADC_Cut_Cumulative_"+to_string(iADC+1);
    //     HistTitle = "GEM-2D Noise Masked nDigis Per TS distribution (ADC<= "+ to_string(iADC+1) +" Only) vs. Feb number for Run "+to_string(RunId);
    //     GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Masked_With_ADC_Cut_Cumulative[iADC] = new TH2D(HistTag,HistTitle,Highest_GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Cumulative_vs_FEB_Overall[iADC]+101,-1,Highest_GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Cumulative_vs_FEB_Overall[iADC]+100,21,-1.5,19.5);
    //     GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Masked_With_ADC_Cut_Cumulative[iADC]->GetXaxis()->SetTitle("nDigis/TS");
    //     GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Masked_With_ADC_Cut_Cumulative[iADC]->GetYaxis()->SetTitle("Feb number");
    // }

    HistTag = "GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Only_No_ADC_Cut";
    HistTitle = "GEM-2D Noise Only nDigis Per TS distribution (No ADC Cut) vs. Feb number for Run " + to_string(RunId);
    TH2D* GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Only_No_ADC_Cut = new TH2D(HistTag, HistTitle, Highest_GEM_Digis_Without_Trigger_Noise_Only_No_ADC_Cut_vs_FEB_Overall + 101, -1, Highest_GEM_Digis_Without_Trigger_Noise_Only_No_ADC_Cut_vs_FEB_Overall + 100, 21, -1.5, 19.5);
    GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Only_No_ADC_Cut->GetXaxis()->SetTitle("nDigis/TS");
    GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Only_No_ADC_Cut->GetYaxis()->SetTitle("Feb number");

    TH2D* GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Only_With_ADC_Cut_Single[31];

    for (Int_t iADC = 0; iADC < 31; iADC++)
    {
        HistTag = "GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Only_With_ADC_Cut_Single_" + to_string(iADC + 1);
        HistTitle = "GEM-2D Noise Only nDigis Per TS distribution (Only ADC= " + to_string(iADC + 1) + ") vs. Feb number for Run " + to_string(RunId);
        GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Only_With_ADC_Cut_Single[iADC] = new TH2D(HistTag, HistTitle, Highest_GEM_Digis_Without_Trigger_Noise_Only_No_ADC_Cut_vs_FEB_Overall + 101, -1, Highest_GEM_Digis_Without_Trigger_Noise_Only_No_ADC_Cut_vs_FEB_Overall + 100, 21, -1.5, 19.5);
        GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Only_With_ADC_Cut_Single[iADC]->GetXaxis()->SetTitle("nDigis/TS");
        GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Only_With_ADC_Cut_Single[iADC]->GetYaxis()->SetTitle("Feb number");
    }

    TH2D* GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Only_With_ADC_Cut_Cumulative[30];

    for (Int_t iADC = 0; iADC < 30; iADC++)
    {
        HistTag = "GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Only_With_ADC_Cut_Cumulative_" + to_string(iADC + 1);
        HistTitle = "GEM-2D Noise Only nDigis Per TS distribution (ADC<= " + to_string(iADC + 1) + " Only) vs. Feb number for Run " + to_string(RunId);
        GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Only_With_ADC_Cut_Cumulative[iADC] = new TH2D(HistTag, HistTitle, Highest_GEM_Digis_Without_Trigger_Noise_Only_No_ADC_Cut_vs_FEB_Overall + 101, -1, Highest_GEM_Digis_Without_Trigger_Noise_Only_No_ADC_Cut_vs_FEB_Overall + 100, 21, -1.5, 19.5);
        GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Only_With_ADC_Cut_Cumulative[iADC]->GetXaxis()->SetTitle("nDigis/TS");
        GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Only_With_ADC_Cut_Cumulative[iADC]->GetYaxis()->SetTitle("Feb number");
    }

    // TH2D* GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Only_With_ADC_Cut_Single[31];

    // for(Int_t iADC=0;iADC<31;iADC++)
    // {
    //     HistTag = "GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Only_With_ADC_Cut_Single_"+to_string(iADC+1);
    //     HistTitle = "GEM-2D Noise Only nDigis Per TS distribution (Only ADC= "+ to_string(iADC+1) +") vs. Feb number for Run "+to_string(RunId);
    //     GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Only_With_ADC_Cut_Single[iADC] = new TH2D(HistTag,HistTitle,Highest_GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Single_vs_FEB_Overall[iADC]+101,-1,Highest_GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Single_vs_FEB_Overall[iADC]+100,21,-1.5,19.5);
    //     GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Only_With_ADC_Cut_Single[iADC]->GetXaxis()->SetTitle("nDigis/TS");
    //     GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Only_With_ADC_Cut_Single[iADC]->GetYaxis()->SetTitle("Feb number");
    // }

    // TH2D* GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Only_With_ADC_Cut_Cumulative[30];

    // for(Int_t iADC=0;iADC<30;iADC++)
    // {
    //     HistTag = "GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Only_With_ADC_Cut_Cumulative_"+to_string(iADC+1);
    //     HistTitle = "GEM-2D Noise Only nDigis Per TS distribution (ADC<= "+ to_string(iADC+1) +" Only) vs. Feb number for Run "+to_string(RunId);
    //     GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Only_With_ADC_Cut_Cumulative[iADC] = new TH2D(HistTag,HistTitle,Highest_GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Cumulative_vs_FEB_Overall[iADC]+101,-1,Highest_GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Cumulative_vs_FEB_Overall[iADC]+100,21,-1.5,19.5);
    //     GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Only_With_ADC_Cut_Cumulative[iADC]->GetXaxis()->SetTitle("nDigis/TS");
    //     GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Only_With_ADC_Cut_Cumulative[iADC]->GetYaxis()->SetTitle("Feb number");
    // }

    HistTag = "GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_ADC_Raw";
    HistTitle = "GEM-2D Raw nDigis Per TS distribution vs. ADC for Run " + to_string(RunId);
    TH2D* GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_ADC_Raw = new TH2D(HistTag, HistTitle, Highest_GEM_Digis_Without_Trigger_Raw_No_ADC_Cut_vs_ADC_Overall + 101, -1, Highest_GEM_Digis_Without_Trigger_Raw_No_ADC_Cut_vs_ADC_Overall + 100, 34, -1.5, 32.5);
    GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_ADC_Raw->GetXaxis()->SetTitle("nDigis/TS");
    GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_ADC_Raw->GetYaxis()->SetTitle("ADC");

    HistTag = "GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_ADC_Noise_Masked";
    HistTitle = "GEM-2D Noise Masked nDigis Per TS distribution vs. ADC for Run " + to_string(RunId);
    TH2D* GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_ADC_Noise_Masked = new TH2D(HistTag, HistTitle, Highest_GEM_Digis_Without_Trigger_Noise_Masked_No_ADC_Cut_vs_ADC_Overall + 101, -1, Highest_GEM_Digis_Without_Trigger_Noise_Masked_No_ADC_Cut_vs_ADC_Overall + 100, 34, -1.5, 32.5);
    GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_ADC_Noise_Masked->GetXaxis()->SetTitle("nDigis/TS");
    GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_ADC_Noise_Masked->GetYaxis()->SetTitle("ADC");

    HistTag = "GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_ADC_Noise_Only";
    HistTitle = "GEM-2D Noise Only nDigis Per TS distribution vs. ADC for Run " + to_string(RunId);
    TH2D* GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_ADC_Noise_Only = new TH2D(HistTag, HistTitle, Highest_GEM_Digis_Without_Trigger_Noise_Only_No_ADC_Cut_vs_ADC_Overall + 101, -1, Highest_GEM_Digis_Without_Trigger_Noise_Only_No_ADC_Cut_vs_ADC_Overall + 100, 34, -1.5, 32.5);
    GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_ADC_Noise_Only->GetXaxis()->SetTitle("nDigis/TS");
    GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_ADC_Noise_Only->GetYaxis()->SetTitle("ADC");

    for (Int_t iTimeSlice = 0; iTimeSlice < nTimeSlices; iTimeSlice++)
    {
        if (GEM_Digis_Without_Trigger_Raw_No_ADC_Cut[iTimeSlice] != 0)
            GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Raw_No_ADC_Cut->Fill(GEM_Digis_Without_Trigger_Raw_No_ADC_Cut[iTimeSlice]);
        if (GEM_Digis_Without_Trigger_Noise_Masked_No_ADC_Cut[iTimeSlice] != 0)
            GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Noise_Masked_No_ADC_Cut->Fill(GEM_Digis_Without_Trigger_Noise_Masked_No_ADC_Cut[iTimeSlice]);
        if (GEM_Digis_Without_Trigger_Noise_Only_No_ADC_Cut[iTimeSlice] != 0)
            GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Noise_Only_No_ADC_Cut->Fill(GEM_Digis_Without_Trigger_Noise_Only_No_ADC_Cut[iTimeSlice]);

        for (Int_t iADC = 0; iADC < 31; iADC++)
        {
            if (GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Single[iADC][iTimeSlice] != 0)
                GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Raw_With_ADC_Cut_Single[iADC]->Fill(GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Single[iADC][iTimeSlice]);
            if (GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Single[iADC][iTimeSlice] != 0)
                GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Noise_Masked_With_ADC_Cut_Single[iADC]->Fill(GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Single[iADC][iTimeSlice]);
            if (GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Single[iADC][iTimeSlice] != 0)
                GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Noise_Only_With_ADC_Cut_Single[iADC]->Fill(GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Single[iADC][iTimeSlice]);
            if (GEM_Digis_Without_Trigger_Raw_No_ADC_Cut_vs_ADC[iADC][iTimeSlice] != 0)
                GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_ADC_Raw->Fill(GEM_Digis_Without_Trigger_Raw_No_ADC_Cut_vs_ADC[iADC][iTimeSlice], iADC);
            if (GEM_Digis_Without_Trigger_Noise_Masked_No_ADC_Cut_vs_ADC[iADC][iTimeSlice] != 0)
                GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_ADC_Noise_Masked->Fill(GEM_Digis_Without_Trigger_Noise_Masked_No_ADC_Cut_vs_ADC[iADC][iTimeSlice], iADC);
            if (GEM_Digis_Without_Trigger_Noise_Only_No_ADC_Cut_vs_ADC[iADC][iTimeSlice] != 0)
                GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_ADC_Noise_Only->Fill(GEM_Digis_Without_Trigger_Noise_Only_No_ADC_Cut_vs_ADC[iADC][iTimeSlice], iADC);
        }

        for (Int_t iADC = 0; iADC < 30; iADC++)
        {
            if (GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Cumulative[iADC][iTimeSlice] != 0)
                GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Raw_With_ADC_Cut_Cumulative[iADC]->Fill(GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Cumulative[iADC][iTimeSlice]);
            if (GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Cumulative[iADC][iTimeSlice] != 0)
                GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Noise_Masked_With_ADC_Cut_Cumulative[iADC]->Fill(GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Cumulative[iADC][iTimeSlice]);
            if (GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Cumulative[iADC][iTimeSlice] != 0)
                GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Noise_Only_With_ADC_Cut_Cumulative[iADC]->Fill(GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Cumulative[iADC][iTimeSlice]);
        }

        for (Int_t iFeb = 0; iFeb < 18; iFeb++)
        {
            if (GEM_Digis_Without_Trigger_Raw_No_ADC_Cut_vs_FEB[iFeb][iTimeSlice] != 0)
                GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Raw_No_ADC_Cut->Fill(GEM_Digis_Without_Trigger_Raw_No_ADC_Cut_vs_FEB[iFeb][iTimeSlice], iFeb);
            if (GEM_Digis_Without_Trigger_Noise_Masked_No_ADC_Cut_vs_FEB[iFeb][iTimeSlice] != 0)
                GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Masked_No_ADC_Cut->Fill(GEM_Digis_Without_Trigger_Noise_Masked_No_ADC_Cut_vs_FEB[iFeb][iTimeSlice], iFeb);
            if (GEM_Digis_Without_Trigger_Noise_Only_No_ADC_Cut_vs_FEB[iFeb][iTimeSlice] != 0)
                GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Only_No_ADC_Cut->Fill(GEM_Digis_Without_Trigger_Noise_Only_No_ADC_Cut_vs_FEB[iFeb][iTimeSlice], iFeb);

            // for(Int_t iADC=0;iADC<31;iADC++)
            // {
            //     GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Raw_With_ADC_Cut_Single[iADC]->Fill(GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Single_vs_FEB[iADC][iFeb][iTimeSlice],iFeb);
            //     GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Masked_With_ADC_Cut_Single[iADC]->Fill(GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Single_vs_FEB[iADC][iFeb][iTimeSlice],iFeb);
            //     GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Only_With_ADC_Cut_Single[iADC]->Fill(GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Single_vs_FEB[iADC][iFeb][iTimeSlice],iFeb);
            // }

            // for(Int_t iADC=0;iADC<30;iADC++)
            // {
            //     GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Raw_With_ADC_Cut_Cumulative[iADC]->Fill(GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Cumulative_vs_FEB[iADC][iFeb][iTimeSlice],iFeb);
            //     GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Masked_With_ADC_Cut_Cumulative[iADC]->Fill(GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Cumulative_vs_FEB[iADC][iFeb][iTimeSlice],iFeb);
            //     GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Only_With_ADC_Cut_Cumulative[iADC]->Fill(GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Cumulative_vs_FEB[iADC][iFeb][iTimeSlice],iFeb);
            // }
        }
    }

    for (iTreeEntry = 0; iTreeEntry < nTimeSlices; iTreeEntry++)
    {
        Tree_Data_Reader.SetEntry(iTreeEntry);
        uint64_t TimeSlice_Start_Time = TimeSlice_Event_Header->GetTsStartTime();

        Int_t iTimeSlice = TimeSlice_Event_Header->GetTsIndex();

        Int_t GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Single_vs_FEB[31][18] = {0};
        Int_t GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Cumulative_vs_FEB[31][18] = {0};

        Int_t GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Single_vs_FEB[31][18] = {0};
        Int_t GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Cumulative_vs_FEB[31][18] = {0};

        Int_t GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Single_vs_FEB[31][18] = {0};
        Int_t GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Cumulative_vs_FEB[31][18] = {0};

        // if(iTreeEntry%100 == 0)
        cout << "iTreeEntry: " << iTreeEntry << "  TS Index: " << iTimeSlice << endl;

        for (Int_t iMuchDigi = 0; iMuchDigi < Much_Digi.GetSize(); iMuchDigi++)
        {
            int32_t Station = CbmMuchAddress::GetStationIndex(Much_Digi[iMuchDigi].GetAddress());
            if (Station != 0)
                continue; // Selecting GEM Only
            int32_t Sector = CbmMuchAddress::GetSectorIndex(Much_Digi[iMuchDigi].GetAddress());
            int32_t Channel = CbmMuchAddress::GetChannelIndex(Much_Digi[iMuchDigi].GetAddress());
            if (Sector > 96 || Channel > 22)
                continue;
            Int_t GEM_FEB = GEM_Feb_no_X_Y[Channel][Sector];
            Int_t GEM_CH = GEM_Ch_no_X_Y[Channel][Sector];
            Int_t ADC_Value = Much_Digi[iMuchDigi].GetAdc() - 1; // One subtracted since ADC start from 1 while array starts from 0

            GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Single_vs_FEB[ADC_Value][GEM_FEB]++;
            for (Int_t iADC = ADC_Value + 1; iADC < 30; iADC++)
                GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Cumulative_vs_FEB[iADC][GEM_FEB]++;

            if (GEM_Noise[Channel][Sector] != -1)
            {
                GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Single_vs_FEB[ADC_Value][GEM_FEB]++;
                for (Int_t iADC = ADC_Value + 1; iADC < 30; iADC++)
                    GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Cumulative_vs_FEB[iADC][GEM_FEB]++;
            }
            else
            {
                GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Single_vs_FEB[ADC_Value][GEM_FEB]++;
                for (Int_t iADC = ADC_Value + 1; iADC < 30; iADC++)
                    GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Cumulative_vs_FEB[iADC][GEM_FEB]++;
            }
        }

        for (Int_t iADC = 0; iADC < 31; iADC++)
        {
            for (Int_t iFeb = 0; iFeb < 18; iFeb++)
            {
                if (GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Single_vs_FEB[iADC][iFeb] != 0)
                    GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Raw_With_ADC_Cut_Single[iADC]->Fill(GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Single_vs_FEB[iADC][iFeb], iFeb);
                if (GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Single_vs_FEB[iADC][iFeb] != 0)
                    GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Masked_With_ADC_Cut_Single[iADC]->Fill(GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Single_vs_FEB[iADC][iFeb], iFeb);
                if (GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Single_vs_FEB[iADC][iFeb] != 0)
                    GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Only_With_ADC_Cut_Single[iADC]->Fill(GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Single_vs_FEB[iADC][iFeb], iFeb);
            }
        }

        for (Int_t iADC = 0; iADC < 30; iADC++)
        {
            for (Int_t iFeb = 0; iFeb < 18; iFeb++)
            {
                if (GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Cumulative_vs_FEB[iADC][iFeb] != 0)
                    GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Raw_With_ADC_Cut_Cumulative[iADC]->Fill(GEM_Digis_Without_Trigger_Raw_With_ADC_Cut_Cumulative_vs_FEB[iADC][iFeb], iFeb);
                if (GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Cumulative_vs_FEB[iADC][iFeb] != 0)
                    GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Masked_With_ADC_Cut_Cumulative[iADC]->Fill(GEM_Digis_Without_Trigger_Noise_Masked_With_ADC_Cut_Cumulative_vs_FEB[iADC][iFeb], iFeb);
                if (GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Cumulative_vs_FEB[iADC][iFeb] != 0)
                    GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Only_With_ADC_Cut_Cumulative[iADC]->Fill(GEM_Digis_Without_Trigger_Noise_Only_With_ADC_Cut_Cumulative_vs_FEB[iADC][iFeb], iFeb);
            }
        }
    }

    fOut->mkdir("nDigis Per TS");
    fOut->mkdir("nDigis Per TS/GEM");
    fOut->mkdir("nDigis Per TS/GEM/Without Trigger");
    fOut->mkdir("nDigis Per TS/GEM/Without Trigger/1D");
    fOut->mkdir("nDigis Per TS/GEM/Without Trigger/1D/Raw");
    fOut->mkdir("nDigis Per TS/GEM/Without Trigger/1D/Raw/No ADC Cut");
    fOut->mkdir("nDigis Per TS/GEM/Without Trigger/1D/Raw/With ADC Cut");
    fOut->mkdir("nDigis Per TS/GEM/Without Trigger/1D/Raw/With ADC Cut/Single ADC Cut");
    fOut->mkdir("nDigis Per TS/GEM/Without Trigger/1D/Raw/With ADC Cut/Cumulative");
    fOut->mkdir("nDigis Per TS/GEM/Without Trigger/1D/Noise Masked");
    fOut->mkdir("nDigis Per TS/GEM/Without Trigger/1D/Noise Masked/No ADC Cut");
    fOut->mkdir("nDigis Per TS/GEM/Without Trigger/1D/Noise Masked/With ADC Cut");
    fOut->mkdir("nDigis Per TS/GEM/Without Trigger/1D/Noise Masked/With ADC Cut/Single ADC Cut");
    fOut->mkdir("nDigis Per TS/GEM/Without Trigger/1D/Noise Masked/With ADC Cut/Cumulative");
    fOut->mkdir("nDigis Per TS/GEM/Without Trigger/1D/Noise Only");
    fOut->mkdir("nDigis Per TS/GEM/Without Trigger/1D/Noise Only/No ADC Cut");
    fOut->mkdir("nDigis Per TS/GEM/Without Trigger/1D/Noise Only/With ADC Cut");
    fOut->mkdir("nDigis Per TS/GEM/Without Trigger/1D/Noise Only/With ADC Cut/Single ADC Cut");
    fOut->mkdir("nDigis Per TS/GEM/Without Trigger/1D/Noise Only/With ADC Cut/Cumulative");
    fOut->mkdir("nDigis Per TS/GEM/Without Trigger/2D");
    fOut->mkdir("nDigis Per TS/GEM/Without Trigger/2D/vs Feb number");
    fOut->mkdir("nDigis Per TS/GEM/Without Trigger/2D/vs Feb number/Raw");
    fOut->mkdir("nDigis Per TS/GEM/Without Trigger/2D/vs Feb number/Raw/No ADC Cut");
    fOut->mkdir("nDigis Per TS/GEM/Without Trigger/2D/vs Feb number/Raw/With ADC Cut");
    fOut->mkdir("nDigis Per TS/GEM/Without Trigger/2D/vs Feb number/Raw/With ADC Cut/Single ADC Cut");
    fOut->mkdir("nDigis Per TS/GEM/Without Trigger/2D/vs Feb number/Raw/With ADC Cut/Cumulative");
    fOut->mkdir("nDigis Per TS/GEM/Without Trigger/2D/vs Feb number/Noise Masked");
    fOut->mkdir("nDigis Per TS/GEM/Without Trigger/2D/vs Feb number/Noise Masked/No ADC Cut");
    fOut->mkdir("nDigis Per TS/GEM/Without Trigger/2D/vs Feb number/Noise Masked/With ADC Cut");
    fOut->mkdir("nDigis Per TS/GEM/Without Trigger/2D/vs Feb number/Noise Masked/With ADC Cut/Single ADC Cut");
    fOut->mkdir("nDigis Per TS/GEM/Without Trigger/2D/vs Feb number/Noise Masked/With ADC Cut/Cumulative");
    fOut->mkdir("nDigis Per TS/GEM/Without Trigger/2D/vs Feb number/Noise Only");
    fOut->mkdir("nDigis Per TS/GEM/Without Trigger/2D/vs Feb number/Noise Only/No ADC Cut");
    fOut->mkdir("nDigis Per TS/GEM/Without Trigger/2D/vs Feb number/Noise Only/With ADC Cut");
    fOut->mkdir("nDigis Per TS/GEM/Without Trigger/2D/vs Feb number/Noise Only/With ADC Cut/Single ADC Cut");
    fOut->mkdir("nDigis Per TS/GEM/Without Trigger/2D/vs Feb number/Noise Only/With ADC Cut/Cumulative");
    fOut->mkdir("nDigis Per TS/GEM/Without Trigger/2D/vs ADC");
    fOut->mkdir("nDigis Per TS/GEM/Without Trigger/2D/vs ADC/Raw");
    fOut->mkdir("nDigis Per TS/GEM/Without Trigger/2D/vs ADC/Noise Masked");
    fOut->mkdir("nDigis Per TS/GEM/Without Trigger/2D/vs ADC/Noise Only");


    fOut->cd("nDigis Per TS/GEM/Without Trigger/1D/Raw/No ADC Cut");
    GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Raw_No_ADC_Cut->Write();

    fOut->cd("nDigis Per TS/GEM/Without Trigger/1D/Raw/With ADC Cut/Single ADC Cut");
    for (Int_t iADC = 0; iADC < 31; iADC++)
        GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Raw_With_ADC_Cut_Single[iADC]->Write();

    fOut->cd("nDigis Per TS/GEM/Without Trigger/1D/Raw/With ADC Cut/Cumulative");
    for (Int_t iADC = 0; iADC < 30; iADC++)
        GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Raw_With_ADC_Cut_Cumulative[iADC]->Write();

    fOut->cd("nDigis Per TS/GEM/Without Trigger/1D/Noise Masked/No ADC Cut");
    GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Noise_Masked_No_ADC_Cut->Write();

    fOut->cd("nDigis Per TS/GEM/Without Trigger/1D/Noise Masked/With ADC Cut/Single ADC Cut");
    for (Int_t iADC = 0; iADC < 31; iADC++)
        GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Noise_Masked_With_ADC_Cut_Single[iADC]->Write();

    fOut->cd("nDigis Per TS/GEM/Without Trigger/1D/Noise Masked/With ADC Cut/Cumulative");
    for (Int_t iADC = 0; iADC < 30; iADC++)
        GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Noise_Masked_With_ADC_Cut_Cumulative[iADC]->Write();

    fOut->cd("nDigis Per TS/GEM/Without Trigger/1D/Noise Only/No ADC Cut");
    GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Noise_Only_No_ADC_Cut->Write();

    fOut->cd("nDigis Per TS/GEM/Without Trigger/1D/Noise Only/With ADC Cut/Single ADC Cut");
    for (Int_t iADC = 0; iADC < 31; iADC++)
        GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Noise_Only_With_ADC_Cut_Single[iADC]->Write();

    fOut->cd("nDigis Per TS/GEM/Without Trigger/1D/Noise Only/With ADC Cut/Cumulative");
    for (Int_t iADC = 0; iADC < 30; iADC++)
        GEM_nDigis_Per_TS_Without_Scintillator_Trigger_Noise_Only_With_ADC_Cut_Cumulative[iADC]->Write();

    fOut->cd("nDigis Per TS/GEM/Without Trigger/2D/vs Feb number/Raw/No ADC Cut");
    GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Raw_No_ADC_Cut->Write();

    fOut->cd("nDigis Per TS/GEM/Without Trigger/2D/vs Feb number/Raw/With ADC Cut/Single ADC Cut");
    for (Int_t iADC = 0; iADC < 31; iADC++)
        GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Raw_With_ADC_Cut_Single[iADC]->Write();

    fOut->cd("nDigis Per TS/GEM/Without Trigger/2D/vs Feb number/Raw/With ADC Cut/Cumulative");
    for (Int_t iADC = 0; iADC < 30; iADC++)
        GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Raw_With_ADC_Cut_Cumulative[iADC]->Write();

    fOut->cd("nDigis Per TS/GEM/Without Trigger/2D/vs Feb number/Noise Masked/No ADC Cut");
    GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Masked_No_ADC_Cut->Write();

    fOut->cd("nDigis Per TS/GEM/Without Trigger/2D/vs Feb number/Noise Masked/With ADC Cut/Single ADC Cut");
    for (Int_t iADC = 0; iADC < 31; iADC++)
        GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Masked_With_ADC_Cut_Single[iADC]->Write();

    fOut->cd("nDigis Per TS/GEM/Without Trigger/2D/vs Feb number/Noise Masked/With ADC Cut/Cumulative");
    for (Int_t iADC = 0; iADC < 30; iADC++)
        GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Masked_With_ADC_Cut_Cumulative[iADC]->Write();

    fOut->cd("nDigis Per TS/GEM/Without Trigger/2D/vs Feb number/Noise Only/No ADC Cut");
    GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Only_No_ADC_Cut->Write();

    fOut->cd("nDigis Per TS/GEM/Without Trigger/2D/vs Feb number/Noise Only/With ADC Cut/Single ADC Cut");
    for (Int_t iADC = 0; iADC < 31; iADC++)
        GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Only_With_ADC_Cut_Single[iADC]->Write();

    fOut->cd("nDigis Per TS/GEM/Without Trigger/2D/vs Feb number/Noise Only/With ADC Cut/Cumulative");
    for (Int_t iADC = 0; iADC < 30; iADC++)
        GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_Feb_number_Noise_Only_With_ADC_Cut_Cumulative[iADC]->Write();

    fOut->cd("nDigis Per TS/GEM/Without Trigger/2D/vs ADC/Raw");
    GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_ADC_Raw->Write();

    fOut->cd("nDigis Per TS/GEM/Without Trigger/2D/vs ADC/Noise Masked");
    GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_ADC_Noise_Masked->Write();

    fOut->cd("nDigis Per TS/GEM/Without Trigger/2D/vs ADC/Noise Only");
    GEM_nDigis_Per_TS_Without_Scintillator_Trigger_vs_ADC_Noise_Only->Write();

    fIn->Close();
    fOut->Close();

    cout << "Output File Name: " << OutputFileName << endl;

    Code_Timer.Stop();

    cout << "_______________________Code Executed Successfully!________________________" << endl;
    cout << "Real time: " << Code_Timer.RealTime() / 60.0 << " m, CPU time: " << Code_Timer.CpuTime() / 60.0 << " m" << endl;
}

void Highlight_RPC_Pad(Int_t Column_number, Int_t Sector_number, TH2F*& PadXandY)
{
    if (Sector_number >= 0 && Column_number >= 0)
    {
        Float_t Radial_Coordinate_initial = 29.056, Radial_Coordinate_final = 29.056, Theta_Coordinate_initial, Theta_Coordinate_final;
        Theta_Coordinate_initial = ((Float_t)(9 - Column_number) * 2.0 - 10.0 + 90.0);
        Theta_Coordinate_final = ((Float_t)(9 - Column_number) * 2.0 - 8.0 + 90.0);
        Float_t Pi = 3.17585;
        Float_t X_offset = 0.0, Y_offset = -50.0;
        Float_t X_Coordinate, Y_Coordinate;

        Float_t Theta_increment;

        //   if(Sector_number>40) Theta_increment = 0.08;
        //   else if (Sector_number > 33) Theta_increment = 0.075;
        //   else if (Sector_number >20) Theta_increment = 0.05;
        //   else if (Sector_number >10) Theta_increment = 0.04;
        //   else if (Sector_number > 5) Theta_increment = 0.03;
        //   else Theta_increment = 0.025;

        if (Sector_number < 5)
            Theta_increment = 0.08;
        else if (Sector_number < 10)
            Theta_increment = 0.075;
        else if (Sector_number < 20)
            Theta_increment = 0.05;
        else if (Sector_number < 33)
            Theta_increment = 0.04;
        else if (Sector_number < 40)
            Theta_increment = 0.035;
        else
            Theta_increment = 0.03;

        for (Int_t i = 0; i < 46; i++)
        {
            Radial_Coordinate_final = Radial_Coordinate_initial + Radial_Coordinate_initial * Pi / 90.0;

            //  cout<< Radial_Coordinate_initial << "   " <<Radial_Coordinate_final << "   " << Radial_Coordinate_final - Radial_Coordinate_initial <<endl;
            if (i == (Sector_number))
                break;

            Radial_Coordinate_initial = Radial_Coordinate_final;
        }

        Float_t Theta_Coordinate_initial_copy = Theta_Coordinate_initial;

        Radial_Coordinate_initial = Radial_Coordinate_initial + 0.01;

        for (; Radial_Coordinate_initial < Radial_Coordinate_final - 0.022; Radial_Coordinate_initial += 0.05)
        {
            for (Theta_Coordinate_initial = Theta_Coordinate_initial_copy + (Theta_increment / 2.0); Theta_Coordinate_initial < Theta_Coordinate_final - 0.005; Theta_Coordinate_initial += Theta_increment)
            {
                X_Coordinate = Radial_Coordinate_initial * TMath::Sin(Theta_Coordinate_initial * 3.14159 / 180.0);
                Y_Coordinate = Radial_Coordinate_initial * TMath::Cos(Theta_Coordinate_initial * 3.14159 / 180.0);
                PadXandY->Fill(X_offset + X_Coordinate, Y_offset + Y_Coordinate);
            }
        }
    }
}

void Highlight_Pad(Int_t Column_number, Int_t Sector_number, Int_t Weight, TH2F*& PadXandY, Bool_t Wall_Near_Smaller_Pads)
{
    if (Sector_number >= 0 && Column_number >= 0)
    {
        if (Wall_Near_Smaller_Pads)
        {
            Float_t Radial_Coordinate_initial = 18.30, Radial_Coordinate_final = 18.3, Theta_Coordinate_initial, Theta_Coordinate_final;
            Theta_Coordinate_initial = ((Float_t)22 - Column_number) - 90.0 - 23.0 / 2.0;
            Theta_Coordinate_final = (Float_t)(22 - (Column_number - 1)) - 90.0 - 23.0 / 2.0;
            Float_t Pi = 3.1468;
            Float_t X_offset = -19.4103 + 60.0, Y_offset = 75.1912 + 85.0;
            Float_t X_Coordinate, Y_Coordinate;

            Float_t Theta_increment = 0.0;

            if (Sector_number > 25 && Sector_number < 50)
                Theta_increment = 0.075;
            else if (Sector_number >= 50 && Sector_number < 80)
                Theta_increment = 0.05;
            else if (Sector_number >= 80)
                Theta_increment = 0.04;
            else
                Theta_increment = 0.15;

            for (Int_t i = 0; i < 97; i++)
            {
                Radial_Coordinate_final = Radial_Coordinate_initial + Radial_Coordinate_initial * Pi / 180;

                if (i == Sector_number)
                    break;

                Radial_Coordinate_initial = Radial_Coordinate_final;
            }

            Float_t Theta_Coordinate_initial_copy = Theta_Coordinate_initial;

            Radial_Coordinate_initial = Radial_Coordinate_initial + 0.01;

            for (; Radial_Coordinate_initial < Radial_Coordinate_final - 0.025; Radial_Coordinate_initial += 0.1)
            {
                for (Theta_Coordinate_initial = Theta_Coordinate_initial_copy + (Theta_increment / 2.0); Theta_Coordinate_initial < Theta_Coordinate_final - 0.005; Theta_Coordinate_initial += Theta_increment)
                {
                    X_Coordinate = Radial_Coordinate_initial * TMath::Sin(Theta_Coordinate_initial * 3.14159 / 180.0);
                    Y_Coordinate = -1.0 * Radial_Coordinate_initial * TMath::Cos(Theta_Coordinate_initial * 3.14159 / 180.0);
                    PadXandY->Fill(-X_offset - X_Coordinate, Y_offset + Y_Coordinate, Weight);
                }
            }
        }
        else
        {
            Float_t Radial_Coordinate_initial = 18.30, Radial_Coordinate_final = 18.3, Theta_Coordinate_initial, Theta_Coordinate_final;
            Theta_Coordinate_initial = ((Float_t)22 - Column_number) + 90.0 - 23.0 / 2.0;
            Theta_Coordinate_final = (Float_t)(22 - (Column_number - 1)) + 90.0 - 23.0 / 2.0;
            Float_t Pi = 3.1468;
            Float_t X_offset = -19.4103 - 110.0 + 60.0, Y_offset = 75.1912 + 85.0;
            Float_t X_Coordinate, Y_Coordinate;

            Float_t Theta_increment = 0.0;

            if (Sector_number > 25 && Sector_number < 50)
                Theta_increment = 0.075;
            else if (Sector_number >= 50 && Sector_number < 80)
                Theta_increment = 0.05;
            else if (Sector_number >= 80)
                Theta_increment = 0.04;
            else
                Theta_increment = 0.15;

            for (Int_t i = 0; i < 97; i++)
            {
                Radial_Coordinate_final = Radial_Coordinate_initial + Radial_Coordinate_initial * Pi / 180;

                if (i == Sector_number)
                    break;

                Radial_Coordinate_initial = Radial_Coordinate_final;
            }

            Float_t Theta_Coordinate_initial_copy = Theta_Coordinate_initial;

            Radial_Coordinate_initial = Radial_Coordinate_initial + 0.01;

            for (; Radial_Coordinate_initial < Radial_Coordinate_final - 0.025; Radial_Coordinate_initial += 0.1)
            {
                for (Theta_Coordinate_initial = Theta_Coordinate_initial_copy + (Theta_increment / 2.0); Theta_Coordinate_initial < Theta_Coordinate_final - 0.005; Theta_Coordinate_initial += Theta_increment)
                {
                    X_Coordinate = Radial_Coordinate_initial * TMath::Sin(Theta_Coordinate_initial * 3.14159 / 180.0);
                    Y_Coordinate = -1.0 * Radial_Coordinate_initial * TMath::Cos(Theta_Coordinate_initial * 3.14159 / 180.0);
                    PadXandY->Fill(-X_offset - X_Coordinate, Y_offset + Y_Coordinate, Weight);
                }
            }
        }
    }
}

void Boundary_Plotter_GEM(Int_t Column_number, Int_t Sector_number, TH2F*& PadXandY, Bool_t left, Bool_t right, Bool_t top, Bool_t bottom, Bool_t Wall_Near_Smaller_Pads)
{
    if (Wall_Near_Smaller_Pads)
    {
        Float_t Radial_Coordinate = 18.30, Theta_Coordinate = 0.0;
        Float_t Theta_Coordinate_initial = ((Float_t)22 - Column_number) - 90.0 - 23.0 / 2.0;
        Float_t Theta_Coordinate_final = (Float_t)(22 - (Column_number - 1)) - 90.0 - 23.0 / 2.0;
        // Float_t Theta_Coordinate_initial=((Float_t) Column_number);
        // Float_t Theta_Coordinate_final=((Float_t) (Column_number+1));
        Float_t X_Coordinate, Y_Coordinate;
        Float_t X_offset = 19.4103 - 60.0, Y_offset = 75.1912 + 85.0;
        Float_t Pi = 3.1468;
        Int_t i = 0;

        Float_t Radial_Coordinate_initial = Radial_Coordinate;
        Float_t Radial_Coordinate_final = Radial_Coordinate;

        for (Int_t i = 0; i < 97; i++)
        {
            Radial_Coordinate_final = Radial_Coordinate_initial + Radial_Coordinate_initial * Pi / 180;

            if (i == Sector_number)
                break;

            Radial_Coordinate_initial = Radial_Coordinate_final;
        }

        if (top)
        {
            for (Theta_Coordinate = Theta_Coordinate_initial; Theta_Coordinate < Theta_Coordinate_final + 0.01; Theta_Coordinate += 0.002)
            {
                X_Coordinate = -1.0 * Radial_Coordinate_initial * TMath::Sin(Theta_Coordinate * 3.14159 / 180.0);
                Y_Coordinate = -1.0 * Radial_Coordinate_initial * TMath::Cos(Theta_Coordinate * 3.14159 / 180.0);
                PadXandY->Fill(X_offset + X_Coordinate, Y_offset + Y_Coordinate);
            }
        }

        if (bottom)
        {
            for (Theta_Coordinate = Theta_Coordinate_initial; Theta_Coordinate < Theta_Coordinate_final + 0.01; Theta_Coordinate += 0.002)
            {
                X_Coordinate = -1.0 * Radial_Coordinate_final * TMath::Sin(Theta_Coordinate * 3.14159 / 180.0);
                Y_Coordinate = -1.0 * Radial_Coordinate_final * TMath::Cos(Theta_Coordinate * 3.14159 / 180.0);
                PadXandY->Fill(X_offset + X_Coordinate, Y_offset + Y_Coordinate);
            }
        }

        if (right)
        {
            Theta_Coordinate = Theta_Coordinate_final;

            for (Radial_Coordinate = Radial_Coordinate_initial; Radial_Coordinate < Radial_Coordinate_final + 0.01; Radial_Coordinate += 0.002)
            {
                X_Coordinate = -1.0 * Radial_Coordinate * TMath::Sin(Theta_Coordinate * 3.14159 / 180.0);
                Y_Coordinate = -1.0 * Radial_Coordinate * TMath::Cos(Theta_Coordinate * 3.14159 / 180.0);
                PadXandY->Fill(X_offset + X_Coordinate, Y_offset + Y_Coordinate);
            }
        }

        if (left)
        {
            Theta_Coordinate = Theta_Coordinate_initial;

            for (Radial_Coordinate = Radial_Coordinate_initial; Radial_Coordinate < Radial_Coordinate_final + 0.01; Radial_Coordinate += 0.002)
            {
                X_Coordinate = -1.0 * Radial_Coordinate * TMath::Sin(Theta_Coordinate * 3.14159 / 180.0);
                Y_Coordinate = -1.0 * Radial_Coordinate * TMath::Cos(Theta_Coordinate * 3.14159 / 180.0);
                PadXandY->Fill(X_offset + X_Coordinate, Y_offset + Y_Coordinate);
            }
        }
    }
    else
    {
        Float_t Radial_Coordinate = 18.30, Theta_Coordinate = 0.0;
        Float_t Theta_Coordinate_initial = ((Float_t)22 - Column_number) + 90.0 - 23.0 / 2.0;
        Float_t Theta_Coordinate_final = (Float_t)(22 - (Column_number - 1)) + 90.0 - 23.0 / 2.0;
        // Float_t Theta_Coordinate_initial=((Float_t) Column_number);
        // Float_t Theta_Coordinate_final=((Float_t) (Column_number+1));
        Float_t X_Coordinate, Y_Coordinate;
        Float_t X_offset = 19.4103 + 110.0 - 60.0, Y_offset = 75.1912 + 85.0;
        Float_t Pi = 3.1468;
        Int_t i = 0;

        Float_t Radial_Coordinate_initial = Radial_Coordinate;
        Float_t Radial_Coordinate_final = Radial_Coordinate;

        for (Int_t i = 0; i < 97; i++)
        {
            Radial_Coordinate_final = Radial_Coordinate_initial + Radial_Coordinate_initial * Pi / 180;

            if (i == Sector_number)
                break;

            Radial_Coordinate_initial = Radial_Coordinate_final;
        }

        if (top)
        {
            for (Theta_Coordinate = Theta_Coordinate_initial; Theta_Coordinate < Theta_Coordinate_final + 0.01; Theta_Coordinate += 0.002)
            {
                X_Coordinate = -1.0 * Radial_Coordinate_initial * TMath::Sin(Theta_Coordinate * 3.14159 / 180.0);
                Y_Coordinate = -1.0 * Radial_Coordinate_initial * TMath::Cos(Theta_Coordinate * 3.14159 / 180.0);
                PadXandY->Fill(X_offset + X_Coordinate, Y_offset + Y_Coordinate);
            }
        }

        if (bottom)
        {
            for (Theta_Coordinate = Theta_Coordinate_initial; Theta_Coordinate < Theta_Coordinate_final + 0.01; Theta_Coordinate += 0.002)
            {
                X_Coordinate = -1.0 * Radial_Coordinate_final * TMath::Sin(Theta_Coordinate * 3.14159 / 180.0);
                Y_Coordinate = -1.0 * Radial_Coordinate_final * TMath::Cos(Theta_Coordinate * 3.14159 / 180.0);
                PadXandY->Fill(X_offset + X_Coordinate, Y_offset + Y_Coordinate);
            }
        }

        if (right)
        {
            Theta_Coordinate = Theta_Coordinate_final;

            for (Radial_Coordinate = Radial_Coordinate_initial; Radial_Coordinate < Radial_Coordinate_final + 0.01; Radial_Coordinate += 0.002)
            {
                X_Coordinate = -1.0 * Radial_Coordinate * TMath::Sin(Theta_Coordinate * 3.14159 / 180.0);
                Y_Coordinate = -1.0 * Radial_Coordinate * TMath::Cos(Theta_Coordinate * 3.14159 / 180.0);
                PadXandY->Fill(X_offset + X_Coordinate, Y_offset + Y_Coordinate);
            }
        }

        if (left)
        {
            Theta_Coordinate = Theta_Coordinate_initial;

            for (Radial_Coordinate = Radial_Coordinate_initial; Radial_Coordinate < Radial_Coordinate_final + 0.01; Radial_Coordinate += 0.002)
            {
                X_Coordinate = -1.0 * Radial_Coordinate * TMath::Sin(Theta_Coordinate * 3.14159 / 180.0);
                Y_Coordinate = -1.0 * Radial_Coordinate * TMath::Cos(Theta_Coordinate * 3.14159 / 180.0);
                PadXandY->Fill(X_offset + X_Coordinate, Y_offset + Y_Coordinate);
            }
        }
    }
}

void Draw_Feb(Double_t PosX, Double_t PosY, Int_t Feb_no, TH2F*& PadXandY)
{
    PosX -= 60.0;
    PosY += 3.001;
    PosY += 85.0;

    if (Feb_no > 17)
    {
        cout << "Invalid Feb no. " << endl;
        return;
    }
    // Print F

    for (Double_t X = 0; X < 1.2; X += 0.01)
    {
        PadXandY->Fill(PosX + X, PosY + 0);
    }

    for (Double_t X = 0; X < 1.2; X += 0.01)
    {
        PadXandY->Fill(PosX + X, PosY - 1.5);
    }

    for (Double_t Y = 0; Y > -3; Y -= 0.01)
    {
        PadXandY->Fill(PosX + 0.0, PosY + Y);
    }

    // Print E

    for (Double_t X = 0; X < 1.2; X += 0.01)
    {
        PadXandY->Fill(PosX + X + 2.0, PosY + 0);
    }

    for (Double_t X = 0; X < 1.2; X += 0.01)
    {
        PadXandY->Fill(PosX + X + 2.0, PosY - 1.5);
    }

    for (Double_t X = 0; X < 1.2; X += 0.01)
    {
        PadXandY->Fill(PosX + X + 2.0, PosY - 3.0);
    }

    for (Double_t Y = 0; Y > -3; Y -= 0.01)
    {
        PadXandY->Fill(PosX + 2.0, PosY + Y);
    }

    // Print b

    for (Double_t X = 0; X < 1.2; X += 0.01)
    {
        PadXandY->Fill(PosX + X + 4.0, PosY - 1.5);
    }

    for (Double_t X = 0; X < 1.2; X += 0.01)
    {
        PadXandY->Fill(PosX + X + 4.0, PosY - 3.0);
    }

    for (Double_t Y = -1.5; Y > -3; Y -= 0.01)
    {
        PadXandY->Fill(PosX + 4.0 + 1.2, PosY + Y);
    }

    for (Double_t Y = 0; Y > -3; Y -= 0.01)
    {
        PadXandY->Fill(PosX + 4.0, PosY + Y);
    }

    Int_t Seven_Seg_Display[7] = {0};

    if (Feb_no < 10)
    {
        switch (Feb_no)
        {
            case 0:
                Seven_Seg_Display[0] = 1;
                Seven_Seg_Display[1] = 1;
                Seven_Seg_Display[2] = 1;
                Seven_Seg_Display[3] = 1;
                Seven_Seg_Display[4] = 1;
                Seven_Seg_Display[5] = 1;
                Seven_Seg_Display[6] = 0;
                break;
            case 1:
                Seven_Seg_Display[0] = 0;
                Seven_Seg_Display[1] = 1;
                Seven_Seg_Display[2] = 1;
                Seven_Seg_Display[3] = 0;
                Seven_Seg_Display[4] = 0;
                Seven_Seg_Display[5] = 0;
                Seven_Seg_Display[6] = 0;
                break;
            case 2:
                Seven_Seg_Display[0] = 1;
                Seven_Seg_Display[1] = 1;
                Seven_Seg_Display[2] = 0;
                Seven_Seg_Display[3] = 1;
                Seven_Seg_Display[4] = 1;
                Seven_Seg_Display[5] = 0;
                Seven_Seg_Display[6] = 1;
                break;
            case 3:
                Seven_Seg_Display[0] = 1;
                Seven_Seg_Display[1] = 1;
                Seven_Seg_Display[2] = 1;
                Seven_Seg_Display[3] = 1;
                Seven_Seg_Display[4] = 0;
                Seven_Seg_Display[5] = 0;
                Seven_Seg_Display[6] = 1;
                break;
            case 4:
                Seven_Seg_Display[0] = 0;
                Seven_Seg_Display[1] = 1;
                Seven_Seg_Display[2] = 1;
                Seven_Seg_Display[3] = 0;
                Seven_Seg_Display[4] = 0;
                Seven_Seg_Display[5] = 1;
                Seven_Seg_Display[6] = 1;
                break;
            case 5:
                Seven_Seg_Display[0] = 1;
                Seven_Seg_Display[1] = 0;
                Seven_Seg_Display[2] = 1;
                Seven_Seg_Display[3] = 1;
                Seven_Seg_Display[4] = 0;
                Seven_Seg_Display[5] = 1;
                Seven_Seg_Display[6] = 1;
                break;
            case 6:
                Seven_Seg_Display[0] = 1;
                Seven_Seg_Display[1] = 0;
                Seven_Seg_Display[2] = 1;
                Seven_Seg_Display[3] = 1;
                Seven_Seg_Display[4] = 1;
                Seven_Seg_Display[5] = 1;
                Seven_Seg_Display[6] = 1;
                break;
            case 7:
                Seven_Seg_Display[0] = 1;
                Seven_Seg_Display[1] = 1;
                Seven_Seg_Display[2] = 1;
                Seven_Seg_Display[3] = 0;
                Seven_Seg_Display[4] = 0;
                Seven_Seg_Display[5] = 0;
                Seven_Seg_Display[6] = 0;
                break;
            case 8:
                Seven_Seg_Display[0] = 1;
                Seven_Seg_Display[1] = 1;
                Seven_Seg_Display[2] = 1;
                Seven_Seg_Display[3] = 1;
                Seven_Seg_Display[4] = 1;
                Seven_Seg_Display[5] = 1;
                Seven_Seg_Display[6] = 1;
                break;
            case 9:
                Seven_Seg_Display[0] = 1;
                Seven_Seg_Display[1] = 1;
                Seven_Seg_Display[2] = 1;
                Seven_Seg_Display[3] = 1;
                Seven_Seg_Display[4] = 0;
                Seven_Seg_Display[5] = 1;
                Seven_Seg_Display[6] = 1;
                break;
            default:
                return;
        }

        // Print 8
        // Code for 7 Segment display

        for (Double_t X = 0; X < 1.0 && Seven_Seg_Display[0] == 1; X += 0.01) //a
        {
            PadXandY->Fill(PosX + X + 6.5, PosY);
        }

        for (Double_t Y = 0; Y > -1.5 && Seven_Seg_Display[1] == 1; Y -= 0.01) //b
        {
            PadXandY->Fill(PosX + 6.5 + 1.0, PosY + Y);
        }

        for (Double_t Y = -1.5; Y > -3 && Seven_Seg_Display[2] == 1; Y -= 0.01) //c
        {
            PadXandY->Fill(PosX + 6.5 + 1.0, PosY + Y);
        }

        for (Double_t X = 0; X < 1.0 && Seven_Seg_Display[3] == 1; X += 0.01) //d
        {
            PadXandY->Fill(PosX + X + 6.5, PosY - 3.0);
        }

        for (Double_t Y = -1.5; Y > -3 && Seven_Seg_Display[4] == 1; Y -= 0.01) //e
        {
            PadXandY->Fill(PosX + 6.5, PosY + Y);
        }

        for (Double_t Y = 0; Y > -1.5 && Seven_Seg_Display[5] == 1; Y -= 0.01) //f
        {
            PadXandY->Fill(PosX + 6.5, PosY + Y);
        }

        for (Double_t X = 0; X < 1.0 && Seven_Seg_Display[6] == 1; X += 0.01) //g
        {
            PadXandY->Fill(PosX + X + 6.5, PosY - 1.5);
        }
    }
    else
    {
        Feb_no = Feb_no % 10;

        switch (Feb_no)
        {
            case 0:
                Seven_Seg_Display[0] = 1;
                Seven_Seg_Display[1] = 1;
                Seven_Seg_Display[2] = 1;
                Seven_Seg_Display[3] = 1;
                Seven_Seg_Display[4] = 1;
                Seven_Seg_Display[5] = 1;
                Seven_Seg_Display[6] = 0;
                break;
            case 1:
                Seven_Seg_Display[0] = 0;
                Seven_Seg_Display[1] = 1;
                Seven_Seg_Display[2] = 1;
                Seven_Seg_Display[3] = 0;
                Seven_Seg_Display[4] = 0;
                Seven_Seg_Display[5] = 0;
                Seven_Seg_Display[6] = 0;
                break;
            case 2:
                Seven_Seg_Display[0] = 1;
                Seven_Seg_Display[1] = 1;
                Seven_Seg_Display[2] = 0;
                Seven_Seg_Display[3] = 1;
                Seven_Seg_Display[4] = 1;
                Seven_Seg_Display[5] = 0;
                Seven_Seg_Display[6] = 1;
                break;
            case 3:
                Seven_Seg_Display[0] = 1;
                Seven_Seg_Display[1] = 1;
                Seven_Seg_Display[2] = 1;
                Seven_Seg_Display[3] = 1;
                Seven_Seg_Display[4] = 0;
                Seven_Seg_Display[5] = 0;
                Seven_Seg_Display[6] = 1;
                break;
            case 4:
                Seven_Seg_Display[0] = 0;
                Seven_Seg_Display[1] = 1;
                Seven_Seg_Display[2] = 1;
                Seven_Seg_Display[3] = 0;
                Seven_Seg_Display[4] = 0;
                Seven_Seg_Display[5] = 1;
                Seven_Seg_Display[6] = 1;
                break;
            case 5:
                Seven_Seg_Display[0] = 1;
                Seven_Seg_Display[1] = 0;
                Seven_Seg_Display[2] = 1;
                Seven_Seg_Display[3] = 1;
                Seven_Seg_Display[4] = 0;
                Seven_Seg_Display[5] = 1;
                Seven_Seg_Display[6] = 1;
                break;
            case 6:
                Seven_Seg_Display[0] = 1;
                Seven_Seg_Display[1] = 0;
                Seven_Seg_Display[2] = 1;
                Seven_Seg_Display[3] = 1;
                Seven_Seg_Display[4] = 1;
                Seven_Seg_Display[5] = 1;
                Seven_Seg_Display[6] = 1;
                break;
            case 7:
                Seven_Seg_Display[0] = 1;
                Seven_Seg_Display[1] = 1;
                Seven_Seg_Display[2] = 1;
                Seven_Seg_Display[3] = 0;
                Seven_Seg_Display[4] = 0;
                Seven_Seg_Display[5] = 0;
                Seven_Seg_Display[6] = 0;
                break;
            case 8:
                Seven_Seg_Display[0] = 1;
                Seven_Seg_Display[1] = 1;
                Seven_Seg_Display[2] = 1;
                Seven_Seg_Display[3] = 1;
                Seven_Seg_Display[4] = 1;
                Seven_Seg_Display[5] = 1;
                Seven_Seg_Display[6] = 1;
                break;
            case 9:
                Seven_Seg_Display[0] = 1;
                Seven_Seg_Display[1] = 1;
                Seven_Seg_Display[2] = 1;
                Seven_Seg_Display[3] = 1;
                Seven_Seg_Display[4] = 0;
                Seven_Seg_Display[5] = 1;
                Seven_Seg_Display[6] = 1;
                break;
            default:
                return;
        }

        for (Double_t Y = 0; Y > -1.5; Y -= 0.01) //b for 1
        {
            PadXandY->Fill(PosX + 6.5 + 1.0, PosY + Y);
        }

        for (Double_t Y = -1.5; Y > -3; Y -= 0.01) //c for 1
        {
            PadXandY->Fill(PosX + 6.5 + 1.0, PosY + Y);
        }

        for (Double_t X = 0; X < 1.0 && Seven_Seg_Display[0] == 1; X += 0.01) //a
        {
            PadXandY->Fill(PosX + X + 8.5, PosY);
        }

        for (Double_t Y = 0; Y > -1.5 && Seven_Seg_Display[1] == 1; Y -= 0.01) //b
        {
            PadXandY->Fill(PosX + 8.5 + 1.0, PosY + Y);
        }

        for (Double_t Y = -1.5; Y > -3 && Seven_Seg_Display[2] == 1; Y -= 0.01) //c
        {
            PadXandY->Fill(PosX + 8.5 + 1.0, PosY + Y);
        }

        for (Double_t X = 0; X < 1.0 && Seven_Seg_Display[3] == 1; X += 0.01) //d
        {
            PadXandY->Fill(PosX + X + 8.5, PosY - 3.0);
        }

        for (Double_t Y = -1.5; Y > -3 && Seven_Seg_Display[4] == 1; Y -= 0.01) //e
        {
            PadXandY->Fill(PosX + 8.5, PosY + Y);
        }

        for (Double_t Y = 0; Y > -1.5 && Seven_Seg_Display[5] == 1; Y -= 0.01) //f
        {
            PadXandY->Fill(PosX + 8.5, PosY + Y);
        }

        for (Double_t X = 0; X < 1.0 && Seven_Seg_Display[6] == 1; X += 0.01) //g
        {
            PadXandY->Fill(PosX + X + 8.5, PosY - 1.5);
        }
    }
}

void Draw_Arrow_Left_To_Right(Double_t X1, Double_t X2, Double_t Y, TH2F*& PadXandY)
{
    X1 -= 60.0;
    X2 -= 60.0;
    Y += 85.0;

    for (Double_t X = X1; X < X2; X += 0.01)
    {
        PadXandY->Fill(X, Y);
    }
}

void Draw_Arrow_Top_To_Bottom(Double_t X, Double_t Y1, Double_t Y2, TH2F*& PadXandY)
{
    X -= 60.0;
    Y1 += 85.0;
    Y2 += 85.0;

    for (Double_t Y = Y1; Y > Y2; Y -= 0.01)
    {
        PadXandY->Fill(X, Y);
    }
}

void Write_Wall(Bool_t Wall_Is_On_The_Left, Bool_t Wall_Is_On_The_Right, TH2F*& PadXandY)
{
    if (Wall_Is_On_The_Left == Wall_Is_On_The_Right)
    {
        cout << "Wall cannot be on both sides choosing left side for now" << endl;
        Wall_Is_On_The_Left = true;
        Wall_Is_On_The_Right = false;
    }

    Double_t PosX = 11.0 - 60.0;
    Double_t PosY = 100.0;

    if (Wall_Is_On_The_Right)
        PosX = 133.0 - 60.0;
    //Print W
    PosY = 108.0 + 85.0;

    for (Double_t Y = -1.5; Y > -3; Y -= 0.01) //c
    {
        PadXandY->Fill(PosX + 3.5 + 1.0, PosY + Y);
    }

    for (Double_t X = 0; X < 1.0; X += 0.01) //d
    {
        PadXandY->Fill(PosX + X + 3.5, PosY - 3.0);
    }

    for (Double_t X = 0; X < 1.0; X += 0.01) //d
    {
        PadXandY->Fill(PosX + X + 4.5, PosY - 3.0);
    }

    for (Double_t Y = -1.5; Y > -3; Y -= 0.01) //e
    {
        PadXandY->Fill(PosX + 3.5, PosY + Y);
    }

    for (Double_t Y = 0; Y > -1.5; Y -= 0.01) //f
    {
        PadXandY->Fill(PosX + 3.5, PosY + Y);
    }

    for (Double_t Y = -1.5; Y > -3; Y -= 0.01) //e
    {
        PadXandY->Fill(PosX + 5.5, PosY + Y);
    }

    for (Double_t Y = 0; Y > -1.5; Y -= 0.01) //f
    {
        PadXandY->Fill(PosX + 5.5, PosY + Y);
    }

    //Print A
    PosY = 103.0 + 85.0;

    for (Double_t X = 0; X < 1.0; X += 0.01) //a
    {
        PadXandY->Fill(PosX + X + 4.0, PosY);
    }

    for (Double_t Y = 0; Y > -1.5; Y -= 0.01) //b
    {
        PadXandY->Fill(PosX + 4.0 + 1.0, PosY + Y);
    }

    for (Double_t Y = -1.5; Y > -3; Y -= 0.01) //c
    {
        PadXandY->Fill(PosX + 4.0 + 1.0, PosY + Y);
    }

    for (Double_t Y = -1.5; Y > -3; Y -= 0.01) //e
    {
        PadXandY->Fill(PosX + 4.0, PosY + Y);
    }

    for (Double_t Y = 0; Y > -1.5; Y -= 0.01) //f
    {
        PadXandY->Fill(PosX + 4.0, PosY + Y);
    }

    for (Double_t X = 0; X < 1.0; X += 0.01) //g
    {
        PadXandY->Fill(PosX + X + 4.0, PosY - 1.5);
    }


    //Print L
    PosY = 98.0 + 85.0;

    for (Double_t X = 0; X < 1.0; X += 0.01) //d
    {
        PadXandY->Fill(PosX + X + 4.0, PosY - 3.0);
    }

    for (Double_t Y = -1.5; Y > -3; Y -= 0.01) //e
    {
        PadXandY->Fill(PosX + 4.0, PosY + Y);
    }

    for (Double_t Y = 0; Y > -1.5; Y -= 0.01) //f
    {
        PadXandY->Fill(PosX + 4.0, PosY + Y);
    }


    //Print L
    PosY = 93.0 + 85.0;

    for (Double_t X = 0; X < 1.0; X += 0.01) //d
    {
        PadXandY->Fill(PosX + X + 4.0, PosY - 3.0);
    }

    for (Double_t Y = -1.5; Y > -3; Y -= 0.01) //e
    {
        PadXandY->Fill(PosX + 4.0, PosY + Y);
    }

    for (Double_t Y = 0; Y > -1.5; Y -= 0.01) //f
    {
        PadXandY->Fill(PosX + 4.0, PosY + Y);
    }
}
