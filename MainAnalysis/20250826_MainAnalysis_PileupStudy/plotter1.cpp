#include "runslicer.h"
#include <iostream>
#include <TLegend.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TStyle.h>
#include <TGraph.h>
#include <TF1.h>
#include <map>
#include <cmath>
#include <TLatex.h>

using namespace std;

void assign_runcolor(string species, runpartition* r){

    vector<int> colors = {kOrange, kBlue, kGreen + 3, kRed, kBlack, kCyan, kMagenta, kSpring, kGray, kViolet, kTeal, kPink};
    vector<int> OOstarts{0,2,6,15,24,26,33,34,43};
    vector<int> NENEstarts{0,3,9,35};

    int color = -1;

    if (species == "NENE") {
        for(int i = 0; i < r->PThist.size(); i++){
            for(int j = 0; j < NENEstarts.size(); j++){
                if(i == NENEstarts[j]){color +=1; break;}
            }
            r->PThist[i]->SetLineColor(colors[color]);
            r->MultHist[i]->SetLineColor(colors[color]);
            r->VZHist[i]->SetLineColor(colors[color]);
            //cout << "Setting color for NENE histogram " << i << " to " << colors[color] << endl;
        }

    } else if (species == "OO") {
        for(int i = 0; i < r->PThist.size(); i++){
            for(int j = 0; j < OOstarts.size(); j++){
                if(i == OOstarts[j]){color +=1; break;}
            }
            r->PThist[i]->SetLineColor(colors[color]);
            r->MultHist[i]->SetLineColor(colors[color]);
            r->VZHist[i]->SetLineColor(colors[color]);
            //cout << "Setting color for OO histogram " << i << " to " << colors[color] << endl;
        }
    }
}

void plot_correlation_allalone(int runno, runpartition* r, string species) {
    TCanvas* c = new TCanvas();
    c->cd();
    // Increase margins slightly
    c->SetLeftMargin(0.12);
    c->SetRightMargin(0.05);
    c->SetTopMargin(0.05);
    c->SetBottomMargin(0.12);
    //c->SetLogx();

    TGraph* main = new TGraph();
    TGraph* all = new TGraph();

    for(int i = 0; i < r->PThist.size(); i++) {
        all->AddPoint(r->LSbins[i][3], r->PThist[i]->Integral(r->PThist[i]->FindBin(0), r->PThist[i]->FindBin(10)));
        if(r->LSbins[i][0] != runno) {continue;}
        main->AddPoint(r->LSbins[i][3], r->PThist[i]->Integral(r->PThist[i]->FindBin(0), r->PThist[i]->FindBin(10)));
    }
    main->SetMarkerColor(kRed);
    main->SetMarkerStyle(20);
    main->SetMarkerSize(1.3);

    all->SetMarkerColor(kGray);
    all->SetMarkerStyle(20);
    all->SetMarkerSize(1.0);

    all->GetXaxis()->SetTitle("Average Pileup");
    all->GetYaxis()->SetTitle("Tracks with pT 3 < 10 GeV/c");

    // Create proper legend labels with run numbers
    char runLabel[50];
    char allOtherLabel[50];
    sprintf(runLabel, "Run %d", runno);
    sprintf(allOtherLabel, "All other %s runs", species.c_str());
    
    const char* runLegendEntry = runLabel;
    const char* allOtherLegendEntry = allOtherLabel;

    TLegend* L = new TLegend(0.6,0.2,0.88,0.45);
    L->AddEntry(all, allOtherLegendEntry, "p");
    L->AddEntry(main, runLegendEntry, "p");

    all->Draw("AP");
    main->Draw("P SAME");
    L->Draw();

    c->SaveAs(("plotting/correlation_run" + to_string(runno) + ".pdf").c_str());
}

void plot_multiplicity_allalone(int runno, runpartition* r, string species, int isratio){

    TCanvas* c = new TCanvas();
    c->cd();
    // Increase margins slightly
    c->SetLeftMargin(0.12);
    c->SetRightMargin(0.05);
    c->SetTopMargin(0.05);
    c->SetBottomMargin(0.12);
    c->SetLogx();
    if(isratio != 1) {
        c->SetLogy();
    }

    if(isratio == 1) {
        // Plot ratios with respect to MultHist[0]
        TH1* reference = (TH1*)r->MultHist[0]->Clone();
        for(int i = 0; i < r->MultHist.size(); i++) {
            TH1* ratio = (TH1*)r->MultHist[i]->Clone();
            ratio->Divide(reference);
            
            if(r->LSbins[i][0] != runno) {
                ratio->SetLineColor(kGray);
            }
            else{
                ratio->SetLineColor(kRed);
            }
            ratio->GetXaxis()->SetTitle("Multiplicity (tracks with pT > 0.4 GeV/c)");
            ratio->GetYaxis()->SetTitle("Ratio to Reference");
            ratio->SetMinimum(0);
            ratio->SetMaximum(2);
            ratio->Draw("HIST SAME");
        }
    } else {
        // Plot normal distributions
        for(int i = 0; i < r->MultHist.size(); i++) {
            if(r->LSbins[i][0] != runno) {
                r->MultHist[i]->SetLineColor(kGray);
            }
            else{
                r->MultHist[i]->SetLineColor(kRed);
            }
            r->MultHist[i]->GetXaxis()->SetTitle("Multiplicity (tracks with pT > 0.4 GeV/c)");
            r->MultHist[i]->GetYaxis()->SetTitle("AU");
            r->MultHist[i]->Draw("HIST SAME");
        }
    }

    c->SaveAs(("plotting/multiplicity_run" + to_string(runno) + ".pdf").c_str());

}

void plot_multiplicity_vspu_allalone(vector<float> metrics, int runno, runpartition* r, string species, string title_text) {
    cout << "start" << endl;
    TCanvas* c = new TCanvas();
    c->cd();
    // Increase margins slightly
    c->SetLeftMargin(0.12);
    c->SetRightMargin(0.05);
    c->SetTopMargin(0.08);
    c->SetBottomMargin(0.12);
    //c->SetLogx();
    //c->SetLogy();

    TGraph* main = new TGraph();
    TGraph* all = new TGraph();

    for(int i = 0; i< metrics.size(); i++) {
        // Check for NaN, infinity, or invalid values before adding points
        if(!std::isnan(metrics[i]) && !std::isinf(metrics[i]) && !std::isnan(r->LSbins[i][3]) && !std::isinf(r->LSbins[i][3])) {
            all->AddPoint(r->LSbins[i][3], metrics[i]);
            if(r->LSbins[i][0] != runno) {continue;}
            main->AddPoint(r->LSbins[i][3], metrics[i]);
        }
    }

    main->SetMarkerColor(kRed);
    main->SetMarkerStyle(20);
    main->SetMarkerSize(1.3);

    all->SetMarkerColor(kGray);
    all->SetMarkerStyle(20);
    all->SetMarkerSize(1.0);

    all->GetXaxis()->SetTitle("Average Pileup");
    all->GetYaxis()->SetTitle("Multiplicity");

    // Create proper legend labels with run numbers
    char runLabel[50];
    char allOtherLabel[50];
    sprintf(runLabel, "Run %d", runno);
    sprintf(allOtherLabel, "All other %s runs", species.c_str());
    
    const char* runLegendEntry = runLabel;
    const char* allOtherLegendEntry = allOtherLabel;

    TLegend* L = new TLegend(0.5,0.2,0.88,0.45);
    L->AddEntry(all, allOtherLegendEntry, "p");
    L->AddEntry(main, runLegendEntry, "p");

    all->Draw("AP");
    main->Draw("P SAME");
    L->Draw();

    // Add TLatex text at the top of the plot
    TLatex* latex = new TLatex();
    latex->SetNDC();
    latex->SetTextAlign(22); // Center alignment
    latex->SetTextSize(0.04);
    latex->DrawLatex(0.5, 0.95, title_text.c_str());

    cout << "end" << endl;
    c->SaveAs(("plotting/multiplicityvspu_run" + to_string(runno) + ".pdf").c_str());
}





int main(){

    gStyle->SetOptStat(0);


    // Use the filename constructor instead of TFile pointer
    runpartition partition1("output_NeNe_1vtx/MergedOutput.root", "NENE");
    runpartition partition2("output_OO/MergedOutput.root", "OO");
    
    std::cout << "NENE partition loaded with " << partition1.PThist.size() << " pT histograms" << std::endl;
    std::cout << "OO partition loaded with " << partition2.PThist.size() << " pT histograms" << std::endl;


    vector<int> OOstarts{0,2,6,15,24,26,33,34,43};
    vector<int> NENEstarts{0,3,9,35};

    

    ///////////////////////////////
    
    
    // ALL PT SPECTRA
    
    
    ///////////////////////////////

    TCanvas*c1 = new TCanvas();
    c1->cd();
    c1->SetLogy();
    c1->SetLogx();

    for(int i = 0; i< partition2.PThist.size(); i++){
        //if( i > 32 || i < 26){continue;}
        partition2.PThist[i]->Draw("SAME");
        if(partition2.PThist[i]->Integral() == 0){
            cout << "Histogram " << partition2.PThist[i]->GetName() << " is empty!" << endl;
        }
    }

    cout << endl;

    for(int i = 0; i< partition1.PThist.size(); i++){
        partition1.PThist[i]->Draw("SAME");
        if(partition1.PThist[i]->Integral() == 0){
            cout << "Histogram " << partition1.PThist[i]->GetName() << " is empty!" << endl;
        }
    }


    c1->SaveAs("plotting/output.pdf");
    
    ///////////////////////////////
    
    
    //  PT SPECTRA BY RUN
    
    
    ///////////////////////////////
    

    vector<int> colors = {kOrange, kBlue, kGreen + 3, kRed, kBlack, kCyan, kMagenta, kSpring, kGray, kViolet, kTeal, kPink, kAzure};
    

    assign_runcolor("OO", &partition2);
    assign_runcolor("NENE", &partition1);

    TCanvas*c2 = new TCanvas();
    c2->cd();
    c2->SetLogy();
    c2->SetLogx();

    for(int i = 0; i< 43; i++){
        partition2.PThist[i]->Draw("HIST SAME");
        partition2.PThist[i]->GetXaxis()->SetTitle("pT (GeV/c)");
    }

    TLegend* LOO2 = new TLegend(0.6,0.6,0.88,0.88);
    for(int i = 0; i < OOstarts.size()-1; i++){
        string entry = Form("Run %d", int(partition2.LSbins[OOstarts[i]][0]));
        LOO2->AddEntry(partition2.PThist[OOstarts[i]], entry.c_str(), "l");
    }

    LOO2->Draw();

    c2->SaveAs("plotting/output_OOruns.pdf");

    // FOR NENE 

    TCanvas*c3 = new TCanvas();
    c3->cd();
    c3->SetLogy();
    c3->SetLogx();

    for(int i = 0; i< 35; i++){
        partition1.PThist[i]->Draw("HIST SAME");
        partition1.PThist[i]->GetXaxis()->SetTitle("pT (GeV/c)");
    }

    TLegend* LNeNe2 = new TLegend(0.6,0.6,0.88,0.88);
    for(int i = 0; i < NENEstarts.size()-1; i++){
        string entry = Form("Run %d", int(partition1.LSbins[NENEstarts[i]][0]));
        LNeNe2->AddEntry(partition1.PThist[NENEstarts[i]], entry.c_str(), "l");
    }

    LNeNe2->Draw();

    c3->SaveAs("plotting/output_NENEruns.pdf");


    ///////////////////////////////
    
    
    //  PT SPECTRA BY RUN -- Ratio 
    
    
    ///////////////////////////////


    TCanvas*c4 = new TCanvas();
    c4->cd();
    c4->SetLogx();

    // Take ratio to first run for OO using clones
    TH1* first_OO = (TH1*)partition2.PThist[OOstarts[0]]->Clone();
    for (int i = 0; i < partition2.PThist.size(); ++i) {
        TH1* clone = (TH1*)partition2.PThist[i]->Clone();
        clone->Divide(first_OO);
        clone->Draw("COLZ SAME");
        if (i == OOstarts[0]){
            clone->SetMinimum(0.5);
            clone->SetMaximum(1.5);
        }
    }
    LOO2->Draw();
    c4->SaveAs("plotting/OOrunsRatio.pdf");

    TCanvas*c5 = new TCanvas();
    c5->cd();
    c5->SetLogx();

    // Take ratio to first run for NENE using clones
    TH1* first_NENE = (TH1*)partition1.PThist[NENEstarts[0]]->Clone();
    for (int i = 0; i < partition1.PThist.size(); ++i) {
        TH1* clone = (TH1*)partition1.PThist[i]->Clone();
        clone->Divide(first_NENE);
        clone->Draw("COLZ SAME");
        if (i == NENEstarts[0]){
            clone->SetMinimum(0.94);
            clone->SetMaximum(1.06);
        }; 
    }
    LNeNe2->Draw();

    c5->SaveAs("plotting/NENErunsRatio.pdf");

    
    ///////////////////////////////
    
    
    //  PT SPECTRA BY RUN -- Correlations
    
    
    ///////////////////////////////

    cout << "hi" << endl;

    int runno = 0;
    for(int i = 0; i < partition2.PThist.size(); i++) {
        if(partition2.LSbins[i][0] != runno) {
            runno = partition2.LSbins[i][0];
            plot_correlation_allalone(runno, &partition2, "OO");
        }
            
    }

    runno = 0;
    for(int i = 0; i < partition1.PThist.size(); i++) {
        if(partition1.LSbins[i][0] != runno) {
            runno = partition1.LSbins[i][0];
            plot_correlation_allalone(runno, &partition1, "NENE");
        }
            
    }

    TGraph* gNeNeAll = new TGraph();
    gNeNeAll->SetName("gNeNeAll");
    gNeNeAll->SetTitle("NENE All Runs");
    gNeNeAll->GetXaxis()->SetTitle("Average Pileup");
    gNeNeAll->GetYaxis()->SetTitle("Tracks with pT 3 < 10 GeV/c");

    for(int i = 0; i < partition1.PThist.size(); i++) {
        gNeNeAll->AddPoint(partition1.LSbins[i][3], partition1.PThist[i]->Integral(partition1.PThist[i]->FindBin(0), partition1.PThist[i]->FindBin(10)));
    }

    // Fit a linear function to the graph
    TF1* linearFit = new TF1("linearFit", "pol1", 0, 100);
    linearFit->SetLineColor(kRed);
    linearFit->SetLineWidth(2);
    
    gNeNeAll->Fit(linearFit, "Q");  // "Q" for quiet fit (less output)
    
    // Create canvas and plot the graph with fit
    TCanvas* cFit = new TCanvas("cFit", "NENE Linear Fit", 800, 600);
    cFit->cd();
    cFit->SetLeftMargin(0.12);
    cFit->SetRightMargin(0.05);
    cFit->SetTopMargin(0.05);
    cFit->SetBottomMargin(0.12);
    
    gNeNeAll->SetMarkerStyle(20);
    gNeNeAll->SetMarkerColor(kBlue);
    gNeNeAll->SetMarkerSize(0.8);
    
    gNeNeAll->Draw("AP");
    linearFit->Draw("SAME");
    
    // Add legend with fit parameters
    TLegend* fitLegend = new TLegend(0.15, 0.70, 0.4, 0.85);
    fitLegend->AddEntry(gNeNeAll, "NENE Data", "p");
    char fitLabel[100];
    sprintf(fitLabel, "Linear Fit: y = %.2f + %.2fx", linearFit->GetParameter(0), linearFit->GetParameter(1));
    fitLegend->AddEntry(linearFit, fitLabel, "l");
    fitLegend->Draw();
    
    cFit->SaveAs("plotting/NENE_linear_fit.pdf");


    ///////////////////////////////
    
    
    //  Multiplicity SPECTRA BY RUN 
    
    
    ///////////////////////////////
    runno = 0;
    for(int i = 0; i < partition2.PThist.size(); i++) {
        if(partition2.LSbins[i][0] != runno) {
            runno = partition2.LSbins[i][0];
            plot_multiplicity_allalone(runno, &partition2, "OO", 0);
        }
            
    }

    runno = 0;
    for(int i = 0; i < partition1.PThist.size(); i++) {
        if(partition1.LSbins[i][0] != runno) {
            runno = partition1.LSbins[i][0];
            plot_multiplicity_allalone(runno, &partition1, "NENE", 1);
        }
            
    }

    ///////////////////////////////
    
    
    //  Multiplicity SPECTRA BY RUN 
    
    
    ///////////////////////////////


    vector<float> nenelowermultfrac;
    vector<float> neneuppermultsfrac;
    vector<float> nenelowermults;
    vector<float> neneuppermults;
    vector<float> nenealltracks;

    for(int i = 0; i < partition1.MultHist.size(); i++) {
        //cout << "Run " << partition1.LSbins[i][0] << ": ";
        nenealltracks.push_back(partition1.MultHist[i]->Integral());
        //cout << "  All Tracks: " << nenealltracks.back() << endl;
        neneuppermults.push_back(partition1.MultHist[i]->Integral(partition1.MultHist[i]->FindBin(200), partition1.MultHist[i]->FindBin(1000)));
        //cout << "  Upper Multiplicity (200-1000): " << neneuppermults.back() << endl;
        nenelowermults.push_back(partition1.MultHist[i]->Integral(partition1.MultHist[i]->FindBin(1), partition1.MultHist[i]->FindBin(100)));
        //cout << "  Lower Multiplicity (1-100): " << nenelowermults.back() << endl;
        nenelowermultfrac.push_back(nenelowermults.back() / nenealltracks.back());
        //cout << "  Lower Multiplicity Fraction (1-100): " << nenelowermultfrac.back() << endl;
        neneuppermultsfrac.push_back(neneuppermults.back() / nenealltracks.back());
        //cout << "  Upper Multiplicity Fraction (200-1000): " << neneuppermultsfrac.back() << endl;
    }

    vector<float> oolowermultfrac;
    vector<float> oouppermultsfrac;
    vector<float> oolowermults;
    vector<float> oouppermults;
    vector<float> ooalltracks;

    for(int i = 0; i < partition2.MultHist.size(); i++) {
        ooalltracks.push_back(partition2.MultHist[i]->Integral());
        oouppermults.push_back(partition2.MultHist[i]->Integral(partition2.MultHist[i]->FindBin(200), partition2.MultHist[i]->FindBin(1000)));
        oolowermults.push_back(partition2.MultHist[i]->Integral(partition2.MultHist[i]->FindBin(1), partition2.MultHist[i]->FindBin(100)));
        oolowermultfrac.push_back(oolowermults.back() / ooalltracks.back());
        oouppermultsfrac.push_back(oouppermults.back() / ooalltracks.back());
    }

    runno = 0;
    for(int i = 0; i < partition1.MultHist.size(); i++) {
        if(partition1.LSbins[i][0] != runno) {
            runno = partition1.LSbins[i][0];
            plot_multiplicity_vspu_allalone(nenealltracks, runno, &partition1, "NeNe", " All Events");
        }
    }

    return 0;
}