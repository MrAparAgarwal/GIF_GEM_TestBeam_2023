// Copyright (C) Experimental High Energy Physics and Applications Group, VECC, Kolkata
// Author(s): Apar Agarwal

// This macro has been created for the purpose
// analysing various aspects of Digi Distributions
// of GEM for GIF++ 2023 data

// This Code requires the CbmRoot version DEC 21 or higher
// Recommended on Linux 20.04 --> Fairsoftversion : APR21p2 and FAIRROOTVERSION: v18.6.7 or higher

// Code first tested on 9th October, 2023

void Highlight_RPC_Pad(Int_t Column_number, Int_t Sector_number, TH2F*& PadXandY);
void Highlight_Pad(Int_t Column_number, Int_t Sector_number, Int_t Weight, TH2F*& PadXandY, Bool_t Wall_Near_Smaller_Pads);

void Boundary_Plotter_GEM(Int_t Column_number, Int_t Sector_number, TH2F*& PadXandY, Bool_t left, Bool_t right, Bool_t top, Bool_t bottom, Bool_t Wall_Near_Smaller_Pads);
void Draw_Arrow_Left_To_Right(Double_t X1, Double_t X2, Double_t Y, TH2F*& PadXandY);
void Draw_Arrow_Top_To_Bottom(Double_t X, Double_t Y1, Double_t Y2, TH2F*& PadXandY);
void Draw_Feb(Double_t PosX, Double_t PosY, Int_t Feb_no, TH2F*& PadXandY);
void Write_Wall(Bool_t Wall_Is_On_The_Left, Bool_t Wall_Is_On_The_Right, TH2F*& PadXandY);

void GIF_GEM_Without_Trigger_Digi_Distributions_Acceptance_Corrected(Int_t RunId = 966,
                                                                     Bool_t Wall_Near_Smaller_Pads = false)
{
    TStopwatch Code_Timer;
    Code_Timer.Start();

    TString InputFileName = "Run" + to_string(RunId) + ".digi.root";
    TString GEM_Noise_File_Name = "Run_" + to_string(RunId) + "_GEM_Noise.txt";
    TString Three_Fold_Acceptance_File_Name = "Three_Fold_Area_Config_4.txt";

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

    TString OutputFileName = "data/Digi_Distributions/Run" + to_string(RunId) + "_GEM_Without_Trigger_Digi_Distribution.root";
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

    HistTag = "GEM_Feb_Boundaries_and_Labels";
    HistTitle = "GEM Boundaries and Labels";
    TH2F* GEM_Feb_Boundaries_and_Labels = new TH2F("GEM_Feb_Boundaries_and_Labels", "", 3640, -50, 90, 3280, 125, 195);
    GEM_Feb_Boundaries_and_Labels->GetXaxis()->SetTitle("X Coordinate (cm)");
    GEM_Feb_Boundaries_and_Labels->GetYaxis()->SetTitle("Y Coordinate (cm)");

    HistTag = "GEM_Without_Trigger_Column_Sector_Wise_1D_X_Raw_No_ADC_Cut";
    HistTitle = "GEM-1D Raw Digi distribution (No ADC Cut) column number wise for Run " + to_string(RunId);
    TH1D* GEM_Without_Trigger_Column_Sector_Wise_1D_X_Raw_No_ADC_Cut = new TH1D(HistTag, HistTitle, 26, -1.5, 24.5);
    GEM_Without_Trigger_Column_Sector_Wise_1D_X_Raw_No_ADC_Cut->GetXaxis()->SetTitle("Column number");
    GEM_Without_Trigger_Column_Sector_Wise_1D_X_Raw_No_ADC_Cut->GetYaxis()->SetTitle("Counts");

    TH1D* GEM_Without_Trigger_Column_Sector_Wise_1D_X_Raw_With_ADC_Cut_Single[31];

    for (Int_t iADC = 1; iADC < 32; iADC++)
    {
        HistTag = "GEM_Without_Trigger_Column_Sector_Wise_1D_X_Raw_With_ADC_Cut_Single_" + to_string(iADC);
        HistTitle = "GEM-1D Raw Digi distribution (Only ADC= " + to_string(iADC) + ") column number wise for Run " + to_string(RunId);
        GEM_Without_Trigger_Column_Sector_Wise_1D_X_Raw_With_ADC_Cut_Single[iADC - 1] = new TH1D(HistTag, HistTitle, 26, -1.5, 24.5);
        GEM_Without_Trigger_Column_Sector_Wise_1D_X_Raw_With_ADC_Cut_Single[iADC - 1]->GetXaxis()->SetTitle("Column number");
        GEM_Without_Trigger_Column_Sector_Wise_1D_X_Raw_With_ADC_Cut_Single[iADC - 1]->GetYaxis()->SetTitle("Counts");
    }

    TH1D* GEM_Without_Trigger_Column_Sector_Wise_1D_X_Raw_With_ADC_Cut_Single_Masked[31];

    for (Int_t iADC = 1; iADC < 32; iADC++)
    {
        HistTag = "GEM_Without_Trigger_Column_Sector_Wise_1D_X_Raw_With_ADC_Cut_Single_Masked_" + to_string(iADC);
        HistTitle = "GEM-1D Raw Digi distribution (Only ADC= " + to_string(iADC) + " Masked) column number wise for Run " + to_string(RunId);
        GEM_Without_Trigger_Column_Sector_Wise_1D_X_Raw_With_ADC_Cut_Single_Masked[iADC - 1] = new TH1D(HistTag, HistTitle, 26, -1.5, 24.5);
        GEM_Without_Trigger_Column_Sector_Wise_1D_X_Raw_With_ADC_Cut_Single_Masked[iADC - 1]->GetXaxis()->SetTitle("Column number");
        GEM_Without_Trigger_Column_Sector_Wise_1D_X_Raw_With_ADC_Cut_Single_Masked[iADC - 1]->GetYaxis()->SetTitle("Counts");
    }

    TH1D* GEM_Without_Trigger_Column_Sector_Wise_1D_X_Raw_With_ADC_Cut_Cumulative[30];

    for (Int_t iADC = 1; iADC < 31; iADC++)
    {
        HistTag = "GEM_Without_Trigger_Column_Sector_Wise_1D_X_Raw_With_ADC_Cut_Cumulative_" + to_string(iADC);
        HistTitle = "GEM-1D Raw Digi distribution (ADC<= " + to_string(iADC) + " Masked) column number wise for Run " + to_string(RunId);
        GEM_Without_Trigger_Column_Sector_Wise_1D_X_Raw_With_ADC_Cut_Cumulative[iADC - 1] = new TH1D(HistTag, HistTitle, 26, -1.5, 24.5);
        GEM_Without_Trigger_Column_Sector_Wise_1D_X_Raw_With_ADC_Cut_Cumulative[iADC - 1]->GetXaxis()->SetTitle("Column number");
        GEM_Without_Trigger_Column_Sector_Wise_1D_X_Raw_With_ADC_Cut_Cumulative[iADC - 1]->GetYaxis()->SetTitle("Counts");
    }


    HistTag = "GEM_Without_Trigger_Column_Sector_Wise_1D_X_Noise_Masked_No_ADC_Cut";
    HistTitle = "GEM-1D Noise Masked Digi distribution (No ADC Cut) column number wise for Run " + to_string(RunId);
    TH1D* GEM_Without_Trigger_Column_Sector_Wise_1D_X_Noise_Masked_No_ADC_Cut = new TH1D(HistTag, HistTitle, 26, -1.5, 24.5);
    GEM_Without_Trigger_Column_Sector_Wise_1D_X_Noise_Masked_No_ADC_Cut->GetXaxis()->SetTitle("Column number");
    GEM_Without_Trigger_Column_Sector_Wise_1D_X_Noise_Masked_No_ADC_Cut->GetYaxis()->SetTitle("Counts");

    TH1D* GEM_Without_Trigger_Column_Sector_Wise_1D_X_Noise_Masked_With_ADC_Cut_Single[31];

    for (Int_t iADC = 1; iADC < 32; iADC++)
    {
        HistTag = "GEM_Without_Trigger_Column_Sector_Wise_1D_X_Noise_Masked_With_ADC_Cut_Single_" + to_string(iADC);
        HistTitle = "GEM-1D Noise Masked Digi distribution (Only ADC= " + to_string(iADC) + ") column number wise for Run " + to_string(RunId);
        GEM_Without_Trigger_Column_Sector_Wise_1D_X_Noise_Masked_With_ADC_Cut_Single[iADC - 1] = new TH1D(HistTag, HistTitle, 26, -1.5, 24.5);
        GEM_Without_Trigger_Column_Sector_Wise_1D_X_Noise_Masked_With_ADC_Cut_Single[iADC - 1]->GetXaxis()->SetTitle("Column number");
        GEM_Without_Trigger_Column_Sector_Wise_1D_X_Noise_Masked_With_ADC_Cut_Single[iADC - 1]->GetYaxis()->SetTitle("Counts");
    }

    TH1D* GEM_Without_Trigger_Column_Sector_Wise_1D_X_Noise_Masked_With_ADC_Cut_Single_Masked[31];

    for (Int_t iADC = 1; iADC < 32; iADC++)
    {
        HistTag = "GEM_Without_Trigger_Column_Sector_Wise_1D_X_Noise_Masked_With_ADC_Cut_Single_Masked_" + to_string(iADC);
        HistTitle = "GEM-1D Noise Masked Digi distribution (Only ADC= " + to_string(iADC) + " Masked) column number wise for Run " + to_string(RunId);
        GEM_Without_Trigger_Column_Sector_Wise_1D_X_Noise_Masked_With_ADC_Cut_Single_Masked[iADC - 1] = new TH1D(HistTag, HistTitle, 26, -1.5, 24.5);
        GEM_Without_Trigger_Column_Sector_Wise_1D_X_Noise_Masked_With_ADC_Cut_Single_Masked[iADC - 1]->GetXaxis()->SetTitle("Column number");
        GEM_Without_Trigger_Column_Sector_Wise_1D_X_Noise_Masked_With_ADC_Cut_Single_Masked[iADC - 1]->GetYaxis()->SetTitle("Counts");
    }

    TH1D* GEM_Without_Trigger_Column_Sector_Wise_1D_X_Noise_Masked_With_ADC_Cut_Cumulative[30];

    for (Int_t iADC = 1; iADC < 31; iADC++)
    {
        HistTag = "GEM_Without_Trigger_Column_Sector_Wise_1D_X_Noise_Masked_With_ADC_Cut_Cumulative_" + to_string(iADC);
        HistTitle = "GEM-1D Noise Masked Digi distribution (ADC<= " + to_string(iADC) + " Masked) column number wise for Run " + to_string(RunId);
        GEM_Without_Trigger_Column_Sector_Wise_1D_X_Noise_Masked_With_ADC_Cut_Cumulative[iADC - 1] = new TH1D(HistTag, HistTitle, 26, -1.5, 24.5);
        GEM_Without_Trigger_Column_Sector_Wise_1D_X_Noise_Masked_With_ADC_Cut_Cumulative[iADC - 1]->GetXaxis()->SetTitle("Column number");
        GEM_Without_Trigger_Column_Sector_Wise_1D_X_Noise_Masked_With_ADC_Cut_Cumulative[iADC - 1]->GetYaxis()->SetTitle("Counts");
    }


    HistTag = "GEM_Without_Trigger_Column_Sector_Wise_1D_X_Noise_Only_No_ADC_Cut";
    HistTitle = "GEM-1D Noise Only Digi distribution (No ADC Cut) column number wise for Run " + to_string(RunId);
    TH1D* GEM_Without_Trigger_Column_Sector_Wise_1D_X_Noise_Only_No_ADC_Cut = new TH1D(HistTag, HistTitle, 26, -1.5, 24.5);
    GEM_Without_Trigger_Column_Sector_Wise_1D_X_Noise_Only_No_ADC_Cut->GetXaxis()->SetTitle("Column number");
    GEM_Without_Trigger_Column_Sector_Wise_1D_X_Noise_Only_No_ADC_Cut->GetYaxis()->SetTitle("Counts");

    TH1D* GEM_Without_Trigger_Column_Sector_Wise_1D_X_Noise_Only_With_ADC_Cut_Single[31];

    for (Int_t iADC = 1; iADC < 32; iADC++)
    {
        HistTag = "GEM_Without_Trigger_Column_Sector_Wise_1D_X_Noise_Only_With_ADC_Cut_Single_" + to_string(iADC);
        HistTitle = "GEM-1D Noise Only Digi distribution (Only ADC= " + to_string(iADC) + ") column number wise for Run " + to_string(RunId);
        GEM_Without_Trigger_Column_Sector_Wise_1D_X_Noise_Only_With_ADC_Cut_Single[iADC - 1] = new TH1D(HistTag, HistTitle, 26, -1.5, 24.5);
        GEM_Without_Trigger_Column_Sector_Wise_1D_X_Noise_Only_With_ADC_Cut_Single[iADC - 1]->GetXaxis()->SetTitle("Column number");
        GEM_Without_Trigger_Column_Sector_Wise_1D_X_Noise_Only_With_ADC_Cut_Single[iADC - 1]->GetYaxis()->SetTitle("Counts");
    }

    TH1D* GEM_Without_Trigger_Column_Sector_Wise_1D_X_Noise_Only_With_ADC_Cut_Single_Masked[31];

    for (Int_t iADC = 1; iADC < 32; iADC++)
    {
        HistTag = "GEM_Without_Trigger_Column_Sector_Wise_1D_X_Noise_Only_With_ADC_Cut_Single_Masked_" + to_string(iADC);
        HistTitle = "GEM-1D Noise Only Digi distribution (Only ADC= " + to_string(iADC) + " Masked) column number wise for Run " + to_string(RunId);
        GEM_Without_Trigger_Column_Sector_Wise_1D_X_Noise_Only_With_ADC_Cut_Single_Masked[iADC - 1] = new TH1D(HistTag, HistTitle, 26, -1.5, 24.5);
        GEM_Without_Trigger_Column_Sector_Wise_1D_X_Noise_Only_With_ADC_Cut_Single_Masked[iADC - 1]->GetXaxis()->SetTitle("Column number");
        GEM_Without_Trigger_Column_Sector_Wise_1D_X_Noise_Only_With_ADC_Cut_Single_Masked[iADC - 1]->GetYaxis()->SetTitle("Counts");
    }

    TH1D* GEM_Without_Trigger_Column_Sector_Wise_1D_X_Noise_Only_With_ADC_Cut_Cumulative[30];

    for (Int_t iADC = 1; iADC < 31; iADC++)
    {
        HistTag = "GEM_Without_Trigger_Column_Sector_Wise_1D_X_Noise_Only_With_ADC_Cut_Cumulative_" + to_string(iADC);
        HistTitle = "GEM-1D Noise Only Digi distribution (ADC<= " + to_string(iADC) + " Masked) column number wise for Run " + to_string(RunId);
        GEM_Without_Trigger_Column_Sector_Wise_1D_X_Noise_Only_With_ADC_Cut_Cumulative[iADC - 1] = new TH1D(HistTag, HistTitle, 26, -1.5, 24.5);
        GEM_Without_Trigger_Column_Sector_Wise_1D_X_Noise_Only_With_ADC_Cut_Cumulative[iADC - 1]->GetXaxis()->SetTitle("Column number");
        GEM_Without_Trigger_Column_Sector_Wise_1D_X_Noise_Only_With_ADC_Cut_Cumulative[iADC - 1]->GetYaxis()->SetTitle("Counts");
    }


    HistTag = "GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Raw_No_ADC_Cut";
    HistTitle = "GEM-1D Raw Digi distribution (No ADC Cut) sector number wise for Run " + to_string(RunId);
    TH1D* GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Raw_No_ADC_Cut = new TH1D(HistTag, HistTitle, 100, -1.5, 98.5);
    GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Raw_No_ADC_Cut->GetXaxis()->SetTitle("Sector number");
    GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Raw_No_ADC_Cut->GetYaxis()->SetTitle("Counts");

    TH1D* GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Raw_With_ADC_Cut_Single[31];

    for (Int_t iADC = 1; iADC < 32; iADC++)
    {
        HistTag = "GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Raw_With_ADC_Cut_Single_" + to_string(iADC);
        HistTitle = "GEM-1D Raw Digi distribution (Only ADC= " + to_string(iADC) + ") sector number wise for Run " + to_string(RunId);
        GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Raw_With_ADC_Cut_Single[iADC - 1] = new TH1D(HistTag, HistTitle, 100, -1.5, 98.5);
        GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Raw_With_ADC_Cut_Single[iADC - 1]->GetXaxis()->SetTitle("Sector number");
        GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Raw_With_ADC_Cut_Single[iADC - 1]->GetYaxis()->SetTitle("Counts");
    }

    TH1D* GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Raw_With_ADC_Cut_Single_Masked[31];

    for (Int_t iADC = 1; iADC < 32; iADC++)
    {
        HistTag = "GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Raw_With_ADC_Cut_Single_Masked_" + to_string(iADC);
        HistTitle = "GEM-1D Raw Digi distribution (Only ADC= " + to_string(iADC) + " Masked) sector number wise for Run " + to_string(RunId);
        GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Raw_With_ADC_Cut_Single_Masked[iADC - 1] = new TH1D(HistTag, HistTitle, 100, -1.5, 98.5);
        GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Raw_With_ADC_Cut_Single_Masked[iADC - 1]->GetXaxis()->SetTitle("Sector number");
        GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Raw_With_ADC_Cut_Single_Masked[iADC - 1]->GetYaxis()->SetTitle("Counts");
    }

    TH1D* GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Raw_With_ADC_Cut_Cumulative[30];

    for (Int_t iADC = 1; iADC < 31; iADC++)
    {
        HistTag = "GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Raw_With_ADC_Cut_Cumulative_" + to_string(iADC);
        HistTitle = "GEM-1D Raw Digi distribution (ADC<= " + to_string(iADC) + " Masked) sector number wise for Run " + to_string(RunId);
        GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Raw_With_ADC_Cut_Cumulative[iADC - 1] = new TH1D(HistTag, HistTitle, 100, -1.5, 98.5);
        GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Raw_With_ADC_Cut_Cumulative[iADC - 1]->GetXaxis()->SetTitle("Sector number");
        GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Raw_With_ADC_Cut_Cumulative[iADC - 1]->GetYaxis()->SetTitle("Counts");
    }


    HistTag = "GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Noise_Masked_No_ADC_Cut";
    HistTitle = "GEM-1D Noise Masked Digi distribution (No ADC Cut) sector number wise for Run " + to_string(RunId);
    TH1D* GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Noise_Masked_No_ADC_Cut = new TH1D(HistTag, HistTitle, 100, -1.5, 98.5);
    GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Noise_Masked_No_ADC_Cut->GetXaxis()->SetTitle("Sector number");
    GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Noise_Masked_No_ADC_Cut->GetYaxis()->SetTitle("Counts");

    TH1D* GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Noise_Masked_With_ADC_Cut_Single[31];

    for (Int_t iADC = 1; iADC < 32; iADC++)
    {
        HistTag = "GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Noise_Masked_With_ADC_Cut_Single_" + to_string(iADC);
        HistTitle = "GEM-1D Noise Masked Digi distribution (Only ADC= " + to_string(iADC) + ") sector number wise for Run " + to_string(RunId);
        GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Noise_Masked_With_ADC_Cut_Single[iADC - 1] = new TH1D(HistTag, HistTitle, 100, -1.5, 98.5);
        GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Noise_Masked_With_ADC_Cut_Single[iADC - 1]->GetXaxis()->SetTitle("Sector number");
        GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Noise_Masked_With_ADC_Cut_Single[iADC - 1]->GetYaxis()->SetTitle("Counts");
    }

    TH1D* GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Noise_Masked_With_ADC_Cut_Single_Masked[31];

    for (Int_t iADC = 1; iADC < 32; iADC++)
    {
        HistTag = "GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Noise_Masked_With_ADC_Cut_Single_Masked_" + to_string(iADC);
        HistTitle = "GEM-1D Noise Masked Digi distribution (Only ADC= " + to_string(iADC) + " Masked) sector number wise for Run " + to_string(RunId);
        GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Noise_Masked_With_ADC_Cut_Single_Masked[iADC - 1] = new TH1D(HistTag, HistTitle, 100, -1.5, 98.5);
        GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Noise_Masked_With_ADC_Cut_Single_Masked[iADC - 1]->GetXaxis()->SetTitle("Sector number");
        GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Noise_Masked_With_ADC_Cut_Single_Masked[iADC - 1]->GetYaxis()->SetTitle("Counts");
    }

    TH1D* GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Noise_Masked_With_ADC_Cut_Cumulative[30];

    for (Int_t iADC = 1; iADC < 31; iADC++)
    {
        HistTag = "GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Noise_Masked_With_ADC_Cut_Cumulative_" + to_string(iADC);
        HistTitle = "GEM-1D Noise Masked Digi distribution (ADC<= " + to_string(iADC) + " Masked) sector number wise for Run " + to_string(RunId);
        GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Noise_Masked_With_ADC_Cut_Cumulative[iADC - 1] = new TH1D(HistTag, HistTitle, 100, -1.5, 98.5);
        GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Noise_Masked_With_ADC_Cut_Cumulative[iADC - 1]->GetXaxis()->SetTitle("Sector number");
        GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Noise_Masked_With_ADC_Cut_Cumulative[iADC - 1]->GetYaxis()->SetTitle("Counts");
    }


    HistTag = "GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Noise_Only_No_ADC_Cut";
    HistTitle = "GEM-1D Noise Only Digi distribution (No ADC Cut) sector number wise for Run " + to_string(RunId);
    TH1D* GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Noise_Only_No_ADC_Cut = new TH1D(HistTag, HistTitle, 100, -1.5, 98.5);
    GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Noise_Only_No_ADC_Cut->GetXaxis()->SetTitle("Sector number");
    GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Noise_Only_No_ADC_Cut->GetYaxis()->SetTitle("Counts");

    TH1D* GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Noise_Only_With_ADC_Cut_Single[31];

    for (Int_t iADC = 1; iADC < 32; iADC++)
    {
        HistTag = "GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Noise_Only_With_ADC_Cut_Single_" + to_string(iADC);
        HistTitle = "GEM-1D Noise Only Digi distribution (Only ADC= " + to_string(iADC) + ") sector number wise for Run " + to_string(RunId);
        GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Noise_Only_With_ADC_Cut_Single[iADC - 1] = new TH1D(HistTag, HistTitle, 100, -1.5, 98.5);
        GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Noise_Only_With_ADC_Cut_Single[iADC - 1]->GetXaxis()->SetTitle("Sector number");
        GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Noise_Only_With_ADC_Cut_Single[iADC - 1]->GetYaxis()->SetTitle("Counts");
    }

    TH1D* GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Noise_Only_With_ADC_Cut_Single_Masked[31];

    for (Int_t iADC = 1; iADC < 32; iADC++)
    {
        HistTag = "GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Noise_Only_With_ADC_Cut_Single_Masked_" + to_string(iADC);
        HistTitle = "GEM-1D Noise Only Digi distribution (Only ADC= " + to_string(iADC) + " Masked) sector number wise for Run " + to_string(RunId);
        GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Noise_Only_With_ADC_Cut_Single_Masked[iADC - 1] = new TH1D(HistTag, HistTitle, 100, -1.5, 98.5);
        GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Noise_Only_With_ADC_Cut_Single_Masked[iADC - 1]->GetXaxis()->SetTitle("Sector number");
        GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Noise_Only_With_ADC_Cut_Single_Masked[iADC - 1]->GetYaxis()->SetTitle("Counts");
    }

    TH1D* GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Noise_Only_With_ADC_Cut_Cumulative[30];

    for (Int_t iADC = 1; iADC < 31; iADC++)
    {
        HistTag = "GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Noise_Only_With_ADC_Cut_Cumulative_" + to_string(iADC);
        HistTitle = "GEM-1D Noise Only Digi distribution (ADC<= " + to_string(iADC) + " Masked) sector number wise for Run " + to_string(RunId);
        GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Noise_Only_With_ADC_Cut_Cumulative[iADC - 1] = new TH1D(HistTag, HistTitle, 100, -1.5, 98.5);
        GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Noise_Only_With_ADC_Cut_Cumulative[iADC - 1]->GetXaxis()->SetTitle("Sector number");
        GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Noise_Only_With_ADC_Cut_Cumulative[iADC - 1]->GetYaxis()->SetTitle("Counts");
    }


    HistTag = "GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Raw_No_ADC_Cut";
    HistTitle = "GEM-2D Column Sector wise Raw Digi distribution (No ADC Cut) for Run " + to_string(RunId);
    TH2F* GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Raw_No_ADC_Cut = new TH2F(HistTag, HistTitle, 100, -1.5, 98.5, 26, -1.5, 24.5);
    GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Raw_No_ADC_Cut->GetXaxis()->SetTitle("Sector number");
    GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Raw_No_ADC_Cut->GetYaxis()->SetTitle("Column number");

    TH2F* GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Raw_With_ADC_Cut_Single[31];

    for (Int_t iADC = 1; iADC < 32; iADC++)
    {
        HistTag = "GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Raw_With_ADC_Cut_Single_" + to_string(iADC);
        HistTitle = "GEM-2D Column Sector wise Raw Digi distribution (Only ADC= " + to_string(iADC) + ") for Run " + to_string(RunId);
        GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Raw_With_ADC_Cut_Single[iADC - 1] = new TH2F(HistTag, HistTitle, 100, -1.5, 98.5, 26, -1.5, 24.5);
        GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Raw_With_ADC_Cut_Single[iADC - 1]->GetXaxis()->SetTitle("Sector number");
        GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Raw_With_ADC_Cut_Single[iADC - 1]->GetYaxis()->SetTitle("Column number");
    }

    TH2F* GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Raw_With_ADC_Cut_Single_Masked[31];

    for (Int_t iADC = 1; iADC < 32; iADC++)
    {
        HistTag = "GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Raw_With_ADC_Cut_Single_Masked_" + to_string(iADC);
        HistTitle = "GEM-2D Column Sector wise Raw Digi distribution (Only ADC= " + to_string(iADC) + " Masked) for Run " + to_string(RunId);
        GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Raw_With_ADC_Cut_Single_Masked[iADC - 1] = new TH2F(HistTag, HistTitle, 100, -1.5, 98.5, 26, -1.5, 24.5);
        GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Raw_With_ADC_Cut_Single_Masked[iADC - 1]->GetXaxis()->SetTitle("Sector number");
        GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Raw_With_ADC_Cut_Single_Masked[iADC - 1]->GetYaxis()->SetTitle("Column number");
    }

    TH2F* GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Raw_With_ADC_Cut_Cumulative[30];

    for (Int_t iADC = 1; iADC < 31; iADC++)
    {
        HistTag = "GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Raw_With_ADC_Cut_Cumulative_" + to_string(iADC);
        HistTitle = "GEM-2D Column Sector wise Raw Digi distribution (ADC<= " + to_string(iADC) + " Masked) for Run " + to_string(RunId);
        GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Raw_With_ADC_Cut_Cumulative[iADC - 1] = new TH2F(HistTag, HistTitle, 100, -1.5, 98.5, 26, -1.5, 24.5);
        GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Raw_With_ADC_Cut_Cumulative[iADC - 1]->GetXaxis()->SetTitle("Sector number");
        GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Raw_With_ADC_Cut_Cumulative[iADC - 1]->GetYaxis()->SetTitle("Column number");
    }


    HistTag = "GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Noise_Masked_No_ADC_Cut";
    HistTitle = "GEM-2D Column Sector wise Noise Masked Digi distribution (No ADC Cut) for Run " + to_string(RunId);
    TH2F* GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Noise_Masked_No_ADC_Cut = new TH2F(HistTag, HistTitle, 100, -1.5, 98.5, 26, -1.5, 24.5);
    GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Noise_Masked_No_ADC_Cut->GetXaxis()->SetTitle("Sector number");
    GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Noise_Masked_No_ADC_Cut->GetYaxis()->SetTitle("Column number");

    TH2F* GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Noise_Masked_With_ADC_Cut_Single[31];

    for (Int_t iADC = 1; iADC < 32; iADC++)
    {
        HistTag = "GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Noise_Masked_With_ADC_Cut_Single_" + to_string(iADC);
        HistTitle = "GEM-2D Column Sector wise Noise Masked Digi distribution (Only ADC= " + to_string(iADC) + ") for Run " + to_string(RunId);
        GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Noise_Masked_With_ADC_Cut_Single[iADC - 1] = new TH2F(HistTag, HistTitle, 100, -1.5, 98.5, 26, -1.5, 24.5);
        GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Noise_Masked_With_ADC_Cut_Single[iADC - 1]->GetXaxis()->SetTitle("Sector number");
        GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Noise_Masked_With_ADC_Cut_Single[iADC - 1]->GetYaxis()->SetTitle("Column number");
    }

    TH2F* GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Noise_Masked_With_ADC_Cut_Single_Masked[31];

    for (Int_t iADC = 1; iADC < 32; iADC++)
    {
        HistTag = "GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Noise_Masked_With_ADC_Cut_Single_Masked_" + to_string(iADC);
        HistTitle = "GEM-2D Column Sector wise Noise Masked Digi distribution (Only ADC= " + to_string(iADC) + " Masked) for Run " + to_string(RunId);
        GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Noise_Masked_With_ADC_Cut_Single_Masked[iADC - 1] = new TH2F(HistTag, HistTitle, 100, -1.5, 98.5, 26, -1.5, 24.5);
        GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Noise_Masked_With_ADC_Cut_Single_Masked[iADC - 1]->GetXaxis()->SetTitle("Sector number");
        GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Noise_Masked_With_ADC_Cut_Single_Masked[iADC - 1]->GetYaxis()->SetTitle("Column number");
    }

    TH2F* GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Noise_Masked_With_ADC_Cut_Cumulative[30];

    for (Int_t iADC = 1; iADC < 31; iADC++)
    {
        HistTag = "GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Noise_Masked_With_ADC_Cut_Cumulative_" + to_string(iADC);
        HistTitle = "GEM-2D Column Sector wise Noise Masked Digi distribution (ADC<= " + to_string(iADC) + " Masked) for Run " + to_string(RunId);
        GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Noise_Masked_With_ADC_Cut_Cumulative[iADC - 1] = new TH2F(HistTag, HistTitle, 100, -1.5, 98.5, 26, -1.5, 24.5);
        GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Noise_Masked_With_ADC_Cut_Cumulative[iADC - 1]->GetXaxis()->SetTitle("Sector number");
        GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Noise_Masked_With_ADC_Cut_Cumulative[iADC - 1]->GetYaxis()->SetTitle("Column number");
    }


    HistTag = "GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Noise_Only_No_ADC_Cut";
    HistTitle = "GEM-2D Column Sector wise Noise Only Digi distribution (No ADC Cut) for Run " + to_string(RunId);
    TH2F* GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Noise_Only_No_ADC_Cut = new TH2F(HistTag, HistTitle, 100, -1.5, 98.5, 26, -1.5, 24.5);
    GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Noise_Only_No_ADC_Cut->GetXaxis()->SetTitle("Sector number");
    GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Noise_Only_No_ADC_Cut->GetYaxis()->SetTitle("Column number");

    TH2F* GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Noise_Only_With_ADC_Cut_Single[31];

    for (Int_t iADC = 1; iADC < 32; iADC++)
    {
        HistTag = "GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Noise_Only_With_ADC_Cut_Single_" + to_string(iADC);
        HistTitle = "GEM-2D Column Sector wise Noise Only Digi distribution (Only ADC= " + to_string(iADC) + ") for Run " + to_string(RunId);
        GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Noise_Only_With_ADC_Cut_Single[iADC - 1] = new TH2F(HistTag, HistTitle, 100, -1.5, 98.5, 26, -1.5, 24.5);
        GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Noise_Only_With_ADC_Cut_Single[iADC - 1]->GetXaxis()->SetTitle("Sector number");
        GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Noise_Only_With_ADC_Cut_Single[iADC - 1]->GetYaxis()->SetTitle("Column number");
    }

    TH2F* GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Noise_Only_With_ADC_Cut_Single_Masked[31];

    for (Int_t iADC = 1; iADC < 32; iADC++)
    {
        HistTag = "GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Noise_Only_With_ADC_Cut_Single_Masked_" + to_string(iADC);
        HistTitle = "GEM-2D Column Sector wise Noise Only Digi distribution (Only ADC= " + to_string(iADC) + " Masked) for Run " + to_string(RunId);
        GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Noise_Only_With_ADC_Cut_Single_Masked[iADC - 1] = new TH2F(HistTag, HistTitle, 100, -1.5, 98.5, 26, -1.5, 24.5);
        GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Noise_Only_With_ADC_Cut_Single_Masked[iADC - 1]->GetXaxis()->SetTitle("Sector number");
        GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Noise_Only_With_ADC_Cut_Single_Masked[iADC - 1]->GetYaxis()->SetTitle("Column number");
    }

    TH2F* GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Noise_Only_With_ADC_Cut_Cumulative[30];

    for (Int_t iADC = 1; iADC < 31; iADC++)
    {
        HistTag = "GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Noise_Only_With_ADC_Cut_Cumulative_" + to_string(iADC);
        HistTitle = "GEM-2D Column Sector wise Noise Only Digi distribution (ADC<= " + to_string(iADC) + " Masked) for Run " + to_string(RunId);
        GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Noise_Only_With_ADC_Cut_Cumulative[iADC - 1] = new TH2F(HistTag, HistTitle, 100, -1.5, 98.5, 26, -1.5, 24.5);
        GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Noise_Only_With_ADC_Cut_Cumulative[iADC - 1]->GetXaxis()->SetTitle("Sector number");
        GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Noise_Only_With_ADC_Cut_Cumulative[iADC - 1]->GetYaxis()->SetTitle("Column number");
    }


    HistTag = "GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Raw_No_ADC_Cut";
    HistTitle = "GEM-2D Raw Digi distribution (No ADC Cut) for Run " + to_string(RunId);
    TH2F* GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Raw_No_ADC_Cut = new TH2F(HistTag, HistTitle, 3640, -50, 90, 3280, 125, 195);
    GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Raw_No_ADC_Cut->GetXaxis()->SetTitle("X Coordinate (cm)");
    GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Raw_No_ADC_Cut->GetYaxis()->SetTitle("Y Coordinate (cm)");

    // TH2F* GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Raw_With_ADC_Cut_Single[31];

    // for(Int_t iADC=1;iADC<32;iADC++)
    // {
    //     HistTag = "GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Raw_With_ADC_Cut_Single_"+to_string(iADC);
    //     HistTitle = "GEM-2D Raw Digi distribution (Only ADC= "+ to_string(iADC) +") for Run "+to_string(RunId);
    //     GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Raw_With_ADC_Cut_Single[iADC-1] = new TH2F(HistTag,HistTitle,3640,-50,90,3280,125,195);
    //     GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Raw_With_ADC_Cut_Single[iADC-1]->GetXaxis()->SetTitle("X Coordinate (cm)");
    //     GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Raw_With_ADC_Cut_Single[iADC-1]->GetYaxis()->SetTitle("Y Coordinate (cm)");
    // }

    // TH2F* GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Raw_With_ADC_Cut_Single_Masked[31];

    // for(Int_t iADC=1;iADC<32;iADC++)
    // {
    //     HistTag = "GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Raw_With_ADC_Cut_Single_Masked_"+to_string(iADC);
    //     HistTitle = "GEM-2D Raw Digi distribution (Only ADC= "+ to_string(iADC) +" Masked) for Run "+to_string(RunId);
    //     GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Raw_With_ADC_Cut_Single_Masked[iADC-1] = new TH2F(HistTag,HistTitle,3640,-50,90,3280,125,195);
    //     GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Raw_With_ADC_Cut_Single_Masked[iADC-1]->GetXaxis()->SetTitle("X Coordinate (cm)");
    //     GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Raw_With_ADC_Cut_Single_Masked[iADC-1]->GetYaxis()->SetTitle("Y Coordinate (cm)");
    // }

    // TH2F* GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Raw_With_ADC_Cut_Cumulative[30];

    // for(Int_t iADC=1;iADC<31;iADC++)
    // {
    //     HistTag = "GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Raw_With_ADC_Cut_Cumulative_"+to_string(iADC);
    //     HistTitle = "GEM-2D Raw Digi distribution (ADC<= "+ to_string(iADC) +" Masked) for Run "+to_string(RunId);
    //     GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Raw_With_ADC_Cut_Cumulative[iADC-1] = new TH2F(HistTag,HistTitle,3640,-50,90,3280,125,195);
    //     GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Raw_With_ADC_Cut_Cumulative[iADC-1]->GetXaxis()->SetTitle("X Coordinate (cm)");
    //     GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Raw_With_ADC_Cut_Cumulative[iADC-1]->GetYaxis()->SetTitle("Y Coordinate (cm)");
    // }


    HistTag = "GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Noise_Masked_No_ADC_Cut";
    HistTitle = "GEM-2D Noise Masked Digi distribution (No ADC Cut) for Run " + to_string(RunId);
    TH2F* GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Noise_Masked_No_ADC_Cut = new TH2F(HistTag, HistTitle, 3640, -50, 90, 3280, 125, 195);
    GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Noise_Masked_No_ADC_Cut->GetXaxis()->SetTitle("X Coordinate (cm)");
    GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Noise_Masked_No_ADC_Cut->GetYaxis()->SetTitle("Y Coordinate (cm)");

    // TH2F* GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Noise_Masked_With_ADC_Cut_Single[31];

    // for(Int_t iADC=1;iADC<32;iADC++)
    // {
    //     HistTag = "GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Noise_Masked_With_ADC_Cut_Single_"+to_string(iADC);
    //     HistTitle = "GEM-2D Noise Masked Digi distribution (Only ADC= "+ to_string(iADC) +") for Run "+to_string(RunId);
    //     GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Noise_Masked_With_ADC_Cut_Single[iADC-1] = new TH2F(HistTag,HistTitle,3640,-50,90,3280,125,195);
    //     GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Noise_Masked_With_ADC_Cut_Single[iADC-1]->GetXaxis()->SetTitle("X Coordinate (cm)");
    //     GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Noise_Masked_With_ADC_Cut_Single[iADC-1]->GetYaxis()->SetTitle("Y Coordinate (cm)");
    // }

    // TH2F* GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Noise_Masked_With_ADC_Cut_Single_Masked[31];

    // for(Int_t iADC=1;iADC<32;iADC++)
    // {
    //     HistTag = "GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Noise_Masked_With_ADC_Cut_Single_Masked_"+to_string(iADC);
    //     HistTitle = "GEM-2D Noise Masked Digi distribution (Only ADC= "+ to_string(iADC) +" Masked) for Run "+to_string(RunId);
    //     GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Noise_Masked_With_ADC_Cut_Single_Masked[iADC-1] = new TH2F(HistTag,HistTitle,3640,-50,90,3280,125,195);
    //     GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Noise_Masked_With_ADC_Cut_Single_Masked[iADC-1]->GetXaxis()->SetTitle("X Coordinate (cm)");
    //     GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Noise_Masked_With_ADC_Cut_Single_Masked[iADC-1]->GetYaxis()->SetTitle("Y Coordinate (cm)");
    // }

    // TH2F* GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Noise_Masked_With_ADC_Cut_Cumulative[30];

    // for(Int_t iADC=1;iADC<31;iADC++)
    // {
    //     HistTag = "GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Noise_Masked_With_ADC_Cut_Cumulative_"+to_string(iADC);
    //     HistTitle = "GEM-2D Noise Masked Digi distribution (ADC<= "+ to_string(iADC) +" Masked) for Run "+to_string(RunId);
    //     GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Noise_Masked_With_ADC_Cut_Cumulative[iADC-1] = new TH2F(HistTag,HistTitle,3640,-50,90,3280,125,195);
    //     GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Noise_Masked_With_ADC_Cut_Cumulative[iADC-1]->GetXaxis()->SetTitle("X Coordinate (cm)");
    //     GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Noise_Masked_With_ADC_Cut_Cumulative[iADC-1]->GetYaxis()->SetTitle("Y Coordinate (cm)");
    // }


    HistTag = "GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Noise_Only_No_ADC_Cut";
    HistTitle = "GEM-2D Noise Only Digi distribution (No ADC Cut) for Run " + to_string(RunId);
    TH2F* GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Noise_Only_No_ADC_Cut = new TH2F(HistTag, HistTitle, 3640, -50, 90, 3280, 125, 195);
    GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Noise_Only_No_ADC_Cut->GetXaxis()->SetTitle("X Coordinate (cm)");
    GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Noise_Only_No_ADC_Cut->GetYaxis()->SetTitle("Y Coordinate (cm)");

    // TH2F* GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Noise_Only_With_ADC_Cut_Single[31];

    // for(Int_t iADC=1;iADC<32;iADC++)
    // {
    //     HistTag = "GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Noise_Only_With_ADC_Cut_Single_"+to_string(iADC);
    //     HistTitle = "GEM-2D Noise Only Digi distribution (Only ADC= "+ to_string(iADC) +") for Run "+to_string(RunId);
    //     GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Noise_Only_With_ADC_Cut_Single[iADC-1] = new TH2F(HistTag,HistTitle,3640,-50,90,3280,125,195);
    //     GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Noise_Only_With_ADC_Cut_Single[iADC-1]->GetXaxis()->SetTitle("X Coordinate (cm)");
    //     GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Noise_Only_With_ADC_Cut_Single[iADC-1]->GetYaxis()->SetTitle("Y Coordinate (cm)");
    // }

    // TH2F* GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Noise_Only_With_ADC_Cut_Single_Masked[31];

    // for(Int_t iADC=1;iADC<32;iADC++)
    // {
    //     HistTag = "GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Noise_Only_With_ADC_Cut_Single_Masked_"+to_string(iADC);
    //     HistTitle = "GEM-2D Noise Only Digi distribution (Only ADC= "+ to_string(iADC) +") for Run "+to_string(RunId);
    //     GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Noise_Only_With_ADC_Cut_Single_Masked[iADC-1] = new TH2F(HistTag,HistTitle,3640,-50,90,3280,125,195);
    //     GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Noise_Only_With_ADC_Cut_Single_Masked[iADC-1]->GetXaxis()->SetTitle("X Coordinate (cm)");
    //     GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Noise_Only_With_ADC_Cut_Single_Masked[iADC-1]->GetYaxis()->SetTitle("Y Coordinate (cm)");
    // }

    // TH2F* GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Noise_Only_With_ADC_Cut_Cumulative[30];

    // for(Int_t iADC=1;iADC<31;iADC++)
    // {
    //     HistTag = "GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Noise_Only_With_ADC_Cut_Cumulative_"+to_string(iADC);
    //     HistTitle = "GEM-2D Noise Only Digi distribution (ADC<= "+ to_string(iADC) +" Masked) for Run "+to_string(RunId);
    //     GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Noise_Only_With_ADC_Cut_Cumulative[iADC-1] = new TH2F(HistTag,HistTitle,3640,-50,90,3280,125,195);
    //     GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Noise_Only_With_ADC_Cut_Cumulative[iADC-1]->GetXaxis()->SetTitle("X Coordinate (cm)");
    //     GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Noise_Only_With_ADC_Cut_Cumulative[iADC-1]->GetYaxis()->SetTitle("Y Coordinate (cm)");
    // }


    TH1D* GEM_Without_Trigger_Feb_Channel_Wise_1D_Channel_Map_Raw_Feb[18];

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTag = "GEM_Without_Trigger_Feb_Channel_Wise_1D_Channel_Map_Raw_Feb_" + to_string(iFeb);
        HistTitle = "GEM-1D Raw Digi distribution (No ADC Cut) channel wise of Feb-" + to_string(iFeb) + " for Run " + to_string(RunId);
        GEM_Without_Trigger_Feb_Channel_Wise_1D_Channel_Map_Raw_Feb[iFeb] = new TH1D(HistTag, HistTitle, 131, -1.5, 129.5);
        GEM_Without_Trigger_Feb_Channel_Wise_1D_Channel_Map_Raw_Feb[iFeb]->GetXaxis()->SetTitle("Channel number");
        GEM_Without_Trigger_Feb_Channel_Wise_1D_Channel_Map_Raw_Feb[iFeb]->GetYaxis()->SetTitle("Counts");
    }


    TH1D* GEM_Without_Trigger_Feb_Channel_Wise_1D_Channel_Map_Noise_Masked_Feb[18];

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTag = "GEM_Without_Trigger_Feb_Channel_Wise_1D_Channel_Map_Noise_Masked_Feb_" + to_string(iFeb);
        HistTitle = "GEM-1D Noise Masked Digi distribution (No ADC Cut) channel wise of Feb-" + to_string(iFeb) + " for Run " + to_string(RunId);
        GEM_Without_Trigger_Feb_Channel_Wise_1D_Channel_Map_Noise_Masked_Feb[iFeb] = new TH1D(HistTag, HistTitle, 131, -1.5, 129.5);
        GEM_Without_Trigger_Feb_Channel_Wise_1D_Channel_Map_Noise_Masked_Feb[iFeb]->GetXaxis()->SetTitle("Channel number");
        GEM_Without_Trigger_Feb_Channel_Wise_1D_Channel_Map_Noise_Masked_Feb[iFeb]->GetYaxis()->SetTitle("Counts");
    }

    TH1D* GEM_Without_Trigger_Feb_Channel_Wise_1D_Channel_Map_Noise_Only_Feb[18];

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTag = "GEM_Without_Trigger_Feb_Channel_Wise_1D_Channel_Map_Noise_Only_Feb_" + to_string(iFeb);
        HistTitle = "GEM-1D Noise Only Digi distribution (No ADC Cut) channel wise of Feb-" + to_string(iFeb) + " for Run " + to_string(RunId);
        GEM_Without_Trigger_Feb_Channel_Wise_1D_Channel_Map_Noise_Only_Feb[iFeb] = new TH1D(HistTag, HistTitle, 131, -1.5, 129.5);
        GEM_Without_Trigger_Feb_Channel_Wise_1D_Channel_Map_Noise_Only_Feb[iFeb]->GetXaxis()->SetTitle("Channel number");
        GEM_Without_Trigger_Feb_Channel_Wise_1D_Channel_Map_Noise_Only_Feb[iFeb]->GetYaxis()->SetTitle("Counts");
    }


    HistTag = "GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Raw_No_ADC_Cut";
    HistTitle = "GEM-2D Feb Channel wise Raw Digi distribution (No ADC Cut) for Run " + to_string(RunId);
    TH2F* GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Raw_No_ADC_Cut = new TH2F(HistTag, HistTitle, 131, -1.5, 129.5, 21, -1.5, 19.5);
    GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Raw_No_ADC_Cut->GetXaxis()->SetTitle("Channel number");
    GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Raw_No_ADC_Cut->GetYaxis()->SetTitle("FEB number");

    TH2F* GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Raw_With_ADC_Cut_Single[31];

    for (Int_t iADC = 1; iADC < 32; iADC++)
    {
        HistTag = "GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Raw_With_ADC_Cut_Single_" + to_string(iADC);
        HistTitle = "GEM-2D Feb Channel wise Raw Digi distribution (Only ADC= " + to_string(iADC) + ") for Run " + to_string(RunId);
        GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Raw_With_ADC_Cut_Single[iADC - 1] = new TH2F(HistTag, HistTitle, 131, -1.5, 129.5, 21, -1.5, 19.5);
        GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Raw_With_ADC_Cut_Single[iADC - 1]->GetXaxis()->SetTitle("Channel number");
        GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Raw_With_ADC_Cut_Single[iADC - 1]->GetYaxis()->SetTitle("FEB number");
    }

    TH2F* GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Raw_With_ADC_Cut_Single_Masked[31];

    for (Int_t iADC = 1; iADC < 32; iADC++)
    {
        HistTag = "GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Raw_With_ADC_Cut_Single_Masked_" + to_string(iADC);
        HistTitle = "GEM-2D Feb Channel wise Raw Digi distribution (Only ADC= " + to_string(iADC) + " Masked) for Run " + to_string(RunId);
        GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Raw_With_ADC_Cut_Single_Masked[iADC - 1] = new TH2F(HistTag, HistTitle, 131, -1.5, 129.5, 21, -1.5, 19.5);
        GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Raw_With_ADC_Cut_Single_Masked[iADC - 1]->GetXaxis()->SetTitle("Channel number");
        GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Raw_With_ADC_Cut_Single_Masked[iADC - 1]->GetYaxis()->SetTitle("FEB number");
    }

    TH2F* GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Raw_With_ADC_Cut_Cumulative[30];

    for (Int_t iADC = 1; iADC < 31; iADC++)
    {
        HistTag = "GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Raw_With_ADC_Cut_Cumulative_" + to_string(iADC);
        HistTitle = "GEM-2D Feb Channel wise Raw Digi distribution (ADC<= " + to_string(iADC) + " Masked) for Run " + to_string(RunId);
        GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Raw_With_ADC_Cut_Cumulative[iADC - 1] = new TH2F(HistTag, HistTitle, 131, -1.5, 129.5, 21, -1.5, 19.5);
        GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Raw_With_ADC_Cut_Cumulative[iADC - 1]->GetXaxis()->SetTitle("Channel number");
        GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Raw_With_ADC_Cut_Cumulative[iADC - 1]->GetYaxis()->SetTitle("FEB number");
    }

    HistTag = "GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Noise_Masked_No_ADC_Cut";
    HistTitle = "GEM-2D Feb Channel wise Noise Masked Digi distribution (No ADC Cut) for Run " + to_string(RunId);
    TH2F* GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Noise_Masked_No_ADC_Cut = new TH2F(HistTag, HistTitle, 131, -1.5, 129.5, 21, -1.5, 19.5);
    GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Noise_Masked_No_ADC_Cut->GetXaxis()->SetTitle("Channel number");
    GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Noise_Masked_No_ADC_Cut->GetYaxis()->SetTitle("FEB number");

    TH2F* GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Noise_Masked_With_ADC_Cut_Single[31];

    for (Int_t iADC = 1; iADC < 32; iADC++)
    {
        HistTag = "GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Noise_Masked_With_ADC_Cut_Single_" + to_string(iADC);
        HistTitle = "GEM-2D Feb Channel wise Noise Masked Digi distribution (Only ADC= " + to_string(iADC) + ") for Run " + to_string(RunId);
        GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Noise_Masked_With_ADC_Cut_Single[iADC - 1] = new TH2F(HistTag, HistTitle, 131, -1.5, 129.5, 21, -1.5, 19.5);
        GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Noise_Masked_With_ADC_Cut_Single[iADC - 1]->GetXaxis()->SetTitle("Channel number");
        GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Noise_Masked_With_ADC_Cut_Single[iADC - 1]->GetYaxis()->SetTitle("FEB number");
    }

    TH2F* GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Noise_Masked_With_ADC_Cut_Single_Masked[31];

    for (Int_t iADC = 1; iADC < 32; iADC++)
    {
        HistTag = "GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Noise_Masked_With_ADC_Cut_Single_Masked_" + to_string(iADC);
        HistTitle = "GEM-2D Feb Channel wise Noise Masked Digi distribution (Only ADC= " + to_string(iADC) + " Masked) for Run " + to_string(RunId);
        GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Noise_Masked_With_ADC_Cut_Single_Masked[iADC - 1] = new TH2F(HistTag, HistTitle, 131, -1.5, 129.5, 21, -1.5, 19.5);
        GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Noise_Masked_With_ADC_Cut_Single_Masked[iADC - 1]->GetXaxis()->SetTitle("Channel number");
        GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Noise_Masked_With_ADC_Cut_Single_Masked[iADC - 1]->GetYaxis()->SetTitle("FEB number");
    }

    TH2F* GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Noise_Masked_With_ADC_Cut_Cumulative[30];

    for (Int_t iADC = 1; iADC < 31; iADC++)
    {
        HistTag = "GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Noise_Masked_With_ADC_Cut_Cumulative_" + to_string(iADC);
        HistTitle = "GEM-2D Feb Channel wise Noise Masked Digi distribution (ADC<= " + to_string(iADC) + " Masked) for Run " + to_string(RunId);
        GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Noise_Masked_With_ADC_Cut_Cumulative[iADC - 1] = new TH2F(HistTag, HistTitle, 131, -1.5, 129.5, 21, -1.5, 19.5);
        GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Noise_Masked_With_ADC_Cut_Cumulative[iADC - 1]->GetXaxis()->SetTitle("Channel number");
        GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Noise_Masked_With_ADC_Cut_Cumulative[iADC - 1]->GetYaxis()->SetTitle("FEB number");
    }


    HistTag = "GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Noise_Only_No_ADC_Cut";
    HistTitle = "GEM-2D Feb Channel wise Noise Only Digi distribution (No ADC Cut) for Run " + to_string(RunId);
    TH2F* GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Noise_Only_No_ADC_Cut = new TH2F(HistTag, HistTitle, 131, -1.5, 129.5, 21, -1.5, 19.5);
    GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Noise_Only_No_ADC_Cut->GetXaxis()->SetTitle("Channel number");
    GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Noise_Only_No_ADC_Cut->GetYaxis()->SetTitle("FEB number");

    TH2F* GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Noise_Only_With_ADC_Cut_Single[31];

    for (Int_t iADC = 1; iADC < 32; iADC++)
    {
        HistTag = "GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Noise_Only_With_ADC_Cut_Single_" + to_string(iADC);
        HistTitle = "GEM-2D Feb Channel wise Noise Only Digi distribution (Only ADC= " + to_string(iADC) + ") for Run " + to_string(RunId);
        GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Noise_Only_With_ADC_Cut_Single[iADC - 1] = new TH2F(HistTag, HistTitle, 131, -1.5, 129.5, 21, -1.5, 19.5);
        GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Noise_Only_With_ADC_Cut_Single[iADC - 1]->GetXaxis()->SetTitle("Channel number");
        GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Noise_Only_With_ADC_Cut_Single[iADC - 1]->GetYaxis()->SetTitle("FEB number");
    }

    TH2F* GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Noise_Only_With_ADC_Cut_Single_Masked[31];

    for (Int_t iADC = 1; iADC < 32; iADC++)
    {
        HistTag = "GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Noise_Only_With_ADC_Cut_Single_Masked_" + to_string(iADC);
        HistTitle = "GEM-2D Feb Channel wise Noise Only Digi distribution (Only ADC= " + to_string(iADC) + " Masked) for Run " + to_string(RunId);
        GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Noise_Only_With_ADC_Cut_Single_Masked[iADC - 1] = new TH2F(HistTag, HistTitle, 131, -1.5, 129.5, 21, -1.5, 19.5);
        GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Noise_Only_With_ADC_Cut_Single_Masked[iADC - 1]->GetXaxis()->SetTitle("Channel number");
        GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Noise_Only_With_ADC_Cut_Single_Masked[iADC - 1]->GetYaxis()->SetTitle("FEB number");
    }

    TH2F* GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Noise_Only_With_ADC_Cut_Cumulative[30];

    for (Int_t iADC = 1; iADC < 31; iADC++)
    {
        HistTag = "GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Noise_Only_With_ADC_Cut_Cumulative_" + to_string(iADC);
        HistTitle = "GEM-2D Feb Channel wise Noise Only Digi distribution (ADC<= " + to_string(iADC) + " Masked) for Run " + to_string(RunId);
        GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Noise_Only_With_ADC_Cut_Cumulative[iADC - 1] = new TH2F(HistTag, HistTitle, 131, -1.5, 129.5, 21, -1.5, 19.5);
        GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Noise_Only_With_ADC_Cut_Cumulative[iADC - 1]->GetXaxis()->SetTitle("Channel number");
        GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Noise_Only_With_ADC_Cut_Cumulative[iADC - 1]->GetYaxis()->SetTitle("FEB number");
    }


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

            Boundary_Plotter_GEM(iChannel, iSector, GEM_Feb_Boundaries_and_Labels, left, right, top, bottom, Wall_Near_Smaller_Pads);
        }
    }

    if (Wall_Near_Smaller_Pads)
    {
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

        Write_Wall(true, false, GEM_Feb_Boundaries_and_Labels);
    }
    else
    {
        Draw_Arrow_Left_To_Right(110.5, 114.0, 75.5, GEM_Feb_Boundaries_and_Labels);
        Draw_Feb(140.0 - 25.5, 74.0, 0, GEM_Feb_Boundaries_and_Labels);

        Draw_Arrow_Left_To_Right(108.0, 125.0, 73.5, GEM_Feb_Boundaries_and_Labels);
        Draw_Feb(140.0 - 14.0, 76.0, 1, GEM_Feb_Boundaries_and_Labels);

        Draw_Arrow_Left_To_Right(108.0, 125.0, 77.5, GEM_Feb_Boundaries_and_Labels);
        Draw_Feb(140.0 - 14.0, 72.0, 2, GEM_Feb_Boundaries_and_Labels);

        Draw_Arrow_Left_To_Right(103.0, 114.0, 71.5, GEM_Feb_Boundaries_and_Labels);
        Draw_Feb(140.0 - 25.5, 78.0, 3, GEM_Feb_Boundaries_and_Labels);

        Draw_Arrow_Top_To_Bottom(140.0 - 48.2, 89.0, 77.2, GEM_Feb_Boundaries_and_Labels);
        Draw_Arrow_Top_To_Bottom(140.0 - 46.0, 89.0, 75.6, GEM_Feb_Boundaries_and_Labels);
        Draw_Feb(140.0 - 49, 90.0, 4, GEM_Feb_Boundaries_and_Labels);

        Draw_Arrow_Top_To_Bottom(100.6, 72.2, 60.0, GEM_Feb_Boundaries_and_Labels);
        Draw_Feb(140.0 - 43.0, 55.0, 4, GEM_Feb_Boundaries_and_Labels);

        Draw_Arrow_Left_To_Right(103.0, 114.0, 79.5, GEM_Feb_Boundaries_and_Labels);
        Draw_Feb(140.0 - 25.5, 70.0, 5, GEM_Feb_Boundaries_and_Labels);

        Draw_Arrow_Top_To_Bottom(89.5, 99.0, 80.0, GEM_Feb_Boundaries_and_Labels);
        Draw_Feb(140.0 - 52.5, 100.0, 6, GEM_Feb_Boundaries_and_Labels);

        Draw_Arrow_Top_To_Bottom(85.0, 89.0, 77.0, GEM_Feb_Boundaries_and_Labels);
        Draw_Feb(140.0 - 59.0, 90.0, 7, GEM_Feb_Boundaries_and_Labels);

        Draw_Arrow_Top_To_Bottom(85.0, 69.6, 60.0, GEM_Feb_Boundaries_and_Labels);
        Draw_Arrow_Top_To_Bottom(87.0, 73.0, 60.0, GEM_Feb_Boundaries_and_Labels);
        Draw_Feb(140.0 - 58.0, 55.0, 8, GEM_Feb_Boundaries_and_Labels);

        Draw_Arrow_Top_To_Bottom(74.0, 89.0, 76.2, GEM_Feb_Boundaries_and_Labels);
        Draw_Feb(140.0 - 70.0, 90.0, 8, GEM_Feb_Boundaries_and_Labels);

        Draw_Arrow_Top_To_Bottom(94.0, 68.5, 50.0, GEM_Feb_Boundaries_and_Labels);
        Draw_Feb(140.0 - 50.0, 45.5, 9, GEM_Feb_Boundaries_and_Labels);

        Draw_Arrow_Top_To_Bottom(70.5, 72.0, 60.0, GEM_Feb_Boundaries_and_Labels);
        Draw_Arrow_Top_To_Bottom(72.5, 72.0, 60.0, GEM_Feb_Boundaries_and_Labels);
        Draw_Arrow_Top_To_Bottom(74.5, 72.2, 60.0, GEM_Feb_Boundaries_and_Labels);
        Draw_Feb(140.0 - 70.0, 55.0, 9, GEM_Feb_Boundaries_and_Labels);

        Draw_Arrow_Top_To_Bottom(79.0, 99.0, 83.5, GEM_Feb_Boundaries_and_Labels);
        Draw_Feb(140.0 - 66.5, 100, 10, GEM_Feb_Boundaries_and_Labels);

        Draw_Arrow_Top_To_Bottom(62.0, 91.0, 80.5, GEM_Feb_Boundaries_and_Labels);
        Draw_Feb(140.0 - 82.0, 92, 11, GEM_Feb_Boundaries_and_Labels);

        Draw_Arrow_Top_To_Bottom(52.0, 99.0, 77.0, GEM_Feb_Boundaries_and_Labels);
        Draw_Arrow_Top_To_Bottom(56.0, 99.0, 67.5, GEM_Feb_Boundaries_and_Labels);
        Draw_Feb(140.0 - 90.0, 100, 12, GEM_Feb_Boundaries_and_Labels);

        Draw_Arrow_Top_To_Bottom(79.0, 70.0, 50.0, GEM_Feb_Boundaries_and_Labels);
        Draw_Feb(140.0 - 63.5, 45.5, 13, GEM_Feb_Boundaries_and_Labels);

        Draw_Arrow_Top_To_Bottom(66.0, 72.5, 50.0, GEM_Feb_Boundaries_and_Labels);
        Draw_Feb(140.0 - 78.0, 45.5, 13, GEM_Feb_Boundaries_and_Labels);

        Draw_Arrow_Top_To_Bottom(60.0, 65.0, 53.5, GEM_Feb_Boundaries_and_Labels);
        Draw_Feb(140.0 - 85.0, 50, 14, GEM_Feb_Boundaries_and_Labels);

        Draw_Arrow_Left_To_Right(25.0, 34.0, 86.5, GEM_Feb_Boundaries_and_Labels);
        Draw_Feb(140.0 - 125.0, 60.0, 15, GEM_Feb_Boundaries_and_Labels);

        Draw_Arrow_Left_To_Right(25.0, 34.0, 74.5, GEM_Feb_Boundaries_and_Labels);
        Draw_Feb(140.0 - 125.0, 73.0, 16, GEM_Feb_Boundaries_and_Labels);

        Draw_Arrow_Left_To_Right(25.0, 34.0, 61.5, GEM_Feb_Boundaries_and_Labels);
        Draw_Feb(140.0 - 125.0, 85.0, 17, GEM_Feb_Boundaries_and_Labels);

        Write_Wall(true, false, GEM_Feb_Boundaries_and_Labels);
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

    Int_t GEM_Column_Sector_Digis_Raw[23][97] = {0};
    Int_t GEM_Column_Digis_Raw[23] = {0};
    Int_t GEM_Sector_Digis_Raw[97] = {0};
    Int_t GEM_Column_Sector_Digis_Raw_Single_ADC[31][23][97] = {0};
    Int_t GEM_Column_Digis_Raw_Single_ADC[31][23] = {0};
    Int_t GEM_Sector_Digis_Raw_Single_ADC[31][97] = {0};
    // Int_t GEM_Column_Sector_Digis_Raw_Single_ADC_Masked[31][23][97]={0};
    Int_t GEM_Column_Sector_Digis_Raw_Cumulative_ADC_Masked[30][23][97] = {0};
    Int_t GEM_Column_Sector_Digis_Noise_Masked[23][97] = {0};
    Int_t GEM_Column_Digis_Noise_Masked[23] = {0};
    Int_t GEM_Sector_Digis_Noise_Masked[97] = {0};
    Int_t GEM_Column_Sector_Digis_Noise_Masked_Single_ADC[31][23][97] = {0};
    Int_t GEM_Column_Digis_Noise_Masked_Single_ADC[31][23] = {0};
    Int_t GEM_Sector_Digis_Noise_Masked_Single_ADC[31][97] = {0};
    // Int_t GEM_Column_Sector_Digis_Noise_Masked_Single_ADC_Masked[31][23][97]={0};
    Int_t GEM_Column_Sector_Digis_Noise_Masked_Cumulative_ADC_Masked[30][23][97] = {0};
    Int_t GEM_Column_Sector_Digis_Noise_Only[23][97] = {0};
    Int_t GEM_Column_Digis_Noise_Only[23] = {0};
    Int_t GEM_Sector_Digis_Noise_Only[97] = {0};
    Int_t GEM_Column_Sector_Digis_Noise_Only_Single_ADC[31][23][97] = {0};
    Int_t GEM_Column_Digis_Noise_Only_Single_ADC[31][23] = {0};
    Int_t GEM_Sector_Digis_Noise_Only_Single_ADC[31][97] = {0};
    // Int_t GEM_Column_Sector_Digis_Noise_Only_Single_ADC_Masked[31][23][97]={0};
    Int_t GEM_Column_Sector_Digis_Noise_Only_Cumulative_ADC_Masked[30][23][97] = {0};

    Int_t GEM_FEB_Channel_Digis_Raw[18][128] = {0};
    Int_t GEM_FEB_Channel_Digis_Raw_Single_ADC[31][18][128] = {0};
    // Int_t GEM_FEB_Channel_Digis_Raw_Single_ADC_Masked[31][18][128]={0};
    Int_t GEM_FEB_Channel_Digis_Raw_Cumulative_ADC_Masked[30][18][128] = {0};
    Int_t GEM_FEB_Channel_Digis_Noise_Masked[18][128] = {0};
    Int_t GEM_FEB_Channel_Digis_Noise_Masked_Single_ADC[31][18][128] = {0};
    // Int_t GEM_FEB_Channel_Digis_Noise_Masked_Single_ADC_Masked[31][18][128]={0};
    Int_t GEM_FEB_Channel_Digis_Noise_Masked_Cumulative_ADC_Masked[30][18][128] = {0};
    Int_t GEM_FEB_Channel_Digis_Noise_Only[18][128] = {0};
    Int_t GEM_FEB_Channel_Digis_Noise_Only_Single_ADC[31][18][128] = {0};
    // Int_t GEM_FEB_Channel_Digis_Noise_Only_Single_ADC_Masked[31][18][128]={0};
    Int_t GEM_FEB_Channel_Digis_Noise_Only_Cumulative_ADC_Masked[30][18][128] = {0};

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
            Int_t ADC_Value = Much_Digi[iMuchDigi].GetAdc();

            GEM_Column_Sector_Digis_Raw[Channel][Sector]++;
            GEM_Column_Digis_Raw[Channel]++;
            GEM_Sector_Digis_Raw[Sector]++;
            GEM_FEB_Channel_Digis_Raw[GEM_FEB][GEM_CH]++;
            GEM_Column_Sector_Digis_Raw_Single_ADC[ADC_Value - 1][Channel][Sector]++;
            GEM_Column_Digis_Raw_Single_ADC[ADC_Value - 1][Channel]++;
            GEM_Sector_Digis_Raw_Single_ADC[ADC_Value - 1][Sector]++;
            GEM_FEB_Channel_Digis_Raw_Single_ADC[ADC_Value - 1][GEM_FEB][GEM_CH]++;

            if (GEM_Noise[Channel][Sector] != -1)
            {
                GEM_Column_Sector_Digis_Noise_Masked[Channel][Sector]++;
                GEM_Column_Digis_Noise_Masked[Channel]++;
                GEM_Sector_Digis_Noise_Masked[Sector]++;
                GEM_FEB_Channel_Digis_Noise_Masked[GEM_FEB][GEM_CH]++;
                GEM_Column_Sector_Digis_Noise_Masked_Single_ADC[ADC_Value - 1][Channel][Sector]++;
                GEM_Column_Digis_Noise_Masked_Single_ADC[ADC_Value - 1][Channel]++;
                GEM_Sector_Digis_Noise_Masked_Single_ADC[ADC_Value - 1][Sector]++;
                GEM_FEB_Channel_Digis_Noise_Masked_Single_ADC[ADC_Value - 1][GEM_FEB][GEM_CH]++;
            }
            else
            {
                GEM_Column_Sector_Digis_Noise_Only[Channel][Sector]++;
                GEM_Column_Digis_Noise_Only[Channel]++;
                GEM_Sector_Digis_Noise_Only[Sector]++;
                GEM_FEB_Channel_Digis_Noise_Only[GEM_FEB][GEM_CH]++;
                GEM_Column_Sector_Digis_Noise_Only_Single_ADC[ADC_Value - 1][Channel][Sector]++;
                GEM_Column_Digis_Noise_Only_Single_ADC[ADC_Value - 1][Channel]++;
                GEM_Sector_Digis_Noise_Only_Single_ADC[ADC_Value - 1][Sector]++;
                GEM_FEB_Channel_Digis_Noise_Only_Single_ADC[ADC_Value - 1][GEM_FEB][GEM_CH]++;
            }
        }
    }


    // 1D Digi Distributions

    for (Int_t iChannel = 0; iChannel < 23; iChannel++)
    {
        GEM_Without_Trigger_Column_Sector_Wise_1D_X_Raw_No_ADC_Cut->AddBinContent(iChannel + 2, GEM_Column_Digis_Raw[iChannel]);
        GEM_Without_Trigger_Column_Sector_Wise_1D_X_Noise_Masked_No_ADC_Cut->AddBinContent(iChannel + 2, GEM_Column_Digis_Noise_Masked[iChannel]);
        GEM_Without_Trigger_Column_Sector_Wise_1D_X_Noise_Only_No_ADC_Cut->AddBinContent(iChannel + 2, GEM_Column_Digis_Noise_Only[iChannel]);

        for (Int_t iADC = 1; iADC < 32; iADC++)
        {
            GEM_Without_Trigger_Column_Sector_Wise_1D_X_Raw_With_ADC_Cut_Single[iADC - 1]->AddBinContent(iChannel + 2, GEM_Column_Digis_Raw_Single_ADC[iADC - 1][iChannel]);
            GEM_Without_Trigger_Column_Sector_Wise_1D_X_Noise_Masked_With_ADC_Cut_Single[iADC - 1]->AddBinContent(iChannel + 2, GEM_Column_Digis_Noise_Masked_Single_ADC[iADC - 1][iChannel]);
            GEM_Without_Trigger_Column_Sector_Wise_1D_X_Noise_Only_With_ADC_Cut_Single[iADC - 1]->AddBinContent(iChannel + 2, GEM_Column_Digis_Noise_Only_Single_ADC[iADC - 1][iChannel]);
            GEM_Without_Trigger_Column_Sector_Wise_1D_X_Raw_With_ADC_Cut_Single_Masked[iADC - 1]->AddBinContent(iChannel + 2, GEM_Column_Digis_Raw[iChannel] - GEM_Column_Digis_Raw_Single_ADC[iADC - 1][iChannel]);
            GEM_Without_Trigger_Column_Sector_Wise_1D_X_Noise_Masked_With_ADC_Cut_Single_Masked[iADC - 1]->AddBinContent(iChannel + 2, GEM_Column_Digis_Noise_Masked[iChannel] - GEM_Column_Digis_Noise_Masked_Single_ADC[iADC - 1][iChannel]);
            GEM_Without_Trigger_Column_Sector_Wise_1D_X_Noise_Only_With_ADC_Cut_Single_Masked[iADC - 1]->AddBinContent(iChannel + 2, GEM_Column_Digis_Noise_Only[iChannel] - GEM_Column_Digis_Noise_Only_Single_ADC[iADC - 1][iChannel]);

            for (Int_t iADC1 = iADC + 1; iADC1 < 32; iADC1++)
            {
                GEM_Without_Trigger_Column_Sector_Wise_1D_X_Raw_With_ADC_Cut_Cumulative[iADC - 1]->AddBinContent(iChannel + 2, GEM_Column_Digis_Raw_Single_ADC[iADC1 - 1][iChannel]);
                GEM_Without_Trigger_Column_Sector_Wise_1D_X_Noise_Masked_With_ADC_Cut_Cumulative[iADC - 1]->AddBinContent(iChannel + 2, GEM_Column_Digis_Noise_Masked_Single_ADC[iADC1 - 1][iChannel]);
                GEM_Without_Trigger_Column_Sector_Wise_1D_X_Noise_Only_With_ADC_Cut_Cumulative[iADC - 1]->AddBinContent(iChannel + 2, GEM_Column_Digis_Noise_Only_Single_ADC[iADC1 - 1][iChannel]);
            }
        }
    }

    for (Int_t iSector = 0; iSector < 97; iSector++)
    {
        GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Raw_No_ADC_Cut->AddBinContent(iSector + 2, GEM_Sector_Digis_Raw[iSector]);
        GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Noise_Masked_No_ADC_Cut->AddBinContent(iSector + 2, GEM_Sector_Digis_Noise_Masked[iSector]);
        GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Noise_Only_No_ADC_Cut->AddBinContent(iSector + 2, GEM_Sector_Digis_Noise_Only[iSector]);

        for (Int_t iADC = 1; iADC < 32; iADC++)
        {
            GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Raw_With_ADC_Cut_Single[iADC - 1]->AddBinContent(iSector + 2, GEM_Sector_Digis_Raw_Single_ADC[iADC - 1][iSector]);
            GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Noise_Masked_With_ADC_Cut_Single[iADC - 1]->AddBinContent(iSector + 2, GEM_Sector_Digis_Noise_Masked_Single_ADC[iADC - 1][iSector]);
            GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Noise_Only_With_ADC_Cut_Single[iADC - 1]->AddBinContent(iSector + 2, GEM_Sector_Digis_Noise_Only_Single_ADC[iADC - 1][iSector]);
            GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Raw_With_ADC_Cut_Single_Masked[iADC - 1]->AddBinContent(iSector + 2, GEM_Sector_Digis_Raw[iSector] - GEM_Sector_Digis_Raw_Single_ADC[iADC - 1][iSector]);
            GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Noise_Masked_With_ADC_Cut_Single_Masked[iADC - 1]->AddBinContent(iSector + 2, GEM_Sector_Digis_Noise_Masked[iSector] - GEM_Sector_Digis_Noise_Masked_Single_ADC[iADC - 1][iSector]);
            GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Noise_Only_With_ADC_Cut_Single_Masked[iADC - 1]->AddBinContent(iSector + 2, GEM_Sector_Digis_Noise_Only[iSector] - GEM_Sector_Digis_Noise_Only_Single_ADC[iADC - 1][iSector]);

            for (Int_t iADC1 = iADC + 1; iADC1 < 32; iADC1++)
            {
                GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Raw_With_ADC_Cut_Cumulative[iADC - 1]->AddBinContent(iSector + 2, GEM_Sector_Digis_Raw_Single_ADC[iADC1 - 1][iSector]);
                GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Noise_Masked_With_ADC_Cut_Cumulative[iADC - 1]->AddBinContent(iSector + 2, GEM_Sector_Digis_Noise_Masked_Single_ADC[iADC1 - 1][iSector]);
                GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Noise_Only_With_ADC_Cut_Cumulative[iADC - 1]->AddBinContent(iSector + 2, GEM_Sector_Digis_Noise_Only_Single_ADC[iADC1 - 1][iSector]);
            }
        }
    }

    for (Int_t iChannel = 0; iChannel < 23; iChannel++)
    {
        for (Int_t iSector = 0; iSector < 97; iSector++)
        {
            GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Raw_No_ADC_Cut->Fill(iSector, iChannel, GEM_Column_Sector_Digis_Raw[iChannel][iSector]);
            GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Noise_Masked_No_ADC_Cut->Fill(iSector, iChannel, GEM_Column_Sector_Digis_Noise_Masked[iChannel][iSector]);
            GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Noise_Only_No_ADC_Cut->Fill(iSector, iChannel, GEM_Column_Sector_Digis_Noise_Only[iChannel][iSector]);

            Int_t Weight = GEM_Column_Sector_Digis_Raw[iChannel][iSector];
            Highlight_Pad(iChannel, iSector, Weight, GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Raw_No_ADC_Cut, Wall_Near_Smaller_Pads);
            Weight = GEM_Column_Sector_Digis_Noise_Masked[iChannel][iSector];
            Highlight_Pad(iChannel, iSector, Weight, GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Noise_Masked_No_ADC_Cut, Wall_Near_Smaller_Pads);
            Weight = GEM_Column_Sector_Digis_Noise_Only[iChannel][iSector];
            Highlight_Pad(iChannel, iSector, Weight, GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Noise_Only_No_ADC_Cut, Wall_Near_Smaller_Pads);

            for (Int_t iADC = 1; iADC < 32; iADC++)
            {
                GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Raw_With_ADC_Cut_Single[iADC - 1]->Fill(iSector, iChannel, GEM_Column_Sector_Digis_Raw_Single_ADC[iADC - 1][iChannel][iSector]);
                GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Raw_With_ADC_Cut_Single_Masked[iADC - 1]->Fill(iSector, iChannel, GEM_Column_Sector_Digis_Raw[iChannel][iSector] - GEM_Column_Sector_Digis_Raw_Single_ADC[iADC - 1][iChannel][iSector]);
                GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Noise_Masked_With_ADC_Cut_Single[iADC - 1]->Fill(iSector, iChannel, GEM_Column_Sector_Digis_Noise_Masked_Single_ADC[iADC - 1][iChannel][iSector]);
                GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Noise_Masked_With_ADC_Cut_Single_Masked[iADC - 1]->Fill(iSector, iChannel, GEM_Column_Sector_Digis_Noise_Masked[iChannel][iSector] - GEM_Column_Sector_Digis_Noise_Masked_Single_ADC[iADC - 1][iChannel][iSector]);
                GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Noise_Only_With_ADC_Cut_Single[iADC - 1]->Fill(iSector, iChannel, GEM_Column_Sector_Digis_Noise_Only_Single_ADC[iADC - 1][iChannel][iSector]);
                GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Noise_Only_With_ADC_Cut_Single_Masked[iADC - 1]->Fill(iSector, iChannel, GEM_Column_Sector_Digis_Noise_Only[iChannel][iSector] - GEM_Column_Sector_Digis_Noise_Masked_Single_ADC[iADC - 1][iChannel][iSector]);

                // Weight = GEM_Column_Sector_Digis_Raw_Single_ADC[iADC-1][iChannel][iSector];
                // TH2F* Local_Histogram = GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Raw_With_ADC_Cut_Single[iADC-1];
                // Highlight_Pad(iChannel,iSector,Weight,Local_Histogram,Wall_Near_Smaller_Pads);

                // Weight = GEM_Column_Sector_Digis_Raw[iChannel][iSector] - GEM_Column_Sector_Digis_Raw_Single_ADC[iADC-1][iChannel][iSector];
                // Local_Histogram = GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Raw_With_ADC_Cut_Single_Masked[iADC-1];
                // Highlight_Pad(iChannel,iSector,Weight,Local_Histogram,Wall_Near_Smaller_Pads);

                // Weight = GEM_Column_Sector_Digis_Noise_Masked_Single_ADC[iADC-1][iChannel][iSector];
                // Local_Histogram = GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Noise_Masked_With_ADC_Cut_Single[iADC-1];
                // Highlight_Pad(iChannel,iSector,Weight,Local_Histogram,Wall_Near_Smaller_Pads);

                // Weight = GEM_Column_Sector_Digis_Noise_Masked[iChannel][iSector] - GEM_Column_Sector_Digis_Noise_Masked_Single_ADC[iADC-1][iChannel][iSector];
                // Local_Histogram = GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Noise_Masked_With_ADC_Cut_Single_Masked[iADC-1];
                // Highlight_Pad(iChannel,iSector,Weight,Local_Histogram,Wall_Near_Smaller_Pads);

                // Weight = GEM_Column_Sector_Digis_Noise_Only_Single_ADC[iADC-1][iChannel][iSector];
                // Local_Histogram = GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Noise_Only_With_ADC_Cut_Single[iADC-1];
                // Highlight_Pad(iChannel,iSector,Weight,Local_Histogram,Wall_Near_Smaller_Pads);

                // Weight = GEM_Column_Sector_Digis_Noise_Only[iChannel][iSector] - GEM_Column_Sector_Digis_Noise_Only_Single_ADC[iADC-1][iChannel][iSector];
                // Local_Histogram = GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Noise_Only_With_ADC_Cut_Single_Masked[iADC-1];
                // Highlight_Pad(iChannel,iSector,Weight,Local_Histogram,Wall_Near_Smaller_Pads);

                for (Int_t iADC1 = iADC + 1; iADC1 < 32; iADC1++)
                {
                    GEM_Column_Sector_Digis_Raw_Cumulative_ADC_Masked[iADC - 1][iChannel][iSector] += GEM_Column_Sector_Digis_Raw_Single_ADC[iADC1 - 1][iChannel][iSector];
                    GEM_Column_Sector_Digis_Noise_Masked_Cumulative_ADC_Masked[iADC - 1][iChannel][iSector] += GEM_Column_Sector_Digis_Noise_Masked_Single_ADC[iADC1 - 1][iChannel][iSector];
                    GEM_Column_Sector_Digis_Noise_Only_Cumulative_ADC_Masked[iADC - 1][iChannel][iSector] += GEM_Column_Sector_Digis_Noise_Only_Single_ADC[iADC1 - 1][iChannel][iSector];
                }

                if (iADC != 31)
                {
                    GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Raw_With_ADC_Cut_Cumulative[iADC - 1]->Fill(iSector, iChannel, GEM_Column_Sector_Digis_Raw_Cumulative_ADC_Masked[iADC - 1][iChannel][iSector]);
                    GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Noise_Masked_With_ADC_Cut_Cumulative[iADC - 1]->Fill(iSector, iChannel, GEM_Column_Sector_Digis_Noise_Masked_Cumulative_ADC_Masked[iADC - 1][iChannel][iSector]);
                    GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Noise_Only_With_ADC_Cut_Cumulative[iADC - 1]->Fill(iSector, iChannel, GEM_Column_Sector_Digis_Noise_Only_Cumulative_ADC_Masked[iADC - 1][iChannel][iSector]);

                    // Weight = GEM_Column_Sector_Digis_Raw_Cumulative_ADC_Masked[iADC-1][iChannel][iSector];
                    // Local_Histogram = GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Raw_With_ADC_Cut_Cumulative[iADC-1];
                    // Highlight_Pad(iChannel,iSector,Weight,Local_Histogram,Wall_Near_Smaller_Pads);

                    // Weight = GEM_Column_Sector_Digis_Noise_Masked_Cumulative_ADC_Masked[iADC-1][iChannel][iSector];
                    // Local_Histogram = GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Noise_Masked_With_ADC_Cut_Cumulative[iADC-1];
                    // Highlight_Pad(iChannel,iSector,Weight,Local_Histogram,Wall_Near_Smaller_Pads);

                    // Weight = GEM_Column_Sector_Digis_Noise_Only_Cumulative_ADC_Masked[iADC-1][iChannel][iSector];
                    // Local_Histogram = GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Noise_Only_With_ADC_Cut_Cumulative[iADC-1];
                    // Highlight_Pad(iChannel,iSector,Weight,Local_Histogram,Wall_Near_Smaller_Pads);
                }
            }
        }
    }


    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        for (Int_t iChannel = 0; iChannel < 128; iChannel++)
        {
            GEM_Without_Trigger_Feb_Channel_Wise_1D_Channel_Map_Raw_Feb[iFeb]->SetBinContent(iChannel + 2, GEM_FEB_Channel_Digis_Raw[iFeb][iChannel]);
            GEM_Without_Trigger_Feb_Channel_Wise_1D_Channel_Map_Noise_Masked_Feb[iFeb]->SetBinContent(iChannel + 2, GEM_FEB_Channel_Digis_Noise_Masked[iFeb][iChannel]);
            GEM_Without_Trigger_Feb_Channel_Wise_1D_Channel_Map_Noise_Only_Feb[iFeb]->SetBinContent(iChannel + 2, GEM_FEB_Channel_Digis_Noise_Only[iFeb][iChannel]);

            GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Raw_No_ADC_Cut->Fill(iChannel, iFeb, GEM_FEB_Channel_Digis_Raw[iFeb][iChannel]);
            GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Noise_Masked_No_ADC_Cut->Fill(iChannel, iFeb, GEM_FEB_Channel_Digis_Noise_Masked[iFeb][iChannel]);
            GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Noise_Only_No_ADC_Cut->Fill(iChannel, iFeb, GEM_FEB_Channel_Digis_Noise_Only[iFeb][iChannel]);

            for (Int_t iADC = 1; iADC < 32; iADC++)
            {
                GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Raw_With_ADC_Cut_Single[iADC - 1]->Fill(iChannel, iFeb, GEM_FEB_Channel_Digis_Raw_Single_ADC[iADC - 1][iFeb][iChannel]);
                GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Raw_With_ADC_Cut_Single_Masked[iADC - 1]->Fill(iChannel, iFeb, GEM_FEB_Channel_Digis_Raw[iFeb][iChannel] - GEM_FEB_Channel_Digis_Raw_Single_ADC[iADC - 1][iFeb][iChannel]);
                GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Noise_Masked_With_ADC_Cut_Single[iADC - 1]->Fill(iChannel, iFeb, GEM_FEB_Channel_Digis_Noise_Masked_Single_ADC[iADC - 1][iFeb][iChannel]);
                GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Noise_Masked_With_ADC_Cut_Single_Masked[iADC - 1]->Fill(iChannel, iFeb, GEM_FEB_Channel_Digis_Noise_Masked[iFeb][iChannel] - GEM_FEB_Channel_Digis_Noise_Masked_Single_ADC[iADC - 1][iFeb][iChannel]);
                GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Noise_Only_With_ADC_Cut_Single[iADC - 1]->Fill(iChannel, iFeb, GEM_FEB_Channel_Digis_Noise_Only_Single_ADC[iADC - 1][iFeb][iChannel]);
                GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Noise_Only_With_ADC_Cut_Single_Masked[iADC - 1]->Fill(iChannel, iFeb, GEM_FEB_Channel_Digis_Noise_Only[iFeb][iChannel] - GEM_FEB_Channel_Digis_Noise_Only_Single_ADC[iADC - 1][iFeb][iChannel]);

                for (Int_t iADC1 = iADC + 1; iADC1 < 32; iADC1++)
                {
                    GEM_FEB_Channel_Digis_Raw_Cumulative_ADC_Masked[iADC - 1][iFeb][iChannel] += GEM_FEB_Channel_Digis_Raw_Single_ADC[iADC1 - 1][iFeb][iChannel];
                    GEM_FEB_Channel_Digis_Noise_Masked_Cumulative_ADC_Masked[iADC - 1][iFeb][iChannel] += GEM_FEB_Channel_Digis_Noise_Masked_Single_ADC[iADC1 - 1][iFeb][iChannel];
                    GEM_FEB_Channel_Digis_Noise_Only_Cumulative_ADC_Masked[iADC - 1][iFeb][iChannel] += GEM_FEB_Channel_Digis_Noise_Only_Single_ADC[iADC1 - 1][iFeb][iChannel];
                }

                if (iADC != 31)
                {
                    GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Raw_With_ADC_Cut_Cumulative[iADC - 1]->Fill(iChannel, iFeb, GEM_FEB_Channel_Digis_Raw_Cumulative_ADC_Masked[iADC - 1][iFeb][iChannel]);
                    GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Noise_Masked_With_ADC_Cut_Cumulative[iADC - 1]->Fill(iChannel, iFeb, GEM_FEB_Channel_Digis_Noise_Masked_Cumulative_ADC_Masked[iADC - 1][iFeb][iChannel]);
                    GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Noise_Only_With_ADC_Cut_Cumulative[iADC - 1]->Fill(iChannel, iFeb, GEM_FEB_Channel_Digis_Noise_Only_Cumulative_ADC_Masked[iADC - 1][iFeb][iChannel]);
                }
            }
        }
    }

    fOut->mkdir("Digi Distributions");
    fOut->mkdir("Digi Distributions/GEM");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Column Wise");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Column Wise/Raw");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Column Wise/Raw/No ADC Cut");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Column Wise/Raw/With ADC Cut");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Column Wise/Raw/With ADC Cut/Single ADC");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Column Wise/Raw/With ADC Cut/Single Masked ADC");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Column Wise/Raw/With ADC Cut/Cumulative");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Column Wise/Noise Masked");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Column Wise/Noise Masked/No ADC Cut");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Column Wise/Noise Masked/With ADC Cut");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Column Wise/Noise Masked/With ADC Cut/Single ADC");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Column Wise/Noise Masked/With ADC Cut/Single Masked ADC");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Column Wise/Noise Masked/With ADC Cut/Cumulative");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Column Wise/Noise Only");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Column Wise/Noise Only/No ADC Cut");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Column Wise/Noise Only/With ADC Cut");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Column Wise/Noise Only/With ADC Cut/Single ADC");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Column Wise/Noise Only/With ADC Cut/Single Masked ADC");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Column Wise/Noise Only/With ADC Cut/Cumulative");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Sector Wise");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Sector Wise/Raw");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Sector Wise/Raw/No ADC Cut");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Sector Wise/Raw/With ADC Cut");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Sector Wise/Raw/With ADC Cut/Single ADC");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Sector Wise/Raw/With ADC Cut/Single Masked ADC");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Sector Wise/Raw/With ADC Cut/Cumulative");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Sector Wise/Noise Masked");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Sector Wise/Noise Masked/No ADC Cut");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Sector Wise/Noise Masked/With ADC Cut");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Sector Wise/Noise Masked/With ADC Cut/Single ADC");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Sector Wise/Noise Masked/With ADC Cut/Single Masked ADC");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Sector Wise/Noise Masked/With ADC Cut/Cumulative");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Sector Wise/Noise Only");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Sector Wise/Noise Only/No ADC Cut");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Sector Wise/Noise Only/With ADC Cut");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Sector Wise/Noise Only/With ADC Cut/Single ADC");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Sector Wise/Noise Only/With ADC Cut/Single Masked ADC");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Sector Wise/Noise Only/With ADC Cut/Cumulative");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Rectangular");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Rectangular/Raw");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Rectangular/Raw/No ADC Cut");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Rectangular/Raw/With ADC Cut");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Rectangular/Raw/With ADC Cut/Single ADC");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Rectangular/Raw/With ADC Cut/Single Masked ADC");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Rectangular/Raw/With ADC Cut/Cumulative");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Rectangular/Noise Masked");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Rectangular/Noise Masked/No ADC Cut");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Rectangular/Noise Masked/With ADC Cut");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Rectangular/Noise Masked/With ADC Cut/Single ADC");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Rectangular/Noise Masked/With ADC Cut/Single Masked ADC");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Rectangular/Noise Masked/With ADC Cut/Cumulative");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Rectangular/Noise Only");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Rectangular/Noise Only/No ADC Cut");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Rectangular/Noise Only/With ADC Cut");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Rectangular/Noise Only/With ADC Cut/Single ADC");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Rectangular/Noise Only/With ADC Cut/Single Masked ADC");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Rectangular/Noise Only/With ADC Cut/Cumulative");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Trapezoidal");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Trapezoidal/Labels");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Trapezoidal/Raw");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Trapezoidal/Raw/No ADC Cut");
    // fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Trapezoidal/Raw/With ADC Cut");
    // fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Trapezoidal/Raw/With ADC Cut/Single ADC");
    // fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Trapezoidal/Raw/With ADC Cut/Single Masked ADC");
    // fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Trapezoidal/Raw/With ADC Cut/Cumulative");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Trapezoidal/Noise Masked");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Trapezoidal/Noise Masked/No ADC Cut");
    // fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Trapezoidal/Noise Masked/With ADC Cut");
    // fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Trapezoidal/Noise Masked/With ADC Cut/Single ADC");
    // fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Trapezoidal/Noise Masked/With ADC Cut/Single Masked ADC");
    // fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Trapezoidal/Noise Masked/With ADC Cut/Cumulative");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Trapezoidal/Noise Only");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Trapezoidal/Noise Only/No ADC Cut");
    // fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Trapezoidal/Noise Only/With ADC Cut");
    // fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Trapezoidal/Noise Only/With ADC Cut/Single ADC");
    // fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Trapezoidal/Noise Only/With ADC Cut/Single Masked ADC");
    // fOut->mkdir("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Trapezoidal/Noise Only/With ADC Cut/Cumulative");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Feb Channel Wise");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Feb Channel Wise/1D");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Feb Channel Wise/1D/Channel Map");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Feb Channel Wise/1D/Channel Map/Raw");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Feb Channel Wise/1D/Channel Map/Noise Masked");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Feb Channel Wise/1D/Channel Map/Noise Only");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Feb Channel Wise/2D");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Feb Channel Wise/2D/Rectangular");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Feb Channel Wise/2D/Rectangular/Raw");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Feb Channel Wise/2D/Rectangular/Raw/No ADC Cut");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Feb Channel Wise/2D/Rectangular/Raw/With ADC Cut");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Feb Channel Wise/2D/Rectangular/Raw/With ADC Cut/Single ADC");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Feb Channel Wise/2D/Rectangular/Raw/With ADC Cut/Single Masked ADC");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Feb Channel Wise/2D/Rectangular/Raw/With ADC Cut/Cumulative");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Feb Channel Wise/2D/Rectangular/Noise Masked");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Feb Channel Wise/2D/Rectangular/Noise Masked/No ADC Cut");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Feb Channel Wise/2D/Rectangular/Noise Masked/With ADC Cut");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Feb Channel Wise/2D/Rectangular/Noise Masked/With ADC Cut/Single ADC");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Feb Channel Wise/2D/Rectangular/Noise Masked/With ADC Cut/Single Masked ADC");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Feb Channel Wise/2D/Rectangular/Noise Masked/With ADC Cut/Cumulative");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Feb Channel Wise/2D/Rectangular/Noise Only");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Feb Channel Wise/2D/Rectangular/Noise Only/No ADC Cut");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Feb Channel Wise/2D/Rectangular/Noise Only/With ADC Cut");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Feb Channel Wise/2D/Rectangular/Noise Only/With ADC Cut/Single ADC");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Feb Channel Wise/2D/Rectangular/Noise Only/With ADC Cut/Single Masked ADC");
    fOut->mkdir("Digi Distributions/GEM/Without Trigger/Feb Channel Wise/2D/Rectangular/Noise Only/With ADC Cut/Cumulative");


    fOut->cd("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Column Wise/Raw/No ADC Cut");
    GEM_Without_Trigger_Column_Sector_Wise_1D_X_Raw_No_ADC_Cut->Write();

    fOut->cd("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Column Wise/Raw/With ADC Cut/Single ADC");
    for (Int_t iADC = 1; iADC < 32; iADC++)
        GEM_Without_Trigger_Column_Sector_Wise_1D_X_Raw_With_ADC_Cut_Single[iADC - 1]->Write();

    fOut->cd("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Column Wise/Raw/With ADC Cut/Single Masked ADC");
    for (Int_t iADC = 1; iADC < 32; iADC++)
        GEM_Without_Trigger_Column_Sector_Wise_1D_X_Raw_With_ADC_Cut_Single_Masked[iADC - 1]->Write();

    fOut->cd("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Column Wise/Raw/With ADC Cut/Cumulative");
    for (Int_t iADC = 1; iADC < 31; iADC++)
        GEM_Without_Trigger_Column_Sector_Wise_1D_X_Raw_With_ADC_Cut_Cumulative[iADC - 1]->Write();

    fOut->cd("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Column Wise/Noise Masked/No ADC Cut");
    GEM_Without_Trigger_Column_Sector_Wise_1D_X_Noise_Masked_No_ADC_Cut->Write();

    fOut->cd("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Column Wise/Noise Masked/With ADC Cut/Single ADC");
    for (Int_t iADC = 1; iADC < 32; iADC++)
        GEM_Without_Trigger_Column_Sector_Wise_1D_X_Noise_Masked_With_ADC_Cut_Single[iADC - 1]->Write();

    fOut->cd("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Column Wise/Noise Masked/With ADC Cut/Single Masked ADC");
    for (Int_t iADC = 1; iADC < 32; iADC++)
        GEM_Without_Trigger_Column_Sector_Wise_1D_X_Noise_Masked_With_ADC_Cut_Single_Masked[iADC - 1]->Write();

    fOut->cd("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Column Wise/Noise Masked/With ADC Cut/Cumulative");
    for (Int_t iADC = 1; iADC < 31; iADC++)
        GEM_Without_Trigger_Column_Sector_Wise_1D_X_Noise_Masked_With_ADC_Cut_Cumulative[iADC - 1]->Write();

    fOut->cd("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Column Wise/Noise Only/No ADC Cut");
    GEM_Without_Trigger_Column_Sector_Wise_1D_X_Noise_Only_No_ADC_Cut->Write();

    fOut->cd("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Column Wise/Noise Only/With ADC Cut/Single ADC");
    for (Int_t iADC = 1; iADC < 32; iADC++)
        GEM_Without_Trigger_Column_Sector_Wise_1D_X_Noise_Only_With_ADC_Cut_Single[iADC - 1]->Write();

    fOut->cd("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Column Wise/Noise Only/With ADC Cut/Single Masked ADC");
    for (Int_t iADC = 1; iADC < 32; iADC++)
        GEM_Without_Trigger_Column_Sector_Wise_1D_X_Noise_Only_With_ADC_Cut_Single_Masked[iADC - 1]->Write();

    fOut->cd("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Column Wise/Noise Only/With ADC Cut/Cumulative");
    for (Int_t iADC = 1; iADC < 31; iADC++)
        GEM_Without_Trigger_Column_Sector_Wise_1D_X_Noise_Only_With_ADC_Cut_Cumulative[iADC - 1]->Write();

    fOut->cd("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Sector Wise/Raw/No ADC Cut");
    GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Raw_No_ADC_Cut->Write();

    fOut->cd("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Sector Wise/Raw/With ADC Cut/Single ADC");
    for (Int_t iADC = 1; iADC < 32; iADC++)
        GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Raw_With_ADC_Cut_Single[iADC - 1]->Write();

    fOut->cd("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Sector Wise/Raw/With ADC Cut/Single Masked ADC");
    for (Int_t iADC = 1; iADC < 32; iADC++)
        GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Raw_With_ADC_Cut_Single_Masked[iADC - 1]->Write();

    fOut->cd("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Sector Wise/Raw/With ADC Cut/Cumulative");
    for (Int_t iADC = 1; iADC < 31; iADC++)
        GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Raw_With_ADC_Cut_Cumulative[iADC - 1]->Write();

    fOut->cd("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Sector Wise/Noise Masked/No ADC Cut");
    GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Noise_Masked_No_ADC_Cut->Write();

    fOut->cd("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Sector Wise/Noise Masked/With ADC Cut/Single ADC");
    for (Int_t iADC = 1; iADC < 32; iADC++)
        GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Noise_Masked_With_ADC_Cut_Single[iADC - 1]->Write();

    fOut->cd("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Sector Wise/Noise Masked/With ADC Cut/Single Masked ADC");
    for (Int_t iADC = 1; iADC < 32; iADC++)
        GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Noise_Masked_With_ADC_Cut_Single_Masked[iADC - 1]->Write();

    fOut->cd("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Sector Wise/Noise Masked/With ADC Cut/Cumulative");
    for (Int_t iADC = 1; iADC < 31; iADC++)
        GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Noise_Masked_With_ADC_Cut_Cumulative[iADC - 1]->Write();

    fOut->cd("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Sector Wise/Noise Only/No ADC Cut");
    GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Noise_Only_No_ADC_Cut->Write();

    fOut->cd("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Sector Wise/Noise Only/With ADC Cut/Single ADC");
    for (Int_t iADC = 1; iADC < 32; iADC++)
        GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Noise_Only_With_ADC_Cut_Single[iADC - 1]->Write();

    fOut->cd("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Sector Wise/Noise Only/With ADC Cut/Single Masked ADC");
    for (Int_t iADC = 1; iADC < 32; iADC++)
        GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Noise_Only_With_ADC_Cut_Single_Masked[iADC - 1]->Write();

    fOut->cd("Digi Distributions/GEM/Without Trigger/Column Sector Wise/1D/Sector Wise/Noise Only/With ADC Cut/Cumulative");
    for (Int_t iADC = 1; iADC < 31; iADC++)
        GEM_Without_Trigger_Column_Sector_Wise_1D_Y_Noise_Only_With_ADC_Cut_Cumulative[iADC - 1]->Write();

    fOut->cd("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Rectangular/Raw/No ADC Cut");
    GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Raw_No_ADC_Cut->Write();

    fOut->cd("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Rectangular/Raw/With ADC Cut/Single ADC");
    for (Int_t iADC = 1; iADC < 32; iADC++)
        GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Raw_With_ADC_Cut_Single[iADC - 1]->Write();

    fOut->cd("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Rectangular/Raw/With ADC Cut/Single Masked ADC");
    for (Int_t iADC = 1; iADC < 32; iADC++)
        GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Raw_With_ADC_Cut_Single_Masked[iADC - 1]->Write();

    fOut->cd("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Rectangular/Raw/With ADC Cut/Cumulative");
    for (Int_t iADC = 1; iADC < 31; iADC++)
        GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Raw_With_ADC_Cut_Cumulative[iADC - 1]->Write();

    fOut->cd("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Rectangular/Noise Masked/No ADC Cut");
    GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Noise_Masked_No_ADC_Cut->Write();

    fOut->cd("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Rectangular/Noise Masked/With ADC Cut/Single ADC");
    for (Int_t iADC = 1; iADC < 32; iADC++)
        GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Noise_Masked_With_ADC_Cut_Single[iADC - 1]->Write();

    fOut->cd("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Rectangular/Noise Masked/With ADC Cut/Single Masked ADC");
    for (Int_t iADC = 1; iADC < 32; iADC++)
        GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Noise_Masked_With_ADC_Cut_Single_Masked[iADC - 1]->Write();

    fOut->cd("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Rectangular/Noise Masked/With ADC Cut/Cumulative");
    for (Int_t iADC = 1; iADC < 31; iADC++)
        GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Noise_Masked_With_ADC_Cut_Cumulative[iADC - 1]->Write();

    fOut->cd("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Rectangular/Noise Only/No ADC Cut");
    GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Noise_Only_No_ADC_Cut->Write();

    fOut->cd("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Rectangular/Noise Only/With ADC Cut/Single ADC");
    for (Int_t iADC = 1; iADC < 32; iADC++)
        GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Noise_Only_With_ADC_Cut_Single[iADC - 1]->Write();

    fOut->cd("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Rectangular/Noise Only/With ADC Cut/Single Masked ADC");
    for (Int_t iADC = 1; iADC < 32; iADC++)
        GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Noise_Only_With_ADC_Cut_Single_Masked[iADC - 1]->Write();

    fOut->cd("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Rectangular/Noise Only/With ADC Cut/Cumulative");
    for (Int_t iADC = 1; iADC < 31; iADC++)
        GEM_Without_Trigger_Column_Sector_Wise_2D_Rectangular_Noise_Only_With_ADC_Cut_Cumulative[iADC - 1]->Write();

    fOut->cd("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Trapezoidal/Labels");
    GEM_Feb_Boundaries_and_Labels->Write();

    fOut->cd("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Trapezoidal/Raw/No ADC Cut");
    GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Raw_No_ADC_Cut->Write();

    // fOut->cd("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Trapezoidal/Raw/With ADC Cut/Single ADC");
    // for(Int_t iADC=1;iADC<32;iADC++)    GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Raw_With_ADC_Cut_Single[iADC-1]->Write();

    // fOut->cd("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Trapezoidal/Raw/With ADC Cut/Single Masked ADC");
    // for(Int_t iADC=1;iADC<32;iADC++)    GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Raw_With_ADC_Cut_Single_Masked[iADC-1]->Write();

    // fOut->cd("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Trapezoidal/Raw/With ADC Cut/Cumulative");
    // for(Int_t iADC=1;iADC<31;iADC++)    GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Raw_With_ADC_Cut_Cumulative[iADC-1]->Write();

    fOut->cd("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Trapezoidal/Noise Masked/No ADC Cut");
    GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Noise_Masked_No_ADC_Cut->Write();

    // fOut->cd("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Trapezoidal/Noise Masked/With ADC Cut/Single ADC");
    // for(Int_t iADC=1;iADC<32;iADC++)    GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Noise_Masked_With_ADC_Cut_Single[iADC-1]->Write();

    // fOut->cd("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Trapezoidal/Noise Masked/With ADC Cut/Single Masked ADC");
    // for(Int_t iADC=1;iADC<32;iADC++)    GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Noise_Masked_With_ADC_Cut_Single_Masked[iADC-1]->Write();

    // fOut->cd("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Trapezoidal/Noise Masked/With ADC Cut/Cumulative");
    // for(Int_t iADC=1;iADC<31;iADC++)    GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Noise_Masked_With_ADC_Cut_Cumulative[iADC-1]->Write();

    fOut->cd("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Trapezoidal/Noise Only/No ADC Cut");
    GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Noise_Only_No_ADC_Cut->Write();

    // fOut->cd("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Trapezoidal/Noise Only/With ADC Cut/Single ADC");
    // for(Int_t iADC=1;iADC<32;iADC++)    GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Noise_Only_With_ADC_Cut_Single[iADC-1]->Write();

    // fOut->cd("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Trapezoidal/Noise Only/With ADC Cut/Single Masked ADC");
    // for(Int_t iADC=1;iADC<32;iADC++)    GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Noise_Only_With_ADC_Cut_Single_Masked[iADC-1]->Write();

    // fOut->cd("Digi Distributions/GEM/Without Trigger/Column Sector Wise/2D/Trapezoidal/Noise Only/With ADC Cut/Cumulative");
    // for(Int_t iADC=1;iADC<31;iADC++)    GEM_Without_Trigger_Column_Sector_Wise_2D_Trapezoidal_Noise_Only_With_ADC_Cut_Cumulative[iADC-1]->Write();

    fOut->cd("Digi Distributions/GEM/Without Trigger/Feb Channel Wise/1D/Channel Map/Raw");
    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
        GEM_Without_Trigger_Feb_Channel_Wise_1D_Channel_Map_Raw_Feb[iFeb]->Write();

    fOut->cd("Digi Distributions/GEM/Without Trigger/Feb Channel Wise/1D/Channel Map/Noise Masked");
    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
        GEM_Without_Trigger_Feb_Channel_Wise_1D_Channel_Map_Noise_Masked_Feb[iFeb]->Write();

    fOut->cd("Digi Distributions/GEM/Without Trigger/Feb Channel Wise/1D/Channel Map/Noise Only");
    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
        GEM_Without_Trigger_Feb_Channel_Wise_1D_Channel_Map_Noise_Only_Feb[iFeb]->Write();

    fOut->cd("Digi Distributions/GEM/Without Trigger/Feb Channel Wise/2D/Rectangular/Raw/No ADC Cut");
    GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Raw_No_ADC_Cut->Write();

    fOut->cd("Digi Distributions/GEM/Without Trigger/Feb Channel Wise/2D/Rectangular/Raw/With ADC Cut/Single ADC");
    for (Int_t iADC = 1; iADC < 32; iADC++)
        GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Raw_With_ADC_Cut_Single[iADC - 1]->Write();

    fOut->cd("Digi Distributions/GEM/Without Trigger/Feb Channel Wise/2D/Rectangular/Raw/With ADC Cut/Single Masked ADC");
    for (Int_t iADC = 1; iADC < 32; iADC++)
        GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Raw_With_ADC_Cut_Single_Masked[iADC - 1]->Write();

    fOut->cd("Digi Distributions/GEM/Without Trigger/Feb Channel Wise/2D/Rectangular/Raw/With ADC Cut/Cumulative");
    for (Int_t iADC = 1; iADC < 31; iADC++)
        GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Raw_With_ADC_Cut_Cumulative[iADC - 1]->Write();

    fOut->cd("Digi Distributions/GEM/Without Trigger/Feb Channel Wise/2D/Rectangular/Noise Masked/No ADC Cut");
    GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Noise_Masked_No_ADC_Cut->Write();

    fOut->cd("Digi Distributions/GEM/Without Trigger/Feb Channel Wise/2D/Rectangular/Noise Masked/With ADC Cut/Single ADC");
    for (Int_t iADC = 1; iADC < 32; iADC++)
        GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Noise_Masked_With_ADC_Cut_Single[iADC - 1]->Write();

    fOut->cd("Digi Distributions/GEM/Without Trigger/Feb Channel Wise/2D/Rectangular/Noise Masked/With ADC Cut/Single Masked ADC");
    for (Int_t iADC = 1; iADC < 32; iADC++)
        GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Noise_Masked_With_ADC_Cut_Single_Masked[iADC - 1]->Write();

    fOut->cd("Digi Distributions/GEM/Without Trigger/Feb Channel Wise/2D/Rectangular/Noise Masked/With ADC Cut/Cumulative");
    for (Int_t iADC = 1; iADC < 31; iADC++)
        GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Noise_Masked_With_ADC_Cut_Cumulative[iADC - 1]->Write();

    fOut->cd("Digi Distributions/GEM/Without Trigger/Feb Channel Wise/2D/Rectangular/Noise Only/No ADC Cut");
    GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Noise_Only_No_ADC_Cut->Write();

    fOut->cd("Digi Distributions/GEM/Without Trigger/Feb Channel Wise/2D/Rectangular/Noise Only/With ADC Cut/Single ADC");
    for (Int_t iADC = 1; iADC < 32; iADC++)
        GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Noise_Only_With_ADC_Cut_Single[iADC - 1]->Write();

    fOut->cd("Digi Distributions/GEM/Without Trigger/Feb Channel Wise/2D/Rectangular/Noise Only/With ADC Cut/Single Masked ADC");
    for (Int_t iADC = 1; iADC < 32; iADC++)
        GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Noise_Only_With_ADC_Cut_Single_Masked[iADC - 1]->Write();

    fOut->cd("Digi Distributions/GEM/Without Trigger/Feb Channel Wise/2D/Rectangular/Noise Only/With ADC Cut/Cumulative");
    for (Int_t iADC = 1; iADC < 31; iADC++)
        GEM_Without_Trigger_Feb_Channel_Wise_2D_Rectangular_Noise_Only_With_ADC_Cut_Cumulative[iADC - 1]->Write();

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
