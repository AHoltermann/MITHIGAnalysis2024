#ifndef RUNSLICER_H
#define RUNSLICER_H
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <TFile.h>
#include <TKey.h>
#include <TH1D.h>
#include "Messenger.h"
using namespace std;
#include "utilities.h"

void NormalizeByBinWidth(TH1* hist) {
    for (int i = 1; i <= hist->GetNbinsX(); ++i) {
        double binContent = hist->GetBinContent(i);
        double binError   = hist->GetBinError(i);
        double binWidth   = hist->GetBinWidth(i);

        // Normalize
        hist->SetBinContent(i, binContent / binWidth);
        hist->SetBinError(i, binError / binWidth);
    }
}

vector<vector<float>> OORuns = {
    {394153, 40, 100, 0.1422942633, 69976.02142},
    {394153, 101, 169, 0.143184377, 79648.34292},
    {394154, 1, 100, 0.1923415035, 155061.8733},
    {394154, 101, 200, 0.1857535822, 149750.8229},
    {394154, 201, 300, 0.1783096156, 143749.6459},
    {394154, 301, 387, 0.1726308363, 121079.229},
    {394169, 39, 140, 0.2308099907, 189795.8863},
    {394169, 141, 240, 0.2079947941, 167681.2431},
    {394169, 241, 340, 0.1895313155, 152796.3559},
    {394169, 341, 440, 0.1751585071, 141209.2852},
    {394169, 441, 540, 0.1617528136, 130401.8832},
    {394169, 541, 640, 0.1508092941, 121579.4367},
    {394169, 641, 740, 0.1384417038, 111608.9328},
    {394169, 741, 840, 0.1287038531, 103758.4723},
    {394169, 841, 997, 0.1199162304, 151778.2846},
    {394175, 47, 150, 0.2234009346, 187305.4201},
    {394175, 151, 250, 0.2096336263, 169002.4369},
    {394175, 251, 350, 0.2031168212, 163748.7189},
    {394175, 351, 450, 0.1967183184, 158590.3739},
    {394175, 451, 550, 0.1908001301, 153819.2489},
    {394175, 551, 650, 0.1823322486, 146992.6122},
    {394175, 651, 750, 0.1745240597, 140697.8064},
    {394175, 751, 850, 0.1696992586, 136808.1483},
    {394175, 851, 988, 0.1638071309, 182240.0852},
    {394183, 15, 116, 0.1580518306, 129966.5893},
    {394183, 117, 239, 0.1518212989, 150546.2126},
    {394184, 1, 100, 0.1437174141, 115862.1049},
    {394184, 101, 200, 0.1416895093, 114227.2486},
    {394184, 201, 300, 0.1367416927, 110238.4178},
    {394184, 301, 400, 0.1327921927, 107054.41},
    {394184, 401, 500, 0.1279066644, 103115.7947},
    {394184, 501, 600, 0.1248519104, 100653.1132},
    {394184, 601, 667, 0.1120000011, 60495.74778},
    {394189, 1, 72, 0.1116318951, 64796.68886},
    {394204, 38, 140, 0.1889005518, 156856.4822},
    {394204, 141, 240, 0.1847827532, 148968.16},
    {394204, 241, 340, 0.1777618002, 143308.0081},
    {394204, 341, 440, 0.1716081589, 138347.0656},
    {394204, 441, 540, 0.1660775619, 133888.4089},
    {394204, 541, 640, 0.1620032134, 130603.7506},
    {394204, 641, 740, 0.1568248368, 126429.047},
    {394204, 741, 840, 0.1514017811, 122057.0879},
    {394204, 841, 932, 0.1485305746, 110162.9883},
    {394209, 23, 110, 0.2137189112, 151620.4024},
    {394209, 111, 210, 0.2065290446, 166499.5852},
    {394209, 211, 310, 0.2003254732, 161498.39},
    {394209, 311, 410, 0.1936090978, 156083.7825},
    {394209, 411, 510, 0.188389782, 151876.0745},
    {394209, 511, 610, 0.1823289368, 146989.9423},
    {394209, 611, 710, 0.1776011413, 143178.4881},
    {394209, 711, 810, 0.1727915822, 139301.1177},
    {394209, 811, 910, 0.1673838119, 134941.4815},
    {394209, 911, 1010, 0.1637579565, 132018.3894},
    {394209, 1011, 1110, 0.1589211793, 128119.0763},
    {394209, 1110, 1209, 0.1549259761, 124898.2234},
    {394217, 41, 150, 0.1982880328, 175841.4309},
    {394217, 151, 271, 0.1910159278, 186331.797},
    {394217, 667, 750, 0.1661707214, 112529.3502},
    {394217, 751, 838, 0.1599105161, 113446.6607},
};

vector<vector<float>> NeNeRuns = {
    {394270, 1, 30, 0.10786, 20429.35473},
    {394270, 31, 61, 0.13182, 25799.81464},
    {394270, 62, 95, 0.12619, 27088.08768},
    {394271, 1, 30, 0.12914, 24459.59055},
    {394271, 31, 60, 0.12834, 24307.20663},
    {394271, 61, 106, 0.12619, 36646.65145},
    {394271, 172, 230, 0.12191, 45411.08787},
    {394271, 231, 270, 0.12007, 30321.93844},
    {394271, 271, 306, 0.11768, 26746.22485},
    {394272, 1, 30, 0.11759, 22272.42765},
    {394272, 31, 60, 0.11759, 22272.12776},
    {394272, 61, 90, 0.11593, 21957.05414},
    {394272, 91, 120, 0.11460, 21705.89857},
    {394272, 121, 150, 0.11383, 21560.46283},
    {394272, 151, 180, 0.11300, 21402.39136},
    {394272, 181, 210, 0.11221, 21253.4167},
    {394272, 211, 240, 0.11148, 21115.48677},
    {394272, 241, 270, 0.11088, 21000.17935},
    {394272, 271, 330, 0.10927, 41392.46221},
    {394272, 331, 357, 0.10782, 18378.61271},
    {394272, 663, 700, 0.10204, 24481.48483},
    {394272, 701, 730, 0.10176, 19274.60257},
    {394272, 731, 760, 0.10017, 18971.78963},
    {394272, 761, 790, 0.09939, 18824.97189},
    {394272, 791, 820, 0.09993, 18926.35776},
    {394272, 821, 850, 0.09862, 18678.92308},
    {394272, 851, 880, 0.09898, 18746.24347},
    {394272, 881, 910, 0.09804, 18569.71522},
    {394272, 911, 940, 0.09676, 18326.37831},
    {394272, 941, 970, 0.09635, 18248.39929},
    {394272, 971, 1000, 0.09573, 18132.41369},
    {394272, 1001, 1030, 0.09515, 18021.01997},
    {394272, 1031, 1060, 0.09477, 17950.50766},
    {394272, 1061, 1090, 0.09434, 17868.24207},
    {394272, 1091, 1135, 0.09272, 26341.0501},
};

const Int_t nPtBins_log = 68; // Vipul's binning
const Double_t pTBins_log[nPtBins_log + 1] = {
    0.5,  0.6,  0.7,  0.8,  0.9,  1.0,  1.1,  1.2,  1.3,  1.4,   1.5,   1.6,  1.7,  1.8,  1.9, 2.0,  2.2,  2.4,
    2.6,  2.8,  3.0,  3.2,  3.4,  3.6,  3.8,  4.0,  4.4,  4.8,   5.2,   5.6,  6.0,  6.4,  6.8, 7.2,  7.6,  8.0,
    8.5,  9.0,  9.5,  10.0, 11.,  12.,  13.,  14.,  15.,  16.,   17.,   18.,  19.,  20.,  21., 22.6, 24.6, 26.6,
    28.6, 32.6, 36.6, 42.6, 48.6, 54.6, 60.6, 74.0, 86.4, 103.6, 120.8, 140., 165., 250., 400.};

const Int_t nMultBins = 34;
const Double_t multBins[nMultBins + 1] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 
    12, 14, 16, 18, 20, 24, 28, 32, 36, 
    40, 45, 50, 55, 60, 70, 80, 100,
    120, 150, 200, 250, 300, 400, 1000
};

class runpartition {

public:

    string species;
    vector<vector<float>> LSbins;
    // ARRAY STRUCTURE:
    // 1. RUN
    // 2. LS START
    // 3. LS END
    // 4. PU
    // 5. LUMI

    vector<TH1D*> PThist;
    vector<TH1D*> VZHist;
    vector<TH1D*> MultHist;

    runpartition(string species) : species(species) {
        initialize(species); 
    }
    
    // Constructor to load from ROOT file
    runpartition(const char* filename, string species) : species(species) {
        initialize(species);
        loadFromFile(filename);
    }
    
    ~runpartition() {}

    void initialize(string species) {

        if(species == "OO") {
            LSbins = OORuns;
        } else if(species == "NENE") {
            LSbins = NeNeRuns;
        }
        else{
           cout << "  AAAAAAH ERRROR ERRROR" << endl;
        }

        for(int i = 0; i < LSbins.size(); i++) {
            string histname = Form("hPt_%s_Run%d_LS%dto%d", species.c_str(), int(LSbins[i][0]), int(LSbins[i][1]), int(LSbins[i][2]));
            PThist.push_back(new TH1D(histname.c_str(), "", nPtBins_log, pTBins_log));
            histname = Form("hVZ_%s_Run%d_LS%dto%d", species.c_str(), int(LSbins[i][0]), int(LSbins[i][1]), int(LSbins[i][2]));
            VZHist.push_back(new TH1D(histname.c_str(), "", 100, -25, 25));
            histname = Form("hMult_%s_Run%d_LS%dto%d", species.c_str(), int(LSbins[i][0]), int(LSbins[i][1]), int(LSbins[i][2]));
            MultHist.push_back(new TH1D(histname.c_str(), "", 200, 0, 2000));
        }



    }

    void fill_event(ChargedHadronRAATreeMessenger* m, float evtWeight) {
        if(m == nullptr) return;
        
        int run = m->Run;
        int lumi = m->Lumi;

        for(int i = 0; i < LSbins.size(); i++) {
            if(run == LSbins[i][0] && lumi >= LSbins[i][1] && lumi <= LSbins[i][2]) {
                VZHist[i]->Fill(m->VZ, evtWeight);
                MultHist[i]->Fill(m->multiplicityEta2p4, evtWeight);
                //cout << "Event: " << run << " " << lumi << " " << m->VZ << " " << m->multiplicityEta2p4  << " " << evtWeight<< endl;
            }
        }

    }

    void fill_track(ChargedHadronRAATreeMessenger* m, float trackWeight, int index) {
        if(m == nullptr) return;
        
        int run = m->Run;
        int lumi = m->Lumi;

        for(int i = 0; i < LSbins.size(); i++) {
            if(run == LSbins[i][0] && lumi >= LSbins[i][1] && lumi <= LSbins[i][2]) {
                PThist[i]->Fill(m->trkPt->at(index), trackWeight / m->trkPt->at(index));
                //cout << m->trkPt->at(index) << " " << trackWeight / m->trkPt->at(index) << endl;
            }
        }

    }

    void savehists() {

        for(int i = 0; i < LSbins.size(); i++) {
            // Scale by luminosity to get proper cross-section normalization
            PThist[i]->Scale(1000 / (4.0 * TMath::Pi() * LSbins[i][4]));  // Remove luminosity scaling for now
            VZHist[i]->Scale(1000 /  LSbins[i][4]);  // Keep VZ histograms as counts
            MultHist[i]->Scale(1000 /  LSbins[i][4]);  // Keep multiplicity histograms as counts

            NormalizeByBinWidth(PThist[i]);
            NormalizeByBinWidth(VZHist[i]);
            NormalizeByBinWidth(MultHist[i]);
        }

        for(auto hist : PThist) {
            //smartWrite(hist);
            hist->Write();
        }
        for(auto hist : VZHist) {
            //smartWrite(hist);
            hist->Write();  
        }
        for(auto hist : MultHist) {
            //smartWrite(hist);
            hist->Write();
        }
    }

    void deletehists() {
        for(auto hist : PThist) {
            delete hist;
        }
        for(auto hist : VZHist) {
            delete hist;
        }
        for(auto hist : MultHist) {
            delete hist;
        }
    }

    vector<vector<float>> integrate(vector<TH1D*> histograms, float xmin, float xmax) {
        
        vector<vector<float>> results;
        float sum = 0;
        float error = 0;

        for(int i = 0; i < histograms.size(); i++) {
            vector<float> integratedValues = {0,0};
            for (int bin = 1; bin <= histograms[i]->GetNbinsX(); bin++) {
                float binCenter = histograms[i]->GetBinCenter(bin);
                if (binCenter >= xmin && binCenter <= xmax) {
                    integratedValues[0] += histograms[i]->GetBinContent(bin);
                    integratedValues[1] += histograms[i]->GetBinError(bin)*histograms[i]->GetBinError(bin);
                }
            }
            integratedValues[1] = sqrt(integratedValues[1]);
            results.push_back(integratedValues);
        }
        return results;
    }

    void loadFromFile(const char* filename) {

        TFile* file = TFile::Open(filename, "READ");
        
        // Clear existing histograms first
        for(auto hist : PThist) delete hist;
        for(auto hist : VZHist) delete hist;
        for(auto hist : MultHist) delete hist;
        PThist.clear();
        VZHist.clear();
        MultHist.clear();
        
        // Read all histograms for this species
        TIter next(file->GetListOfKeys());
        TKey* key;
        while ((key = (TKey*)next())) {
            TString name = key->GetName();
            if (name.Contains(species.c_str())) {
                TH1D* hist = (TH1D*)file->Get(name);
                if (hist) {
                    if (name.Contains("hPt_")) {
                        PThist.push_back((TH1D*)hist->Clone());
                        PThist.back()->SetDirectory(0);
                    }
                    else if (name.Contains("hVZ_")) {
                        VZHist.push_back((TH1D*)hist->Clone());
                        VZHist.back()->SetDirectory(0);
                    }
                    else if (name.Contains("hMult_")) {
                        MultHist.push_back((TH1D*)hist->Clone());
                        MultHist.back()->SetDirectory(0);
                    }
                }
            }
        }
        
        file->Close();
        cout << "Loaded " << PThist.size() << " pT histograms, " 
             << VZHist.size() << " VZ histograms, " 
             << MultHist.size() << " multiplicity histograms" << endl;
             
        // Print pT histogram names alongside LSbins info
        cout << "\npT Histograms and LSbins info:" << endl;
        cout << "Index | Histogram Name | Run | LS Start | LS End | Pileup | Luminosity" << endl;
        cout << "------|----------------|-----|----------|--------|--------|------------" << endl;
        
        size_t maxSize = max(PThist.size(), LSbins.size());
        for (size_t i = 0; i < maxSize; i++) {
            TString histName = (i < PThist.size()) ? PThist[i]->GetName() : "N/A";
            
            if (i < LSbins.size()) {
                cout << "  [" << i << "] | " << histName 
                     << " | " << int(LSbins[i][0])  // Run
                     << " | " << int(LSbins[i][1])  // LS Start  
                     << " | " << int(LSbins[i][2])  // LS End
                     << " | " << LSbins[i][3]       // Pileup
                     << " | " << LSbins[i][4]       // Luminosity
                     << endl;
            } else {
                cout << "  [" << i << "] | " << histName << " | No LSbins data" << endl;
            }
        }
        cout << endl;
    }
};

#endif // RUNSLICER_H