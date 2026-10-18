// -----------------------------------------------------------------------------
// Copyright (C) Experimental High Energy Physics and Applications Group, VECC, Kolkata
// Draw_FEB_Text_GIF.C
// Project : GEM detector test beam at CERN GIF++ (2023)
// Author  : Apar Agarwal
// Usage   : root -l -b -q 'Draw_FEB_Text_GIF.C'  (see README for input file layout)
// -----------------------------------------------------------------------------

void Highlight_Pad(Int_t Column_number, Int_t Sector_number, TH2F*& PadXandY);
void Gem_Readout_Frame(TH2F*& PadXandY);
void Highlight_RPC_Pad(Int_t Column_number, Int_t Sector_number, TH2F*& PadXandY);
void RPC_Readout_Frame(TH2F*& PadXandY);

void Boundary_Plotter_Gem(Int_t Column_number, Int_t Sector_number, TH2F*& PadXandY, Bool_t left, Bool_t right, Bool_t top, Bool_t bottom);
void Boundary_Plotter_RPC(Int_t Column_number, Int_t Sector_number, TH2F*& PadXandY, Bool_t left, Bool_t right, Bool_t top, Bool_t bottom);

void Draw_Arrow_Left_To_Right(Double_t X1, Double_t X2, Double_t Y, TH2F*& PadXandY);
void Draw_Arrow_Top_To_Bottom(Double_t X, Double_t Y1, Double_t Y2, TH2F*& PadXandY);
void Draw_Feb(Double_t PosX, Double_t PosY, Int_t Feb_no, TH2F*& PadXandY);
void Draw_CROB(Double_t PosX, Double_t PosY, Int_t Feb_no, TH2F*& PadXandY);

void Draw_FEB_Text_GIF(Int_t RunId = 2272)
{
    TH2F* Simple_Hist = new TH2F("Simple_Hist", "Simple Title", 3640, 10, 140, 3280, 40, 110);

    TString Input_X_Mapping_Gem1 = "PadX_GEM.txt";
    TString Input_Y_Mapping_Gem1 = "PadY_GEM.txt";
    TString Input_X_Mapping_Gem2 = "PadX_GEM.txt";
    TString Input_Y_Mapping_Gem2 = "PadY_GEM.txt";

    if (gSystem->AccessPathName(Input_X_Mapping_Gem1))
    {
        cout << "The file: " + Input_X_Mapping_Gem1 + " does not exist!" << endl;
        return;
    }

    if (gSystem->AccessPathName(Input_Y_Mapping_Gem1))
    {
        cout << "The file: " + Input_Y_Mapping_Gem1 + " does not exist!" << endl;
        return;
    }

    if (gSystem->AccessPathName(Input_X_Mapping_Gem2))
    {
        cout << "The file: " + Input_X_Mapping_Gem2 + " does not exist!" << endl;
        return;
    }

    if (gSystem->AccessPathName(Input_Y_Mapping_Gem2))
    {
        cout << "The file: " + Input_Y_Mapping_Gem2 + " does not exist!" << endl;
        return;
    }

    //******************** START: Reading the Mapping of Febs in Gem 1, Gem 2 and mRPC ***************************************

    //****************************** START: Reading Mapping for Gem 1 ********************************************************

    ifstream Gem1_Infile_X;

    Gem1_Infile_X.open(Input_X_Mapping_Gem1);

    Int_t Gem1_Coordinate_Feb_Channel[18][128][2] = {0}; //0 for X and 1 for Y coordinate

    for (Int_t i = 0; i < 18; i++)
    {
        for (Int_t j = 0; j < 128; j++)
        {
            Gem1_Infile_X >> Gem1_Coordinate_Feb_Channel[i][j][0];
        }
    }

    Gem1_Infile_X.close();

    ifstream Gem1_Infile_Y;

    Gem1_Infile_Y.open(Input_Y_Mapping_Gem1);

    for (Int_t i = 0; i < 18; i++)
    {
        for (Int_t j = 0; j < 128; j++)
        {
            Gem1_Infile_Y >> Gem1_Coordinate_Feb_Channel[i][j][1];
        }
    }

    Gem1_Infile_Y.close();

    Int_t Gem1_Feb_no_X_Y[23][97] = {0};
    Int_t Gem1_Ch_no_X_Y[23][97] = {0};

    for (Int_t j = 0; j < 97; j++)
    {
        for (Int_t i = 0; i < 23; i++)
        {
            Gem1_Feb_no_X_Y[i][j] = -1;
            Gem1_Ch_no_X_Y[i][j] = -1;
        }
    }

    for (Int_t i = 0; i < 18; i++)
    {
        for (Int_t j = 0; j < 128; j++)
        {
            if (Gem1_Coordinate_Feb_Channel[i][j][0] >= 0 && Gem1_Coordinate_Feb_Channel[i][j][1] >= 0)
            {
                Gem1_Feb_no_X_Y[Gem1_Coordinate_Feb_Channel[i][j][0]][Gem1_Coordinate_Feb_Channel[i][j][1]] = i;
                Gem1_Ch_no_X_Y[Gem1_Coordinate_Feb_Channel[i][j][0]][Gem1_Coordinate_Feb_Channel[i][j][1]] = j;
            }
        }
    }

    //****************************** END: Reading Mapping for Gem 1 ***********************************************************

    //****************************** START: Reading Mapping for Gem 2 ********************************************************

    ifstream Gem2_Infile_X;

    Gem2_Infile_X.open(Input_X_Mapping_Gem2);

    Int_t Gem2_Coordinate_Feb_Channel[18][128][2] = {0}; //0 for X and 1 for Y coordinate

    for (Int_t i = 0; i < 18; i++)
    {
        for (Int_t j = 0; j < 128; j++)
        {
            Gem2_Infile_X >> Gem2_Coordinate_Feb_Channel[i][j][0];
        }
    }

    Gem2_Infile_X.close();

    ifstream Gem2_Infile_Y;

    Gem2_Infile_Y.open(Input_Y_Mapping_Gem2);

    for (Int_t i = 0; i < 18; i++)
    {
        for (Int_t j = 0; j < 128; j++)
        {
            Gem2_Infile_Y >> Gem2_Coordinate_Feb_Channel[i][j][1];
        }
    }

    Gem2_Infile_Y.close();

    Int_t Gem2_Feb_no_X_Y[23][97] = {0};
    Int_t Gem2_Ch_no_X_Y[23][97] = {0};

    for (Int_t j = 0; j < 97; j++)
    {
        for (Int_t i = 0; i < 23; i++)
        {
            Gem2_Feb_no_X_Y[i][j] = -1;
            Gem2_Ch_no_X_Y[i][j] = -1;
        }
    }

    for (Int_t i = 0; i < 18; i++)
    {
        for (Int_t j = 0; j < 128; j++)
        {
            if (Gem2_Coordinate_Feb_Channel[i][j][0] >= 0 && Gem2_Coordinate_Feb_Channel[i][j][1] >= 0)
            {
                Gem2_Feb_no_X_Y[Gem2_Coordinate_Feb_Channel[i][j][0]][Gem2_Coordinate_Feb_Channel[i][j][1]] = i;
                Gem2_Ch_no_X_Y[Gem2_Coordinate_Feb_Channel[i][j][0]][Gem2_Coordinate_Feb_Channel[i][j][1]] = j;
            }
        }
    }


    //****************************** END: Reading Mapping for Gem 2 ***********************************************************

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

    //******************** END: Reading the Mapping of Febs in Gem 1, Gem 2 and mRPC ******************************************

    Int_t HistColor[18] = {2, 3, 4, 5, 6, 7, 8, 9, 11, 12, 14, 28, 29, 30, 32, 40, 42, 44};

    TH2F* Gem1_Feb_Positions[18];
    TH2F* Gem2_Feb_Positions[18];
    TH2F* mRPC_Feb_Positions[4];

    TH2F* Gem1_CROB_1001_Coverage = new TH2F("Gem1_CROB_1001_Coverage", "Coverage of Area by CROB 1001 in Gem 1", 3640, 10, 140, 3280, 40, 110);
    Gem1_CROB_1001_Coverage->SetMarkerColor(HistColor[0]);
    Gem1_CROB_1001_Coverage->SetFillColor(HistColor[0]);

    TH2F* Gem1_CROB_1002_Coverage = new TH2F("Gem1_CROB_1002_Coverage", "Coverage of Area by CROB 1002 in Gem 1", 3640, 10, 140, 3280, 40, 110);
    Gem1_CROB_1002_Coverage->SetMarkerColor(HistColor[1]);
    Gem1_CROB_1002_Coverage->SetFillColor(HistColor[1]);

    TH2F* Gem1_CROB_1003_Coverage = new TH2F("Gem1_CROB_1003_Coverage", "Coverage of Area by CROB 1003 in Gem 1", 3640, 10, 140, 3280, 40, 110);
    Gem1_CROB_1003_Coverage->SetMarkerColor(HistColor[2]);
    Gem1_CROB_1003_Coverage->SetFillColor(HistColor[2]);

    TH2F* Gem1_Feb_Boundaries_and_Labels = new TH2F("Gem1_Feb_Boundaries_and_Labels", "", 3640, 10, 140, 3280, 40, 110);
    Gem1_Feb_Boundaries_and_Labels->SetTitle("Various Febs in Gem 1 Module");

    TH2F* Gem1_CROB_Boundaries_and_Labels = new TH2F("Gem1_CROB_Boundaries_and_Labels", "", 3640, 10, 140, 3280, 40, 110);
    Gem1_CROB_Boundaries_and_Labels->SetTitle("Various CROBs in Gem 1 Module");

    TH2F* Gem2_CROB_2002_Coverage = new TH2F("Gem2_CROB_2002_Coverage", "Coverage of Area by CROB 2002 in Gem 2", 3640, 10, 140, 3280, 40, 110);
    Gem2_CROB_2002_Coverage->SetMarkerColor(HistColor[0]);
    Gem2_CROB_2002_Coverage->SetFillColor(HistColor[0]);

    TH2F* Gem2_CROB_2003_Coverage = new TH2F("Gem2_CROB_2003_Coverage", "Coverage of Area by CROB 2003 in Gem 2", 3640, 10, 140, 3280, 40, 110);
    Gem2_CROB_2003_Coverage->SetMarkerColor(HistColor[1]);
    Gem2_CROB_2003_Coverage->SetFillColor(HistColor[1]);

    TH2F* Gem2_Feb_Boundaries_and_Labels = new TH2F("Gem2_Feb_Boundaries_and_Labels", "", 3640, 10, 140, 3280, 40, 110);
    Gem2_Feb_Boundaries_and_Labels->SetTitle("Various Febs in GEM Module");

    TH2F* Gem2_CROB_Boundaries_and_Labels = new TH2F("Gem2_CROB_Boundaries_and_Labels", "", 3640, 10, 140, 3280, 40, 110);
    Gem2_CROB_Boundaries_and_Labels->SetTitle("Various CROBs in Gem 2 Module");

    TH2F* mRPC_CROB_1006_Coverage = new TH2F("mRPC_CROB_1006_Coverage", "Coverage of Area by CROB 1006 in mRPC", 4800, -60.0, 60.0, 5200, -30.0, 100.0);
    mRPC_CROB_1006_Coverage->SetMarkerColor(HistColor[0]);
    mRPC_CROB_1006_Coverage->SetFillColor(HistColor[0]);

    TH2F* mRPC_Feb_Boundaries_and_Labels = new TH2F("mRPC_Feb_Boundaries_and_Labels", "", 4800, -60.0, 60.0, 5200, -30.0, 100.0);
    mRPC_Feb_Boundaries_and_Labels->SetTitle("Various Febs in mRPC Module");

    TH2F* mRPC_CROB_Boundaries_and_Labels = new TH2F("mRPC_CROB_Boundaries_and_Labels", "", 4800, -60.0, 60.0, 5200, -30.0, 100.0);
    mRPC_CROB_Boundaries_and_Labels->SetTitle("Various CROBs in mRPC Module");

    // Spiral convention        0  ,  1  ,  2  ,  3  ,  4  ,  5  ,  6  ,  7  ,  8  ,  9  , 10  , 11  , 12  , 13  , 14  , 15  , 16  , 17

    Int_t CROBID_Gem1[18] = {-1, -1, -1, 1003, 1003, 1002, -1, 1001, 1001, 1001, 1001, 1001, 1002, 1002, 1001, 1002, 1002, 1001};
    Int_t CROBID_Gem2[18] = {-1, -1, -1, 2003, 2002, 2002, 2002, 2003, 2002, 2002, 2003, 2003, 2003, 2003, 2002, 2002, 2002, 2003};
    Int_t CROBID_RPC[4] = {1006, 1006, 1006, 1006};
    TString HistTitle, HistTag;

    for (Int_t iFeb = 0; iFeb < 18; iFeb++)
    {
        HistTitle = "Position of Various Febs of Gem Module 1 for Run " + to_string(RunId);
        HistTag = "Gem1_Feb_Positions_" + to_string(iFeb);
        Gem1_Feb_Positions[iFeb] = new TH2F(HistTag, HistTitle, 3640, 10, 140, 3280, 40, 110);
        Gem1_Feb_Positions[iFeb]->SetStats(0);
        Gem1_Feb_Positions[iFeb]->SetMarkerColor(HistColor[iFeb]);
        Gem1_Feb_Positions[iFeb]->SetFillColor(HistColor[iFeb]);

        HistTitle = "Position of Various Febs of Gem Module 2 for Run " + to_string(RunId);
        HistTag = "Gem2_Feb_Positions_" + to_string(iFeb);
        Gem2_Feb_Positions[iFeb] = new TH2F(HistTag, HistTitle, 3640, 10, 140, 3280, 40, 110);
        Gem2_Feb_Positions[iFeb]->SetStats(0);
        Gem2_Feb_Positions[iFeb]->SetMarkerColor(HistColor[iFeb]);
        Gem2_Feb_Positions[iFeb]->SetFillColor(HistColor[iFeb]);

        if (iFeb < 4)
        {
            HistTitle = "Position of Various Febs of mRPC for Run " + to_string(RunId);
            HistTag = "mRPC_Feb_Positions_" + to_string(iFeb);
            mRPC_Feb_Positions[iFeb] = new TH2F(HistTag, HistTitle, 4800, -60.0, 60.0, 5200, -30.0, 100.0);
            mRPC_Feb_Positions[iFeb]->SetStats(0);
            mRPC_Feb_Positions[iFeb]->SetMarkerColor(HistColor[iFeb]);
            mRPC_Feb_Positions[iFeb]->SetFillColor(HistColor[iFeb]);
        }
    }


    for (Int_t iSector = 0; iSector < 97; iSector++)
    {
        for (Int_t iChannel = 0; iChannel < 23; iChannel++)
        {
            if (Gem1_Feb_no_X_Y[iChannel][iSector] != -1)
                Highlight_Pad(iChannel, iSector, Gem1_Feb_Positions[Gem1_Feb_no_X_Y[iChannel][iSector]]);

            Bool_t top = false;
            Bool_t bottom = false;
            Bool_t left = false;
            Bool_t right = false;

            if (iSector == 0)
                top = true;
            else if (Gem1_Feb_no_X_Y[iChannel][iSector] != Gem1_Feb_no_X_Y[iChannel][iSector - 1])
                top = true;

            if (iSector == 96)
                bottom = true;
            else if (Gem1_Feb_no_X_Y[iChannel][iSector] != Gem1_Feb_no_X_Y[iChannel][iSector + 1])
                bottom = true;

            if (iChannel == 0)
                right = true;
            else if (Gem1_Feb_no_X_Y[iChannel][iSector] != Gem1_Feb_no_X_Y[iChannel - 1][iSector])
                right = true;

            if (iChannel == 22)
                left = true;
            else if (Gem1_Feb_no_X_Y[iChannel][iSector] != Gem1_Feb_no_X_Y[iChannel + 1][iSector])
            {
                left = true;
                //cout<<iChannel<<"\t"<<iSector<<"\t"<<Gem1_Feb_no_X_Y[iChannel][iSector]<<"\t"<<Gem1_Feb_no_X_Y[iChannel+1][iSector]<<endl;
            }

            Boundary_Plotter_Gem(iChannel, iSector, Gem1_Feb_Boundaries_and_Labels, left, right, top, bottom);
        }
    }


    for (Int_t iSector = 0; iSector < 97; iSector++)
    {
        for (Int_t iChannel = 0; iChannel < 23; iChannel++)
        {
            if (CROBID_Gem1[Gem1_Feb_no_X_Y[iChannel][iSector]] != -1)
            {
                if (CROBID_Gem1[Gem1_Feb_no_X_Y[iChannel][iSector]] == 1001)
                {
                    Highlight_Pad(iChannel, iSector, Gem1_CROB_1001_Coverage);
                }
                else if (CROBID_Gem1[Gem1_Feb_no_X_Y[iChannel][iSector]] == 1002)
                {
                    Highlight_Pad(iChannel, iSector, Gem1_CROB_1002_Coverage);
                }
                else if (CROBID_Gem1[Gem1_Feb_no_X_Y[iChannel][iSector]] == 1003)
                {
                    Highlight_Pad(iChannel, iSector, Gem1_CROB_1003_Coverage);
                }
            }

            Bool_t top = false;
            Bool_t bottom = false;
            Bool_t left = false;
            Bool_t right = false;

            if (iSector == 0)
                top = true;
            else if (CROBID_Gem1[Gem1_Feb_no_X_Y[iChannel][iSector]] != CROBID_Gem1[Gem1_Feb_no_X_Y[iChannel][iSector - 1]])
                top = true;

            if (iSector == 96)
                bottom = true;
            else if (CROBID_Gem1[Gem1_Feb_no_X_Y[iChannel][iSector]] != CROBID_Gem1[Gem1_Feb_no_X_Y[iChannel][iSector + 1]])
                bottom = true;

            if (iChannel == 0)
                right = true;
            else if (CROBID_Gem1[Gem1_Feb_no_X_Y[iChannel][iSector]] != CROBID_Gem1[Gem1_Feb_no_X_Y[iChannel - 1][iSector]])
                right = true;

            if (iChannel == 22)
                left = true;
            else if (CROBID_Gem1[Gem1_Feb_no_X_Y[iChannel][iSector]] != CROBID_Gem1[Gem1_Feb_no_X_Y[iChannel + 1][iSector]])
                left = true;

            Boundary_Plotter_Gem(iChannel, iSector, Gem1_CROB_Boundaries_and_Labels, left, right, top, bottom);
        }
    }


    for (Int_t iSector = 0; iSector < 97; iSector++)
    {
        for (Int_t iChannel = 0; iChannel < 23; iChannel++)
        {
            if (Gem2_Feb_no_X_Y[iChannel][iSector] != -1)
                Highlight_Pad(iChannel, iSector, Gem2_Feb_Positions[Gem2_Feb_no_X_Y[iChannel][iSector]]);

            Bool_t top = false;
            Bool_t bottom = false;
            Bool_t left = false;
            Bool_t right = false;

            if (iSector == 0)
                top = true;
            else if (Gem2_Feb_no_X_Y[iChannel][iSector] != Gem2_Feb_no_X_Y[iChannel][iSector - 1])
                top = true;

            if (iSector == 96)
                bottom = true;
            else if (Gem2_Feb_no_X_Y[iChannel][iSector] != Gem2_Feb_no_X_Y[iChannel][iSector + 1])
                bottom = true;

            if (iChannel == 0)
                right = true;
            else if (Gem2_Feb_no_X_Y[iChannel][iSector] != Gem2_Feb_no_X_Y[iChannel - 1][iSector])
                right = true;

            if (iChannel == 22)
                left = true;
            else if (Gem2_Feb_no_X_Y[iChannel][iSector] != Gem2_Feb_no_X_Y[iChannel + 1][iSector])
                left = true;

            Boundary_Plotter_Gem(iChannel, iSector, Gem2_Feb_Boundaries_and_Labels, left, right, top, bottom);
        }
    }

    for (Int_t iSector = 0; iSector < 97; iSector++)
    {
        for (Int_t iChannel = 0; iChannel < 23; iChannel++)
        {
            if (CROBID_Gem2[Gem2_Feb_no_X_Y[iChannel][iSector]] != -1)
            {
                if (CROBID_Gem2[Gem2_Feb_no_X_Y[iChannel][iSector]] == 2002)
                {
                    Highlight_Pad(iChannel, iSector, Gem2_CROB_2002_Coverage);
                }
                else if (CROBID_Gem2[Gem2_Feb_no_X_Y[iChannel][iSector]] == 2003)
                {
                    Highlight_Pad(iChannel, iSector, Gem2_CROB_2003_Coverage);
                }
            }

            Bool_t top = false;
            Bool_t bottom = false;
            Bool_t left = false;
            Bool_t right = false;

            if (iSector == 0)
                top = true;
            else if (CROBID_Gem2[Gem2_Feb_no_X_Y[iChannel][iSector]] != CROBID_Gem2[Gem2_Feb_no_X_Y[iChannel][iSector - 1]])
                top = true;

            if (iSector == 96)
                bottom = true;
            else if (CROBID_Gem2[Gem2_Feb_no_X_Y[iChannel][iSector]] != CROBID_Gem2[Gem2_Feb_no_X_Y[iChannel][iSector + 1]])
                bottom = true;

            if (iChannel == 0)
                right = true;
            else if (CROBID_Gem2[Gem2_Feb_no_X_Y[iChannel][iSector]] != CROBID_Gem2[Gem2_Feb_no_X_Y[iChannel - 1][iSector]])
                right = true;

            if (iChannel == 22)
                left = true;
            else if (CROBID_Gem2[Gem2_Feb_no_X_Y[iChannel][iSector]] != CROBID_Gem2[Gem2_Feb_no_X_Y[iChannel + 1][iSector]])
                left = true;

            Boundary_Plotter_Gem(iChannel, iSector, Gem2_CROB_Boundaries_and_Labels, left, right, top, bottom);
        }
    }

    for (Int_t iChannel = 0; iChannel < 10; iChannel++)
    {
        for (Int_t iSector = 0; iSector < 46; iSector++)
        {
            if (RPC_Feb_no_X_Y[iChannel][iSector] != -1)
                Highlight_RPC_Pad(iChannel, iSector, mRPC_Feb_Positions[RPC_Feb_no_X_Y[iChannel][iSector]]);

            Bool_t top = false;
            Bool_t bottom = false;
            Bool_t left = false;
            Bool_t right = false;

            if (iSector == 0)
                top = true;
            else if (RPC_Feb_no_X_Y[iChannel][iSector] != RPC_Feb_no_X_Y[iChannel][iSector - 1])
                top = true;

            if (iSector == 45)
                bottom = true;
            else if (RPC_Feb_no_X_Y[iChannel][iSector] != RPC_Feb_no_X_Y[iChannel][iSector + 1])
                bottom = true;

            if (iChannel == 0)
                left = true;
            else if (RPC_Feb_no_X_Y[iChannel][iSector] != RPC_Feb_no_X_Y[iChannel - 1][iSector])
                left = true;

            if (iChannel == 9)
                right = true;
            else if (RPC_Feb_no_X_Y[iChannel][iSector] != RPC_Feb_no_X_Y[iChannel + 1][iSector])
                right = true;

            Boundary_Plotter_RPC(iChannel, iSector, mRPC_Feb_Boundaries_and_Labels, left, right, top, bottom);
        }
    }

    for (Int_t iChannel = 0; iChannel < 10; iChannel++)
    {
        for (Int_t iSector = 0; iSector < 46; iSector++)
        {
            if (CROBID_RPC[RPC_Feb_no_X_Y[iChannel][iSector]] != -1)
            {
                if (CROBID_RPC[RPC_Feb_no_X_Y[iChannel][iSector]] == 1006)
                {
                    Highlight_RPC_Pad(iChannel, iSector, mRPC_CROB_1006_Coverage);
                }
            }

            Bool_t top = false;
            Bool_t bottom = false;
            Bool_t left = false;
            Bool_t right = false;

            if (iSector == 0)
                top = true;
            else if (CROBID_RPC[RPC_Feb_no_X_Y[iChannel][iSector]] != CROBID_RPC[RPC_Feb_no_X_Y[iChannel][iSector - 1]])
                top = true;

            if (iSector == 45)
                bottom = true;
            else if (CROBID_RPC[RPC_Feb_no_X_Y[iChannel][iSector]] != CROBID_RPC[RPC_Feb_no_X_Y[iChannel][iSector + 1]])
                bottom = true;

            if (iChannel == 0)
                left = true;
            else if (CROBID_RPC[RPC_Feb_no_X_Y[iChannel][iSector]] != CROBID_RPC[RPC_Feb_no_X_Y[iChannel - 1][iSector]])
                left = true;

            if (iChannel == 9)
                right = true;
            else if (CROBID_RPC[RPC_Feb_no_X_Y[iChannel][iSector]] != CROBID_RPC[RPC_Feb_no_X_Y[iChannel + 1][iSector]])
                right = true;

            Boundary_Plotter_RPC(iChannel, iSector, mRPC_CROB_Boundaries_and_Labels, left, right, top, bottom);
        }
    }

    Draw_Arrow_Left_To_Right(18.0, 25.0, 55.0, Gem1_Feb_Boundaries_and_Labels);
    Draw_Feb(25.5, 54.0, 0, Gem1_Feb_Boundaries_and_Labels);
    // t.DrawLatex(25.5,54,"Feb 0");
    // TArrow *ar_Gem1_Feb0 = new TArrow(18.0,55.0,25.0,55.0,0.005,"<");

    Draw_Arrow_Left_To_Right(-27.0, 11.6, 55.0, Gem1_Feb_Boundaries_and_Labels);
    Draw_Feb(-35.5, 54.0, 1, Gem1_Feb_Boundaries_and_Labels);

    // TArrow *ar_Gem1_Feb1 = new TArrow(-30.0,55.0,11.6,55.0,0.005,">");
    // ar_Gem1_Feb1->Draw();
    // t.DrawLatex(-35.5,54,"Feb 1");

    Draw_Arrow_Left_To_Right(18.0, 25.0, 51.0, Gem1_Feb_Boundaries_and_Labels);
    Draw_Feb(25.5, 50.0, 2, Gem1_Feb_Boundaries_and_Labels);

    // TArrow *ar_Gem1_Feb2 = new TArrow(18.0,52.0,25.0,52.0,0.005,"<");
    // ar_Gem1_Feb2->Draw();
    // t.DrawLatex(25.5,51.0,"Feb 2");

    Draw_Arrow_Left_To_Right(18.0, 25.0, 47.0, Gem1_Feb_Boundaries_and_Labels);
    Draw_Feb(25.5, 46.0, 3, Gem1_Feb_Boundaries_and_Labels);

    // TArrow *ar_Gem1_Feb3 = new TArrow(18.0,47.0,25.0,47.0,0.005,"<");
    // ar_Gem1_Feb3->Draw();
    // t.DrawLatex(25.5,46.0,"Feb 3");

    Draw_Arrow_Left_To_Right(-27.0, 14.0, 45.1, Gem1_Feb_Boundaries_and_Labels);
    Draw_Feb(-35.5, 44.0, 4, Gem1_Feb_Boundaries_and_Labels);

    // TArrow *ar_Gem1_Feb4 = new TArrow(-30.0,45.0,14.0,45.0,0.005,">");
    // ar_Gem1_Feb4->Draw();
    // t.DrawLatex(-35.5,44.0,"Feb 4");

    Draw_Arrow_Left_To_Right(-27.0, 10.0, 50.0, Gem1_Feb_Boundaries_and_Labels);
    Draw_Feb(-35.5, 49.0, 5, Gem1_Feb_Boundaries_and_Labels);

    // TArrow *ar_Gem1_Feb5 = new TArrow(-30.0,50.0,10.0,50.0,0.005,">");
    // ar_Gem1_Feb5->Draw();
    // t.DrawLatex(-35.5,49.0,"Feb 5");

    Draw_Arrow_Left_To_Right(-27.0, 6.0, 40.0, Gem1_Feb_Boundaries_and_Labels);
    Draw_Feb(-35.5, 39.0, 6, Gem1_Feb_Boundaries_and_Labels);

    // TArrow *ar_Gem1_Feb6 = new TArrow(-30.0,40.0,6.0,40.0,0.005,">");
    // ar_Gem1_Feb6->Draw();
    // t.DrawLatex(-35.5,39.0,"Feb 6");

    Draw_Arrow_Left_To_Right(-27.0, 5.0, 25.0, Gem1_Feb_Boundaries_and_Labels);
    Draw_Feb(-35.5, 24.0, 7, Gem1_Feb_Boundaries_and_Labels);

    // TArrow *ar_Gem1_Feb7 = new TArrow(-30.0,25.0,5.0,25.0,0.005,">");
    // ar_Gem1_Feb7->Draw();
    // t.DrawLatex(-35.5,24.0,"Feb 7");

    Draw_Arrow_Left_To_Right(-27.0, 12.1, 35.0, Gem1_Feb_Boundaries_and_Labels);
    Draw_Feb(-35.5, 34.0, 8, Gem1_Feb_Boundaries_and_Labels);

    // TArrow *ar_Gem1_Feb8 = new TArrow(-30.0,35.0,12.1,35.0,0.005,">");
    // ar_Gem1_Feb8->Draw();
    // t.DrawLatex(-35.5,34.0,"Feb 8");

    Draw_Arrow_Left_To_Right(18.0, 25.0, 34.0, Gem1_Feb_Boundaries_and_Labels);
    Draw_Feb(25.5, 33.0, 9, Gem1_Feb_Boundaries_and_Labels);

    // TArrow *ar_Gem1_Feb9 = new TArrow(18.0,34.0,25.0,34.0,0.005,"<");
    // ar_Gem1_Feb9->Draw();
    // t.DrawLatex(25.5,33.0,"Feb 9");

    Draw_Arrow_Left_To_Right(18.0, 25.0, 25.0, Gem1_Feb_Boundaries_and_Labels);
    Draw_Feb(25.5, 24.0, 10, Gem1_Feb_Boundaries_and_Labels);

    // TArrow *ar_Gem1_Feb10 = new TArrow(18.0,25.0,25.0,25.0,0.005,"<");
    // ar_Gem1_Feb10->Draw();
    // t.DrawLatex(25.5,24.0,"Feb 10");

    Draw_Arrow_Left_To_Right(-25.0, 9.0, 12.0, Gem1_Feb_Boundaries_and_Labels);
    Draw_Feb(-36.0, 11.0, 11, Gem1_Feb_Boundaries_and_Labels);

    // TArrow *ar_Gem1_Feb11 = new TArrow(-30.0,12.0,9.0,12.0,0.005,">");
    // ar_Gem1_Feb11->Draw();
    // t.DrawLatex(-36,11.0,"Feb 11");

    Draw_Arrow_Left_To_Right(-25.0, 4.0, 8.0, Gem1_Feb_Boundaries_and_Labels);
    Draw_Feb(-36, 7.0, 12, Gem1_Feb_Boundaries_and_Labels);

    // TArrow *ar_Gem1_Feb12 = new TArrow(-30.0,8.0,4.0,8.0,0.005,">");
    // ar_Gem1_Feb12->Draw();
    // t.DrawLatex(-36,7.0,"Feb 12");

    Draw_Arrow_Left_To_Right(-25.0, 4.0, 30.0, Gem1_Feb_Boundaries_and_Labels);
    Draw_Feb(-36, 29.0, 13, Gem1_Feb_Boundaries_and_Labels);

    // TArrow *ar_Gem1_Feb13_0 = new TArrow(-30.0,30.0,4.0,30.0,0.005,">");
    // ar_Gem1_Feb13_0->Draw();
    // t.DrawLatex(-36,29.0,"Feb 13");

    Draw_Arrow_Left_To_Right(-25.0, -2.5, -0.5, Gem1_Feb_Boundaries_and_Labels);
    Draw_Feb(-36, -1.5, 13, Gem1_Feb_Boundaries_and_Labels);

    // TArrow *ar_Gem1_Feb13_1 = new TArrow(-30.0,-0.5,-2.5,-0.5,0.005,">");
    // ar_Gem1_Feb13_1->Draw();
    // t.DrawLatex(-36,-1.5,"Feb 13");

    Draw_Arrow_Left_To_Right(-25.0, -2.0, -10.0, Gem1_Feb_Boundaries_and_Labels);
    Draw_Feb(-36, -11, 13, Gem1_Feb_Boundaries_and_Labels);

    // TArrow *ar_Gem1_Feb13_2 = new TArrow(-30.0,-10.0,-2.0,-10.0,0.005,">");
    // ar_Gem1_Feb13_2->Draw();
    // t.DrawLatex(-36,-11,"Feb 13");

    Draw_Arrow_Left_To_Right(-25.0, -2.0, 20.0, Gem1_Feb_Boundaries_and_Labels);
    Draw_Feb(-36, 19, 14, Gem1_Feb_Boundaries_and_Labels);

    // TArrow *ar_Gem1_Feb14_0 = new TArrow(-30.0,20.0,-2.0,20.0,0.005,">");
    // ar_Gem1_Feb14_0->Draw();
    // t.DrawLatex(-36,19,"Feb 14");

    Draw_Arrow_Left_To_Right(-25.0, -4.5, 3.01, Gem1_Feb_Boundaries_and_Labels);
    Draw_Feb(-36, 2.0, 14, Gem1_Feb_Boundaries_and_Labels);

    // TArrow *ar_Gem1_Feb14_1 = new TArrow(-30.0,3.0,-4.5,3.0,0.005,">");
    // ar_Gem1_Feb14_1->Draw();
    // t.DrawLatex(-36,2.0,"Feb 14");

    Draw_Arrow_Left_To_Right(-25.0, -15.0, -15.0, Gem1_Feb_Boundaries_and_Labels);
    Draw_Feb(-36, -16.0, 15, Gem1_Feb_Boundaries_and_Labels);

    // TArrow *ar_Gem1_Feb15 = new TArrow(-30.0,-15.0,-15.0,-15.0,0.005,">");
    // ar_Gem1_Feb15->Draw();
    // t.DrawLatex(-36,-16.0,"Feb 15");

    Draw_Arrow_Left_To_Right(-25.0, -10.0, -5.0, Gem1_Feb_Boundaries_and_Labels);
    Draw_Feb(-36, -6.0, 16, Gem1_Feb_Boundaries_and_Labels);

    // TArrow *ar_Gem1_Feb16_0 = new TArrow(-30.0,-5.0,-10.0,-5.0,0.005,">");
    // ar_Gem1_Feb16_0->Draw();
    // t.DrawLatex(-36,-6.0,"Feb 16");

    Draw_Arrow_Left_To_Right(-25.0, 4.0, -20.0, Gem1_Feb_Boundaries_and_Labels);
    Draw_Feb(-36, -21.0, 16, Gem1_Feb_Boundaries_and_Labels);

    // TArrow *ar_Gem1_Feb16_1 = new TArrow(-30.0,-20.0,4.0,-20.0,0.005,">");
    // ar_Gem1_Feb16_1->Draw();
    // t.DrawLatex(-36,-21.0,"Feb 16");

    Draw_Arrow_Left_To_Right(18.0, 25.0, -10.0, Gem1_Feb_Boundaries_and_Labels);
    Draw_Feb(25.5, -11.0, 17, Gem1_Feb_Boundaries_and_Labels);


    Draw_Arrow_Left_To_Right(34.0, 39.0, 75.5, Gem2_Feb_Boundaries_and_Labels);
    Draw_Feb(25.5, 74.0, 0, Gem2_Feb_Boundaries_and_Labels);

    Draw_Arrow_Left_To_Right(22.5, 41.5, 73.5, Gem2_Feb_Boundaries_and_Labels);
    Draw_Feb(14.0, 72.0, 1, Gem2_Feb_Boundaries_and_Labels);

    Draw_Arrow_Left_To_Right(22.5, 41.5, 77.5, Gem2_Feb_Boundaries_and_Labels);
    Draw_Feb(14.0, 76.0, 2, Gem2_Feb_Boundaries_and_Labels);

    Draw_Arrow_Left_To_Right(34.0, 45.0, 71.5, Gem2_Feb_Boundaries_and_Labels);
    Draw_Feb(25.5, 70.0, 3, Gem2_Feb_Boundaries_and_Labels);

    Draw_Arrow_Top_To_Bottom(53.0, 75.0, 60.0, Gem2_Feb_Boundaries_and_Labels);
    Draw_Arrow_Top_To_Bottom(54.0, 73.4, 60.0, Gem2_Feb_Boundaries_and_Labels);
    Draw_Feb(49, 55.0, 4, Gem2_Feb_Boundaries_and_Labels);

    Draw_Arrow_Top_To_Bottom(48.2, 89.0, 78.2, Gem2_Feb_Boundaries_and_Labels);
    Draw_Feb(44.0, 90.0, 4, Gem2_Feb_Boundaries_and_Labels);

    Draw_Arrow_Left_To_Right(34.0, 45.0, 79.5, Gem2_Feb_Boundaries_and_Labels);
    Draw_Feb(25.5, 78.0, 5, Gem2_Feb_Boundaries_and_Labels);

    Draw_Arrow_Top_To_Bottom(58.0, 70.0, 50.0, Gem2_Feb_Boundaries_and_Labels);
    Draw_Feb(55.5, 45.5, 6, Gem2_Feb_Boundaries_and_Labels);

    Draw_Arrow_Top_To_Bottom(65.0, 73.4, 60.0, Gem2_Feb_Boundaries_and_Labels);
    Draw_Feb(60, 55.0, 7, Gem2_Feb_Boundaries_and_Labels);

    Draw_Arrow_Top_To_Bottom(60.0, 89.0, 77.5, Gem2_Feb_Boundaries_and_Labels);
    Draw_Arrow_Top_To_Bottom(64.0, 89.0, 80.5, Gem2_Feb_Boundaries_and_Labels);
    Draw_Feb(58.0, 90.0, 8, Gem2_Feb_Boundaries_and_Labels);

    Draw_Arrow_Top_To_Bottom(77.0, 74.0, 60.0, Gem2_Feb_Boundaries_and_Labels);
    Draw_Feb(72.0, 55.0, 8, Gem2_Feb_Boundaries_and_Labels);

    Draw_Arrow_Top_To_Bottom(56.0, 99.0, 81.0, Gem2_Feb_Boundaries_and_Labels);
    Draw_Feb(52.0, 100.0, 9, Gem2_Feb_Boundaries_and_Labels);

    Draw_Arrow_Top_To_Bottom(74.0, 89.0, 78.1, Gem2_Feb_Boundaries_and_Labels);
    Draw_Arrow_Top_To_Bottom(76.5, 89.0, 78.1, Gem2_Feb_Boundaries_and_Labels);
    Draw_Arrow_Top_To_Bottom(78.2, 89.0, 78.1, Gem2_Feb_Boundaries_and_Labels);
    Draw_Feb(72.0, 90.0, 9, Gem2_Feb_Boundaries_and_Labels);

    Draw_Arrow_Top_To_Bottom(71.0, 66.0, 50.0, Gem2_Feb_Boundaries_and_Labels);
    Draw_Feb(67.5, 45.5, 10, Gem2_Feb_Boundaries_and_Labels);

    Draw_Arrow_Top_To_Bottom(87.5, 69.0, 59.0, Gem2_Feb_Boundaries_and_Labels);
    Draw_Feb(82.5, 55.0, 11, Gem2_Feb_Boundaries_and_Labels);

    Draw_Arrow_Top_To_Bottom(93.0, 83.0, 50.0, Gem2_Feb_Boundaries_and_Labels);
    Draw_Arrow_Top_To_Bottom(94.0, 73.0, 50.0, Gem2_Feb_Boundaries_and_Labels);
    Draw_Feb(89.0, 45.5, 12, Gem2_Feb_Boundaries_and_Labels);

    Draw_Arrow_Top_To_Bottom(68.5, 99.0, 80.0, Gem2_Feb_Boundaries_and_Labels);
    Draw_Feb(63.5, 100, 13, Gem2_Feb_Boundaries_and_Labels);

    Draw_Arrow_Top_To_Bottom(83.0, 99.0, 78.0, Gem2_Feb_Boundaries_and_Labels);
    Draw_Feb(78.0, 100, 13, Gem2_Feb_Boundaries_and_Labels);

    Draw_Arrow_Top_To_Bottom(90.0, 94.0, 88.7, Gem2_Feb_Boundaries_and_Labels);
    Draw_Feb(85.0, 95, 14, Gem2_Feb_Boundaries_and_Labels);

    Draw_Arrow_Left_To_Right(115.0, 124.0, 86.5, Gem2_Feb_Boundaries_and_Labels);
    Draw_Feb(125.0, 85.0, 15, Gem2_Feb_Boundaries_and_Labels);

    Draw_Arrow_Left_To_Right(115.0, 124.0, 74.5, Gem2_Feb_Boundaries_and_Labels);
    Draw_Feb(125.0, 73.0, 16, Gem2_Feb_Boundaries_and_Labels);

    Draw_Arrow_Left_To_Right(115.0, 124.0, 61.5, Gem2_Feb_Boundaries_and_Labels);
    Draw_Feb(125.0, 60.0, 17, Gem2_Feb_Boundaries_and_Labels);


    // TArrow *ar_Gem1_Feb17 = new TArrow(18.0,-10.0,25.0,-10.0,0.005,"<");
    // ar_Gem1_Feb17->Draw();
    // t.DrawLatex(25.5,-11.0,"Feb 17");

    // Gem1_Feb_Boundaries_and_Labels->Draw();
    Gem2_Feb_Boundaries_and_Labels->Draw();
}

void Highlight_Pad(Int_t Column_number, Int_t Sector_number, TH2F*& PadXandY)
{
    if (Sector_number >= 0 && Column_number >= 0)
    {
        Float_t Radial_Coordinate_initial = 18.30, Radial_Coordinate_final = 18.3, Theta_Coordinate_initial, Theta_Coordinate_final;
        //   Theta_Coordinate_initial=((Float_t) Column_number);
        //   Theta_Coordinate_final=((Float_t) (Column_number+1));
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

void Gem_Readout_Frame(TH2F*& PadXandY)
{
    Float_t Radial_Coordinate = 18.30, Theta_Coordinate = 0.0;
    Float_t X_Coordinate, Y_Coordinate;
    Float_t X_offset = -19.4103, Y_offset = 75.1912;
    Float_t Pi = 3.1468;
    Int_t i = 0;

    for (i = 0; i < 98; i++)
    {
        for (Theta_Coordinate = 0.0000; Theta_Coordinate < 23.0001; Theta_Coordinate += 0.02)
        {
            X_Coordinate = Radial_Coordinate * TMath::Sin(Theta_Coordinate * 3.14159 / 180.0);
            Y_Coordinate = -1.0 * Radial_Coordinate * TMath::Cos(Theta_Coordinate * 3.14159 / 180.0);
            PadXandY->Fill(-X_offset - X_Coordinate, Y_offset + Y_Coordinate);
        }
        Radial_Coordinate += Radial_Coordinate * Pi / 180;
    }

    for (Theta_Coordinate = 0.0000; Theta_Coordinate < 23.0100; Theta_Coordinate += 1.0000)
    {
        for (Radial_Coordinate = 18.3000; Radial_Coordinate < 98.301; Radial_Coordinate += 0.02)
        {
            X_Coordinate = Radial_Coordinate * TMath::Sin(Theta_Coordinate * 3.14159 / 180.0);
            Y_Coordinate = -1.0 * Radial_Coordinate * TMath::Cos(Theta_Coordinate * 3.14159 / 180.0);
            PadXandY->Fill(-X_offset - X_Coordinate, Y_offset + Y_Coordinate);
        }
    }
}

void Highlight_RPC_Pad(Int_t Column_number, Int_t Sector_number, TH2F*& PadXandY)
{
    if (Sector_number >= 0 && Column_number >= 0)
    {
        Float_t Radial_Coordinate_initial = 29.056, Radial_Coordinate_final = 29.056, Theta_Coordinate_initial, Theta_Coordinate_final;
        Theta_Coordinate_initial = ((Float_t)Column_number * 2.0 - 10.0);
        Theta_Coordinate_final = ((Float_t)Column_number * 2.0 - 8.0);
        Float_t Pi = 3.17585;
        Float_t X_offset = 0.0, Y_offset = -50.0;
        Float_t X_Coordinate, Y_Coordinate;

        Float_t Theta_increment;

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
            if (i == Sector_number)
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

void RPC_Readout_Frame(TH2F*& PadXandY)
{
    Float_t Radial_Coordinate = 29.056, Theta_Coordinate = 0.0;
    Float_t X_Coordinate, Y_Coordinate;
    Float_t X_offset = 0.0, Y_offset = -50.0;
    Float_t Pi = 3.17585;
    Int_t i = 0;

    Int_t Radial_Coordinate_initial = Radial_Coordinate;
    Int_t Radial_Coordinate_final = Radial_Coordinate;

    for (Int_t i = 0; i < 46; i++)
    {
        Radial_Coordinate_final = Radial_Coordinate_initial + Radial_Coordinate_initial * Pi / 90.0;

        //  cout<< Radial_Coordinate_initial << "   " <<Radial_Coordinate_final << "   " << Radial_Coordinate_final - Radial_Coordinate_initial <<endl;

        Radial_Coordinate_initial = Radial_Coordinate_final;
    }

    Radial_Coordinate_initial = Radial_Coordinate;

    for (i = 0; i < 47; i++)
    {
        for (Theta_Coordinate = -10.0000; Theta_Coordinate < 20.0001; Theta_Coordinate += 0.02)
        {
            X_Coordinate = Radial_Coordinate * TMath::Sin(Theta_Coordinate * 3.14159 / 180.0);
            Y_Coordinate = Radial_Coordinate * TMath::Cos(Theta_Coordinate * 3.14159 / 180.0);
            PadXandY->Fill(X_offset + X_Coordinate, Y_offset + Y_Coordinate);
        }
        Radial_Coordinate += Radial_Coordinate * Pi / 90.0;
    }

    for (Theta_Coordinate = -10.0000; Theta_Coordinate < 10.0100; Theta_Coordinate += 2.0000)
    {
        for (Radial_Coordinate = Radial_Coordinate_initial; Radial_Coordinate < Radial_Coordinate_final; Radial_Coordinate += 0.02)
        {
            X_Coordinate = Radial_Coordinate * TMath::Sin(Theta_Coordinate * 3.14159 / 180.0);
            Y_Coordinate = Radial_Coordinate * TMath::Cos(Theta_Coordinate * 3.14159 / 180.0);
            PadXandY->Fill(X_offset + X_Coordinate, Y_offset + Y_Coordinate);
        }
    }
}

void Boundary_Plotter_Gem(Int_t Column_number, Int_t Sector_number, TH2F*& PadXandY, Bool_t left, Bool_t right, Bool_t top, Bool_t bottom)
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

void Boundary_Plotter_RPC(Int_t Column_number, Int_t Sector_number, TH2F*& PadXandY, Bool_t left, Bool_t right, Bool_t top, Bool_t bottom)
{
    Float_t Radial_Coordinate, Radial_Coordinate_initial = 29.056, Radial_Coordinate_final = 29.056, Theta_Coordinate, Theta_Coordinate_initial, Theta_Coordinate_final;
    Theta_Coordinate_initial = ((Float_t)Column_number * 2.0 - 10.0);
    Theta_Coordinate_final = ((Float_t)Column_number * 2.0 - 8.0);
    Float_t Pi = 3.17585;
    Float_t X_offset = 0.0, Y_offset = -50.0;
    Float_t X_Coordinate, Y_Coordinate;

    for (Int_t i = 0; i < 46; i++)
    {
        Radial_Coordinate_final = Radial_Coordinate_initial + Radial_Coordinate_initial * Pi / 90.0;

        //  cout<< Radial_Coordinate_initial << "   " <<Radial_Coordinate_final << "   " << Radial_Coordinate_final - Radial_Coordinate_initial <<endl;
        if (i == Sector_number)
            break;

        Radial_Coordinate_initial = Radial_Coordinate_final;
    }

    if (top)
    {
        for (Theta_Coordinate = Theta_Coordinate_initial; Theta_Coordinate < Theta_Coordinate_final + 0.01; Theta_Coordinate += 0.002)
        {
            X_Coordinate = Radial_Coordinate_initial * TMath::Sin(Theta_Coordinate * 3.14159 / 180.0);
            Y_Coordinate = Radial_Coordinate_initial * TMath::Cos(Theta_Coordinate * 3.14159 / 180.0);
            PadXandY->Fill(X_offset + X_Coordinate, Y_offset + Y_Coordinate);
        }
    }

    if (bottom)
    {
        for (Theta_Coordinate = Theta_Coordinate_initial; Theta_Coordinate < Theta_Coordinate_final + 0.01; Theta_Coordinate += 0.002)
        {
            X_Coordinate = Radial_Coordinate_final * TMath::Sin(Theta_Coordinate * 3.14159 / 180.0);
            Y_Coordinate = Radial_Coordinate_final * TMath::Cos(Theta_Coordinate * 3.14159 / 180.0);
            PadXandY->Fill(X_offset + X_Coordinate, Y_offset + Y_Coordinate);
        }
    }

    if (left)
    {
        Theta_Coordinate = Theta_Coordinate_initial;

        for (Radial_Coordinate = Radial_Coordinate_initial; Radial_Coordinate < Radial_Coordinate_final + 0.01; Radial_Coordinate += 0.002)
        {
            X_Coordinate = Radial_Coordinate * TMath::Sin(Theta_Coordinate * 3.14159 / 180.0);
            Y_Coordinate = Radial_Coordinate * TMath::Cos(Theta_Coordinate * 3.14159 / 180.0);
            PadXandY->Fill(X_offset + X_Coordinate, Y_offset + Y_Coordinate);
        }
    }

    if (right)
    {
        Theta_Coordinate = Theta_Coordinate_final;

        for (Radial_Coordinate = Radial_Coordinate_initial; Radial_Coordinate < Radial_Coordinate_final + 0.01; Radial_Coordinate += 0.002)
        {
            X_Coordinate = Radial_Coordinate * TMath::Sin(Theta_Coordinate * 3.14159 / 180.0);
            Y_Coordinate = Radial_Coordinate * TMath::Cos(Theta_Coordinate * 3.14159 / 180.0);
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

void Draw_CROB(Double_t PosX, Double_t PosY, Int_t Feb_no, TH2F*& PadXandY)
{
}
