import sys
from ROOT import RooRealVar, RooArgList
from ROOT import RooPolynomial

sys.path.append('../../') 
from Naming.varDict import varDict
from Fitting.fitFunction import fitFunction

class AFit():
    def __init__(self, ctk_degree: int = 3, ctl_degree: int = 3, phi_degree: int = 1) -> None:
        var_dict = varDict()
        self.B_cos_theta_K = RooRealVar(var_dict["ctk"], "cos(#theta_{K})", -1, 1)
        self.B_cos_theta_L = RooRealVar(var_dict["ctl"], "cos(#theta_{L})", -1, 1)
        self.B_phi = RooRealVar(var_dict["phi"], "#phi", -3.15, 3.15)
        self.A_mass_fit = {}

        self.ctk_degree = ctk_degree
        self.ctl_degree = ctl_degree
        self.phi_degree = phi_degree
    
    def define_fit_functions(self):
        coef_names_ctk = [f"coef_theta_K_{number}" for number in range(1, self.ctk_degree+1)]
        coef_names_ctl = [f"coef_theta_L_{number}" for number in range(1, self.ctl_degree+1)]
        coef_names_phi = [f"coef_phi_{number}" for number in range(1, self.phi_degree+1)]

        coefs_list_ctk = [RooRealVar(coef_name, coef_name, 0.1, -1, 1) for coef_name in coef_names_ctk]
        coefs_list_ctl = [RooRealVar(coef_name, coef_name, 0.1, -1, 1) for coef_name in coef_names_ctl]
        coefs_list_phi = [RooRealVar(coef_name, coef_name, 0.1, -3.15, 3.15) for coef_name in coef_names_phi]

        coefList_cos_theta_K = RooArgList(*coefs_list_ctk)
        coefList_cos_theta_L = RooArgList(*coefs_list_ctl)
        coefList_phi = RooArgList(*coefs_list_phi)

        poly_theta_K = RooPolynomial("poly_theta_K", "Polynomial Fit for B_Theta_K", self.B_cos_theta_K, coefList_cos_theta_K)
        self.A_mass_fit['ctk'] = fitFunction(poly_theta_K, coefList_cos_theta_K)

        poly_theta_L = RooPolynomial("poly_theta_L", "Polynomial Fit for B_Theta_L", self.B_cos_theta_L, coefList_cos_theta_L)
        self.A_mass_fit['ctl'] = fitFunction(poly_theta_L, coefList_cos_theta_L)

        poly_phi = RooPolynomial("poly_phi", "Polynomial Fit for B_Phi", self.B_phi, coefList_phi)
        self.A_mass_fit['phi'] = fitFunction(poly_phi, coefList_phi)

    def get_fit_function(self, tag:str):

        if tag.startswith("p"):
            degrees = tag[1:]
            if len(degrees) != 3:
                raise ValueError("The polynomial degrees should be 3 ints after 'p'")
            try:
                self.ctk_degree = int(degrees[0])
                self.ctl_degree = int(degrees[1])
                self.phi_degree = int(degrees[2])
            except ValueError:
                raise ValueError("Degrees should be int type")
            
            self.define_fit_functions()

            poly_theta_K = self.A_mass_fit["ctk"].f
            poly_theta_L = self.A_mass_fit["ctl"].f
            poly_phi = self.A_mass_fit["phi"].f
            return poly_theta_K, poly_theta_L, poly_phi
        else:
            raise ValueError("Unsupported angular fit function tag. Angule fit function needs to start with 'p")
    
    def get_ndof(self, tag: str):

        if tag.startswith("p"):
            degrees = tag[1:]
            ctk_degree = int(degrees[0])
            ctl_degree = int(degrees[1])
            phi_degree = int(degrees[2])
    
        return ctk_degree, ctl_degree, phi_degree
