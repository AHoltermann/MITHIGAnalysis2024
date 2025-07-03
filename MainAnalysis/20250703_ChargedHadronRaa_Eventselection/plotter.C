using namespace std;

void Plotter1D(vector<TObject*> objects,  // these vectors should all be the same size, with any TF1 objects plotted last
               vector<int> colors, 
               vector<string> labels,
               const char* xAxisTitle,
               const char* yAxisTitle,
               float xMin = 0.0,
               float xMax = 10.0,
               float yMin = 0.0,
               float yMax = 100.0,
               float legendX = 0.7,
               float legendY = 0.7,
               int logy = 0,
               const char* outputFileName = "plotter_output.pdf") {


    TCanvas *c = new TCanvas("c", "Canvas", 800, 600);
    TLegend* legend = new TLegend(legendX, legendY, legendX + 0.18, legendY + 0.18);
    int colorIdx = 0;
    int labelIdx = 0;
    for (size_t i = 0; i < objects.size(); ++i) {
        TH1D *h = dynamic_cast<TH1D*>(objects[i]);
        TF1  *f = dynamic_cast<TF1*>(objects[i]);
        if (h) {
            h->SetLineColor(colors[colorIdx]);
            h->SetTitle(labels[labelIdx].c_str());
            h->GetXaxis()->SetTitle(xAxisTitle);
            h->GetYaxis()->SetTitle(yAxisTitle);
            h->GetXaxis()->SetRangeUser(xMin, xMax);
            h->GetYaxis()->SetRangeUser(yMin, yMax);
            if (logy) c->SetLogy();
            h->Draw(colorIdx == 0 ? "" : "SAME");
            legend->AddEntry(h, labels[labelIdx].c_str(), "l");
            colorIdx++;
            labelIdx++;
        } else if (f) {
            f->SetLineColor(colors[colorIdx]);
            f->SetTitle(labels[labelIdx].c_str());
            if (logy) c->SetLogy();
            f->Draw(colorIdx == 0 ? "" : "SAME");
            legend->AddEntry(f, labels[labelIdx].c_str(), "l");
            colorIdx++;
            labelIdx++;
        }
    }
    legend->Draw();

    c->SaveAs(outputFileName);

}

void Plotter2D(vector<TObject*> objects,  // TH2D objects first, TF1 after and TLines last
               const char* title,
               const char* xAxisTitle,
               const char* yAxisTitle,
               const char* outputFileName = "plotter_output.pdf",
               int logy = 0,
               float xMin = 0.0,
               float xMax = 200.0,
               float yMin = 0.0,
               float yMax = 200.0,
               vector<int> colors = {kRed, kBlue, kGreen+2})
{
    

    TCanvas *c = new TCanvas("c", "Canvas", 800, 600);
    c->SetTickx();
    c->SetTicky();
    int colorIdx = 0;
    int labelIdx = 0;
    for (size_t i = 0; i < objects.size(); ++i) {
        TH2D *h = dynamic_cast<TH2D*>(objects[i]);
        TF1  *f = dynamic_cast<TF1*>(objects[i]);
        TLine *l = dynamic_cast<TLine*>(objects[i]);
        if (h) { 
            h->SetTitle(title);//setting custom coloring for histo could be added
            h->GetXaxis()->SetTitle(xAxisTitle); //could add TLatex to add some labels
            h->GetYaxis()->SetTitle(yAxisTitle);
            h->GetXaxis()->SetRangeUser(xMin, xMax);
            h->GetYaxis()->SetRangeUser(yMin, yMax);
            if (logy) c->SetLogz();
            h->Draw(colorIdx == 0 ? "COLZ" : "COLZ SAME");
        } else if (f) {
            f->SetLineColor(colors[colorIdx]);
            f->Draw("SAME");
            colorIdx++;
            labelIdx++;
        }
        else if (l) {
            l->SetLineColor(colors[colorIdx]);
            l->SetLineStyle(2);
            l->Draw("SAME");
            colorIdx++;
            labelIdx++;

        }
    }

    c->SaveAs(outputFileName);

}



//get and draw percents on ratio plot


//function to draw percent lines on ratio plot
void DrawPercents(TF1 *func, double percent, float range_low, float range_high, Color_t color, TLegend *legend) 
{
    TF1 *f_diff0 = new TF1("f_diff0", Form("func(x) - %f", percent), range_low, range_high);
    double x_cross = f_diff0->GetX(0, range_low, range_high);

    TLine *vline = new TLine(x_cross, 0, x_cross, percent);
    vline->SetLineColor(color);
    vline->SetLineStyle(2);
    vline->Draw("SAME");

    TLine *hline = new TLine(0, percent, x_cross, percent);
    hline->SetLineColor(color);
    hline->SetLineStyle(2);
    hline->Draw("SAME");

    legend->AddEntry(vline, Form("%.0f%% at x = %.2f", percent*100, x_cross), "l");

}

void RatioPlot(TH1D *h,
               const char* title,
               const char* xAxisTitle ,
               const char* yAxisTitle, 
               const char* outputFileName = "plotter_output.pdf", 
               int ptCut = 0,
               //vector<float> init_guess,,
               float xMin = 0.0,
               float xMax = 100.0,
               float yMin = 0.0,
               float yMax = 1.01,
               float legendX = 0.65,
               float legendY = 0.15,

               float fit_low = 6,
               float fit_high = 200.0,                
               vector<int> colors = {kBlack, kRed, kBlue, kGreen+2, kRed},
               vector<float> percents = {0.9, 0.95, 0.99}) {


    TCanvas *c = new TCanvas("c", "Canvas", 800, 600);
    TLegend* legend = new TLegend(legendX, legendY, legendX + 0.25, legendY + 0.25);
    legend->SetTextSize(0.03);
    int colorIdx = 0;
    int labelIdx = 0;
    h->SetLineColor(colors[colorIdx]);
    h->SetTitle(title);
    h->GetXaxis()->SetTitle(xAxisTitle);
    h->GetYaxis()->SetTitle(yAxisTitle);
    h->GetXaxis()->SetRangeUser(xMin, xMax);
    h->GetYaxis()->SetRangeUser(yMin, yMax);
    h->SetMarkerStyle(20);
    h->SetMarkerSize(1);
    h->Draw("P");
    legend->AddEntry(h, "HIJING OO", "p");
    colorIdx++;
    labelIdx++;
    
    TF1 *func = new TF1("func", "[0]*exp([1]*x) + [2]*exp([3]*x) + [4]", fit_low, fit_high);
    func->SetParameters(-9.98, -0.498, 73.32, -1.30, 1.00);
    //func->SetParameters(-0.5, -0.0633, -4.64, -0.469, 0.99);
    h->Fit("func", "0", "", fit_low, fit_high);

    func->SetLineColor(colors[colorIdx]);
    func->SetLineWidth(2);
    func->SetLineStyle(1);
    func->Draw("SAME");
    legend->AddEntry(func, "Fit", "l");
    colorIdx++;
    labelIdx++;

    DrawPercents(func, percents[0], fit_low, fit_high, colors[colorIdx], legend);
    DrawPercents(func, percents[1], fit_low, fit_high, colors[colorIdx + 1], legend);
    DrawPercents(func, percents[2], fit_low, fit_high, colors[colorIdx + 2], legend);
    
    legend->Draw();

    TLatex *tex = new TLatex();
    tex->SetTextSize(0.03);
    tex->SetNDC();
    tex->DrawLatex(0.40, 0.35, "PVFilter");
    tex->DrawLatex(0.40, 0.30, "CCFilter");
    tex->DrawLatex(0.40, 0.25, "|VZ| < 15");
    tex->DrawLatex(0.40, 0.20, "nVtx > 0");
    if (ptCut != 0)
        tex->DrawLatex(0.40, 0.15, Form("track p_{T} > %i GeV", ptCut));
        tex->DrawLatex(0.40, 0.15, Form("track p_{T} > %i GeV/c", ptCut));

    c->SaveAs(outputFileName);

}

void CentralityPlot(TH1D *hon, TH1D *hoff, const char* honcut, const char* hoffcu, float legendx, float legendy, int logy, const char* title){

    TCanvas *c = new TCanvas("c", "Canvas", 800, 600);
    TLegend* legend = new TLegend(legendx, legendy, legendx + 0.18, legendy + 0.18);
    legend->SetTextSize(0.03);

    hon->SetLineColor(kRed);
    //hon->SetTitle("Centrality Distribution with Online Cut");
    hon->GetXaxis()->SetTitle("2x Centrality (%)");
    hon->GetYaxis()->SetTitle("Events");
    hon->SetMinimum(0);
    hon->Draw("HIST");
    legend->AddEntry(hon, honcut, "l");

    hoff->SetLineColor(kBlue);
    hoff->Draw("HIST SAME");
    legend->AddEntry(hoff, hoffcu, "l");

    legend->SetLineWidth(0);

    if (logy) c->SetLogy();

    legend->Draw();
    
    c->SaveAs(title);

}

void CentralityRatio(TH1D* hon, const char* hcut, float legendx, float legendy,int logy, const char* onlinesel, int pTmin, const char* title){
    
    TCanvas *c = new TCanvas("c", "Canvas", 800, 600);
    TLegend* legend = new TLegend(legendx, legendy, legendx + 0.18, legendy + 0.18);
    legend->SetTextSize(0.03);

    hon->SetLineColor(kBlue);
    hon->SetLineWidth(2);
    //hon->SetTitle("Centrality Distribution with Online Cut");
    hon->GetXaxis()->SetTitle("2x Centrality (%)");
    hon->GetYaxis()->SetTitle("Offline HF Filter Efficiency");
    hon->SetMinimum(0);
    hon->Draw("HIST");
    legend->AddEntry(hon, hcut, "l");
    legend->SetLineWidth(0);

    int nbins = hon->GetNbinsX();
    int firstBelow99 = -1;
    for (int i = 1; i <= nbins; ++i) {
        if (hon->GetBinContent(i) < 0.99) {
            firstBelow99 = i;
            break;
        }
    }
    if (firstBelow99 > 0) {
        double xval = hon->GetBinLowEdge(firstBelow99);
        TLine* vline99 = new TLine(xval, hon->GetMinimum(), xval, hon->GetMaximum());
        vline99->SetLineColor(kMagenta+2);
        vline99->SetLineStyle(7);
        vline99->SetLineWidth(2);
        vline99->Draw("SAME");
        legend->AddEntry(vline99, Form("First bin < 99%% at Centrality = %.2f", xval/2.0), "l");
    }

    if (logy) c->SetLogy();

    legend->Draw();

    TLatex* tex = new TLatex();
    tex->SetNDC();
    tex->SetTextSize(0.025);
    tex->DrawLatex(legendx, 0.55, "|V_{z}| < 15 cm");
    tex->DrawLatex(legendx, 0.51, "PVFilter");
    tex->DrawLatex(legendx, 0.47, "CCFilter");
    tex->DrawLatex(legendx, 0.43, ("Min Track pT = " + to_string(pTmin) + " GeV/c").c_str());
    tex->DrawLatex(legendx, 0.39, onlinesel);
    
    c->SaveAs(title);

}

void plotter(){
    cout << "Start" << endl;
    gStyle->SetOptStat(0);
    TFile* f = new TFile("online.root");

    RatioPlot((TH1D*)f->Get("ratio14AND"), "Ratio of events with and without online 14 AND cut",
    "minimum HFEMax [GeV]", "Ratio", "ratio14AND.pdf");
    RatioPlot((TH1D*)f->Get("ratio14ANDpt2"),"Ratio of events with and without online 14 AND, pt > 2 cut",
    "minimum HFEMax [GeV]", "Ratio", "ratio14ANDpt2.pdf", 2);
    RatioPlot((TH1D*)f->Get("ratio14ANDpt3"),"Ratio of events with and without online 14 AND, pt > 3 cut",
    "minimum HFEMax [GeV]", "Ratio", "ratio14ANDpt3.pdf", 3);
    RatioPlot((TH1D*)f->Get("ratio14ANDpt4"),"Ratio of events with and without online 14 AND, pt > 4 cut",
    "minimum HFEMax [GeV]", "Ratio", "ratio14ANDpt4.pdf", 4);

    RatioPlot((TH1D*)f->Get("ratio14OR"),"Ratio of events with and without online 14 OR cut",
    "maximum HFEMax [GeV]", "Ratio", "ratio14OR.pdf");
    RatioPlot((TH1D*)f->Get("ratio14ORpt2"),"Ratio of events with and without online 14 OR, pt > 2 cut",
    "maximum HFEMax [GeV]", "Ratio", "ratio14ORpt2.pdf", 2);
    RatioPlot((TH1D*)f->Get("ratio14ORpt3"),"Ratio of events with and without online 14 OR, pt > 3 cut",
    "maximum HFEMax [GeV]", "Ratio", "ratio14ORpt3.pdf", 3);
    RatioPlot((TH1D*)f->Get("ratio14ORpt4"),"Ratio of events with and without online 14 OR, pt > 4 cut",
    "maximum HFEMax [GeV]", "Ratio", "ratio14ORpt4.pdf", 4);

    RatioPlot((TH1D*)f->Get("ratio16AND"),"Ratio of events with and without online 16 AND cut",
    "minimum HFEMax [GeV]", "Ratio", "ratio16AND.pdf");
    RatioPlot((TH1D*)f->Get("ratio16ANDpt2"),"Ratio of events with and without online 16 AND, pt > 2 cut",
    "minimum HFEMax [GeV]", "Ratio", "ratio16ANDpt2.pdf", 2);
    RatioPlot((TH1D*)f->Get("ratio16ANDpt3"),"Ratio of events with and without online 16 AND, pt > 3 cut",
    "minimum HFEMax [GeV]", "Ratio", "ratio16ANDpt3.pdf", 3);
    RatioPlot((TH1D*)f->Get("ratio16ANDpt4"),"Ratio of events with and without online 16 AND, pt > 4 cut",
    "minimum HFEMax [GeV]", "Ratio", "ratio16ANDpt4.pdf", 4);

    RatioPlot((TH1D*)f->Get("ratio16OR"),"Ratio of events with and without online 16 OR cut",
    "maximum HFEMax [GeV]", "Ratio", "ratio16OR.pdf");
    RatioPlot((TH1D*)f->Get("ratio16ORpt2"),"Ratio of events with and without online 16 OR, pt > 2 cut",
    "maximum HFEMax [GeV]", "Ratio", "ratio16ORpt2.pdf", 2);
    RatioPlot((TH1D*)f->Get("ratio16ORpt3"),"Ratio of events with and without online 16 OR, pt > 3 cut",
    "maximum HFEMax [GeV]", "Ratio", "ratio16ORpt3.pdf", 3);
    RatioPlot((TH1D*)f->Get("ratio16ORpt4"),"Ratio of events with and without online 16 OR, pt > 4 cut",
    "maximum HFEMax [GeV]", "Ratio", "ratio16ORpt4.pdf", 4);

    Plotter2D({f->Get("HFEMaxPlusMinusScatter")},"HFEMaxPlus vs HFEMaxMinus","HFEMaxPlus [GeV]",
    "HFEMaxMinus [GeV]","HFEMaxPlusMinusScatter.pdf", 1);
    Plotter2D({f->Get("HFEMaxOnlineOfflineANDscatter")},"Minimum offline VS online HF",
    "minimum offline HF energy [GeV]",  "minimum online HF signal [Adc]",
    "HFEMaxOnlineOfflineANDscatter.pdf", 1, 0, 400, 0, 150);
    Plotter2D({f->Get("HFEMaxOnlineOfflineORscatter")},"Maximum offline VS online HF",
    "maximum offline HF signal [GeV]",  "maximum online HF energy [Adc]",
    "HFEMaxOnlineOfflineORscatter.pdf",  1, 0, 400, 0, 100);

     Plotter2D({f->Get("HFEMaxOnlineOfflineANDscatter")},"Minimum offline VS online HF",
    "minimum offline HF energy [GeV]", "minimum online HF signal [Adc]",
    "HFEMaxOnlineOfflineANDscatter_zoomin.pdf", 1, 2, 30, 6, 30);
    Plotter2D({f->Get("HFEMaxOnlineOfflineORscatter")},"Maximum offline VS online HF",
    "maximum offline HF signal [GeV]", "maximum online HF energy [Adc]", 
    "HFEMaxOnlineOfflineORscatter_zoomin.pdf",  1, 2, 30, 6, 30);

    TH1D *hCent16OR = (TH1D*)f->Get("Cent16OR");
    TH1D *hCent14OR = (TH1D*)f->Get("Cent14OR");
    TH1D *hCent16AND = (TH1D*)f->Get("Cent16AND");
    TH1D *hCent14AND = (TH1D*)f->Get("Cent14AND");

    TH1D *hCent14OR_pt3 = (TH1D*)f->Get("Cent14OR_pt3");
    TH1D *hCent14AND_pt3 = (TH1D*)f->Get("Cent14AND_pt3");
    TH1D *hCent16OR_pt3 = (TH1D*)f->Get("Cent16OR_pt3");
    TH1D *hCent16AND_pt3 = (TH1D*)f->Get("Cent16AND_pt3");

    TH1D *hCent16OR_wp = (TH1D*)f->Get("Cent16OR_wp");
    TH1D *hCent14OR_wp = (TH1D*)f->Get("Cent14OR_wp");
    TH1D *hCent16AND_wp = (TH1D*)f->Get("Cent16AND_wp");
    TH1D *hCent14AND_wp = (TH1D*)f->Get("Cent14AND_wp");

    TH1D *hCent16OR_pt3_wp = (TH1D*)f->Get("Cent16OR_wppt3");
    TH1D *hCent14OR_pt3_wp = (TH1D*)f->Get("Cent14OR_wppt3");
    TH1D *hCent16AND_pt3_wp = (TH1D*)f->Get("Cent16AND_wppt3");
    TH1D *hCent14AND_pt3_wp = (TH1D*)f->Get("Cent14AND_wppt3");

    TH1D *hCentratio16OR = (TH1D*)f->Get("cent_16OR");
    TH1D *hCentratio14OR = (TH1D*)f->Get("cent_14OR");
    TH1D *hCentratio16AND = (TH1D*)f->Get("cent_16AND");
    TH1D *hCentratio14AND = (TH1D*)f->Get("cent_14AND");

    TH1D *hCentratio16OR_pt3 = (TH1D*)f->Get("cent_16OR_pt3");
    TH1D *hCentratio14OR_pt3 = (TH1D*)f->Get("cent_14OR_pt3");
    TH1D *hCentratio16AND_pt3 = (TH1D*)f->Get("cent_16AND_pt3");
    TH1D *hCentratio14AND_pt3 = (TH1D*)f->Get("cent_14AND_pt3");

    CentralityPlot(hCent16OR, hCent16OR_wp, "Online 16 OR", "Offline working point: 14 OR", 0.2, 0.2, 0, "centplot16OR.pdf");
    CentralityPlot(hCent14OR, hCent14OR_wp, "Online 14 OR", "Offline working point: 16 OR", 0.2, 0.2, 0, "centplot14OR.pdf");
    CentralityPlot(hCent16AND, hCent16AND_wp, "Online 16 AND", "Offline working point: 14 AND", 0.2, 0.2, 0, "centplot16AND.pdf");
    CentralityPlot(hCent14AND, hCent14AND_wp, "Online 14 AND", "Offline working point: 16 AND", 0.2, 0.2, 0, "centplot14AND.pdf");
    
    CentralityRatio(hCentratio16OR, "Offline 14 OR Efficiency", 0.2, 0.2, 0, "Online 16 OR", 0, "centratio16OR.pdf");
    CentralityRatio(hCentratio14OR, "Offline 9 OR Efficiency", 0.2, 0.2, 0, "Online 14 OR", 0, "centratio14OR.pdf");
    CentralityRatio(hCentratio16AND, "Offline 15 AND Efficiency", 0.2, 0.2, 0, "Online 16 AND", 0, "centratio16AND.pdf");
    CentralityRatio(hCentratio14AND, "Offline 9.5 AND Efficiency", 0.2, 0.2, 0, "Online 14 AND", 0, "centratio14AND.pdf");


    CentralityPlot(hCent16OR_pt3, hCent16OR_pt3_wp, "Online 15 OR, pt > 3", "Offline working point: 14 OR, pt > 3", 0.2, 0.2, 0, "centplot16OR_pt3.pdf");
    CentralityPlot(hCent14OR_pt3, hCent14OR_pt3_wp, "Online 10.5 OR, pt > 3", "Offline working point: 16 OR, pt > 3", 0.2, 0.2, 0, "centplot14OR_pt3.pdf");
    CentralityPlot(hCent16AND_pt3, hCent16AND_pt3_wp, "Online 15.5 AND, pt > 3", "Offline working point: 14 AND, pt > 3", 0.2, 0.2, 0, "centplot16AND_pt3.pdf");
    CentralityPlot(hCent14AND_pt3, hCent14AND_pt3_wp, "Online 11 AND, pt > 3", "Offline working point: 16 AND, pt > 3", 0.2, 0.2, 0, "centplot14AND_pt3.pdf");

    CentralityRatio(hCentratio16OR_pt3, "Offline 14 OR Efficiency, pt > 3", 0.2, 0.2, 0, "Online 16 OR, pt > 3", 3, "centratio16OR_pt3.pdf");
    CentralityRatio(hCentratio14OR_pt3, "Offline 9 OR Efficiency, pt > 3", 0.2, 0.2, 0, "Online 14 OR, pt > 3", 3, "centratio14OR_pt3.pdf");
    CentralityRatio(hCentratio16AND_pt3, "Offline 15 AND Efficiency, pt > 3", 0.2, 0.2, 0, "Online 16 AND, pt > 3", 3, "centratio16AND_pt3.pdf");
    CentralityRatio(hCentratio14AND_pt3, "Offline 9.5 AND Efficiency, pt > 3", 0.2, 0.2, 0, "Online 14 AND, pt > 3", 3, "centratio14AND_pt3.pdf");  

    
  cout << "Done!" << endl;
}
