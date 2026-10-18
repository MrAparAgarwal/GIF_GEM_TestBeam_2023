// -----------------------------------------------------------------------------
// Copyright (C) Experimental High Energy Physics and Applications Group, VECC, Kolkata
// Draw_CROB_Info_GIF2023.C
// Project : GEM detector test beam at CERN GIF++ (2023)
// Author  : Apar Agarwal
// Usage   : root -l -b -q 'Draw_CROB_Info_GIF2023.C'  (see README for input file layout)
// -----------------------------------------------------------------------------

void Draw_CROB_Info_GIF2023()
{
    TString Gem_CROB_Image_String = "CROB FMC.png";
    TImage* Gem_CROB_FMC_image = TImage::Open(Gem_CROB_Image_String);

    Gem_CROB_FMC_image->SetConstRatio(kFALSE);

    TCanvas* cCROB_1001 = new TCanvas("CROB_1001", "CROB_1001", 1100, 680);
    cCROB_1001->Divide(3, 1);
    cCROB_1001->cd(1);
    Gem_CROB_FMC_image->Draw();
    cCROB_1001->cd(2);
    Gem_CROB_FMC_image->Draw();
    cCROB_1001->cd(3);
    Gem_CROB_FMC_image->Draw();

    TCanvas* cCROB_1002 = new TCanvas("CROB_1002", "CROB_1002", 1100, 680);
    cCROB_1002->Divide(3, 1);
    cCROB_1002->cd(1);
    Gem_CROB_FMC_image->Draw();
    cCROB_1002->cd(2);
    Gem_CROB_FMC_image->Draw();
    cCROB_1002->cd(3);
    Gem_CROB_FMC_image->Draw();

    TCanvas* cCROB_1003 = new TCanvas("CROB_1003", "CROB_1003", 1100, 680);
    cCROB_1003->Divide(3, 1);
    cCROB_1003->cd(1);
    Gem_CROB_FMC_image->Draw();
    cCROB_1003->cd(2);
    Gem_CROB_FMC_image->Draw();
    cCROB_1003->cd(3);
    Gem_CROB_FMC_image->Draw();

    TCanvas* cCROB_2001 = new TCanvas("CROB_2001", "CROB_2001", 1100, 680);
    cCROB_2001->Divide(3, 1);
    cCROB_2001->cd(1);
    Gem_CROB_FMC_image->Draw();
    cCROB_2001->cd(2);
    Gem_CROB_FMC_image->Draw();
    cCROB_2001->cd(3);
    Gem_CROB_FMC_image->Draw();

    TCanvas* cCROB_2002 = new TCanvas("CROB_2002", "CROB_2002", 1100, 680);
    cCROB_2002->Divide(3, 1);
    cCROB_2002->cd(1);
    Gem_CROB_FMC_image->Draw();
    cCROB_2002->cd(2);
    Gem_CROB_FMC_image->Draw();
    cCROB_2002->cd(3);
    Gem_CROB_FMC_image->Draw();

    TCanvas* cCROB_2003 = new TCanvas("CROB_2003", "CROB_2003", 1100, 680);
    cCROB_2003->Divide(3, 1);
    cCROB_2003->cd(1);
    Gem_CROB_FMC_image->Draw();
    cCROB_2003->cd(2);
    Gem_CROB_FMC_image->Draw();
    cCROB_2003->cd(3);
    Gem_CROB_FMC_image->Draw();

    TString Text_String = "CROB 1001 Connector Number(Run " + to_string(RunId) + ")";
    TText* tTitle_CROB1001_Connector_No = new TText(0.01, 1.06, Text_String);
    TText* tCROB1001_Connector0 = new TText(0.770, 0.384, "C 1"); //"Pos 0"          --Only Change Comments
    TText* tCROB1001_Connector1 = new TText(0.477, 0.384, "C 2"); //"Pos 1"          --Only Change Comments
    TText* tCROB1001_Connector2 = new TText(0.150, 0.384, "C 3"); //"Pos 2"          --Only Change Comments
    TText* tCROB1001_Connector3 = new TText(0.770, 0.571, "C 4"); //"Pos 3"          --Only Change Comments
    TText* tCROB1001_Connector4 = new TText(0.477, 0.571, "C 5"); //"Pos 4"          --Only Change Comments
    TText* tCROB1001_Connector5 = new TText(0.150, 0.571, "C 6"); //"Pos 5"          --Only Change Comments
    TText* tCROB1001_Connector6 = new TText(0.770, 0.739, "C 7"); //"Pos 6"          --Only Change Comments
    TText* tCROB1001_Connector7 = new TText(0.477, 0.739, "C 8"); //"Pos 7"          --Only Change Comments
    TText* tCROB1001_Connector8 = new TText(0.150, 0.739, "C 9"); //"Pos 8"          --Only Change Comments

    Text_String = "CROB 1001 Elink Positions(Run " + to_string(RunId) + ")";
    TText* tTitle_CROB1001_ELink_No = new TText(0.01, 1.06, Text_String);
    TText* tCROB1001_ELink0 = new TText(0.736, 0.384, "33->37"); //"Pos 0"           --Only Change Comments
    TText* tCROB1001_ELink1 = new TText(0.446, 0.384, "5->9");   //"Pos 1"           --Only Change Comments
    TText* tCROB1001_ELink2 = new TText(0.150, 0.384, "0->4");   //"Pos 2"           --Only Change Comments
    TText* tCROB1001_ELink3 = new TText(0.736, 0.571, "28->32"); //"Pos 3"           --Only Change Comments
    TText* tCROB1001_ELink4 = new TText(0.446, 0.571, "14->18"); //"Pos 4"           --Only Change Comments
    TText* tCROB1001_ELink5 = new TText(0.150, 0.571, "10->13"); //"Pos 5"           --Only Change Comments
    TText* tCROB1001_ELink6 = new TText(0.736, 0.739, "38->41"); //"Pos 6"           --Only Change Comments
    TText* tCROB1001_ELink7 = new TText(0.446, 0.739, "19->23"); //"Pos 7"           --Only Change Comments
    TText* tCROB1001_ELink8 = new TText(0.150, 0.739, "24->27"); //"Pos 8"           --Only Change Comments

    Text_String = "CROB 1001 Feb Connections(Run " + to_string(RunId) + ")";
    TText* tTitle_CROB1001_Feb_Connections_No = new TText(0.01, 1.06, Text_String);
    TText* tCROB1001_Feb_Connector0 = new TText(0.704, 0.384, "G1 Pos11"); //"Pos 0"
    TText* tCROB1001_Feb_Connector1 = new TText(0.43, 0.384, "G1 Pos8");   //"Pos 1"
    TText* tCROB1001_Feb_Connector2 = new TText(0.122, 0.384, "G1 Pos7");  //"Pos 2"
    TText* tCROB1001_Feb_Connector3 = new TText(0.704, 0.571, "G1 Pos14"); //"Pos 3"
    TText* tCROB1001_Feb_Connector4 = new TText(0.43, 0.571, "G1 Pos10");  //"Pos 4"
    TText* tCROB1001_Feb_Connector5 = new TText(0.122, 0.571, "XXXXX");    //"Pos 5"
    TText* tCROB1001_Feb_Connector6 = new TText(0.704, 0.739, "XXXXX");    //"Pos 6"
    TText* tCROB1001_Feb_Connector7 = new TText(0.43, 0.739, "G1 Pos17");  //"Pos 7"
    TText* tCROB1001_Feb_Connector8 = new TText(0.122, 0.739, "G1 Pos9");  //"Pos 8"

    Text_String = "CROB 1002 Connector Number(Run " + to_string(RunId) + ")";
    TText* tTitle_CROB1002_Connector_No = new TText(0.01, 1.06, Text_String);
    TText* tCROB1002_Connector0 = new TText(0.770, 0.384, "C 1"); //"Pos 0"
    TText* tCROB1002_Connector1 = new TText(0.477, 0.384, "C 2"); //"Pos 1"
    TText* tCROB1002_Connector2 = new TText(0.150, 0.384, "C 3"); //"Pos 2"
    TText* tCROB1002_Connector3 = new TText(0.770, 0.571, "C 4"); //"Pos 3"
    TText* tCROB1002_Connector4 = new TText(0.477, 0.571, "C 5"); //"Pos 4"
    TText* tCROB1002_Connector5 = new TText(0.150, 0.571, "C 6"); //"Pos 5"
    TText* tCROB1002_Connector6 = new TText(0.770, 0.739, "C 7"); //"Pos 6"
    TText* tCROB1002_Connector7 = new TText(0.477, 0.739, "C 8"); //"Pos 7"
    TText* tCROB1002_Connector8 = new TText(0.150, 0.739, "C 9"); //"Pos 8"

    Text_String = "CROB 1002 Elink Positions(Run " + to_string(RunId) + ")";
    TText* tTitle_CROB1002_ELink_No = new TText(0.01, 1.06, Text_String);
    TText* tCROB1002_ELink0 = new TText(0.736, 0.384, "33->37"); //"Pos 0"
    TText* tCROB1002_ELink1 = new TText(0.446, 0.384, "5->9");   //"Pos 1"
    TText* tCROB1002_ELink2 = new TText(0.150, 0.384, "0->4");   //"Pos 2"
    TText* tCROB1002_ELink3 = new TText(0.736, 0.571, "28->32"); //"Pos 3"
    TText* tCROB1002_ELink4 = new TText(0.446, 0.571, "14->18"); //"Pos 4"
    TText* tCROB1002_ELink5 = new TText(0.150, 0.571, "10->13"); //"Pos 5"
    TText* tCROB1002_ELink6 = new TText(0.736, 0.739, "38->41"); //"Pos 6"
    TText* tCROB1002_ELink7 = new TText(0.446, 0.739, "19->23"); //"Pos 7"
    TText* tCROB1002_ELink8 = new TText(0.150, 0.739, "24->27"); //"Pos 8"

    Text_String = "CROB 1002 Feb Connections(Run " + to_string(RunId) + ")";
    TText* tTitle_CROB1002_Feb_Connections_No = new TText(0.01, 1.06, Text_String);
    TText* tCROB1002_Feb_Connector0 = new TText(0.704, 0.384, "G1 Pos15"); //"Pos 0"
    TText* tCROB1002_Feb_Connector1 = new TText(0.43, 0.384, "G1 Pos5");   //"Pos 1"
    TText* tCROB1002_Feb_Connector2 = new TText(0.122, 0.384, "XXXXX");    //"Pos 2"
    TText* tCROB1002_Feb_Connector3 = new TText(0.704, 0.571, "XXXXX");    //"Pos 3"
    TText* tCROB1002_Feb_Connector4 = new TText(0.43, 0.571, "G1 Pos16");  //"Pos 4"
    TText* tCROB1002_Feb_Connector5 = new TText(0.122, 0.571, "XXXXX");    //"Pos 5"
    TText* tCROB1002_Feb_Connector6 = new TText(0.704, 0.739, "XXXXX");    //"Pos 6"
    TText* tCROB1002_Feb_Connector7 = new TText(0.43, 0.739, "G1 Pos13");  //"Pos 7"
    TText* tCROB1002_Feb_Connector8 = new TText(0.122, 0.739, "G1 Pos12"); //"Pos 8"

    Text_String = "CROB 1003 Connector Number(Run " + to_string(RunId) + ")";
    TText* tTitle_CROB1003_Connector_No = new TText(0.01, 1.06, Text_String);
    TText* tCROB1003_Connector0 = new TText(0.770, 0.384, "C 1"); //"Pos 0"
    TText* tCROB1003_Connector1 = new TText(0.477, 0.384, "C 2"); //"Pos 1"
    TText* tCROB1003_Connector2 = new TText(0.150, 0.384, "C 3"); //"Pos 2"
    TText* tCROB1003_Connector3 = new TText(0.770, 0.571, "C 4"); //"Pos 3"
    TText* tCROB1003_Connector4 = new TText(0.477, 0.571, "C 5"); //"Pos 4"
    TText* tCROB1003_Connector5 = new TText(0.150, 0.571, "C 6"); //"Pos 5"
    TText* tCROB1003_Connector6 = new TText(0.770, 0.739, "C 7"); //"Pos 6"
    TText* tCROB1003_Connector7 = new TText(0.477, 0.739, "C 8"); //"Pos 7"
    TText* tCROB1003_Connector8 = new TText(0.150, 0.739, "C 9"); //"Pos 8"

    Text_String = "CROB 1003 Elink Positions(Run " + to_string(RunId) + ")";
    TText* tTitle_CROB1003_ELink_No = new TText(0.01, 1.06, Text_String);
    TText* tCROB1003_ELink0 = new TText(0.736, 0.384, "33->37"); //"Pos 0"
    TText* tCROB1003_ELink1 = new TText(0.446, 0.384, "5->9");   //"Pos 1"
    TText* tCROB1003_ELink2 = new TText(0.150, 0.384, "0->4");   //"Pos 2"
    TText* tCROB1003_ELink3 = new TText(0.736, 0.571, "28->32"); //"Pos 3"
    TText* tCROB1003_ELink4 = new TText(0.446, 0.571, "14->18"); //"Pos 4"
    TText* tCROB1003_ELink5 = new TText(0.150, 0.571, "10->13"); //"Pos 5"
    TText* tCROB1003_ELink6 = new TText(0.736, 0.739, "38->41"); //"Pos 6"
    TText* tCROB1003_ELink7 = new TText(0.446, 0.739, "19->23"); //"Pos 7"
    TText* tCROB1003_ELink8 = new TText(0.150, 0.739, "24->27"); //"Pos 8"

    Text_String = "CROB 1003 Feb Connections(Run " + to_string(RunId) + ")";
    TText* tTitle_CROB1003_Feb_Connections_No = new TText(0.01, 1.06, Text_String);
    TText* tCROB1003_Feb_Connector0 = new TText(0.704, 0.384, "Trigger"); //"Pos 0"
    TText* tCROB1003_Feb_Connector1 = new TText(0.43, 0.384, "XXXXX");    //"Pos 1"
    TText* tCROB1003_Feb_Connector2 = new TText(0.122, 0.384, "XXXXX");   //"Pos 2"
    TText* tCROB1003_Feb_Connector3 = new TText(0.704, 0.571, "XXXXX");   //"Pos 3"
    TText* tCROB1003_Feb_Connector4 = new TText(0.43, 0.571, "G1 Pos4");  //"Pos 4"
    TText* tCROB1003_Feb_Connector5 = new TText(0.122, 0.571, "XXXXX");   //"Pos 5"
    TText* tCROB1003_Feb_Connector6 = new TText(0.704, 0.739, "XXXXX");   //"Pos 6"
    TText* tCROB1003_Feb_Connector7 = new TText(0.43, 0.739, "G1 Pos3");  //"Pos 7"
    TText* tCROB1003_Feb_Connector8 = new TText(0.122, 0.739, "XXXXX");   //"Pos 8"

    Text_String = "CROB 2001 Connector Number(Run " + to_string(RunId) + ")";
    TText* tTitle_CROB2001_Connector_No = new TText(0.01, 1.06, Text_String);
    TText* tCROB2001_Connector0 = new TText(0.770, 0.384, "C 1"); //"Pos 0"
    TText* tCROB2001_Connector1 = new TText(0.477, 0.384, "C 2"); //"Pos 1"
    TText* tCROB2001_Connector2 = new TText(0.150, 0.384, "C 3"); //"Pos 2"
    TText* tCROB2001_Connector3 = new TText(0.770, 0.571, "C 4"); //"Pos 3"
    TText* tCROB2001_Connector4 = new TText(0.477, 0.571, "C 5"); //"Pos 4"
    TText* tCROB2001_Connector5 = new TText(0.150, 0.571, "C 6"); //"Pos 5"
    TText* tCROB2001_Connector6 = new TText(0.770, 0.739, "C 7"); //"Pos 6"
    TText* tCROB2001_Connector7 = new TText(0.477, 0.739, "C 8"); //"Pos 7"
    TText* tCROB2001_Connector8 = new TText(0.150, 0.739, "C 9"); //"Pos 8"

    Text_String = "CROB 2001 Elink Positions(Run " + to_string(RunId) + ")";
    TText* tTitle_CROB2001_ELink_No = new TText(0.01, 1.06, Text_String);
    TText* tCROB2001_ELink0 = new TText(0.736, 0.384, "33->37"); //"Pos 0"
    TText* tCROB2001_ELink1 = new TText(0.446, 0.384, "5->9");   //"Pos 1"
    TText* tCROB2001_ELink2 = new TText(0.150, 0.384, "0->4");   //"Pos 2"
    TText* tCROB2001_ELink3 = new TText(0.736, 0.571, "28->32"); //"Pos 3"
    TText* tCROB2001_ELink4 = new TText(0.446, 0.571, "14->18"); //"Pos 4"
    TText* tCROB2001_ELink5 = new TText(0.150, 0.571, "10->13"); //"Pos 5"
    TText* tCROB2001_ELink6 = new TText(0.736, 0.739, "38->41"); //"Pos 6"
    TText* tCROB2001_ELink7 = new TText(0.446, 0.739, "19->23"); //"Pos 7"
    TText* tCROB2001_ELink8 = new TText(0.150, 0.739, "24->27"); //"Pos 8"

    Text_String = "CROB 2001 Feb Connections(Run " + to_string(RunId) + ")";
    TText* tTitle_CROB2001_Feb_Connections_No = new TText(0.01, 1.06, Text_String);
    TText* tCROB2001_Feb_Connector0 = new TText(0.704, 0.384, "G2 Pos2"); //"Pos 0"
    TText* tCROB2001_Feb_Connector1 = new TText(0.43, 0.384, "G2 Pos14"); //"Pos 1"
    TText* tCROB2001_Feb_Connector2 = new TText(0.122, 0.384, "G2 Pos1"); //"Pos 2"
    TText* tCROB2001_Feb_Connector3 = new TText(0.704, 0.571, "XXXXX");   //"Pos 3"
    TText* tCROB2001_Feb_Connector4 = new TText(0.43, 0.571, "XXXXX");    //"Pos 4"
    TText* tCROB2001_Feb_Connector5 = new TText(0.122, 0.571, "XXXXX");   //"Pos 5"
    TText* tCROB2001_Feb_Connector6 = new TText(0.704, 0.739, "G2 Pos6"); //"Pos 6"
    TText* tCROB2001_Feb_Connector7 = new TText(0.43, 0.739, "G2 Pos7");  //"Pos 7"
    TText* tCROB2001_Feb_Connector8 = new TText(0.122, 0.739, "XXXXX");   //"Pos 8"


    Text_String = "CROB 2002 Connector Number(Run " + to_string(RunId) + ")";
    TText* tTitle_CROB2002_Connector_No = new TText(0.01, 1.06, Text_String);
    TText* tCROB2002_Connector0 = new TText(0.770, 0.384, "C 1"); //"Pos 0"
    TText* tCROB2002_Connector1 = new TText(0.477, 0.384, "C 2"); //"Pos 1"
    TText* tCROB2002_Connector2 = new TText(0.150, 0.384, "C 3"); //"Pos 2"
    TText* tCROB2002_Connector3 = new TText(0.770, 0.571, "C 4"); //"Pos 3"
    TText* tCROB2002_Connector4 = new TText(0.477, 0.571, "C 5"); //"Pos 4"
    TText* tCROB2002_Connector5 = new TText(0.150, 0.571, "C 6"); //"Pos 5"
    TText* tCROB2002_Connector6 = new TText(0.770, 0.739, "C 7"); //"Pos 6"
    TText* tCROB2002_Connector7 = new TText(0.477, 0.739, "C 8"); //"Pos 7"
    TText* tCROB2002_Connector8 = new TText(0.150, 0.739, "C 9"); //"Pos 8"

    Text_String = "CROB 2002 Elink Positions(Run " + to_string(RunId) + ")";
    TText* tTitle_CROB2002_ELink_No = new TText(0.01, 1.06, Text_String);
    TText* tCROB2002_ELink0 = new TText(0.736, 0.384, "33->37"); //"Pos 0"
    TText* tCROB2002_ELink1 = new TText(0.446, 0.384, "5->9");   //"Pos 1"
    TText* tCROB2002_ELink2 = new TText(0.150, 0.384, "0->4");   //"Pos 2"
    TText* tCROB2002_ELink3 = new TText(0.736, 0.571, "28->32"); //"Pos 3"
    TText* tCROB2002_ELink4 = new TText(0.446, 0.571, "14->18"); //"Pos 4"
    TText* tCROB2002_ELink5 = new TText(0.150, 0.571, "10->13"); //"Pos 5"
    TText* tCROB2002_ELink6 = new TText(0.736, 0.739, "38->41"); //"Pos 6"
    TText* tCROB2002_ELink7 = new TText(0.446, 0.739, "19->23"); //"Pos 7"
    TText* tCROB2002_ELink8 = new TText(0.150, 0.739, "24->27"); //"Pos 8"

    Text_String = "CROB 2002 Feb Connections(Run " + to_string(RunId) + ")";
    TText* tTitle_CROB2002_Feb_Connections_No = new TText(0.01, 1.06, Text_String);
    TText* tCROB2002_Feb_Connector0 = new TText(0.704, 0.384, "G2 Pos8");  //"Pos 0"
    TText* tCROB2002_Feb_Connector1 = new TText(0.43, 0.384, "G2 Pos5");   //"Pos 1"
    TText* tCROB2002_Feb_Connector2 = new TText(0.122, 0.384, "G2 Pos15"); //"Pos 2"
    TText* tCROB2002_Feb_Connector3 = new TText(0.704, 0.571, "XXXXX");    //"Pos 3"
    TText* tCROB2002_Feb_Connector4 = new TText(0.43, 0.571, "XXXXX");     //"Pos 4"
    TText* tCROB2002_Feb_Connector5 = new TText(0.122, 0.571, "G2 Pos9");  //"Pos 5"
    TText* tCROB2002_Feb_Connector6 = new TText(0.704, 0.739, "G2 Pos0");  //"Pos 6"
    TText* tCROB2002_Feb_Connector7 = new TText(0.43, 0.739, "G2 Pos16");  //"Pos 7"
    TText* tCROB2002_Feb_Connector8 = new TText(0.122, 0.739, "G2 Pos4");  //"Pos 8"

    Text_String = "CROB 2003 Connector Number(Run " + to_string(RunId) + ")";
    TText* tTitle_CROB2003_Connector_No = new TText(0.01, 1.06, Text_String);
    TText* tCROB2003_Connector0 = new TText(0.770, 0.384, "C 1"); //"Pos 0"
    TText* tCROB2003_Connector1 = new TText(0.477, 0.384, "C 2"); //"Pos 1"
    TText* tCROB2003_Connector2 = new TText(0.150, 0.384, "C 3"); //"Pos 2"
    TText* tCROB2003_Connector3 = new TText(0.770, 0.571, "C 4"); //"Pos 3"
    TText* tCROB2003_Connector4 = new TText(0.477, 0.571, "C 5"); //"Pos 4"
    TText* tCROB2003_Connector5 = new TText(0.150, 0.571, "C 6"); //"Pos 5"
    TText* tCROB2003_Connector6 = new TText(0.770, 0.739, "C 7"); //"Pos 6"
    TText* tCROB2003_Connector7 = new TText(0.477, 0.739, "C 8"); //"Pos 7"
    TText* tCROB2003_Connector8 = new TText(0.150, 0.739, "C 9"); //"Pos 8"

    Text_String = "CROB 2003 Elink Positions(Run " + to_string(RunId) + ")";
    TText* tTitle_CROB2003_ELink_No = new TText(0.01, 1.06, Text_String);
    TText* tCROB2003_ELink0 = new TText(0.736, 0.384, "33->37"); //"Pos 0"
    TText* tCROB2003_ELink1 = new TText(0.446, 0.384, "5->9");   //"Pos 1"
    TText* tCROB2003_ELink2 = new TText(0.150, 0.384, "0->4");   //"Pos 2"
    TText* tCROB2003_ELink3 = new TText(0.736, 0.571, "28->32"); //"Pos 3"
    TText* tCROB2003_ELink4 = new TText(0.446, 0.571, "14->18"); //"Pos 4"
    TText* tCROB2003_ELink5 = new TText(0.150, 0.571, "10->13"); //"Pos 5"
    TText* tCROB2003_ELink6 = new TText(0.736, 0.739, "38->41"); //"Pos 6"
    TText* tCROB2003_ELink7 = new TText(0.446, 0.739, "19->23"); //"Pos 7"
    TText* tCROB2003_ELink8 = new TText(0.150, 0.739, "24->27"); //"Pos 8"

    Text_String = "CROB 2003 Feb Connections(Run " + to_string(RunId) + ")";
    TText* tTitle_CROB2003_Feb_Connections_No = new TText(0.01, 1.06, Text_String);
    TText* tCROB2003_Feb_Connector0 = new TText(0.704, 0.384, "G2 Pos11"); //"Pos 0"
    TText* tCROB2003_Feb_Connector1 = new TText(0.43, 0.384, "G2 Pos13");  //"Pos 1"
    TText* tCROB2003_Feb_Connector2 = new TText(0.122, 0.384, "G2 Pos3");  //"Pos 2"
    TText* tCROB2003_Feb_Connector3 = new TText(0.704, 0.571, "XXXXX");    //"Pos 3"
    TText* tCROB2003_Feb_Connector4 = new TText(0.43, 0.571, "G2 Pos17");  //"Pos 4"
    TText* tCROB2003_Feb_Connector5 = new TText(0.122, 0.571, "G2 Pos12"); //"Pos 5"
    TText* tCROB2003_Feb_Connector6 = new TText(0.704, 0.739, "XXXXX");    //"Pos 6"
    TText* tCROB2003_Feb_Connector7 = new TText(0.43, 0.739, "G2 Pos10");  //"Pos 7"
    TText* tCROB2003_Feb_Connector8 = new TText(0.122, 0.739, "XXXXX");    //"Pos 8"


    tTitle_CROB1001_Connector_No->SetTextFont(43);
    tTitle_CROB1001_Connector_No->SetTextSize(20);
    tTitle_CROB1001_Connector_No->SetTextAngle(0);

    tTitle_CROB1001_ELink_No->SetTextFont(43);
    tTitle_CROB1001_ELink_No->SetTextSize(20);
    tTitle_CROB1001_ELink_No->SetTextAngle(0);

    tTitle_CROB1001_Feb_Connections_No->SetTextFont(43);
    tTitle_CROB1001_Feb_Connections_No->SetTextSize(20);
    tTitle_CROB1001_Feb_Connections_No->SetTextAngle(0);

    tTitle_CROB1002_Connector_No->SetTextFont(43);
    tTitle_CROB1002_Connector_No->SetTextSize(20);
    tTitle_CROB1002_Connector_No->SetTextAngle(0);

    tTitle_CROB1002_ELink_No->SetTextFont(43);
    tTitle_CROB1002_ELink_No->SetTextSize(20);
    tTitle_CROB1002_ELink_No->SetTextAngle(0);

    tTitle_CROB1002_Feb_Connections_No->SetTextFont(43);
    tTitle_CROB1002_Feb_Connections_No->SetTextSize(20);
    tTitle_CROB1002_Feb_Connections_No->SetTextAngle(0);

    tTitle_CROB1003_Connector_No->SetTextFont(43);
    tTitle_CROB1003_Connector_No->SetTextSize(20);
    tTitle_CROB1003_Connector_No->SetTextAngle(0);

    tTitle_CROB1003_ELink_No->SetTextFont(43);
    tTitle_CROB1003_ELink_No->SetTextSize(20);
    tTitle_CROB1003_ELink_No->SetTextAngle(0);

    tTitle_CROB1003_Feb_Connections_No->SetTextFont(43);
    tTitle_CROB1003_Feb_Connections_No->SetTextSize(20);
    tTitle_CROB1003_Feb_Connections_No->SetTextAngle(0);

    tTitle_CROB2001_Connector_No->SetTextFont(43);
    tTitle_CROB2001_Connector_No->SetTextSize(20);
    tTitle_CROB2001_Connector_No->SetTextAngle(0);

    tTitle_CROB2001_ELink_No->SetTextFont(43);
    tTitle_CROB2001_ELink_No->SetTextSize(20);
    tTitle_CROB2001_ELink_No->SetTextAngle(0);

    tTitle_CROB2001_Feb_Connections_No->SetTextFont(43);
    tTitle_CROB2001_Feb_Connections_No->SetTextSize(20);
    tTitle_CROB2001_Feb_Connections_No->SetTextAngle(0);

    tTitle_CROB2002_Connector_No->SetTextFont(43);
    tTitle_CROB2002_Connector_No->SetTextSize(20);
    tTitle_CROB2002_Connector_No->SetTextAngle(0);

    tTitle_CROB2002_ELink_No->SetTextFont(43);
    tTitle_CROB2002_ELink_No->SetTextSize(20);
    tTitle_CROB2002_ELink_No->SetTextAngle(0);

    tTitle_CROB2002_Feb_Connections_No->SetTextFont(43);
    tTitle_CROB2002_Feb_Connections_No->SetTextSize(20);
    tTitle_CROB2002_Feb_Connections_No->SetTextAngle(0);

    tTitle_CROB2003_Connector_No->SetTextFont(43);
    tTitle_CROB2003_Connector_No->SetTextSize(20);
    tTitle_CROB2003_Connector_No->SetTextAngle(0);

    tTitle_CROB2003_ELink_No->SetTextFont(43);
    tTitle_CROB2003_ELink_No->SetTextSize(20);
    tTitle_CROB2003_ELink_No->SetTextAngle(0);

    tTitle_CROB2003_Feb_Connections_No->SetTextFont(43);
    tTitle_CROB2003_Feb_Connections_No->SetTextSize(20);
    tTitle_CROB2003_Feb_Connections_No->SetTextAngle(0);

    tCROB1001_Connector0->SetTextColor(kWhite);
    tCROB1001_Connector0->SetTextFont(43);
    tCROB1001_Connector0->SetTextSize(15);

    tCROB1001_Connector1->SetTextColor(kWhite);
    tCROB1001_Connector1->SetTextFont(43);
    tCROB1001_Connector1->SetTextSize(15);

    tCROB1001_Connector2->SetTextColor(kWhite);
    tCROB1001_Connector2->SetTextFont(43);
    tCROB1001_Connector2->SetTextSize(15);

    tCROB1001_Connector3->SetTextColor(kWhite);
    tCROB1001_Connector3->SetTextFont(43);
    tCROB1001_Connector3->SetTextSize(15);

    tCROB1001_Connector4->SetTextColor(kWhite);
    tCROB1001_Connector4->SetTextFont(43);
    tCROB1001_Connector4->SetTextSize(15);

    tCROB1001_Connector5->SetTextColor(kWhite);
    tCROB1001_Connector5->SetTextFont(43);
    tCROB1001_Connector5->SetTextSize(15);

    tCROB1001_Connector6->SetTextColor(kWhite);
    tCROB1001_Connector6->SetTextFont(43);
    tCROB1001_Connector6->SetTextSize(15);

    tCROB1001_Connector7->SetTextColor(kWhite);
    tCROB1001_Connector7->SetTextFont(43);
    tCROB1001_Connector7->SetTextSize(15);

    tCROB1001_Connector8->SetTextColor(kWhite);
    tCROB1001_Connector8->SetTextFont(43);
    tCROB1001_Connector8->SetTextSize(15);

    tCROB1001_ELink0->SetTextColor(kWhite);
    tCROB1001_ELink0->SetTextFont(43);
    tCROB1001_ELink0->SetTextSize(15);

    tCROB1001_ELink1->SetTextColor(kWhite);
    tCROB1001_ELink1->SetTextFont(43);
    tCROB1001_ELink1->SetTextSize(15);

    tCROB1001_ELink2->SetTextColor(kWhite);
    tCROB1001_ELink2->SetTextFont(43);
    tCROB1001_ELink2->SetTextSize(15);

    tCROB1001_ELink3->SetTextColor(kWhite);
    tCROB1001_ELink3->SetTextFont(43);
    tCROB1001_ELink3->SetTextSize(15);

    tCROB1001_ELink4->SetTextColor(kWhite);
    tCROB1001_ELink4->SetTextFont(43);
    tCROB1001_ELink4->SetTextSize(15);

    tCROB1001_ELink5->SetTextColor(kWhite);
    tCROB1001_ELink5->SetTextFont(43);
    tCROB1001_ELink5->SetTextSize(15);

    tCROB1001_ELink6->SetTextColor(kWhite);
    tCROB1001_ELink6->SetTextFont(43);
    tCROB1001_ELink6->SetTextSize(15);

    tCROB1001_ELink7->SetTextColor(kWhite);
    tCROB1001_ELink7->SetTextFont(43);
    tCROB1001_ELink7->SetTextSize(15);

    tCROB1001_ELink8->SetTextColor(kWhite);
    tCROB1001_ELink8->SetTextFont(43);
    tCROB1001_ELink8->SetTextSize(15);

    tCROB1001_Feb_Connector0->SetTextColor(kWhite);
    tCROB1001_Feb_Connector0->SetTextFont(43);
    tCROB1001_Feb_Connector0->SetTextSize(15);

    tCROB1001_Feb_Connector1->SetTextColor(kWhite);
    tCROB1001_Feb_Connector1->SetTextFont(43);
    tCROB1001_Feb_Connector1->SetTextSize(15);

    tCROB1001_Feb_Connector2->SetTextColor(kWhite);
    tCROB1001_Feb_Connector2->SetTextFont(43);
    tCROB1001_Feb_Connector2->SetTextSize(15);

    tCROB1001_Feb_Connector3->SetTextColor(kWhite);
    tCROB1001_Feb_Connector3->SetTextFont(43);
    tCROB1001_Feb_Connector3->SetTextSize(15);

    tCROB1001_Feb_Connector4->SetTextColor(kWhite);
    tCROB1001_Feb_Connector4->SetTextFont(43);
    tCROB1001_Feb_Connector4->SetTextSize(15);

    tCROB1001_Feb_Connector5->SetTextColor(kWhite);
    tCROB1001_Feb_Connector5->SetTextFont(43);
    tCROB1001_Feb_Connector5->SetTextSize(15);

    tCROB1001_Feb_Connector6->SetTextColor(kWhite);
    tCROB1001_Feb_Connector6->SetTextFont(43);
    tCROB1001_Feb_Connector6->SetTextSize(15);

    tCROB1001_Feb_Connector7->SetTextColor(kWhite);
    tCROB1001_Feb_Connector7->SetTextFont(43);
    tCROB1001_Feb_Connector7->SetTextSize(15);

    tCROB1001_Feb_Connector8->SetTextColor(kWhite);
    tCROB1001_Feb_Connector8->SetTextFont(43);
    tCROB1001_Feb_Connector8->SetTextSize(15);


    tCROB1002_Connector0->SetTextColor(kWhite);
    tCROB1002_Connector0->SetTextFont(43);
    tCROB1002_Connector0->SetTextSize(15);

    tCROB1002_Connector1->SetTextColor(kWhite);
    tCROB1002_Connector1->SetTextFont(43);
    tCROB1002_Connector1->SetTextSize(15);

    tCROB1002_Connector2->SetTextColor(kWhite);
    tCROB1002_Connector2->SetTextFont(43);
    tCROB1002_Connector2->SetTextSize(15);

    tCROB1002_Connector3->SetTextColor(kWhite);
    tCROB1002_Connector3->SetTextFont(43);
    tCROB1002_Connector3->SetTextSize(15);

    tCROB1002_Connector4->SetTextColor(kWhite);
    tCROB1002_Connector4->SetTextFont(43);
    tCROB1002_Connector4->SetTextSize(15);

    tCROB1002_Connector5->SetTextColor(kWhite);
    tCROB1002_Connector5->SetTextFont(43);
    tCROB1002_Connector5->SetTextSize(15);

    tCROB1002_Connector6->SetTextColor(kWhite);
    tCROB1002_Connector6->SetTextFont(43);
    tCROB1002_Connector6->SetTextSize(15);

    tCROB1002_Connector7->SetTextColor(kWhite);
    tCROB1002_Connector7->SetTextFont(43);
    tCROB1002_Connector7->SetTextSize(15);

    tCROB1002_Connector8->SetTextColor(kWhite);
    tCROB1002_Connector8->SetTextFont(43);
    tCROB1002_Connector8->SetTextSize(15);

    tCROB1002_ELink0->SetTextColor(kWhite);
    tCROB1002_ELink0->SetTextFont(43);
    tCROB1002_ELink0->SetTextSize(15);

    tCROB1002_ELink1->SetTextColor(kWhite);
    tCROB1002_ELink1->SetTextFont(43);
    tCROB1002_ELink1->SetTextSize(15);

    tCROB1002_ELink2->SetTextColor(kWhite);
    tCROB1002_ELink2->SetTextFont(43);
    tCROB1002_ELink2->SetTextSize(15);

    tCROB1002_ELink3->SetTextColor(kWhite);
    tCROB1002_ELink3->SetTextFont(43);
    tCROB1002_ELink3->SetTextSize(15);

    tCROB1002_ELink4->SetTextColor(kWhite);
    tCROB1002_ELink4->SetTextFont(43);
    tCROB1002_ELink4->SetTextSize(15);

    tCROB1002_ELink5->SetTextColor(kWhite);
    tCROB1002_ELink5->SetTextFont(43);
    tCROB1002_ELink5->SetTextSize(15);

    tCROB1002_ELink6->SetTextColor(kWhite);
    tCROB1002_ELink6->SetTextFont(43);
    tCROB1002_ELink6->SetTextSize(15);

    tCROB1002_ELink7->SetTextColor(kWhite);
    tCROB1002_ELink7->SetTextFont(43);
    tCROB1002_ELink7->SetTextSize(15);

    tCROB1002_ELink8->SetTextColor(kWhite);
    tCROB1002_ELink8->SetTextFont(43);
    tCROB1002_ELink8->SetTextSize(15);

    tCROB1002_Feb_Connector0->SetTextColor(kWhite);
    tCROB1002_Feb_Connector0->SetTextFont(43);
    tCROB1002_Feb_Connector0->SetTextSize(15);

    tCROB1002_Feb_Connector1->SetTextColor(kWhite);
    tCROB1002_Feb_Connector1->SetTextFont(43);
    tCROB1002_Feb_Connector1->SetTextSize(15);

    tCROB1002_Feb_Connector2->SetTextColor(kWhite);
    tCROB1002_Feb_Connector2->SetTextFont(43);
    tCROB1002_Feb_Connector2->SetTextSize(15);

    tCROB1002_Feb_Connector3->SetTextColor(kWhite);
    tCROB1002_Feb_Connector3->SetTextFont(43);
    tCROB1002_Feb_Connector3->SetTextSize(15);

    tCROB1002_Feb_Connector4->SetTextColor(kWhite);
    tCROB1002_Feb_Connector4->SetTextFont(43);
    tCROB1002_Feb_Connector4->SetTextSize(15);

    tCROB1002_Feb_Connector5->SetTextColor(kWhite);
    tCROB1002_Feb_Connector5->SetTextFont(43);
    tCROB1002_Feb_Connector5->SetTextSize(15);

    tCROB1002_Feb_Connector6->SetTextColor(kWhite);
    tCROB1002_Feb_Connector6->SetTextFont(43);
    tCROB1002_Feb_Connector6->SetTextSize(15);

    tCROB1002_Feb_Connector7->SetTextColor(kWhite);
    tCROB1002_Feb_Connector7->SetTextFont(43);
    tCROB1002_Feb_Connector7->SetTextSize(15);

    tCROB1002_Feb_Connector8->SetTextColor(kWhite);
    tCROB1002_Feb_Connector8->SetTextFont(43);
    tCROB1002_Feb_Connector8->SetTextSize(15);


    tCROB1003_Connector0->SetTextColor(kWhite);
    tCROB1003_Connector0->SetTextFont(43);
    tCROB1003_Connector0->SetTextSize(15);

    tCROB1003_Connector1->SetTextColor(kWhite);
    tCROB1003_Connector1->SetTextFont(43);
    tCROB1003_Connector1->SetTextSize(15);

    tCROB1003_Connector2->SetTextColor(kWhite);
    tCROB1003_Connector2->SetTextFont(43);
    tCROB1003_Connector2->SetTextSize(15);

    tCROB1003_Connector3->SetTextColor(kWhite);
    tCROB1003_Connector3->SetTextFont(43);
    tCROB1003_Connector3->SetTextSize(15);

    tCROB1003_Connector4->SetTextColor(kWhite);
    tCROB1003_Connector4->SetTextFont(43);
    tCROB1003_Connector4->SetTextSize(15);

    tCROB1003_Connector5->SetTextColor(kWhite);
    tCROB1003_Connector5->SetTextFont(43);
    tCROB1003_Connector5->SetTextSize(15);

    tCROB1003_Connector6->SetTextColor(kWhite);
    tCROB1003_Connector6->SetTextFont(43);
    tCROB1003_Connector6->SetTextSize(15);

    tCROB1003_Connector7->SetTextColor(kWhite);
    tCROB1003_Connector7->SetTextFont(43);
    tCROB1003_Connector7->SetTextSize(15);

    tCROB1003_Connector8->SetTextColor(kWhite);
    tCROB1003_Connector8->SetTextFont(43);
    tCROB1003_Connector8->SetTextSize(15);

    tCROB1003_ELink0->SetTextColor(kWhite);
    tCROB1003_ELink0->SetTextFont(43);
    tCROB1003_ELink0->SetTextSize(15);

    tCROB1003_ELink1->SetTextColor(kWhite);
    tCROB1003_ELink1->SetTextFont(43);
    tCROB1003_ELink1->SetTextSize(15);

    tCROB1003_ELink2->SetTextColor(kWhite);
    tCROB1003_ELink2->SetTextFont(43);
    tCROB1003_ELink2->SetTextSize(15);

    tCROB1003_ELink3->SetTextColor(kWhite);
    tCROB1003_ELink3->SetTextFont(43);
    tCROB1003_ELink3->SetTextSize(15);

    tCROB1003_ELink4->SetTextColor(kWhite);
    tCROB1003_ELink4->SetTextFont(43);
    tCROB1003_ELink4->SetTextSize(15);

    tCROB1003_ELink5->SetTextColor(kWhite);
    tCROB1003_ELink5->SetTextFont(43);
    tCROB1003_ELink5->SetTextSize(15);

    tCROB1003_ELink6->SetTextColor(kWhite);
    tCROB1003_ELink6->SetTextFont(43);
    tCROB1003_ELink6->SetTextSize(15);

    tCROB1003_ELink7->SetTextColor(kWhite);
    tCROB1003_ELink7->SetTextFont(43);
    tCROB1003_ELink7->SetTextSize(15);

    tCROB1003_ELink8->SetTextColor(kWhite);
    tCROB1003_ELink8->SetTextFont(43);
    tCROB1003_ELink8->SetTextSize(15);

    tCROB1003_Feb_Connector0->SetTextColor(kWhite);
    tCROB1003_Feb_Connector0->SetTextFont(43);
    tCROB1003_Feb_Connector0->SetTextSize(15);

    tCROB1003_Feb_Connector1->SetTextColor(kWhite);
    tCROB1003_Feb_Connector1->SetTextFont(43);
    tCROB1003_Feb_Connector1->SetTextSize(15);

    tCROB1003_Feb_Connector2->SetTextColor(kWhite);
    tCROB1003_Feb_Connector2->SetTextFont(43);
    tCROB1003_Feb_Connector2->SetTextSize(15);

    tCROB1003_Feb_Connector3->SetTextColor(kWhite);
    tCROB1003_Feb_Connector3->SetTextFont(43);
    tCROB1003_Feb_Connector3->SetTextSize(15);

    tCROB1003_Feb_Connector4->SetTextColor(kWhite);
    tCROB1003_Feb_Connector4->SetTextFont(43);
    tCROB1003_Feb_Connector4->SetTextSize(15);

    tCROB1003_Feb_Connector5->SetTextColor(kWhite);
    tCROB1003_Feb_Connector5->SetTextFont(43);
    tCROB1003_Feb_Connector5->SetTextSize(15);

    tCROB1003_Feb_Connector6->SetTextColor(kWhite);
    tCROB1003_Feb_Connector6->SetTextFont(43);
    tCROB1003_Feb_Connector6->SetTextSize(15);

    tCROB1003_Feb_Connector7->SetTextColor(kWhite);
    tCROB1003_Feb_Connector7->SetTextFont(43);
    tCROB1003_Feb_Connector7->SetTextSize(15);

    tCROB1003_Feb_Connector8->SetTextColor(kWhite);
    tCROB1003_Feb_Connector8->SetTextFont(43);
    tCROB1003_Feb_Connector8->SetTextSize(15);

    tCROB2001_Connector0->SetTextColor(kWhite);
    tCROB2001_Connector0->SetTextFont(43);
    tCROB2001_Connector0->SetTextSize(15);

    tCROB2001_Connector1->SetTextColor(kWhite);
    tCROB2001_Connector1->SetTextFont(43);
    tCROB2001_Connector1->SetTextSize(15);

    tCROB2001_Connector2->SetTextColor(kWhite);
    tCROB2001_Connector2->SetTextFont(43);
    tCROB2001_Connector2->SetTextSize(15);

    tCROB2001_Connector3->SetTextColor(kWhite);
    tCROB2001_Connector3->SetTextFont(43);
    tCROB2001_Connector3->SetTextSize(15);

    tCROB2001_Connector4->SetTextColor(kWhite);
    tCROB2001_Connector4->SetTextFont(43);
    tCROB2001_Connector4->SetTextSize(15);

    tCROB2001_Connector5->SetTextColor(kWhite);
    tCROB2001_Connector5->SetTextFont(43);
    tCROB2001_Connector5->SetTextSize(15);

    tCROB2001_Connector6->SetTextColor(kWhite);
    tCROB2001_Connector6->SetTextFont(43);
    tCROB2001_Connector6->SetTextSize(15);

    tCROB2001_Connector7->SetTextColor(kWhite);
    tCROB2001_Connector7->SetTextFont(43);
    tCROB2001_Connector7->SetTextSize(15);

    tCROB2001_Connector8->SetTextColor(kWhite);
    tCROB2001_Connector8->SetTextFont(43);
    tCROB2001_Connector8->SetTextSize(15);

    tCROB2001_ELink0->SetTextColor(kWhite);
    tCROB2001_ELink0->SetTextFont(43);
    tCROB2001_ELink0->SetTextSize(15);

    tCROB2001_ELink1->SetTextColor(kWhite);
    tCROB2001_ELink1->SetTextFont(43);
    tCROB2001_ELink1->SetTextSize(15);

    tCROB2001_ELink2->SetTextColor(kWhite);
    tCROB2001_ELink2->SetTextFont(43);
    tCROB2001_ELink2->SetTextSize(15);

    tCROB2001_ELink3->SetTextColor(kWhite);
    tCROB2001_ELink3->SetTextFont(43);
    tCROB2001_ELink3->SetTextSize(15);

    tCROB2001_ELink4->SetTextColor(kWhite);
    tCROB2001_ELink4->SetTextFont(43);
    tCROB2001_ELink4->SetTextSize(15);

    tCROB2001_ELink5->SetTextColor(kWhite);
    tCROB2001_ELink5->SetTextFont(43);
    tCROB2001_ELink5->SetTextSize(15);

    tCROB2001_ELink6->SetTextColor(kWhite);
    tCROB2001_ELink6->SetTextFont(43);
    tCROB2001_ELink6->SetTextSize(15);

    tCROB2001_ELink7->SetTextColor(kWhite);
    tCROB2001_ELink7->SetTextFont(43);
    tCROB2001_ELink7->SetTextSize(15);

    tCROB2001_ELink8->SetTextColor(kWhite);
    tCROB2001_ELink8->SetTextFont(43);
    tCROB2001_ELink8->SetTextSize(15);

    tCROB2001_Feb_Connector0->SetTextColor(kWhite);
    tCROB2001_Feb_Connector0->SetTextFont(43);
    tCROB2001_Feb_Connector0->SetTextSize(15);

    tCROB2001_Feb_Connector1->SetTextColor(kWhite);
    tCROB2001_Feb_Connector1->SetTextFont(43);
    tCROB2001_Feb_Connector1->SetTextSize(15);

    tCROB2001_Feb_Connector2->SetTextColor(kWhite);
    tCROB2001_Feb_Connector2->SetTextFont(43);
    tCROB2001_Feb_Connector2->SetTextSize(15);

    tCROB2001_Feb_Connector3->SetTextColor(kWhite);
    tCROB2001_Feb_Connector3->SetTextFont(43);
    tCROB2001_Feb_Connector3->SetTextSize(15);

    tCROB2001_Feb_Connector4->SetTextColor(kWhite);
    tCROB2001_Feb_Connector4->SetTextFont(43);
    tCROB2001_Feb_Connector4->SetTextSize(15);

    tCROB2001_Feb_Connector5->SetTextColor(kWhite);
    tCROB2001_Feb_Connector5->SetTextFont(43);
    tCROB2001_Feb_Connector5->SetTextSize(15);

    tCROB2001_Feb_Connector6->SetTextColor(kWhite);
    tCROB2001_Feb_Connector6->SetTextFont(43);
    tCROB2001_Feb_Connector6->SetTextSize(15);

    tCROB2001_Feb_Connector7->SetTextColor(kWhite);
    tCROB2001_Feb_Connector7->SetTextFont(43);
    tCROB2001_Feb_Connector7->SetTextSize(15);

    tCROB2001_Feb_Connector8->SetTextColor(kWhite);
    tCROB2001_Feb_Connector8->SetTextFont(43);
    tCROB2001_Feb_Connector8->SetTextSize(15);

    tCROB2002_Connector0->SetTextColor(kWhite);
    tCROB2002_Connector0->SetTextFont(43);
    tCROB2002_Connector0->SetTextSize(15);

    tCROB2002_Connector1->SetTextColor(kWhite);
    tCROB2002_Connector1->SetTextFont(43);
    tCROB2002_Connector1->SetTextSize(15);

    tCROB2002_Connector2->SetTextColor(kWhite);
    tCROB2002_Connector2->SetTextFont(43);
    tCROB2002_Connector2->SetTextSize(15);

    tCROB2002_Connector3->SetTextColor(kWhite);
    tCROB2002_Connector3->SetTextFont(43);
    tCROB2002_Connector3->SetTextSize(15);

    tCROB2002_Connector4->SetTextColor(kWhite);
    tCROB2002_Connector4->SetTextFont(43);
    tCROB2002_Connector4->SetTextSize(15);

    tCROB2002_Connector5->SetTextColor(kWhite);
    tCROB2002_Connector5->SetTextFont(43);
    tCROB2002_Connector5->SetTextSize(15);

    tCROB2002_Connector6->SetTextColor(kWhite);
    tCROB2002_Connector6->SetTextFont(43);
    tCROB2002_Connector6->SetTextSize(15);

    tCROB2002_Connector7->SetTextColor(kWhite);
    tCROB2002_Connector7->SetTextFont(43);
    tCROB2002_Connector7->SetTextSize(15);

    tCROB2002_Connector8->SetTextColor(kWhite);
    tCROB2002_Connector8->SetTextFont(43);
    tCROB2002_Connector8->SetTextSize(15);

    tCROB2002_ELink0->SetTextColor(kWhite);
    tCROB2002_ELink0->SetTextFont(43);
    tCROB2002_ELink0->SetTextSize(15);

    tCROB2002_ELink1->SetTextColor(kWhite);
    tCROB2002_ELink1->SetTextFont(43);
    tCROB2002_ELink1->SetTextSize(15);

    tCROB2002_ELink2->SetTextColor(kWhite);
    tCROB2002_ELink2->SetTextFont(43);
    tCROB2002_ELink2->SetTextSize(15);

    tCROB2002_ELink3->SetTextColor(kWhite);
    tCROB2002_ELink3->SetTextFont(43);
    tCROB2002_ELink3->SetTextSize(15);

    tCROB2002_ELink4->SetTextColor(kWhite);
    tCROB2002_ELink4->SetTextFont(43);
    tCROB2002_ELink4->SetTextSize(15);

    tCROB2002_ELink5->SetTextColor(kWhite);
    tCROB2002_ELink5->SetTextFont(43);
    tCROB2002_ELink5->SetTextSize(15);

    tCROB2002_ELink6->SetTextColor(kWhite);
    tCROB2002_ELink6->SetTextFont(43);
    tCROB2002_ELink6->SetTextSize(15);

    tCROB2002_ELink7->SetTextColor(kWhite);
    tCROB2002_ELink7->SetTextFont(43);
    tCROB2002_ELink7->SetTextSize(15);

    tCROB2002_ELink8->SetTextColor(kWhite);
    tCROB2002_ELink8->SetTextFont(43);
    tCROB2002_ELink8->SetTextSize(15);

    tCROB2002_Feb_Connector0->SetTextColor(kWhite);
    tCROB2002_Feb_Connector0->SetTextFont(43);
    tCROB2002_Feb_Connector0->SetTextSize(15);

    tCROB2002_Feb_Connector1->SetTextColor(kWhite);
    tCROB2002_Feb_Connector1->SetTextFont(43);
    tCROB2002_Feb_Connector1->SetTextSize(15);

    tCROB2002_Feb_Connector2->SetTextColor(kWhite);
    tCROB2002_Feb_Connector2->SetTextFont(43);
    tCROB2002_Feb_Connector2->SetTextSize(15);

    tCROB2002_Feb_Connector3->SetTextColor(kWhite);
    tCROB2002_Feb_Connector3->SetTextFont(43);
    tCROB2002_Feb_Connector3->SetTextSize(15);

    tCROB2002_Feb_Connector4->SetTextColor(kWhite);
    tCROB2002_Feb_Connector4->SetTextFont(43);
    tCROB2002_Feb_Connector4->SetTextSize(15);

    tCROB2002_Feb_Connector5->SetTextColor(kWhite);
    tCROB2002_Feb_Connector5->SetTextFont(43);
    tCROB2002_Feb_Connector5->SetTextSize(15);

    tCROB2002_Feb_Connector6->SetTextColor(kWhite);
    tCROB2002_Feb_Connector6->SetTextFont(43);
    tCROB2002_Feb_Connector6->SetTextSize(15);

    tCROB2002_Feb_Connector7->SetTextColor(kWhite);
    tCROB2002_Feb_Connector7->SetTextFont(43);
    tCROB2002_Feb_Connector7->SetTextSize(15);

    tCROB2002_Feb_Connector8->SetTextColor(kWhite);
    tCROB2002_Feb_Connector8->SetTextFont(43);
    tCROB2002_Feb_Connector8->SetTextSize(15);


    tCROB2003_Connector0->SetTextColor(kWhite);
    tCROB2003_Connector0->SetTextFont(43);
    tCROB2003_Connector0->SetTextSize(15);

    tCROB2003_Connector1->SetTextColor(kWhite);
    tCROB2003_Connector1->SetTextFont(43);
    tCROB2003_Connector1->SetTextSize(15);

    tCROB2003_Connector2->SetTextColor(kWhite);
    tCROB2003_Connector2->SetTextFont(43);
    tCROB2003_Connector2->SetTextSize(15);

    tCROB2003_Connector3->SetTextColor(kWhite);
    tCROB2003_Connector3->SetTextFont(43);
    tCROB2003_Connector3->SetTextSize(15);

    tCROB2003_Connector4->SetTextColor(kWhite);
    tCROB2003_Connector4->SetTextFont(43);
    tCROB2003_Connector4->SetTextSize(15);

    tCROB2003_Connector5->SetTextColor(kWhite);
    tCROB2003_Connector5->SetTextFont(43);
    tCROB2003_Connector5->SetTextSize(15);

    tCROB2003_Connector6->SetTextColor(kWhite);
    tCROB2003_Connector6->SetTextFont(43);
    tCROB2003_Connector6->SetTextSize(15);

    tCROB2003_Connector7->SetTextColor(kWhite);
    tCROB2003_Connector7->SetTextFont(43);
    tCROB2003_Connector7->SetTextSize(15);

    tCROB2003_Connector8->SetTextColor(kWhite);
    tCROB2003_Connector8->SetTextFont(43);
    tCROB2003_Connector8->SetTextSize(15);

    tCROB2003_ELink0->SetTextColor(kWhite);
    tCROB2003_ELink0->SetTextFont(43);
    tCROB2003_ELink0->SetTextSize(15);

    tCROB2003_ELink1->SetTextColor(kWhite);
    tCROB2003_ELink1->SetTextFont(43);
    tCROB2003_ELink1->SetTextSize(15);

    tCROB2003_ELink2->SetTextColor(kWhite);
    tCROB2003_ELink2->SetTextFont(43);
    tCROB2003_ELink2->SetTextSize(15);

    tCROB2003_ELink3->SetTextColor(kWhite);
    tCROB2003_ELink3->SetTextFont(43);
    tCROB2003_ELink3->SetTextSize(15);

    tCROB2003_ELink4->SetTextColor(kWhite);
    tCROB2003_ELink4->SetTextFont(43);
    tCROB2003_ELink4->SetTextSize(15);

    tCROB2003_ELink5->SetTextColor(kWhite);
    tCROB2003_ELink5->SetTextFont(43);
    tCROB2003_ELink5->SetTextSize(15);

    tCROB2003_ELink6->SetTextColor(kWhite);
    tCROB2003_ELink6->SetTextFont(43);
    tCROB2003_ELink6->SetTextSize(15);

    tCROB2003_ELink7->SetTextColor(kWhite);
    tCROB2003_ELink7->SetTextFont(43);
    tCROB2003_ELink7->SetTextSize(15);

    tCROB2003_ELink8->SetTextColor(kWhite);
    tCROB2003_ELink8->SetTextFont(43);
    tCROB2003_ELink8->SetTextSize(15);

    tCROB2003_Feb_Connector0->SetTextColor(kWhite);
    tCROB2003_Feb_Connector0->SetTextFont(43);
    tCROB2003_Feb_Connector0->SetTextSize(15);

    tCROB2003_Feb_Connector1->SetTextColor(kWhite);
    tCROB2003_Feb_Connector1->SetTextFont(43);
    tCROB2003_Feb_Connector1->SetTextSize(15);

    tCROB2003_Feb_Connector2->SetTextColor(kWhite);
    tCROB2003_Feb_Connector2->SetTextFont(43);
    tCROB2003_Feb_Connector2->SetTextSize(15);

    tCROB2003_Feb_Connector3->SetTextColor(kWhite);
    tCROB2003_Feb_Connector3->SetTextFont(43);
    tCROB2003_Feb_Connector3->SetTextSize(15);

    tCROB2003_Feb_Connector4->SetTextColor(kWhite);
    tCROB2003_Feb_Connector4->SetTextFont(43);
    tCROB2003_Feb_Connector4->SetTextSize(15);

    tCROB2003_Feb_Connector5->SetTextColor(kWhite);
    tCROB2003_Feb_Connector5->SetTextFont(43);
    tCROB2003_Feb_Connector5->SetTextSize(15);

    tCROB2003_Feb_Connector6->SetTextColor(kWhite);
    tCROB2003_Feb_Connector6->SetTextFont(43);
    tCROB2003_Feb_Connector6->SetTextSize(15);

    tCROB2003_Feb_Connector7->SetTextColor(kWhite);
    tCROB2003_Feb_Connector7->SetTextFont(43);
    tCROB2003_Feb_Connector7->SetTextSize(15);

    tCROB2003_Feb_Connector8->SetTextColor(kWhite);
    tCROB2003_Feb_Connector8->SetTextFont(43);
    tCROB2003_Feb_Connector8->SetTextSize(15);

    cCROB_1001->cd(1);
    tTitle_CROB1001_Connector_No->Draw();
    tCROB1001_Connector0->Draw();
    tCROB1001_Connector1->Draw();
    tCROB1001_Connector2->Draw();
    tCROB1001_Connector3->Draw();
    tCROB1001_Connector4->Draw();
    tCROB1001_Connector5->Draw();
    tCROB1001_Connector6->Draw();
    tCROB1001_Connector7->Draw();
    tCROB1001_Connector8->Draw();
    cCROB_1001->cd();

    cCROB_1001->cd(2);
    tTitle_CROB1001_ELink_No->Draw();
    tCROB1001_ELink0->Draw();
    tCROB1001_ELink1->Draw();
    tCROB1001_ELink2->Draw();
    tCROB1001_ELink3->Draw();
    tCROB1001_ELink4->Draw();
    tCROB1001_ELink5->Draw();
    tCROB1001_ELink6->Draw();
    tCROB1001_ELink7->Draw();
    tCROB1001_ELink8->Draw();
    cCROB_1001->cd();

    cCROB_1001->cd(3);
    tTitle_CROB1001_Feb_Connections_No->Draw();
    tCROB1001_Feb_Connector0->Draw();
    tCROB1001_Feb_Connector1->Draw();
    tCROB1001_Feb_Connector2->Draw();
    tCROB1001_Feb_Connector3->Draw();
    tCROB1001_Feb_Connector4->Draw();
    tCROB1001_Feb_Connector5->Draw();
    tCROB1001_Feb_Connector6->Draw();
    tCROB1001_Feb_Connector7->Draw();
    tCROB1001_Feb_Connector8->Draw();
    cCROB_1001->cd();


    cCROB_1002->cd(1);
    tTitle_CROB1002_Connector_No->Draw();
    tCROB1002_Connector0->Draw();
    tCROB1002_Connector1->Draw();
    tCROB1002_Connector2->Draw();
    tCROB1002_Connector3->Draw();
    tCROB1002_Connector4->Draw();
    tCROB1002_Connector5->Draw();
    tCROB1002_Connector6->Draw();
    tCROB1002_Connector7->Draw();
    tCROB1002_Connector8->Draw();
    cCROB_1002->cd();

    cCROB_1002->cd(2);
    tTitle_CROB1002_ELink_No->Draw();
    tCROB1002_ELink0->Draw();
    tCROB1002_ELink1->Draw();
    tCROB1002_ELink2->Draw();
    tCROB1002_ELink3->Draw();
    tCROB1002_ELink4->Draw();
    tCROB1002_ELink5->Draw();
    tCROB1002_ELink6->Draw();
    tCROB1002_ELink7->Draw();
    tCROB1002_ELink8->Draw();
    cCROB_1002->cd();

    cCROB_1002->cd(3);
    tTitle_CROB1002_Feb_Connections_No->Draw();
    tCROB1002_Feb_Connector0->Draw();
    tCROB1002_Feb_Connector1->Draw();
    tCROB1002_Feb_Connector2->Draw();
    tCROB1002_Feb_Connector3->Draw();
    tCROB1002_Feb_Connector4->Draw();
    tCROB1002_Feb_Connector5->Draw();
    tCROB1002_Feb_Connector6->Draw();
    tCROB1002_Feb_Connector7->Draw();
    tCROB1002_Feb_Connector8->Draw();
    cCROB_1002->cd();


    cCROB_1003->cd(1);
    tTitle_CROB1003_Connector_No->Draw();
    tCROB1003_Connector0->Draw();
    tCROB1003_Connector1->Draw();
    tCROB1003_Connector2->Draw();
    tCROB1003_Connector3->Draw();
    tCROB1003_Connector4->Draw();
    tCROB1003_Connector5->Draw();
    tCROB1003_Connector6->Draw();
    tCROB1003_Connector7->Draw();
    tCROB1003_Connector8->Draw();
    cCROB_1003->cd();

    cCROB_1003->cd(2);
    tTitle_CROB1003_ELink_No->Draw();
    tCROB1003_ELink0->Draw();
    tCROB1003_ELink1->Draw();
    tCROB1003_ELink2->Draw();
    tCROB1003_ELink3->Draw();
    tCROB1003_ELink4->Draw();
    tCROB1003_ELink5->Draw();
    tCROB1003_ELink6->Draw();
    tCROB1003_ELink7->Draw();
    tCROB1003_ELink8->Draw();
    cCROB_1003->cd();

    cCROB_1003->cd(3);
    tTitle_CROB1003_Feb_Connections_No->Draw();
    tCROB1003_Feb_Connector0->Draw();
    tCROB1003_Feb_Connector1->Draw();
    tCROB1003_Feb_Connector2->Draw();
    tCROB1003_Feb_Connector3->Draw();
    tCROB1003_Feb_Connector4->Draw();
    tCROB1003_Feb_Connector5->Draw();
    tCROB1003_Feb_Connector6->Draw();
    tCROB1003_Feb_Connector7->Draw();
    tCROB1003_Feb_Connector8->Draw();
    cCROB_1003->cd();

    cCROB_2001->cd(1);
    tTitle_CROB2001_Connector_No->Draw();
    tCROB2001_Connector0->Draw();
    tCROB2001_Connector1->Draw();
    tCROB2001_Connector2->Draw();
    tCROB2001_Connector3->Draw();
    tCROB2001_Connector4->Draw();
    tCROB2001_Connector5->Draw();
    tCROB2001_Connector6->Draw();
    tCROB2001_Connector7->Draw();
    tCROB2001_Connector8->Draw();
    cCROB_2001->cd();

    cCROB_2001->cd(2);
    tTitle_CROB2001_ELink_No->Draw();
    tCROB2001_ELink0->Draw();
    tCROB2001_ELink1->Draw();
    tCROB2001_ELink2->Draw();
    tCROB2001_ELink3->Draw();
    tCROB2001_ELink4->Draw();
    tCROB2001_ELink5->Draw();
    tCROB2001_ELink6->Draw();
    tCROB2001_ELink7->Draw();
    tCROB2001_ELink8->Draw();
    cCROB_2001->cd();

    cCROB_2001->cd(3);
    tTitle_CROB2001_Feb_Connections_No->Draw();
    tCROB2001_Feb_Connector0->Draw();
    tCROB2001_Feb_Connector1->Draw();
    tCROB2001_Feb_Connector2->Draw();
    tCROB2001_Feb_Connector3->Draw();
    tCROB2001_Feb_Connector4->Draw();
    tCROB2001_Feb_Connector5->Draw();
    tCROB2001_Feb_Connector6->Draw();
    tCROB2001_Feb_Connector7->Draw();
    tCROB2001_Feb_Connector8->Draw();
    cCROB_2001->cd();

    cCROB_2002->cd(1);
    tTitle_CROB2002_Connector_No->Draw();
    tCROB2002_Connector0->Draw();
    tCROB2002_Connector1->Draw();
    tCROB2002_Connector2->Draw();
    tCROB2002_Connector3->Draw();
    tCROB2002_Connector4->Draw();
    tCROB2002_Connector5->Draw();
    tCROB2002_Connector6->Draw();
    tCROB2002_Connector7->Draw();
    tCROB2002_Connector8->Draw();
    cCROB_2002->cd();

    cCROB_2002->cd(2);
    tTitle_CROB2002_ELink_No->Draw();
    tCROB2002_ELink0->Draw();
    tCROB2002_ELink1->Draw();
    tCROB2002_ELink2->Draw();
    tCROB2002_ELink3->Draw();
    tCROB2002_ELink4->Draw();
    tCROB2002_ELink5->Draw();
    tCROB2002_ELink6->Draw();
    tCROB2002_ELink7->Draw();
    tCROB2002_ELink8->Draw();
    cCROB_2002->cd();

    cCROB_2002->cd(3);
    tTitle_CROB2002_Feb_Connections_No->Draw();
    tCROB2002_Feb_Connector0->Draw();
    tCROB2002_Feb_Connector1->Draw();
    tCROB2002_Feb_Connector2->Draw();
    tCROB2002_Feb_Connector3->Draw();
    tCROB2002_Feb_Connector4->Draw();
    tCROB2002_Feb_Connector5->Draw();
    tCROB2002_Feb_Connector6->Draw();
    tCROB2002_Feb_Connector7->Draw();
    tCROB2002_Feb_Connector8->Draw();
    cCROB_2002->cd();


    cCROB_2003->cd(1);
    tTitle_CROB2003_Connector_No->Draw();
    tCROB2003_Connector0->Draw();
    tCROB2003_Connector1->Draw();
    tCROB2003_Connector2->Draw();
    tCROB2003_Connector3->Draw();
    tCROB2003_Connector4->Draw();
    tCROB2003_Connector5->Draw();
    tCROB2003_Connector6->Draw();
    tCROB2003_Connector7->Draw();
    tCROB2003_Connector8->Draw();
    cCROB_2003->cd();

    cCROB_2003->cd(2);
    tTitle_CROB2003_ELink_No->Draw();
    tCROB2003_ELink0->Draw();
    tCROB2003_ELink1->Draw();
    tCROB2003_ELink2->Draw();
    tCROB2003_ELink3->Draw();
    tCROB2003_ELink4->Draw();
    tCROB2003_ELink5->Draw();
    tCROB2003_ELink6->Draw();
    tCROB2003_ELink7->Draw();
    tCROB2003_ELink8->Draw();
    cCROB_2003->cd();

    cCROB_2003->cd(3);
    tTitle_CROB2003_Feb_Connections_No->Draw();
    tCROB2003_Feb_Connector0->Draw();
    tCROB2003_Feb_Connector1->Draw();
    tCROB2003_Feb_Connector2->Draw();
    tCROB2003_Feb_Connector3->Draw();
    tCROB2003_Feb_Connector4->Draw();
    tCROB2003_Feb_Connector5->Draw();
    tCROB2003_Feb_Connector6->Draw();
    tCROB2003_Feb_Connector7->Draw();
    tCROB2003_Feb_Connector8->Draw();
    cCROB_2003->cd();
}
