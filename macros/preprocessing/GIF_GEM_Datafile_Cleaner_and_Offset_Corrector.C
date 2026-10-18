// Copyright (C) Experimental High Energy Physics and Applications Group, VECC, Kolkata
// Author(s): Apar Agarwal [Developer]

// This macro "GIF_GEM_Datafile_Cleaner_and_Offset_Corrector.C" has been created for the purpose of
// correcting the time offset between trigger and GEM, removing noisy/buggy pads
// remove potential ringing,remove RPC digis and correct for walk effects

// This Code requires the CbmRoot version DEC 21 or above
// Tested on Linux 20.04.05 --> Fairsoftversion : APR21p2 and FAIRROOTVERSION: v18.6.7 ---> Works Well!

// Last Updated: 27th November 2023
// For questions or issues, please open an issue on the GitHub repository.

void GIF_GEM_Datafile_Cleaner_and_Offset_Corrector(Int_t RunId = 966,
                                                   uint64_t Time_Offset = 362)
{
    TStopwatch Code_Timer;
    Code_Timer.Start();

    TString InputFileName = "Run" + to_string(RunId) + ".digi.root";
    TString GEM_Noise_File_Name = "Run_" + to_string(RunId) + "_GEM_Noise.txt";
    TString Output_Data_File_Name = "Run" + to_string(RunId) + "_Noise_Removed_Ringing_Walk_Corrected.digi.root";

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

    TFile* Input_Data_File = new TFile(InputFileName);
    cout << "Input Data File Name :" << InputFileName << endl;

    TTree* Input_Data_File_Tree = (TTree*)Input_Data_File->Get("cbmsim;"); // Unnecessary but kept in case other functionalities are needed
    // Data_File_Tree->Print();

    if (!Input_Data_File_Tree)
    {
        cout << "Tree Not Found!" << endl;
        return;
    }

    Int_t nTimeSlices = Input_Data_File_Tree->GetEntries();

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


    TFile* Output_Data_File = new TFile(Output_Data_File_Name, "recreate");
    Output_Data_File->WriteTObject((TObject*)Input_Data_File->Get("FileHeader"), "FileHeader");
    Output_Data_File->WriteTObject((TObject*)Input_Data_File->Get("cbmout"), "cbmout");
    Output_Data_File->WriteTObject((TObject*)Input_Data_File->Get("BranchList"), "BranchList");
    Output_Data_File->WriteTObject((TObject*)Input_Data_File->Get("TimeBasedBranchList"), "TimeBasedBranchList");

    TTree* New_Data_File_Tree = Input_Data_File_Tree->CloneTree(0);

    CbmTimeSlice* Input_Timeslice = new CbmTimeSlice();
    CbmTsEventHeader* Input_Event_Header = new CbmTsEventHeader();
    std::vector<CbmMuchDigi>* Input_Much_Digi = new std::vector<CbmMuchDigi>;
    std::vector<CbmMuchDigi>* Input_Much_Pulser_Digi = new std::vector<CbmMuchDigi>;

    Input_Data_File_Tree->SetBranchAddress("TimeSlice.", &Input_Timeslice);
    Input_Data_File_Tree->SetBranchAddress("EventHeader.", &Input_Event_Header);
    Input_Data_File_Tree->SetBranchAddress("MuchDigi", &Input_Much_Digi);
    Input_Data_File_Tree->SetBranchAddress("MuchDigiPulser", &Input_Much_Pulser_Digi);

    CbmTimeSlice* Output_Timeslice = new CbmTimeSlice();
    CbmTsEventHeader* Output_Event_Header = new CbmTsEventHeader();
    std::vector<CbmMuchDigi>* Output_Much_Digi = new std::vector<CbmMuchDigi>;
    std::vector<CbmMuchDigi>* Output_Much_Pulser_Digi = new std::vector<CbmMuchDigi>;

    New_Data_File_Tree->SetBranchAddress("TimeSlice.", &Output_Timeslice);
    New_Data_File_Tree->SetBranchAddress("EventHeader.", &Output_Event_Header);
    New_Data_File_Tree->SetBranchAddress("MuchDigi", &Output_Much_Digi);
    New_Data_File_Tree->SetBranchAddress("MuchDigiPulser", &Output_Much_Pulser_Digi);

    for (Int_t iTreeEntry = 0; iTreeEntry < nTimeSlices; iTreeEntry++)
    {
        Input_Data_File_Tree->GetEntry(iTreeEntry);
        Int_t iTimeSlice = Input_Event_Header->GetTsIndex();
        Output_Event_Header = Input_Event_Header;
        Output_Timeslice = Input_Timeslice;
        Output_Much_Pulser_Digi = Input_Much_Pulser_Digi;

        cout << "TimeSlice Number " << iTimeSlice << "  under digi Cleanup.... ";

        for (Int_t iMuchDigi = 0; iMuchDigi < Input_Much_Digi->size(); iMuchDigi++)
        {
            int32_t Station = CbmMuchAddress::GetStationIndex(Input_Much_Digi->at(iMuchDigi).GetAddress());
            if (Station != 0)
                continue; // Only storing GEM Digis
            // int32_t  Layer     = CbmMuchAddress::GetLayerIndex(Input_Much_Digi->at(iMuchDigi).GetAddress());
            int32_t Sector = CbmMuchAddress::GetSectorIndex(Input_Much_Digi->at(iMuchDigi).GetAddress());
            int32_t Channel = CbmMuchAddress::GetChannelIndex(Input_Much_Digi->at(iMuchDigi).GetAddress());

            if (Sector < 0 || Sector > 96)
                continue;
            if (Channel < 0 || Channel > 22)
                continue;
            if (GEM_Noise[Channel][Sector] == -1)
                continue; // Removing Noisy Digis

            Int_t FEB_GEM = GEM_Feb_no_X_Y[Channel][Sector];
            Int_t FEB_CH = GEM_Ch_no_X_Y[Channel][Sector];
            Int_t ADC_Value = Input_Much_Digi->at(iMuchDigi).GetAdc();

            // Bool_t Ringing_Digi = false;

            // if((FEB_GEM==14 && ADC_Value<12) || (FEB_GEM==13 && ADC_Value<4) || (FEB_GEM==10 && ADC_Value<4) || (FEB_GEM==9  && ADC_Value<10))  // Low ADC Noise injection in certain FEBs due to ringing effect
            // {
            //     for(Int_t iMuchDigi1=iMuchDigi-1;iMuchDigi1>0;iMuchDigi1--)
            //     {
            //         Double_t Time_difference = ((Double_t)Input_Much_Digi->at(iMuchDigi).GetTime()-(Double_t)Input_Much_Digi->at(iMuchDigi1).GetTime() )/ 1000.0 ;
            //         if(Time_difference > 1.2000) break;

            //         int32_t  Station1   = CbmMuchAddress::GetStationIndex(Input_Much_Digi->at(iMuchDigi1).GetAddress());
            //         if(Station1!=0) continue; // Only storing GEM Digis
            //         // int32_t  Layer     = CbmMuchAddress::GetLayerIndex(Input_Much_Digi->at(iMuchDigi).GetAddress());
            //         int32_t  Sector1    = CbmMuchAddress::GetSectorIndex(Input_Much_Digi->at(iMuchDigi1).GetAddress());
            //         int32_t  Channel1   = CbmMuchAddress::GetChannelIndex(Input_Much_Digi->at(iMuchDigi1).GetAddress());

            //         if(Sector1<0 || Sector1>96) continue;
            //         if(Channel1<0 || Channel1>22) continue;
            //         if(GEM_Noise[Channel1][Sector1]==-1) continue; // Removing Noisy Digis

            //         Int_t FEB_GEM1 = GEM_Feb_no_X_Y[Channel1][Sector1];
            //         if(FEB_GEM != FEB_GEM1) continue;
            //         // Int_t FEB_CH1 = GEM_Ch_no_X_Y[Channel1][Sector1];
            //         Int_t ADC_Value1 = Input_Much_Digi->at(iMuchDigi1).GetAdc();

            //         if(FEB_GEM==14 && ADC_Value1>=12)
            //         {
            //             Ringing_Digi = true;
            //             break;
            //         }
            //         else if(FEB_GEM==13 && ADC_Value1>=4)
            //         {
            //             Ringing_Digi = true;
            //             break;
            //         }
            //         else if(FEB_GEM==10 && ADC_Value1>=4)
            //         {
            //             Ringing_Digi = true;
            //             break;
            //         }
            //         else if(FEB_GEM==9 && ADC_Value1>=10)
            //         {
            //             Ringing_Digi = true;
            //             break;
            //         }
            //     }
            // }

            // if(Ringing_Digi) continue; // Ignore crosstalk or ringing digis

            CbmMuchDigi* Corrected_Gem_Digi = &Input_Much_Digi->at(iMuchDigi);

            if (ADC_Value < 15) // Performing walk correction
            {
                Corrected_Gem_Digi->SetTime(Corrected_Gem_Digi->GetTime() - 2 * (15 - ADC_Value));
            }

            Output_Much_Digi->push_back(Corrected_Gem_Digi);
        }


        for (Int_t iMuchDigi = 0; iMuchDigi < Input_Much_Digi->size(); iMuchDigi++)
        {
            int32_t Station = CbmMuchAddress::GetStationIndex(Input_Much_Digi->at(iMuchDigi).GetAddress());
            if (Station != 1)
                continue; // Only storing Trigger Digis
            // int32_t  Layer     = CbmMuchAddress::GetLayerIndex(Input_Much_Digi->at(iMuchDigi).GetAddress());
            // int32_t  Sector    = CbmMuchAddress::GetSectorIndex(Input_Much_Digi->at(iMuchDigi).GetAddress());
            // int32_t  Channel   = CbmMuchAddress::GetChannelIndex(Input_Much_Digi->at(iMuchDigi).GetAddress());

            CbmMuchDigi* Corrected_Trigger_Digi = &Input_Much_Digi->at(iMuchDigi);
            Corrected_Trigger_Digi->SetTime(Corrected_Trigger_Digi->GetTime() - Time_Offset);
            Output_Much_Digi->push_back(Corrected_Trigger_Digi);
        }

        std::sort(Output_Much_Digi->begin(), Output_Much_Digi->end(), [](const CbmMuchDigi& lhs, const CbmMuchDigi& rhs)
                  { return lhs.GetTime() < rhs.GetTime(); });

        Int_t iMuchStartDigi = 0;

        for (Int_t iMuchDigi = 0; iMuchDigi < Output_Much_Digi->size(); iMuchDigi++)
        {
            int32_t Station = CbmMuchAddress::GetStationIndex(Output_Much_Digi->at(iMuchDigi).GetAddress());
            if (Station != 1)
                continue; // Only storing Trigger Digis
            int32_t Channel = CbmMuchAddress::GetChannelIndex(Output_Much_Digi->at(iMuchDigi).GetAddress());

            for (Int_t iMuchDigi1 = iMuchStartDigi; iMuchDigi1 < Output_Much_Digi->size(); iMuchDigi1++)
            {
                if (iMuchDigi == iMuchDigi1)
                    continue;
                Double_t Time_difference = ((Double_t)Output_Much_Digi->at(iMuchDigi).GetTime() - (Double_t)Output_Much_Digi->at(iMuchDigi1).GetTime()) / 1000.0;

                if (Time_difference > -0.125)
                {
                    iMuchStartDigi = iMuchDigi1;
                    continue;
                }

                if (Time_difference < -0.800)
                    break;

                int32_t Station1 = CbmMuchAddress::GetStationIndex(Output_Much_Digi->at(iMuchDigi1).GetAddress());
                if (Station1 != 0)
                    continue; // Only selecting Much Digis
                // int32_t  Layer1     = CbmMuchAddress::GetLayerIndex(Input_Much_Digi->at(iMuchDigi).GetAddress());
                int32_t Sector1 = CbmMuchAddress::GetSectorIndex(Output_Much_Digi->at(iMuchDigi1).GetAddress());
                int32_t Channel1 = CbmMuchAddress::GetChannelIndex(Output_Much_Digi->at(iMuchDigi).GetAddress());

                Int_t FEB_GEM1 = GEM_Feb_no_X_Y[Channel1][Sector1];
                // Int_t FEB_CH1 = GEM_Ch_no_X_Y[Channel1][Sector1];
                Int_t ADC_Value1 = Output_Much_Digi->at(iMuchDigi1).GetAdc();

                if ((FEB_GEM1 == 14 && ADC_Value1 < 12) || (FEB_GEM1 == 13 && ADC_Value1 < 4) || (FEB_GEM1 == 10 && ADC_Value1 < 4) || (FEB_GEM1 == 9 && ADC_Value1 < 10))
                    Output_Much_Digi->erase(Output_Much_Digi->begin() + iMuchDigi1);
            }
        }

        // Just a precautionary step !
        std::sort(Output_Much_Digi->begin(), Output_Much_Digi->end(), [](const CbmMuchDigi& lhs, const CbmMuchDigi& rhs)
                  { return lhs.GetTime() < rhs.GetTime(); });

        New_Data_File_Tree->Fill();

        Output_Much_Digi->clear();
        cout << " Done! " << endl;
    }

    New_Data_File_Tree->Write();

    Input_Data_File->Close();
    Output_Data_File->Close();

    Code_Timer.Stop();

    cout << "Output File Name is: " << Output_Data_File_Name << endl;

    cout << "_______________________Code Executed Successfully!________________________" << endl;
    cout << "Real time: " << Code_Timer.RealTime() / 60.0 << " m, CPU time: " << Code_Timer.CpuTime() / 60.0 << " m" << endl;
}
