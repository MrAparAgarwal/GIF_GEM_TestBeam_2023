// Copyright (C) Experimental High Energy Physics and Applications Group, VECC, Kolkata
// Author(s): Apar Agarwal [Developer]

// This macro "GIF_GEM_Noise_Analysis.C" has been created for the purpose of
// analysing noise criteria and give a reason for the choice of threshold

// This Code requires the CbmRoot version DEC 21 or above
// Tested on Linux 20.04.05 --> Fairsoftversion : APR21p2 and FAIRROOTVERSION: v18.6.7 ---> Works Well!

// Last Updated: 27th Feb 2024
// For questions or issues, please open an issue on the GitHub repository.

void GIF_GEM_Noise_Analysis(Int_t RunId = 966)
{
    TStopwatch Code_Timer;
    Code_Timer.Start();

    TString InputFileName = "Run" + to_string(RunId) + ".digi.root";

    if (gSystem->AccessPathName(InputFileName))
    {
        cout << "The file: " << InputFileName << " does not exist!" << endl;
        return;
    }

    TFile* Input_Data_File = new TFile(InputFileName);
    cout << "Input Data File Name :" << InputFileName << endl;

    TTree* Input_Tree = (TTree*)Input_Data_File->Get("cbmsim");

    if (!Input_Tree)
    {
        cout << "Tree Not Found!" << endl;
        return;
    }

    Int_t nTimeSlices = Input_Tree->GetEntries();

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

    cout << "Number of Events in the Tree: " << Input_Tree->GetEntries() << endl;

    TTreeReader Tree_Data_Reader("cbmsim;", Input_Data_File); // Both TTree and TTreeReader are not required generally but kept in case some functionalities of TTree are needed

    TTreeReaderArray<CbmMuchDigi> Much_Digi(Tree_Data_Reader, "MuchDigi"); // MuchDigi -> Branch Name  and Much_Digi --> Element to Read Much data
    TTreeReaderValue<CbmTsEventHeader> TimeSlice_Event_Header(Tree_Data_Reader, "EventHeader.");

    Int_t iTreeEntry;
    Int_t FirstDigiFlag = 0;
    Int_t LastDigiFlag = 0;
    uint64_t First_Digi_Time;
    uint64_t Last_Digi_Time;
    TString HistTitle, HistTag;

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

    Int_t nOffspill_Timeslices = 0;
    Int_t GEM_X_and_Y[23][97] = {0};
    Double_t GEM_X_and_Y_Per_Ts[23][97];

    for (iTreeEntry = 0; iTreeEntry < nTimeSlices; iTreeEntry++)
    {
        Tree_Data_Reader.SetEntry(iTreeEntry);
        uint64_t TimeSlice_Start_Time = TimeSlice_Event_Header->GetTsStartTime();

        Int_t iTimeSlice = TimeSlice_Event_Header->GetTsIndex();

        // if(iTreeEntry%100 == 0)
        cout << "iTreeEntry: " << iTreeEntry << "  TS Index: " << iTimeSlice << endl;

        Int_t Offspill_flag = 0;
        Int_t Offspill_3_Fold_Trigger = 0;

        for (Int_t iMuchDigi = 0; iMuchDigi < Much_Digi.GetSize(); iMuchDigi++)
        {
            int32_t Channel = CbmMuchAddress::GetChannelIndex(Much_Digi[iMuchDigi].GetAddress());
            if (Channel != 125 && Channel != 5)
                continue; // Single Channel from both 2 Fold or 3 Fold Coincidence
            if (Channel >= Two_Fold_Lower_Bound && Channel < Two_Fold_Upper_Bound)
                continue; // 3-FOLD Coincidence only

            int32_t Station = CbmMuchAddress::GetStationIndex(Much_Digi[iMuchDigi].GetAddress());
            if (Station != 1)
                continue; // Selecting Only Trigger Hits

            Offspill_3_Fold_Trigger++;
        }

        if (Offspill_3_Fold_Trigger < 1) // More than or equal to 1 trigger per time slice is rejected
        {
            Offspill_flag = true;
            nOffspill_Timeslices++;
        }

        for (Int_t iMuchDigi = 0; iMuchDigi < Much_Digi.GetSize() && Offspill_flag; iMuchDigi++)
        {
            int32_t Station = CbmMuchAddress::GetStationIndex(Much_Digi[iMuchDigi].GetAddress());
            if (Station != 0)
                continue; // Selecting Only GEM Digis

            int32_t Sector = CbmMuchAddress::GetSectorIndex(Much_Digi[iMuchDigi].GetAddress());
            int32_t Channel = CbmMuchAddress::GetChannelIndex(Much_Digi[iMuchDigi].GetAddress());

            if (Sector > 96 || Channel > 22)
                continue;
            GEM_X_and_Y[Channel][Sector]++;
        }
    }

    TH2F* X_Y_Plot = new TH2F("X_Y_Plot", "Number of digis in offspill region", 100, -1.5, 98.5, 25, -1.5, 23.5);
    X_Y_Plot->GetXaxis()->SetTitle("Sector number");
    X_Y_Plot->GetYaxis()->SetTitle("Channel number");

    Double_t List_Of_Thresholds[30] = {0.001, 0.002, 0.003, 0.004, 0.005, 0.006, 0.007, 0.008, 0.009, 0.01, 0.0125, 0.015, 0.0175, 0.02, 0.03, 0.04, 0.05, 0.06, 0.07, 0.08, 0.09, 0.1, 0.2, 0.5, 1.0, 2.0, 5.0, 10.0, 100.0, 1000.0};
    // Total 27 thresholds

    for (Int_t iChannel = 0; iChannel < 23; iChannel++)
    {
        for (Int_t iSector = 0; iSector < 97; iSector++)
        {
            GEM_X_and_Y_Per_Ts[iChannel][iSector] = (GEM_X_and_Y[iChannel][iSector] / (nOffspill_Timeslices * 1.0));
            cout << iChannel << "  " << iSector << "  " << GEM_X_and_Y_Per_Ts[iChannel][iSector] << endl;
            X_Y_Plot->Fill(iSector, iChannel, GEM_X_and_Y_Per_Ts[iChannel][iSector]);
        }
    }

    Double_t List_of_Accepted_Channels[30] = {0};

    for (Int_t iThreshold = 0; iThreshold < 30; iThreshold++)
    {
        Int_t Number_of_accepted_channels = 0;

        for (Int_t iChannel = 0; iChannel < 23; iChannel++)
        {
            for (Int_t iSector = 0; iSector < 97; iSector++)
            {
                if (GEM_X_and_Y_Per_Ts[iChannel][iSector] < List_Of_Thresholds[iThreshold])
                {
                    Number_of_accepted_channels++;
                }
            }
        }

        List_of_Accepted_Channels[iThreshold] = Number_of_accepted_channels;
    }

    TGraph* Number_of_accepted_channels_vs_threshold = new TGraph(30, List_Of_Thresholds, List_of_Accepted_Channels);
    Number_of_accepted_channels_vs_threshold->GetXaxis()->SetTitle("Cut on average digis per channel per TS in offspill");
    Number_of_accepted_channels_vs_threshold->GetYaxis()->SetTitle("Number of accepted channels");

    Number_of_accepted_channels_vs_threshold->Draw("ap");
    // X_Y_Plot->Draw("colz");
    // Input_Data_File->Close();
}
