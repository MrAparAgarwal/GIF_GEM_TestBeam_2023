// Copyright (C) Experimental High Energy Physics and Applications Group, VECC, Kolkata
// Author(s): Apar Agarwal

// This macro has been created for the purpose
// analysing various kinds of spill structures
// of GEM with Scintillator trigger for GIF++ 2023 data

// This Code requires the CbmRoot version DEC 21 or higher
// Recommended on Linux 20.04 --> Fairsoftversion : APR21p2 and FAIRROOTVERSION: v18.6.7 or higher

// Code first tested on 22nd November, 2023

void GIF_GEM_With_Scintillator_Trigger_Efficiency_Acceptance_Corrected(Int_t RunId = 893,
                                                                       Double_t TCorr_Mean = 300.0, // Mean (ns)
                                                                       Double_t TCorr_Sigma = 15.0, // Sigma (ns)
                                                                       Int_t Number_Of_SD_Cut = 3.0)
{
    TStopwatch Code_Timer;
    Code_Timer.Start();

    // TString InputFileName = "data/Run"+ to_string(RunId) +".digi.root";
    TString InputFileName = "Run" + to_string(RunId) + ".digi.root";
    TString GEM_Noise_File_Name = "Run_" + to_string(RunId) + "_GEM_Noise.txt";
    // TString Three_Fold_Acceptance_File_Name = "Three_Fold_Area_Config_4.txt";
    TString Three_Fold_Acceptance_File_Name = "Three_Fold_Clean_Gaussian_Profile.txt";

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

    TString OutputFileName = "data/Efficiency/Run" + to_string(RunId) + "_GEM_With_Scintillator_Trigger_Efficiency_Acceptance_Corrected." + to_string(Number_Of_SD_Cut) + "_Sigma_SD_Cut.root";
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

    // for(iTreeEntry = 0; iTreeEntry < 100 ;iTreeEntry++)
    for (iTreeEntry = 0; iTreeEntry < nTimeSlices; iTreeEntry++)
    {
        Tree_Data_Reader.SetEntry(iTreeEntry);
        uint64_t TimeSlice_Start_Time = TimeSlice_Event_Header->GetTsStartTime();

        Int_t iTimeSlice = TimeSlice_Event_Header->GetTsIndex();

        if (iTreeEntry % 100 == 0)
            cout << "iTreeEntry: " << iTreeEntry << "  TS Index: " << iTimeSlice << endl;

        Int_t iMuchStartDigi = 0;

        std::vector<Int_t> Much2F_Signal_Plus_Background(Much_Digi.GetSize());
        std::vector<Int_t> Much2F_Background_Only(Much_Digi.GetSize());
        std::vector<Int_t> Much3F_Signal_Plus_Background(Much_Digi.GetSize());
        std::vector<Int_t> Much3F_Background_Only(Much_Digi.GetSize());

        for (Int_t iDigi = 0; iDigi < Much_Digi.GetSize(); iDigi++)
        {
            Much2F_Signal_Plus_Background.push_back(0);
            Much2F_Background_Only.push_back(0);
            Much3F_Signal_Plus_Background.push_back(0);
            Much3F_Background_Only.push_back(0);
        }

        uint64_t Last_2F_Trigger_Time = 0;
        uint64_t Last_3F_Trigger_Time = 0;

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
                // cout<<"Time 1: "<<Much_Digi[iMuchDigi].GetTime()<<"  Time 2: "<<Last_2F_Trigger_Time<<" Diff:  "<< Much_Digi[iMuchDigi].GetTime() - Last_2F_Trigger_Time  << " Limit: "<<(uint64_t)TMath::Ceil(2.0*Number_Of_SD_Cut*TCorr_Sigma) <<endl;

                // if(Much_Digi[iMuchDigi].GetTime() - Last_2F_Trigger_Time < (uint64_t)TMath::Ceil(2.0*Number_Of_SD_Cut*TCorr_Sigma)) cout<<"Diff is less than Limit"<<endl;
                // else cout<<"Diff is more than limit"<<endl;

                // if(Last_2F_Trigger_Time!=0)
                // {
                // if((uint64_t)((uint64_t)Much_Digi[iMuchDigi].GetTime() - Last_2F_Trigger_Time) < (uint64_t)TMath::Ceil(2.0*Number_Of_SD_Cut*TCorr_Sigma) )
                // {
                // cout<<"Time 1: "<<Much_Digi[iMuchDigi].GetTime()<<"  Time 2: "<<Last_2F_Trigger_Time<<" Diff:  "<< Much_Digi[iMuchDigi].GetTime() - Last_2F_Trigger_Time  << " Limit: "<<(uint64_t)TMath::Ceil(2.0*Number_Of_SD_Cut*TCorr_Sigma) <<endl;
                // continue;
                // }
                // }

                Two_Fold_Trigger_Digis++;
            }
            else
            {
                // if(Last_3F_Trigger_Time!=0)
                // {
                // if(Much_Digi[iMuchDigi].GetTime() - Last_3F_Trigger_Time < (uint64_t)TMath::Ceil(2.0*Number_Of_SD_Cut*TCorr_Sigma) )
                // continue;
                // }

                Three_Fold_Trigger_Digis++;
            }

            Int_t Trigger_Digi = 0;
            Bool_t Two_Fold_Trigger_Flag_Signal_Plus_Background = false;
            // Bool_t Two_Fold_Trigger_Flag_Background_Only = false;
            Bool_t Three_Fold_Trigger_Flag_Signal_Plus_Background = false;
            // Bool_t Three_Fold_Trigger_Flag_Background_Only = false;

            Int_t iLastDigi = iMuchStartDigi;
            Int_t Tolerance = (Int_t)TMath::Ceil(Much_Digi.GetSize() / (1280.0)); // 1 Ts = ~128ms --> 1/(1280) = ~100 #mus

            Int_t iMuchStartDigi = iMuchDigi - Tolerance;
            if (iMuchStartDigi < 0)
                iMuchStartDigi = 0;

            Int_t iMuchLastDigi = iMuchDigi + Tolerance;
            if (iMuchLastDigi > Much_Digi.GetSize() - 1)
                iMuchLastDigi = Much_Digi.GetSize() - 1;

            // Double_t Lower_Bound_Time = ((Double_t)Much_Digi[iMuchDigi].GetTime()-(Double_t)Much_Digi[iMuchStartDigi].GetTime() )/ 1000.0 ;
            // if(Lower_Bound_Time < 0.5) cout<<"Lower bound time too low: "<<Lower_Bound_Time<<endl;

            // Double_t Upper_Bound_Time = ((Double_t)Much_Digi[iMuchLastDigi].GetTime()-(Double_t)Much_Digi[iMuchDigi].GetTime() )/ 1000.0 ;
            // if(Upper_Bound_Time < 0.5) cout<<"Upper bound time too high: "<<Upper_Bound_Time<<endl;

            for (Int_t iMuchDigi1 = iMuchStartDigi; iMuchDigi1 < iMuchLastDigi; iMuchDigi1++)
            {
                if (iMuchDigi == iMuchDigi1)
                    continue;

                int32_t Station1 = CbmMuchAddress::GetStationIndex(Much_Digi[iMuchDigi1].GetAddress());
                if (Station1 != 0)
                    continue; // Selecting only GEM

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
                        int32_t Sector1 = CbmMuchAddress::GetSectorIndex(Much_Digi[iMuchDigi1].GetAddress());
                        int32_t Channel1 = CbmMuchAddress::GetChannelIndex(Much_Digi[iMuchDigi1].GetAddress());

                        if (Sector1 > 96 || Channel1 > 22)
                            continue;
                        // Int_t FEB_GEM = GEM_Feb_no_X_Y[Channel1][Sector1];
                        // Int_t FEB_CH = GEM_Ch_no_X_Y[Channel1][Sector1];
                        // Int_t ADC_Value = Much_Digi[iMuchDigi1].GetAdc();
                        // uint64_t Much_Digi_Time = TimeSlice_Start_Time + (uint64_t)Much_Digi[iMuchDigi1].GetTime() - First_Digi_Time;

                        // Double_t Much_Digi_Time_ms = Much_Digi_Time / 1000000.0 ;
                        // if(GEM_Noise[Channel1][Sector1]==-1) continue;
                        // cout<<iMuchDigi<<"  FEB: "<<FEB_GEM<<"  Channel: "<<FEB_CH<<"  ADC: "<<Much_Digi[iMuchDigi].GetAdc()<<" Timestamp: "<<(uint64_t)Much_Digi[iMuchDigi1].GetTime()<<endl;

                        if (Channel >= Two_Fold_Lower_Bound && Channel < Two_Fold_Upper_Bound)
                        {
                            if (Much2F_Signal_Plus_Background[iMuchDigi1] == 1)
                                continue; // Avoid Double Counting
                            Two_Fold_Trigger_Flag_Signal_Plus_Background = true;

                            Much2F_Signal_Plus_Background[iMuchDigi1] = 1;
                        }
                        else if (Channel >= Three_Fold_Lower_Bound && Channel < Three_Fold_Upper_Bound)
                        {
                            if (Much3F_Signal_Plus_Background[iMuchDigi1] == 1)
                                continue; // Avoid Double Counting

                            if (Three_Fold_Area[Channel1][Sector1] != 1)
                                continue;
                            Three_Fold_Trigger_Flag_Signal_Plus_Background = true;

                            Much3F_Signal_Plus_Background[iMuchDigi1] = 1;
                        }
                    }
                }
                else
                {
                    if (Time_difference < 0.0)
                        break;
                }
            }

            // for(Int_t iMuchDigi1=iMuchStartDigi;iMuchDigi1 < iMuchLastDigi;iMuchDigi1++)
            // {
            //     if(iMuchDigi == iMuchDigi1) continue;

            //     int32_t  Station1   = CbmMuchAddress::GetStationIndex(Much_Digi[iMuchDigi1].GetAddress());
            //     if(Station1!=0) continue;   // Selecting only GEM

            //     Double_t First_Time_difference = ((Double_t)Much_Digi[iMuchDigi1].GetTime()-(Double_t)Much_Digi[0].GetTime() )/ 1000.0 ;

            //     // cout<<"Sector: "<<Sector1<<"  Channel: "<<Channel1<<"  ADC: "<<Much_Digi[iMuchDigi].GetAdc()<<" Timestamp: "<<(uint64_t)Much_Digi[iMuchDigi1].GetTime()<<endl;

            //     Double_t Time_difference = ((Double_t)Much_Digi[iMuchDigi].GetTime()-(Double_t)Much_Digi[iMuchDigi1].GetTime() )/ 1000.0 ;

            //     if(First_Time_difference > 0.0)
            //     {
            //         // cout<<Time_difference<<endl;

            //         if(Time_difference > Coincidence_Window_Upper_Bound_mus + 1.0)
            //         {
            //             continue;
            //         }
            //         else if(Time_difference < Coincidence_Window_Lower_Bound_mus + 1.0)
            //         {
            //             break;
            //         }
            //         else
            //         {
            //             int32_t  Sector1   = CbmMuchAddress::GetSectorIndex(Much_Digi[iMuchDigi1].GetAddress());
            //             int32_t  Channel1   = CbmMuchAddress::GetChannelIndex(Much_Digi[iMuchDigi1].GetAddress());

            //             if(Sector1>96 || Channel1>22) continue;
            //             // Int_t FEB_GEM = GEM_Feb_no_X_Y[Channel1][Sector1];
            //             // Int_t FEB_CH = GEM_Ch_no_X_Y[Channel1][Sector1];
            //             // Int_t ADC_Value = Much_Digi[iMuchDigi1].GetAdc();
            //             // uint64_t Much_Digi_Time = TimeSlice_Start_Time + (uint64_t)Much_Digi[iMuchDigi1].GetTime() - First_Digi_Time;

            //             // Double_t Much_Digi_Time_ms = Much_Digi_Time / 1000000.0 ;
            //             // if(GEM_Noise[Channel1][Sector1]==-1) continue;
            //             // cout<<iMuchDigi<<"  FEB: "<<FEB_GEM<<"  Channel: "<<FEB_CH<<"  ADC: "<<Much_Digi[iMuchDigi].GetAdc()<<" Timestamp: "<<(uint64_t)Much_Digi[iMuchDigi1].GetTime()<<endl;

            //             if(Channel>=Two_Fold_Lower_Bound && Channel<Two_Fold_Upper_Bound)
            //             {
            //                 if(Much2F_Background_Only[iMuchDigi1]==1) continue;  // Avoid Double Counting

            //                 Two_Fold_Trigger_Flag_Background_Only = true;

            //                 Much2F_Background_Only[iMuchDigi1]=1;
            //             }
            //             else if(Channel>=Three_Fold_Lower_Bound && Channel<Three_Fold_Upper_Bound)
            //             {
            //                 if(Much3F_Background_Only[iMuchDigi1]==1) continue;   // Avoid Double Counting
            //                 if(Three_Fold_Area[Channel1][Sector1]!=1) continue;

            //                 Three_Fold_Trigger_Flag_Background_Only = true;

            //                 Much3F_Background_Only[iMuchDigi1]=1;
            //             }
            //         }
            //     }
            //     else
            //     {
            //         if(Time_difference < 0.0 ) break;
            //     }
            // }

            // TRandom1 Random_Hit;
            // if(Channel>=Two_Fold_Lower_Bound && Channel<Two_Fold_Upper_Bound)
            // {
            //     // if(!Two_Fold_Trigger_Flag_Signal_Plus_Background)
            //     // {
            //     Double_t Random_Hit_Probability = Random_Hit.Uniform(0.0,1.0); // Check if it falls in dead region
            //     // cout<<"Two_Fold_Trigger_Flag_Signal_Plus_Background: "<<Two_Fold_Trigger_Flag_Signal_Plus_Background<<" Random_Hit_Probability: "<<Random_Hit_Probability<<endl;
            //     if(Random_Hit_Probability < 0.33) Two_Fold_Trigger_Flag_Signal_Plus_Background = true;
            //     // }
            // }
            // else
            // {
            //     // if(!Three_Fold_Trigger_Flag_Signal_Plus_Background)
            //     // {
            //     Double_t Random_Hit_Probability = Random_Hit.Uniform(0.0,1.0); // Check if it falls in dead region
            //     // cout<<"Three_Fold_Trigger_Flag_Signal_Plus_Background: "<<Three_Fold_Trigger_Flag_Signal_Plus_Background<<" Random_Hit_Probability: "<<Random_Hit_Probability<<endl;
            //     if(Random_Hit_Probability < 0.33) Three_Fold_Trigger_Flag_Signal_Plus_Background = true;
            //     // }
            // }

            if (Two_Fold_Trigger_Flag_Signal_Plus_Background)
                Two_Fold_GEM_Digis_Signal_Plus_Background++;
            // if(Two_Fold_Trigger_Flag_Background_Only) Two_Fold_GEM_Digis_Background_Only++;
            if (Three_Fold_Trigger_Flag_Signal_Plus_Background)
                Three_Fold_GEM_Digis_Signal_Plus_Background++;
            // if(Three_Fold_Trigger_Flag_Background_Only) Three_Fold_GEM_Digis_Background_Only++;

            if (Channel >= Two_Fold_Lower_Bound && Channel < Two_Fold_Upper_Bound)
                Last_2F_Trigger_Time = Much_Digi[iMuchDigi].GetTime();
            else
                Last_3F_Trigger_Time = Much_Digi[iMuchDigi].GetTime();
        }

        Much2F_Signal_Plus_Background.clear();
        // Much2F_Background_Only.clear();
        Much3F_Signal_Plus_Background.clear();
        // Much3F_Background_Only.clear();
    }

    cout << "Efficiency for Two Fold = " << Two_Fold_GEM_Digis_Signal_Plus_Background << "/ " << Two_Fold_Trigger_Digis << " = " << 1.0 * (Two_Fold_GEM_Digis_Signal_Plus_Background) / (Two_Fold_Trigger_Digis) << endl;
    // cout<<"Efficiency for Two Fold Acceptance corrected= "<<Two_Fold_GEM_Digis_Signal_Plus_Background <<"/("<<Two_Fold_Trigger_Digis<<"* 0.8"<<") = "<<1.0*(Two_Fold_GEM_Digis_Signal_Plus_Background)/(Two_Fold_Trigger_Digis*0.8)<<endl;
    cout << "Efficiency for Three Fold = " << Three_Fold_GEM_Digis_Signal_Plus_Background << "/ 0.5 *" << Three_Fold_Trigger_Digis << " = " << 1.0 * (Three_Fold_GEM_Digis_Signal_Plus_Background) / (0.5 * Three_Fold_Trigger_Digis) << endl;
    // cout<<"Efficiency for Three Fold = "<<Three_Fold_GEM_Digis_Signal_Plus_Background <<"/ "<<Three_Fold_Trigger_Digis <<" = "<<1.0*(Three_Fold_GEM_Digis_Signal_Plus_Background)/(Three_Fold_Trigger_Digis)<<endl;
    // cout<<"Efficiency for Three Fold Acceptance corrected= "<<Three_Fold_GEM_Digis_Signal_Plus_Background <<"/("<<Three_Fold_Trigger_Digis<<"* 0.8"<<") = "<<1.0*(Three_Fold_GEM_Digis_Signal_Plus_Background)/(Three_Fold_Trigger_Digis*0.8)<<endl;

    fIn->Close();
    fOut->Close();

    cout << "Output File Name: " << OutputFileName << endl;

    Code_Timer.Stop();

    cout << "_______________________Code Executed Successfully!________________________" << endl;
    cout << "Real time: " << Code_Timer.RealTime() / 60.0 << " m, CPU time: " << Code_Timer.CpuTime() / 60.0 << " m" << endl;
}
