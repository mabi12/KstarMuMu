import sys, os
import ROOT
import numpy as np
import uproot as uproot
import uproot_methods as um
import collections
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages
from itertools import chain
from datetime import datetime

#hack to load from parent directory
sys.path.append('/home/marek/Kstarmumu/') 
os.chdir("triggerStudies")

from Naming.mcDict import mcDict
from Naming.cuts import cuts_run1, var
from Helpers.timeMeasuring import timer

#turn off graphics mode
ROOT.gROOT.SetBatch(True)

path = "/eos/atlas/atlascerngroupdisk/phys-beauty/RKstarRun2/ntuples/muon_channel_v0.9/ntuples_updated/"
path = "../data/"

mc_dict = mcDict(debugPrint=False)

file_name = os.path.join(path,"ntuple-"+mc_dict[decay]+"_part_01.root")

max_events = 10_000
# max_events = -1

load_string = "/accepted_triggers|event_number"
load_variables = [ ]
                #      "B_nDoF",
                #     "B_chi2",
                #     "B_mass",
                #     "Bbar_mass",
                #     "meson0_pT",
                #     "meson1_pT"
                #     #|BmumuKst_charge_meson0|B_mass_err|BmumuKst_pT_muon0|BmumuKst_pT_muon1|BmumuKst_eta_muon0|BmumuKst_eta_muon1|B_pT|pass_GRL|run_number|B_mu1_eta|B_mu1_pT|B_mu2_eta|B_mu2_pT|B_Jpsi_mass/
                # ]
for v in load_variables:
    load_string.append("|BmumuKst_"+v)

timer.start()
allCand = uproot.open(file_name)["Nominal/BaseSelection_KStarMuMu_BmumuKstSelection"]
if max_events and max_events > 0:
    vtx =  allCand.arrays(load_string, outputtype = collections.namedtuple, entrystop = max_events) 
else:
    vtx =  allCand.arrays(load_string, outputtype = collections.namedtuple)

print("data loaded",f"{timer.time()} s")

top = "HLT_j0_perf_L1RD0_FILLED"
trig_counter = {}

timer.start()
x = vtx.accepted_triggers
for i, lt in enumerate(x):
    for j,t in enumerate(lt):
        x[i][j]=str(t)
        
for lt in x:
    if top not in lt:
        for t in lt:
            print(type(t))
            if t in trig_counter:
                trig_counter[t] += 1
            else:
                trig_counter[t] = 1

#data loaded 07/02/2023 - 20:32:07
#data loaded 07/02/2023 - 20:35:29
#
# for t in trig_counter:            
#     if trig_counter[t]>800_000:
#         print(t,trig_counter[t])
# dd/mm/YY H:M:S
print("looped over triggers",f"{timer.time()} s")

with open(f"out/{decay}_{cutFlag}.log", 'w') as f:
    f.write(f"{cutFlag} {cut} \n")
    f.write(f"events: {N_events}\n")
    f.write(f"all: {N_all}, passed run1 cuts: {N_passed}\n")
    f.write(f"selected: {N_cut}, passed run1 cuts: {N_cut_passed}\n")
    f.write(f"selected over events: {N_cut/N_events}, passed run1 cuts: {N_cut_passed/N_events}\n")
    f.write("\n")
    f.write("plotted vars in range:\n")
    for r in range_:
        f.write(f"{r} - {range_[r]} \n")
    f.write("\n")
    f.write("\n")
    f.write(f"{cuts_run1}\n")

# print("events:",len(vtx.event_number),"candidates", len(vtx.BmumuKst_B_mass))

#print (len(vtx.evt_number))
#print (len(vtx2.HLT_2mu6_bJpsimumu_L1BPH_2M9_2MU6_BPH_2DR15_2MU6))
#for i in range(10):
#    print(vtx2[i].HLT_2mu6_bJpsimumu_L1BPH_2M9_2MU6_BPH_2DR15_2MU6)