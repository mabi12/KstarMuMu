import ROOT
import numpy as np
import uproot as uproot
import uproot_methods as um
import collections
import matplotlib.pyplot as plt

from matplotlib.backends.backend_pdf import PdfPages
from itertools import chain

from datetime import datetime

file_name = "../data/ntuple-300700_part_01.root"

max_events = 10_000
# max_events = -1

load_string = "/accepted_triggers|event_number|BmumuKst_B_nDoF|BmumuKst_B_chi2|BmumuKst_B_mass|BmumuKst_pT_meson0|BmumuKst_pT_meson1|BmumuKst_charge_meson0|B_mass_err|BmumuKst_pT_muon0|BmumuKst_pT_muon1|BmumuKst_eta_muon0|BmumuKst_eta_muon1|B_pT|pass_GRL|run_number|B_mu1_eta|B_mu1_pT|B_mu2_eta|B_mu2_pT|B_Jpsi_mass/" #|Jpsi_m+|B_+|pass_GRL
#trigString = "/HLT_2mu6_bJpsimumu_L1BPH_2M9_2MU6_BPH_2DR15_2MU6|HLT_2mu6_bBmumuxv2_L1LFV_MU6|HLT_mu11_mu6_bDimu|HLT_mu11_mu6_bBmumuxv2|HLT_mu11_2mu4noL1_bNocut_L1MU11_2MU6|HLT_mu20_2mu2noL1_JpsimumuFS|HLT_mu20_2mu4_JpsimumuL2|HLT_mu6_mu4_bBmumuxv2|HLT_2mu14|HLT_mu50|HLT_mu6_mu4_bBmumux_BsmumuPhi_L1BPH_2M9_MU6MU4_BPH_0DR15_MU6MU4|HLT_2mu4_bBmumux_BsmumuPhi_L1BPH_2M9_2MU4_BPH_0DR15_2MU4|HLT_mu22_mu8noL1/"

print(name,max_events)