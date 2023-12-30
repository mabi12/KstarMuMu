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
# os.chdir("5dFit")
from Naming.mcDict import mcDict
from Naming.varDict import varDict
from Naming.cuts import cuts_run1, var
from Helpers.timeMeasuring import timer
from Fitting.Mass.Bfunctions import *
from Fitting.Mass.Kfunctions import *
from Fitting.Angles.Afunctions import *

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
variables = RooArgSet(B_cos_theta_K, B_cos_theta_L, B_phi, m_B, m_Kstar)
data = RooDataSet("data", "My RooDataSet", ch, variables)

# class fitFunction():
#     def __init__(self, function, params, n_params):
#         self.f = function
#         self.ps = params
#         self.n_params = n_params

#     def __mul__(self, other):
#         pass
#         # TODO Treturn fitFunction()

angularPdf = RooProdPdf("angularPdf", "angularPdf", RooArgList(poly_theta_K, poly_theta_L, poly_phi ))
prodPdf = RooProdPdf("prodPdf", "prodPdf", RooArgList(johnson, voigt, angularPdf))
# [3,3,3,4,2]):

fit_result = prodPdf.fitTo(data, RooFit.Save())
fit_result.Print("v")
# param_values.append(variable.getVal())
chi2s.append(fit_result.Chi2()/fit_result.NDF())
var_Plot =  ROOT.MyPlot(variable, 250, "./out/", "Internal")
var_Plot.plotVarAndPull(data, fit, number, f"{variable}")
# chi2


# with open(csv_file_path, "a") as csv_file:
#      writer = csv.writer(csv_file, delimiter=" ")
#      writer.writerow(["Variable", "Parameter Value", "Chi2"])
#      for variable, param_value, chi2 in zip(variable_names, param_values, chi2s):
#         writer.writerow([variable, param_value, chi2])


