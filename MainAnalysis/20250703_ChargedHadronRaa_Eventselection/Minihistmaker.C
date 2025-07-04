/// begin aslkdfj;aslkjf;lkj;lkjelja;shgasufandskm
#include "plotter.C"
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

void RatioPlot(float wp14OR_data_f, float mc_wp_eff, TH1D* ratio14OR, TF1* func, int ptCut, string filename)
{
    TCanvas *c = new TCanvas("c", "Canvas", 800, 600);
    TLegend* legend = new TLegend(0.45, 0.15, 0.45 + 0.45, 0.15 + 0.25);
    legend->SetTextSize(0.03);
    vector<int> color = {kRed, kBlue, kGreen+2};
    int colorIdx = 0;
    int labelIdx = 0;
    ratio14OR->SetLineColor(color[colorIdx]);
    ratio14OR->SetTitle("Ratio of offline HF max tower energy minimum with event selection and online 14 OR");
    ratio14OR->GetXaxis()->SetTitle("maximum HFEMax [GeV]");
    ratio14OR->GetYaxis()->SetTitle("Ratio");
    ratio14OR->GetXaxis()->SetRangeUser(0, 100);
    ratio14OR->GetYaxis()->SetRangeUser(0, 1.01);
    ratio14OR->SetMarkerStyle(20);
    ratio14OR->SetMarkerSize(1);
    ratio14OR->Draw("P");
    legend->AddEntry(ratio14OR, "HIJING OO", "p");
    colorIdx++;
    labelIdx++;
    

    func->SetLineColor(color[colorIdx]);
    func->SetLineWidth(2);
    func->SetLineStyle(1);
    func->Draw("SAME");
    legend->AddEntry(func, "Fit", "l");
    colorIdx++;
    labelIdx++;

    TLine *vline = new TLine(wp14OR_data_f, 0, wp14OR_data_f, 0.99);
    vline->SetLineColor(color[0]);
    vline->SetLineStyle(2);
    vline->Draw("SAME");

    TLine *hline = new TLine(0, 0.99, wp14OR_data_f, 0.99);
    hline->SetLineColor(color[0]);
    hline->SetLineStyle(2);
    hline->Draw("SAME");

    TLine *vline2 = new TLine(13.5, 0, 13.5, mc_wp_eff);
    vline2->SetLineColor(color[1]);
    vline2->SetLineStyle(2);
    vline2->Draw("SAME");

    TLine *hline2 = new TLine(0, mc_wp_eff, 13.5, mc_wp_eff);
    hline2->SetLineColor(color[1]);
    hline2->SetLineStyle(2);
    hline2->Draw("SAME");

    legend->AddEntry(vline, Form("99%% at x = %.2f", wp14OR_data_f), "l");
    legend->AddEntry(vline2, Form("x = 13.5 GeV, efficiency =  %.0f", mc_wp_eff*100), "l");
    
    legend->Draw();

    TLatex *tex = new TLatex();
    tex->SetTextSize(0.03);
    tex->SetNDC();
    tex->DrawLatex(0.25, 0.35, "PVFilter");
    tex->DrawLatex(0.25, 0.30, "CCFilter");
    tex->DrawLatex(0.25, 0.25, "#left|V_{Z}#right| < 15");
    tex->DrawLatex(0.25, 0.20, "nVtx > 0");
    tex->DrawLatex(0.25, 0.15, Form("track p_{T} > %i GeV/c", ptCut));

    c->SaveAs(filename.c_str());
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

float roundUpToHalfOrWhole(double x) {
    float intPart = std::floor(x);
    float decimal = x - intPart;

    if (decimal == 0.0f) {
        return x; // already a whole number
    } else if (decimal <= 0.5f) {
        return intPart + 0.5f;
    } else {
        return intPart + 1.0f;
    }
}

float GetWorkingPoint(TH1D* hist, TF1 *func, int range_low, int range_high, double percent) //maybe can add bool IsData, to carry out fitting
{
    func->SetParameters(-9.98, -0.498, 73.32, -1.30, 1.00);
    hist->Fit("func", "0", "", range_low, range_high);

    TF1 *f_diff0 = new TF1("f_diff0", Form("func(x) - %f", percent), range_low, range_high);
    double x_cross = f_diff0->GetX(0, range_low, range_high);
    return roundUpToHalfOrWhole(x_cross);
}

void Minihistmaker(){

    /////////////////////////////
    /////////////////////////////
    ////                     ////
    ////  INPUT FILES HERE   ////
    ////                     ////
    /////////////////////////////
    /////////////////////////////

    //Angantyr: /data00/OOsamples/Skims20250704/20250704_HiForest_250520_Pythia_Angantyr_OO_OO_5362GeV_250626.root
    //Hijing: /data00/OOsamples/Skims20250704/skim_HiForest_250520_Hijing_MinimumBias_b015_OO_5362GeV_250518.root
    //spurious: /data00/bakovacs/OOsamples/Skims/20250704_OO_PhysicsIonPhysics0_394075.root
    TFile* f = new TFile("/data00/bakovacs/OOsamples/Skims/20250704_OO_PhysicsIonPhysics0_394075.root"); //spurious data
    TTree* T = (TTree*)f->Get("Tree"); 


    gStyle->SetOptStat(0);
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
    TCut zeroBias("HLT_OxyZeroBias_v1 == 1");
    TCut online14OR("HLT_MinimumBiasHF_OR_BptxAND_v1 == 1");

    //EVENT SELECTIONs

    TCut eventsel("1");//VZcut && PVcut && CCcut && Nvtxcut; //restore!!
    TCut eventsel_pt3 = eventsel && TRKPTCUT(3.0);
    TCut event_zeroBias = eventsel && zeroBias;
    TCut event_zeroBias_pt3 = event_zeroBias && TRKPTCUT(3.0);
    TCut event_online14OR = eventsel && online14OR;
    TCut event_online14OR_pt3 = event_online14OR && TRKPTCUT(3.0);

    TCut wp14OR = event_online14OR && HFOFFLINE(13.5,"||");
    TCut wp14OR_pt3 = wp14OR && TRKPTCUT(3.0);

    TH1D *minHFEMaxEvent = new TH1D("minHFEMaxeventsel", "HFEMax min with event selection data", 101, -0.5, 200.5);
    TH1D *minHFEMaxEventpt3 = new TH1D("minHFEMaxeventsel_pt3", "HFEMax min with event selection and pt > 3 data", 101, -0.5, 200.5);
    TH1D *minHFEMaxEventOnline14OR = new TH1D("minHFEMaxevent_online14OR", "HFEMax min with event and online 14 OR selection data", 101, -0.5, 200.5);
    TH1D *minHFEMaxEventOnline14ORpt3 = new TH1D("minHFEMaxevent_online14OR_pt3", "HFEMax min with event and online 14 OR pt > 3 selection data", 101, -0.5, 200.5);
    HFEMax_Minimum(T, event_zeroBias, minHFEMaxEvent);
    HFEMax_Minimum(T, event_zeroBias_pt3, minHFEMaxEventpt3);
    HFEMax_Minimum(T, event_online14OR, minHFEMaxEventOnline14OR);
    HFEMax_Minimum(T, event_online14OR_pt3, minHFEMaxEventOnline14ORpt3);

    TH1D* ratio14OR = Divide(minHFEMaxEventOnline14OR, minHFEMaxEvent, "ratio14OR");
    TH1D* ratio14ORpt3 = Divide(minHFEMaxEventOnline14ORpt3, minHFEMaxEventpt3, "ratio14ORpt3");

    int fit_low = 6;
    int fit_high = 100;

    TF1 *func = new TF1("func", "[0]*exp([1]*x) + [2]*exp([3]*x) + [4]", fit_low, fit_high);

    float wp14OR_data_f = GetWorkingPoint(ratio14OR, func, fit_low, fit_high, 0.99);
    float wp14OR_data_pt3_f = GetWorkingPoint(ratio14ORpt3, func, fit_low, fit_high, 0.99);

    TCut wp14OR_data = eventsel && online14OR && HFOFFLINE(wp14OR_data_f,"||");
    TCut wp14OR_data_pt3 = eventsel && online14OR && HFOFFLINE(wp14OR_data_pt3_f, "||") && TRKPTCUT(3.0);
    double mc_wp_eff = func->Eval(13.5);

    cout << "Working point for 99% efficiency" << wp14OR_data_f << endl;
    cout << "Efficiency for MC working point (13.5 GeV): " << mc_wp_eff << endl;

    RatioPlot(wp14OR_data_f, mc_wp_eff, ratio14OR, func, 0, "ratio14OR_spurious.pdf");
    RatioPlot(wp14OR_data_f, mc_wp_eff, ratio14ORpt3, func, 3, "ratio14OR_pt3_spurious.pdf");
    

    //add centrality plots here

    TH1D* Cent14OR = new TH1D("Cent14OR", ";Centrality;Events", 100, -0.5, 199.5);
    Centrality(T, event_online14OR, Cent14OR);

    TH1D* Cent14OR_pt3 = new TH1D("Cent14OR_pt3", ";Centrality;Events", 100, -0.5, 199.5);
    Centrality(T, event_online14OR_pt3, Cent14OR_pt3);

    TH1D* Cent14OR_wp = new TH1D("Cent14OR_wp", ";Centrality;Events", 100, -0.5, 199.5);
    Centrality(T, wp14OR, Cent14OR_wp);  

    TH1D* Cent14OR_wp_pt3 = new TH1D("Cent14OR_wp", ";Centrality;Events", 100, -0.5, 199.5);
    Centrality(T, wp14OR_pt3, Cent14OR_wp_pt3);  

    TH1D* Cent14OR_wp_data = new TH1D("Cent14OR_wp_data", ";Centrality;Events", 100, -0.5, 199.5);
    Centrality(T, wp14OR_data, Cent14OR_wp_data);

    TH1D* Cent14OR_wp_pt3_data = new TH1D("Cent14OR_wppt3_data", ";Centrality;Events", 100, -0.5, 199.5);
    Centrality(T, wp14OR_data_pt3, Cent14OR_wp_pt3_data);

    CentralityPlot(Cent14OR, Cent14OR_wp, "Online 14 OR", "Offline working point 13.5 GeV OR", 0.2, 0.2, 0, "centPlot14OR_spurious.pdf", 0, 0.45);
    CentralityPlot(Cent14OR_pt3, Cent14OR_wp_pt3, "Online 14 OR pt > 3", "Offline working point 13.5 GeV OR pt > 3", 0.2, 0.2, 0, "centPlot14OR_pt3_spurious.pdf", 0, 0.45);
    CentralityPlot(Cent14OR, Cent14OR_wp_data, "Online 14 OR data", Form("Offline working point %f GeV OR", wp14OR_data_f), 0.2, 0.2, 0, "centPlot14OR_wp_data_spurious.pdf", 0, 0.45);
    CentralityPlot(Cent14OR_wp_data, Cent14OR_wp_pt3_data, "Online 14 OR data pt > 3", "Offline working point 13.5 GeV OR pt > 3 data", 0.2, 0.2, 0, "centPlot14OR_wp_pt3_data_spurious.pdf", 0, 0.45);

    TH1D* centratio14OR = Divide(Cent14OR_wp, Cent14OR, "cent_14OR");
    TH1D* centratio14OR_data = Divide(Cent14OR_wp_data, Cent14OR, "cent_14OR_pt3");
    TH1D* centratio14ORpt3 = Divide(Cent14OR_wp_pt3, Cent14OR_pt3, "cent_14OR_pt3");
    TH1D* centratio14ORpt3_data = Divide(Cent14OR_wp_pt3_data, Cent14OR_pt3, "cent_14OR_pt3_data");

    CentralityRatio(centratio14OR, Form("Online 14 OR + offline %.1f OR Efficiency", wp14OR_data_f), 0.2, 0.2, 0, "Online 14 OR", 0, "centratio14OR_spurious.pdf");
    CentralityRatio(centratio14ORpt3, Form("Online 14 OR + offline %.1f OR Efficiency", wp14OR_data_pt3_f), 0.15, 0.15, 0, "Online 14 OR, pt > 3", 3, "centratio14OR_pt3_spurious.pdf");
    CentralityRatio(centratio14OR_data, Form("Online 14 OR + offline %.1f OR Efficiency", wp14OR_data_f), 0.2, 0.2, 0, "Online 14 OR data", 0, "centratio14OR_wp_data_spurious.pdf");
    CentralityRatio(centratio14ORpt3_data, Form("Online 14 OR + offline %.1f OR Efficiency", wp14OR_data_pt3_f), 0.15, 0.15, 0, "Online 14 OR data, pt > 3", 3, "centratio14OR_wp_pt3_data_spurious.pdf");
    ///////////////////////////// 
    /////////////////////////////
    ////                     ////
    ////  Generate Hists     ////
    ////                     ////
    /////////////////////////////
    /////////////////////////////

    /////////////// Centrality Plots ///////////////////////

    /////////////////////////////
    /////////////////////////////
    ////                     ////
    ////  Save to output     ////
    ////                     ////
    /////////////////////////////
    /////////////////////////////

    /*TFile* outfile = new TFile("online.root", "RECREATE");

    HFEMaxPlusMinusScatter->Write();
    HFEMaxOnlineOfflineANDScatter->Write();
    HFEMaxOnlineOfflineORScatter->Write();*/


    /*minHFEMaxEvent->Write();
    minHFEMaxEventpt3->Write();


    maxHFEMaxEvent->Write();
    maxHFEMaxEventpt3->Write();
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

    outfile->Close();*/
}


