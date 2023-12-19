import sys, os
from ROOT import *
import ROOT
import csv 
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
os.chdir("5dFit")
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

csv_file_path = "./out/fit_results.csv"

ch = TChain("BdBestChi")
ch.Add(f"../data/sg_wide_bestCand.root")
c = TCanvas("c", "c", 512,512)

chi2s = []
param_values = []
variable_names = ["B_cos_theta_K", "B_cos_theta_L", "B_phi", "m_B", "m_Kstar"]

for variable, fit, number in zip([B_cos_theta_K, B_cos_theta_L, B_phi, m_B, m_Kstar], [poly_theta_K, poly_theta_L, poly_phi, johnson, voigt], [3,3,3,4,2]):
    data = RooDataSet("data", "My RooDataSet", ch, RooArgSet(variable))
    fit_result = fit.fitTo(data, RooFit.Save())
    param_values.append(variable.getVal())
    chi2s.append(fit_result.Chi2())
    var_Plot =  ROOT.MyPlot(variable, 250, "./out/", "Internal")
    var_Plot.plotVarAndPull(data, fit, number, f"{variable}")

with open(csv_file_path, "a") as csv_file:
     writer = csv.writer(csv_file, delimiter=" ")
     writer.writerow(["Variable", "Parameter Value", "Chi2"])
     for variable, param_value, chi2 in zip(variable_names, param_values, chi2s):
        writer.writerow([variable, param_value, chi2])


