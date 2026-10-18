// -----------------------------------------------------------------------------
// Copyright (C) Experimental High Energy Physics and Applications Group, VECC, Kolkata
// Draw_Gem_Readout_Feb_Positions.C
// Project : GEM detector test beam at CERN GIF++ (2023)
// Author  : Apar Agarwal
// Usage   : root -l -b -q 'Draw_Gem_Readout_Feb_Positions.C'  (see README for input file layout)
// -----------------------------------------------------------------------------

void Draw_Gem_Readout_Feb_Positions(Int_t RunId = 2186)
{
    TString Gem_Feb_Image = "basic pad.png";
    TImage* Gem_back_image = TImage::Open(Gem_Feb_Image);

    Gem_back_image->SetConstRatio(kFALSE);

    TCanvas* cConnector_Gem1_Feb_Unique_Number = new TCanvas("FebGem1UIdPad", "FebGem1UIdPad", 980, 680);
    cConnector_Gem1_Feb_Unique_Number->Divide(2, 1);
    cConnector_Gem1_Feb_Unique_Number->cd(1);
    Gem_back_image->Draw();
    cConnector_Gem1_Feb_Unique_Number->cd(2);
    Gem_back_image->Draw();
    cConnector_Gem1_Feb_Unique_Number->cd();

    TCanvas* cConnector_Gem1_Feb_CROB_Number = new TCanvas("FebGem1CROBPad", "FebGem1CROBPad", 980, 680);
    cConnector_Gem1_Feb_CROB_Number->Divide(2, 1);
    cConnector_Gem1_Feb_CROB_Number->cd(1);
    Gem_back_image->Draw();
    cConnector_Gem1_Feb_CROB_Number->cd(2);
    Gem_back_image->Draw();
    cConnector_Gem1_Feb_CROB_Number->cd();

    TCanvas* cConnector_Gem1_Feb_VRefP_Values = new TCanvas("FebGem1VRefPPad", "FebGem1VRefPPad", 980, 680);
    cConnector_Gem1_Feb_VRefP_Values->Divide(2, 1);
    cConnector_Gem1_Feb_VRefP_Values->cd(1);
    Gem_back_image->Draw();
    cConnector_Gem1_Feb_VRefP_Values->cd(2);
    Gem_back_image->Draw();
    cConnector_Gem1_Feb_VRefP_Values->cd();

    TCanvas* cConnector_Gem2_Feb_Unique_Number = new TCanvas("FebGem2UIdPad", "FebGem2UIdPad", 980, 680);
    cConnector_Gem2_Feb_Unique_Number->Divide(2, 1);
    cConnector_Gem2_Feb_Unique_Number->cd(1);
    Gem_back_image->Draw();
    cConnector_Gem2_Feb_Unique_Number->cd(2);
    Gem_back_image->Draw();
    cConnector_Gem2_Feb_Unique_Number->cd();

    TCanvas* cConnector_Gem2_Feb_CROB_Number = new TCanvas("FebGem2CROBPad", "FebGem2CROBPad", 980, 680);
    cConnector_Gem2_Feb_CROB_Number->Divide(2, 1);
    cConnector_Gem2_Feb_CROB_Number->cd(1);
    Gem_back_image->Draw();
    cConnector_Gem2_Feb_CROB_Number->cd(2);
    Gem_back_image->Draw();
    cConnector_Gem2_Feb_CROB_Number->cd();

    TCanvas* cConnector_Gem2_Feb_VRefP_Values = new TCanvas("FebGem2VRefPPad", "FebGem2VRefPPad", 980, 680);
    cConnector_Gem2_Feb_VRefP_Values->Divide(2, 1);
    cConnector_Gem2_Feb_VRefP_Values->cd(1);
    Gem_back_image->Draw();
    cConnector_Gem2_Feb_VRefP_Values->cd(2);
    Gem_back_image->Draw();
    cConnector_Gem2_Feb_VRefP_Values->cd();

    TString Text_String = "Feb Connector Positions in GEM";
    TText* tTitle_Gem1_Feb_no = new TText(0.5, 1.06, Text_String);
    TText* tGem1_Feb0 = new TText(0.497, 0.779, "FEB 0");   //"Pos 0"  FebUId XXXXX CROB XXXXX        --Only Change Comments
    TText* tGem1_Feb1 = new TText(0.544, 0.634, "FEB 2");   //"Pos 1"  FebUId XXXXX CROB XXXXX         --Only Change Comments
    TText* tGem1_Feb2 = new TText(0.447, 0.657, "FEB 1");   //"Pos 2"  FebUId XXXXX CROB XXXXX        --Only Change Comments
    TText* tGem1_Feb3 = new TText(0.397, 0.537, "FEB 3");   //"Pos 3"  FebUId 162 CROB 1003         --Only Change Comments
    TText* tGem1_Feb4 = new TText(0.499, 0.511, "FEB 4");   //"Pos 4"  FebUId 149 CROB 1003        --Only Change Comments
    TText* tGem1_Feb5 = new TText(0.607, 0.500, "FEB 5");   //"Pos 5"  FebUId 148 CROB 1002         --Only Change Comments
    TText* tGem1_Feb6 = new TText(0.659, 0.356, "FEB 9");   //"Pos 6"  FebUId XXXXX CROB XXXXX         --Only Change Comments
    TText* tGem1_Feb7 = new TText(0.553, 0.379, "FEB 8");   //"Pos 7"  FebUId 171 CROB 1001          --Only Change Comments
    TText* tGem1_Feb8 = new TText(0.445, 0.385, "FEB 7");   //"Pos 8"  FebUId 166 CROB 1001         --Only Change Comments
    TText* tGem1_Feb9 = new TText(0.335, 0.413, "FEB 6");   //"Pos 9"  FebUId 179 CROB 1001         --Only Change Comments
    TText* tGem1_Feb10 = new TText(0.281, 0.276, "FEB 10"); //"Pos 10" FebUId 145 CROB 1001        --Only Change Comments
    TText* tGem1_Feb11 = new TText(0.399, 0.257, "FEB 11"); //"Pos 11" FebUId 173 CROB 1001         --Only Change Comments
    TText* tGem1_Feb12 = new TText(0.500, 0.250, "FEB 12"); //"Pos 12" FebUId 160 CROB 1002         --Only Change Comments
    TText* tGem1_Feb13 = new TText(0.610, 0.231, "FEB 13"); //"Pos 13" FebUId 163 CROB 1002        --Only Change Comments
    TText* tGem1_Feb14 = new TText(0.719, 0.217, "FEB 14"); //"Pos 14" FebUId 178 CROB 1001         --Only Change Comments
    TText* tGem1_Feb15 = new TText(0.670, 0.113, "FEB 15"); //"Pos 15" FebUId 147 CROB 1002          --Only Change Comments
    TText* tGem1_Feb16 = new TText(0.403, 0.113, "FEB 16"); //"Pos 16" FebUId 146 CROB 1002        --Only Change Comments
    TText* tGem1_Feb17 = new TText(0.232, 0.155, "FEB 17"); //"Pos 17" FebUId 151 CROB 1001        --Only Change Comments

    Text_String = "Feb Unique Id in Gem 1 (Run " + to_string(RunId) + ")";
    TText* tTitle_Gem1_Unique_Id_Feb = new TText(0.12, 1.043, Text_String);
    TText* tGem1_Unique_Id_Feb0 = new TText(0.497, 0.779, "XXXXX");    //"Pos 0"  FebUId XXXXX CROB XXXXX
    TText* tGem1_Unique_Id_Feb1 = new TText(0.544, 0.634, "XXXXX");    //"Pos 1"  FebUId XXXXX CROB XXXXX
    TText* tGem1_Unique_Id_Feb2 = new TText(0.447, 0.657, "XXXXX");    //"Pos 2"  FebUId XXXXX CROB XXXXX
    TText* tGem1_Unique_Id_Feb3 = new TText(0.397, 0.537, "Feb 162");  //"Pos 3"  FebUId 162 CROB 1003
    TText* tGem1_Unique_Id_Feb4 = new TText(0.4948, 0.511, "Feb 149"); //"Pos 4"  FebUId 149 CROB 1003
    TText* tGem1_Unique_Id_Feb5 = new TText(0.607, 0.500, "Feb 148");  //"Pos 5"  FebUId 148 CROB 1002
    TText* tGem1_Unique_Id_Feb6 = new TText(0.659, 0.356, "XXXXX");    //"Pos 6"  FebUId XXXXX CROB XXXXX
    TText* tGem1_Unique_Id_Feb7 = new TText(0.553, 0.379, "Feb 171");  //"Pos 7"  FebUId 171 CROB 1001
    TText* tGem1_Unique_Id_Feb8 = new TText(0.445, 0.385, "Feb 166");  //"Pos 8"  FebUId 166 CROB 1001
    TText* tGem1_Unique_Id_Feb9 = new TText(0.335, 0.413, "Feb 179");  //"Pos 9"  FebUId 179 CROB 1001
    TText* tGem1_Unique_Id_Feb10 = new TText(0.281, 0.276, "Feb 145"); //"Pos 10" FebUId 145 CROB 1001
    TText* tGem1_Unique_Id_Feb11 = new TText(0.399, 0.257, "Feb 173"); //"Pos 11" FebUId 173 CROB 1001
    TText* tGem1_Unique_Id_Feb12 = new TText(0.500, 0.250, "Feb 160"); //"Pos 12" FebUId 160 CROB 1002
    TText* tGem1_Unique_Id_Feb13 = new TText(0.610, 0.231, "Feb 163"); //"Pos 13" FebUId 163 CROB 1002
    TText* tGem1_Unique_Id_Feb14 = new TText(0.719, 0.217, "Feb 178"); //"Pos 14" FebUId 178 CROB 1001
    TText* tGem1_Unique_Id_Feb15 = new TText(0.670, 0.113, "Feb 147"); //"Pos 15" FebUId 147 CROB 1002
    TText* tGem1_Unique_Id_Feb16 = new TText(0.403, 0.113, "Feb 146"); //"Pos 16" FebUId 146 CROB 1002
    TText* tGem1_Unique_Id_Feb17 = new TText(0.232, 0.155, "Feb 151"); //"Pos 17" FebUId 151 CROB 1001

    Text_String = "Feb CROB Id in Gem 1 (Run " + to_string(RunId) + ")";
    TText* tTitle_Gem1_CROB_Id_Feb = new TText(0.12, 1.043, Text_String);
    TText* tGem1_CROB_Id_Feb0 = new TText(0.497, 0.779, "XXXXX");      //"Pos 0"  FebUId XXXXX CROB XXXXX
    TText* tGem1_CROB_Id_Feb1 = new TText(0.544, 0.634, "XXXXX");      //"Pos 1"  FebUId XXXXX CROB XXXXX
    TText* tGem1_CROB_Id_Feb2 = new TText(0.447, 0.657, "XXXXX");      //"Pos 2"  FebUId XXXXX CROB XXXXX
    TText* tGem1_CROB_Id_Feb3 = new TText(0.397, 0.537, "CROB 1003");  //"Pos 3"  FebUId 162 CROB 1003
    TText* tGem1_CROB_Id_Feb4 = new TText(0.4948, 0.511, "CROB 1003"); //"Pos 4"  FebUId 149 CROB 1003
    TText* tGem1_CROB_Id_Feb5 = new TText(0.607, 0.500, "CROB 1002");  //"Pos 5"  FebUId 148 CROB 1002
    TText* tGem1_CROB_Id_Feb6 = new TText(0.659, 0.356, "XXXXX");      //"Pos 6"  FebUId XXXXX CROB XXXXX
    TText* tGem1_CROB_Id_Feb7 = new TText(0.553, 0.379, "CROB 1001");  //"Pos 7"  FebUId 171 CROB 1001
    TText* tGem1_CROB_Id_Feb8 = new TText(0.445, 0.385, "CROB 1001");  //"Pos 8"  FebUId 166 CROB 1001
    TText* tGem1_CROB_Id_Feb9 = new TText(0.335, 0.413, "CROB 1001");  //"Pos 9"  FebUId 179 CROB 1001
    TText* tGem1_CROB_Id_Feb10 = new TText(0.281, 0.276, "CROB 1001"); //"Pos 10" FebUId 145 CROB 1001
    TText* tGem1_CROB_Id_Feb11 = new TText(0.399, 0.257, "CROB 1001"); //"Pos 11" FebUId 173 CROB 1001
    TText* tGem1_CROB_Id_Feb12 = new TText(0.500, 0.250, "CROB 1002"); //"Pos 12" FebUId 160 CROB 1002
    TText* tGem1_CROB_Id_Feb13 = new TText(0.610, 0.231, "CROB 1002"); //"Pos 13" FebUId 163 CROB 1002
    TText* tGem1_CROB_Id_Feb14 = new TText(0.719, 0.217, "CROB 1001"); //"Pos 14" FebUId 178 CROB 1001
    TText* tGem1_CROB_Id_Feb15 = new TText(0.670, 0.113, "CROB 1002"); //"Pos 15" FebUId 147 CROB 1002
    TText* tGem1_CROB_Id_Feb16 = new TText(0.403, 0.113, "CROB 1002"); //"Pos 16" FebUId 146 CROB 1002
    TText* tGem1_CROB_Id_Feb17 = new TText(0.232, 0.155, "CROB 1001"); //"Pos 17" FebUId 151 CROB 1001

    Text_String = "Feb VRefP Values in Gem 1 (Run " + to_string(RunId) + ")";
    TText* tTitle_Gem1_VRefP_Feb = new TText(0.01, 1.043, Text_String);
    TText* tGem1_VRefP_Feb0 = new TText(0.497, 0.779, "XXXXX");      //"Pos 0"  FebUId XXXXX CROB XXXXX
    TText* tGem1_VRefP_Feb1 = new TText(0.544, 0.634, "XXXXX");      //"Pos 1"  FebUId XXXXX CROB XXXXX
    TText* tGem1_VRefP_Feb2 = new TText(0.447, 0.657, "XXXXX");      //"Pos 2"  FebUId XXXXX CROB XXXXX
    TText* tGem1_VRefP_Feb3 = new TText(0.397, 0.537, "VRefP: 42");  //"Pos 3"  FebUId 162 CROB 1003
    TText* tGem1_VRefP_Feb4 = new TText(0.4948, 0.511, "VRefP: 50"); //"Pos 4"  FebUId 149 CROB 1003
    TText* tGem1_VRefP_Feb5 = new TText(0.607, 0.500, "VRefP: 52");  //"Pos 5"  FebUId 148 CROB 1002
    TText* tGem1_VRefP_Feb6 = new TText(0.659, 0.356, "XXXXX");      //"Pos 6"  FebUId XXXXX CROB XXXXX
    TText* tGem1_VRefP_Feb7 = new TText(0.553, 0.379, "VRefP: 52");  //"Pos 7"  FebUId 171 CROB 1001
    TText* tGem1_VRefP_Feb8 = new TText(0.445, 0.385, "VRefP: 52");  //"Pos 8"  FebUId 166 CROB 1001
    TText* tGem1_VRefP_Feb9 = new TText(0.335, 0.413, "VRefP: 52");  //"Pos 9"  FebUId 179 CROB 1001
    TText* tGem1_VRefP_Feb10 = new TText(0.281, 0.276, "VRefP: 52"); //"Pos 10" FebUId 145 CROB 1001
    TText* tGem1_VRefP_Feb11 = new TText(0.399, 0.257, "VRefP: 52"); //"Pos 11" FebUId 173 CROB 1001
    TText* tGem1_VRefP_Feb12 = new TText(0.500, 0.250, "VRefP: 52"); //"Pos 12" FebUId 160 CROB 1002
    TText* tGem1_VRefP_Feb13 = new TText(0.610, 0.231, "VRefP: 48"); //"Pos 13" FebUId 163 CROB 1002
    TText* tGem1_VRefP_Feb14 = new TText(0.719, 0.217, "VRefP: 46"); //"Pos 14" FebUId 178 CROB 1001
    TText* tGem1_VRefP_Feb15 = new TText(0.670, 0.113, "VRefP: 50"); //"Pos 15" FebUId 147 CROB 1002
    TText* tGem1_VRefP_Feb16 = new TText(0.403, 0.113, "VRefP: 48"); //"Pos 16" FebUId 146 CROB 1002
    TText* tGem1_VRefP_Feb17 = new TText(0.232, 0.155, "VRefP: 52"); //"Pos 17" FebUId 151 CROB 1001

    Text_String = "Feb Connector Positions in Gem 2 (Run " + to_string(RunId) + ")";
    TText* tTitle_Gem2_Feb_no = new TText(0.5, 1.06, Text_String);
    TText* tGem2_Feb0 = new TText(0.497, 0.779, "Pos 0");   //"Pos 0" FebUId XXXXX CROB XXXXX        --Only Change Comments
    TText* tGem2_Feb1 = new TText(0.544, 0.634, "Pos 1");   //"Pos 1" FebUId XXXXX CROB XXXXX         --Only Change Comments
    TText* tGem2_Feb2 = new TText(0.447, 0.657, "Pos 2");   //"Pos 2" FebUId XXXXX CROB XXXXX        --Only Change Comments
    TText* tGem2_Feb3 = new TText(0.397, 0.537, "Pos 3");   //"Pos 3" FebUId 205 CROB 2003         --Only Change Comments
    TText* tGem2_Feb4 = new TText(0.4948, 0.511, "Pos 4");  //"Pos 4" FebUId 202 CROB 2002        --Only Change Comments
    TText* tGem2_Feb5 = new TText(0.607, 0.500, "Pos 5");   //"Pos 5" FebUId 217 CROB 2002         --Only Change Comments
    TText* tGem2_Feb6 = new TText(0.659, 0.356, "Pos 6");   //"Pos 6" FebUId 180 CROB 2002         --Only Change Comments
    TText* tGem2_Feb7 = new TText(0.553, 0.379, "Pos 7");   //"Pos 7" FebUId 150 CROB 2003          --Only Change Comments
    TText* tGem2_Feb8 = new TText(0.445, 0.385, "Pos 8");   //"Pos 8" FebUId 177 CROB 2002         --Only Change Comments
    TText* tGem2_Feb9 = new TText(0.335, 0.413, "Pos 9");   //"Pos 9" FebUId 200 CROB 2002         --Only Change Comments
    TText* tGem2_Feb10 = new TText(0.281, 0.276, "Pos 10"); //"Pos 10" FebUId 165 CROB 2003        --Only Change Comments
    TText* tGem2_Feb11 = new TText(0.399, 0.257, "Pos 11"); //"Pos 11" FebUId 157 CROB 2003         --Only Change Comments
    TText* tGem2_Feb12 = new TText(0.500, 0.250, "Pos 12"); //"Pos 12" FebUId 172 CROB 2003         --Only Change Comments
    TText* tGem2_Feb13 = new TText(0.610, 0.231, "Pos 13"); //"Pos 13" FebUId 158 CROB 2003        --Only Change Comments
    TText* tGem2_Feb14 = new TText(0.719, 0.217, "Pos 14"); //"Pos 14" FebUId 219 CROB 2002         --Only Change Comments
    TText* tGem2_Feb15 = new TText(0.670, 0.113, "Pos 15"); //"Pos 15" FebUId 161 CROB 2002         --Only Change Comments
    TText* tGem2_Feb16 = new TText(0.403, 0.113, "Pos 16"); //"Pos 16" FebUId 204 CROB 2002        --Only Change Comments
    TText* tGem2_Feb17 = new TText(0.232, 0.155, "Pos 17"); //"Pos 17" FebUId 153 CROB 2003        --Only Change Comments

    Text_String = "Feb Unique Id in Gem 2 (Run " + to_string(RunId) + ")";
    TText* tTitle_Gem2_Unique_Id_Feb = new TText(0.12, 1.043, Text_String);
    TText* tGem2_Unique_Id_Feb0 = new TText(0.497, 0.779, "XXXXX");    //"Pos 0" FebUId XXXXX CROB XXXXX
    TText* tGem2_Unique_Id_Feb1 = new TText(0.544, 0.634, "XXXXX");    //"Pos 1" FebUId XXXXX CROB XXXXX
    TText* tGem2_Unique_Id_Feb2 = new TText(0.447, 0.657, "XXXXX");    //"Pos 2" FebUId XXXXX CROB XXXXX
    TText* tGem2_Unique_Id_Feb3 = new TText(0.397, 0.537, "Feb 205");  //"Pos 3" FebUId 205 CROB 2003
    TText* tGem2_Unique_Id_Feb4 = new TText(0.4948, 0.511, "Feb 202"); //"Pos 4" FebUId 202 CROB 2002
    TText* tGem2_Unique_Id_Feb5 = new TText(0.607, 0.500, "Feb 217");  //"Pos 5" FebUId 217 CROB 2002
    TText* tGem2_Unique_Id_Feb6 = new TText(0.659, 0.356, "Feb 180");  //"Pos 6" FebUId 180 CROB 2002
    TText* tGem2_Unique_Id_Feb7 = new TText(0.553, 0.379, "Feb 150");  //"Pos 7" FebUId 150 CROB 2003
    TText* tGem2_Unique_Id_Feb8 = new TText(0.445, 0.385, "Feb 177");  //"Pos 8" FebUId 177 CROB 2002
    TText* tGem2_Unique_Id_Feb9 = new TText(0.335, 0.413, "Feb 200");  //"Pos 9" FebUId 200 CROB 2002
    TText* tGem2_Unique_Id_Feb10 = new TText(0.281, 0.276, "Feb 165"); //"Pos 10" FebUId 165 CROB 2003
    TText* tGem2_Unique_Id_Feb11 = new TText(0.399, 0.257, "Feb 157"); //"Pos 11" FebUId 157 CROB 2003
    TText* tGem2_Unique_Id_Feb12 = new TText(0.500, 0.250, "Feb 172"); //"Pos 12" FebUId 172 CROB 2003
    TText* tGem2_Unique_Id_Feb13 = new TText(0.610, 0.231, "Feb 158"); //"Pos 13" FebUId 158 CROB 2003
    TText* tGem2_Unique_Id_Feb14 = new TText(0.719, 0.217, "Feb 219"); //"Pos 14" FebUId 219 CROB 2002
    TText* tGem2_Unique_Id_Feb15 = new TText(0.670, 0.113, "Feb 161"); //"Pos 15" FebUId 161 CROB 2002
    TText* tGem2_Unique_Id_Feb16 = new TText(0.403, 0.113, "Feb 204"); //"Pos 16" FebUId 204 CROB 2002
    TText* tGem2_Unique_Id_Feb17 = new TText(0.232, 0.155, "Feb 153"); //"Pos 17" FebUId 153 CROB 2003

    Text_String = "Feb CROB Id in Gem 2 (Run " + to_string(RunId) + ")";
    TText* tTitle_Gem2_CROB_Id_Feb = new TText(0.12, 1.043, Text_String);
    TText* tGem2_CROB_Id_Feb0 = new TText(0.497, 0.779, "XXXXX");      //"Pos 0" FebUId XXXXX CROB XXXXX
    TText* tGem2_CROB_Id_Feb1 = new TText(0.544, 0.634, "XXXXX");      //"Pos 1" FebUId XXXXX CROB XXXXX
    TText* tGem2_CROB_Id_Feb2 = new TText(0.447, 0.657, "XXXXX");      //"Pos 2" FebUId XXXXX CROB XXXXX
    TText* tGem2_CROB_Id_Feb3 = new TText(0.397, 0.537, "CROB 2003");  //"Pos 3" FebUId 205 CROB 2003
    TText* tGem2_CROB_Id_Feb4 = new TText(0.4948, 0.511, "CROB 2002"); //"Pos 4" FebUId 202 CROB 2002
    TText* tGem2_CROB_Id_Feb5 = new TText(0.607, 0.500, "CROB 2002");  //"Pos 5" FebUId 217 CROB 2002
    TText* tGem2_CROB_Id_Feb6 = new TText(0.659, 0.356, "CROB 2002");  //"Pos 6" FebUId 180 CROB 2002
    TText* tGem2_CROB_Id_Feb7 = new TText(0.553, 0.379, "CROB 2003");  //"Pos 7" FebUId 150 CROB 2003
    TText* tGem2_CROB_Id_Feb8 = new TText(0.445, 0.385, "CROB 2002");  //"Pos 8" FebUId 177 CROB 2002
    TText* tGem2_CROB_Id_Feb9 = new TText(0.335, 0.413, "CROB 2002");  //"Pos 9" FebUId 200 CROB 2002
    TText* tGem2_CROB_Id_Feb10 = new TText(0.281, 0.276, "CROB 2003"); //"Pos 10" FebUId 165 CROB 2003
    TText* tGem2_CROB_Id_Feb11 = new TText(0.399, 0.257, "CROB 2003"); //"Pos 11" FebUId 157 CROB 2003
    TText* tGem2_CROB_Id_Feb12 = new TText(0.500, 0.250, "CROB 2003"); //"Pos 12" FebUId 172 CROB 2003
    TText* tGem2_CROB_Id_Feb13 = new TText(0.610, 0.231, "CROB 2003"); //"Pos 13" FebUId 158 CROB 2003
    TText* tGem2_CROB_Id_Feb14 = new TText(0.719, 0.217, "CROB 2002"); //"Pos 14" FebUId 219 CROB 2002
    TText* tGem2_CROB_Id_Feb15 = new TText(0.670, 0.113, "CROB 2002"); //"Pos 15" FebUId 161 CROB 2002
    TText* tGem2_CROB_Id_Feb16 = new TText(0.403, 0.113, "CROB 2002"); //"Pos 16" FebUId 204 CROB 2002
    TText* tGem2_CROB_Id_Feb17 = new TText(0.232, 0.155, "CROB 2003"); //"Pos 17" FebUId 153 CROB 2003

    Text_String = "Feb VRefP Values in Gem 2 (Run " + to_string(RunId) + ")";
    TText* tTitle_Gem2_VRefP_Feb = new TText(0.01, 1.043, Text_String);
    TText* tGem2_VRefP_Feb0 = new TText(0.497, 0.779, "XXXXX");      //"Pos 0"  FebUId XXXXX CROB XXXXX
    TText* tGem2_VRefP_Feb1 = new TText(0.544, 0.634, "XXXXX");      //"Pos 1"  FebUId XXXXX CROB XXXXX
    TText* tGem2_VRefP_Feb2 = new TText(0.447, 0.657, "XXXXX");      //"Pos 2"  FebUId XXXXX CROB XXXXX
    TText* tGem2_VRefP_Feb3 = new TText(0.397, 0.537, "VRefP: 60");  //"Pos 3"  FebUId 205 CROB 2003
    TText* tGem2_VRefP_Feb4 = new TText(0.4948, 0.511, "VRefP: 60"); //"Pos 4"  FebUId 202 CROB 2002
    TText* tGem2_VRefP_Feb5 = new TText(0.607, 0.500, "VRefP: 60");  //"Pos 5"  FebUId 217 CROB 2002
    TText* tGem2_VRefP_Feb6 = new TText(0.659, 0.356, "VRefP: 51");  //"Pos 6"  FebUId 180 CROB 2002
    TText* tGem2_VRefP_Feb7 = new TText(0.553, 0.379, "VRefP: 51");  //"Pos 7"  FebUId 150 CROB 2003
    TText* tGem2_VRefP_Feb8 = new TText(0.445, 0.385, "VRefP: 47");  //"Pos 8"  FebUId 177 CROB 2002
    TText* tGem2_VRefP_Feb9 = new TText(0.335, 0.413, "VRefP: 57");  //"Pos 9"  FebUId 200 CROB 2002
    TText* tGem2_VRefP_Feb10 = new TText(0.281, 0.276, "VRefP: 51"); //"Pos 10" FebUId 165 CROB 2003
    TText* tGem2_VRefP_Feb11 = new TText(0.399, 0.257, "VRefP: 51"); //"Pos 11" FebUId 157 CROB 2003
    TText* tGem2_VRefP_Feb12 = new TText(0.500, 0.250, "VRefP: 51"); //"Pos 12" FebUId 172 CROB 2003
    TText* tGem2_VRefP_Feb13 = new TText(0.610, 0.231, "VRefP: 51"); //"Pos 13" FebUId 158 CROB 2003
    TText* tGem2_VRefP_Feb14 = new TText(0.719, 0.217, "VRefP: 57"); //"Pos 14" FebUId 219 CROB 2002
    TText* tGem2_VRefP_Feb15 = new TText(0.670, 0.113, "VRefP: 54"); //"Pos 15" FebUId 161 CROB 2002
    TText* tGem2_VRefP_Feb16 = new TText(0.403, 0.113, "VRefP: 57"); //"Pos 16" FebUId 204 CROB 2002
    TText* tGem2_VRefP_Feb17 = new TText(0.232, 0.155, "VRefP: 51"); //"Pos 17" FebUId 153 CROB 2003

    tTitle_Gem1_Feb_no->SetTextFont(43);
    tTitle_Gem1_Feb_no->SetTextSize(25);
    tTitle_Gem1_Feb_no->SetTextAngle(0);

    tTitle_Gem1_Unique_Id_Feb->SetTextFont(43);
    tTitle_Gem1_Unique_Id_Feb->SetTextSize(25);
    tTitle_Gem1_Unique_Id_Feb->SetTextAngle(0);

    tTitle_Gem1_CROB_Id_Feb->SetTextFont(43);
    tTitle_Gem1_CROB_Id_Feb->SetTextSize(25);
    tTitle_Gem1_CROB_Id_Feb->SetTextAngle(0);

    tTitle_Gem1_VRefP_Feb->SetTextFont(43);
    tTitle_Gem1_VRefP_Feb->SetTextSize(25);
    tTitle_Gem1_VRefP_Feb->SetTextAngle(0);

    tTitle_Gem2_Feb_no->SetTextFont(43);
    tTitle_Gem2_Feb_no->SetTextSize(25);
    tTitle_Gem2_Feb_no->SetTextAngle(0);

    tTitle_Gem2_Unique_Id_Feb->SetTextFont(43);
    tTitle_Gem2_Unique_Id_Feb->SetTextSize(25);
    tTitle_Gem2_Unique_Id_Feb->SetTextAngle(0);

    tTitle_Gem2_CROB_Id_Feb->SetTextFont(43);
    tTitle_Gem2_CROB_Id_Feb->SetTextSize(25);
    tTitle_Gem2_CROB_Id_Feb->SetTextAngle(0);

    tTitle_Gem2_VRefP_Feb->SetTextFont(43);
    tTitle_Gem2_VRefP_Feb->SetTextSize(25);
    tTitle_Gem2_VRefP_Feb->SetTextAngle(0);

    tGem1_Feb0->SetTextAlign(22);
    tGem1_Feb0->SetTextColor(kRed + 2);
    tGem1_Feb0->SetTextFont(43);
    tGem1_Feb0->SetTextSize(15);
    tGem1_Feb0->SetTextAngle(73.3);

    tGem1_Feb1->SetTextAlign(22);
    tGem1_Feb1->SetTextColor(kRed + 2);
    tGem1_Feb1->SetTextFont(43);
    tGem1_Feb1->SetTextSize(15);
    tGem1_Feb1->SetTextAngle(73.3);

    tGem1_Feb2->SetTextAlign(22);
    tGem1_Feb2->SetTextColor(kRed + 2);
    tGem1_Feb2->SetTextFont(43);
    tGem1_Feb2->SetTextSize(15);
    tGem1_Feb2->SetTextAngle(73.3);

    tGem1_Feb3->SetTextAlign(22);
    tGem1_Feb3->SetTextColor(kRed + 2);
    tGem1_Feb3->SetTextFont(43);
    tGem1_Feb3->SetTextSize(15);
    tGem1_Feb3->SetTextAngle(73.3);

    tGem1_Feb4->SetTextAlign(22);
    tGem1_Feb4->SetTextColor(kRed + 2);
    tGem1_Feb4->SetTextFont(43);
    tGem1_Feb4->SetTextSize(15);
    tGem1_Feb4->SetTextAngle(73.3);

    tGem1_Feb5->SetTextAlign(22);
    tGem1_Feb5->SetTextColor(kRed + 2);
    tGem1_Feb5->SetTextFont(43);
    tGem1_Feb5->SetTextSize(15);
    tGem1_Feb5->SetTextAngle(73.3);

    tGem1_Feb6->SetTextAlign(22);
    tGem1_Feb6->SetTextColor(kRed + 2);
    tGem1_Feb6->SetTextFont(43);
    tGem1_Feb6->SetTextSize(15);
    tGem1_Feb6->SetTextAngle(73.3);

    tGem1_Feb7->SetTextAlign(22);
    tGem1_Feb7->SetTextColor(kRed + 2);
    tGem1_Feb7->SetTextFont(43);
    tGem1_Feb7->SetTextSize(15);
    tGem1_Feb7->SetTextAngle(73.3);

    tGem1_Feb8->SetTextAlign(22);
    tGem1_Feb8->SetTextColor(kRed + 2);
    tGem1_Feb8->SetTextFont(43);
    tGem1_Feb8->SetTextSize(15);
    tGem1_Feb8->SetTextAngle(73.3);

    tGem1_Feb9->SetTextAlign(22);
    tGem1_Feb9->SetTextColor(kRed + 2);
    tGem1_Feb9->SetTextFont(43);
    tGem1_Feb9->SetTextSize(15);
    tGem1_Feb9->SetTextAngle(73.3);

    tGem1_Feb10->SetTextAlign(22);
    tGem1_Feb10->SetTextColor(kRed + 2);
    tGem1_Feb10->SetTextFont(43);
    tGem1_Feb10->SetTextSize(15);
    tGem1_Feb10->SetTextAngle(73.3);

    tGem1_Feb11->SetTextAlign(22);
    tGem1_Feb11->SetTextColor(kRed + 2);
    tGem1_Feb11->SetTextFont(43);
    tGem1_Feb11->SetTextSize(15);
    tGem1_Feb11->SetTextAngle(73.3);

    tGem1_Feb12->SetTextAlign(22);
    tGem1_Feb12->SetTextColor(kRed + 2);
    tGem1_Feb12->SetTextFont(43);
    tGem1_Feb12->SetTextSize(15);
    tGem1_Feb12->SetTextAngle(73.3);

    tGem1_Feb13->SetTextAlign(22);
    tGem1_Feb13->SetTextColor(kRed + 2);
    tGem1_Feb13->SetTextFont(43);
    tGem1_Feb13->SetTextSize(15);
    tGem1_Feb13->SetTextAngle(73.3);

    tGem1_Feb14->SetTextAlign(22);
    tGem1_Feb14->SetTextColor(kRed + 2);
    tGem1_Feb14->SetTextFont(43);
    tGem1_Feb14->SetTextSize(15);
    tGem1_Feb14->SetTextAngle(73.3);

    tGem1_Feb15->SetTextAlign(22);
    tGem1_Feb15->SetTextColor(kRed + 2);
    tGem1_Feb15->SetTextFont(43);
    tGem1_Feb15->SetTextSize(15);
    tGem1_Feb15->SetTextAngle(0);

    tGem1_Feb16->SetTextAlign(22);
    tGem1_Feb16->SetTextColor(kRed + 2);
    tGem1_Feb16->SetTextFont(43);
    tGem1_Feb16->SetTextSize(15);
    tGem1_Feb16->SetTextAngle(0);

    tGem1_Feb17->SetTextAlign(22);
    tGem1_Feb17->SetTextColor(kRed + 2);
    tGem1_Feb17->SetTextFont(43);
    tGem1_Feb17->SetTextSize(15);
    tGem1_Feb17->SetTextAngle(73.3);

    tGem1_Unique_Id_Feb0->SetTextAlign(22);
    tGem1_Unique_Id_Feb0->SetTextColor(kRed + 2);
    tGem1_Unique_Id_Feb0->SetTextFont(43);
    tGem1_Unique_Id_Feb0->SetTextSize(15);
    tGem1_Unique_Id_Feb0->SetTextAngle(73.3);

    tGem1_Unique_Id_Feb1->SetTextAlign(22);
    tGem1_Unique_Id_Feb1->SetTextColor(kRed + 2);
    tGem1_Unique_Id_Feb1->SetTextFont(43);
    tGem1_Unique_Id_Feb1->SetTextSize(15);
    tGem1_Unique_Id_Feb1->SetTextAngle(73.3);

    tGem1_Unique_Id_Feb2->SetTextAlign(22);
    tGem1_Unique_Id_Feb2->SetTextColor(kRed + 2);
    tGem1_Unique_Id_Feb2->SetTextFont(43);
    tGem1_Unique_Id_Feb2->SetTextSize(15);
    tGem1_Unique_Id_Feb2->SetTextAngle(73.3);

    tGem1_Unique_Id_Feb3->SetTextAlign(22);
    tGem1_Unique_Id_Feb3->SetTextColor(kRed + 2);
    tGem1_Unique_Id_Feb3->SetTextFont(43);
    tGem1_Unique_Id_Feb3->SetTextSize(15);
    tGem1_Unique_Id_Feb3->SetTextAngle(73.3);

    tGem1_Unique_Id_Feb4->SetTextAlign(22);
    tGem1_Unique_Id_Feb4->SetTextColor(kRed + 2);
    tGem1_Unique_Id_Feb4->SetTextFont(43);
    tGem1_Unique_Id_Feb4->SetTextSize(15);
    tGem1_Unique_Id_Feb4->SetTextAngle(73.3);

    tGem1_Unique_Id_Feb5->SetTextAlign(22);
    tGem1_Unique_Id_Feb5->SetTextColor(kRed + 2);
    tGem1_Unique_Id_Feb5->SetTextFont(43);
    tGem1_Unique_Id_Feb5->SetTextSize(15);
    tGem1_Unique_Id_Feb5->SetTextAngle(73.3);

    tGem1_Unique_Id_Feb6->SetTextAlign(22);
    tGem1_Unique_Id_Feb6->SetTextColor(kRed + 2);
    tGem1_Unique_Id_Feb6->SetTextFont(43);
    tGem1_Unique_Id_Feb6->SetTextSize(15);
    tGem1_Unique_Id_Feb6->SetTextAngle(73.3);

    tGem1_Unique_Id_Feb7->SetTextAlign(22);
    tGem1_Unique_Id_Feb7->SetTextColor(kRed + 2);
    tGem1_Unique_Id_Feb7->SetTextFont(43);
    tGem1_Unique_Id_Feb7->SetTextSize(15);
    tGem1_Unique_Id_Feb7->SetTextAngle(73.3);

    tGem1_Unique_Id_Feb8->SetTextAlign(22);
    tGem1_Unique_Id_Feb8->SetTextColor(kRed + 2);
    tGem1_Unique_Id_Feb8->SetTextFont(43);
    tGem1_Unique_Id_Feb8->SetTextSize(15);
    tGem1_Unique_Id_Feb8->SetTextAngle(73.3);

    tGem1_Unique_Id_Feb9->SetTextAlign(22);
    tGem1_Unique_Id_Feb9->SetTextColor(kRed + 2);
    tGem1_Unique_Id_Feb9->SetTextFont(43);
    tGem1_Unique_Id_Feb9->SetTextSize(15);
    tGem1_Unique_Id_Feb9->SetTextAngle(73.3);

    tGem1_Unique_Id_Feb10->SetTextAlign(22);
    tGem1_Unique_Id_Feb10->SetTextColor(kRed + 2);
    tGem1_Unique_Id_Feb10->SetTextFont(43);
    tGem1_Unique_Id_Feb10->SetTextSize(15);
    tGem1_Unique_Id_Feb10->SetTextAngle(73.3);

    tGem1_Unique_Id_Feb11->SetTextAlign(22);
    tGem1_Unique_Id_Feb11->SetTextColor(kRed + 2);
    tGem1_Unique_Id_Feb11->SetTextFont(43);
    tGem1_Unique_Id_Feb11->SetTextSize(15);
    tGem1_Unique_Id_Feb11->SetTextAngle(73.3);

    tGem1_Unique_Id_Feb12->SetTextAlign(22);
    tGem1_Unique_Id_Feb12->SetTextColor(kRed + 2);
    tGem1_Unique_Id_Feb12->SetTextFont(43);
    tGem1_Unique_Id_Feb12->SetTextSize(15);
    tGem1_Unique_Id_Feb12->SetTextAngle(73.3);

    tGem1_Unique_Id_Feb13->SetTextAlign(22);
    tGem1_Unique_Id_Feb13->SetTextColor(kRed + 2);
    tGem1_Unique_Id_Feb13->SetTextFont(43);
    tGem1_Unique_Id_Feb13->SetTextSize(15);
    tGem1_Unique_Id_Feb13->SetTextAngle(73.3);

    tGem1_Unique_Id_Feb14->SetTextAlign(22);
    tGem1_Unique_Id_Feb14->SetTextColor(kRed + 2);
    tGem1_Unique_Id_Feb14->SetTextFont(43);
    tGem1_Unique_Id_Feb14->SetTextSize(15);
    tGem1_Unique_Id_Feb14->SetTextAngle(73.3);

    tGem1_Unique_Id_Feb15->SetTextAlign(22);
    tGem1_Unique_Id_Feb15->SetTextColor(kRed + 2);
    tGem1_Unique_Id_Feb15->SetTextFont(43);
    tGem1_Unique_Id_Feb15->SetTextSize(15);
    tGem1_Unique_Id_Feb15->SetTextAngle(0);

    tGem1_Unique_Id_Feb16->SetTextAlign(22);
    tGem1_Unique_Id_Feb16->SetTextColor(kRed + 2);
    tGem1_Unique_Id_Feb16->SetTextFont(43);
    tGem1_Unique_Id_Feb16->SetTextSize(15);
    tGem1_Unique_Id_Feb16->SetTextAngle(0);

    tGem1_Unique_Id_Feb17->SetTextAlign(22);
    tGem1_Unique_Id_Feb17->SetTextColor(kRed + 2);
    tGem1_Unique_Id_Feb17->SetTextFont(43);
    tGem1_Unique_Id_Feb17->SetTextSize(15);
    tGem1_Unique_Id_Feb17->SetTextAngle(73.3);

    tGem1_CROB_Id_Feb0->SetTextAlign(22);
    tGem1_CROB_Id_Feb0->SetTextColor(kRed + 2);
    tGem1_CROB_Id_Feb0->SetTextFont(43);
    tGem1_CROB_Id_Feb0->SetTextSize(11);
    tGem1_CROB_Id_Feb0->SetTextAngle(73.3);

    tGem1_CROB_Id_Feb1->SetTextAlign(22);
    tGem1_CROB_Id_Feb1->SetTextColor(kRed + 2);
    tGem1_CROB_Id_Feb1->SetTextFont(43);
    tGem1_CROB_Id_Feb1->SetTextSize(11);
    tGem1_CROB_Id_Feb1->SetTextAngle(73.3);

    tGem1_CROB_Id_Feb2->SetTextAlign(22);
    tGem1_CROB_Id_Feb2->SetTextColor(kRed + 2);
    tGem1_CROB_Id_Feb2->SetTextFont(43);
    tGem1_CROB_Id_Feb2->SetTextSize(11);
    tGem1_CROB_Id_Feb2->SetTextAngle(73.3);

    tGem1_CROB_Id_Feb3->SetTextAlign(22);
    tGem1_CROB_Id_Feb3->SetTextColor(kRed + 2);
    tGem1_CROB_Id_Feb3->SetTextFont(43);
    tGem1_CROB_Id_Feb3->SetTextSize(11);
    tGem1_CROB_Id_Feb3->SetTextAngle(73.3);

    tGem1_CROB_Id_Feb4->SetTextAlign(22);
    tGem1_CROB_Id_Feb4->SetTextColor(kRed + 2);
    tGem1_CROB_Id_Feb4->SetTextFont(43);
    tGem1_CROB_Id_Feb4->SetTextSize(11);
    tGem1_CROB_Id_Feb4->SetTextAngle(73.3);

    tGem1_CROB_Id_Feb5->SetTextAlign(22);
    tGem1_CROB_Id_Feb5->SetTextColor(kRed + 2);
    tGem1_CROB_Id_Feb5->SetTextFont(43);
    tGem1_CROB_Id_Feb5->SetTextSize(11);
    tGem1_CROB_Id_Feb5->SetTextAngle(73.3);

    tGem1_CROB_Id_Feb6->SetTextAlign(22);
    tGem1_CROB_Id_Feb6->SetTextColor(kRed + 2);
    tGem1_CROB_Id_Feb6->SetTextFont(43);
    tGem1_CROB_Id_Feb6->SetTextSize(11);
    tGem1_CROB_Id_Feb6->SetTextAngle(73.3);

    tGem1_CROB_Id_Feb7->SetTextAlign(22);
    tGem1_CROB_Id_Feb7->SetTextColor(kRed + 2);
    tGem1_CROB_Id_Feb7->SetTextFont(43);
    tGem1_CROB_Id_Feb7->SetTextSize(11);
    tGem1_CROB_Id_Feb7->SetTextAngle(73.3);

    tGem1_CROB_Id_Feb8->SetTextAlign(22);
    tGem1_CROB_Id_Feb8->SetTextColor(kRed + 2);
    tGem1_CROB_Id_Feb8->SetTextFont(43);
    tGem1_CROB_Id_Feb8->SetTextSize(11);
    tGem1_CROB_Id_Feb8->SetTextAngle(73.3);

    tGem1_CROB_Id_Feb9->SetTextAlign(22);
    tGem1_CROB_Id_Feb9->SetTextColor(kRed + 2);
    tGem1_CROB_Id_Feb9->SetTextFont(43);
    tGem1_CROB_Id_Feb9->SetTextSize(11);
    tGem1_CROB_Id_Feb9->SetTextAngle(73.3);

    tGem1_CROB_Id_Feb10->SetTextAlign(22);
    tGem1_CROB_Id_Feb10->SetTextColor(kRed + 2);
    tGem1_CROB_Id_Feb10->SetTextFont(43);
    tGem1_CROB_Id_Feb10->SetTextSize(11);
    tGem1_CROB_Id_Feb10->SetTextAngle(73.3);

    tGem1_CROB_Id_Feb11->SetTextAlign(22);
    tGem1_CROB_Id_Feb11->SetTextColor(kRed + 2);
    tGem1_CROB_Id_Feb11->SetTextFont(43);
    tGem1_CROB_Id_Feb11->SetTextSize(11);
    tGem1_CROB_Id_Feb11->SetTextAngle(73.3);

    tGem1_CROB_Id_Feb12->SetTextAlign(22);
    tGem1_CROB_Id_Feb12->SetTextColor(kRed + 2);
    tGem1_CROB_Id_Feb12->SetTextFont(43);
    tGem1_CROB_Id_Feb12->SetTextSize(11);
    tGem1_CROB_Id_Feb12->SetTextAngle(73.3);

    tGem1_CROB_Id_Feb13->SetTextAlign(22);
    tGem1_CROB_Id_Feb13->SetTextColor(kRed + 2);
    tGem1_CROB_Id_Feb13->SetTextFont(43);
    tGem1_CROB_Id_Feb13->SetTextSize(11);
    tGem1_CROB_Id_Feb13->SetTextAngle(73.3);

    tGem1_CROB_Id_Feb14->SetTextAlign(22);
    tGem1_CROB_Id_Feb14->SetTextColor(kRed + 2);
    tGem1_CROB_Id_Feb14->SetTextFont(43);
    tGem1_CROB_Id_Feb14->SetTextSize(11);
    tGem1_CROB_Id_Feb14->SetTextAngle(73.3);

    tGem1_CROB_Id_Feb15->SetTextAlign(22);
    tGem1_CROB_Id_Feb15->SetTextColor(kRed + 2);
    tGem1_CROB_Id_Feb15->SetTextFont(43);
    tGem1_CROB_Id_Feb15->SetTextSize(11);
    tGem1_CROB_Id_Feb15->SetTextAngle(0);

    tGem1_CROB_Id_Feb16->SetTextAlign(22);
    tGem1_CROB_Id_Feb16->SetTextColor(kRed + 2);
    tGem1_CROB_Id_Feb16->SetTextFont(43);
    tGem1_CROB_Id_Feb16->SetTextSize(11);
    tGem1_CROB_Id_Feb16->SetTextAngle(0);

    tGem1_CROB_Id_Feb17->SetTextAlign(22);
    tGem1_CROB_Id_Feb17->SetTextColor(kRed + 2);
    tGem1_CROB_Id_Feb17->SetTextFont(43);
    tGem1_CROB_Id_Feb17->SetTextSize(11);
    tGem1_CROB_Id_Feb17->SetTextAngle(73.3);

    tGem1_VRefP_Feb0->SetTextAlign(22);
    tGem1_VRefP_Feb0->SetTextColor(kRed + 2);
    tGem1_VRefP_Feb0->SetTextFont(43);
    tGem1_VRefP_Feb0->SetTextSize(12);
    tGem1_VRefP_Feb0->SetTextAngle(73.3);

    tGem1_VRefP_Feb1->SetTextAlign(22);
    tGem1_VRefP_Feb1->SetTextColor(kRed + 2);
    tGem1_VRefP_Feb1->SetTextFont(43);
    tGem1_VRefP_Feb1->SetTextSize(12);
    tGem1_VRefP_Feb1->SetTextAngle(73.3);

    tGem1_VRefP_Feb2->SetTextAlign(22);
    tGem1_VRefP_Feb2->SetTextColor(kRed + 2);
    tGem1_VRefP_Feb2->SetTextFont(43);
    tGem1_VRefP_Feb2->SetTextSize(12);
    tGem1_VRefP_Feb2->SetTextAngle(73.3);

    tGem1_VRefP_Feb3->SetTextAlign(22);
    tGem1_VRefP_Feb3->SetTextColor(kRed + 2);
    tGem1_VRefP_Feb3->SetTextFont(43);
    tGem1_VRefP_Feb3->SetTextSize(12);
    tGem1_VRefP_Feb3->SetTextAngle(73.3);

    tGem1_VRefP_Feb4->SetTextAlign(22);
    tGem1_VRefP_Feb4->SetTextColor(kRed + 2);
    tGem1_VRefP_Feb4->SetTextFont(43);
    tGem1_VRefP_Feb4->SetTextSize(12);
    tGem1_VRefP_Feb4->SetTextAngle(73.3);

    tGem1_VRefP_Feb5->SetTextAlign(22);
    tGem1_VRefP_Feb5->SetTextColor(kRed + 2);
    tGem1_VRefP_Feb5->SetTextFont(43);
    tGem1_VRefP_Feb5->SetTextSize(12);
    tGem1_VRefP_Feb5->SetTextAngle(73.3);

    tGem1_VRefP_Feb6->SetTextAlign(22);
    tGem1_VRefP_Feb6->SetTextColor(kRed + 2);
    tGem1_VRefP_Feb6->SetTextFont(43);
    tGem1_VRefP_Feb6->SetTextSize(12);
    tGem1_VRefP_Feb6->SetTextAngle(73.3);

    tGem1_VRefP_Feb7->SetTextAlign(22);
    tGem1_VRefP_Feb7->SetTextColor(kRed + 2);
    tGem1_VRefP_Feb7->SetTextFont(43);
    tGem1_VRefP_Feb7->SetTextSize(12);
    tGem1_VRefP_Feb7->SetTextAngle(73.3);

    tGem1_VRefP_Feb8->SetTextAlign(22);
    tGem1_VRefP_Feb8->SetTextColor(kRed + 2);
    tGem1_VRefP_Feb8->SetTextFont(43);
    tGem1_VRefP_Feb8->SetTextSize(12);
    tGem1_VRefP_Feb8->SetTextAngle(73.3);

    tGem1_VRefP_Feb9->SetTextAlign(22);
    tGem1_VRefP_Feb9->SetTextColor(kRed + 2);
    tGem1_VRefP_Feb9->SetTextFont(43);
    tGem1_VRefP_Feb9->SetTextSize(12);
    tGem1_VRefP_Feb9->SetTextAngle(73.3);

    tGem1_VRefP_Feb10->SetTextAlign(22);
    tGem1_VRefP_Feb10->SetTextColor(kRed + 2);
    tGem1_VRefP_Feb10->SetTextFont(43);
    tGem1_VRefP_Feb10->SetTextSize(12);
    tGem1_VRefP_Feb10->SetTextAngle(73.3);

    tGem1_VRefP_Feb11->SetTextAlign(22);
    tGem1_VRefP_Feb11->SetTextColor(kRed + 2);
    tGem1_VRefP_Feb11->SetTextFont(43);
    tGem1_VRefP_Feb11->SetTextSize(12);
    tGem1_VRefP_Feb11->SetTextAngle(73.3);

    tGem1_VRefP_Feb12->SetTextAlign(22);
    tGem1_VRefP_Feb12->SetTextColor(kRed + 2);
    tGem1_VRefP_Feb12->SetTextFont(43);
    tGem1_VRefP_Feb12->SetTextSize(12);
    tGem1_VRefP_Feb12->SetTextAngle(73.3);

    tGem1_VRefP_Feb13->SetTextAlign(22);
    tGem1_VRefP_Feb13->SetTextColor(kRed + 2);
    tGem1_VRefP_Feb13->SetTextFont(43);
    tGem1_VRefP_Feb13->SetTextSize(12);
    tGem1_VRefP_Feb13->SetTextAngle(73.3);

    tGem1_VRefP_Feb14->SetTextAlign(22);
    tGem1_VRefP_Feb14->SetTextColor(kRed + 2);
    tGem1_VRefP_Feb14->SetTextFont(43);
    tGem1_VRefP_Feb14->SetTextSize(12);
    tGem1_VRefP_Feb14->SetTextAngle(73.3);

    tGem1_VRefP_Feb15->SetTextAlign(22);
    tGem1_VRefP_Feb15->SetTextColor(kRed + 2);
    tGem1_VRefP_Feb15->SetTextFont(43);
    tGem1_VRefP_Feb15->SetTextSize(12);
    tGem1_VRefP_Feb15->SetTextAngle(0);

    tGem1_VRefP_Feb16->SetTextAlign(22);
    tGem1_VRefP_Feb16->SetTextColor(kRed + 2);
    tGem1_VRefP_Feb16->SetTextFont(43);
    tGem1_VRefP_Feb16->SetTextSize(12);
    tGem1_VRefP_Feb16->SetTextAngle(0);

    tGem1_VRefP_Feb17->SetTextAlign(22);
    tGem1_VRefP_Feb17->SetTextColor(kRed + 2);
    tGem1_VRefP_Feb17->SetTextFont(43);
    tGem1_VRefP_Feb17->SetTextSize(12);
    tGem1_VRefP_Feb17->SetTextAngle(73.3);

    tGem2_Feb0->SetTextAlign(22);
    tGem2_Feb0->SetTextColor(kRed + 2);
    tGem2_Feb0->SetTextFont(43);
    tGem2_Feb0->SetTextSize(15);
    tGem2_Feb0->SetTextAngle(73.3);

    tGem2_Feb1->SetTextAlign(22);
    tGem2_Feb1->SetTextColor(kRed + 2);
    tGem2_Feb1->SetTextFont(43);
    tGem2_Feb1->SetTextSize(15);
    tGem2_Feb1->SetTextAngle(73.3);

    tGem2_Feb2->SetTextAlign(22);
    tGem2_Feb2->SetTextColor(kRed + 2);
    tGem2_Feb2->SetTextFont(43);
    tGem2_Feb2->SetTextSize(15);
    tGem2_Feb2->SetTextAngle(73.3);

    tGem2_Feb3->SetTextAlign(22);
    tGem2_Feb3->SetTextColor(kRed + 2);
    tGem2_Feb3->SetTextFont(43);
    tGem2_Feb3->SetTextSize(15);
    tGem2_Feb3->SetTextAngle(73.3);

    tGem2_Feb4->SetTextAlign(22);
    tGem2_Feb4->SetTextColor(kRed + 2);
    tGem2_Feb4->SetTextFont(43);
    tGem2_Feb4->SetTextSize(15);
    tGem2_Feb4->SetTextAngle(73.3);

    tGem2_Feb5->SetTextAlign(22);
    tGem2_Feb5->SetTextColor(kRed + 2);
    tGem2_Feb5->SetTextFont(43);
    tGem2_Feb5->SetTextSize(15);
    tGem2_Feb5->SetTextAngle(73.3);

    tGem2_Feb6->SetTextAlign(22);
    tGem2_Feb6->SetTextColor(kRed + 2);
    tGem2_Feb6->SetTextFont(43);
    tGem2_Feb6->SetTextSize(15);
    tGem2_Feb6->SetTextAngle(73.3);

    tGem2_Feb7->SetTextAlign(22);
    tGem2_Feb7->SetTextColor(kRed + 2);
    tGem2_Feb7->SetTextFont(43);
    tGem2_Feb7->SetTextSize(15);
    tGem2_Feb7->SetTextAngle(73.3);

    tGem2_Feb8->SetTextAlign(22);
    tGem2_Feb8->SetTextColor(kRed + 2);
    tGem2_Feb8->SetTextFont(43);
    tGem2_Feb8->SetTextSize(15);
    tGem2_Feb8->SetTextAngle(73.3);

    tGem2_Feb9->SetTextAlign(22);
    tGem2_Feb9->SetTextColor(kRed + 2);
    tGem2_Feb9->SetTextFont(43);
    tGem2_Feb9->SetTextSize(15);
    tGem2_Feb9->SetTextAngle(73.3);

    tGem2_Feb10->SetTextAlign(22);
    tGem2_Feb10->SetTextColor(kRed + 2);
    tGem2_Feb10->SetTextFont(43);
    tGem2_Feb10->SetTextSize(15);
    tGem2_Feb10->SetTextAngle(73.3);

    tGem2_Feb11->SetTextAlign(22);
    tGem2_Feb11->SetTextColor(kRed + 2);
    tGem2_Feb11->SetTextFont(43);
    tGem2_Feb11->SetTextSize(15);
    tGem2_Feb11->SetTextAngle(73.3);

    tGem2_Feb12->SetTextAlign(22);
    tGem2_Feb12->SetTextColor(kRed + 2);
    tGem2_Feb12->SetTextFont(43);
    tGem2_Feb12->SetTextSize(15);
    tGem2_Feb12->SetTextAngle(73.3);

    tGem2_Feb13->SetTextAlign(22);
    tGem2_Feb13->SetTextColor(kRed + 2);
    tGem2_Feb13->SetTextFont(43);
    tGem2_Feb13->SetTextSize(15);
    tGem2_Feb13->SetTextAngle(73.3);

    tGem2_Feb14->SetTextAlign(22);
    tGem2_Feb14->SetTextColor(kRed + 2);
    tGem2_Feb14->SetTextFont(43);
    tGem2_Feb14->SetTextSize(15);
    tGem2_Feb14->SetTextAngle(73.3);

    tGem2_Feb15->SetTextAlign(22);
    tGem2_Feb15->SetTextColor(kRed + 2);
    tGem2_Feb15->SetTextFont(43);
    tGem2_Feb15->SetTextSize(15);
    tGem2_Feb15->SetTextAngle(0);

    tGem2_Feb16->SetTextAlign(22);
    tGem2_Feb16->SetTextColor(kRed + 2);
    tGem2_Feb16->SetTextFont(43);
    tGem2_Feb16->SetTextSize(15);
    tGem2_Feb16->SetTextAngle(0);

    tGem2_Feb17->SetTextAlign(22);
    tGem2_Feb17->SetTextColor(kRed + 2);
    tGem2_Feb17->SetTextFont(43);
    tGem2_Feb17->SetTextSize(15);
    tGem2_Feb17->SetTextAngle(73.3);

    tGem2_Unique_Id_Feb0->SetTextAlign(22);
    tGem2_Unique_Id_Feb0->SetTextColor(kRed + 2);
    tGem2_Unique_Id_Feb0->SetTextFont(43);
    tGem2_Unique_Id_Feb0->SetTextSize(15);
    tGem2_Unique_Id_Feb0->SetTextAngle(73.3);

    tGem2_Unique_Id_Feb1->SetTextAlign(22);
    tGem2_Unique_Id_Feb1->SetTextColor(kRed + 2);
    tGem2_Unique_Id_Feb1->SetTextFont(43);
    tGem2_Unique_Id_Feb1->SetTextSize(15);
    tGem2_Unique_Id_Feb1->SetTextAngle(73.3);

    tGem2_Unique_Id_Feb2->SetTextAlign(22);
    tGem2_Unique_Id_Feb2->SetTextColor(kRed + 2);
    tGem2_Unique_Id_Feb2->SetTextFont(43);
    tGem2_Unique_Id_Feb2->SetTextSize(15);
    tGem2_Unique_Id_Feb2->SetTextAngle(73.3);

    tGem2_Unique_Id_Feb3->SetTextAlign(22);
    tGem2_Unique_Id_Feb3->SetTextColor(kRed + 2);
    tGem2_Unique_Id_Feb3->SetTextFont(43);
    tGem2_Unique_Id_Feb3->SetTextSize(15);
    tGem2_Unique_Id_Feb3->SetTextAngle(73.3);

    tGem2_Unique_Id_Feb4->SetTextAlign(22);
    tGem2_Unique_Id_Feb4->SetTextColor(kRed + 2);
    tGem2_Unique_Id_Feb4->SetTextFont(43);
    tGem2_Unique_Id_Feb4->SetTextSize(15);
    tGem2_Unique_Id_Feb4->SetTextAngle(73.3);

    tGem2_Unique_Id_Feb5->SetTextAlign(22);
    tGem2_Unique_Id_Feb5->SetTextColor(kRed + 2);
    tGem2_Unique_Id_Feb5->SetTextFont(43);
    tGem2_Unique_Id_Feb5->SetTextSize(15);
    tGem2_Unique_Id_Feb5->SetTextAngle(73.3);

    tGem2_Unique_Id_Feb6->SetTextAlign(22);
    tGem2_Unique_Id_Feb6->SetTextColor(kRed + 2);
    tGem2_Unique_Id_Feb6->SetTextFont(43);
    tGem2_Unique_Id_Feb6->SetTextSize(15);
    tGem2_Unique_Id_Feb6->SetTextAngle(73.3);

    tGem2_Unique_Id_Feb7->SetTextAlign(22);
    tGem2_Unique_Id_Feb7->SetTextColor(kRed + 2);
    tGem2_Unique_Id_Feb7->SetTextFont(43);
    tGem2_Unique_Id_Feb7->SetTextSize(15);
    tGem2_Unique_Id_Feb7->SetTextAngle(73.3);

    tGem2_Unique_Id_Feb8->SetTextAlign(22);
    tGem2_Unique_Id_Feb8->SetTextColor(kRed + 2);
    tGem2_Unique_Id_Feb8->SetTextFont(43);
    tGem2_Unique_Id_Feb8->SetTextSize(15);
    tGem2_Unique_Id_Feb8->SetTextAngle(73.3);

    tGem2_Unique_Id_Feb9->SetTextAlign(22);
    tGem2_Unique_Id_Feb9->SetTextColor(kRed + 2);
    tGem2_Unique_Id_Feb9->SetTextFont(43);
    tGem2_Unique_Id_Feb9->SetTextSize(15);
    tGem2_Unique_Id_Feb9->SetTextAngle(73.3);

    tGem2_Unique_Id_Feb10->SetTextAlign(22);
    tGem2_Unique_Id_Feb10->SetTextColor(kRed + 2);
    tGem2_Unique_Id_Feb10->SetTextFont(43);
    tGem2_Unique_Id_Feb10->SetTextSize(15);
    tGem2_Unique_Id_Feb10->SetTextAngle(73.3);

    tGem2_Unique_Id_Feb11->SetTextAlign(22);
    tGem2_Unique_Id_Feb11->SetTextColor(kRed + 2);
    tGem2_Unique_Id_Feb11->SetTextFont(43);
    tGem2_Unique_Id_Feb11->SetTextSize(15);
    tGem2_Unique_Id_Feb11->SetTextAngle(73.3);

    tGem2_Unique_Id_Feb12->SetTextAlign(22);
    tGem2_Unique_Id_Feb12->SetTextColor(kRed + 2);
    tGem2_Unique_Id_Feb12->SetTextFont(43);
    tGem2_Unique_Id_Feb12->SetTextSize(15);
    tGem2_Unique_Id_Feb12->SetTextAngle(73.3);

    tGem2_Unique_Id_Feb13->SetTextAlign(22);
    tGem2_Unique_Id_Feb13->SetTextColor(kRed + 2);
    tGem2_Unique_Id_Feb13->SetTextFont(43);
    tGem2_Unique_Id_Feb13->SetTextSize(15);
    tGem2_Unique_Id_Feb13->SetTextAngle(73.3);

    tGem2_Unique_Id_Feb14->SetTextAlign(22);
    tGem2_Unique_Id_Feb14->SetTextColor(kRed + 2);
    tGem2_Unique_Id_Feb14->SetTextFont(43);
    tGem2_Unique_Id_Feb14->SetTextSize(15);
    tGem2_Unique_Id_Feb14->SetTextAngle(73.3);

    tGem2_Unique_Id_Feb15->SetTextAlign(22);
    tGem2_Unique_Id_Feb15->SetTextColor(kRed + 2);
    tGem2_Unique_Id_Feb15->SetTextFont(43);
    tGem2_Unique_Id_Feb15->SetTextSize(15);
    tGem2_Unique_Id_Feb15->SetTextAngle(0);

    tGem2_Unique_Id_Feb16->SetTextAlign(22);
    tGem2_Unique_Id_Feb16->SetTextColor(kRed + 2);
    tGem2_Unique_Id_Feb16->SetTextFont(43);
    tGem2_Unique_Id_Feb16->SetTextSize(15);
    tGem2_Unique_Id_Feb16->SetTextAngle(0);

    tGem2_Unique_Id_Feb17->SetTextAlign(22);
    tGem2_Unique_Id_Feb17->SetTextColor(kRed + 2);
    tGem2_Unique_Id_Feb17->SetTextFont(43);
    tGem2_Unique_Id_Feb17->SetTextSize(15);
    tGem2_Unique_Id_Feb17->SetTextAngle(73.3);

    tGem2_CROB_Id_Feb0->SetTextAlign(22);
    tGem2_CROB_Id_Feb0->SetTextColor(kRed + 2);
    tGem2_CROB_Id_Feb0->SetTextFont(43);
    tGem2_CROB_Id_Feb0->SetTextSize(11);
    tGem2_CROB_Id_Feb0->SetTextAngle(73.3);

    tGem2_CROB_Id_Feb1->SetTextAlign(22);
    tGem2_CROB_Id_Feb1->SetTextColor(kRed + 2);
    tGem2_CROB_Id_Feb1->SetTextFont(43);
    tGem2_CROB_Id_Feb1->SetTextSize(11);
    tGem2_CROB_Id_Feb1->SetTextAngle(73.3);

    tGem2_CROB_Id_Feb2->SetTextAlign(22);
    tGem2_CROB_Id_Feb2->SetTextColor(kRed + 2);
    tGem2_CROB_Id_Feb2->SetTextFont(43);
    tGem2_CROB_Id_Feb2->SetTextSize(11);
    tGem2_CROB_Id_Feb2->SetTextAngle(73.3);

    tGem2_CROB_Id_Feb3->SetTextAlign(22);
    tGem2_CROB_Id_Feb3->SetTextColor(kRed + 2);
    tGem2_CROB_Id_Feb3->SetTextFont(43);
    tGem2_CROB_Id_Feb3->SetTextSize(11);
    tGem2_CROB_Id_Feb3->SetTextAngle(73.3);

    tGem2_CROB_Id_Feb4->SetTextAlign(22);
    tGem2_CROB_Id_Feb4->SetTextColor(kRed + 2);
    tGem2_CROB_Id_Feb4->SetTextFont(43);
    tGem2_CROB_Id_Feb4->SetTextSize(11);
    tGem2_CROB_Id_Feb4->SetTextAngle(73.3);

    tGem2_CROB_Id_Feb5->SetTextAlign(22);
    tGem2_CROB_Id_Feb5->SetTextColor(kRed + 2);
    tGem2_CROB_Id_Feb5->SetTextFont(43);
    tGem2_CROB_Id_Feb5->SetTextSize(11);
    tGem2_CROB_Id_Feb5->SetTextAngle(73.3);

    tGem2_CROB_Id_Feb6->SetTextAlign(22);
    tGem2_CROB_Id_Feb6->SetTextColor(kRed + 2);
    tGem2_CROB_Id_Feb6->SetTextFont(43);
    tGem2_CROB_Id_Feb6->SetTextSize(11);
    tGem2_CROB_Id_Feb6->SetTextAngle(73.3);

    tGem2_CROB_Id_Feb7->SetTextAlign(22);
    tGem2_CROB_Id_Feb7->SetTextColor(kRed + 2);
    tGem2_CROB_Id_Feb7->SetTextFont(43);
    tGem2_CROB_Id_Feb7->SetTextSize(11);
    tGem2_CROB_Id_Feb7->SetTextAngle(73.3);

    tGem2_CROB_Id_Feb8->SetTextAlign(22);
    tGem2_CROB_Id_Feb8->SetTextColor(kRed + 2);
    tGem2_CROB_Id_Feb8->SetTextFont(43);
    tGem2_CROB_Id_Feb8->SetTextSize(11);
    tGem2_CROB_Id_Feb8->SetTextAngle(73.3);

    tGem2_CROB_Id_Feb9->SetTextAlign(22);
    tGem2_CROB_Id_Feb9->SetTextColor(kRed + 2);
    tGem2_CROB_Id_Feb9->SetTextFont(43);
    tGem2_CROB_Id_Feb9->SetTextSize(11);
    tGem2_CROB_Id_Feb9->SetTextAngle(73.3);

    tGem2_CROB_Id_Feb10->SetTextAlign(22);
    tGem2_CROB_Id_Feb10->SetTextColor(kRed + 2);
    tGem2_CROB_Id_Feb10->SetTextFont(43);
    tGem2_CROB_Id_Feb10->SetTextSize(11);
    tGem2_CROB_Id_Feb10->SetTextAngle(73.3);

    tGem2_CROB_Id_Feb11->SetTextAlign(22);
    tGem2_CROB_Id_Feb11->SetTextColor(kRed + 2);
    tGem2_CROB_Id_Feb11->SetTextFont(43);
    tGem2_CROB_Id_Feb11->SetTextSize(11);
    tGem2_CROB_Id_Feb11->SetTextAngle(73.3);

    tGem2_CROB_Id_Feb12->SetTextAlign(22);
    tGem2_CROB_Id_Feb12->SetTextColor(kRed + 2);
    tGem2_CROB_Id_Feb12->SetTextFont(43);
    tGem2_CROB_Id_Feb12->SetTextSize(11);
    tGem2_CROB_Id_Feb12->SetTextAngle(73.3);

    tGem2_CROB_Id_Feb13->SetTextAlign(22);
    tGem2_CROB_Id_Feb13->SetTextColor(kRed + 2);
    tGem2_CROB_Id_Feb13->SetTextFont(43);
    tGem2_CROB_Id_Feb13->SetTextSize(11);
    tGem2_CROB_Id_Feb13->SetTextAngle(73.3);

    tGem2_CROB_Id_Feb14->SetTextAlign(22);
    tGem2_CROB_Id_Feb14->SetTextColor(kRed + 2);
    tGem2_CROB_Id_Feb14->SetTextFont(43);
    tGem2_CROB_Id_Feb14->SetTextSize(11);
    tGem2_CROB_Id_Feb14->SetTextAngle(73.3);

    tGem2_CROB_Id_Feb15->SetTextAlign(22);
    tGem2_CROB_Id_Feb15->SetTextColor(kRed + 2);
    tGem2_CROB_Id_Feb15->SetTextFont(43);
    tGem2_CROB_Id_Feb15->SetTextSize(11);
    tGem2_CROB_Id_Feb15->SetTextAngle(0);

    tGem2_CROB_Id_Feb16->SetTextAlign(22);
    tGem2_CROB_Id_Feb16->SetTextColor(kRed + 2);
    tGem2_CROB_Id_Feb16->SetTextFont(43);
    tGem2_CROB_Id_Feb16->SetTextSize(11);
    tGem2_CROB_Id_Feb16->SetTextAngle(0);

    tGem2_CROB_Id_Feb17->SetTextAlign(22);
    tGem2_CROB_Id_Feb17->SetTextColor(kRed + 2);
    tGem2_CROB_Id_Feb17->SetTextFont(43);
    tGem2_CROB_Id_Feb17->SetTextSize(11);
    tGem2_CROB_Id_Feb17->SetTextAngle(73.3);

    tGem2_VRefP_Feb0->SetTextAlign(22);
    tGem2_VRefP_Feb0->SetTextColor(kRed + 2);
    tGem2_VRefP_Feb0->SetTextFont(43);
    tGem2_VRefP_Feb0->SetTextSize(12);
    tGem2_VRefP_Feb0->SetTextAngle(73.3);

    tGem2_VRefP_Feb1->SetTextAlign(22);
    tGem2_VRefP_Feb1->SetTextColor(kRed + 2);
    tGem2_VRefP_Feb1->SetTextFont(43);
    tGem2_VRefP_Feb1->SetTextSize(12);
    tGem2_VRefP_Feb1->SetTextAngle(73.3);

    tGem2_VRefP_Feb2->SetTextAlign(22);
    tGem2_VRefP_Feb2->SetTextColor(kRed + 2);
    tGem2_VRefP_Feb2->SetTextFont(43);
    tGem2_VRefP_Feb2->SetTextSize(12);
    tGem2_VRefP_Feb2->SetTextAngle(73.3);

    tGem2_VRefP_Feb3->SetTextAlign(22);
    tGem2_VRefP_Feb3->SetTextColor(kRed + 2);
    tGem2_VRefP_Feb3->SetTextFont(43);
    tGem2_VRefP_Feb3->SetTextSize(12);
    tGem2_VRefP_Feb3->SetTextAngle(73.3);

    tGem2_VRefP_Feb4->SetTextAlign(22);
    tGem2_VRefP_Feb4->SetTextColor(kRed + 2);
    tGem2_VRefP_Feb4->SetTextFont(43);
    tGem2_VRefP_Feb4->SetTextSize(12);
    tGem2_VRefP_Feb4->SetTextAngle(73.3);

    tGem2_VRefP_Feb5->SetTextAlign(22);
    tGem2_VRefP_Feb5->SetTextColor(kRed + 2);
    tGem2_VRefP_Feb5->SetTextFont(43);
    tGem2_VRefP_Feb5->SetTextSize(12);
    tGem2_VRefP_Feb5->SetTextAngle(73.3);

    tGem2_VRefP_Feb6->SetTextAlign(22);
    tGem2_VRefP_Feb6->SetTextColor(kRed + 2);
    tGem2_VRefP_Feb6->SetTextFont(43);
    tGem2_VRefP_Feb6->SetTextSize(12);
    tGem2_VRefP_Feb6->SetTextAngle(73.3);

    tGem2_VRefP_Feb7->SetTextAlign(22);
    tGem2_VRefP_Feb7->SetTextColor(kRed + 2);
    tGem2_VRefP_Feb7->SetTextFont(43);
    tGem2_VRefP_Feb7->SetTextSize(12);
    tGem2_VRefP_Feb7->SetTextAngle(73.3);

    tGem2_VRefP_Feb8->SetTextAlign(22);
    tGem2_VRefP_Feb8->SetTextColor(kRed + 2);
    tGem2_VRefP_Feb8->SetTextFont(43);
    tGem2_VRefP_Feb8->SetTextSize(12);
    tGem2_VRefP_Feb8->SetTextAngle(73.3);

    tGem2_VRefP_Feb9->SetTextAlign(22);
    tGem2_VRefP_Feb9->SetTextColor(kRed + 2);
    tGem2_VRefP_Feb9->SetTextFont(43);
    tGem2_VRefP_Feb9->SetTextSize(12);
    tGem2_VRefP_Feb9->SetTextAngle(73.3);

    tGem2_VRefP_Feb10->SetTextAlign(22);
    tGem2_VRefP_Feb10->SetTextColor(kRed + 2);
    tGem2_VRefP_Feb10->SetTextFont(43);
    tGem2_VRefP_Feb10->SetTextSize(12);
    tGem2_VRefP_Feb10->SetTextAngle(73.3);

    tGem2_VRefP_Feb11->SetTextAlign(22);
    tGem2_VRefP_Feb11->SetTextColor(kRed + 2);
    tGem2_VRefP_Feb11->SetTextFont(43);
    tGem2_VRefP_Feb11->SetTextSize(12);
    tGem2_VRefP_Feb11->SetTextAngle(73.3);

    tGem2_VRefP_Feb12->SetTextAlign(22);
    tGem2_VRefP_Feb12->SetTextColor(kRed + 2);
    tGem2_VRefP_Feb12->SetTextFont(43);
    tGem2_VRefP_Feb12->SetTextSize(12);
    tGem2_VRefP_Feb12->SetTextAngle(73.3);

    tGem2_VRefP_Feb13->SetTextAlign(22);
    tGem2_VRefP_Feb13->SetTextColor(kRed + 2);
    tGem2_VRefP_Feb13->SetTextFont(43);
    tGem2_VRefP_Feb13->SetTextSize(12);
    tGem2_VRefP_Feb13->SetTextAngle(73.3);

    tGem2_VRefP_Feb14->SetTextAlign(22);
    tGem2_VRefP_Feb14->SetTextColor(kRed + 2);
    tGem2_VRefP_Feb14->SetTextFont(43);
    tGem2_VRefP_Feb14->SetTextSize(12);
    tGem2_VRefP_Feb14->SetTextAngle(73.3);

    tGem2_VRefP_Feb15->SetTextAlign(22);
    tGem2_VRefP_Feb15->SetTextColor(kRed + 2);
    tGem2_VRefP_Feb15->SetTextFont(43);
    tGem2_VRefP_Feb15->SetTextSize(12);
    tGem2_VRefP_Feb15->SetTextAngle(0);

    tGem2_VRefP_Feb16->SetTextAlign(22);
    tGem2_VRefP_Feb16->SetTextColor(kRed + 2);
    tGem2_VRefP_Feb16->SetTextFont(43);
    tGem2_VRefP_Feb16->SetTextSize(12);
    tGem2_VRefP_Feb16->SetTextAngle(0);

    tGem2_VRefP_Feb17->SetTextAlign(22);
    tGem2_VRefP_Feb17->SetTextColor(kRed + 2);
    tGem2_VRefP_Feb17->SetTextFont(43);
    tGem2_VRefP_Feb17->SetTextSize(12);
    tGem2_VRefP_Feb17->SetTextAngle(73.3);

    cConnector_Gem1_Feb_Unique_Number->cd(1);

    tTitle_Gem1_Feb_no->SetTextAlign(22);
    tTitle_Gem1_Feb_no->Draw();

    tGem1_Feb0->Draw();
    tGem1_Feb1->Draw();
    tGem1_Feb2->Draw();
    tGem1_Feb3->Draw();
    tGem1_Feb4->Draw();
    tGem1_Feb5->Draw();
    tGem1_Feb6->Draw();
    tGem1_Feb7->Draw();
    tGem1_Feb8->Draw();
    tGem1_Feb9->Draw();
    tGem1_Feb10->Draw();
    tGem1_Feb11->Draw();
    tGem1_Feb12->Draw();
    tGem1_Feb13->Draw();
    tGem1_Feb14->Draw();
    tGem1_Feb15->Draw();
    tGem1_Feb16->Draw();
    tGem1_Feb17->Draw();

    cConnector_Gem1_Feb_Unique_Number->cd(2);

    tTitle_Gem1_Unique_Id_Feb->Draw();

    tGem1_Unique_Id_Feb0->Draw();
    tGem1_Unique_Id_Feb1->Draw();
    tGem1_Unique_Id_Feb2->Draw();
    tGem1_Unique_Id_Feb3->Draw();
    tGem1_Unique_Id_Feb4->Draw();
    tGem1_Unique_Id_Feb5->Draw();
    tGem1_Unique_Id_Feb6->Draw();
    tGem1_Unique_Id_Feb7->Draw();
    tGem1_Unique_Id_Feb8->Draw();
    tGem1_Unique_Id_Feb9->Draw();
    tGem1_Unique_Id_Feb10->Draw();
    tGem1_Unique_Id_Feb11->Draw();
    tGem1_Unique_Id_Feb12->Draw();
    tGem1_Unique_Id_Feb13->Draw();
    tGem1_Unique_Id_Feb14->Draw();
    tGem1_Unique_Id_Feb15->Draw();
    tGem1_Unique_Id_Feb16->Draw();
    tGem1_Unique_Id_Feb17->Draw();

    cConnector_Gem1_Feb_Unique_Number->cd();

    cConnector_Gem1_Feb_CROB_Number->cd(1);

    tTitle_Gem1_Feb_no->SetTextAlign(22);
    tTitle_Gem1_Feb_no->Draw();

    tGem1_Feb0->Draw();
    tGem1_Feb1->Draw();
    tGem1_Feb2->Draw();
    tGem1_Feb3->Draw();
    tGem1_Feb4->Draw();
    tGem1_Feb5->Draw();
    tGem1_Feb6->Draw();
    tGem1_Feb7->Draw();
    tGem1_Feb8->Draw();
    tGem1_Feb9->Draw();
    tGem1_Feb10->Draw();
    tGem1_Feb11->Draw();
    tGem1_Feb12->Draw();
    tGem1_Feb13->Draw();
    tGem1_Feb14->Draw();
    tGem1_Feb15->Draw();
    tGem1_Feb16->Draw();
    tGem1_Feb17->Draw();

    cConnector_Gem1_Feb_CROB_Number->cd(2);

    tTitle_Gem1_CROB_Id_Feb->Draw();

    tGem1_CROB_Id_Feb0->Draw();
    tGem1_CROB_Id_Feb1->Draw();
    tGem1_CROB_Id_Feb2->Draw();
    tGem1_CROB_Id_Feb3->Draw();
    tGem1_CROB_Id_Feb4->Draw();
    tGem1_CROB_Id_Feb5->Draw();
    tGem1_CROB_Id_Feb6->Draw();
    tGem1_CROB_Id_Feb7->Draw();
    tGem1_CROB_Id_Feb8->Draw();
    tGem1_CROB_Id_Feb9->Draw();
    tGem1_CROB_Id_Feb10->Draw();
    tGem1_CROB_Id_Feb11->Draw();
    tGem1_CROB_Id_Feb12->Draw();
    tGem1_CROB_Id_Feb13->Draw();
    tGem1_CROB_Id_Feb14->Draw();
    tGem1_CROB_Id_Feb15->Draw();
    tGem1_CROB_Id_Feb16->Draw();
    tGem1_CROB_Id_Feb17->Draw();

    cConnector_Gem1_Feb_CROB_Number->cd();

    cConnector_Gem1_Feb_VRefP_Values->cd(1);

    tTitle_Gem1_Feb_no->SetTextAlign(22);
    tTitle_Gem1_Feb_no->Draw();

    tGem1_Feb0->Draw();
    tGem1_Feb1->Draw();
    tGem1_Feb2->Draw();
    tGem1_Feb3->Draw();
    tGem1_Feb4->Draw();
    tGem1_Feb5->Draw();
    tGem1_Feb6->Draw();
    tGem1_Feb7->Draw();
    tGem1_Feb8->Draw();
    tGem1_Feb9->Draw();
    tGem1_Feb10->Draw();
    tGem1_Feb11->Draw();
    tGem1_Feb12->Draw();
    tGem1_Feb13->Draw();
    tGem1_Feb14->Draw();
    tGem1_Feb15->Draw();
    tGem1_Feb16->Draw();
    tGem1_Feb17->Draw();

    cConnector_Gem1_Feb_VRefP_Values->cd(2);

    tTitle_Gem1_VRefP_Feb->Draw();

    tGem1_VRefP_Feb0->Draw();
    tGem1_VRefP_Feb1->Draw();
    tGem1_VRefP_Feb2->Draw();
    tGem1_VRefP_Feb3->Draw();
    tGem1_VRefP_Feb4->Draw();
    tGem1_VRefP_Feb5->Draw();
    tGem1_VRefP_Feb6->Draw();
    tGem1_VRefP_Feb7->Draw();
    tGem1_VRefP_Feb8->Draw();
    tGem1_VRefP_Feb9->Draw();
    tGem1_VRefP_Feb10->Draw();
    tGem1_VRefP_Feb11->Draw();
    tGem1_VRefP_Feb12->Draw();
    tGem1_VRefP_Feb13->Draw();
    tGem1_VRefP_Feb14->Draw();
    tGem1_VRefP_Feb15->Draw();
    tGem1_VRefP_Feb16->Draw();
    tGem1_VRefP_Feb17->Draw();

    cConnector_Gem1_Feb_VRefP_Values->cd();

    cConnector_Gem2_Feb_Unique_Number->cd(1);

    tTitle_Gem2_Feb_no->SetTextAlign(22);
    tTitle_Gem2_Feb_no->Draw();

    tGem2_Feb0->Draw();
    tGem2_Feb1->Draw();
    tGem2_Feb2->Draw();
    tGem2_Feb3->Draw();
    tGem2_Feb4->Draw();
    tGem2_Feb5->Draw();
    tGem2_Feb6->Draw();
    tGem2_Feb7->Draw();
    tGem2_Feb8->Draw();
    tGem2_Feb9->Draw();
    tGem2_Feb10->Draw();
    tGem2_Feb11->Draw();
    tGem2_Feb12->Draw();
    tGem2_Feb13->Draw();
    tGem2_Feb14->Draw();
    tGem2_Feb15->Draw();
    tGem2_Feb16->Draw();
    tGem2_Feb17->Draw();

    cConnector_Gem2_Feb_Unique_Number->cd(2);

    tTitle_Gem2_Unique_Id_Feb->Draw();

    tGem2_Unique_Id_Feb0->Draw();
    tGem2_Unique_Id_Feb1->Draw();
    tGem2_Unique_Id_Feb2->Draw();
    tGem2_Unique_Id_Feb3->Draw();
    tGem2_Unique_Id_Feb4->Draw();
    tGem2_Unique_Id_Feb5->Draw();
    tGem2_Unique_Id_Feb6->Draw();
    tGem2_Unique_Id_Feb7->Draw();
    tGem2_Unique_Id_Feb8->Draw();
    tGem2_Unique_Id_Feb9->Draw();
    tGem2_Unique_Id_Feb10->Draw();
    tGem2_Unique_Id_Feb11->Draw();
    tGem2_Unique_Id_Feb12->Draw();
    tGem2_Unique_Id_Feb13->Draw();
    tGem2_Unique_Id_Feb14->Draw();
    tGem2_Unique_Id_Feb15->Draw();
    tGem2_Unique_Id_Feb16->Draw();
    tGem2_Unique_Id_Feb17->Draw();

    cConnector_Gem2_Feb_Unique_Number->cd();

    cConnector_Gem2_Feb_CROB_Number->cd(1);

    tTitle_Gem2_Feb_no->SetTextAlign(22);
    tTitle_Gem2_Feb_no->Draw();

    tGem2_Feb0->Draw();
    tGem2_Feb1->Draw();
    tGem2_Feb2->Draw();
    tGem2_Feb3->Draw();
    tGem2_Feb4->Draw();
    tGem2_Feb5->Draw();
    tGem2_Feb6->Draw();
    tGem2_Feb7->Draw();
    tGem2_Feb8->Draw();
    tGem2_Feb9->Draw();
    tGem2_Feb10->Draw();
    tGem2_Feb11->Draw();
    tGem2_Feb12->Draw();
    tGem2_Feb13->Draw();
    tGem2_Feb14->Draw();
    tGem2_Feb15->Draw();
    tGem2_Feb16->Draw();
    tGem2_Feb17->Draw();

    cConnector_Gem2_Feb_CROB_Number->cd(2);

    tTitle_Gem2_CROB_Id_Feb->Draw();

    tGem2_CROB_Id_Feb0->Draw();
    tGem2_CROB_Id_Feb1->Draw();
    tGem2_CROB_Id_Feb2->Draw();
    tGem2_CROB_Id_Feb3->Draw();
    tGem2_CROB_Id_Feb4->Draw();
    tGem2_CROB_Id_Feb5->Draw();
    tGem2_CROB_Id_Feb6->Draw();
    tGem2_CROB_Id_Feb7->Draw();
    tGem2_CROB_Id_Feb8->Draw();
    tGem2_CROB_Id_Feb9->Draw();
    tGem2_CROB_Id_Feb10->Draw();
    tGem2_CROB_Id_Feb11->Draw();
    tGem2_CROB_Id_Feb12->Draw();
    tGem2_CROB_Id_Feb13->Draw();
    tGem2_CROB_Id_Feb14->Draw();
    tGem2_CROB_Id_Feb15->Draw();
    tGem2_CROB_Id_Feb16->Draw();
    tGem2_CROB_Id_Feb17->Draw();

    cConnector_Gem2_Feb_CROB_Number->cd();

    cConnector_Gem2_Feb_VRefP_Values->cd(1);

    tTitle_Gem2_Feb_no->SetTextAlign(22);
    tTitle_Gem2_Feb_no->Draw();

    tGem2_Feb0->Draw();
    tGem2_Feb1->Draw();
    tGem2_Feb2->Draw();
    tGem2_Feb3->Draw();
    tGem2_Feb4->Draw();
    tGem2_Feb5->Draw();
    tGem2_Feb6->Draw();
    tGem2_Feb7->Draw();
    tGem2_Feb8->Draw();
    tGem2_Feb9->Draw();
    tGem2_Feb10->Draw();
    tGem2_Feb11->Draw();
    tGem2_Feb12->Draw();
    tGem2_Feb13->Draw();
    tGem2_Feb14->Draw();
    tGem2_Feb15->Draw();
    tGem2_Feb16->Draw();
    tGem2_Feb17->Draw();

    cConnector_Gem2_Feb_VRefP_Values->cd(2);

    tTitle_Gem2_VRefP_Feb->Draw();

    tGem2_VRefP_Feb0->Draw();
    tGem2_VRefP_Feb1->Draw();
    tGem2_VRefP_Feb2->Draw();
    tGem2_VRefP_Feb3->Draw();
    tGem2_VRefP_Feb4->Draw();
    tGem2_VRefP_Feb5->Draw();
    tGem2_VRefP_Feb6->Draw();
    tGem2_VRefP_Feb7->Draw();
    tGem2_VRefP_Feb8->Draw();
    tGem2_VRefP_Feb9->Draw();
    tGem2_VRefP_Feb10->Draw();
    tGem2_VRefP_Feb11->Draw();
    tGem2_VRefP_Feb12->Draw();
    tGem2_VRefP_Feb13->Draw();
    tGem2_VRefP_Feb14->Draw();
    tGem2_VRefP_Feb15->Draw();
    tGem2_VRefP_Feb16->Draw();
    tGem2_VRefP_Feb17->Draw();

    cConnector_Gem2_Feb_VRefP_Values->cd();
}
