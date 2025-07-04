/// 
 
Long64_t HowManyPass(TTree* T, TCut t){
    Long64_t nPass = T->GetEntries(t);
    return nPass;
}

TCut HFONLINE(int threshold, string andor){
    
    string expr = 
        "mMaxL1HFAdcPlus > " +
        std::to_string(threshold) + 
        " " +
        andor +  
        " " +
        "mMaxL1HFAdcMinus > " + 
        std::to_string(threshold);

    const char* cutExpr = expr.c_str();
    TCut ezcut(cutExpr);

    return ezcut;
}

TCut HFOFFLINE(float threshold, string andor){

    string expr = 
        "HFEMaxPlus > " + 
        std::to_string(threshold) + 
        " " + 
        andor + 
        " " +  
        "HFEMaxMinus > " + 
        std::to_string(threshold);

    const char* cutExpr = expr.c_str();
    TCut ezcut(cutExpr);

    return ezcut;
}

TCut TRKPTCUT(float threshold){
    string expr = "leadingPtEta1p0_sel > " + std::to_string(threshold);
    const char* cutExpr = expr.c_str();
    TCut ezcut(cutExpr);
    return ezcut;
}




void EfficiencyCalc(){

    TFile* SDf = TFile::Open("/data00/OOsamples/Skims20250704/skim_HiForest_250514_Starlight_SingleDiffraction_OO_5362GeV_1505PR47944.root");
    TFile* DDf = TFile::Open("/data00/OOsamples/Skims20250704/skim_HiForest_250514_Starlight_DoubleDiffraction_OO_5362GeV_1505PR47944.root");
    TFile* OOf = TFile::Open("/data00/OOsamples/Skims20250704/skim_HiForest_250520_Hijing_MinimumBias_b015_OO_5362GeV_250518.root");

    TTree* SDtree = (TTree*)SDf->Get("Tree");
    TTree* DDtree = (TTree*)DDf->Get("Tree");
    TTree* OOtree = (TTree*)OOf->Get("Tree");   

    float SD_rf = 0.05;
    float DD_rf = 0.10;
    float OO_rf = 0.85;
   
    TCut VZcut("VZ > -15 && VZ < 15");
    TCut PVcut("PVFilter == 1");
    TCut CCcut("ClusterCompatibilityFilter == 1");
    TCut Nvtxcut("nVtx > 0");

    TCut EVENTSEL = VZcut && PVcut && CCcut && Nvtxcut && HFONLINE(14, "||") && HFOFFLINE(14, "||") && TRKPTCUT(0);
    TCut blank("");

    cout << "Calculating efficiencies..." << endl;

    float EfficiencyOO = 
        1.0*HowManyPass(OOtree, EVENTSEL) /
        HowManyPass(OOtree, blank);

    float EfficiencySD = 
        1.0*HowManyPass(SDtree, EVENTSEL) /
        HowManyPass(SDtree, blank);   

    float EfficiencyDD = 
        1.0*HowManyPass(DDtree, EVENTSEL) /
        HowManyPass(DDtree, blank);   

    float contamination = (SD_rf * EfficiencySD + DD_rf * EfficiencyDD)  /  (SD_rf * EfficiencySD + DD_rf * EfficiencyDD + OO_rf * EfficiencyOO);

    cout << "Efficiency for OO: " << EfficiencyOO << endl;
    cout << "Efficiency for SD: " << EfficiencySD << endl;
    cout << "Efficiency for DD: " << EfficiencyDD << endl;     
    cout << "Contamination: " << contamination << endl;

}