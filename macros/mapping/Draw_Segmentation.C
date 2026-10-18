// -----------------------------------------------------------------------------
// Copyright (C) Experimental High Energy Physics and Applications Group, VECC, Kolkata
// Draw_Segmentation.C
// Project : GEM detector test beam at CERN GIF++ (2023)
// Author  : Apar Agarwal
// Usage   : root -l -b -q 'Draw_Segmentation.C'  (see README for input file layout)
// -----------------------------------------------------------------------------

void Draw_Segmentation()
{
    TFile* fOut = new TFile("./data/Digi_Distributions/HV_Segmentation.root", "recreate");
    TString HistTag = "GEM_Foil_Boundaries";
    TString HistTitle = "GEM Boundaries";
    TH2F* GEM_Foil_Boundaries = new TH2F("GEM_Foil_Boundaries", "", 3640, -50, 90, 3280, 125, 195);
    GEM_Foil_Boundaries->GetXaxis()->SetTitle("X Coordinate (cm)");
    GEM_Foil_Boundaries->GetYaxis()->SetTitle("Y Coordinate (cm)");

    Bool_t Wall_Near_Smaller_Pads = true;

    if (Wall_Near_Smaller_Pads)
    {
        Float_t Radial_Coordinate = 18.30, Theta_Coordinate = 0.0;
        Float_t Radial_Coordinate_initial = 18.30;
        Float_t Radial_Coordinate_final = Radial_Coordinate = 18.3 + 5.7 + 4.8 + 8.0 + 8.8 + 9.5 + 7.7 + 7.3 + 6.4 + 6.3 + 5.8 + 5.3 + 4.7;
        Float_t Theta_Coordinate_initial = 0.;
        Float_t Theta_Coordinate_final = 0.;
        // Float_t Theta_Coordinate_initial=((Float_t) Column_number);
        // Float_t Theta_Coordinate_final=((Float_t) (Column_number+1));
        Float_t X_Coordinate, Y_Coordinate;
        Float_t X_offset = 19.4103 - 60.0, Y_offset = 75.1912 + 85.0;
        Float_t Pi = 3.1468;
        Int_t i = 0;

        // for(Int_t i=0;i<98;i++)
        // {
        //     Radial_Coordinate_final=Radial_Coordinate_initial+ Radial_Coordinate_initial*Pi/180.0;
        //     Radial_Coordinate_initial=Radial_Coordinate_final;
        // }

        // Radial_Coordinate_initial = 18.30;

        // Draw Right Boundary

        for (Radial_Coordinate = Radial_Coordinate_initial; Radial_Coordinate < Radial_Coordinate_final + 0.01; Radial_Coordinate += 0.002)
        {
            X_Coordinate = -1.0 * Radial_Coordinate * TMath::Sin(-101.5 * 3.14159 / 180.0);
            Y_Coordinate = -1.0 * Radial_Coordinate * TMath::Cos(-101.5 * 3.14159 / 180.0);
            GEM_Foil_Boundaries->Fill(X_offset + X_Coordinate, Y_offset + Y_Coordinate);
        }

        // Draw Middle Boundary

        for (Radial_Coordinate = Radial_Coordinate_initial; Radial_Coordinate < Radial_Coordinate_final + 0.01; Radial_Coordinate += 0.002)
        {
            X_Coordinate = -1.0 * Radial_Coordinate * TMath::Sin(-90.0 * 3.14159 / 180.0);
            Y_Coordinate = -1.0 * Radial_Coordinate * TMath::Cos(-90.0 * 3.14159 / 180.0);
            GEM_Foil_Boundaries->Fill(X_offset + X_Coordinate, Y_offset + Y_Coordinate);
        }

        // Draw Left Boundary

        for (Radial_Coordinate = Radial_Coordinate_initial; Radial_Coordinate < Radial_Coordinate_final + 0.01; Radial_Coordinate += 0.002)
        {
            X_Coordinate = -1.0 * Radial_Coordinate * TMath::Sin(-78.5 * 3.14159 / 180.0);
            Y_Coordinate = -1.0 * Radial_Coordinate * TMath::Cos(-78.5 * 3.14159 / 180.0);
            GEM_Foil_Boundaries->Fill(X_offset + X_Coordinate, Y_offset + Y_Coordinate);
        }

        // Draw smaller boundary

        Theta_Coordinate_initial = -101.5;
        Theta_Coordinate_final = -78.5;
        Radial_Coordinate = 18.3;

        for (Theta_Coordinate = Theta_Coordinate_initial; Theta_Coordinate < Theta_Coordinate_final + 0.01; Theta_Coordinate += 0.002)
        {
            X_Coordinate = -1.0 * Radial_Coordinate * TMath::Sin(Theta_Coordinate * 3.14159 / 180.0);
            Y_Coordinate = -1.0 * Radial_Coordinate * TMath::Cos(Theta_Coordinate * 3.14159 / 180.0);
            GEM_Foil_Boundaries->Fill(X_offset + X_Coordinate, Y_offset + Y_Coordinate);
        }

        // Draw bigger boundary

        Theta_Coordinate_initial = -101.5;
        Theta_Coordinate_final = -78.5;
        Radial_Coordinate = Radial_Coordinate_final;

        for (Theta_Coordinate = Theta_Coordinate_initial; Theta_Coordinate < Theta_Coordinate_final + 0.01; Theta_Coordinate += 0.002)
        {
            X_Coordinate = -1.0 * Radial_Coordinate * TMath::Sin(Theta_Coordinate * 3.14159 / 180.0);
            Y_Coordinate = -1.0 * Radial_Coordinate * TMath::Cos(Theta_Coordinate * 3.14159 / 180.0);
            GEM_Foil_Boundaries->Fill(X_offset + X_Coordinate, Y_offset + Y_Coordinate);
        }

        // Draw 1st boundary

        Theta_Coordinate_initial = -101.5;
        Theta_Coordinate_final = -78.5;
        Radial_Coordinate = 18.3 + 5.7;

        for (Theta_Coordinate = Theta_Coordinate_initial; Theta_Coordinate < Theta_Coordinate_final + 0.01; Theta_Coordinate += 0.002)
        {
            X_Coordinate = -1.0 * Radial_Coordinate * TMath::Sin(Theta_Coordinate * 3.14159 / 180.0);
            Y_Coordinate = -1.0 * Radial_Coordinate * TMath::Cos(Theta_Coordinate * 3.14159 / 180.0);
            GEM_Foil_Boundaries->Fill(X_offset + X_Coordinate, Y_offset + Y_Coordinate);
        }

        // Draw 2nd boundary

        Theta_Coordinate_initial = -101.5;
        Theta_Coordinate_final = -78.5;
        Radial_Coordinate = 18.3 + 5.7 + 4.8;

        for (Theta_Coordinate = Theta_Coordinate_initial; Theta_Coordinate < Theta_Coordinate_final + 0.01; Theta_Coordinate += 0.002)
        {
            X_Coordinate = -1.0 * Radial_Coordinate * TMath::Sin(Theta_Coordinate * 3.14159 / 180.0);
            Y_Coordinate = -1.0 * Radial_Coordinate * TMath::Cos(Theta_Coordinate * 3.14159 / 180.0);
            GEM_Foil_Boundaries->Fill(X_offset + X_Coordinate, Y_offset + Y_Coordinate);
        }

        // Draw 3rd boundary

        Theta_Coordinate_initial = -101.5;
        Theta_Coordinate_final = -78.5;
        Radial_Coordinate = 18.3 + 5.7 + 4.8 + 8.0;

        for (Theta_Coordinate = Theta_Coordinate_initial; Theta_Coordinate < Theta_Coordinate_final + 0.01; Theta_Coordinate += 0.002)
        {
            X_Coordinate = -1.0 * Radial_Coordinate * TMath::Sin(Theta_Coordinate * 3.14159 / 180.0);
            Y_Coordinate = -1.0 * Radial_Coordinate * TMath::Cos(Theta_Coordinate * 3.14159 / 180.0);
            GEM_Foil_Boundaries->Fill(X_offset + X_Coordinate, Y_offset + Y_Coordinate);
        }

        // Draw 4th boundary

        Theta_Coordinate_initial = -101.5;
        Theta_Coordinate_final = -78.5;
        Radial_Coordinate = 18.3 + 5.7 + 4.8 + 8.0 + 8.8;

        for (Theta_Coordinate = Theta_Coordinate_initial; Theta_Coordinate < Theta_Coordinate_final + 0.01; Theta_Coordinate += 0.002)
        {
            X_Coordinate = -1.0 * Radial_Coordinate * TMath::Sin(Theta_Coordinate * 3.14159 / 180.0);
            Y_Coordinate = -1.0 * Radial_Coordinate * TMath::Cos(Theta_Coordinate * 3.14159 / 180.0);
            GEM_Foil_Boundaries->Fill(X_offset + X_Coordinate, Y_offset + Y_Coordinate);
        }

        // Draw 5th boundary

        Theta_Coordinate_initial = -101.5;
        Theta_Coordinate_final = -78.5;
        Radial_Coordinate = 18.3 + 5.7 + 4.8 + 8.0 + 8.8 + 9.5;

        for (Theta_Coordinate = Theta_Coordinate_initial; Theta_Coordinate < Theta_Coordinate_final + 0.01; Theta_Coordinate += 0.002)
        {
            X_Coordinate = -1.0 * Radial_Coordinate * TMath::Sin(Theta_Coordinate * 3.14159 / 180.0);
            Y_Coordinate = -1.0 * Radial_Coordinate * TMath::Cos(Theta_Coordinate * 3.14159 / 180.0);
            GEM_Foil_Boundaries->Fill(X_offset + X_Coordinate, Y_offset + Y_Coordinate);
        }

        // Draw 6th boundary

        Theta_Coordinate_initial = -101.5;
        Theta_Coordinate_final = -78.5;
        Radial_Coordinate = 18.3 + 5.7 + 4.8 + 8.0 + 8.8 + 9.5 + 7.7;

        for (Theta_Coordinate = Theta_Coordinate_initial; Theta_Coordinate < Theta_Coordinate_final + 0.01; Theta_Coordinate += 0.002)
        {
            X_Coordinate = -1.0 * Radial_Coordinate * TMath::Sin(Theta_Coordinate * 3.14159 / 180.0);
            Y_Coordinate = -1.0 * Radial_Coordinate * TMath::Cos(Theta_Coordinate * 3.14159 / 180.0);
            GEM_Foil_Boundaries->Fill(X_offset + X_Coordinate, Y_offset + Y_Coordinate);
        }

        // Draw 7th boundary

        Theta_Coordinate_initial = -101.5;
        Theta_Coordinate_final = -78.5;
        Radial_Coordinate = 18.3 + 5.7 + 4.8 + 8.0 + 8.8 + 9.5 + 7.7 + 7.3;

        for (Theta_Coordinate = Theta_Coordinate_initial; Theta_Coordinate < Theta_Coordinate_final + 0.01; Theta_Coordinate += 0.002)
        {
            X_Coordinate = -1.0 * Radial_Coordinate * TMath::Sin(Theta_Coordinate * 3.14159 / 180.0);
            Y_Coordinate = -1.0 * Radial_Coordinate * TMath::Cos(Theta_Coordinate * 3.14159 / 180.0);
            GEM_Foil_Boundaries->Fill(X_offset + X_Coordinate, Y_offset + Y_Coordinate);
        }

        // Draw 8th boundary

        Theta_Coordinate_initial = -101.5;
        Theta_Coordinate_final = -78.5;
        Radial_Coordinate = 18.3 + 5.7 + 4.8 + 8.0 + 8.8 + 9.5 + 7.7 + 7.3 + 6.4;

        for (Theta_Coordinate = Theta_Coordinate_initial; Theta_Coordinate < Theta_Coordinate_final + 0.01; Theta_Coordinate += 0.002)
        {
            X_Coordinate = -1.0 * Radial_Coordinate * TMath::Sin(Theta_Coordinate * 3.14159 / 180.0);
            Y_Coordinate = -1.0 * Radial_Coordinate * TMath::Cos(Theta_Coordinate * 3.14159 / 180.0);
            GEM_Foil_Boundaries->Fill(X_offset + X_Coordinate, Y_offset + Y_Coordinate);
        }

        // Draw 9th boundary

        Theta_Coordinate_initial = -101.5;
        Theta_Coordinate_final = -78.5;
        Radial_Coordinate = 18.3 + 5.7 + 4.8 + 8.0 + 8.8 + 9.5 + 7.7 + 7.3 + 6.4 + 6.3;

        for (Theta_Coordinate = Theta_Coordinate_initial; Theta_Coordinate < Theta_Coordinate_final + 0.01; Theta_Coordinate += 0.002)
        {
            X_Coordinate = -1.0 * Radial_Coordinate * TMath::Sin(Theta_Coordinate * 3.14159 / 180.0);
            Y_Coordinate = -1.0 * Radial_Coordinate * TMath::Cos(Theta_Coordinate * 3.14159 / 180.0);
            GEM_Foil_Boundaries->Fill(X_offset + X_Coordinate, Y_offset + Y_Coordinate);
        }

        // Draw 10th boundary

        Theta_Coordinate_initial = -101.5;
        Theta_Coordinate_final = -78.5;
        Radial_Coordinate = 18.3 + 5.7 + 4.8 + 8.0 + 8.8 + 9.5 + 7.7 + 7.3 + 6.4 + 6.3 + 5.8;

        for (Theta_Coordinate = Theta_Coordinate_initial; Theta_Coordinate < Theta_Coordinate_final + 0.01; Theta_Coordinate += 0.002)
        {
            X_Coordinate = -1.0 * Radial_Coordinate * TMath::Sin(Theta_Coordinate * 3.14159 / 180.0);
            Y_Coordinate = -1.0 * Radial_Coordinate * TMath::Cos(Theta_Coordinate * 3.14159 / 180.0);
            GEM_Foil_Boundaries->Fill(X_offset + X_Coordinate, Y_offset + Y_Coordinate);
        }

        // Draw 11th boundary

        Theta_Coordinate_initial = -101.5;
        Theta_Coordinate_final = -78.5;
        Radial_Coordinate = 18.3 + 5.7 + 4.8 + 8.0 + 8.8 + 9.5 + 7.7 + 7.3 + 6.4 + 6.3 + 5.8 + 5.3;

        for (Theta_Coordinate = Theta_Coordinate_initial; Theta_Coordinate < Theta_Coordinate_final + 0.01; Theta_Coordinate += 0.002)
        {
            X_Coordinate = -1.0 * Radial_Coordinate * TMath::Sin(Theta_Coordinate * 3.14159 / 180.0);
            Y_Coordinate = -1.0 * Radial_Coordinate * TMath::Cos(Theta_Coordinate * 3.14159 / 180.0);
            GEM_Foil_Boundaries->Fill(X_offset + X_Coordinate, Y_offset + Y_Coordinate);
        }

        // Draw 12th boundary

        // Theta_Coordinate_initial = -101.5;
        // Theta_Coordinate_final   = -78.5;
        // Radial_Coordinate = 18.3 + 5.7 + 4.8 + 8.0 + 8.8 + 9.5 + 7.7 + 7.3 + 6.4 + 6.3 + 5.8 + 5.3 + 4.7;

        // for(Theta_Coordinate=Theta_Coordinate_initial;Theta_Coordinate<Theta_Coordinate_final+0.01;Theta_Coordinate+=0.002)
        // {
        //     X_Coordinate=-1.0*Radial_Coordinate*TMath::Sin(Theta_Coordinate*3.14159/180.0);
        //     Y_Coordinate=-1.0*Radial_Coordinate*TMath::Cos(Theta_Coordinate*3.14159/180.0);
        //     GEM_Foil_Boundaries->Fill(X_offset+X_Coordinate,Y_offset+Y_Coordinate);
        // }
    }
    // else
    // {
    //     Float_t Radial_Coordinate=18.30,Theta_Coordinate=0.0;
    //     Float_t Theta_Coordinate_initial=((Float_t) 22-Column_number)+90.0 - 23.0/2.0;
    //     Float_t Theta_Coordinate_final=(Float_t) (22-(Column_number-1))+90.0 - 23.0/2.0;
    //     // Float_t Theta_Coordinate_initial=((Float_t) Column_number);
    //     // Float_t Theta_Coordinate_final=((Float_t) (Column_number+1));
    //     Float_t X_Coordinate,Y_Coordinate;
    //     Float_t X_offset=19.4103+110.0 -60.0,Y_offset=75.1912 + 85.0;
    //     Float_t Pi=3.1468;
    //     Int_t i=0;

    //     Float_t Radial_Coordinate_initial=Radial_Coordinate;
    //     Float_t Radial_Coordinate_final=Radial_Coordinate;

    //     for(Int_t i=0;i<97;i++)
    //     {
    //         Radial_Coordinate_final=Radial_Coordinate_initial+ Radial_Coordinate_initial*Pi/180;

    //         if(i==Sector_number)
    //         break;

    //         Radial_Coordinate_initial=Radial_Coordinate_final;
    //     }

    //     if(top)
    //     {
    //         for(Theta_Coordinate=Theta_Coordinate_initial;Theta_Coordinate<Theta_Coordinate_final+0.01;Theta_Coordinate+=0.002)
    //         {
    //             X_Coordinate=-1.0*Radial_Coordinate_initial*TMath::Sin(Theta_Coordinate*3.14159/180.0);
    //             Y_Coordinate=-1.0*Radial_Coordinate_initial*TMath::Cos(Theta_Coordinate*3.14159/180.0);
    //             GEM_Foil_Boundaries->Fill(X_offset+X_Coordinate,Y_offset+Y_Coordinate);
    //         }
    //     }

    //     if(bottom)
    //     {
    //         for(Theta_Coordinate=Theta_Coordinate_initial;Theta_Coordinate<Theta_Coordinate_final+0.01;Theta_Coordinate+=0.002)
    //         {
    //             X_Coordinate=-1.0*Radial_Coordinate_final*TMath::Sin(Theta_Coordinate*3.14159/180.0);
    //             Y_Coordinate=-1.0*Radial_Coordinate_final*TMath::Cos(Theta_Coordinate*3.14159/180.0);
    //             GEM_Foil_Boundaries->Fill(X_offset+X_Coordinate,Y_offset+Y_Coordinate);
    //         }
    //     }

    //     if(right)
    //     {
    //         Theta_Coordinate=Theta_Coordinate_final;

    //         for(Radial_Coordinate=Radial_Coordinate_initial;Radial_Coordinate<Radial_Coordinate_final+0.01;Radial_Coordinate+=0.002)
    //         {
    //             X_Coordinate=-1.0*Radial_Coordinate*TMath::Sin(Theta_Coordinate*3.14159/180.0);
    //             Y_Coordinate=-1.0*Radial_Coordinate*TMath::Cos(Theta_Coordinate*3.14159/180.0);
    //             GEM_Foil_Boundaries->Fill(X_offset+X_Coordinate,Y_offset+Y_Coordinate);
    //         }
    //     }

    //     if(left)
    //     {
    //         Theta_Coordinate=Theta_Coordinate_initial;

    //         for(Radial_Coordinate=Radial_Coordinate_initial;Radial_Coordinate<Radial_Coordinate_final+0.01;Radial_Coordinate+=0.002)
    //         {
    //             X_Coordinate=-1.0*Radial_Coordinate*TMath::Sin(Theta_Coordinate*3.14159/180.0);
    //             Y_Coordinate=-1.0*Radial_Coordinate*TMath::Cos(Theta_Coordinate*3.14159/180.0);
    //             GEM_Foil_Boundaries->Fill(X_offset+X_Coordinate,Y_offset+Y_Coordinate);
    //         }
    //     }
    // }

    // GEM_Foil_Boundaries ->Draw();

    GEM_Foil_Boundaries->Write();
    fOut->Close();
}
