import sys
from ROOT import RooRealVar, RooArgList
from ROOT import RooCBShape, RooGaussian, RooAddPdf

sys.path.append('../../') 
from Naming.varDict import varDict

var_dict = varDict()

m_B = RooRealVar(var_dict["B_mass"],"m_{B} [MeV]",5150,5700)
m_B_err = RooRealVar(var_dict["B_mass"]+"_err","#sigma_{mB}",0,50)

# Define the Crystal Ball function
mean = RooRealVar("mean", "Mean", 5300, 5250, 5350)

# scale_factor = RooRealVar("scale_factor", "per-event error scale factor", 2.5, 0.1, 10)
# sigma = RooProduct("sigma","sigma", RooArgList(scale_factor, m_B_err))
sigma = RooRealVar("sigma", "Sigma", 15, 10, 120)
alpha = RooRealVar("alpha", "Alpha", 1, 0.1, 5)
n = RooRealVar("n", "n", 1, 0.0001, 2)
cb = RooCBShape("cb", "Crystal Ball", m_B, mean, sigma, alpha, n)

# g_scale_factor = RooRealVar("gauss_scale_factor", "per-event error scale factor", 2.5, 0.1, 10)
# g_sigma = RooProduct("gauss_sigma","sigma", RooArgList(scale_factor, m_B_err))
g_mean = RooRealVar("gauss_mean", "Mean", 5300, 5250, 5350)
g_sigma = RooRealVar("gauss_sigma", "Sigma", 15, 10, 120)
g = RooGaussian("g", "Gauss", m_B, g_mean, g_sigma)

gg_mean = RooRealVar("ggauss_mean", "Mean", 5300, 5250, 5350)
gg_sigma = RooRealVar("ggauss_sigma", "Sigma", 15, 5, 40)
gg = RooGaussian("gG", "Gauss", m_B, g_mean, gg_sigma)

frac = RooRealVar("frac", "frac", 0.5, 0.01, 1)
frac2 = RooRealVar("frac2", "frac", 0.1, 0.001, 0.3)
f = RooAddPdf("cbg","cbg",RooArgList(cb,g),RooArgList(frac),True)
# f = RooAddPdf("cbg","cbg",RooArgList(g,gg),RooArgList(frac),True)
# f = g
# f=cb