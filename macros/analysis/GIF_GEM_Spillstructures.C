// Copyright (C) Experimental High Energy Physics and Applications Group, VECC, Kolkata
// Author(s): Apar Agarwal

// This macro has been created for the purpose
// analysing various kinds of spill structures
// of GEM for GIF++ 2023 data

// This Code requires the CbmRoot version DEC 21 or higher
// Recommended on Linux 20.04 --> Fairsoftversion : APR21p2 and FAIRROOTVERSION: v18.6.7 or higher

// Code first tested on 4th October, 2023

void GIF_GEM_Spillstructures(Int_t RunId = 966)
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

    TString OutputFileName = "data/Run" + to_string(RunId) + "_GEM_Spillstructures.root";
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
    uint64_t First_Digi_Time;
    uint64_t Last_Digi_Time;
    TString HistTitle, HistTag;

    // TH2F* RPC_Beam_Spot = new TH2F("RPC_Beam_Spot","Beam Coverage in RPC in GIF++",4800,-60.0,60.0,5200,-30.0,100.0);

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
        GEM_No_Trigger_Raw_Timeslice_Based_2D_vs_ADC_Feb[iFeb] = new TH2F(HistTag, HistTitle, 35, -1.5, 33.5, nTimeSlices + 10, -1.5, nTimeSlices + 8.5);
        GEM_No_Trigger_Raw_Timeslice_Based_2D_vs_ADC_Feb[iFeb]->GetXaxis()->SetTitle("ADC");
        GEM_No_Trigger_Raw_Timeslice_Based_2D_vs_ADC_Feb[iFeb]->GetYaxis()->SetTitle("Timeslice number");
    }

    TH2F* GEM_No_Trigger_Raw_Timeslice_Based_2D_vs_Channel_Feb[18];

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTag = "GEM_No_Trigger_Raw_Timeslice_Based_2D_vs_Channel_Feb_" + to_string(iFeb);
        HistTitle = "GEM Feb " + to_string(iFeb) + " 2D Raw Spillstructure vs. Channel without trigger [Time slice Based]";
        GEM_No_Trigger_Raw_Timeslice_Based_2D_vs_Channel_Feb[iFeb] = new TH2F(HistTag, HistTitle, 131, -1.5, 129.5, nTimeSlices + 10, -1.5, nTimeSlices + 8.5);
        GEM_No_Trigger_Raw_Timeslice_Based_2D_vs_Channel_Feb[iFeb]->GetXaxis()->SetTitle("Channel");
        GEM_No_Trigger_Raw_Timeslice_Based_2D_vs_Channel_Feb[iFeb]->GetYaxis()->SetTitle("Timeslice number");
    }

    HistTag = "GEM_No_Trigger_Raw_Timeslice_Based_2D_vs_Feb_Number";
    HistTitle = "GEM 2D Raw Spillstructure vs. Feb Number without trigger [Time slice based] (All Febs inclusive)";
    TH1I* GEM_No_Trigger_Raw_Timeslice_Based_2D_vs_Feb_Number = new TH1I(HistTag, HistTitle, 21, -1.5, 19.5, nTimeSlices + 10, -1.5, nTimeSlices + 8.5);
    GEM_No_Trigger_Raw_Timeslice_Based_2D_vs_Feb_Number->GetXaxis()->SetTitle("Feb number");
    GEM_No_Trigger_Raw_Timeslice_Based_2D_vs_Feb_Number->GetYaxis()->SetTitle("Timeslice number");


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

    // for(iTreeEntry = nTimeSlices-1; iTreeEntry < nTimeSlices ;iTreeEntry++)
    // {
    //     Tree_Data_Reader.SetEntry(iTreeEntry);
    //     uint64_t TimeSlice_Last_Time = TimeSlice_Event_Header->GetTsStartTime();

    //     if(LastDigiFlag==0)
    //     {
    //         Last_Digi_Time = TimeSlice_Last_Time;
    //         LastDigiFlag = 1;
    //     }
    // }
    Long_t GEM_Hits_Raw[23][97] = {0};
    Long_t GEM_Hits_Triggered_2D[23][97] = {0};
    Long_t GEM_Hits_Triggered_3D[23][97] = {0};

    // cout<<" Last_Digi_Time - First_Digi_Time: "<<1.0*(Last_Digi_Time-First_Digi_Time)/(nTimeSlices*1.0)<<endl;
    // Int_t FirstDigiFlag = 0;
    // Int_t LastDigiFlag = 0;
    // uint64_t First_Digi_Time;
    // uint64_t Last_Digi_Time;

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

    // for(iTreeEntry = 0; iTreeEntry < 200 ;iTreeEntry++)
    // for(iTreeEntry = 2314; iTreeEntry < 2315 ;iTreeEntry++)
    for (iTreeEntry = 0; iTreeEntry < nTimeSlices; iTreeEntry++)
    {
        // if(iTreeEntry<75) continue;
        // if(iTreeEntry>105 && iTreeEntry<245) continue;
        // if(iTreeEntry>275 && iTreeEntry<555) continue;
        // if(iTreeEntry>585 && iTreeEntry<725) continue;
        // if(iTreeEntry>755 && iTreeEntry<1030) continue;
        // if(iTreeEntry>1060 && iTreeEntry<1200) continue;
        // if(iTreeEntry>1235) continue;

        Tree_Data_Reader.SetEntry(iTreeEntry);
        uint64_t TimeSlice_Start_Time = TimeSlice_Event_Header->GetTsStartTime();

        Int_t iTimeSlice = TimeSlice_Event_Header->GetTsIndex();

        // if(iTreeEntry%100 == 0)
        cout << "iTreeEntry: " << iTreeEntry << "  TS Index: " << iTimeSlice << endl;
        // cout<<Much_Digi_Pulser.GetSize()<<endl;
        // if(Much_Digi_Pulser.GetSize()==0) continue;

        Int_t GEM_Digis = 0;
        Int_t iMuchStartDigi = 0;

        // SpillStructureTriggerTS->SetBinContent(iTimeSlice,Much_Digi_Pulser.GetSize());
        Bool_t Trigger_Flag = false;

        for (Int_t iMuchDigi = 0; iMuchDigi < Much_Digi.GetSize(); iMuchDigi++)
        // for(Int_t iMuchDigi=0;iMuchDigi < -1;iMuchDigi++) // Skip Filling the pads to save time
        {
            int32_t Station = CbmMuchAddress::GetStationIndex(Much_Digi[iMuchDigi].GetAddress());
            if (Station != 0)
                continue;
            // if(Much_Digi[iMuchDigi].GetAdc()<31) continue;
            // int32_t  Layer     = CbmMuchAddress::GetLayerIndex(Much_Digi[iMuchDigi].GetAddress());
            int32_t Sector = CbmMuchAddress::GetSectorIndex(Much_Digi[iMuchDigi].GetAddress());
            int32_t Channel = CbmMuchAddress::GetChannelIndex(Much_Digi[iMuchDigi].GetAddress());
            if (Sector > 96 || Channel > 22)
                continue;
            if (GEM_Noise[Channel][Sector] == -1)
                continue;
            uint64_t Much_Digi_Time = TimeSlice_Start_Time + (uint64_t)Much_Digi[iMuchDigi].GetTime() - First_Digi_Time;

            Double_t Trigger_Digi_Time_ms = Much_Digi_Time / 1000000.0;
            // Double_t Trigger_Digi_Time_us = Much_Digi_Time / 1000.0 ;
            // Double_t Trigger_Digi_Time_ns = Much_Digi_Time  ;
            // Int_t GEM_FEB = GEM_Feb_no_X_Y[Channel][Sector];
            // GEM_Hits_Raw[Channel][Sector]++;
            SpillStructureGEM->Fill(Trigger_Digi_Time_ms);
            SpillStructureGEMTS->Fill(iTimeSlice);
            // Highlight_Pad(Channel,Sector,PadXandY_GEM);
            // cout<<"Station: "<<Station<<" Layer: "<<Layer<<" Sector: "<<Sector<<" Channel: "<<Channel<<endl;
        }
    }

    // for(Int_t iChannel=0;iChannel<23;iChannel++)
    // {
    //     for(Int_t iSector=0;iSector<97;iSector++)
    //     {
    //         Long_t Weight = GEM_Hits_Raw[iChannel][iSector];
    //         Highlight_Pad(iChannel,iSector,Weight,PadXandY_GEM);
    //         Weight = GEM_Hits_Triggered_2D[iChannel][iSector];
    //         Highlight_Pad(iChannel,iSector,Weight,PadXandY_GEM_Triggered);
    //     }
    // }

    // Time_Correlation_Feb_Trigger->Draw();
    // Time_Correlation_GEM_Trigger->Draw();
    // SpillStructureTrigger->Draw();
    // PadXandY_GEM->Draw("colz");
    // SpillStructureTriggerTS->Draw("HIST");
    SpillStructureGEMTS->Write();
    SpillStructureGEM->Write();
    // PadXandY_GEM->Write();
    // PadXandY_GEM_Triggered->Write();
    // GEM_Feb_Boundaries_and_Labels->Write();
    fIn->Close();
    fOut->Close();
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
            Theta_increment = 0.03;
        else
            Theta_increment = 0.025;

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

        for (; Radial_Coordinate_initial < Radial_Coordinate_final - 0.02; Radial_Coordinate_initial += 0.05)
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

void Highlight_Pad(Int_t Column_number, Int_t Sector_number, Long_t Weight, TH2F*& PadXandY)
{
    if (Sector_number >= 0 && Column_number >= 0)
    {
        Float_t Radial_Coordinate_initial = 18.30, Radial_Coordinate_final = 18.3, Theta_Coordinate_initial, Theta_Coordinate_final;
        Theta_Coordinate_initial = ((Float_t)22 - Column_number) - 90.0 - 23.0 / 2.0;
        Theta_Coordinate_final = (Float_t)(22 - (Column_number - 1)) - 90.0 - 23.0 / 2.0;
        Float_t Pi = 3.1468;
        Float_t X_offset = -19.4103, Y_offset = 75.1912;
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

        //   if(Sector_number>25 && Sector_number<50)
        //    Theta_increment=0.03;
        //   else if(Sector_number>=50 && Sector_number<80)
        //    Theta_increment=0.03;
        //   else if(Sector_number>=80)
        //    Theta_increment=0.03;
        //   else
        //    Theta_increment=0.03;

        for (Int_t i = 0; i < 97; i++)
        {
            Radial_Coordinate_final = Radial_Coordinate_initial + Radial_Coordinate_initial * Pi / 180;

            if (i == Sector_number)
                break;

            Radial_Coordinate_initial = Radial_Coordinate_final;
        }

        Float_t Theta_Coordinate_initial_copy = Theta_Coordinate_initial;

        Radial_Coordinate_initial = Radial_Coordinate_initial + 0.01;

        for (; Radial_Coordinate_initial < Radial_Coordinate_final - 0.02; Radial_Coordinate_initial += 0.1)
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

void Boundary_Plotter_GEM(Int_t Column_number, Int_t Sector_number, TH2F*& PadXandY, Bool_t left, Bool_t right, Bool_t top, Bool_t bottom)
{
    Float_t Radial_Coordinate = 18.30, Theta_Coordinate = 0.0;
    Float_t Theta_Coordinate_initial = ((Float_t)22 - Column_number) - 90.0 - 23.0 / 2.0;
    Float_t Theta_Coordinate_final = (Float_t)(22 - (Column_number - 1)) - 90.0 - 23.0 / 2.0;
    // Float_t Theta_Coordinate_initial=((Float_t) Column_number);
    // Float_t Theta_Coordinate_final=((Float_t) (Column_number+1));
    Float_t X_Coordinate, Y_Coordinate;
    Float_t X_offset = 19.4103, Y_offset = 75.1912;
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

void Draw_Feb(Double_t PosX, Double_t PosY, Int_t Feb_no, TH2F*& PadXandY)
{
    PosY += 3.001;
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
    for (Double_t X = X1; X < X2; X += 0.01)
    {
        PadXandY->Fill(X, Y);
    }
}

void Draw_Arrow_Top_To_Bottom(Double_t X, Double_t Y1, Double_t Y2, TH2F*& PadXandY)
{
    for (Double_t Y = Y1; Y > Y2; Y -= 0.01)
    {
        PadXandY->Fill(X, Y);
    }
}
