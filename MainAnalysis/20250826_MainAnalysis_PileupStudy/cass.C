void cass(){


    TFile*f = TFile::Open("/eos/cms/store/group/phys_heavyions/aholterm/OO2025/skim_250805_OO_IonPhysics0_LowPtV2/intermediate_batch_04.root");

    int run = 0;
    
    TTree*t = (TTree*)f->Get("Tree");
    t->SetBranchAddress("Run", &run);
    
    for(int i = 0; i< t->GetEntries(); i++){
        if(i%10000 == 0){cout << i << "/" << t->GetEntries() << endl;}
        t->GetEntry(i);
        if(run == 394209){
            cout << "got one" << endl;
        }
    }

}