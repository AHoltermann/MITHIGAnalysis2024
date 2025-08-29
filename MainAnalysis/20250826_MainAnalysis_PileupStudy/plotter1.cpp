#include "runslicer.h"
#include <iostream>
#include <TCanvas.h>
#include <TFile.h>

int main(){

    // Use the filename constructor instead of TFile pointer
    runpartition partition1("output_NeNe/MergedOutput.root", "NENE");
    runpartition partition2("output_OO/MergedOutput.root", "OO");
    
    std::cout << "NENE partition loaded with " << partition1.PThist.size() << " pT histograms" << std::endl;
    std::cout << "OO partition loaded with " << partition2.PThist.size() << " pT histograms" << std::endl;

    TCanvas*c = new TCanvas();
    c->cd();
    c->SetLogy();
    c->SetLogx();

    vector<int> OOstarts{0,2,6,15,24,26,33,34,43};
    vector<int> NENEstarts{0,3,9,35};


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


    c->SaveAs("output.pdf");
    f->Close();

    return 0;
}