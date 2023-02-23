import sys, os
import ROOT
import numpy as np
import uproot3 as uproot
import uproot3_methods as um
import collections
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages
from itertools import chain
from tqdm import tqdm

#hack to load from parent directory
sys.path.append('../') 
os.chdir("triggerStudies")

from Naming.mcDict import mcDict
from Naming.cuts import cuts_run1, var
from Helpers.timeMeasuring import timer

#turn off graphics mode
ROOT.gROOT.SetBatch(True)

### SETUP VARIABLES ###
# path = "/eos/atlas/atlascerngroupdisk/phys-beauty/RKstarRun2/ntuples/muon_channel_v0.9/ntuples_updated/"
path = "../data/"
decay = "Bdbar_Kstar_mumu"
# max_events = 10_000
max_events = -1
load_string = "/accepted_triggers"# |event_number"
load_variables = [ ]
                #      "B_nDoF",
                #     "B_chi2",
                #     "B_mass",
                #     "Bbar_mass",
                #     "meson0_pT",
                #     "meson1_pT"
                #     #|BmumuKst_charge_meson0|B_mass_err|BmumuKst_pT_muon0|BmumuKst_pT_muon1|BmumuKst_eta_muon0|BmumuKst_eta_muon1|B_pT|pass_GRL|run_number|B_mu1_eta|B_mu1_pT|B_mu2_eta|B_mu2_pT|B_Jpsi_mass/
                # ]
#######################

mc_dict = mcDict(debugPrint=False)

file_name = os.path.join(path,"ntuple-"+mc_dict[decay]+"_part_01.root")

#append load variables to liad string
for v in load_variables:
    load_string+="|BmumuKst_"+v
load_string+="/"

#load data
timer.start()
allCand = uproot.open(file_name)["Nominal/BaseSelection_KStarMuMu_BmumuKstSelection"]
if max_events and max_events > 0:
    vtx =  allCand.arrays(load_string, outputtype = collections.namedtuple, entrystop = max_events) 
else:
    vtx =  allCand.arrays(load_string, outputtype = collections.namedtuple)
print("data loaded",f"{timer.time()} s")

# top = "HLT_j0_perf_L1RD0_FILLED"
trig_counter = {}

timer.start()
x = vtx.accepted_triggers
print(len(x), "events")
# for i, lt in enumerate(x):
#     for j,t in enumerate(lt):
#         x[i][j]=str(t)
        
for lt in tqdm(x):
    # if top not in lt:
    for t in lt:
        t=str(t)
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

sorted_d = dict(sorted(trig_counter.items(), key=lambda x: -x[1]))

with open(f'out/trigList{decay}.txt', 'w') as the_file:
    for t in sorted_d:
        if not "L1_" in t:
            the_file.write(str(t).replace("b'","").replace("'","")+": "+str(sorted_d[t])+"\n")

# print("events:",len(vtx.event_number),"candidates", len(vtx.BmumuKst_B_mass))

#print (len(vtx.evt_number))
#print (len(vtx2.HLT_2mu6_bJpsimumu_L1BPH_2M9_2MU6_BPH_2DR15_2MU6))
#for i in range(10):
#    print(vtx2[i].HLT_2mu6_bJpsimumu_L1BPH_2M9_2MU6_BPH_2DR15_2MU6)