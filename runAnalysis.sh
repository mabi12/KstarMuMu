#!/bin/bash

# Define the B mass range constants
B_MASS_MIN=5100
B_MASS_MAX=5500
RUN_SELECTCAND=true  # Default is to run selectCand

# Parse arguments (optional --skip-selectcand flag)
if [ "$1" == "--skip-selectcand" ]; then
    RUN_SELECTCAND=false
fi

# Conditionally run selectCand based on the RUN_SELECTCAND variable
if [ "$RUN_SELECTCAND" = true ]; then
    # Navigate to the directory containing the C++ script
    cd dataPreparation
    # Call the C++ function (assuming you're using ROOT to run the C++ code)
    root -l -b -q "selectCand.cxx(${B_MASS_MIN}, ${B_MASS_MAX}, \"ntuple-300700_part_02\", \"data_300700\", 42)"
    cd -  # Return to the previous directory
fi

# Navigate to the directory containing the Python script
cd fit
source /opt/root6/bin/thisroot.sh
# Call the Python script with argument parser
python3 run.py --B_mass_range ${B_MASS_MIN} ${B_MASS_MAX} --fit_functions "cb_voig_p331"
# Go back to the initial directory if needed
cd ../

