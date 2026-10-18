// -----------------------------------------------------------------------------
// Copyright (C) Experimental High Energy Physics and Applications Group, VECC, Kolkata
// Plot_Gain_TCorr_Sigma.C
// Project : GEM detector test beam at CERN GIF++ (2023)
// Author  : Apar Agarwal
// Usage   : root -l -b -q 'Plot_Gain_TCorr_Sigma.C'  (see README for input file layout)
// -----------------------------------------------------------------------------

void Plot_Gain_TCorr_Sigma()
{
    // ------------------------------Muon Only Analysis---------------------

    Double_t Run_No_Muon_Only[] = {971, 974, 966, 976, 967};
    // Double_t HV_Muon_Only[]         = {4047,4075,4101,4129,4157};
    Double_t HV_Muon_Only[] = {3937, 3965, 3992, 4019, 4047, 4075, 4101, 4129, 4157};
    Double_t HV_Err_Muon_Only[] = {0., 0., 0., 0., 0., 0.0, 0.0, 0.0, 0.0};
    // Double_t Mean_Muon_Only[]       = {373.3,376.0,377.3,380.5,381.7};
    Double_t Mean_Muon_Only[] = {368.5, 369.1, 369.9, 0.0, 371.9, 375.0, 377.7, 380.8, 382.6};
    // Double_t Sigma_Muon_Only[]      = {19.87,19.12,18.36,17.8,17.45};
    Double_t Sigma_Muon_Only[] = {20.93, 20.88, 20.98, 0.0, 17.14, 16.05, 15.42, 14.42, 13.86};
    // Double_t MPV_Muon_Only[]        = {34.69,40.22,43.27,53.82,56.37};
    Double_t MPV_Muon_Only[] = {19.07, 22.08, 24.38, 0., 31.33, 36.2, 38.98, 48.52, 52.47};
    // Double_t MPV_Muon_Only[]        = {31.33,36.2,38.98,48.52,52.47};
    Double_t MPV_Err_Muon_Only[] = {0.25, 0.25, 0.25, 0.25, 0.57, 0.74, 0.78, 1.40, 1.55};
    // Double_t Eff_Muon_Only_3Sigma[] = {96.16,97.29,97.63,97.75,97.79};
    // Double_t Eff_Muon_Only_3Sigma[] = {45.88,47.18,48.53,50.83,50.41};
    // Double_t Eff_Muon_Only_3Sigma[] = {34.6,38.1,40.69,45.53,47.37,48.19,50.42,49.90};
    Double_t Eff_Muon_Only_3Sigma[] = {34.6 / 50.42, 38.1 / 50.42, 40.69 / 50.42, 0.0, 45.53 / 50.42, 47.37 / 50.42, 48.19 / 50.42, 50.42 / 50.42, 49.90 / 50.42};
    Double_t Eff_Muon_Only_3Sigma_Corrected[] = {1.87 * 45.53, 1.87 * 47.37, 1.87 * 48.19, 1.87 * 50.42, 1.87 * 49.90};
    Double_t Eff_Muon_Only_4Fold[] = {81.00, 86.99, 86.29, 93.10, 88.92};

    TGraph* Sigma_vs_HV_Muon_Only = new TGraph(9, HV_Muon_Only, Sigma_Muon_Only);
    Sigma_vs_HV_Muon_Only->SetTitle("Time Correlation Sigma vs. Applied Bias Voltage [Muon Only Runs]");
    Sigma_vs_HV_Muon_Only->GetXaxis()->SetTitle("HV (Volts)");
    Sigma_vs_HV_Muon_Only->GetYaxis()->SetTitle("Time Correlation Sigma (ns)");
    Sigma_vs_HV_Muon_Only->SetFillStyle(1000);
    Sigma_vs_HV_Muon_Only->SetMarkerColor(4);
    Sigma_vs_HV_Muon_Only->SetMarkerStyle(20);
    Sigma_vs_HV_Muon_Only->SetMarkerSize(3);
    // Sigma_vs_HV_Muon_Only->Draw("ap");

    TGraph* Mean_vs_HV_Muon_Only = new TGraph(9, HV_Muon_Only, Mean_Muon_Only);
    Mean_vs_HV_Muon_Only->SetTitle("Time Correlation Mean vs. Applied Bias Voltage [Muon Only Runs]");
    Mean_vs_HV_Muon_Only->GetXaxis()->SetTitle("HV (Volts)");
    Mean_vs_HV_Muon_Only->GetYaxis()->SetTitle("Time Correlation Mean (ns)");
    Mean_vs_HV_Muon_Only->SetFillStyle(1000);
    Mean_vs_HV_Muon_Only->SetMarkerColor(4);
    Mean_vs_HV_Muon_Only->SetMarkerStyle(20);
    Mean_vs_HV_Muon_Only->SetMarkerSize(2);
    // Mean_vs_HV_Muon_Only->Draw("ap");

    TGraphErrors* Clu_ADC_Muon_Only = new TGraphErrors(9, HV_Muon_Only, MPV_Muon_Only, HV_Err_Muon_Only, MPV_Err_Muon_Only);
    Clu_ADC_Muon_Only->SetTitle("Cluster Charge MPV vs. Applied Bias Voltage [Muon Only Runs]");
    Clu_ADC_Muon_Only->GetXaxis()->SetTitle("HV (Volts)");
    Clu_ADC_Muon_Only->GetYaxis()->SetTitle("Cluster Charge MPV (fC)");
    Clu_ADC_Muon_Only->SetFillStyle(1000);
    Clu_ADC_Muon_Only->SetMarkerColor(4);
    Clu_ADC_Muon_Only->SetMarkerStyle(20);
    Clu_ADC_Muon_Only->SetMarkerSize(2.5);
    // Clu_ADC_Muon_Only->Draw("ap");

    TGraph* gEff_Muon_Only = new TGraph(9, HV_Muon_Only, Eff_Muon_Only_3Sigma);
    gEff_Muon_Only->SetTitle("3 Fold Efficiency (Raw) vs. Applied Bias Voltage [Muon Only Runs]");
    gEff_Muon_Only->GetXaxis()->SetTitle("HV (Volts)");
    gEff_Muon_Only->GetYaxis()->SetTitle("Efficiency (%)");
    gEff_Muon_Only->SetFillStyle(1000);
    gEff_Muon_Only->SetMarkerColor(4);
    gEff_Muon_Only->SetMarkerStyle(20);
    gEff_Muon_Only->SetMarkerSize(3);
    // gEff_Muon_Only->Draw("ap");

    TGraph* gEff_Muon_Only_Acceptance_Corrected = new TGraph(5, HV_Muon_Only, Eff_Muon_Only_3Sigma_Corrected);
    gEff_Muon_Only_Acceptance_Corrected->SetTitle("3 Fold Efficiency (Acceptance Corrected) vs. Applied Bias Voltage [Muon Only Runs]");
    gEff_Muon_Only_Acceptance_Corrected->GetXaxis()->SetTitle("HV (Volts)");
    gEff_Muon_Only_Acceptance_Corrected->GetYaxis()->SetTitle("Efficiency (%)");
    gEff_Muon_Only_Acceptance_Corrected->SetFillStyle(1000);
    gEff_Muon_Only_Acceptance_Corrected->SetMarkerColor(4);
    gEff_Muon_Only_Acceptance_Corrected->SetMarkerStyle(20);
    gEff_Muon_Only_Acceptance_Corrected->SetMarkerSize(2.5);
    // gEff_Muon_Only_Acceptance_Corrected->Draw("ap");

    // Double_t Run_No_Muon_Only[]     = {979,990,977,982,971,974,966,976,967};

    // Double_t HV_Muon_Only[]         = {3937,3965,3992,4019,4047,4075,4101,4129,4157};
    Double_t Cluster_Size_Muon_Only_3Sigma[] = {1.076, 1.073, 1.076, 0., 1.097, 1.091, 1.102, 1.110, 1.133};

    TGraph* gCluster_Size_Muon_Only = new TGraph(9, HV_Muon_Only, Cluster_Size_Muon_Only_3Sigma);
    gCluster_Size_Muon_Only->SetTitle("Mean Cluster Size vs. Applied Bias Voltage [Muon Only Runs]");
    gCluster_Size_Muon_Only->GetXaxis()->SetTitle("HV (Volts)");
    gCluster_Size_Muon_Only->GetYaxis()->SetTitle("Mean Cluster Size");
    gCluster_Size_Muon_Only->SetFillStyle(1000);
    gCluster_Size_Muon_Only->SetMarkerColor(1);
    gCluster_Size_Muon_Only->SetMarkerStyle(20);
    gCluster_Size_Muon_Only->SetMarkerSize(3);
    // gCluster_Size_Muon_Only->Draw("ap");

    TGraph* gEff_4Fold_Muon_Only = new TGraph(5, HV_Muon_Only, Eff_Muon_Only_4Fold);
    gEff_4Fold_Muon_Only->SetTitle("4 Fold Efficiency (3 Fold+RPC) (Raw) vs. Applied Bias Voltage [Muon Only Runs]");
    gEff_4Fold_Muon_Only->GetXaxis()->SetTitle("HV (Volts)");
    gEff_4Fold_Muon_Only->GetYaxis()->SetTitle("4 Fold Efficiency (%)");
    gEff_4Fold_Muon_Only->SetFillStyle(1000);
    gEff_4Fold_Muon_Only->SetMarkerColor(4);
    gEff_4Fold_Muon_Only->SetMarkerStyle(20);
    gEff_4Fold_Muon_Only->SetMarkerSize(2.5);
    // gEff_4Fold_Muon_Only->Draw("ap");

    TGraph* gGain_vs_Eff_Muon_Only = new TGraph(9, MPV_Muon_Only, Eff_Muon_Only_3Sigma);
    gGain_vs_Eff_Muon_Only->SetTitle("3 Fold Efficiency (Raw) vs. Cluster Charge MPV [Muon Only Runs]");
    gGain_vs_Eff_Muon_Only->GetXaxis()->SetTitle("Cluster Charge MPV (fC)");
    gGain_vs_Eff_Muon_Only->GetYaxis()->SetTitle("Relative Efficiency");
    gGain_vs_Eff_Muon_Only->SetFillStyle(1000);
    gGain_vs_Eff_Muon_Only->SetMarkerColor(4);
    gGain_vs_Eff_Muon_Only->SetMarkerStyle(20);
    gGain_vs_Eff_Muon_Only->SetMarkerSize(3);
    // gGain_vs_Eff_Muon_Only->Draw("ap");

    Double_t Eff_Muon_Only_2Sigma[] = {97.06, 97.60, 97.95, 97.96, 97.98};
    Double_t Eff_Muon_Only_6Sigma[] = {95.56, 96.18, 96.66, 96.73, 96.96};
    Double_t Eff_Muon_Only_10Sigma[] = {94.44, 95.09, 95.44, 96.00, 95.94};

    TGraph* gEff_Mu_2Sigma = new TGraph(5, HV_Muon_Only, Eff_Muon_Only_2Sigma);
    TGraph* gEff_Mu_3Sigma = new TGraph(5, HV_Muon_Only, Eff_Muon_Only_3Sigma);
    TGraph* gEff_Mu_6Sigma = new TGraph(5, HV_Muon_Only, Eff_Muon_Only_6Sigma);
    TGraph* gEff_Mu_10Sigma = new TGraph(5, HV_Muon_Only, Eff_Muon_Only_10Sigma);
    // gEff_Mu_2Sigma->Draw();
    // gEff_Mu_3Sigma->Draw("same");
    // gEff_Mu_6Sigma->Draw("same");
    // gEff_Mu_10Sigma->Draw("same");

    // ------------------------------Muon + Source Analysis---------------------

    // Double_t ATTF_Inv_1[] = {0,0.010,0.014,0.022,0.030,0.045,0.067,0.1,0.145,0.217,0.303,0.455};
    Double_t ATTF_Inv_1[] = {0.455, 0.303, 0.217, 0.145, 0.1, 0.067, 0.045, 0.030, 0.022, 0.014, 0.010, 0.0};
    Double_t Br_Current_3885V[] = {1420.6, 1419.4, 1418.1, 1417.5, 1416.6, 1416.8, 1416.4, 1416.2, 1416.1, 1416.1, 1415.9, 1415.8};
    Double_t Br_Current_3912V[] = {1430.9, 1429.7, 1428.3, 1427.6, 1427, 1426.8, 1426.3, 1426.1, 1426, 1426, 1425.8, 0.0};
    Double_t Br_Current_3937V[] = {0.0, 0.0, 1437.9, 1437.4, 1436.3, 1436, 1435.6, 0.0, 1435.1, 1435, 1434.9, 0.0};
    Double_t Br_Current_3965V[] = {0.0, 0.0, 1448.6, 1447.7, 1446.7, 1446.4, 1445.9, 1445.7, 1445.4, 1445.3, 1445.1, 0.0};
    Double_t Br_Current_3992V[] = {0.0, 0.0, 1459, 1458, 1456.9, 1456.6, 1455.8, 1455.7, 1455.4, 1455.2, 1455, 1454.6};
    Double_t Br_Current_4019V[] = {0.0, 1472.1, 1469.5, 1468.4, 1467.2, 1466.9, 1466, 1465.6, 1465.3, 1465.2, 1465, 1464.7};
    Double_t Br_Current_4047V[] = {1486.8, 1483.6, 1480.6, 1479.2, 1477.8, 1477.4, 1476.3, 1476, 1475.6, 1475.5, 1475.3, 0.0};
    Double_t Br_Current_4075V[] = {1498.8, 1495.3, 1491.8, 1490.2, 1488.4, 1488, 1486.8, 1486.4, 1485.9, 1485.8, 1485.4, 0.0};
    Double_t Br_Current_4101V[] = {0.0, 1507.2, 1502.2, 1500.4, 1498.5, 1498, 1496.6, 1496.2, 1495.7, 1495.4, 1495.1, 1494.6};
    Double_t Br_Current_4129V[] = {1523, 1519.4, 1513.7, 1511.7, 1509.5, 1508.8, 1507.2, 1506.6, 1506, 1505.8, 1505.4, 1504.7};
    Double_t Br_Current_4157V[] = {1535.9, 1531.9, 1525.6, 1523, 1520.4, 1519.7, 1517.8, 1517.2, 1516.5, 1516.2, 1515.8, 1515};

    TGraph* Br_Current_vs_ATTF_3885V = new TGraph(12, ATTF_Inv_1, Br_Current_3885V);
    TGraph* Br_Current_vs_ATTF_3912V = new TGraph(12, ATTF_Inv_1, Br_Current_3912V);
    TGraph* Br_Current_vs_ATTF_3937V = new TGraph(12, ATTF_Inv_1, Br_Current_3937V);
    TGraph* Br_Current_vs_ATTF_3965V = new TGraph(12, ATTF_Inv_1, Br_Current_3965V);
    TGraph* Br_Current_vs_ATTF_3992V = new TGraph(12, ATTF_Inv_1, Br_Current_3992V);
    TGraph* Br_Current_vs_ATTF_4019V = new TGraph(12, ATTF_Inv_1, Br_Current_4019V);
    TGraph* Br_Current_vs_ATTF_4047V = new TGraph(12, ATTF_Inv_1, Br_Current_4047V);
    TGraph* Br_Current_vs_ATTF_4075V = new TGraph(12, ATTF_Inv_1, Br_Current_4075V);
    TGraph* Br_Current_vs_ATTF_4101V = new TGraph(12, ATTF_Inv_1, Br_Current_4101V);
    TGraph* Br_Current_vs_ATTF_4129V = new TGraph(12, ATTF_Inv_1, Br_Current_4129V);
    TGraph* Br_Current_vs_ATTF_4157V = new TGraph(12, ATTF_Inv_1, Br_Current_4157V);
    // Br_Current_vs_ATTF_3885V->SetMarkerStyle(20);
    // Br_Current_vs_ATTF_3885V->GetYaxis()->SetLimits(0.0,1550.0);
    // Br_Current_vs_ATTF_3885V->Draw("ap");
    // Br_Current_vs_ATTF_3912V->SetMarkerStyle(21);
    // Br_Current_vs_ATTF_3912V->Draw("same");
    // Br_Current_vs_ATTF_3937V->SetMarkerStyle(22);
    // Br_Current_vs_ATTF_3937V->Draw("same");
    // Br_Current_vs_ATTF_3965V->SetMarkerStyle(23);
    // Br_Current_vs_ATTF_3965V->Draw("same");
    // Br_Current_vs_ATTF_3992V->SetMarkerStyle(24);
    // Br_Current_vs_ATTF_3992V->Draw("same");
    // Br_Current_vs_ATTF_4019V->SetMarkerStyle(25);
    // Br_Current_vs_ATTF_4019V->Draw("same");
    // Br_Current_vs_ATTF_4047V->SetMarkerStyle(26);
    // Br_Current_vs_ATTF_4047V->Draw("same");
    // Br_Current_vs_ATTF_4075V->SetMarkerStyle(27);
    // Br_Current_vs_ATTF_4075V->Draw("same");
    // Br_Current_vs_ATTF_4101V->SetMarkerStyle(28);
    // Br_Current_vs_ATTF_4101V->Draw("same");
    // Br_Current_vs_ATTF_4129V->SetMarkerStyle(29);
    // Br_Current_vs_ATTF_4129V->Draw("same");
    // Br_Current_vs_ATTF_4157V->SetMarkerStyle(30);
    // Br_Current_vs_ATTF_4157V->Draw("same");


    Double_t Run_No_4101[] = {966, 893, 999, 996, 995, 992};
    Double_t Run_No_4157[] = {967, 892, 998, 997, 994, 993};
    Double_t ATTF_Inv[] = {0.0, 1 / 22.0, 1 / 15.0, 1 / 10.0, 1 / 6.9, 1 / 4.6};
    // Double_t MPV_4101[]    = {43.27,44.4,45.38,43.43,44.2,44.33};
    Double_t MPV_4101[] = {38.98, 40.46, 41.57, 40.52, 39.45, 39.45};
    // Double_t MPV_4157[]    = {56.37,59.2,57.64,57.12,56.52,48.66};
    Double_t MPV_4157[] = {53.99, 50.02, 55.91, 53.45, 54.08, 52.27};
    Double_t TCorr_Mean_With_Source_4101[] = {377.7, 376.4, 377.4, 377.1, 376.8, 377.4};
    Double_t TCorr_Sigma_With_Source_4101[] = {15.42, 15.22, 15.42, 15.57, 15.25, 15.26};
    Double_t TCorr_Mean_With_Source_4157[] = {380.8, 381.0, 381.5, 381.4, 381.2, 383.4};
    Double_t TCorr_Sigma_With_Source_4157[] = {14.39, 13.55, 13.82, 14.26, 14.07, 25.68};

    TGraph* Sigma_vs_ATTF_Inv_4157V = new TGraph(6, ATTF_Inv, TCorr_Sigma_With_Source_4157);
    Sigma_vs_ATTF_Inv_4157V->SetTitle("Time Correlation Sigma vs. (Attenuation Factor)^{-1}");
    Sigma_vs_ATTF_Inv_4157V->GetXaxis()->SetTitle("(Attenuation Factor)^{-1}");
    Sigma_vs_ATTF_Inv_4157V->GetYaxis()->SetTitle("Time Correlation Sigma (ns)");
    Sigma_vs_ATTF_Inv_4157V->SetFillStyle(1000);
    Sigma_vs_ATTF_Inv_4157V->SetMarkerColor(4);
    Sigma_vs_ATTF_Inv_4157V->SetMarkerStyle(20);
    Sigma_vs_ATTF_Inv_4157V->SetMarkerSize(2);
    // Sigma_vs_ATTF_Inv_4157V->Draw("ap");

    TGraph* Sigma_vs_ATTF_Inv_4101V = new TGraph(6, ATTF_Inv, TCorr_Sigma_With_Source_4101);
    Sigma_vs_ATTF_Inv_4101V->SetTitle("Time Correlation Sigma vs. (Attenuation Factor)^{-1}");
    Sigma_vs_ATTF_Inv_4101V->GetXaxis()->SetTitle("(Attenuation Factor)^{-1}");
    Sigma_vs_ATTF_Inv_4101V->GetYaxis()->SetTitle("Time Correlation Sigma (ns)");
    Sigma_vs_ATTF_Inv_4101V->SetFillStyle(1000);
    Sigma_vs_ATTF_Inv_4101V->SetMarkerColor(4);
    Sigma_vs_ATTF_Inv_4101V->SetMarkerStyle(20);
    Sigma_vs_ATTF_Inv_4101V->SetMarkerSize(2);
    // Sigma_vs_ATTF_Inv_4101V->Draw("same");

    TGraph* Mean_vs_ATTF_Inv_4101V = new TGraph(6, ATTF_Inv, TCorr_Mean_With_Source_4101);
    Mean_vs_ATTF_Inv_4101V->SetTitle("Time Correlation Mean vs. (Attenuation Factor)^{-1}");
    Mean_vs_ATTF_Inv_4101V->GetXaxis()->SetTitle("(Attenuation Factor)^{-1}");
    Mean_vs_ATTF_Inv_4101V->GetYaxis()->SetTitle("Time Correlation Mean (ns)");
    Mean_vs_ATTF_Inv_4101V->SetFillStyle(1000);
    Mean_vs_ATTF_Inv_4101V->SetMarkerColor(4);
    Mean_vs_ATTF_Inv_4101V->SetMarkerStyle(20);
    Mean_vs_ATTF_Inv_4101V->SetMarkerSize(2);
    // Mean_vs_ATTF_Inv_4101V->Draw("ap");

    TGraph* Mean_vs_ATTF_Inv_4157V = new TGraph(6, ATTF_Inv, TCorr_Mean_With_Source_4157);
    Mean_vs_ATTF_Inv_4157V->SetTitle("Time Correlation Mean vs. (Attenuation Factor)^{-1}");
    Mean_vs_ATTF_Inv_4157V->GetXaxis()->SetTitle("(Attenuation Factor)^{-1}");
    Mean_vs_ATTF_Inv_4157V->GetYaxis()->SetTitle("Time Correlation Mean (ns)");
    Mean_vs_ATTF_Inv_4157V->SetFillStyle(1000);
    Mean_vs_ATTF_Inv_4157V->SetMarkerColor(4);
    Mean_vs_ATTF_Inv_4157V->SetMarkerStyle(20);
    Mean_vs_ATTF_Inv_4157V->SetMarkerSize(2);
    // Mean_vs_ATTF_Inv_4157V->Draw("same");

    TGraph* Clu_ADC_4101V = new TGraph(6, ATTF_Inv, MPV_4101);
    TGraph* Clu_ADC_4157V = new TGraph(6, ATTF_Inv, MPV_4157);
    // Clu_ADC_4101V->Draw();
    // Clu_ADC_4157V->Draw("same");


    // Double_t Eff_4101_3Sigma[] = {48.53,51.22,50.42,51.49,52.72,54.33};
    // Double_t Eff_4101_3Sigma[] = {48.19,50.77,49.70,50.77,51.94,53.64};
    Double_t Eff_4101_3Sigma[] = {48.19 / 48.19, 50.77 / 48.19, 49.70 / 48.19, 50.77 / 48.19, 51.94 / 48.19, 53.64 / 48.19};
    // Double_t Eff_4157_3Sigma[] = {50.41,52.88,52.14,53.75,55.06,63.88};
    Double_t Eff_4157_3Sigma[] = {49.90 / 49.90, 51.97 / 49.90, 51.52 / 49.90, 52.74 / 49.90, 53.87 / 49.90, 62.89 / 49.90};
    TGraph* gEff_4101_3Sigma = new TGraph(6, ATTF_Inv, Eff_4101_3Sigma);
    TGraph* gEff_4157_3Sigma = new TGraph(6, ATTF_Inv, Eff_4157_3Sigma);
    gEff_4101_3Sigma->SetTitle("Raw Efficiency vs. Intensity");
    gEff_4101_3Sigma->GetXaxis()->SetTitle("(Attenuation Factor)^{-1}");
    gEff_4101_3Sigma->GetYaxis()->SetTitle("Efficiency (%)");
    // gEff_4101_3Sigma->Draw();
    // gEff_4157_3Sigma->Draw("same");

    TGraph* Gain_vs_Efficiency_4101V = new TGraph(6, MPV_4101, Eff_4101_3Sigma);
    TGraph* Gain_vs_Efficiency_4157V = new TGraph(6, MPV_4157, Eff_4157_3Sigma);
    Gain_vs_Efficiency_4101V->SetTitle("Efficiency vs. Cluster Charge MPV");
    Gain_vs_Efficiency_4101V->GetXaxis()->SetTitle("Cluster Charge MPV (fC)");
    Gain_vs_Efficiency_4101V->GetYaxis()->SetTitle("Efficiency (%)");
    Gain_vs_Efficiency_4101V->GetXaxis()->SetLimits(35.0, 60.0);
    // Gain_vs_Efficiency_4101V->Draw();
    // Gain_vs_Efficiency_4157V->Draw("same");


    Double_t Eff_4101_2Sigma[] = {97.95, 95.89, 94.58, 93.60, 92.20, 91.83};
    Double_t Eff_4157_2Sigma[] = {97.98, 96.19, 95.50, 94.45, 93.91, 95.34};
    Double_t Eff_4101_6Sigma[] = {96.66, 95.47, 94.88, 94.49, 94.65, 95.16};
    Double_t Eff_4157_6Sigma[] = {96.97, 96.03, 95.43, 95.42, 95.45, 95.16};
    Double_t Eff_4101_10Sigma[] = {95.44, 95.33, 95.29, 95.36, 96.21, 96.98};
    Double_t Eff_4157_10Sigma[] = {95.94, 95.64, 95.58, 95.36, 96.52, 99.91};

    TGraph* gEff_4101_2Sigma = new TGraph(6, ATTF_Inv, Eff_4101_2Sigma);
    // TGraph* gEff_4101_3Sigma    = new TGraph(6,ATTF_Inv,Eff_4101_3Sigma);
    TGraph* gEff_4101_6Sigma = new TGraph(6, ATTF_Inv, Eff_4101_6Sigma);
    TGraph* gEff_4101_10Sigma = new TGraph(6, ATTF_Inv, Eff_4101_10Sigma);

    // gEff_4101_2Sigma->Draw();
    // gEff_4101_3Sigma->Draw("same");
    // gEff_4101_6Sigma->Draw("same");
    // gEff_4101_10Sigma->Draw("same");

    TGraph* gEff_4157_2Sigma = new TGraph(6, ATTF_Inv, Eff_4157_2Sigma);
    // TGraph* gEff_4157_3Sigma    = new TGraph(6,ATTF_Inv,Eff_4157_3Sigma);
    TGraph* gEff_4157_6Sigma = new TGraph(6, ATTF_Inv, Eff_4157_6Sigma);
    TGraph* gEff_4157_10Sigma = new TGraph(6, ATTF_Inv, Eff_4157_10Sigma);

    // gEff_4157_2Sigma->Draw();
    // gEff_4157_3Sigma->Draw("same");
    // gEff_4157_6Sigma->Draw("same");
    // gEff_4157_10Sigma->Draw("same");

    Double_t nDigis_Per_TS_Mean_4101V[] = {1272, 638600, 1011000, 1252000, 1861000, 2457000};
    TGraph* gnDigis_Per_TS_4101V = new TGraph(6, ATTF_Inv, nDigis_Per_TS_Mean_4101V);
    gnDigis_Per_TS_4101V->SetTitle("nDigis Per TS vs. Intensity");
    gnDigis_Per_TS_4101V->GetXaxis()->SetTitle("(Attenuation Factor)^{-1}");
    gnDigis_Per_TS_4101V->GetYaxis()->SetTitle("Mean nDigis Per TS");
    gnDigis_Per_TS_4101V->GetXaxis()->SetLimits(-0.02, 0.25);
    gnDigis_Per_TS_4101V->SetFillStyle(1000);
    gnDigis_Per_TS_4101V->SetMarkerColor(3);
    gnDigis_Per_TS_4101V->SetMarkerStyle(20);
    gnDigis_Per_TS_4101V->SetMarkerSize(3.0);
    // gnDigis_Per_TS_4101V->Draw("ap");

    TGraph* gnDigis_Br_Current_Per_TS_4101V = new TGraph(6, ATTF_Inv, nDigis_Per_TS_Mean_4101V);
    gnDigis_Br_Current_Per_TS_4101V->SetTitle("nDigis Per TS vs. Intensity");
    gnDigis_Br_Current_Per_TS_4101V->GetXaxis()->SetTitle("(Attenuation Factor)^{-1}");
    gnDigis_Br_Current_Per_TS_4101V->GetYaxis()->SetTitle("Mean nDigis Per TS");
    gnDigis_Br_Current_Per_TS_4101V->GetXaxis()->SetLimits(-0.02, 0.25);
    gnDigis_Br_Current_Per_TS_4101V->SetFillStyle(1000);
    gnDigis_Br_Current_Per_TS_4101V->SetMarkerColor(3);
    gnDigis_Br_Current_Per_TS_4101V->SetMarkerStyle(20);
    gnDigis_Br_Current_Per_TS_4101V->SetMarkerSize(3.0);


    Double_t nDigis_Per_TS_Mean_4157V[] = {1362, 783300, 1248000, 1532000, 2258000, 5722000};
    TGraph* gnDigis_Per_TS_4157V = new TGraph(6, ATTF_Inv, nDigis_Per_TS_Mean_4157V);
    gnDigis_Per_TS_4157V->SetTitle("nDigis Per TS vs. Intensity");
    gnDigis_Per_TS_4157V->GetXaxis()->SetTitle("(Attenuation Factor)^{-1}");
    gnDigis_Per_TS_4157V->GetYaxis()->SetTitle("Mean nDigis Per TS");
    gnDigis_Per_TS_4157V->GetXaxis()->SetLimits(-0.02, 0.25);
    gnDigis_Per_TS_4157V->SetFillStyle(1000);
    gnDigis_Per_TS_4157V->SetMarkerColor(3);
    gnDigis_Per_TS_4157V->SetMarkerStyle(20);
    gnDigis_Per_TS_4157V->SetMarkerSize(3.0);
    // gnDigis_Per_TS_4157V->Draw("ap");


    // Double_t Run_No_4101[] = {966,893,999,996,995,992};
    Double_t Cluster_Size_4101V[] = {1.102, 1.121, 1.132, 1.157, 1.174, 1.186};
    // Double_t Run_No_4157[] = {967,892,998,997,994,993};
    Double_t Cluster_Size_4157V[] = {1.133, 1.164, 1.172, 1.185, 1.188, 1.392};

    TGraph* gCluster_Size_With_Intensity_4101V = new TGraph(6, ATTF_Inv, Cluster_Size_4101V);
    gCluster_Size_With_Intensity_4101V->SetTitle("Mean Cluster Size vs. Intensity");
    gCluster_Size_With_Intensity_4101V->GetXaxis()->SetTitle("(Attenuation Factor)^{-1}");
    gCluster_Size_With_Intensity_4101V->GetYaxis()->SetTitle("Mean Cluster Size");
    gCluster_Size_With_Intensity_4101V->GetXaxis()->SetLimits(-0.02, 0.25);
    gCluster_Size_With_Intensity_4101V->SetFillStyle(1000);
    gCluster_Size_With_Intensity_4101V->SetMarkerColor(3);
    gCluster_Size_With_Intensity_4101V->SetMarkerStyle(20);
    gCluster_Size_With_Intensity_4101V->SetMarkerSize(3.0);
    // gCluster_Size_With_Intensity_4101V->Draw("ap");

    TGraph* gCluster_Size_With_Intensity_4157V = new TGraph(6, ATTF_Inv, Cluster_Size_4157V);
    gCluster_Size_With_Intensity_4157V->SetTitle("Mean Cluster Size vs. Intensity");
    gCluster_Size_With_Intensity_4157V->GetXaxis()->SetTitle("(Attenuation Factor)^{-1}");
    gCluster_Size_With_Intensity_4157V->GetYaxis()->SetTitle("Mean Cluster Size");
    gCluster_Size_With_Intensity_4157V->GetXaxis()->SetLimits(-0.02, 0.25);
    gCluster_Size_With_Intensity_4157V->SetFillStyle(1000);
    gCluster_Size_With_Intensity_4157V->SetMarkerColor(3);
    gCluster_Size_With_Intensity_4157V->SetMarkerStyle(20);
    gCluster_Size_With_Intensity_4157V->SetMarkerSize(3.0);
    // gCluster_Size_With_Intensity_4157V->Draw("same");


    Double_t Runs_nDigis_Per_TS_With_Voltages[] = {979, 990, 977, 982, 971, 974, 966, 976, 967};
    // Double_t HV_nDigis_Per_TS_With_Voltages[] = {3937,3965,3992,4019,4047,4075,4101,4129,4157};
    Double_t HV_nDigis_Per_TS_With_Voltages[] = {3937, 3965, 3992, 0, 4047, 4075, 4101, 4129, 4157};
    Double_t HV_nDigis_Per_TS_With_Voltages_Errors[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    // Double_t nDigis_Per_TS_With_Voltages[]    = {917.9-100,985.3-100,1029-100,1100-200,1127-175,1112-175,1276-175,1172-200,1361-200};
    Double_t nDigis_Per_TS_With_Voltages[] = {917.9 - 100, 985.3 - 100, 1029 - 100, 0, 1127 - 175, 1112 - 175, 1276 - 175, 1172 - 200, 1361 - 200};
    // Double_t nDigis_Per_TS_With_Voltages_Errors[] = {52.85,58.31,61.98,61.76,60.71,63.66,62.66,64.79,79.81};
    Double_t nDigis_Per_TS_With_Voltages_Errors[] = {52.85, 58.31, 61.98, 0.0, 60.71, 63.66, 62.66, 64.79, 79.81};
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

    Double_t Voltage_for_Response_Plot[] = {
        100.01, 200.01, 500.03,
        999.9, 1499.9, 2000.03,
        2500.03, 2999.95, 3500.03,
        3600.0, 3700.01, 3799.96,
        3900.07, 3999.96, 4100.04,
        4120.01, 4129.98, 4150.0,
        4157.00, 4175.0, 4200.0,
        4225.0};

    Double_t Current_for_Reponse_Plot[] = {
        36.2, 72.6, 181.7, 363.7,
        545.8, 728.2, 910.6,
        1093.1, 1275.6, 1312.0,
        1348.4, 1384.8, 1421.4,
        1457.7, 1494.2, 1501.4,
        1505.1, 1512.4, 1514.8,
        1521.5, 1530.6, 1539.8};

    // TGraph* gVoltage_vs_Current_Response_Plot = new TGraph(22,Current_for_Reponse_Plot,Voltage_for_Response_Plot);
    TGraph* gVoltage_vs_Current_Response_Plot = new TGraph(22, Voltage_for_Response_Plot, Current_for_Reponse_Plot);
    gVoltage_vs_Current_Response_Plot->SetTitle("GEM Response with Voltage");
    // gVoltage_vs_Current_Response_Plot->GetXaxis()->SetTitle("Measured Current (#muA)");
    gVoltage_vs_Current_Response_Plot->GetYaxis()->SetTitle("Measured Current (#muA)");
    // gVoltage_vs_Current_Response_Plot->GetYaxis()->SetTitle("Bias Voltages (V)");
    gVoltage_vs_Current_Response_Plot->GetXaxis()->SetTitle("Bias Voltages (V)");
    gVoltage_vs_Current_Response_Plot->SetFillStyle(1000);
    gVoltage_vs_Current_Response_Plot->SetMarkerColor(1);
    gVoltage_vs_Current_Response_Plot->SetMarkerStyle(20);
    gVoltage_vs_Current_Response_Plot->SetMarkerSize(3.0);
    gVoltage_vs_Current_Response_Plot->Draw("ap");

    Double_t Distance_in_Z_cm[] = {-1, 0.00191, 1.1, 2.9, 4.9};
    Double_t Source_Intenisty_Present_Day[] = {0.5, 5.67 * 1000000000000, 17.18 * 1000000, 2.39 * 1000000, 0.81 * 1000000};
    TGraph* Source_Intenisty_Present_Day_With_Z = new TGraph(5, Distance_in_Z_cm, Source_Intenisty_Present_Day);
    // Source_Intenisty_Present_Day_With_Z->Draw("ap");

    Double_t Run_No_4075_Setup1[] = {-1, 531, 542, 553, 564, 578, 589, 600, 620, 630, 661, 682};
    Double_t Run_No_4101_Setup1[] = {518, 527, 538, 549, 567, 574, 585, 596, -1, 636, 662, -1};
    Double_t Run_No_4129_Setup1[] = {519, 532, 543, 554, 565, 579, 590, 601, 619, 629, 667, 685};
    Double_t ATTF_Inv_Setup1[] = {0.0, 1 / 100.0, 1 / 68.1, 1 / 46.4, 1 / 31.6, 1 / 21.5, 1 / 14.7, 1 / 10.0, 1 / 6.81, 1 / 4.64, 1 / 3.16, 1 / 2.2};
    // Double_t ATTF_Inv_Setup1[] = {0.0,1/73.0,1/54.0,1/39.0,1/28.0,1/20.0,1/14.2,1/7.3,1/5.4,1/3.9,1/2.8,1/2.0};
    Double_t Intensity_Setup1[] = {0.0 * 17.2, 1 * 17.2 / 100.0, 1 * 17.2 / 68.1, 1 * 17.2 / 46.4, 1 * 17.2 / 31.6, 1 * 17.2 / 21.5, 1 * 17.2 / 14.7, 1 * 17.2 / 10.0, 1 * 17.2 / 6.81, 1 * 17.2 / 4.64, 1 * 17.2 / 3.16, 1 * 17.2 / 2.15};

    Double_t nDigis_Per_TS_Mean_Setup1_4075V[] = {-1, 288500, 421000, 514500, 796000, 1040000, 1674500, 2041000, 2984000, 3945000, 6037000, 7825000};
    Double_t nDigis_Per_TS_Mean_Setup1_4101V[] = {650, 315000, 457500, 595800, 861500, 1122500, 1812000, 2195000, -1, 4250000, 6445000, -1};
    Double_t nDigis_Per_TS_Mean_Setup1_4129V[] = {700, 354000, 514000, 617000, 954000, 1242000, 2002000, 2410000, 3495000, 4585000, 6992500, 8955000};

    // TGraph* gnDigis_Per_TS_Setup1_4075V = new TGraph(12,ATTF_Inv_Setup1,nDigis_Per_TS_Mean_Setup1_4075V);
    TGraph* gnDigis_Per_TS_Setup1_4075V = new TGraph(12, Intensity_Setup1, nDigis_Per_TS_Mean_Setup1_4075V);
    gnDigis_Per_TS_Setup1_4075V->SetTitle("nDigis Per TS vs. Intensity");
    // gnDigis_Per_TS_Setup1_4075V->GetXaxis()->SetTitle("(Attenuation Factor)^{-1}");
    gnDigis_Per_TS_Setup1_4075V->GetXaxis()->SetTitle("Gamma Intensity (MHz-cm^{-2})");
    gnDigis_Per_TS_Setup1_4075V->GetYaxis()->SetTitle("Mean nDigis Per TS");
    // gnDigis_Per_TS_Setup1_4075V->GetXaxis()->SetLimits(-0.02,0.55);
    gnDigis_Per_TS_Setup1_4075V->SetFillStyle(1000);
    gnDigis_Per_TS_Setup1_4075V->SetMarkerColor(1);
    gnDigis_Per_TS_Setup1_4075V->SetMarkerStyle(20);
    gnDigis_Per_TS_Setup1_4075V->SetMarkerSize(3.0);
    // gnDigis_Per_TS_Setup1_4075V->Draw("ap");

    // TGraph* gnDigis_Per_TS_Setup1_4101V = new TGraph(12,ATTF_Inv_Setup1,nDigis_Per_TS_Mean_Setup1_4101V);
    TGraph* gnDigis_Per_TS_Setup1_4101V = new TGraph(12, Intensity_Setup1, nDigis_Per_TS_Mean_Setup1_4101V);
    gnDigis_Per_TS_Setup1_4101V->SetTitle("nDigis Per TS vs. Intensity");
    // gnDigis_Per_TS_Setup1_4101V->GetXaxis()->SetTitle("(Attenuation Factor)^{-1}");
    gnDigis_Per_TS_Setup1_4101V->GetXaxis()->SetTitle("Gamma Intensity (MHz-cm^{-2})");
    gnDigis_Per_TS_Setup1_4101V->GetYaxis()->SetTitle("Mean nDigis Per TS");
    // gnDigis_Per_TS_Setup1_4101V->GetXaxis()->SetLimits(-0.02,0.55);
    gnDigis_Per_TS_Setup1_4101V->SetFillStyle(1000);
    gnDigis_Per_TS_Setup1_4101V->SetMarkerColor(1);
    gnDigis_Per_TS_Setup1_4101V->SetMarkerStyle(20);
    gnDigis_Per_TS_Setup1_4101V->SetMarkerSize(3.0);
    // gnDigis_Per_TS_Setup1_4101V->Draw("ap");
    // gnDigis_Per_TS_Setup1_4101V->Draw("p same");

    // TGraph* gnDigis_Per_TS_Setup1_4129V = new TGraph(12,ATTF_Inv_Setup1,nDigis_Per_TS_Mean_Setup1_4129V);
    TGraph* gnDigis_Per_TS_Setup1_4129V = new TGraph(12, Intensity_Setup1, nDigis_Per_TS_Mean_Setup1_4129V);
    gnDigis_Per_TS_Setup1_4129V->SetTitle("nDigis Per TS vs. Intensity");
    // gnDigis_Per_TS_Setup1_4129V->GetXaxis()->SetTitle("(Attenuation Factor)^{-1}");
    gnDigis_Per_TS_Setup1_4129V->GetXaxis()->SetTitle("Gamma Intensity (MHz-cm^{-2})");
    gnDigis_Per_TS_Setup1_4129V->GetYaxis()->SetTitle("Mean nDigis Per TS");
    // gnDigis_Per_TS_Setup1_4129V->GetXaxis()->SetLimits(-0.02,0.55);
    gnDigis_Per_TS_Setup1_4129V->SetFillStyle(1000);
    gnDigis_Per_TS_Setup1_4129V->SetMarkerColor(1);
    gnDigis_Per_TS_Setup1_4129V->SetMarkerStyle(20);
    gnDigis_Per_TS_Setup1_4129V->SetMarkerSize(3.0);
    // gnDigis_Per_TS_Setup1_4129V->Draw("ap");
    // gnDigis_Per_TS_Setup1_4129V->Draw("p same");
}
