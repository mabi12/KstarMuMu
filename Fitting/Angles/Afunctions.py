import sys
from ROOT import RooRealVar, RooArgList
from ROOT import RooPolynomial

sys.path.append('../../') 
from Naming.varDict import varDict

var_dict = varDict()

B_cos_theta_K = RooRealVar(var_dict["B_Theta_K"], "cos(#theta_{K})", -1, 1)
B_cos_theta_L = RooRealVar(var_dict["B_Theta_L"], "cos(#theta_{L})", -1, 1)
B_phi = RooRealVar(var_dict["B_Phi"], "#phi", -3.15, 3.15)

#POLYNOMIAL 
coef_names = ["coef_theta_K_1", "coef_theta_K_2","coef_theta_K_3", "coef_theta_L_1", "coef_theta_L_2", "coef_theta_L_3", "coef_phi_1","coef_phi_2","coef_phi_3"]
coefs_list = [RooRealVar(coef_name, coef_name, 0.1, -1, 1) for coef_name in coef_names]

coefList_cos_theta_K = RooArgList(*coefs_list[:3])
coefList_cos_theta_L = RooArgList(*coefs_list[3:6])
coefList_phi = RooArgList(*coefs_list[6:9])

poly_theta_K = RooPolynomial("poly_theta_K", "Pol3 Fit for B_Theta_K", B_cos_theta_K, coefList_cos_theta_K)
poly_theta_L = RooPolynomial("poly_theta_L", "Pol3 Fit for B_Theta_L", B_cos_theta_L, coefList_cos_theta_L)
poly_phi = RooPolynomial("poly_phi", "Polynomial Fit for B_Phi", B_phi, coefList_phi)

