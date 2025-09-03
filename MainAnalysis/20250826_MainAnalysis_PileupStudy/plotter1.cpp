#include "runslicer.h"
#include <iostream>
#include <TLegend.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TStyle.h>

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

void plot_correlation_allalone(


int main(){

    gStyle->SetOptStat(0);

    // Use the filename constructor instead of TFile pointer
    runpartition partition1("output_NeNe/MergedOutput.root", "NENE");
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


    TGraph* corrOO_All = new TGraph(43);
    TGraph* corrNENE_All = new TGraph(35);

    TGraph* corrOO_spec[8];
    TGraph* corrNENE_spec[3];
    for (int i = 0; i < 8; i++) {
        corrOO_spec[i] = new TGraph();
    }
    for (int i = 0; i < 3; i++) {
        corrNENE_spec[i] = new TGraph();
    }


    int funindex = 0;
    int countup = 0;
    for (int i = 0; i < 43; i++) {
        corrOO_All->SetPoint(i, partition2.PThist[i]->Integral(0,10), partition2.LSbins[i][3]);
        for(int j = 0; j < OOstarts.size(); j++){
            if(i == OOstarts[j]){funindex = j; countup = 0; break;}
        }
        corrOO_spec[funindex]->SetPoint(countup, partition2.PThist[i]->Integral(0,10), partition2.LSbins[i][3]);
        countup+=1;
    }

    for (int i = 0; i < 35; i++) {
        corrNENE_All->SetPoint(i, partition1.PThist[i]->Integral(0,10), partition1.LSbins[i][3]);
        for(int j = 0; j < NENEstarts.size(); j++){
            if(i == NENEstarts[j]){funindex = j; countup = 0; break;}
        }
        corrNENE_spec[funindex]->SetPoint(countup, partition1.PThist[i]->Integral(0,10), partition1.LSbins[i][3]);
        countup+=1;
    }


    

    return 0;
}