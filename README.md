## Bd->K*mumu angular analysis

Repository includes 2 types of directories:
- actual tests or systematics (starting with lower case)
  - mcStudies
  - ...
- helping classes (starting with upper case)
  - Fitting
    - Mass
    - ...
  - ...  

# Naming
includes naming conventions:
```
path = "../data/"
decay = "Bdbar_Kstar_mumu"
from Naming.mcDict import mcDict
mc_dict = mcDict(debugPrint=False)
file_name = os.path.join(path,"ntuple-"+mc_dict[decay]+"_part_01.root")
``` 
MC files:
- Bd_Kstar_Jpsi_ee 300592
- Bd_Kstar_mumu 300700
- Bdbar_Kstar_mumu 300701
- Bd_Kstar_Jpsi_mumu 300702
- Bdbar_Kstar_Jpsi_mumu 300703
- Bd_Kstar_psi2S_mumu 300704
- Bdbar_Kstar_psi2S_mumu 300705
- Bd_Kpi_Jpsi_mumu 300736
- Bdbar_Kpi_Jpsi_mumu 300737
- Bd_Kpi_psi2S_mumu 300740
- Bdbar_Kpi_psi2S_mumu 300741
- Bd_Kstar_eta_mumu 300746
- Bdbar_Kstar_eta_mumu 300747

