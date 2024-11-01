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
c = TCanvas("c", "c", 512,512)
ch.Draw(var_dict['B_mass']+"_err","BmumuKst_isTrueNonresBd==1")
c.SaveAs("./out/Bmass_trueSignal.pdf")
ch.Draw(var_dict['B_mass'],"BmumuKst_isTrueNonresBd==0")
c.SaveAs("./out/Bmass_falseSignal.pdf")


data = RooDataSet("data", "My RooDataSet", ch, RooArgSet(m_B,m_Kstar,m_B_err))

prodPdf = RooProdPdf("prodPdf", "prodPdf", RooArgList(f, voigt))

fit_result = prodPdf.fitTo(data, RooFit.Save())
fit_result.Print("v")


BPlot = ROOT.MyPlot(m_B, 250, "./out/", "Internal")
BPlot.plotVarAndPull(data, prodPdf, 7, "fit_B")

KstarPlot = ROOT.MyPlot(m_Kstar, 250, "./out/", "Internal")
KstarPlot.plotVarAndPull(data, prodPdf, 2,"fit_Ks")


exit()

frame = m_Kstar.frame()
data.plotOn(frame)
voigt.plotOn(frame)
from AtlasStyle import AtlasStyle
from AtlasStyle import AtlasLabels
from AtlasStyle import AtlasUtils

SetAtlasStyle()

c = TCanvas("combi","", 600,600)
upper_pad = TPad("upper_pad", "upper_pad", 0, 0.3, 1, 1)
# upper_pad.SetBottomMargin(0.01)
upper_pad.Draw()
upper_pad.cd() 
frame.Draw()

l = 0.058
down = 0.1
up = 0.86
left = 0.2

c.cd()
lower_pad = TPad("lower_pad", "lower_pad", 0, 0, 1, 0.31)
# Set the line color and style
lines = []
for y, col in [(0,kGreen),(-1,kGreen),(1,kGreen),(-3,kYellow),(3,kYellow)]:
    l = TLine(5150, y, 5700,y)
    l.SetLineColor(col)
    l.SetLineStyle(kDashed)
    lines.append(l)

lower_pad.SetTopMargin(0.0)
lower_pad.SetBottomMargin(0.3)
lower_pad.Draw()
lower_pad.cd()
# h_empty = TH1F("h","h",10,5150,5700)
# pull = frame.pullHist()
# pull.GetYaxis().SetNdivisions(10,2,0)
# pull.GetYaxis().SetRangeUser(-4.9,4.9)
# pull.GetXaxis().SetRangeUser(5150,5700)
# h_empty.GetYaxis().SetRangeUser(-4.9,4.9)
# h_empty.Draw()
# pull.Draw("PE")
# for l in lines: l.Draw("same")
# pull.Draw("same PE")

# for lab in ["Internal", "Preliminary", ""]:
# upper_pad.cd()
# frame.Draw()

# ATLASLabel(left, down+2*l, lab)
# myText(left,down+l,1,"#sqrt{s} = 13.6 TeV, 30.7 fb^{-1}")
# myText(left,down,2,"#sqrt{s} = 13.0 TeV, 58.8 fb^{-1}")

# myText(0.6,up,1,"B_{d} #rightarrow J/#psi(#mu#mu) K*(K#pi)")
# myMarkerText( 0.75, up-l, 1, 20, "Data 2022",1.3);
# myMarkerText( 0.75, up-2*l, 2, 20, "Data 2018",1.3);

# lower_pad.cd()
# # hdiff.Draw("PE")
# # line.Draw("same")
# # hdiff.Draw("same P")
# myText(0.68, 0.88, 1,"" )

c.Draw()
c.SaveAs(f"out/time_err_vs_B_pT.pdf")
