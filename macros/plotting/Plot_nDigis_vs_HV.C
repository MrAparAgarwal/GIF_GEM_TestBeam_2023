// -----------------------------------------------------------------------------
// Copyright (C) Experimental High Energy Physics and Applications Group, VECC, Kolkata
// Plot_nDigis_vs_HV.C
// Project : GEM detector test beam at CERN GIF++ (2023)
// Author  : Apar Agarwal
// Usage   : root -l -b -q 'Plot_nDigis_vs_HV.C'  (see README for input file layout)
// -----------------------------------------------------------------------------

void Plot_nDigis_vs_HV()
{
    Double_t Runs_nDigis_Per_TS_With_Voltages[] = {979, 990, 977, 982, 971, 974, 966, 976, 967};
    Double_t HV_nDigis_Per_TS_With_Voltages[] = {3937, 3965, 3992, 4019, 4047, 4075, 4101, 4129, 4157};
    Double_t HV_nDigis_Per_TS_With_Voltages_Errors[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    Double_t nDigis_Per_TS_With_Voltages[] = {917.9 - 100, 985.3 - 100, 1029 - 100, 1100 - 200, 1127 - 175, 1112 - 175, 1276 - 175, 1172 - 200, 1361 - 200};
    Double_t nDigis_Per_TS_With_Voltages_Errors[] = {52.85, 58.31, 61.98, 61.76, 60.71, 63.66, 62.66, 64.79, 79.81};
    TGraphErrors* gnDigis_Per_TS_With_Voltages = new TGraphErrors(9, HV_nDigis_Per_TS_With_Voltages, nDigis_Per_TS_With_Voltages, HV_nDigis_Per_TS_With_Voltages_Errors, nDigis_Per_TS_With_Voltages_Errors);
    gnDigis_Per_TS_With_Voltages->SetTitle("nDigis Per TS vs. Bias Voltage");
    gnDigis_Per_TS_With_Voltages->GetXaxis()->SetTitle("Bias Voltages");
    gnDigis_Per_TS_With_Voltages->GetYaxis()->SetTitle("Mean nDigis Per TS");
    // gnDigis_Per_TS_With_Voltages->GetXaxis()->SetLimits(-0.02,0.25);
    gnDigis_Per_TS_With_Voltages->SetFillStyle(1000);
    gnDigis_Per_TS_With_Voltages->SetMarkerColor(3);
    gnDigis_Per_TS_With_Voltages->SetMarkerStyle(20);
    gnDigis_Per_TS_With_Voltages->SetMarkerSize(3.0);
    // gnDigis_Per_TS_With_Voltages->Draw("ap");
}
