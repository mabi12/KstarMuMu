import sys
from ROOT import RooRealVar, RooArgList
from ROOT import RooVoigtian

sys.path.append('../../') 
from Naming.varDict import varDict
from Fitting import fitFunction

var_dict = varDict()

m_Kstar = RooRealVar(var_dict["Kstar_mass"],"m_{K*} [MeV]",846,946)

K_mass_fit = {}

v_mean = RooRealVar("v_mean", "Mean", 900, 880, 920)
v_width = RooRealVar("v_width", "width", 50, 10, 100)
v_sigma = RooRealVar("v_sigma", "Sigma", 15, 1, 120)
voigt = RooVoigtian("voigt", "voigt", m_Kstar, v_mean, v_width, v_sigma)

v_width.setVal(50)
v_width.setConstant(True)
K_mass_fit['vfix'] = fitFunction(voigt, RooArgList(v_mean, v_width, v_sigma))