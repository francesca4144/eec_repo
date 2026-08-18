// main142.cc is a part of the PYTHIA event generator.
// Copyright 2025 Torbjorn Sjostrand.
// PYTHIA is licenced under the GNU GPL v2 or later, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

// Authors: Dag Gillberg <dag.gillberg@cern.ch>

// Keywords: root; jets; event display

// This is a program to use ROOT to visualize different jet algoritms.
// The produced figure is used in the article "50 years of Quantum
// Chromodynamics" in celebration of the 50th anniversary of QCD (EPJC).

#include "Pythia8/Pythia.h"
#include "TCanvas.h"
#include "TString.h"
#include "TFile.h"
#include "TH2D.h"
#include "TEllipse.h"
#include "TMath.h"
#include "TPave.h"
#include "TMarker.h"
#include "TLatex.h"
#include "TRandom3.h"
#include "TStyle.h"
#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"

using namespace fastjet;
using namespace Pythia8;


//==========================================================================

int getPartonIndex(int pid) {

    if(std::abs(pid) == 21) return 0;
    if(std::abs(pid) == 1 || std::abs(pid) == 2 || std::abs(pid) == 3) return 1;
    if(std::abs(pid) == 4) return 2;
    if(std::abs(pid) == 5) return 3;
    
    return -1;
}

int main(int argc, char* argv[]) {

    int jobNumber = -1;
    int pTHat = -1;
    int nEvents = 1000000;
    std::string outDir = "";
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-j" && i + 1 < argc) {
            jobNumber = std::stoi(argv[++i]);
            std::cout << "making job " << jobNumber << std::endl;
        }
        if (arg == "-p" && i + 1 < argc) {
            pTHat = std::stoi(argv[++i]);
            std::cout << "making pTHat " << pTHat << std::endl;
            if(pTHat == 10 || pTHat == 20 || pTHat == 30) continue;
            
            std::cerr << "Passed pTHat of " << pTHat << " is not valid, pass either 10, 20 or 30 and try again" << std::endl;
            return 0;
        }
        if (arg == "-n" && i + 1 < argc) {
            nEvents = std::stoi(argv[++i]);
            std::cout << "Running over " << nEvents << " events" << std::endl;
        }
        if(arg == "-o" && i+1 < argc) {
            outDir = argv[++i];
            std::cout << "Outputting file to directory " << outDir.c_str() << std::endl;
        }
    }
    
    if(pTHat == -1) pTHat = 10;
    
    int nRBins = 34;
    vector<double> dRbins;
    double logMin = log10(0.001);
    double logMax = log10(0.4);
    for(int i=0; i<=nRBins; i++)
    {
        dRbins.push_back(pow(10, logMin + (logMax - logMin) * i / nRBins));
    }
    
    std::string symbols[5] = {"all","g","lf","c","b"};
    std::string names[5] = {"All","Gluon","Light Flavor","Charm","Bottom"};
    int colors[5] = {1, 831, 591, 891, 871};
    int markers[5] = {20, 33, 29, 43, 45};
    
    double pTBins[4] = {20,30,40,60};
    
    TString outName = "FrancescaEECs_singleJob.root";
    if(jobNumber != -1) outName = TString::Format("/data/rke_group/francesca/pp/%s/pp_EECs_%dk_pTHat%d_%d.root",outDir.c_str(),nEvents/1000,pTHat,jobNumber);
    
    TFile *outfile = new TFile(outName,"RECREATE");
    
    TH1D *pTAll[5];
    TH1D *pT_20_30[5];
    TH1D *pT_30_40[5];
    TH1D *pT_40_60[5];
    for(int i=0; i<5; i++) {
        pTAll[i] = new TH1D(TString::Format("EEC_pTAll_%s",symbols[i].c_str()), TString::Format("%s Initiating Parton,All p_{T}^{Jet}; #DeltaR;EEC",names[i].c_str()), nRBins, &dRbins[0]);
        pTAll[i]->SetMarkerStyle(markers[i]);
        pTAll[i]->SetMarkerColor(colors[i]);
        pTAll[i]->SetLineColor(colors[i]);
        pT_20_30[i] = new TH1D(TString::Format("EEC_pT_20_30_%s",symbols[i].c_str()), TString::Format("%s Initiating Parton, 20<p_{T}^{Jet}<30;  #DeltaR;EEC",names[i].c_str()), nRBins, &dRbins[0]);
        pT_20_30[i]->SetMarkerStyle(markers[i]);
        pT_20_30[i]->SetMarkerColor(colors[i]);
        pT_20_30[i]->SetLineColor(colors[i]);
        pT_30_40[i] = new TH1D(TString::Format("EEC_pT_30_40_%s",symbols[i].c_str()), TString::Format("%s Initiating Parton, 30<p_{T}^{Jet}<40; #DeltaR;EEC",names[i].c_str()), nRBins, &dRbins[0]);
        pT_30_40[i]->SetMarkerStyle(markers[i]);
        pT_30_40[i]->SetMarkerColor(colors[i]);
        pT_30_40[i]->SetLineColor(colors[i]);
        pT_40_60[i] = new TH1D(TString::Format("EEC_pT_40_60_%s",symbols[i].c_str()), TString::Format("%s Initiating Parton, 40<p_{T}^{Jet}<60; #DeltaR;EEC",names[i].c_str()), nRBins, &dRbins[0]);
        pT_40_60[i]->SetMarkerStyle(markers[i]);
        pT_40_60[i]->SetMarkerColor(colors[i]);
        pT_40_60[i]->SetLineColor(colors[i]);
    }


    // Generator. Process selection. LHC initialization.
    Pythia pythia;
    pythia.readString("Beams:idA = 2212");
    pythia.readString("Beams:idB = 2212");
    pythia.readString("Beams:eCM = 200.");
    pythia.readString("HardQCD:all = on");
    pythia.readString(TString::Format("PhaseSpace:pTHatMin = %d",pTHat).Data());
    pythia.readString("Beams:frameType = 2");
    pythia.readString("Random:setSeed = on");
    pythia.readString("Random:seed = 67");

    // If Pythia fails to initialize, exit with error.
    if (!pythia.init()) return 1;

    
    // Setup fasjet. Create map with (key, value) = (descriptive text, jetDef).
    JetDefinition jet_def(antikt_algorithm, 0.4);

    int nUnmatchedJet = 0;
    int nMatchedJet = 0;

    auto &event = pythia.event;
    for (int iEvent = 0; iEvent < nEvents; ++iEvent) {
        if (!pythia.next()) continue;

        PseudoJet p1(event[5].px(), event[5].py(), event[5].pz(), event[5].e());
        int pid1 = getPartonIndex(event[5].id());
        PseudoJet p2(event[6].px(), event[6].py(), event[6].pz(), event[6].e());
        int pid2 = getPartonIndex(event[6].id());
        
        std::pair<PseudoJet, int> scatteredPartons[2] = {{p1, pid1}, {p2, pid2}};
        
        

        // Identify particles. Jets are built from all stable particles after
        // hadronization (particle-level jets).
        std::vector<PseudoJet> stbl_ptcls;
        for (int i = 0; i < event.size(); ++i) {
            auto &p = event[i];
            if (not p.isFinal()) continue;
            stbl_ptcls.push_back(PseudoJet(p.px(), p.py(), p.pz(), p.e()));
        }
    
        ClusterSequence cs(stbl_ptcls, jet_def);
        vector<PseudoJet> jets = sorted_by_pt(cs.inclusive_jets(20));

        int jetIndex = -1;

        for(auto jet:jets) {
            //std::cout << "jet pT: " << jet.pt() << "   with " << jet.constituents().size() << " constituents" << std::endl;
            
            jetIndex++;

            std::cout << "event " << iEvent << std::endl;
            std::cout << "   jet " << jetIndex << std::endl;
            std::cout << "   jet pT: " << jet.pt() << "   jet eta: " << jet.eta() << "   phi: " << jet.phi_std() << std::endl;
    
            int partonIndex = -1;      
            for(int p=0; p<2; p++)
            {
                fastjet::PseudoJet part = scatteredPartons[p].first;
                double dPhi = part.phi_std() - jet.phi_std();
                if(dPhi > TMath::Pi()) dPhi -= 2*TMath::Pi();
                if(dPhi < -TMath::Pi()) dPhi += 2*TMath::Pi();
                    
                double dEta = part.eta() - jet.eta();
                double dR = sqrt(dPhi*dPhi + dEta*dEta);

                std::cout << "      parton " << p << " pT: " << part.pt() << "   eta: " << part.eta() << "   phi: " << part.phi_std() << std::endl;
                std::cout << "         dEta: " << dEta << "   dPhi: " << dPhi << "   dR: " << dR << std::endl;
                if(dR > 0.3) continue;

                partonIndex = scatteredPartons[p].second;
                std::cout << "         jet matches to initiating parton " << p << " with parton index " << partonIndex << std::endl;
                nMatchedJet++;
                break;
            }
            if(partonIndex == -1) {
                std::cerr << "      jet does not match to one of the scattered partons. Not including in EEC" << std::endl;
                nUnmatchedJet++;
                continue;
            }
            
            
            
            vector<fastjet::PseudoJet> partsForEEC;
            for(int p=0; p<stbl_ptcls.size(); p++)
            {
                fastjet::PseudoJet part = stbl_ptcls[p];
                double dPhi = part.phi_std() - jet.phi_std();
                if(dPhi > TMath::Pi()) dPhi -= 2*TMath::Pi();
                if(dPhi < -TMath::Pi()) dPhi += 2*TMath::Pi();
                    
                double dEta = part.eta() - jet.eta();
                double dR = sqrt(dPhi*dPhi + dEta*dEta);
                if(dR > 0.4) continue;
                partsForEEC.push_back(part);                
            }
            
            for (int i = 0; i < partsForEEC.size(); i++) {
                fastjet::PseudoJet c_i = partsForEEC[i];
                if (c_i.pt() < 1e-50) continue;
                for (int j = i + 1; j < partsForEEC.size(); j++) {
                    fastjet::PseudoJet c_j = partsForEEC[j];
                    if (c_j.pt() < 1e-50) continue;

                    double dPhi = c_i.phi_std() - c_j.phi_std();
                    if(dPhi > TMath::Pi()) dPhi -= 2*TMath::Pi();
                    if(dPhi < -TMath::Pi()) dPhi += 2*TMath::Pi();
                    
                    double dEta = c_i.eta() - c_j.eta();
                    double dR = sqrt(dPhi*dPhi + dEta*dEta);
                    pTAll[0]->Fill(dR, c_i.pt()*c_j.pt());
                    pTAll[partonIndex+1]->Fill(dR, c_i.pt()*c_j.pt());

                    if(jet.pt() > 20 && jet.pt() <= 30) {
                        pT_20_30[0]->Fill(dR, c_i.pt()*c_j.pt());
                        pT_20_30[partonIndex+1]->Fill(dR, c_i.pt()*c_j.pt());
                    }
                    else if(jet.pt() > 30 && jet.pt() <= 40) {
                        pT_30_40[0]->Fill(dR, c_i.pt()*c_j.pt());
                        pT_30_40[partonIndex+1]->Fill(dR, c_i.pt()*c_j.pt());
                    }
                    else if(jet.pt() > 40 && jet.pt() <= 60) {
                        pT_40_60[0]->Fill(dR, c_i.pt()*c_j.pt());
                        pT_40_60[partonIndex+1]->Fill(dR, c_i.pt()*c_j.pt());
                    }
                }
            }
        }
    } // end loop over events


    for(int i=0; i<5; i++) {
        if(i > 0) std::cout << "   ";
        std::cout << symbols[i] << " entries: " << pTAll[i]->GetEntries();
    }
    std::cout << std::endl;

    outfile->cd();
    for(int i=0; i<5; i++) {
        pTAll[i]->Write();
        pT_20_30[i]->Write();
        pT_30_40[i]->Write();
        pT_40_60[i]->Write();
    }
    outfile->Close();

    std::cout << "number of jets matched to initiating parton: " << nMatchedJet << std::endl;

    std::cout << "number of jets not matched to initiating parton: " << nUnmatchedJet << std::endl;

    // Done.
    return 0;
}
