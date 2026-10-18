# data/

All macros read their inputs from, and write their outputs to, this folder (relative to the
repository root, where ROOT should be started). Its contents are ignored by git.

Expected layout:

```
data/
  Run<RunId>.digi.root          # unpacked GIF++ runs (CbmRoot digi files)
  ADC_Related_Plots/
  Digi_Distributions/
  Efficiency/
  nDigis_Per_TS_Distributions/
  Noise_Analysis/
  Spill_Structures/
  Time_Correlations/
```

Create the sub-folders once with:

```bash
mkdir -p data/{ADC_Related_Plots,Digi_Distributions,Efficiency,nDigis_Per_TS_Distributions,Noise_Analysis,Spill_Structures,Time_Correlations}
```

If your files live elsewhere (e.g. on a cluster file system), symlink them here instead of
editing the macros: `ln -s /path/to/unpacked/files/*.digi.root data/`.
