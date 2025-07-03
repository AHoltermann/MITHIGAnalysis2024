/// begin aslkdfj;aslkjf;lkj;lkjelja;shgasufandskm
using namespace std;

void Centrality(TTree* T, TCut t, TH1D* h){
     T->Project(h->GetName(), "hiBin", t);
}

void HFEMaxPlus(TTree * T, TCut t, TH1D* h){
     T->Project(h->GetName(), "HFEMaxPlus", t);
}

void HFEMaxMinus(TTree * T, TCut t, TH1D* h){
     T->Project(h->GetName(), "HFEMaxMinus", t);
}

void HFEMax_Maximum(TTree* T, TCut t, TH1D* h) {
    TString expr = Form("TMath::Max(%s,%s)", "HFEMaxPlus", "HFEMaxMinus");
    T->Project(h->GetName(), expr, t);
}

void HFEMax_Minimum(TTree* T, TCut t, TH1D* h) {
    TString expr = Form("TMath::Min(%s,%s)", "HFEMaxPlus", "HFEMaxMinus");
    T->Project(h->GetName(), expr, t);
}

void ADCPlus(TTree* T, TCut t, TH1D* h) {
    T->Project(h->GetName(), "mMaxL1HFAdcPlus", t);
}

void ADCMinus(TTree* T, TCut t, TH1D* h) {
    T->Project(h->GetName(), "mMaxL1HFAdcMinus", t);
}  

void ADC_Maximum(TTree* T, TCut t, TH1D* h) {
    TString expr = Form("TMath::Max(%s,%s)", "mMaxL1HFAdcPlus", "mMaxL1HFAdcMinus");
    T->Project(h->GetName(), expr, t);
}   

void ADC_Minimum(TTree* T, TCut t, TH1D* h) {
    TString expr = Form("TMath::Min(%s,%s)", "mMaxL1HFAdcPlus", "mMaxL1HFAdcMinus");
    T->Project(h->GetName(), expr, t);
}

void HFEOnlineOfflineScatter(TTree* T, TCut t, TH2D* h, bool isAND) { //2D hist of same online and offline
    TString exprX = isAND ? Form("TMath::Max(%s, %s)", "HFEMaxPlus", "HFEMaxMinus") : Form("TMath::Min(%s, %s)", "HFEMaxPlus", "HFEMaxMinus");
    TString exprY = isAND ? Form("TMath::Max(%s, %s)", "mMaxL1HFAdcPlus", "mMaxL1HFAdcMinus") : Form("TMath::Min(%s, %s)", "mMaxL1HFAdcPlus", "mMaxL1HFAdcMinus");
    T->Project(h->GetName(), exprY + ":" + exprX, t);

}

void HFEPlusMinusScatter(TTree* T, TCut t, TH2D* h) {
    TString exprX = "HFEMaxPlus";
    TString exprY = "HFEMaxMinus";
    T->Project(h->GetName(), exprY + ":" + exprX, t);
}

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

TH1D* Divide(TH1D*h1, TH1D*h2, string name){
    TH1D* h3 = (TH1D*)h1->Clone(name.c_str());
    h3->Divide(h2);
    return h3;
}

void histmaker(){

    /////////////////////////////
    /////////////////////////////
    ////                     ////
    ////  INPUT FILES HERE   ////
    ////                     ////
    /////////////////////////////
    /////////////////////////////

    TFile* f = new TFile("/data00/OOsamples/Skims20250629/skim_HiForest_250520_Hijing_MinimumBias_b015_OO_5362GeV_250518.root");
    TTree* T = (TTree*)f->Get("Tree"); 

    /////////////////////////////
    /////////////////////////////
    ////                     ////
    ////  Event Selection    ////
    ////                     ////
    /////////////////////////////
    /////////////////////////////

    TCut VZcut("VZ > -15 && VZ < 15");
    TCut PVcut("PVFilter == 1");
    TCut CCcut("ClusterCompatibilityFilter == 1");
    TCut Nvtxcut("nVtx > 0");
    // use HFONLINE and HFOFFLINE functions for a tcut
    // use TRKPT for track pt cuts

    //EVENT SELECTIONs

    TCut eventsel = VZcut && PVcut && CCcut && Nvtxcut;
    TCut eventsel_pt2 = eventsel && TRKPTCUT(2.0);
    TCut eventsel_pt3 = eventsel && TRKPTCUT(3.0);
    TCut eventsel_pt4 = eventsel && TRKPTCUT(4.0);
    TCut event_online14AND = eventsel && HFONLINE(14, "&&");
    TCut event_online14AND_pt2 = event_online14AND && TRKPTCUT(2.0);
    TCut event_online14AND_pt3 = event_online14AND && TRKPTCUT(3.0);
    TCut event_online14AND_pt4 = event_online14AND && TRKPTCUT(4.0);

    TCut event_online14OR = eventsel && HFONLINE(14, "||");
    TCut event_online14OR_pt2 = event_online14OR && TRKPTCUT(2.0);
    TCut event_online14OR_pt3 = event_online14OR && TRKPTCUT(3.0);
    TCut event_online14OR_pt4 = event_online14OR && TRKPTCUT(4.0);

    TCut event_online16AND = eventsel && HFONLINE(16, "&&");
    TCut event_online16AND_pt2 = event_online16AND && TRKPTCUT(2.0);
    TCut event_online16AND_pt3 = event_online16AND && TRKPTCUT(3.0);
    TCut event_online16AND_pt4 = event_online16AND && TRKPTCUT(4.0);

    TCut event_online16OR = eventsel && HFONLINE(16, "||");
    TCut event_online16OR_pt2 = event_online16OR && TRKPTCUT(2.0);
    TCut event_online16OR_pt3 = event_online16OR && TRKPTCUT(3.0);
    TCut event_online16OR_pt4 = event_online16OR && TRKPTCUT(4.0);


    TCut wp16OR = eventsel && HFONLINE(16, "||") && HFOFFLINE(15,"||");
    TCut wp16AND = eventsel && HFONLINE(16, "&&") && HFOFFLINE(15.5,"&&");
    TCut wp14OR = eventsel && HFONLINE(14, "||") && HFOFFLINE(10.5,"||");
    TCut wp14AND = eventsel && HFONLINE(14, "&&") && HFOFFLINE(12,"&&");

    TCut wp16OR_pt3 = eventsel && HFONLINE(16, "||") && HFOFFLINE(14,"||") && TRKPTCUT(3.0);
    TCut wp16AND_pt3 = eventsel && HFONLINE(16, "&&") && HFOFFLINE(15,"&&") && TRKPTCUT(3.0);
    TCut wp14OR_pt3 = eventsel && HFONLINE(14, "||") && HFOFFLINE(9,"||") && TRKPTCUT(3.0);
    TCut wp14AND_pt3 = eventsel && HFONLINE(14, "&&") && HFOFFLINE(9.5,"&&") && TRKPTCUT(3.0);

    /*TCut pure_online14OR = HFONLINE(14, "||");
    TCut pure_online16OR = HFONLINE(16, "||");
    TCut pure_online14AND = HFONLINE(14, "&&");
    TCut pure_online16AND = HFONLINE(16, "&&");*/

    /////////////////////////////
    /////////////////////////////
    ////                     ////
    ////  Generate Hists     ////
    ////                     ////
    /////////////////////////////
    /////////////////////////////
  
    TH2D *HFEMaxPlusMinusScatter = new TH2D("HFEMaxPlusMinusScatter", "HFEMaxPlus vs HFEMaxMinus", 201, -0.5, 200.5, 201, -0.5, 200.5);
    HFEPlusMinusScatter(T, eventsel, HFEMaxPlusMinusScatter);

    TH2D *HFEMaxOnlineOfflineANDScatter = new TH2D("HFEMaxOnlineOfflineANDscatter", "Minimum online VS offline HF", 401, -0.5, 400.5, 151, -0.5, 150.5);
    HFEOnlineOfflineScatter(T, eventsel, HFEMaxOnlineOfflineANDScatter, true);

    TH2D *HFEMaxOnlineOfflineORScatter = new TH2D("HFEMaxOnlineOfflineORscatter", "Maximum online VS offline HF", 401, -0.5, 400.5, 101, -0.5, 100.5);
    HFEOnlineOfflineScatter(T, eventsel, HFEMaxOnlineOfflineORScatter, false);

    TH1D* minHFEMaxEvent = new TH1D("minHFEMaxeventsel", "HFEMax min with event selection", 101, -0.5, 200.5);
    TH1D *minHFEMaxEventpt2 = new TH1D("minHFEMaxeventsel_pt2", "HFEMax min with event selection and pt > 2", 101, -0.5, 200.5);
    TH1D *minHFEMaxEventpt3 = new TH1D("minHFEMaxeventsel_pt3", "HFEMax min with event selection and pt > 3", 101, -0.5, 200.5);
    TH1D *minHFEMaxEventpt4 = new TH1D("minHFEMaxeventsel_pt4", "HFEMax min with event selection and pt > 4", 101, -0.5, 200.5);
    TH1D* minHFEMaxEventOnline14AND = new TH1D("minHFEMaxevent_online14AND", "HFEMax min with event and online 14 AND selection", 101, -0.5, 200.5);
    TH1D* minHFEMaxEventOnline14ANDpt2 = new TH1D("minHFEMaxevent_online14AND_pt2", "HFEMax min with event and online 14 AND pt > 2 selection", 101, -0.5, 200.5);
    TH1D* minHFEMaxEventOnline14ANDpt3 = new TH1D("minHFEMaxevent_online14AND_pt3", "HFEMax min with event and online 14 AND pt > 3 selection", 101, -0.5, 200.5);
    TH1D* minHFEMaxEventOnline14ANDpt4 = new TH1D("minHFEMaxevent_online14AND_pt4", "HFEMax min with event and online 14 AND pt > 4 selection", 101, -0.5, 200.5);

    HFEMax_Minimum(T, eventsel, minHFEMaxEvent);
    HFEMax_Minimum(T, eventsel_pt2, minHFEMaxEventpt2);
    HFEMax_Minimum(T, eventsel_pt3, minHFEMaxEventpt3);
    HFEMax_Minimum(T, eventsel_pt4, minHFEMaxEventpt4);
    HFEMax_Minimum(T, event_online14AND, minHFEMaxEventOnline14AND);
    HFEMax_Minimum(T, event_online14AND_pt2, minHFEMaxEventOnline14ANDpt2);
    HFEMax_Minimum(T, event_online14AND_pt3, minHFEMaxEventOnline14ANDpt3);
    HFEMax_Minimum(T, event_online14AND_pt4, minHFEMaxEventOnline14ANDpt4);

    TH1D* ratio14AND = Divide(minHFEMaxEventOnline14AND, minHFEMaxEvent, "ratio14AND");
    TH1D* ratio14ANDpt2 = Divide(minHFEMaxEventOnline14ANDpt2, minHFEMaxEventpt2, "ratio14ANDpt2");
    TH1D* ratio14ANDpt3 = Divide(minHFEMaxEventOnline14ANDpt3, minHFEMaxEventpt3, "ratio14ANDpt3");
    TH1D* ratio14ANDpt4 = Divide(minHFEMaxEventOnline14ANDpt4, minHFEMaxEventpt4, "ratio14ANDpt4");

    TH1D *maxHFEMaxEvent = new TH1D("maxHFEMaxeventsel", "HFEMax max with event selection", 101, -0.5, 200.5);
    TH1D *maxHFEMaxEventpt2 = new TH1D("maxHFEMaxeventsel_pt2", "HFEMax max with event selection and pt > 2", 101, -0.5, 200.5);
    TH1D *maxHFEMaxEventpt3 = new TH1D("maxHFEMaxeventsel_pt3", "HFEMax max with event selection and pt > 3", 101, -0.5, 200.5);
    TH1D *maxHFEMaxEventpt4 = new TH1D("maxHFEMaxeventsel_pt4", "HFEMax max with event selection and pt > 4", 101, -0.5, 200.5);
    TH1D *maxHFEMaxEventOnline14OR= new TH1D("maxHFEMaxevent_online14OR", "HFEMax max with event and online 14 OR selection", 101, -0.5, 200.5);
    TH1D *maxHFEMaxEventOnline14ORpt2 = new TH1D("maxHFEMaxevent_online14ORpt2", "HFEMax max with event and online 14 OR  and pt > 2 selection", 101, -0.5, 200.5);
    TH1D *maxHFEMaxEventOnline14ORpt3 = new TH1D("maxHFEMaxevent_online14ORpt3", "HFEMax max with event and online 14 OR  and pt > 3 selection", 101, -0.5, 200.5);
    TH1D *maxHFEMaxEventOnline14ORpt4 = new TH1D("maxHFEMaxevent_online14ORpt4", "HFEMax max with event and online 14 OR  and pt > 4 selection", 101, -0.5, 200.5); 

    HFEMax_Maximum(T, eventsel, maxHFEMaxEvent);
    HFEMax_Maximum(T, eventsel_pt2, maxHFEMaxEventpt2);
    HFEMax_Maximum(T, eventsel_pt3, maxHFEMaxEventpt3);
    HFEMax_Maximum(T, eventsel_pt4, maxHFEMaxEventpt4);
    HFEMax_Maximum(T, event_online14OR, maxHFEMaxEventOnline14OR);
    HFEMax_Maximum(T, event_online14OR_pt2, maxHFEMaxEventOnline14ORpt2);
    HFEMax_Maximum(T, event_online14OR_pt3, maxHFEMaxEventOnline14ORpt3);
    HFEMax_Maximum(T, event_online14OR_pt4, maxHFEMaxEventOnline14ORpt4);

    TH1D* ratio14OR = Divide(maxHFEMaxEventOnline14OR, maxHFEMaxEvent, "ratio14OR");
    TH1D* ratio14ORpt2 = Divide(maxHFEMaxEventOnline14ORpt2, maxHFEMaxEventpt2, "ratio14ORpt2");
    TH1D* ratio14ORpt3 = Divide(maxHFEMaxEventOnline14ORpt3, maxHFEMaxEventpt3, "ratio14ORpt3");
    TH1D* ratio14ORpt4 = Divide(maxHFEMaxEventOnline14ORpt4, maxHFEMaxEventpt4, "ratio14ORpt4");

    TH1D* minHFEMaxEventOnline16AND = new TH1D("minHFEMaxevent_online16AND", "HFEMax min with event and online 16 AND selection", 101, -0.5, 200.5);
    TH1D* minHFEMaxEventOnline16ANDpt2 = new TH1D("minHFEMaxevent_online16AND_pt2", "HFEMax min with event and online 16 AND pt > 2 selection", 101, -0.5, 200.5);
    TH1D* minHFEMaxEventOnline16ANDpt3 = new TH1D("minHFEMaxevent_online16AND_pt3", "HFEMax min with event and online 16 AND pt > 3 selection", 101, -0.5, 200.5);
    TH1D* minHFEMaxEventOnline16ANDpt4 = new TH1D("minHFEMaxevent_online16AND_pt4", "HFEMax min with event and online 16 AND pt > 4 selection", 101, -0.5, 200.5);

    HFEMax_Minimum(T, event_online16AND, minHFEMaxEventOnline16AND);
    HFEMax_Minimum(T, event_online16AND_pt2, minHFEMaxEventOnline16ANDpt2);
    HFEMax_Minimum(T, event_online16AND_pt3, minHFEMaxEventOnline16ANDpt3);
    HFEMax_Minimum(T, event_online16AND_pt4, minHFEMaxEventOnline16ANDpt4);

    TH1D* ratio16AND = Divide(minHFEMaxEventOnline16AND, minHFEMaxEvent, "ratio16AND");
    TH1D* ratio16ANDpt2 = Divide(minHFEMaxEventOnline16ANDpt2, minHFEMaxEventpt2, "ratio16ANDpt2");
    TH1D* ratio16ANDpt3 = Divide(minHFEMaxEventOnline16ANDpt3, minHFEMaxEventpt3, "ratio16ANDpt3");
    TH1D* ratio16ANDpt4 = Divide(minHFEMaxEventOnline16ANDpt4, minHFEMaxEventpt4, "ratio16ANDpt4");

    TH1D* maxHFEMaxEventOnline16OR = new TH1D("maxHFEMaxevent_online16OR", "HFEMax max with event and online 16 OR selection", 101, -0.5, 200.5);
    TH1D* maxHFEMaxEventOnline16ORpt2 = new TH1D("maxHFEMaxevent_online16OR_pt2", "HFEMax max with event and online 16 OR pt > 2 selection", 101, -0.5, 200.5);
    TH1D* maxHFEMaxEventOnline16ORpt3 = new TH1D("maxHFEMaxevent_online16OR_pt3", "HFEMax max with event and online 16 OR pt > 3 selection", 101, -0.5, 200.5);
    TH1D* maxHFEMaxEventOnline16ORpt4 = new TH1D("maxHFEMaxevent_online16OR_pt4", "HFEMax max with event and online 16 OR pt > 4 selection", 101, -0.5, 200.5); 

    HFEMax_Maximum(T, event_online16OR, maxHFEMaxEventOnline16OR);
    HFEMax_Maximum(T, event_online16OR_pt2, maxHFEMaxEventOnline16ORpt2);
    HFEMax_Maximum(T, event_online16OR_pt3, maxHFEMaxEventOnline16ORpt3);
    HFEMax_Maximum(T, event_online16OR_pt4, maxHFEMaxEventOnline16ORpt4);

    TH1D* ratio16OR = Divide(maxHFEMaxEventOnline16OR, maxHFEMaxEvent, "ratio16OR");
    TH1D* ratio16ORpt2 = Divide(maxHFEMaxEventOnline16ORpt2, maxHFEMaxEventpt2, "ratio16ORpt2");
    TH1D* ratio16ORpt3 = Divide(maxHFEMaxEventOnline16ORpt3, maxHFEMaxEventpt3, "ratio16ORpt3");
    TH1D* ratio16ORpt4 = Divide(maxHFEMaxEventOnline16ORpt4, maxHFEMaxEventpt4, "ratio16ORpt4");

    /////////////// Centrality Plots ///////////////////////

    
    TH1D* Cent16OR = new TH1D("Cent16OR", ";Centrality;Events", 100, -0.5, 199.5);
    TH1D* Cent14OR = new TH1D("Cent14OR", ";Centrality;Events", 100, -0.5, 199.5);
    TH1D* Cent16AND = new TH1D("Cent16AND", ";Centrality;Events", 100, -0.5, 199.5);
    TH1D* Cent14AND = new TH1D("Cent14AND", ";Centrality; Events", 100, -0.5, 199.5);

    Centrality(T, event_online16OR, Cent16OR);
    Centrality(T, event_online14OR, Cent14OR);
    Centrality(T, event_online16AND, Cent16AND);   
    Centrality(T, event_online14AND, Cent14AND);

    TH1D* Cent16OR_pt3 = new TH1D("Cent16OR_pt3", ";Centrality;Events", 100, -0.5, 199.5);
    TH1D* Cent14OR_pt3 = new TH1D("Cent14OR_pt3", ";Centrality;Events", 100, -0.5, 199.5);
    TH1D* Cent16AND_pt3 = new TH1D("Cent16AND_pt3", ";Centrality;Events", 100, -0.5, 199.5);
    TH1D* Cent14AND_pt3 = new TH1D("Cent14AND_pt3", ";Centrality;Events", 100, -0.5, 199.5);

    Centrality(T, event_online16OR_pt3, Cent16OR_pt3);
    Centrality(T, event_online14OR_pt3, Cent14OR_pt3);
    Centrality(T, event_online16AND_pt3, Cent16AND_pt3);
    Centrality(T, event_online14AND_pt3, Cent14AND_pt3);    

    TH1D* Cent16OR_wp = new TH1D("Cent16OR_wp", ";Centrality;Events", 100, -0.5, 199.5);
    TH1D* Cent14OR_wp = new TH1D("Cent14OR_wp", ";Centrality;Events", 100, -0.5, 199.5);
    TH1D* Cent16AND_wp = new TH1D("Cent16AND_wp", ";Centrality;Events", 100, -0.5, 199.5);
    TH1D* Cent14AND_wp = new TH1D("Cent14AND_wp", ";Centrality; Events", 100, -0.5, 199.5);

    Centrality(T, wp16OR, Cent16OR_wp);
    Centrality(T, wp14OR, Cent14OR_wp);   
    Centrality(T, wp16AND, Cent16AND_wp);
    Centrality(T, wp14AND, Cent14AND_wp);

    TH1D* Cent16OR_wppt3 = new TH1D("Cent16OR_wppt3", ";Centrality;Events", 100, -0.5, 199.5);
    TH1D* Cent16AND_wppt3 = new TH1D("Cent16AND_wppt3", ";Centrality;Events", 100, -0.5, 199.5);
    TH1D* Cent14OR_wppt3 = new TH1D("Cent14OR_wppt3", ";Centrality;Events", 100, -0.5, 199.5);
    TH1D* Cent14AND_wppt3 = new TH1D("Cent14AND_wppt3", ";Centrality;Events", 100, -0.5, 199.5);

    Centrality(T, wp16OR_pt3, Cent16OR_wppt3);
    Centrality(T, wp16AND_pt3, Cent16AND_wppt3);
    Centrality(T, wp14OR_pt3, Cent14OR_wppt3);
    Centrality(T, wp14AND_pt3, Cent14AND_wppt3);    
    
    TH1D* centratio16OR = Divide(Cent16OR_wp, Cent16OR, "cent_16OR");
    TH1D* centratio14OR = Divide(Cent14OR_wp, Cent14OR, "cent_14OR");
    TH1D* centratio16AND = Divide(Cent16AND_wp, Cent16AND, "cent_16AND");
    TH1D* centratio14AND = Divide(Cent14AND_wp, Cent14AND, "cent_14AND");  

    TH1D* centratio16ORpt3 = Divide(Cent16OR_wppt3, Cent16OR_pt3, "cent_16OR_pt3");
    TH1D* centratio14ORpt3 = Divide(Cent14OR_wppt3, Cent14OR_pt3, "cent_14OR_pt3");
    TH1D* centratio16ANDpt3 = Divide(Cent16AND_wppt3, Cent16AND_pt3, "cent_16AND_pt3");
    TH1D* centratio14ANDpt3 = Divide(Cent14AND_wppt3, Cent14AND_pt3, "cent_14AND_pt3");

    /////////////////////////////
    /////////////////////////////
    ////                     ////
    ////  Save to output     ////
    ////                     ////
    /////////////////////////////
    /////////////////////////////

    TFile* outfile = new TFile("online.root", "RECREATE");

    HFEMaxPlusMinusScatter->Write();
    HFEMaxOnlineOfflineANDScatter->Write();
    HFEMaxOnlineOfflineORScatter->Write();


    minHFEMaxEvent->Write();
    minHFEMaxEventpt2->Write();
    minHFEMaxEventpt3->Write();
    minHFEMaxEventpt4->Write();
    minHFEMaxEventOnline14AND->Write();
    minHFEMaxEventOnline14ANDpt2->Write();
    minHFEMaxEventOnline14ANDpt3->Write();
    minHFEMaxEventOnline14ANDpt4->Write();
    ratio14AND->Write();
    ratio14ANDpt2->Write();
    ratio14ANDpt3->Write();
    ratio14ANDpt4->Write();


    maxHFEMaxEvent->Write();
    maxHFEMaxEventpt2->Write();
    maxHFEMaxEventpt3->Write();
    maxHFEMaxEventpt4->Write();
    maxHFEMaxEventOnline14OR->Write();
    maxHFEMaxEventOnline14ORpt2->Write();
    maxHFEMaxEventOnline14ORpt3->Write();
    maxHFEMaxEventOnline14ORpt4->Write();
    ratio14OR->Write();
    ratio14ORpt2->Write();
    ratio14ORpt3->Write();
    ratio14ORpt4->Write();

    minHFEMaxEventOnline16AND->Write();
    minHFEMaxEventOnline16ANDpt2->Write();
    minHFEMaxEventOnline16ANDpt3->Write();
    minHFEMaxEventOnline16ANDpt4->Write();
    ratio16AND->Write();
    ratio16ANDpt2->Write();
    ratio16ANDpt3->Write();
    ratio16ANDpt4->Write();

    maxHFEMaxEventOnline16OR->Write();
    maxHFEMaxEventOnline16ORpt2->Write();
    maxHFEMaxEventOnline16ORpt3->Write();
    maxHFEMaxEventOnline16ORpt4->Write();
    ratio16OR->Write(); 
    ratio16ORpt2->Write();
    ratio16ORpt3->Write();
    ratio16ORpt4->Write();

    maxHFEMaxEvent->Write();
    maxHFEMaxEventpt2->Write();
    maxHFEMaxEventpt3->Write();
    maxHFEMaxEventpt4->Write();
    maxHFEMaxEventOnline14OR->Write();
    maxHFEMaxEventOnline14ORpt2->Write();
    maxHFEMaxEventOnline14ORpt3->Write();
    maxHFEMaxEventOnline14ORpt4->Write();
    ratio14OR->Write();
    ratio14ORpt2->Write();
    ratio14ORpt3->Write();
    ratio14ORpt4->Write();

    minHFEMaxEventOnline16AND->Write();
    minHFEMaxEventOnline16ANDpt2->Write();
    minHFEMaxEventOnline16ANDpt3->Write();
    minHFEMaxEventOnline16ANDpt4->Write();
    ratio16AND->Write();
    ratio16ANDpt2->Write();
    ratio16ANDpt3->Write();
    ratio16ANDpt4->Write();

    maxHFEMaxEventOnline16OR->Write();
    maxHFEMaxEventOnline16ORpt2->Write();
    maxHFEMaxEventOnline16ORpt3->Write();
    maxHFEMaxEventOnline16ORpt4->Write();
    ratio16OR->Write(); 
    ratio16ORpt2->Write();
    ratio16ORpt3->Write();
    ratio16ORpt4->Write();


    Cent16OR->Write();
    Cent14OR->Write();
    Cent16AND->Write();
    Cent14AND->Write();

    Cent16OR_wp->Write();
    Cent14OR_wp->Write();
    Cent16AND_wp->Write();
    Cent14AND_wp->Write();

    Cent16OR_pt3->Write();
    Cent14OR_pt3->Write();
    Cent16AND_pt3->Write();
    Cent14AND_pt3->Write();

    Cent16OR_wppt3->Write();
    Cent16AND_wppt3->Write();       
    Cent14OR_wppt3->Write();
    Cent14AND_wppt3->Write();

    centratio16OR->Write();
    centratio14OR->Write();
    centratio16AND->Write();
    centratio14AND->Write();

    centratio16ORpt3->Write();
    centratio14ORpt3->Write();
    centratio16ANDpt3->Write();
    centratio14ANDpt3->Write(); 

    outfile->Close();
}


