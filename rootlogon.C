// -----------------------------------------------------------------------------
// Copyright (C) Experimental High Energy Physics and Applications Group, VECC, Kolkata
// rootlogon.C
// Project : GEM detector test beam at CERN GIF++ (2023)
// Author  : Apar Agarwal
// Usage   : Loaded automatically by ROOT when started from this directory (sets plotting style).
// -----------------------------------------------------------------------------

{
    TStyle* AparStyle = new TStyle("AparStyle", "Apar's Root Styles");
    AparStyle->SetPalette(1, 0); // avoid horrible default color scheme
    AparStyle->SetOptStat(0);
    AparStyle->SetOptFit(0011);
    //AparStyle->SetOptTitle(0);
    AparStyle->SetDrawOption("HIST");
    //AparStyle->SetOptDate(0);
    AparStyle->SetLabelSize(0.03); // size of axis value font
    //AparStyle->SetTitleSize(0.035); // size of axis title font
    //AparStyle->SetTitleFont(22); // font option
    //AparStyle->SetLabelFont(22,"xyz");
    AparStyle->SetTitleX(0.5);
    AparStyle->SetTitleAlign(23);
    AparStyle->SetTitleOffset(1.2, "y"); // default canvas options
    //AparStyle->SetCanvasDefW(500);
    //AparStyle->SetCanvasDefH(500);
    AparStyle->SetCanvasColor(0); // canvas...
    //AparStyle->SetCanvasBorderMode(0);
    //AparStyle->SetCanvasBorderSize(0);
    //AparStyle->SetPadBottomMargin(0.1); //margins...
    //AparStyle->SetPadTopMargin(0.1);
    //AparStyle->SetPadLeftMargin(0.1);
    //AparStyle->SetPadRightMargin(0.1);
    //AparStyle->SetPadGridX(0); // grids, tickmarks
    //AparStyle->SetPadGridY(0);
    //AparStyle->SetPadTickX(1);
    //AparStyle->SetPadTickY(1);
    // AparStyle->SetMarkerStyle(20);
    // AparStyle->SetDrawOption("E2");
    gStyle->SetErrorX(0.0001);
    AparStyle->SetFrameBorderMode(0);
    AparStyle->SetPaperSize(20, 24); // US letter size
    gROOT->SetStyle("AparStyle");
    cout << "My Style is Set!" << endl;
    return;
}
