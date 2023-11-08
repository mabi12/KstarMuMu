

var = {}
var["tau"] = "BmumuKst_B_tau_invM_PVMinA0" #alternativa constM  
tag = "_refit"

def cutsRun1():
    #RUN1
    cuts = {}
    cuts["hadron_pt"] = "BmumuKst_meson0_pT > 500 && BmumuKst_meson1_pT > 500"
    cuts["mu_pt"] = "BmumuKst_muon0_pT > 3500 && BmumuKst_muon1_pT > 3500"
    cuts["mus_eta"] = "abs(BmumuKst_muon0_eta) < 2.5 && abs(BmumuKst_muon1_eta) < 2.5"
    cuts["mesons_eta"] = "abs(BmumuKst_meson0_eta) < 2.5 && abs(BmumuKst_meson1_eta) < 2.5"
    cuts["Kstar_pt"] = "BmumuKst_diMeson_pT>3000"
    cuts["q2"] = "BmumuKst_diMuon_mass*BmumuKst_diMuon_mass>1000000 && BmumuKst_diMuon_mass*BmumuKst_diMuon_mass<12000000" 
    cuts["B_tau"] = f"{var['tau']}{tag}/{var['tau']}_err{tag}>12.5" 
    cuts["diMu_chi"] = f"BmumuKst_diMuon_chi2_over_nDoF<10"
    cuts["B_chi"] = f"BmumuKst_chi2_over_nDoF{tag}<2" 
    cuts["B_cos_angle"] = f"BmumuKst_cos_Theta_PVMinA0{tag}>0.999" 
    
    cuts_B_and_Kstar_mass =       "((BmumuKst_kaonPion_mass > 846 && BmumuKst_kaonPion_mass < 946) && (BmumuKst_B_mass>5150 && BmumuKst_B_mass<5700))"
    cuts_Bbar_and_Kstarbar_mass = "((BmumuKst_pionKaon_mass > 846 && BmumuKst_pionKaon_mass < 946) && (BmumuKst_Bbar_mass>5150 && BmumuKst_Bbar_mass<5700))" 
    
    cuts_B_mass_Jpsi_radiative =  f"abs((BmumuKst_B_mass{tag} - 5279.58) - (BmumuKst_diMuon_mass{tag} - 3096.9))>130" 
    cuts_B_mass_Psi2s_radiative = f"abs((BmumuKst_B_mass{tag} - 5279.58) - (BmumuKst_diMuon_mass{tag} - 3686.09))>130" 
    
    cuts_Bbar_mass_Jpsi_radiative =  f"abs((BmumuKst_Bbar_mass{tag} - 5279.58) - (BmumuKst_diMuon_mass{tag} - 3096.9))>130" 
    cuts_Bbar_mass_Psi2s_radiative = f"abs((BmumuKst_Bbar_mass{tag} - 5279.58) - (BmumuKst_diMuon_mass{tag} - 3686.09))>130" 
    
    cuts_B = f"({cuts_B_and_Kstar_mass} && {cuts_B_mass_Jpsi_radiative} && {cuts_B_mass_Psi2s_radiative})" 
    cuts_Bbar = f"({cuts_Bbar_and_Kstarbar_mass} && {cuts_Bbar_mass_Jpsi_radiative} && {cuts_Bbar_mass_Psi2s_radiative})" 
    cuts["B_mass"] = cuts_B  +" || "+ cuts_Bbar
    
    cuts_run1 = ""
    for c in cuts:
        cuts_run1+=cuts[c]
        cuts_run1+=" && "
    cuts_run1 = cuts_run1[:-4]    

    return cuts_run1

cuts_run1 = cutsRun1()


#TODO ranges
def rangesRun1():
    #RUN1
    ranges = {}
    ranges["B_mass"] = (5150,5700)
    
    return ranges

ranges = rangesRun1()


if __name__ == "__main__":
    print(cuts_run1)