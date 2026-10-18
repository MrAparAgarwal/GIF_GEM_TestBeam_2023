# GEM Detector Test Beam at CERN GIF++ (2023)

ROOT/CbmRoot analysis macros for a triple-GEM module of the CBM Muon Chamber (MuCh) system,
tested at CERN's Gamma Irradiation Facility (GIF++) in 2023. GIF++ combines a high-activity
<sup>137</sup>Cs gamma source with a muon beam, so the detector can be studied under
a high background rate similar to running conditions.

The macros take unpacked, free-streaming (triggerless) data and cover the full chain:
data cleaning and time-offset correction, clustering, ADC/cluster-charge spectra,
spill structure, time correlations with the RPC and scintillator triggers, acceptance
correction and efficiency versus high voltage.

<!-- Publication: add the citation/DOI of the GIF++ paper here. -->

## Repository layout

```
macros/
  preprocessing/   data cleaning, time-offset correction, clustering, unpacked-file inspection
  analysis/        ADC spectra, digi distributions, hit maps, spill structures,
                   time correlations, acceptance and efficiency studies
  mapping/         FEB / CROB / pad-segmentation drawing for the GEM readout plane
  plotting/        summary plots (gain, time-correlation sigma, nDigis vs HV)
data/              input and output ROOT files (not tracked, see data/README.md)
rootlogon.C        common plotting style, loaded automatically by ROOT
```

## Analysis chain

1. **Clean the raw data**:
   `GIF_GEM_Datafile_Cleaner_and_Offset_Corrector.C` corrects the time offset between the
   trigger and the GEM and masks noisy pads.
2. **Characterise noise**:
   `GIF_GEM_Noise_Analysis.C` studies noise behaviour and motivates the choice of threshold.
3. **Cluster hits**: `GIF_GEM_Clusterizer.C`.
4. **Physics and performance studies**: the macros in `macros/analysis/`, for example:
   - `GIF_GEM_ADC_Spectra*.C`: ADC and cluster-charge spectra (on-spill, off-spill,
     gamma background only, cross-talk only, with ADC cut, acceptance-corrected)
   - `GIF_GEM_*Spillstructures.C`: spill structure with and without trigger
   - `GIF_GEM_*Timecorrelations*.C`: GEM–trigger and inter-FEB time correlations
   - `GIF_GEM_*RPC_Acceptance_Finder_Three_Fold.C`: common GEM–RPC acceptance
   - `GIF_GEM_With_Scintillator_Trigger_Efficiency*.C`: detection efficiency
5. **Summary plots**: `macros/plotting/`.

## Requirements

- [CbmRoot](https://git.cbm.gsi.de/computing/cbmroot) (DEC21 or newer), which provides the
  `CbmMuchDigi`, `CbmTofDigi` and related data classes
- FairSoft APR21p2 / FairRoot v18.6.7 (tested on Ubuntu 20.04)

## Usage

Set up the CbmRoot environment, then run from the repository root so that `rootlogon.C` and
`data/` are picked up:

```bash
source /path/to/cbmroot/build/config.sh
mkdir -p data/{ADC_Related_Plots,Digi_Distributions,Efficiency,nDigis_Per_TS_Distributions,Noise_Analysis,Spill_Structures,Time_Correlations}

root -l -b -q 'macros/preprocessing/GIF_GEM_Clusterizer.C(966, 50, false)'
root -l -b -q 'macros/analysis/GIF_GEM_Timecorrelations.C(966)'
```

Most macros take the run number as their first argument; check each macro's signature for
the other parameters and their defaults.

### Auxiliary text inputs

Besides the ROOT files, several macros read small text files from the working directory.
These files are not part of this repository:

| File | Content |
|------|---------|
| `PadX_GEM.txt`, `PadY_GEM.txt` | channel-to-pad (column/sector) mapping of the GEM readout |
| `Run_<RunId>_GEM_Noise.txt`, `Run_<RunId>_RPC_Noise.txt` | list of noisy pads masked for that run |
| `Three_Fold_Area_Config_*.txt`, `Three_Fold_Clean_Gaussian_Profile.txt` | GEM–RPC common-acceptance definitions |

## Author

Apar Agarwal

## Copyright

Copyright (C) Experimental High Energy Physics and Applications Group, VECC, Kolkata.
