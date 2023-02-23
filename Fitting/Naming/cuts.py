

var = {}
var["tau"] = "BmumuKst_B_tau_invM_PVMinA0" #alternativa constM  
  
def cutsRun1():
    #RUN1
    cuts = {}
    cuts["hadron_pt"] = "BmumuKst_meson0_pT > 500 && BmumuKst_meson1_pT > 500"
    cuts["mu_pt"] = "BmumuKst_muon0_pT > 3500 && BmumuKst_muon1_pT > 3500"
    cuts["mu_eta"] = "BmumuKst_muon0_eta < 2.5 && BmumuKst_muon1_eta < 2.5"
    cuts["meson_eta"] = "BmumuKst_meson0_eta < 2.5 && BmumuKst_meson1_eta < 2.5"
    cuts["Kstar_pt"] = "BmumuKst_diMeson_pT>3000"
    cuts["Kstar_mass"] = "(BmumuKst_kaonPion_mass > 846 && BmumuKst_kaonPion_mass < 946" +" || "+"BmumuKst_pionKaon_mass > 846 && BmumuKst_pionKaon_mass < 946)" 
    cuts["q2"] = "BmumuKst_diMuon_mass*BmumuKst_diMuon_mass>1 && BmumuKst_diMuon_mass*BmumuKst_diMuon_mass<12000000" 
    cuts["B_mass"] = "BmumuKst_B_mass>5150 && BmumuKst_B_mass<5700" 
    cuts["B_tau"] = f"{var['tau']}/{var['tau']}_err>12.5" 
    cuts["diMu_chi"] = "BmumuKst_diMuon_chi2_over_nDoF<10"
    cuts["B_chi"] = "BmumuKst_chi2_over_nDoF<2" 
    # cuts["B_cos_angle"] = "BmumuKst_B_mass>0.999" 
    
    cuts_run1 = ""
    for c in cuts:
        cuts_run1+=cuts[c]
        cuts_run1+=" && "
    cuts_run1 = cuts_run1[:-4]    

    return cuts_run1

cuts_run1 = cutsRun1()
