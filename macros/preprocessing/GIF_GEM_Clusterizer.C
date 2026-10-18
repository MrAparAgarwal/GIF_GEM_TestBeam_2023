// Copyright (C) Experimental High Energy Physics and Applications Group, VECC, Kolkata
// Author(s): Apar Agarwal [Developer]

// This macro "GIF_GEM_Clusterizer.C" has been created for the purpose of
// cluseterizing GEM Hits in GEM

// This Code requires the CbmRoot version DEC 21 or above
// Tested on Linux 20.04.05 --> Fairsoftversion : APR21p2 and FAIRROOTVERSION: v18.6.7 ---> Works Well!

// Last Updated: 27th November 2023
// For questions or issues, please open an issue on the GitHub repository.


Double_t GetX(Int_t Column_number, Int_t Sector_number, Bool_t Wall_Near_Smaller_Pads);
Double_t GetY(Int_t Column_number, Int_t Sector_number, Bool_t Wall_Near_Smaller_Pads);
Double_t Getdx(Int_t Column_number, Int_t Sector_number);
Double_t Getdy(Int_t Column_number, Int_t Sector_number);

// Double_t MuchZ = 125.0;

void GIF_GEM_Clusterizer(Int_t RunId = 966,
                         uint64_t Cluster_Seperation_Time = 50, // Unit: ns
                         Bool_t Wall_Near_Smaller_Pads = false)
{
    TStopwatch Code_Timer;
    Code_Timer.Start();

    TString InputFileName = "Run" + to_string(RunId) + "_Noise_Removed_Ringing_Walk_Corrected.digi.root";
    TString InputRefName = "2256.5.0e+06.200.10.rec.root";
    TString Output_Data_File_Name = "Run" + to_string(RunId) + ".rec.root";
    TString GEM_Noise_File_Name = "Run_" + to_string(RunId) + "_GEM_Noise.txt";

    if (gSystem->AccessPathName(InputFileName))
    {
        cout << "The file: " << InputFileName << " does not exist!" << endl;
        return;
    }

    if (gSystem->AccessPathName(InputRefName))
    {
        cout << "The file: " << InputRefName << " does not exist!" << endl;
        return;
    }

    TFile* Input_Data_File = new TFile(InputFileName);
    cout << "Input Data File Name :" << InputFileName << endl;

    TTree* Input_Data_File_Tree = (TTree*)Input_Data_File->Get("cbmsim;");
    // Data_File_Tree->Print();

    TFile* Input_Ref_File = new TFile(InputRefName);
    cout << "Input Reference File Name :" << InputRefName << endl;

    TTree* Input_Ref_File_Tree = (TTree*)Input_Ref_File->Get("cbmsim;");
    // Data_File_Tree->Print();

    if (!Input_Data_File_Tree)
    {
        cout << "Tree Not Found!" << endl;
        return;
    }

    if (!Input_Ref_File_Tree)
    {
        cout << "Reference Tree Not Found!" << endl;
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

    // TTree* New_Data_File_Tree = new TTree("cbmsim;","A Simple Tree");
    TTree* New_Data_File_Tree = Input_Ref_File_Tree->CloneTree(0);

    CbmTimeSlice* Input_Timeslice = new CbmTimeSlice();
    CbmTsEventHeader* Input_Event_Header = new CbmTsEventHeader();
    std::vector<CbmMuchDigi>* Input_Much_Digi = new std::vector<CbmMuchDigi>;
    std::vector<CbmMuchDigi>* Input_Much_Pulser_Digi = new std::vector<CbmMuchDigi>;

    Input_Data_File_Tree->SetBranchAddress("TimeSlice.", &Input_Timeslice);
    Input_Data_File_Tree->SetBranchAddress("EventHeader.", &Input_Event_Header);
    Input_Data_File_Tree->SetBranchAddress("MuchDigi", &Input_Much_Digi);
    Input_Data_File_Tree->SetBranchAddress("MuchDigiPulser", &Input_Much_Pulser_Digi);

    FairEventHeader* Output_Event_Header = new FairEventHeader();
    std::vector<CbmEvent>* Output_Event = new std::vector<CbmEvent>;
    // Event makes no sense in GIF++ Experiment because of gamma source hence writing this code is all the more challenging
    std::vector<CbmMuchCluster>* Output_Time_Cluster = new std::vector<CbmMuchCluster>;
    std::vector<CbmMuchCluster>* Output_Cluster = new std::vector<CbmMuchCluster>;
    std::vector<CbmMuchPixelHit>* Output_Pixel_Hit = new std::vector<CbmMuchPixelHit>;

    // New_Data_File_Tree->Branch("EventHeader.",&Output_Event_Header);
    // New_Data_File_Tree->Branch("CbmEvent",&Output_Event);
    // New_Data_File_Tree->Branch("MuchCluster",&Output_Cluster);
    // New_Data_File_Tree->Branch("MuchPixelHit",&Output_Pixel_Hit);

    New_Data_File_Tree->SetBranchAddress("EventHeader.", &Output_Event_Header);
    New_Data_File_Tree->SetBranchAddress("CbmEvent", &Output_Event);
    New_Data_File_Tree->SetBranchAddress("MuchCluster", &Output_Cluster);
    New_Data_File_Tree->SetBranchAddress("MuchPixelHit", &Output_Pixel_Hit);

    for (Int_t iTreeEntry = 0; iTreeEntry < nTimeSlices; iTreeEntry++)
    {
        Input_Data_File_Tree->GetEntry(iTreeEntry);
        Int_t iTimeSlice = Input_Event_Header->GetTsIndex();
        Int_t Cluster_Number = 0, Cluster_Digi_Number = 0;

        // std::vector<CbmMuchPad*> fFiredPads;
        std::vector<std::vector<Int_t>> fNeighbours;
        std::vector<Int_t> fDigiIndices;
        fDigiIndices.push_back(0); // Assume by default that 0th digi exists and it is part of a cluster
        // new ((*Output_Time_Cluster)[0]) CbmMuchCluster();
        CbmMuchCluster* Time_Cluster = new CbmMuchCluster(); //(CbmMuchCluster*)(*Output_Time_Cluster)[0];

        for (Int_t iMuchDigi = 1; iMuchDigi < Input_Much_Digi->size(); iMuchDigi++)
        {
            int32_t Station = CbmMuchAddress::GetStationIndex(Input_Much_Digi->at(iMuchDigi).GetAddress());
            if (Station != 0)
                continue; // Using only GEM Digis for clusterization
            // int32_t  Layer     = CbmMuchAddress::GetLayerIndex(Input_Much_Digi->at(iMuchDigi).GetAddress());
            int32_t Sector = CbmMuchAddress::GetSectorIndex(Input_Much_Digi->at(iMuchDigi).GetAddress());
            int32_t Channel = CbmMuchAddress::GetChannelIndex(Input_Much_Digi->at(iMuchDigi).GetAddress());

            uint64_t Time_Difference = Input_Much_Digi->at(iMuchDigi).GetTime() - Input_Much_Digi->at(iMuchDigi - 1).GetTime();

            if (Time_Difference > Cluster_Seperation_Time)
            {
                Int_t Module_Address = CbmMuchAddress::GetAddress(CbmMuchAddress::GetStationIndex(Input_Much_Digi->at(iMuchDigi).GetAddress()), CbmMuchAddress::GetLayerIndex(Input_Much_Digi->at(iMuchDigi).GetAddress()),
                                                                  CbmMuchAddress::GetLayerSideIndex(Input_Much_Digi->at(iMuchDigi).GetAddress()), CbmMuchAddress::GetModuleIndex(Input_Much_Digi->at(iMuchDigi).GetAddress()));
                Time_Cluster->SetAddress(Module_Address); // Since there is only one module this line is added for iMuchDigi and not iMuchDigi-1

                if (iMuchDigi == 1 && CbmMuchAddress::GetStationIndex(Input_Much_Digi->at(0).GetAddress()) == 0) // Only if 0th digi is much digi
                    Time_Cluster->AddDigis(fDigiIndices);
                else if (iMuchDigi > 1)
                    Time_Cluster->AddDigis(fDigiIndices);
                else
                    continue;

                // new ((*Output_Time_Cluster)[++Cluster_Number]) CbmMuchCluster();
                // Time_Cluster = (*Output_Time_Cluster)[Cluster_Number];
                Output_Time_Cluster->push_back(*Time_Cluster);
                // Time_Cluster->clear();
                fDigiIndices.clear();
                // Cluster_Digi_Number = 0;
            }

            // CbmMuchDigi* Cluster_Much_Digi = &Input_Much_Digi->at(iMuchDigi);
            fDigiIndices.push_back(iMuchDigi);
        }

        // fDigiIndices.clear();
        // std::vector<Int_t> Local_Digis_Flag;
        // // new ((*Output_Cluster)[0]) CbmMuchCluster();
        // // CbmMuchCluster* Local_Cluster = (*Output_Cluster)[0];
        // CbmMuchCluster* Local_Cluster = new CbmMuchCluster();
        // Cluster_Number=0;

        // for(Int_t iTimeCluster =0; iTimeCluster<Output_Time_Cluster->size();iTimeCluster++)
        // {
        //     Local_Digis_Flag = Output_Time_Cluster->at(iTimeCluster).GetDigis();
        //     Time_Cluster = (*Output_Time_Cluster)[iTimeCluster];

        //     for(Int_t iMuchDigi=0;iMuchDigi < Time_Cluster->GetNofDigis();iMuchDigi++)
        //     {
        //         if(Local_Digis_Flag->at(iMuchDigi)==-1) continue;
        //         int32_t  Station   = CbmMuchAddress::GetStationIndex(Input_Much_Digi->at(iMuchDigi).GetAddress());
        //         int32_t  Sector   = CbmMuchAddress::GetSectorIndex(Input_Much_Digi->at(iMuchDigi).GetAddress());
        //         int32_t  Channel   = CbmMuchAddress::GetChannelIndex(Input_Much_Digi->at(iMuchDigi).GetAddress());

        //         Local_Digis_Flag->at(iMuchDigi) = -1;
        //         fDigiIndices.push_back(iMuchDigi);

        //         for(Int_t iMuchDigi1=iMuchDigi +1;iMuchDigi1 < Time_Cluster->GetNofDigis();iMuchDigi1++)
        //         {
        //             if(Local_Digis_Flag->at(iMuchDigi1)==-1) continue;
        //             int32_t  Station1   = CbmMuchAddress::GetStationIndex(Input_Much_Digi->at(iMuchDigi1).GetAddress());
        //             int32_t  Sector1   = CbmMuchAddress::GetSectorIndex(Input_Much_Digi->at(iMuchDigi1).GetAddress());
        //             int32_t  Channel1   = CbmMuchAddress::GetChannelIndex(Input_Much_Digi->at(iMuchDigi1).GetAddress());

        //             if(TMath::Sqrt((Sector-Sector1)*(Sector-Sector1)+(Channel-Channel1)*(Channel-Channel1))< 2.85) // sqrt(8) = 2.8282 which is max distance 2 pads diagonally
        //             {
        //                 Local_Digis_Flag->at(iMuchDigi1) = -1;
        //                 fDigiIndices.push_back(iMuchDigi1);
        //             }
        //         }

        //         Int_t Module_Address   = CbmMuchAddress::GetAddress(CbmMuchAddress::GetStationIndex(Input_Much_Digi->at(iMuchDigi).GetAddress()), CbmMuchAddress::GetLayerIndex(Input_Much_Digi->at(iMuchDigi).GetAddress()),
        //                                                               CbmMuchAddress::GetLayerSideIndex(Input_Much_Digi->at(iMuchDigi).GetAddress()), CbmMuchAddress::GetModuleIndex(Input_Much_Digi->at(iMuchDigi).GetAddress()));
        //         Local_Cluster->SetAddress(Module_Address);
        //         Local_Cluster->AddDigis(fDigiIndices);
        //         new ((*Output_Cluster)[++Cluster_Number]) CbmMuchCluster();
        //         Local_Cluster = (CbmMuchCluster*) Output_Cluster->at(Cluster_Number);
        //     }
        // }

        // Int_t iHit = 0;

        // for(Int_t iLocalCluster =0; iLocalCluster<Output_Cluster->size();iLocalCluster++)
        // {
        //     Int_t nDigis  = Output_Cluster->at(iLocalCluster).GetNofDigis();
        //     Double_t sumq = 0, sumx = 0, sumy = 0, sumt = 0, sumdx2 = 0, sumdy2 = 0, sumdxy2 = 0, sumdt2 = 0;
        //     Double_t q = 0, x = 0, y = 0, t = 0, z = MuchZ, dx = 0, dy = 0, dxy = 0, dt = 0, tmin =0;
        //     Int_t address = 0;
        //     Int_t planeId = 0;

        //     for(Int_t iLocalClusterDigi=0;iLocalClusterDigi < nDigis;iLocalClusterDigi++)
        //     {
        //         Int_t iMuchDigi = Output_Cluster->at(iLocalCluster).GetDigi(iLocalClusterDigi);
        //         if(iLocalClusterDigi == 0) tmin = Input_Much_Digi->at(iMuchDigi).GetTime();
        //         int32_t  Sector   = CbmMuchAddress::GetSectorIndex(Input_Much_Digi->at(iMuchDigi).GetAddress());
        //         int32_t  Channel   = CbmMuchAddress::GetChannelIndex(Input_Much_Digi->at(iMuchDigi).GetAddress());
        //         x = GetX(Channel,Sector,Wall_Near_Smaller_Pads);
        //         y = GetY(Channel,Sector,Wall_Near_Smaller_Pads);
        //         q = Input_Much_Digi->at(iMuchDigi).GetAdc();
        //         dx = Getdx(Channel,Sector);
        //         dy = Getdy(Channel,Sector);;
        //         dxy = 0;
        //         sumq += q;
        //         sumx += q * x;
        //         sumy += q * y;
        //         sumt += q * t;
        //         sumdx2 += q * q * dx * dx;
        //         sumdy2 += q * q * dy * dy;
        //         sumdxy2 += q * q * dxy * dxy;
        //         sumdt2 += q * q * dt * dt;
        //     }

        //     dt = 4.0;               // Taken from class CbmMuchFindHitsGem.cxx
        //     x = sumx / sumq;
        //     y = sumy / sumq;
        //     t          = tmin;
        //     dx         = sqrt(sumdx2 / 12) / sumq;
        //     dy         = sqrt(sumdy2 / 12) / sumq;
        //     dxy        = sqrt(sumdxy2 / 12) / sumq;
        //     dt         = sqrt(sumdt2) / sumq;

        //     new ((*Output_Pixel_Hit)[iHit++]) CbmMuchPixelHit(address, x, y, z, dx, dy, 0, dxy, iLocalCluster, planeId, t, dt);
        // }
        New_Data_File_Tree->Fill();
    }

    New_Data_File_Tree->Write();

    Input_Data_File->Close();
    Input_Ref_File->Close();
    Output_Data_File->Close();

    cout << "Output File Name: " << Output_Data_File_Name << endl;

    Code_Timer.Stop();

    cout << "_______________________Code Executed Successfully!________________________" << endl;
    cout << "Real time: " << Code_Timer.RealTime() / 60.0 << " m, CPU time: " << Code_Timer.CpuTime() / 60.0 << " m" << endl;
}

Double_t GetX(Int_t Column_number, Int_t Sector_number, Bool_t Wall_Near_Smaller_Pads)
{
    if (Wall_Near_Smaller_Pads)
    {
        Float_t Radial_Coordinate_initial = 18.30, Radial_Coordinate_final = 18.3, Theta_Coordinate_initial, Theta_Coordinate_final;
        Theta_Coordinate_initial = ((Float_t)22 - Column_number) - 90.0 - 23.0 / 2.0;
        Theta_Coordinate_final = (Float_t)(22 - (Column_number - 1)) - 90.0 - 23.0 / 2.0;
        Float_t Pi = 3.1468;
        Float_t X_offset = -19.4103 + 60.0, Y_offset = 75.1912 + 85.0;

        for (Int_t i = 0; i < 97; i++)
        {
            Radial_Coordinate_final = Radial_Coordinate_initial + Radial_Coordinate_initial * Pi / 180;

            if (i == Sector_number)
                break;

            Radial_Coordinate_initial = Radial_Coordinate_final;
        }

        Double_t Centre_X = (Radial_Coordinate_initial * TMath::Sin(Theta_Coordinate_initial * 3.14159 / 180.0) + Radial_Coordinate_initial * TMath::Sin(Theta_Coordinate_final * 3.14159 / 180.0));
        Centre_X += (Radial_Coordinate_initial * TMath::Sin(Theta_Coordinate_initial * 3.14159 / 180.0) + Radial_Coordinate_initial * TMath::Sin(Theta_Coordinate_final * 3.14159 / 180.0));
        return -X_offset - Centre_X / 4.0;
    }
    else
    {
        Float_t Radial_Coordinate_initial = 18.30, Radial_Coordinate_final = 18.3, Theta_Coordinate_initial, Theta_Coordinate_final;
        Theta_Coordinate_initial = ((Float_t)22 - Column_number) + 90.0 - 23.0 / 2.0;
        Theta_Coordinate_final = (Float_t)(22 - (Column_number - 1)) + 90.0 - 23.0 / 2.0;
        Float_t Pi = 3.1468;
        Float_t X_offset = -19.4103 - 110.0 + 60.0, Y_offset = 75.1912 + 85.0;

        for (Int_t i = 0; i < 97; i++)
        {
            Radial_Coordinate_final = Radial_Coordinate_initial + Radial_Coordinate_initial * Pi / 180;

            if (i == Sector_number)
                break;

            Radial_Coordinate_initial = Radial_Coordinate_final;
        }

        Double_t Centre_X = (Radial_Coordinate_initial * TMath::Sin(Theta_Coordinate_initial * 3.14159 / 180.0) + Radial_Coordinate_initial * TMath::Sin(Theta_Coordinate_final * 3.14159 / 180.0));
        Centre_X += (Radial_Coordinate_initial * TMath::Sin(Theta_Coordinate_initial * 3.14159 / 180.0) + Radial_Coordinate_initial * TMath::Sin(Theta_Coordinate_final * 3.14159 / 180.0));
        return -X_offset - Centre_X / 4.0;
    }
}

Double_t GetY(Int_t Column_number, Int_t Sector_number, Bool_t Wall_Near_Smaller_Pads)
{
    if (Wall_Near_Smaller_Pads)
    {
        Float_t Radial_Coordinate_initial = 18.30, Radial_Coordinate_final = 18.3, Theta_Coordinate_initial, Theta_Coordinate_final;
        Theta_Coordinate_initial = ((Float_t)22 - Column_number) - 90.0 - 23.0 / 2.0;
        Theta_Coordinate_final = (Float_t)(22 - (Column_number - 1)) - 90.0 - 23.0 / 2.0;
        Float_t Pi = 3.1468;
        Float_t X_offset = -19.4103 + 60.0, Y_offset = 75.1912 + 85.0;

        for (Int_t i = 0; i < 97; i++)
        {
            Radial_Coordinate_final = Radial_Coordinate_initial + Radial_Coordinate_initial * Pi / 180;

            if (i == Sector_number)
                break;

            Radial_Coordinate_initial = Radial_Coordinate_final;
        }

        Double_t Centre_Y = (Radial_Coordinate_initial * TMath::Cos(Theta_Coordinate_initial * 3.14159 / 180.0) + Radial_Coordinate_initial * TMath::Cos(Theta_Coordinate_final * 3.14159 / 180.0));
        Centre_Y += (Radial_Coordinate_initial * TMath::Cos(Theta_Coordinate_initial * 3.14159 / 180.0) + Radial_Coordinate_initial * TMath::Cos(Theta_Coordinate_final * 3.14159 / 180.0));
        return Y_offset - Centre_Y / 4.0;
    }
    else
    {
        Float_t Radial_Coordinate_initial = 18.30, Radial_Coordinate_final = 18.3, Theta_Coordinate_initial, Theta_Coordinate_final;
        Theta_Coordinate_initial = ((Float_t)22 - Column_number) + 90.0 - 23.0 / 2.0;
        Theta_Coordinate_final = (Float_t)(22 - (Column_number - 1)) + 90.0 - 23.0 / 2.0;
        Float_t Pi = 3.1468;
        Float_t X_offset = -19.4103 - 110.0 + 60.0, Y_offset = 75.1912 + 85.0;

        for (Int_t i = 0; i < 97; i++)
        {
            Radial_Coordinate_final = Radial_Coordinate_initial + Radial_Coordinate_initial * Pi / 180;

            if (i == Sector_number)
                break;

            Radial_Coordinate_initial = Radial_Coordinate_final;
        }

        Double_t Centre_Y = (Radial_Coordinate_initial * TMath::Cos(Theta_Coordinate_initial * 3.14159 / 180.0) + Radial_Coordinate_initial * TMath::Cos(Theta_Coordinate_final * 3.14159 / 180.0));
        Centre_Y += (Radial_Coordinate_initial * TMath::Cos(Theta_Coordinate_initial * 3.14159 / 180.0) + Radial_Coordinate_initial * TMath::Cos(Theta_Coordinate_final * 3.14159 / 180.0));
        return Y_offset - Centre_Y / 4.0;
    }
}

Double_t Getdx(Int_t Column_number, Int_t Sector_number)
{
    Float_t Radial_Coordinate_initial = 18.30, Radial_Coordinate_final = 18.3, Theta_Coordinate_initial, Theta_Coordinate_final;
    Theta_Coordinate_initial = ((Float_t)22 - Column_number) - 90.0 - 23.0 / 2.0;
    Theta_Coordinate_final = (Float_t)(22 - (Column_number - 1)) - 90.0 - 23.0 / 2.0;
    Float_t Pi = 3.1468;
    Float_t X_offset = -19.4103 + 60.0, Y_offset = 75.1912 + 85.0;

    for (Int_t i = 0; i < 97; i++)
    {
        Radial_Coordinate_final = Radial_Coordinate_initial + Radial_Coordinate_initial * Pi / 180;

        if (i == Sector_number)
            break;

        Radial_Coordinate_initial = Radial_Coordinate_final;
    }

    return Radial_Coordinate_final - Radial_Coordinate_initial;
}

Double_t Getdy(Int_t Column_number, Int_t Sector_number)
{
    Float_t Radial_Coordinate_initial = 18.30, Radial_Coordinate_final = 18.3, Theta_Coordinate_initial, Theta_Coordinate_final;
    Theta_Coordinate_initial = ((Float_t)22 - Column_number) - 90.0 - 23.0 / 2.0;
    Theta_Coordinate_final = (Float_t)(22 - (Column_number - 1)) - 90.0 - 23.0 / 2.0;
    Float_t Pi = 3.1468;
    Float_t X_offset = -19.4103 + 60.0, Y_offset = 75.1912 + 85.0;

    for (Int_t i = 0; i < 97; i++)
    {
        Radial_Coordinate_final = Radial_Coordinate_initial + Radial_Coordinate_initial * Pi / 180;

        if (i == Sector_number)
            break;

        Radial_Coordinate_initial = Radial_Coordinate_final;
    }

    return Radial_Coordinate_final - Radial_Coordinate_initial;
}
