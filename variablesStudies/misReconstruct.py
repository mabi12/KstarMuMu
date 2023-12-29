import sys, os
from ROOT import *
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
# os.chdir("massFit")
from Naming.mcDict import mcDict
from Naming.varDict import varDict
from Naming.cuts import cuts_run1, var
from Helpers.timeMeasuring import timer
from Fitting.mass.Bfunctions import *
from Fitting.mass.Kfunctions import *

gROOT.SetBatch(True)

# Load the header and implementation files
gROOT.ProcessLine('.L ../fitCore/MyPlot.h')
gROOT.ProcessLine('.L ../fitCore/MyPlot.cxx')

# Declare the class in PyROOT
gROOT.ProcessLine("class MyPlot;")

mc_dict = mcDict(debugPrint=False)
var_dict = varDict()

# ch = TChain("Nominal/BaseSelection_KStarMuMu_BmumuKstSelection", "Nominal/BaseSelection_KStarMuMu_BmumuKstSelection")
ch = TChain("BdBestChi")
ch.Add(f"../data/ntuple-{mc_dict['Bd_Kstar_mumu']}_part_combi12_bestCand.root")
# ch.Add(f"../data/ntuple-{mc_dict['Bdbar_Kstar_mumu']}_part_01.root")
# ch.Add(f"../data/ntuple-data18_13TeV_periodK_part_03.root")


ks = var_dict["Kstar_mass"]
ksb = var_dict["Kstarb_mass"]
cut = "BmumuKst_isTrueNonresBd==1"

sig_range = (846, 946)
pdg = 896
# h_ks = TH1F("h_ks", "Kstar_mass Histogram", 100, sig_range[0], sig_range[1])
# h_ksb = TH1F("h_ksb", "Kstar_bar_mass Histogram", 100, sig_range[0], sig_range[1])

# ch.Draw(ks+">>h_ks", cut+"&&"+ks+">"+str(sig_range[0])+"&&"+ks+"<"+str(sig_range[1]))
# ch.Draw(ksb+">>h_ksb", cut+"&&"+ksb+">"+str(sig_range[0])+"&&"+ksb+"<"+str(sig_range[1]))

# h_ks.Scale(1.0/h_ks.Integral())
# h_ksb.Scale(1.0/h_ksb.Integral())
# h_ksb.SetLineColor(kRed)

# c.cd(1)
# h_ks.Draw()
# h_ksb.Draw("same")

# c.cd(2)
range = (-55, 55)

ks = var_dict["Kstar_mass"]
ksb = var_dict["Kstarb_mass"]
cut = "BmumuKst_isTrueNonresBd==1"

hksp = TH1F("h_ksp", "Kstar_mass Histogram", 100, 0, range[1])
hksm = TH1F("h_ksm", "Kstar_mass Histogram", 100, range[0], 0)
hksbp = TH1F("h_ksbp", "Kstar_mass Histogram", 100, 0, range[1])
hksbm = TH1F("h_ksbm", "Kstar_mass Histogram", 100, range[0], 0)
# h_ksb = TH1F("h_ksb", "Kstar_bar_mass Histogram", 100, range[0], range[1])

ch.Draw(str(pdg)+"-"+ks +">>h_ksp", str(pdg)+"-"+ks +">0 &&"+cut +"&&"+ks+">"+str(sig_range[0])+"&&"+ks+"<"+str(sig_range[1])+"&&"+ksb+">"+str(sig_range[0])+"&&"+ksb+"<"+str(sig_range[1])) #   +">>h_diff"
ch.Draw(str(pdg)+"-"+ks +">>h_ksm", str(pdg)+"-"+ks +"<0 &&"+cut +"&&"+ks+">"+str(sig_range[0])+"&&"+ks+"<"+str(sig_range[1])+"&&"+ksb+">"+str(sig_range[0])+"&&"+ksb+"<"+str(sig_range[1])) #   +">>h_diff"
ch.Draw(str(pdg)+"-"+ksb +">>h_ksbp", str(pdg)+"-"+ksb +">0 &&"+cut +"&&"+ks+">"+str(sig_range[0])+"&&"+ks+"<"+str(sig_range[1])+"&&"+ksb+">"+str(sig_range[0])+"&&"+ksb+"<"+str(sig_range[1])) #   +">>h_diff"
ch.Draw(str(pdg)+"-"+ksb +">>h_ksbm", str(pdg)+"-"+ksb +"<0 &&"+cut +"&&"+ks+">"+str(sig_range[0])+"&&"+ks+"<"+str(sig_range[1])+"&&"+ksb+">"+str(sig_range[0])+"&&"+ksb+"<"+str(sig_range[1])) #   +">>h_diff"
# ch.Draw(ksb+">>h_ksb", cut+"&&"+ksb+">"+str(range[0])+"&&"+ksb+"<"+str(range[1]))

c = TCanvas("c", "c", 1024,1024)
c.Divide(2,2)
c.cd(1)
hksm.Draw()
c.cd(2)
hksp.Draw()
c.cd(3)
hksbm.Draw()
c.cd(4)
hksbp.Draw()

# h_ks.Scale(1.0/h_ks.Integral())
# h_ksb.Scale(1.0/h_ksb.Integral())
# h_ksb.SetLineColor(kRed)




# range = (0, 100)
# ks = var_dict["Kstar_mass"]+"_err"
# ksb = var_dict["Kstarb_mass"]+"_err"

# h_ks = TH1F("h_ks_err", "Kstar_mass Histogram", 100, range[0], range[1])
# h_ksb = TH1F("h_ksb_err", "Kstar_bar_mass Histogram", 100, range[0], range[1])

# ch.Draw(ks+">>h_ks_err", cut+"&&"+ks+">"+str(range[0])+"&&"+ks+"<"+str(range[1]))
# ch.Draw(ksb+">>h_ksb_err", cut+"&&"+ksb+">"+str(range[0])+"&&"+ksb+"<"+str(range[1]))

# h_ks.Scale(1.0/h_ks.Integral())
# h_ksb.Scale(1.0/h_ksb.Integral())
# h_ksb.SetLineColor(kRed)

# c.cd(1)
# ch.Draw(ks+"_err","BmumuKst_isTrueNonresBd==1")
# c.cd(1)
# ch.Draw(ks+"_err","BmumuKst_isTrueNonresBd==1")


c.SaveAs("./out/kstar.pdf")
# ch.Draw(var_dict['B_mass'],"BmumuKst_isTrueNonresBd==0")