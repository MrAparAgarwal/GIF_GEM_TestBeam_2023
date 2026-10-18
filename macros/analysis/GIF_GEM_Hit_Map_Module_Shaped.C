// -----------------------------------------------------------------------------
// Copyright (C) Experimental High Energy Physics and Applications Group, VECC, Kolkata
// GIF_GEM_Hit_Map_Module_Shaped.C
// Project : GEM detector test beam at CERN GIF++ (2023)
// Author  : Apar Agarwal
// Usage   : root -l -b -q 'GIF_GEM_Hit_Map_Module_Shaped.C'  (see README for input file layout)
// -----------------------------------------------------------------------------

void Highlight_RPC_Pad(Int_t Column_number, Int_t Sector_number, TH2F*& PadXandY);
void Highlight_Pad(Int_t Column_number, Int_t Sector_number, Long_t Weight, TH2F*& PadXandY);

void Boundary_Plotter_GEM(Int_t Column_number, Int_t Sector_number, TH2F*& PadXandY, Bool_t left, Bool_t right, Bool_t top, Bool_t bottom);
void Draw_Arrow_Left_To_Right(Double_t X1, Double_t X2, Double_t Y, TH2F*& PadXandY);
void Draw_Arrow_Top_To_Bottom(Double_t X, Double_t Y1, Double_t Y2, TH2F*& PadXandY);
void Draw_Feb(Double_t PosX, Double_t PosY, Int_t Feb_no, TH2F*& PadXandY);
void Write_Wall(Bool_t Wall_Is_On_The_Left, Bool_t Wall_Is_On_The_Right, TH2F*& PadXandY);

void GIF_GEM_Hit_Map_Module_Shaped(Int_t RunId = 966)
{
    TString InputFileName = "Run" + to_string(RunId) + "_0001.digi.root";
    TFile* fIn = new TFile(InputFileName);
    TTree* Input_Tree = (TTree*)fIn->Get("cbmsim");

    TString GEM_Noise_File_Name = "Run_" + to_string(RunId) + "_GEM_Noise.txt";

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

    TString OutputFileName = "data/Run" + to_string(RunId) + "_GEM_Hit_Map_Module_Shaped_v4.root";
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

    TTreeReaderArray<CbmMuchDigi> Much_Digi(Tree_Data_Reader, "MuchDigi");              // MuchDigi -> Branch Name  and Much_Digi --> Element to Read Much data
    TTreeReaderArray<CbmMuchDigi> Much_Digi_Pulser(Tree_Data_Reader, "MuchDigiPulser"); // MuchDigiPulser -> Branch Name  and Much_Digi_Pulser --> Element to Read Much data
    TTreeReaderValue<CbmTsEventHeader> TimeSlice_Event_Header(Tree_Data_Reader, "EventHeader.");

    Int_t iTreeEntry;
    Int_t FirstDigiFlag = 0;
    Int_t LastDigiFlag = 0;
    uint64_t First_Digi_Time;
    uint64_t Last_Digi_Time;

    // TH2F* RPC_Beam_Spot = new TH2F("RPC_Beam_Spot","Beam Coverage in RPC in GIF++",4800,-60.0,60.0,5200,-30.0,100.0);
    TH2F* GEM_Feb_Boundaries_and_Labels = new TH2F("GEM_Feb_Boundaries_and_Labels", "", 3640, 10, 140, 3280, 40, 110);
    GEM_Feb_Boundaries_and_Labels->SetTitle("Various Febs in GEM Module");

    TH2F* PadXandY_GEM = new TH2F("PadXandY_GEM", "Pad Hits in GEM for Run ", 3640, 10, 140, 3280, 40, 110);
    PadXandY_GEM->GetXaxis()->SetTitle("X Coordinate (cm) -->");
    PadXandY_GEM->GetYaxis()->SetTitle("Y Coordinate (cm) -->");

    TH2F* PadXandY_GEM_Triggered = new TH2F("PadXandY_GEM_Triggered", "Pad Hits in GEM for Run ", 3640, 10, 140, 3280, 40, 110);
    PadXandY_GEM_Triggered->GetXaxis()->SetTitle("X Coordinate (cm) -->");
    PadXandY_GEM_Triggered->GetYaxis()->SetTitle("Y Coordinate (cm) -->");

    TH1I* SpillStructureGEM = new TH1I("SpillStructureGEM", "Spill Structure for GEM Run", 1800000, -1000000.0, 179000000.0);
    SpillStructureGEM->GetXaxis()->SetTitle("Time (#mus)-->");
    SpillStructureGEM->GetYaxis()->SetTitle("Counts -->");

    TH1I* SpillStructureTrigger = new TH1I("SpillStructureTrigger", "Spill Structure for Trigger", 12900, -1.0, 128.0);
    SpillStructureTrigger->GetXaxis()->SetTitle("Time (ms)-->");
    SpillStructureTrigger->GetYaxis()->SetTitle("Counts -->");

    TH1I* SpillStructureGEMTS = new TH1I("SpillStructureGEMTS", "Spill Structure for GEM Run", nTimeSlices + 10, -1.5, nTimeSlices + 8.5);
    SpillStructureGEMTS->GetXaxis()->SetTitle("Timeslice no.-->");
    SpillStructureGEMTS->GetYaxis()->SetTitle("Counts -->");

    TH1I* SpillStructureTriggerTS = new TH1I("SpillStructureTriggerTS", "Spill Structure for Trigger", nTimeSlices + 10, -1.5, nTimeSlices + 8.5);
    SpillStructureTriggerTS->GetXaxis()->SetTitle("Timeslice no.-->");
    SpillStructureTriggerTS->GetYaxis()->SetTitle("Counts -->");

    for (Int_t iSector = 0; iSector < 97; iSector++)
    {
        for (Int_t iChannel = 0; iChannel < 23; iChannel++)
        {
            Bool_t top = false;
            Bool_t bottom = false;
            Bool_t left = false;
            Bool_t right = false;

            if (iSector == 0)
                top = true;
            else if (GEM_Feb_no_X_Y[iChannel][iSector] != GEM_Feb_no_X_Y[iChannel][iSector - 1])
                top = true;

            if (iSector == 96)
                bottom = true;
            else if (GEM_Feb_no_X_Y[iChannel][iSector] != GEM_Feb_no_X_Y[iChannel][iSector + 1])
                bottom = true;

            if (iChannel == 0)
                right = true;
            else if (GEM_Feb_no_X_Y[iChannel][iSector] != GEM_Feb_no_X_Y[iChannel - 1][iSector])
                right = true;

            if (iChannel == 22)
                left = true;
            else if (GEM_Feb_no_X_Y[iChannel][iSector] != GEM_Feb_no_X_Y[iChannel + 1][iSector])
                left = true;

            Boundary_Plotter_GEM(iChannel, iSector, GEM_Feb_Boundaries_and_Labels, left, right, top, bottom);
        }
    }

    Draw_Arrow_Left_To_Right(34.0, 39.0, 75.5, GEM_Feb_Boundaries_and_Labels);
    Draw_Feb(25.5, 74.0, 0, GEM_Feb_Boundaries_and_Labels);

    Draw_Arrow_Left_To_Right(22.5, 41.5, 73.5, GEM_Feb_Boundaries_and_Labels);
    Draw_Feb(14.0, 72.0, 1, GEM_Feb_Boundaries_and_Labels);

    Draw_Arrow_Left_To_Right(22.5, 41.5, 77.5, GEM_Feb_Boundaries_and_Labels);
    Draw_Feb(14.0, 76.0, 2, GEM_Feb_Boundaries_and_Labels);

    Draw_Arrow_Left_To_Right(34.0, 45.0, 71.5, GEM_Feb_Boundaries_and_Labels);
    Draw_Feb(25.5, 70.0, 3, GEM_Feb_Boundaries_and_Labels);

    Draw_Arrow_Top_To_Bottom(53.0, 75.0, 60.0, GEM_Feb_Boundaries_and_Labels);
    Draw_Arrow_Top_To_Bottom(54.0, 73.4, 60.0, GEM_Feb_Boundaries_and_Labels);
    Draw_Feb(49, 55.0, 4, GEM_Feb_Boundaries_and_Labels);

    Draw_Arrow_Top_To_Bottom(48.2, 89.0, 78.2, GEM_Feb_Boundaries_and_Labels);
    Draw_Feb(44.0, 90.0, 4, GEM_Feb_Boundaries_and_Labels);

    Draw_Arrow_Left_To_Right(34.0, 45.0, 79.5, GEM_Feb_Boundaries_and_Labels);
    Draw_Feb(25.5, 78.0, 5, GEM_Feb_Boundaries_and_Labels);

    Draw_Arrow_Top_To_Bottom(58.0, 70.0, 50.0, GEM_Feb_Boundaries_and_Labels);
    Draw_Feb(55.5, 45.5, 6, GEM_Feb_Boundaries_and_Labels);

    Draw_Arrow_Top_To_Bottom(65.0, 73.4, 60.0, GEM_Feb_Boundaries_and_Labels);
    Draw_Feb(60, 55.0, 7, GEM_Feb_Boundaries_and_Labels);

    Draw_Arrow_Top_To_Bottom(60.0, 89.0, 77.5, GEM_Feb_Boundaries_and_Labels);
    Draw_Arrow_Top_To_Bottom(64.0, 89.0, 80.6, GEM_Feb_Boundaries_and_Labels);
    Draw_Feb(58.0, 90.0, 8, GEM_Feb_Boundaries_and_Labels);

    Draw_Arrow_Top_To_Bottom(77.0, 74.0, 60.0, GEM_Feb_Boundaries_and_Labels);
    Draw_Feb(72.0, 55.0, 8, GEM_Feb_Boundaries_and_Labels);

    Draw_Arrow_Top_To_Bottom(56.0, 99.0, 81.0, GEM_Feb_Boundaries_and_Labels);
    Draw_Feb(52.0, 100.0, 9, GEM_Feb_Boundaries_and_Labels);

    Draw_Arrow_Top_To_Bottom(74.0, 89.0, 78.1, GEM_Feb_Boundaries_and_Labels);
    Draw_Arrow_Top_To_Bottom(76.5, 89.0, 78.1, GEM_Feb_Boundaries_and_Labels);
    Draw_Arrow_Top_To_Bottom(78.2, 89.0, 78.1, GEM_Feb_Boundaries_and_Labels);
    Draw_Feb(72.0, 90.0, 9, GEM_Feb_Boundaries_and_Labels);

    Draw_Arrow_Top_To_Bottom(71.0, 66.0, 50.0, GEM_Feb_Boundaries_and_Labels);
    Draw_Feb(67.5, 45.5, 10, GEM_Feb_Boundaries_and_Labels);

    Draw_Arrow_Top_To_Bottom(87.5, 69.0, 59.0, GEM_Feb_Boundaries_and_Labels);
    Draw_Feb(82.5, 55.0, 11, GEM_Feb_Boundaries_and_Labels);

    Draw_Arrow_Top_To_Bottom(93.0, 83.0, 50.0, GEM_Feb_Boundaries_and_Labels);
    Draw_Arrow_Top_To_Bottom(94.0, 73.0, 50.0, GEM_Feb_Boundaries_and_Labels);
    Draw_Feb(89.0, 45.5, 12, GEM_Feb_Boundaries_and_Labels);

    Draw_Arrow_Top_To_Bottom(68.5, 99.0, 80.0, GEM_Feb_Boundaries_and_Labels);
    Draw_Feb(63.5, 100, 13, GEM_Feb_Boundaries_and_Labels);

    Draw_Arrow_Top_To_Bottom(83.0, 99.0, 78.0, GEM_Feb_Boundaries_and_Labels);
    Draw_Feb(78.0, 100, 13, GEM_Feb_Boundaries_and_Labels);

    Draw_Arrow_Top_To_Bottom(90.0, 94.0, 88.7, GEM_Feb_Boundaries_and_Labels);
    Draw_Feb(85.0, 95, 14, GEM_Feb_Boundaries_and_Labels);

    Draw_Arrow_Left_To_Right(115.0, 124.0, 86.5, GEM_Feb_Boundaries_and_Labels);
    Draw_Feb(125.0, 85.0, 15, GEM_Feb_Boundaries_and_Labels);

    Draw_Arrow_Left_To_Right(115.0, 124.0, 74.5, GEM_Feb_Boundaries_and_Labels);
    Draw_Feb(125.0, 73.0, 16, GEM_Feb_Boundaries_and_Labels);

    Draw_Arrow_Left_To_Right(115.0, 124.0, 61.5, GEM_Feb_Boundaries_and_Labels);
    Draw_Feb(125.0, 60.0, 17, GEM_Feb_Boundaries_and_Labels);

    // Write_Wall(true,false,GEM_Feb_Boundaries_and_Labels);
    Write_Wall(false, true, GEM_Feb_Boundaries_and_Labels);

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

    for (iTreeEntry = 0; iTreeEntry < 200; iTreeEntry++)
    // for(iTreeEntry = 2314; iTreeEntry < 2315 ;iTreeEntry++)
    // for(iTreeEntry = 0; iTreeEntry < nTimeSlices ;iTreeEntry++)
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
            // Int_t GEM_FEB = GEM_Feb_no_X_Y[Channel][Sector];
            GEM_Hits_Raw[Channel][Sector]++;

            // Highlight_Pad(Channel,Sector,PadXandY_GEM);
            // cout<<"Station: "<<Station<<" Layer: "<<Layer<<" Sector: "<<Sector<<" Channel: "<<Channel<<endl;
        }

        iMuchStartDigi = 0;
        Int_t iLastDigi = iMuchStartDigi;

        for (Int_t iMuchDigi = 0; iMuchDigi < Much_Digi.GetSize(); iMuchDigi++, Trigger_Flag = false)
        // for(Int_t iMuchDigi=iMuchStartDigi;iMuchDigi1 < Much_Digi.GetSize();iMuchDigi1++,TriggerFlag=false)
        {
            int32_t Channel = CbmMuchAddress::GetChannelIndex(Much_Digi[iMuchDigi].GetAddress());
            if (Channel != 125)
                continue; // 2-FOLD Coincidence only
            // if(Channel>=64) continue; // 2-FOLD Coincidence only
            // if(Channel<64) continue; // 3-FOLD Coincidence only
            int32_t Station = CbmMuchAddress::GetStationIndex(Much_Digi[iMuchDigi].GetAddress());
            if (Station != 1)
                continue;

            Int_t Trigger_Digi = 0;

            Int_t iMuchStartDigi = iMuchDigi - 5000;
            if (iMuchStartDigi < 0)
                iMuchStartDigi = 0;
            Int_t iMuchLastDigi = iMuchDigi + 5000;
            if (iMuchLastDigi > Much_Digi.GetSize() - 1)
                iMuchLastDigi = Much_Digi.GetSize() - 1;

            for (Int_t iMuchDigi1 = iMuchStartDigi; iMuchDigi1 < iMuchLastDigi; iMuchDigi1++)
            // for(Int_t iMuchDigi1=iMuchStartDigi;iMuchDigi1 < Much_Digi.GetSize();iMuchDigi1++)
            {
                if (iMuchDigi == iMuchDigi1)
                    continue;
                int32_t Station1 = CbmMuchAddress::GetStationIndex(Much_Digi[iMuchDigi1].GetAddress());
                if (Station1 != 0)
                    continue;
                Double_t First_Time_difference = ((Double_t)Much_Digi[iMuchDigi1].GetTime() - (Double_t)Much_Digi[0].GetTime()) / 1000.0;

                Double_t Time_difference = ((Double_t)Much_Digi[iMuchDigi].GetTime() - (Double_t)Much_Digi[iMuchDigi1].GetTime()) / 1000.0;
                // cout<<First_Time_difference<<"    "<<iMuchStartDigi<<"     "<<iMuchDigi<<"    "<<iMuchDigi1<<"/"<<Much_Digi.GetSize()<<"  "<<Time_difference<<endl;

                if (First_Time_difference > 0.0)
                {
                    // cout<<Time_difference<<endl;
                    // if(Time_difference > 1000.0 )
                    // {
                    //     iMuchStartDigi = iMuchDigi1;
                    //     iMuchDigi1+=100000;
                    //     continue;
                    // }
                    // if(Time_difference > 100.0 )
                    // {
                    //     iMuchStartDigi = iMuchDigi1;
                    //     iMuchDigi1+=4000;
                    //     continue;
                    // }
                    // if(Time_difference > 50.0 )
                    // {
                    //     iMuchStartDigi = iMuchDigi1;
                    //     iMuchDigi1+=100;
                    //     continue;
                    // }
                    // if(Time_difference > 15.0 )
                    // {
                    //     iMuchStartDigi = iMuchDigi1;
                    //     iMuchDigi1+=10;
                    //     continue;
                    // }
                    // if(Time_difference > 11.0 )
                    // {
                    //     iMuchStartDigi = iMuchDigi1;
                    //     iMuchDigi1++;
                    //     continue;
                    // }


                    if (Time_difference > 0.450)
                    {
                        // iMuchStartDigi = iMuchDigi1;
                        // iMuchDigi1++;
                        continue;
                    }
                    else if (Time_difference < 0.250)
                        break;
                    else
                    {
                        int32_t Sector1 = CbmMuchAddress::GetSectorIndex(Much_Digi[iMuchDigi1].GetAddress());
                        int32_t Channel1 = CbmMuchAddress::GetChannelIndex(Much_Digi[iMuchDigi1].GetAddress());

                        if (Sector1 > 96 || Channel1 > 22)
                            continue;
                        Int_t FEB_GEM = GEM_Feb_no_X_Y[Channel1][Sector1];
                        Int_t FEB_CH = GEM_Ch_no_X_Y[Channel1][Sector1];
                        if (GEM_Noise[Channel1][Sector1] == -1)
                            continue;
                        // cout<<iMuchDigi<<"  Sector: "<<Sector1<<"  Channel: "<<Channel1<<"  ADC: "<<Much_Digi[iMuchDigi].GetAdc()<<" Timestamp: "<<(uint64_t)Much_Digi[iMuchDigi1].GetTime()<<endl;
                        // cout<<iMuchDigi<<"  FEB: "<<FEB_GEM<<"  Channel: "<<FEB_CH<<"  ADC: "<<Much_Digi[iMuchDigi].GetAdc()<<" Timestamp: "<<(uint64_t)Much_Digi[iMuchDigi1].GetTime()<<endl;

                        // if(FEB_GEM==6 || FEB_GEM==14 || FEB_GEM==9) continue;
                        // if(iMuchDigi1!=0)
                        if (false)
                        {
                            if (Much_Digi[iMuchDigi1].GetAddress() == Much_Digi[iLastDigi].GetAddress())
                            {
                                if (Much_Digi[iMuchDigi1].GetTime() - Much_Digi[iLastDigi].GetTime() < 20000)
                                    continue;
                            }
                        }

                        iLastDigi = iMuchDigi1;
                        // cout<<iMuchDigi1<<"     "<<iLastDigi<<"  FEB: "<<FEB_GEM<<"  Channel: "<<FEB_CH<<"  Address: "<<Much_Digi[iMuchDigi].GetAddress()<<" Timestamp: "<<(uint64_t)Much_Digi[iMuchDigi1].GetTime()<<endl;

                        int32_t Triggered_Sector = CbmMuchAddress::GetSectorIndex(Much_Digi[iMuchDigi1].GetAddress());
                        int32_t Triggered_Channel = CbmMuchAddress::GetChannelIndex(Much_Digi[iMuchDigi1].GetAddress());
                        GEM_Hits_Triggered_2D[Triggered_Channel][Triggered_Sector]++;
                        // Highlight_Pad(Triggered_Channel,Triggered_Sector,PadXandY_GEM_Triggered);
                        // Trigger_Flag = true;
                        // break;
                        // Time_Correlation_GEM_Trigger->Fill((Double_t)Much_Digi[iMuchDigi].GetTime()-(Double_t)Much_Digi[iMuchDigi1].GetTime());
                    }
                }
                else
                {
                    if (Time_difference < -11.0)
                        break;
                }
            }

            // if(Trigger_Flag)
            // {
            //     int32_t  Triggered_Sector    = CbmMuchAddress::GetSectorIndex(Much_Digi[Trigger_Digi].GetAddress());
            //     int32_t  Triggered_Channel   = CbmMuchAddress::GetChannelIndex(Much_Digi[Trigger_Digi].GetAddress());

            //     Highlight_Pad(Triggered_Channel,Triggered_Sector,PadXandY_GEM_Triggered);
            // }
        }
    }

    for (Int_t iChannel = 0; iChannel < 23; iChannel++)
    {
        for (Int_t iSector = 0; iSector < 97; iSector++)
        {
            Long_t Weight = GEM_Hits_Raw[iChannel][iSector];
            Highlight_Pad(iChannel, iSector, Weight, PadXandY_GEM);
            Weight = GEM_Hits_Triggered_2D[iChannel][iSector];
            Highlight_Pad(iChannel, iSector, Weight, PadXandY_GEM_Triggered);
        }
    }

    // Time_Correlation_Feb_Trigger->Draw();
    // Time_Correlation_GEM_Trigger->Draw();
    // SpillStructureTrigger->Draw();
    // PadXandY_GEM->Draw("colz");
    // SpillStructureTriggerTS->Draw("HIST");
    // SpillStructureGEMTS->Draw("HIST");
    // SpillStructureGEM->Draw("HIST");
    PadXandY_GEM->Write();
    PadXandY_GEM_Triggered->Write();
    GEM_Feb_Boundaries_and_Labels->Write();
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

void Write_Wall(Bool_t Wall_Is_On_The_Left, Bool_t Wall_Is_On_The_Right, TH2F*& PadXandY)
{
    if (Wall_Is_On_The_Left == Wall_Is_On_The_Right)
    {
        cout << "Wall cannot be on both sides choosing left side for now" << endl;
        Wall_Is_On_The_Left = true;
        Wall_Is_On_The_Right = false;
    }

    Double_t PosX = 11.0, PosY = 100.0;

    if (Wall_Is_On_The_Right)
        PosX = 133.0;
    //Print W
    PosY = 108.0;

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
    PosY = 103.0;

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
    PosY = 98.0;

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
    PosY = 93.0;

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
