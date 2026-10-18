// -----------------------------------------------------------------------------
// Copyright (C) Experimental High Energy Physics and Applications Group, VECC, Kolkata
// Study_Unpack_File_GIF.C
// Project : GEM detector test beam at CERN GIF++ (2023)
// Author  : Apar Agarwal
// Usage   : root -l -b -q 'Study_Unpack_File_GIF.C'  (see README for input file layout)
// -----------------------------------------------------------------------------

void Highlight_RPC_Pad(Int_t Column_number, Int_t Sector_number, TH2F*& PadXandY);
void Highlight_Pad(Int_t Column_number, Int_t Sector_number, TH2F*& PadXandY);
void Study_Unpack_File_GIF()
{
    TFile* fIn = new TFile("Run166_0000.digi.root");
    TTree* Input_Tree = (TTree*)fIn->Get("cbmsim");

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

    TTreeReaderArray<CbmMuchDigi> Much_Digi(Tree_Data_Reader, "MuchDigi");              // MuchDigi -> Branch Name  and Much_Digi --> Element to Read Much data
    TTreeReaderArray<CbmMuchDigi> Much_Digi_Pulser(Tree_Data_Reader, "MuchDigiPulser"); // MuchDigiPulser -> Branch Name  and Much_Digi_Pulser --> Element to Read Much data
    TTreeReaderValue<CbmTsEventHeader> TimeSlice_Event_Header(Tree_Data_Reader, "EventHeader.");

    Int_t iTreeEntry;
    Int_t FirstDigiFlag = 0;
    Int_t LastDigiFlag = 0;
    uint64_t First_Digi_Time;
    uint64_t Last_Digi_Time;

    // TH2F* RPC_Beam_Spot = new TH2F("RPC_Beam_Spot","Beam Coverage in RPC in GIF++",4800,-60.0,60.0,5200,-30.0,100.0);
    TH2F* RPC_Beam_Spot = new TH2F("RPC_Beam_Spot", "Beam Coverage in RPC in GIF++", 4800, 25.0, 145.0, 5200, -85.0, -15.0);
    RPC_Beam_Spot->SetMarkerColor(2);
    RPC_Beam_Spot->SetFillColor(2);

    TH2F* PadXandY_GEM_Noisy = new TH2F("PadXandY_GEM_Noisy", "Pad Hits in Gem 1 (with Noise) for Run ", 3280, -40, 41, 3640, -30, 60);
    PadXandY_GEM_Noisy->GetXaxis()->SetTitle("X Coordinate (cm) -->");
    PadXandY_GEM_Noisy->GetYaxis()->SetTitle("Y Coordinate (cm) -->");

    TH1I* Time_Correlation_Feb_Trigger = new TH1I("Time_Correlation_Feb_Trigger", "Time Correlation b/w Feb 0 and Trigger", 10000, -20.0, 20.0);
    Time_Correlation_Feb_Trigger->GetXaxis()->SetTitle("Time difference (us)");
    Time_Correlation_Feb_Trigger->GetYaxis()->SetTitle("Counts");

    TH1I* Time_Correlation_GEM_Trigger = new TH1I("Time_Correlation_GEM_Trigger", "Time Correlation b/w GEM and Trigger", 20000, -10000, 10000);
    Time_Correlation_GEM_Trigger->GetXaxis()->SetTitle("Time difference (ns)");
    Time_Correlation_GEM_Trigger->GetYaxis()->SetTitle("Counts");

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

    TH2I* ChannelMapFebWise = new TH2I("ChannelMapFebWise", "Channel Map GEM Feb no:", );

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

    // cout<<" Last_Digi_Time - First_Digi_Time: "<<1.0*(Last_Digi_Time-First_Digi_Time)/(nTimeSlices*1.0)<<endl;

    for (iTreeEntry = 0; iTreeEntry < 100; iTreeEntry++)
    // for(iTreeEntry = 2314; iTreeEntry < 2315 ;iTreeEntry++)
    // for(iTreeEntry = 45; iTreeEntry < nTimeSlices ;iTreeEntry++)
    {
        // if(iTreeEntry<45) continue;
        // if(iTreeEntry>85 && iTreeEntry<215) continue;
        // if(iTreeEntry>245 && iTreeEntry<525) continue;
        // if(iTreeEntry>555 && iTreeEntry<695) continue;
        // if(iTreeEntry>725 && iTreeEntry<1005) continue;
        // if(iTreeEntry>1035 && iTreeEntry<1175) continue;
        // if(iTreeEntry>1205) continue;

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
        // Bool_t Trigger_Flag = false;

        for (Int_t iMuchDigi = 0; iMuchDigi < Much_Digi.GetSize(); iMuchDigi++)
        // for(Int_t iMuchDigi=0;iMuchDigi < -1;iMuchDigi++)
        {
            int32_t Station = CbmMuchAddress::GetStationIndex(Much_Digi[iMuchDigi].GetAddress());
            if (Station != 0)
                continue;
            // if(Station !=2) continue;
            // if(Much_Digi[iMuchDigi].GetAdc()<31) continue;
            // int32_t  Layer     = CbmMuchAddress::GetLayerIndex(Much_Digi[iMuchDigi].GetAddress());
            int32_t Sector = CbmMuchAddress::GetSectorIndex(Much_Digi[iMuchDigi].GetAddress());
            int32_t Channel = CbmMuchAddress::GetChannelIndex(Much_Digi[iMuchDigi].GetAddress());
            // if(Channel ==6 && Sector < 5 )
            // cout<<Sector<<"  "<<Channel<<endl;
            if (Sector > 96 || Channel > 22)
                continue;
            int FEB_GEM = GEM_Feb_no_X_Y[Channel][Sector];
            // if(FEB_GEM<9 || FEB_GEM>13) continue;
            if (FEB_GEM != 1)
                continue;
            // GEM_Digis++;
            // int FEB_CH_GEM = GEM_Ch_no_X_Y[Channel][Sector];

            uint64_t Much_Digi_Time = TimeSlice_Start_Time + (uint64_t)Much_Digi[iMuchDigi].GetTime() - First_Digi_Time;

            // Double_t Much_Digi_Time_ms = Much_Digi_Time / 1000000.0 ;
            // Double_t Much_Digi_Time_us = Much_Digi_Time / 1000.0 ;
            // Double_t Much_Digi_Time_ns = Much_Digi_Time  ;

            // if(Sector>100)
            // cout<<"Station: "<<Station<<" Layer: "<<Layer<<" Sector: "<<Sector<<" Channel: "<<Channel<<" FEB: "<<FEB_GEM<<endl;

            // if(Sector>100) continue;
            // if(Much_Digi[iMuchDigi].GetAdc()<12) continue;
            // cout<<Much_Digi_Pulser[iMuchDigi].GetTime()<<endl;
            // for(Int_t iMuchDigi1=0;iMuchDigi1 < Much_Digi_Pulser.GetSize();iMuchDigi1++)
            // for(Int_t iMuchDigi1=iMuchStartDigi;iMuchDigi1 < Much_Digi_Pulser.GetSize();iMuchDigi1++)
            // {
            //     Double_t First_Time_difference = ((Double_t)Much_Digi[iMuchDigi].GetTime()-(Double_t)Much_Digi_Pulser[0].GetTime() )/ 1000.0 ;

            //                 // if(iMuchDigi==iMuchDigi1) continue;
            //                 // int32_t  Station1   = CbmMuchAddress::GetStationIndex(Much_Digi_Pulser[iMuchDigi1].GetAddress());
            //                 // int32_t  Layer1     = CbmMuchAddress::GetLayerIndex(Much_Digi_Pulser[iMuchDigi1].GetAddress());
            //                 // int32_t  Sector1    = CbmMuchAddress::GetSectorIndex(Much_Digi_Pulser[iMuchDigi1].GetAddress());
            //                 // int32_t  Channel1   = CbmMuchAddress::GetChannelIndex(Much_Digi_Pulser[iMuchDigi1].GetAddress());
            //                 // if(Sector1) cout<<"Station: "<<Station1<<" Layer: "<<Layer1<<" Sector: "<<Sector1<<" Channel: "<<Channel1<<endl;
            //                 // if(Sector1<100) continue;
            //                 // if(Much_Digi_Pulser[iMuchDigi1].GetAdc()!=31) continue;
            //                 // uint64_t Much_Digi_Time_1 = TimeSlice_Start_Time + (uint64_t)Much_Digi_Pulser[iMuchDigi1].GetTime() - First_Digi_Time;
            //                 // Double_t TimeDiff = (Much_Digi_Time_1-Much_Digi_Time)/1000000.0;
            //     Double_t Time_difference = ((Double_t)Much_Digi[iMuchDigi].GetTime()-(Double_t)Much_Digi_Pulser[iMuchDigi1].GetTime() )/ 1000.0 ;
            //     // if(TMath::Abs(Time_difference)< 0.5) Trigger_Flag = true;

            //     if(First_Time_difference > 0.0)
            //     {
            //         if(Time_difference > 10.0 )
            //         {
            //             iMuchStartDigi = iMuchDigi1;
            //             iMuchDigi1++;
            //             continue;
            //         }
            //         else if(Time_difference < -10.0) break;
            //     }
            //     else
            //     {
            //         if(Time_difference < -10.0 ) break;
            //     }

            //                 // cout<< (Double_t)Much_Digi[iMuchDigi].GetTime() << "\t"<< (Double_t)Much_Digi_Pulser[iMuchDigi1].GetTime()<<endl;
            //                 // Time_Correlation_Feb_Trigger->Fill(Time_difference);
            //     Time_Correlation_GEM_Trigger->Fill(Time_difference);
            // }

            // Int_t FEB = RPC_Feb_no_X_Y[Channel][Sector];
            // Int_t FEB_CH = RPC_Ch_no_X_Y[Channel][Sector];
            // if(FEB==3) FEB=2;
            // else if(FEB==2) FEB=3;
            // Channel = RPCarrayX[FEB*128+(127-FEB_CH)];
            // Sector = RPCarrayY[FEB*128+(127-FEB_CH)];
            // if(Trigger_Flag) Highlight_Pad(Channel,Sector,PadXandY_GEM_Noisy);
            // Trigger_Flag = false;
            // if(Sector<100)
            // Highlight_RPC_Pad(Channel,Sector,RPC_Beam_Spot);
            // if(Sector<100)
            // cout<<"Time: "<<First_Digi_Time<<"  Much digi time: "<<Much_Digi_Time_ms<<endl;
            SpillStructureGEM->Fill(Much_Digi_Time);
            // cout<<"Station: "<<Station<<" Layer: "<<Layer<<" Sector: "<<Sector<<" Channel: "<<Channel<<endl;
        }

        // SpillStructureGEMTS->SetBinContent(iTreeEntry+1,GEM_Digis);
        // cout<<GEM_Digis<<endl;
        // GEM_Digis=0;
    }
    // Time_Correlation_Feb_Trigger->Draw();
    // Time_Correlation_GEM_Trigger->Draw();
    // SpillStructureTrigger->Draw();
    // PadXandY_GEM_Noisy->Draw("colz");
    // SpillStructureTriggerTS->Draw("HIST");
    // SpillStructureGEMTS->Draw("HIST");
    SpillStructureGEM->Draw("HIST");
    // RPC_Beam_Spot->Draw("colz");
    // fIn->Close();
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
            }
        }
    }
}
