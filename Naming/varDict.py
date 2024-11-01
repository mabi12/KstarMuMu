def varDict(debugPrint=False):
    """
    return dictMC where 
    """
    tag = "_refit"
    var_dict = {}
    var_dict["B_mass"] = "BmumuKst_B_mass" + tag
    var_dict["Bb_mass"] = "BmumuKst_Bbar_mass" + tag
    var_dict["Kstar_mass"] = "BmumuKst_kaonPion_mass" + tag
    var_dict["Kstarb_mass"] = "BmumuKst_pionKaon_mass" + tag
    var_dict["ctk"] = "BmumuKst_Bd_decay_angle_cos_thetaK" + tag
    var_dict["ctl"] = "BmumuKst_Bd_decay_angle_cos_thetaL" + tag
    var_dict["phi"] = "BmumuKst_Bd_decay_angle_phi" + tag
    
    if debugPrint:
        print(5*"="+"varDict"+5*"=")
        for v in var_dict:
            print(v,":",var_dict)
        print(5*"="+7*"="+5*"=")
        
    return var_dict
