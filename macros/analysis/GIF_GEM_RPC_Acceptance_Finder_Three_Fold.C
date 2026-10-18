// Copyright (C) Experimental High Energy Physics and Applications Group, VECC, Kolkata
// Author(s): Apar Agarwal

// This macro has been created for the purpose
// of finding common acceptance between GEM and RPC
// worthy of calculating the efficiency

// This Code requires the CbmRoot version DEC 21 or higher
// Recommended on Linux 20.04 --> Fairsoftversion : APR21p2 and FAIRROOTVERSION: v18.6.7 or higher

// Code first tested on 15th December, 2023

void Highlight_RPC_Pad(Int_t Column_number, Int_t Sector_number, TH2F*& PadXandY);
void Highlight_Pad(Int_t Column_number, Int_t Sector_number, TH2F*& PadXandY);

void GIF_GEM_RPC_Acceptance_Finder_Three_Fold(Int_t RunId = 966,
                                              Double_t TCorr_Mean = 377.9,  // Mean (ns)
                                              Double_t TCorr_Sigma = 17.87, // Sigma (ns)
                                              Int_t Number_Of_SD_Cut = 3.0)
{
    TStopwatch Code_Timer;
    Code_Timer.Start();

    // TString InputFileName = "data/Run"+ to_string(RunId) +".digi.root";
    TString InputFileName = "Run" + to_string(RunId) + ".digi.root";
    TString GEM_Noise_File_Name = "Run_" + to_string(RunId) + "_GEM_Noise.txt";
    // TString Three_Fold_Acceptance_File_Name = "Three_Fold_Area_Config_4.txt";
    // TString Three_Fold_Acceptance_File_Name = "Three_Fold_Clean_Gaussian_Profile.txt";
    TString Three_Fold_Acceptance_File_Name = "Three_Fold_Reduced_Gaussian_Profile.txt";
    TString Three_Fold_RPC_Acceptance_File_Name = "Three_Fold_Clean_RPC_Profile.txt";

    Double_t Coincidence_Window_Lower_Bound_mus = 0.001 * (TCorr_Mean - Number_Of_SD_Cut * TCorr_Sigma);
    Double_t Coincidence_Window_Upper_Bound_mus = 0.001 * (TCorr_Mean + Number_Of_SD_Cut * TCorr_Sigma);

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

    if (gSystem->AccessPathName(Three_Fold_RPC_Acceptance_File_Name))
    {
        cout << "The file: " + Three_Fold_RPC_Acceptance_File_Name + " does not exist!" << endl;
        return;
    }

    TFile* fIn = new TFile(InputFileName);
    cout << "Input Run Number:" << RunId << "  File Name: " << InputFileName << endl;

    TTree* Input_Tree = (TTree*)fIn->Get("cbmsim");

    Int_t Three_Fold_Area[23][97] = {0};
    Int_t Three_Fold_Channel, Three_Fold_Sector;

    ifstream Three_Fold_Area_Infile;

    Three_Fold_Area_Infile.open(Three_Fold_Acceptance_File_Name);

    while (!Three_Fold_Area_Infile.eof())
    {
        Three_Fold_Area_Infile >> Three_Fold_Channel >> Three_Fold_Sector;
        Three_Fold_Area[Three_Fold_Channel][Three_Fold_Sector] = 1;
    }

    Three_Fold_Area_Infile.close();

    Int_t Three_Fold_RPC_Area[10][96] = {0};
    Int_t Three_Fold_RPC_Channel, Three_Fold_RPC_Sector;

    ifstream Three_Fold_RPC_Area_Infile;

    Three_Fold_RPC_Area_Infile.open(Three_Fold_RPC_Acceptance_File_Name);

    while (!Three_Fold_RPC_Area_Infile.eof())
    {
        Three_Fold_RPC_Area_Infile >> Three_Fold_RPC_Channel >> Three_Fold_RPC_Sector;
        Three_Fold_RPC_Area[Three_Fold_RPC_Channel][Three_Fold_RPC_Sector] = 1;
    }

    Three_Fold_RPC_Area_Infile.close();

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

    // TString OutputFileName = "data/Efficiency/Run"+ to_string(RunId) +"_GEM_With_Scintillator_GEM_Trigger_On_RPC_Reduced_Common_Acceptance."+to_string(Number_Of_SD_Cut)+"_Sigma_SD_Cut.root";
    TString OutputFileName = "data/Efficiency/Run" + to_string(RunId) + "_GEM_With_Scintillator_RPC_Trigger_On_GEM_Reduced_Common_Acceptance." + to_string(Number_Of_SD_Cut) + "_Sigma_SD_Cut.root";
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

    TH2F* RPC_Beam_Spot = new TH2F("RPC_Beam_Spot", "Common Beam Coverage for RPC with GEM in GIF++", 4800, 25.0, 145.0, 5200, -85.0, -15.0);
    RPC_Beam_Spot->GetXaxis()->SetTitle("X Coordinate (cm)");
    RPC_Beam_Spot->GetYaxis()->SetTitle("Y Coordinate (cm)");

    TH2F* RPC_Channel_Sector = new TH2F("RPC_Channel_Sector", "RPC Channel Sector Plot", 50, -1.5, 48.5, 13, -1.5, 11.5);
    RPC_Channel_Sector->GetXaxis()->SetTitle("Sector");
    RPC_Channel_Sector->GetYaxis()->SetTitle("Channel");

    TH2F* RPC_Channel_Sector_Position = new TH2F("RPC_Channel_Sector_Position", "RPC Channel Sector Plot Position", 500 * 2, -1.5, 48.5, 130 * 2, -1.5, 11.5);
    RPC_Channel_Sector_Position->GetXaxis()->SetTitle("Sector");
    RPC_Channel_Sector_Position->GetYaxis()->SetTitle("Channel");

    TH2F* GEM_Beam_Spot = new TH2F("GEM_Beam_Spot", "Common Beam Coverage for GEM with RPC in GIF++", 3280, -70, 11, 3640, 50, 140);
    GEM_Beam_Spot->GetXaxis()->SetTitle("X Coordinate (cm) -->");
    GEM_Beam_Spot->GetYaxis()->SetTitle("Y Coordinate (cm) -->");

    TH2F* GEM_Channel_Sector = new TH2F("GEM_Channel_Sector", "GEM Channel Sector Plot", 100, -1.5, 98.5, 25, -1.5, 23.5);
    GEM_Channel_Sector->GetXaxis()->SetTitle("Sector");
    GEM_Channel_Sector->GetYaxis()->SetTitle("Channel");

    Int_t iTreeEntry;
    Int_t FirstDigiFlag = 0;
    Int_t LastDigiFlag = 0;
    uint64_t First_Digi_Time;
    uint64_t Last_Digi_Time;
    TString HistTitle, HistTag;

    Int_t Two_Fold_Trigger_Digis = 0;
    Int_t Three_Fold_Trigger_Digis = 0;
    Int_t Two_Fold_GEM_Digis_Signal_Plus_Background = 0;
    Int_t Two_Fold_GEM_Digis_Background_Only = 0;
    Int_t Three_Fold_GEM_Digis_Signal_Plus_Background = 0;
    Int_t Three_Fold_GEM_Digis_Background_Only = 0;

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

    // Int_t Three_Fold_Trigger_Digis =0;
    Int_t Four_Fold_Trigger_Digis = 0;
    Int_t Four_Fold_GEM_Trigger_Digis = 0;

    // for(iTreeEntry = 0; iTreeEntry < 100 ;iTreeEntry++)
    for (iTreeEntry = 0; iTreeEntry < nTimeSlices; iTreeEntry++)
    {
        Tree_Data_Reader.SetEntry(iTreeEntry);
        uint64_t TimeSlice_Start_Time = TimeSlice_Event_Header->GetTsStartTime();

        Int_t iTimeSlice = TimeSlice_Event_Header->GetTsIndex();

        if (iTreeEntry % 100 == 0)
            cout << "iTreeEntry: " << iTreeEntry << "  TS Index: " << iTimeSlice << endl;

        Int_t iMuchStartDigi = 0;

        for (Int_t iMuchDigi = 0; iMuchDigi < Much_Digi.GetSize(); iMuchDigi++)
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


            if (Channel >= Two_Fold_Lower_Bound && Channel < Two_Fold_Upper_Bound)
            {
                continue;
            }
            else
            {
                Three_Fold_Trigger_Digis++;
            }

            Bool_t Three_Fold_GEM_Triggered = false;
            Bool_t Three_Fold_RPC_Triggered = false;
            Bool_t Four_Fold_GEM_Triggered = false;

            Int_t iLastDigi = iMuchStartDigi;
            Int_t Tolerance = (Int_t)TMath::Ceil(Much_Digi.GetSize() / (1280.0)); // 1 Ts = ~128ms --> 1/(1280) = ~100 #mus

            Int_t iMuchStartDigi = iMuchDigi - Tolerance;
            if (iMuchStartDigi < 0)
                iMuchStartDigi = 0;

            Int_t iMuchLastDigi = iMuchDigi + Tolerance;
            if (iMuchLastDigi > Much_Digi.GetSize() - 1)
                iMuchLastDigi = Much_Digi.GetSize() - 1;

            Double_t Weighted_RPC_Column = 0.0;
            Double_t Weighted_RPC_Sector = 0.0;
            Double_t Summed_RPC_ADC = 0.0;


            for (Int_t iMuchDigi1 = iMuchStartDigi; iMuchDigi1 < iMuchLastDigi; iMuchDigi1++)
            {
                if (iMuchDigi == iMuchDigi1)
                    continue;

                int32_t Station1 = CbmMuchAddress::GetStationIndex(Much_Digi[iMuchDigi1].GetAddress());
                if (Station1 != 2)
                    continue; // Selecting only RPC

                int32_t Sector1 = CbmMuchAddress::GetSectorIndex(Much_Digi[iMuchDigi1].GetAddress());
                int32_t Channel1 = CbmMuchAddress::GetChannelIndex(Much_Digi[iMuchDigi1].GetAddress());

                if (Sector1 > 95 || Channel1 > 9)
                    continue;
                // if(Much_Digi[iMuchDigi1].GetAdc()<15) continue;

                Double_t First_Time_difference = ((Double_t)Much_Digi[iMuchDigi1].GetTime() - (Double_t)Much_Digi[0].GetTime()) / 1000.0;

                // cout<<"Sector: "<<Sector1<<"  Channel: "<<Channel1<<"  ADC: "<<Much_Digi[iMuchDigi].GetAdc()<<" Timestamp: "<<(uint64_t)Much_Digi[iMuchDigi1].GetTime()<<endl;

                Double_t Time_difference = ((Double_t)Much_Digi[iMuchDigi].GetTime() - (Double_t)Much_Digi[iMuchDigi1].GetTime()) / 1000.0;

                if (First_Time_difference > 0.0)
                {
                    // cout<<Time_difference<<endl;

                    // if(Time_difference > Coincidence_Window_Upper_Bound_mus )
                    if (Time_difference > 0.475)
                    {
                        continue;
                    }
                    // else if(Time_difference < Coincidence_Window_Lower_Bound_mus)
                    else if (Time_difference < 0.440)
                    {
                        break;
                    }
                    else
                    {
                        if (Three_Fold_RPC_Area[Channel1][Sector1] != 1)
                            continue;
                        // if(!Three_Fold_GEM_Triggered) break;
                        // Three_Fold_RPC_Triggered = true;

                        Weighted_RPC_Column += Channel1 * Much_Digi[iMuchDigi1].GetAdc();
                        Weighted_RPC_Sector += Sector1 * Much_Digi[iMuchDigi1].GetAdc();
                        Summed_RPC_ADC += Much_Digi[iMuchDigi1].GetAdc();

                        // cout<<Channel1<<"   "<<Sector1<<"  "<<Much_Digi[iMuchDigi1].GetAdc()<<"   "<<(int32_t)Much_Digi[iMuchDigi].GetTime()<<endl;
                        // Highlight_RPC_Pad(Channel1,Sector1,RPC_Beam_Spot);
                        //RPC_Channel_Sector->Fill(Sector1,Channel1);
                    }
                }
                else
                {
                    if (Time_difference < 0.0)
                        break;
                }
            }

            Int_t RPC_Hit_Column = 0, RPC_Hit_Sector = 0; // Taken 0 0 assuming this can not be hit -- Not ideal solution but makes my life simpler
            // if(Three_Fold_RPC_Triggered) cout<<endl;
            if (Summed_RPC_ADC > 1)
                RPC_Hit_Column = TMath::Ceil(Weighted_RPC_Column / Summed_RPC_ADC);
            if (Summed_RPC_ADC > 1)
                RPC_Hit_Sector = TMath::Ceil(Weighted_RPC_Sector / Summed_RPC_ADC);
            // if(RPC_Hit_Column!=0 && RPC_Hit_Sector!=0) cout<<RPC_Hit_Column<<"  "<<RPC_Hit_Sector<<endl;

            // if(RPC_Hit_Column!=0 && RPC_Hit_Sector!=0)
            if ((Weighted_RPC_Column / Summed_RPC_ADC) > 3.75 && (Weighted_RPC_Column / Summed_RPC_ADC) < 6.0 && (Weighted_RPC_Sector / Summed_RPC_ADC) > 26.50 && (Weighted_RPC_Sector / Summed_RPC_ADC) < 29.0)
            // if(Three_Fold_RPC_Area[RPC_Hit_Column][RPC_Hit_Sector]==1)
            {
                Highlight_RPC_Pad(RPC_Hit_Column, RPC_Hit_Sector, RPC_Beam_Spot);
                RPC_Channel_Sector->Fill(RPC_Hit_Sector, RPC_Hit_Column);
                RPC_Channel_Sector_Position->Fill((Weighted_RPC_Sector / Summed_RPC_ADC), (Weighted_RPC_Column / Summed_RPC_ADC));
                Three_Fold_RPC_Triggered = true;
            }

            for (Int_t iMuchDigi1 = iMuchStartDigi; iMuchDigi1 < iMuchLastDigi; iMuchDigi1++)
            {
                if (iMuchDigi == iMuchDigi1)
                    continue;

                int32_t Station1 = CbmMuchAddress::GetStationIndex(Much_Digi[iMuchDigi1].GetAddress());
                if (Station1 != 0)
                    continue; // Selecting only GEM

                int32_t Sector1 = CbmMuchAddress::GetSectorIndex(Much_Digi[iMuchDigi1].GetAddress());
                int32_t Channel1 = CbmMuchAddress::GetChannelIndex(Much_Digi[iMuchDigi1].GetAddress());

                if (Sector1 > 96 || Channel1 > 22)
                    continue;

                Double_t First_Time_difference = ((Double_t)Much_Digi[iMuchDigi1].GetTime() - (Double_t)Much_Digi[0].GetTime()) / 1000.0;

                // cout<<"Sector: "<<Sector1<<"  Channel: "<<Channel1<<"  ADC: "<<Much_Digi[iMuchDigi].GetAdc()<<" Timestamp: "<<(uint64_t)Much_Digi[iMuchDigi1].GetTime()<<endl;

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
                        if (Three_Fold_Area[Channel1][Sector1] != 1)
                            continue;
                        if (!Three_Fold_RPC_Triggered)
                            break;

                        Three_Fold_GEM_Triggered = true;

                        Highlight_Pad(Channel1, Sector1, GEM_Beam_Spot);
                        GEM_Channel_Sector->Fill(Sector1, Channel1);
                    }
                }
                else
                {
                    if (Time_difference < 0.0)
                        break;
                }
            }

            Three_Fold_Trigger_Digis++;
            if (Three_Fold_RPC_Triggered)
                Four_Fold_Trigger_Digis++;
            if (Three_Fold_GEM_Triggered)
                Four_Fold_GEM_Trigger_Digis++;
        }
    }

    cout << "Three Fold Trigger Digis: " << Three_Fold_Trigger_Digis << endl;
    cout << "For Fold Efficiency (with RPC included): " << Four_Fold_GEM_Trigger_Digis << " / " << Four_Fold_Trigger_Digis << " = " << Four_Fold_GEM_Trigger_Digis / (1.0 * Four_Fold_Trigger_Digis) << endl;

    // cout<<"Efficiency for Two Fold = "<<Two_Fold_GEM_Digis_Signal_Plus_Background <<"/ "<<Two_Fold_Trigger_Digis <<" = "<<1.0*(Two_Fold_GEM_Digis_Signal_Plus_Background)/(Two_Fold_Trigger_Digis)<<endl;
    // cout<<"Efficiency for Two Fold Acceptance corrected= "<<Two_Fold_GEM_Digis_Signal_Plus_Background <<"/("<<Two_Fold_Trigger_Digis<<"* 0.8"<<") = "<<1.0*(Two_Fold_GEM_Digis_Signal_Plus_Background)/(Two_Fold_Trigger_Digis*0.8)<<endl;
    // cout<<"Efficiency for Three Fold = "<<Three_Fold_GEM_Digis_Signal_Plus_Background <<"/ 0.5 *"<<Three_Fold_Trigger_Digis <<" = "<<1.0*(Three_Fold_GEM_Digis_Signal_Plus_Background)/(0.5*Three_Fold_Trigger_Digis)<<endl;
    // cout<<"Efficiency for Three Fold = "<<Three_Fold_GEM_Digis_Signal_Plus_Background <<"/ "<<Three_Fold_Trigger_Digis <<" = "<<1.0*(Three_Fold_GEM_Digis_Signal_Plus_Background)/(Three_Fold_Trigger_Digis)<<endl;
    // cout<<"Efficiency for Three Fold Acceptance corrected= "<<Three_Fold_GEM_Digis_Signal_Plus_Background <<"/("<<Three_Fold_Trigger_Digis<<"* 0.8"<<") = "<<1.0*(Three_Fold_GEM_Digis_Signal_Plus_Background)/(Three_Fold_Trigger_Digis*0.8)<<endl;

    GEM_Beam_Spot->Write();
    GEM_Channel_Sector->Write();
    RPC_Beam_Spot->Write();
    RPC_Channel_Sector->Write();
    RPC_Channel_Sector_Position->Write();

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

void Highlight_Pad(Int_t Column_number, Int_t Sector_number, TH2F*& PadXandY)
{
    if (Sector_number >= 0 && Column_number >= 0)
    {
        Float_t Radial_Coordinate_initial = 18.30, Radial_Coordinate_final = 18.3, Theta_Coordinate_initial, Theta_Coordinate_final;
        Theta_Coordinate_initial = ((Float_t)Column_number) + 90.0 + 23.0 / 2.0;
        Theta_Coordinate_final = ((Float_t)(Column_number + 1)) + 90.0 + 23.0 / 2.0;
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
                PadXandY->Fill(-X_offset - X_Coordinate, Y_offset + Y_Coordinate);
                // cout<<-X_offset-X_Coordinate<<"   "<<Y_offset+Y_Coordinate<<endl;
            }
        }
    }
}
