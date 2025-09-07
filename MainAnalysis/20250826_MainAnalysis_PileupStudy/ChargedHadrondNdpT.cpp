#include <TCanvas.h>
#include <TCut.h>
#include <TFile.h>
#include <TH1F.h>
#include <TF1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TLegend.h>
#include <TTree.h>

#include <iostream>

using namespace std;
#include "CommandLine.h"
#include "runslicer.h"
#include "ProgressBar.h"
#include "parameter.h" // Parameters for the analysis
//#include "utilities.h" // Utility functions for the analysis
#include <map>
#include <vector>
#include <utility> // for std::pair

bool checkError(const Parameters &par) { return false; }

//============================================================//
// Data analyzer class
//============================================================//

class DataAnalyzer {
public:
  TFile *inf, *outf;
  TH1D *hTrkPt, *hTrkPtNoTrk, *hTrkPtNoEvt, *hTrkPtNoPartSpecies, *hTrkEta, *hTrkPtUnweighted, *hTrkEtaUnweighted;
  ChargedHadronRAATreeMessenger *MChargedHadronRAA;
  string title;
  runpartition* _partition; 

  DataAnalyzer(const char *filename, const char *outFilename, const char *mytitle = "")
      : inf(new TFile(filename)),
        MChargedHadronRAA(new ChargedHadronRAATreeMessenger(*inf, string("Tree"), false, false, 2)), title(mytitle),
        outf(new TFile(outFilename, "recreate")), _partition(nullptr) {  // Initialize pointer to nullptr
    outf->cd();
  }

  ~DataAnalyzer() {
    deleteHistograms();
    inf->Close();
    outf->Close();
    delete MChargedHadronRAA;
    delete _partition;

  }

  void analyze(Parameters &par) {
    outf->cd();

    // Create partition with correct syntax
    _partition = new runpartition(par.Species);

    // SPECIES CORRRECTION FACTORS
    TF1 *correctionFactor_ppRefLike = new TF1("correctionFactor_ppRefLike","0.991543 - [0]*exp([1]*x + [2]*x*x)",3.0, 200.0);
    correctionFactor_ppRefLike->SetParameters(-0.0796242, 0.00923081, -0.0409613);
    correctionFactor_ppRefLike->SetParNames("p0", "p1", "p2");

    TF1 *correctionFactor_dNdeta100 = new TF1("correctionFactor_dNdeta100", "0.99796 - [0]*exp([1]*x + [2]*x*x)",3.0, 200.0);
    correctionFactor_dNdeta100->SetParameters(-0.160674, -0.133007, -0.0221863);
    correctionFactor_dNdeta100->SetParNames("p0", "p1", "p2");

    TF1 *correctionFactor_dNdeta40 = new TF1("correctionFactor_dNdeta40", "0.99796 - [0]*exp([1]*x + [2]*x*x)", 3.0, 200.0);
    correctionFactor_dNdeta40->SetParameters(-0.136137, -0.104656, -0.0285181);
    correctionFactor_dNdeta40->SetParNames("p0", "p1", "p2");


    // EVENT WEIGHTS 
    TFile *fileEventWeight = nullptr;
    if(par.Species == "NENE") {
        if(par.EventSelectionOption == 1){fileEventWeight = TFile::Open("../../CommonCode/root/NENERAA_MULT_EFFICIENCY_HIJING_HF10AND.root");}
        if(par.EventSelectionOption == 2){fileEventWeight = TFile::Open("../../CommonCode/root/NENERAA_MULT_EFFICIENCY_HIJING_HF13AND.root");}
        if(par.EventSelectionOption == 3){fileEventWeight = TFile::Open("../../CommonCode/root/NENERAA_MULT_EFFICIENCY_HIJING_HF19AND.root");}
    }
    if(par.Species == "OO") {
        if(par.EventSelectionOption == 1){fileEventWeight = TFile::Open("../../CommonCode/root/OORAA_MULT_EFFICIENCY_HIJING_HF10AND.root");}
        if(par.EventSelectionOption == 2){fileEventWeight = TFile::Open("../../CommonCode/root/OORAA_MULT_EFFICIENCY_HIJING_HF13AND.root");}
        if(par.EventSelectionOption == 3){fileEventWeight = TFile::Open("../../CommonCode/root/OORAA_MULT_EFFICIENCY_HIJING_HF19AND.root");}
    }
    TH1D *histEventWeight = dynamic_cast<TH1D*>(fileEventWeight->Get("pTCorrection"));
    TH1D* hist_EffWeight = dynamic_cast<TH1D*>(fileEventWeight->Get("hEff"));


    /*hTrkPt = new TH1D(Form("hTrkPt%s", title.c_str()), "", nPtBins_log, pTBins_log);
    hTrkPtNoEvt = new TH1D(Form("hTrkPt%sNoEvt", title.c_str()), "", nPtBins_log, pTBins_log);
    hTrkPtNoTrk = new TH1D(Form("hTrkPt%sNoTrk", title.c_str()), "", nPtBins_log, pTBins_log);
    hTrkPtNoPartSpecies = new TH1D(Form("hTrkPt%sNoPartSpecies", title.c_str()), "", nPtBins_log, pTBins_log);
    hTrkEta = new TH1D(Form("hTrkEta%s", title.c_str()), "", 50, -3.0, 3.0);
    hTrkPtUnweighted = new TH1D(Form("hTrkPt%sUnweighted", title.c_str()), "", nPtBins_log, pTBins_log);
    hTrkEtaUnweighted = new TH1D(Form("hTrkEta%sUnweighted", title.c_str()), "", 50, -3.0, 3.0);

    hTrkPt->Sumw2();
    hTrkEta->Sumw2();

    hTrkPtUnweighted->Sumw2();
    hTrkEtaUnweighted->Sumw2();*/

    par.printParameters();
    unsigned long nEntry = MChargedHadronRAA->GetEntries() * par.scaleFactor;
    ProgressBar Bar(cout, nEntry);
    Bar.SetStyle(1);
    cout << nEntry << endl;
    // event loop
    for (unsigned long i = 0; i < nEntry; i++) {
      MChargedHadronRAA->GetEntry(i);
      if (!par.HideProgressBar && i % 1000 == 0) {
        Bar.Update(i);
        Bar.Print();
      }

      //if(MChargedHadronRAA->nVtx != 1){continue;}

      //int Run = MChargedHadronRAA->Run;
      //int lumi = MChargedHadronRAA->Lumi;
      //comment out if you want to use our "forest x golden" json
      //if ( ( (Run==394271 && lumi>=1 && lumi<=106) || (Run==394271 && lumi>=172 && lumi<=306) || (Run==394272 && lumi>=1 && lumi<=357) || (Run==394272 && lumi>=663 && lumi<=1135)) == false ) continue;
		
      // check trigger
      if (par.CollisionType && par.TriggerChoice == 0 && MChargedHadronRAA->HLT_OxyZeroBias_v1 == false)
        continue;
      if (par.CollisionType && par.TriggerChoice == 1 && MChargedHadronRAA->HLT_MinimumBiasHF_OR_BptxAND_v1 == false)
        continue;
      if (!(MChargedHadronRAA->passBaselineEventSelection))
        continue;

      // event selection and weighting
      float evtWeight = 1.0;
      if(par.CollisionType == true){
        if(par.EventSelectionOption == 1){
          if(par.ApplyEventSelection == 1 && !(MChargedHadronRAA->passHFAND_10_Offline)) {continue;}
          else if(par.UseEventWeight == true){evtWeight /=  hist_EffWeight->GetBinContent(hist_EffWeight->FindBin(MChargedHadronRAA->multiplicityEta2p4));} 
        }
        if(par.EventSelectionOption == 2){
          if(par.ApplyEventSelection == 1 && !(MChargedHadronRAA->passHFAND_13_Offline)) {continue;}
          else if(par.UseEventWeight == true){evtWeight /=  hist_EffWeight->GetBinContent(hist_EffWeight->FindBin(MChargedHadronRAA->multiplicityEta2p4));}
        }
        if(par.EventSelectionOption == 3){
          if(par.ApplyEventSelection == 1 && !(MChargedHadronRAA->passHFAND_19_Offline)) {continue;}
          else if(par.UseEventWeight == true){evtWeight /=  hist_EffWeight->GetBinContent(hist_EffWeight->FindBin(MChargedHadronRAA->multiplicityEta2p4));}
        } 

      }

      

      // Fill partition with event-level information
      _partition->fill_event(MChargedHadronRAA, evtWeight);

      // track loop
      for (unsigned long j = 0; j < MChargedHadronRAA->trkPt->size(); j++) {
        // get track selection option
        float trkWeight = 1.0; //assume weight 1, i.e., the track only has non-1 weight if it satisfies the track selection below AND we use track weight

	      if (par.TrackSelectionOption == 1 && MChargedHadronRAA->trkPassChargedHadron_Loose->at(j) == false) continue;
	      if (par.TrackSelectionOption == 2 && MChargedHadronRAA->trkPassChargedHadron_Nominal->at(j) == false) continue;
	      if (par.TrackSelectionOption == 3 && MChargedHadronRAA->trkPassChargedHadron_Tight->at(j) == false) continue;

	      if (par.UseTrackWeight){
          if (par.TrackSelectionOption == 1 ) trkWeight = MChargedHadronRAA->trackingEfficiency_Loose->at(j); 
          if (par.TrackSelectionOption == 2 ) trkWeight = MChargedHadronRAA->trackingEfficiency_Nominal->at(j); 
          if (par.TrackSelectionOption == 3 ) trkWeight = MChargedHadronRAA->trackingEfficiency_Tight->at(j); 
        }

        float partSpeciesWeight = 1.;
        if (par.UseSpeciesWeight && MChargedHadronRAA->trkPt->at(j) < 200.0) {
          if (par.SpeciesCorrectionOption == 1)
            partSpeciesWeight = correctionFactor_ppRefLike->Eval(MChargedHadronRAA->trkPt->at(j));
          else if (par.SpeciesCorrectionOption == 2)
            partSpeciesWeight = correctionFactor_dNdeta40->Eval(MChargedHadronRAA->trkPt->at(j));
          else if (par.SpeciesCorrectionOption == 3)
            partSpeciesWeight = correctionFactor_dNdeta100->Eval(MChargedHadronRAA->trkPt->at(j));
        }

        if(par.UseEventWeight == 1){
          int binEvtWeight = histEventWeight->FindBin(MChargedHadronRAA->trkPt->at(j));;
          evtWeight = histEventWeight->GetBinContent(binEvtWeight); //pT-dependent weight, for the moment keeping central-value since we are dropping the EvtSel variations
        } else {
          evtWeight = 1.0;
        }

        
        // eta hist before applying eta cut
        //hTrkEta->Fill(MChargedHadronRAA->trkEta->at(j), trkWeight * evtWeight * partSpeciesWeight);
        //hTrkEtaUnweighted->Fill(MChargedHadronRAA->trkEta->at(j));
        // apply eta cut (last track selection)
        if (fabs(MChargedHadronRAA->trkEta->at(j)) > 1.0)
          continue;

        // fill dN/dpT
        double pT = MChargedHadronRAA->trkPt->at(j);
        if (par.CollisionType == false)  evtWeight = 1.0; // event weight = 1 for ppRef, placeholder.
        /*hTrkPtNoEvt->Fill(MChargedHadronRAA->trkPt->at(j), evtWeight / pT);
        hTrkPtNoTrk->Fill(MChargedHadronRAA->trkPt->at(j), trkWeight / pT);
        hTrkPtNoPartSpecies->Fill(MChargedHadronRAA->trkPt->at(j), partSpeciesWeight / pT);
        hTrkPt->Fill(MChargedHadronRAA->trkPt->at(j), trkWeight * evtWeight * partSpeciesWeight / pT);
        hTrkPtUnweighted->Fill(MChargedHadronRAA->trkPt->at(j), 1.0 / pT);*/
        // Fill partition with track-level information
        float totalWeight = trkWeight * evtWeight * partSpeciesWeight;
        _partition->fill_track(MChargedHadronRAA, totalWeight, j);

      } // end of track loop
    } // end of event loop

  } // end of analyze

  void writeHistograms(TFile *outf) {
    outf->cd();

    // Save partition histograms if partition exists
    if (_partition) {
      _partition->savehists();
    }
    
    /*hTrkPtNoEvt->Scale(1.0 / (4 * TMath::Pi()));
    hTrkPtNoTrk->Scale(1.0 / (4 * TMath::Pi()));
    hTrkPtNoPartSpecies->Scale(1.0 / (4 * TMath::Pi()));
    hTrkPt->Scale(1.0 / (4 * TMath::Pi()));
    hTrkPtUnweighted->Scale(1.0 / (4 * TMath::Pi()));
	  
    NormalizeByBinWidth(hTrkPtNoEvt);
    NormalizeByBinWidth(hTrkPtNoTrk);
    NormalizeByBinWidth(hTrkPtNoPartSpecies);
    NormalizeByBinWidth(hTrkPt);
    NormalizeByBinWidth(hTrkPtUnweighted);
    NormalizeByBinWidth(hTrkEta);
    NormalizeByBinWidth(hTrkEtaUnweighted);

    smartWrite(hTrkPtNoEvt);
    smartWrite(hTrkPtNoTrk);
    smartWrite(hTrkPtNoPartSpecies);
    smartWrite(hTrkPt);
    smartWrite(hTrkPtUnweighted);
    smartWrite(hTrkEta);
    smartWrite(hTrkEtaUnweighted);*/
  }

private:
  void deleteHistograms() {
    // Clean up partition histograms safely
    if (_partition) {
      _partition->deletehists();
    }
    
    /*delete hTrkPtNoEvt;
    delete hTrkPtNoTrk;
    delete hTrkPtNoPartSpecies;
    delete hTrkPt;
    delete hTrkEta;
    delete hTrkPtUnweighted;
    delete hTrkEtaUnweighted;*/
  }
};

//============================================================//
// Main analysis
//============================================================//
int main(int argc, char *argv[]) {

  // if (printHelpMessage(argc, argv))
  //   return 0;

  CommandLine CL(argc, argv);     
  int TriggerChoice = CL.GetInt("TriggerChoice");  // Flag indication choice of trigger
  float scaleFactor = CL.GetDouble("ScaleFactor"); // Fraction of the total number of events to be processed

  Parameters par(TriggerChoice, scaleFactor);
  par.input = CL.Get("Input", "input.root");         // Input file
  par.output = CL.Get("Output", "output.root");      // Output file
  par.CollisionType = CL.GetBool("CollisionSystem"); // Flag to indicate if the analysis is for Proton-Proton
                                                     // collisions, false for PP, true for OO/NeNe
  par.ApplyEventSelection = CL.GetBool("ApplyEventSelection"); 
  par.UseSpeciesWeight = CL.GetBool("UseSpeciesWeight");
  par.UseTrackWeight = CL.GetBool("UseTrackWeight");
  par.UseEventWeight = CL.GetBool("UseEventWeight");
  par.EventSelectionOption = CL.GetInt("EventSelectionOption"); 
  par.TrackSelectionOption = CL.GetInt("TrackWeightSelection");
  par.SpeciesCorrectionOption = CL.GetInt("SpeciesCorrectionOption");
  par.HideProgressBar = CL.GetBool("HideProgressBar", false);
  par.Species = CL.Get("Species", "OO");

  if (checkError(par))
    return -1;
  std::cout << "Parameters are set" << std::endl;

  // Analyze Data
  DataAnalyzer analyzer(par.input.c_str(), par.output.c_str(), "");
  analyzer.analyze(par);
  analyzer.writeHistograms(analyzer.outf);
  saveParametersToHistograms(par, analyzer.outf);
  cout << "done!" << analyzer.outf->GetName() << endl;

  return 0;
}
