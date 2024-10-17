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
import re
import argparse

script_dir = os.path.dirname(os.path.abspath(__file__))
parent_dir = os.path.join(script_dir, '..')
fit_dir = os.path.join(parent_dir, 'fit')
sys.path.append(parent_dir)
if os.path.exists(fit_dir):
    os.chdir(fit_dir)
else:
    raise FileNotFoundError(f"The directory '{fit_dir}' does not exist.")

from Naming.mcDict import mcDict
from Naming.varDict import varDict
from Naming.cuts import cuts_run1, var
from Helpers.timeMeasuring import timer
from Fitting.Mass.Bfunctions import BFit
from Fitting.Mass.Kfunctions import KFit
from Fitting.Angles.Afunctions import AFit
from Helpers.nameCleaning import *

parser = argparse.ArgumentParser(description='Process input parameters for the ML fit.')
parser.add_argument('--fit_functions', default="dg_voig_p331", help='fitfunctions for Bmass_Kstmass_angles, s for spherical harmonics, p for product of polynomials, numbers for the order of polynomial for cos theta_K, cos theta_L, phi respectively')
parser.add_argument('--B_mass_range', nargs = 2, type = float, default=[5000, 5700], help='array of 2 int: [min_B_mass, max_B_mass]')
parser.add_argument('--K_mass_range', nargs = 2, type = float, default=[846, 946], help='array of 2 int: [min_K_mass, max_K_mass]')
parser.add_argument('--B_mass_err_range', nargs = 2, type = float, default=[0, 50], help='array of 2 int: [min_B_mass_err, max_B_mass_err]')
parser.add_argument('--K_mass_err_range', nargs = 2, type = float, default=[0, 50], help='array of 2 int: [min_K_mass_err, max_K_mass_err]')
#TODO parser bool var for data preparation
args = parser.parse_args()

gROOT.SetBatch(True)
gROOT.ProcessLine('.L ../fitCore/MyPlot.h')
gROOT.ProcessLine('.L ../fitCore/MyPlot.cxx')
gROOT.ProcessLine("class MyPlot;")

def main():
    chi2s = []
    param_values = []

    BFit_instance = BFit(args.B_mass_range, args.B_mass_err_range)
    BFit_instance.define_fit_functions()
    KFit_instance = KFit(args.K_mass_range, args.K_mass_err_range)
    KFit_instance.define_fit_functions()
    AFit_instance = AFit()
    AFit_instance.define_fit_functions()

    B_mass_fit_function = BFit_instance.get_fit_function(args.fit_functions)
    K_mass_fit_function = KFit_instance.get_fit_function(args.fit_functions)
    poly_theta_K, poly_theta_L, poly_phi = AFit_instance.get_fit_function(args.fit_functions)
    ctk_ndof, ctl_ndof, phi_ndof = AFit_instance.get_ndof(args.fit_functions)

    ch = TChain("BdBestChi")
    ch.Add("../data/data_300700_bestCand.root")
    c = TCanvas("c", "c", 512,512)

    variables = RooArgSet(BFit_instance.m_B, KFit_instance.m_Kstar, AFit_instance.B_cos_theta_K, AFit_instance.B_cos_theta_L, AFit_instance.B_phi)
    data = RooDataSet("data", "My RooDataSet", ch, variables)
    #print(data.sumEntries())
    #angularPdf = RooProdPdf("angularPdf", "angularPdf", RooArgList(poly_theta_K, poly_theta_L, poly_phi))
    prodPdf = RooProdPdf("prodPdf", "prodPdf", RooArgList(B_mass_fit_function, K_mass_fit_function)) #[5, 2, 3, 3, 0]

    ndofs = [5, 2] #number degrees of freedom 

    fit_result = prodPdf.fitTo(data, RooFit.Save())
    fit_result.Print("v")
    # param_values.append(variable.getVal())
    #chi2s.append(fit_result.Chi2()/fit_result.NDF())

    variable_names = ["m_B", "m_Kstar", "B_cos_theta_K", "B_cos_theta_L", "B_phi"]
    for variable, name, ndof in zip(variables, variable_names, ndofs):
        var_Plot =  ROOT.MyPlot(extract_clean_var_name(variable), 250, "./out/", "Internal")
        var_Plot.plotVarAndPull(data, prodPdf, ndof, f"{name}")
    # chi2
    # with open(csv_file_path, "a") as csv_file:
    #      writer = csv.writer(csv_file, delimiter=" ")
    #      writer.writerow(["Variable", "Parameter Value", "Chi2"])
    #      for variable, param_value, chi2 in zip(variable_names, param_values, chi2s):
    #         writer.writerow([variable, param_value, chi2])

if __name__ == "__main__":
    main()
