# Bd->K*mumu Angular Analysis

This repository contains tools and modules for performing the mass and angular analysis of the decay process Bd→K*mumu. It is organized into directories for different functionalities.

## Directory Structure

### Main Directories
- **Actual tests or systematics** (lowercase):
  - `mcStudies`
  - `triggerStudies`
  - `mass`
- **Helper classes** (uppercase):
  - `Fitting`
    - `Mass`
    - `Angles`
  - `Helpers`
  - `Naming`
  - `Plotting`
  - `Data Preparation`
  - `Running the Analysis`
- **`data`** directory

### `data` Directory
Ntuples are stored in the following path:
```
/eos/atlas/atlascerngroupdisk/phys-beauty/RKstarRun2/ntuples/muon_channel_v0.9/ntuples_updated/
```

## Naming

### `mcDict.py`

Example usage:
```python
path = "../data/"
decay = "Bdbar_Kstar_mumu"
from Naming.mcDict import mcDict
mc_dict = mcDict(debugPrint=False)
file_name = os.path.join(path, "ntuple-" + mc_dict[decay] + "_part_01.root")
```

#### MC Files
- `Bd_Kstar_Jpsi_ee`: 300592
- `Bd_Kstar_mumu`: 300700
- `Bdbar_Kstar_mumu`: 300701
- `Bd_Kstar_Jpsi_mumu`: 300702
- `Bdbar_Kstar_Jpsi_mumu`: 300703
- `Bd_Kstar_psi2S_mumu`: 300704
- `Bdbar_Kstar_psi2S_mumu`: 300705
- `Bd_Kpi_Jpsi_mumu`: 300736
- `Bdbar_Kpi_Jpsi_mumu`: 300737
- `Bd_Kpi_psi2S_mumu`: 300740
- `Bdbar_Kpi_psi2S_mumu`: 300741
- `Bd_Kstar_eta_mumu`: 300746
- `Bdbar_Kstar_eta_mumu`: 300747

### `cuts.py`

Example usage:
```python
from Naming.cuts import cuts_run1
# Use cuts_run1 as a string
```

## Helpers

### `timeMeasuring.py`

Example usage:
```python
from Helpers.timeMeasuring import timer
timer.start()
# Perform time-consuming operation
timer.print("looped over triggers")
# Output: "looped over triggers", "{timer.time()} s"
```

## Fitting

### Mass

The `Mass` module in the `Fitting` directory provides tools for fitting mass distributions using ROOT's RooFit library. The module supports multiple fit functions such as Crystal Ball, Gaussian, Double Gaussian, Johnson, and combinations with exponential functions for B mesons and Voigtian for K* mesons.

#### Features
- Flexible initialization with custom mass and error ranges.
- Predefined fit functions available by tags.

#### Example Usage
```python
from Fitting.BFit import BFit
from Fitting.KFit import KFit

# Initialize the fitter
b_fit = BFit([5000, 6000], [0.1, 10])
k_fit = KFit([800, 946], [0.1, 20])

# Define fit functions
b_fit.define_fit_functions()
k_fit.define_fit_functions()

# Retrieve a specific fit function
fit_func_b = b_fit.get_fit_function('cb')   # Example: Crystal Ball
fit_func_k = b_fit.get_fit_function('voig') # Example: Voigtian

print(fit_func)
```

### Angles

The `Angles` module in the `Fitting` directory provides tools for angular fits in the Bd→K*mumu decay analysis using ROOT's RooFit library. It supports polynomial n-th degree fits for angular distributions: `cos(θK)`, `cos(θL)`, and `φ`.

#### Features
- Configurable polynomial degrees for each angular distribution.
- Supports dynamic generation of polynomial fit functions.

#### Example Usage
```python
from Fitting.AFit import AFit

# Initialize the angular fitter with specific polynomial degrees
a_fit = AFit(ctk_degree=3, ctl_degree=2, phi_degree=1)

# Define the fit functions
a_fit.define_fit_functions()

# Retrieve specific angular fit functions
poly_theta_K, poly_theta_L, poly_phi = a_fit.get_fit_function('p321')  # Degrees: 3 for θK, 2 for θL, 1 for φ
print(poly_theta_K, poly_theta_L, poly_phi)

# Retrieve the degrees of freedom (ndof) for the fit
ndof_ctk, ndof_ctl, ndof_phi = a_fit.get_ndof('p321')
print(f"Degrees of Freedom: θK={ndof_ctk}, θL={ndof_ctl}, φ={ndof_phi}")
```

## Data Preparation

### `selectcand.cxx`

#### Parameters
- **`B_mass_min`**: Lower bound for B meson mass selection (integer).
- **`B_mass_max`**: Upper bound for B meson mass selection (integer).
- **`inputFileName`**: Input ROOT file containing raw data (TString).
- **`outputFileName`**: Name of the output file where the processed data will be stored (TString).
- **`seed`**: The seed value for reproducibility.

#### Example Usage
```cpp
#include "selectCand.h"

int main() {
    // You can also provide custom parameters if needed
    selectCand(5200, 5300, "ntuple-300700_part_03", "data_300700_filtered", 42);
    return 0;
}
```

## Running the Analysis

### `runAnalysis.sh`

The script orchestrates the analysis by preparing data and running fits.

#### Features
- Runs `selectcand.cxx` for data filtering.
- Executes `run.py` with command-line parameters.

#### Parameters of `run.py`

- **`--fit_functions`**: Specifies the fit functions for mass and angular distributions.
  - **Default**: `john_voig_p331`.
  - **Details**:
    - `s`: Spherical harmonics.
    - `p`: Product of polynomials.
    - **Numbers**: Polynomial orders for `cos(θK)`, `cos(θL)`, and `φ`.

- **`--B_mass_range`**: B mass range (array of 2 integers).
  - **Default**: `[5000, 5700]`.

- **`--K_mass_range`**: K* mass range (array of 2 integers).
  - **Default**: `[846, 946]`.

- **`--B_mass_err_range`**: B mass error range (array of 2 integers).
  - **Default**: `[0, 50]`.

- **`--K_mass_err_range`**: K* mass error range (array of 2 integers).
  - **Default**: `[0, 50]`.

