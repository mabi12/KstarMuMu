import sys
from ROOT import RooRealVar, RooArgList
from ROOT import RooVoigtian

sys.path.append('../../') 
from Naming.varDict import varDict
from Fitting.fitFunction import fitFunction

class KFit(): 
    def __init__(self, K_mass_range: list = [800, 946], K_mass_err_range: list = [0, 50]):
        var_dict = varDict()

        self.m_Kstar = RooRealVar(var_dict["Kstar_mass"],"m_{K*} [MeV]",K_mass_range[0],K_mass_range[1])
        
        self.K_mass_fit = {}

    def define_fit_functions(self):
        
        v_mean = RooRealVar("v_mean", "Mean", 900, 880, 920)
        v_width = RooRealVar("v_width", "width", 50, 10, 100)
        v_sigma = RooRealVar("v_sigma", "Sigma", 15, 1, 120)
        voigt = RooVoigtian("voigt", "voigt", self.m_Kstar, v_mean, v_width, v_sigma)

        v_width.setVal(50)
        v_width.setConstant(True)
        self.K_mass_fit['voig'] = fitFunction(voigt, RooArgList(v_mean, v_width, v_sigma))

    def get_fit_function(self, tag:str):
        mass_fit_func = self.K_mass_fit.get(tag)
        if not mass_fit_func:
            raise ValueError(f"K mass fit function '{tag} not defined'")
        return mass_fit_func.f
    
    def get_ndof(self):
        #TODO
        return None 