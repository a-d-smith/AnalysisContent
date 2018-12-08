#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1.h"

void plots(const std::string &fileName, const std::string &treeName)
{
    // Open the input file and get the tree
    TFile *f = new TFile(fileName.c_str(), "READ");
    TTree *t = nullptr;
    f->GetObject(treeName.c_str(), t);

    // Set up the canvas
    TCanvas *c = new TCanvas();

    // Set up the histograms
    const unsigned int nBins(100); // Number of bins
    const float pMin(0.f);         // Minimum momentum to plot / GeV
    const float pMax(3.f);         // Maximum momentum to plot / GeV

    TH1F *hAll = new TH1F("hAll", "", nBins, pMin, pMax);
    TH1F *hOneHit = new TH1F("hOneHit", "", nBins, pMin, pMax);
    TH1F *hReconstructable = new TH1F("hReconstructable", "", nBins, pMin, pMax);

    // Style the histograms
    hAll->SetLineColor(kBlack);
    hOneHit->SetLineColor(kRed + 1);
    hReconstructable->SetLineColor(kAzure -1);

    // Define the required details for plotting
    const std::vector<int> pdgs({13, 211, 2212, 22, 11, 321});
    const std::vector<std::string> names({"muons", "charge pions", "protons", "photons", "electrons", "charged kaons"});

    // Make the plots for each particle type
    for (unsigned int i = 0; i < pdgs.size(); ++i)
    {
        const int pdg(pdgs.at(i));
        const std::string name(names.at(i));

        // Set the titles
        hAll->GetXaxis()->SetTitle("True Momentum / GeV");
        hAll->GetYaxis()->SetTitle(("Number of " + name).c_str());

        // Draw the histograms
        t->Draw("momentum >> hAll", ("abs(pdg) == " + std::to_string(pdg)).c_str());
        t->Draw("momentum >> hOneHit", ("(nHitsU + nHitsV + nHitsW) > 0 && abs(pdg) == " + std::to_string(pdg)).c_str(), "same");
        t->Draw("momentum >> hReconstructable", ("isReconstructable && abs(pdg) == " + std::to_string(pdg)).c_str(), "same");

        // Save the plots
        c->SaveAs(("MomentumThreshold_" + name + ".png").c_str());
        c->SaveAs(("MomentumThreshold_" + name + ".pdf").c_str());
    }
}
