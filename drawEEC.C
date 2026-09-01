const std::string symbols[5] = {"all","g","lf","c","b"};
const std::string names[5] = {"All","Gluon","Light Flavor","Charm","Bottom"};
const int pTHats [3] = {10, 20, 30}; 
const double pTMin [3] = {20, 30, 40}; 
const double pTMax [3] = {30, 40, 60}; 

void drawEEC(bool pp = true, std::string outDir = ""){ //put the job output file in parentheses
for (int i = 0; i<3; i++) { 
    int pTHat = pTHats[i]; 
TFile *f = new TFile(TString::Format("/data/rke_group/francesca/%s/%s/%s_EECs_pTHat%d.root",(pp ? "pp" : "ep"),outDir.c_str(),(pp ? "pp" : "ep"),pTHat).Data(),"READ"); 
TH1D *pTAll[5];
TH1D *pT_20_30[5]; 
TH1D *pT_30_40[5]; 
TH1D *pT_40_60[5]; 

for(int j=0; j<5; j++) {
    pTAll[j] = (TH1D*)f->Get(TString::Format("EEC_pTAll_%s",symbols[j].c_str()));
    pT_20_30[j] = (TH1D*)f->Get(TString::Format("EEC_pT_20_30_%s",symbols[j].c_str()));
    pT_30_40[j] = (TH1D*)f->Get(TString::Format("EEC_pT_30_40_%s",symbols[j].c_str()));
    pT_40_60[j] = (TH1D*)f->Get(TString::Format("EEC_pT_40_60_%s",symbols[j].c_str()));
}
    

for (int j = 0; j < 5; j++) {
    if (pTAll[j] && pTAll[j]->Integral() > 0) {
        pTAll[j]->Scale(1.0 / pTAll[j]->Integral());
        pTAll[j]->Scale(1.0, "width");
    }

    if (pT_20_30[j] && pT_20_30[j]->Integral() > 0) {
        pT_20_30[j]->Scale(1.0 / pT_20_30[j]->Integral());
        pT_20_30[j]->Scale(1.0, "width");
    }

    if (pT_30_40[j] && pT_30_40[j]->Integral() > 0) {
        pT_30_40[j]->Scale(1.0 / pT_30_40[j]->Integral());
        pT_30_40[j]->Scale(1.0, "width");
    }

    if (pT_40_60[j] && pT_40_60[j]->Integral() > 0) {
        pT_40_60[j]->Scale(1.0 / pT_40_60[j]->Integral());
        pT_40_60[j]->Scale(1.0, "width");
    }
} 

for(int i=0; i<5; i++) { if(!pTAll[i]) std::cout << "Missing: EEC_pTAll_" << symbols[i] << std::endl; 
if(!pT_20_30[i]) std::cout << "Missing: EEC_pT_20_30_" << symbols[i] << std::endl;
if(!pT_30_40[i]) std::cout << "Missing: EEC_pT_30_40_" << symbols[i] << std::endl; 
if(!pT_40_60[i]) std::cout << "Missing: EEC_pT_40_60_" << symbols[i] << std::endl; 
    
} pTAll[0]->SetTitle("All p_{T}^{jet}"); 
pT_20_30[0]->SetTitle("20<p_{T}^{jet}<30 GeV");
pT_30_40[0]->SetTitle("30<p_{T}^{jet}<40 GeV");
pT_40_60[0]->SetTitle("40<p_{T}^{jet}<60 GeV"); 
pTAll[0]->GetXaxis()->SetTitle("#Delta R"); 
pT_20_30[0]->GetXaxis()->SetTitle("#Delta R"); 
pT_30_40[0]->GetXaxis()->SetTitle("#Delta R"); 
pT_40_60[0]->GetXaxis()->SetTitle("#Delta R");
pTAll[0]->GetYaxis()->SetTitle("EEC");
pT_20_30[0]->GetYaxis()->SetTitle("EEC"); 
pT_30_40[0]->GetYaxis()->SetTitle("EEC"); 
pT_40_60[0]->GetYaxis()->SetTitle("EEC"); 
auto legend = new TLegend(0.1,0.7,0.3,0.9);
for(int i=0; i<5; i++){ legend->AddEntry(pTAll[i],names[i].c_str(),"p");
//legend->AddEntry(pT_20_30[i],names[i].c_str(),"p");
//legend->AddEntry(pT_30_40[i],names[i].c_str(),"p");
//legend->AddEntry(pT_40_60[i],names[i].c_str(),"p"); 
    
} TCanvas *c1 = new TCanvas(); 
gStyle->SetOptStat(0);
c1->SetLogy(); 
c1->SetLogx();
pTAll[0]->Draw("P");
for(int i=1; i<5; i++) { pTAll[i]->Draw("PSame");
TPaveText *pt = new TPaveText(0.3,0.8,0.5,0.9, "NDC");
TText *t1 = pt->AddText(TString::Format("%s-Proton", (pp ? "Proton" : "Electron")).Data()); 
t1->SetTextColor(kViolet+6); 
TText *t2 = pt->AddText("PTHat: 20-60 GeV"); 
t2->SetTextColor(kMagenta-7); 
TText *t3 = pt->AddText("PTHatMin: 10 GeV");
t3->SetTextColor(kBlue+1); 
pt->Draw();
} legend->Draw();
c1->SaveAs(TString::Format("/data/rke_group/francesca/%s/%s/Plots/%s_EEC_pTAll_pTHat%d.pdf",(pp ? "pp" : "ep"),outDir.c_str(),(pp ? "pp" : "ep"),pTHat).Data()); 
c1->Clear();
pT_20_30[0]->Draw("P"); 
for(int i=1; i<5; i++) { pT_20_30[i]->Draw("PSame"); 
TPaveText *pt = new TPaveText(0.3,0.8,0.5,0.9, "NDC"); 
TText *t1 = pt->AddText(TString::Format("%s-Proton", (pp ? "Proton" : "Electron")).Data()); 
t1->SetTextColor(kViolet+6); 
TText *t2 = pt->AddText("PTHat: 20-30 GeV"); 
t2->SetTextColor(kMagenta-7);
TText *t3 = pt->AddText("PTHatMin: 10 GeV"); 
t3->SetTextColor(kBlue+1);
pt->Draw();
} legend->Draw();
c1->SaveAs(TString::Format("/data/rke_group/francesca/%s/%s/Plots/%s_EEC_pT_20_30_pTHat%d.pdf",(pp ? "pp" : "ep"),outDir.c_str(),(pp ? "pp" : "ep"),pTHat).Data());
c1->Clear();
pT_30_40[0]->Draw("P"); 
for(int i=1; i<5; i++) { pT_30_40[i]->Draw("PSame");
TPaveText *pt = new TPaveText(0.3,0.8,0.5,0.9, "NDC");
TText *t1 = pt->AddText(TString::Format("%s-Proton", (pp ? "Proton" : "Electron")).Data()); 
t1->SetTextColor(kViolet+6);
TText *t2 = pt->AddText("PTHat: 30-40 GeV");
t2->SetTextColor(kMagenta-7);
TText *t3 = pt->AddText("PTHatMin: 20 GeV");
t3->SetTextColor(kBlue+1); 
pt->Draw(); 
    
} legend->Draw(); 
c1->SaveAs(TString::Format("/data/rke_group/francesca/%s/%s/Plots/%s_EEC_pT_30_40_pTHat%d.pdf",(pp ? "pp" : "ep"),outDir.c_str(),(pp ? "pp" : "ep"),pTHat).Data());
c1->Clear(); 
pT_40_60[0]->Draw("P"); 
for(int i=1; i<5; i++) { pT_40_60[i]->Draw("PSame"); 
TPaveText *pt = new TPaveText(0.3,0.8,0.5,0.9, "NDC");
TText *t1 = pt->AddText(TString::Format("%s-Proton", (pp ? "Proton" : "Electron")).Data()); 
t1->SetTextColor(kViolet+6); 
TText *t2 = pt->AddText("PTHat: 40-60 GeV");
t2->SetTextColor(kMagenta-7); 
TText *t3 = pt->AddText("PTHatMin: 30 GeV");
t3->SetTextColor(kBlue+1); 
pt->Draw();
} legend->Draw(); 
c1->SaveAs(TString::Format("/data/rke_group/francesca/%s/%s/Plots/%s_EEC_pT_40_60_pTHat%d.pdf",(pp ? "pp" : "ep"),outDir.c_str(),(pp ? "pp" : "ep"),pTHat).Data()); 
c1->Clear();
} 
}