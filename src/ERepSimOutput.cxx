#include "ERepSimOutput.hxx"

ERepSim::Output* ERepSim::Output::ThisOutput = NULL;

ERepSim::Output& ERepSim::Output::Get(void) {
    if (!ThisOutput) {
        ThisOutput = new ERepSim::Output;
    }
    return *ThisOutput;
}

ERepSim::Output::Output() {}
ERepSim::Output::Output(const Output&) {}
ERepSim::Output& ERepSim::Output::operator=(const Output&) {return *ThisOutput;}

void ERepSim::Output::CreateTrees() {
    PropertyTree = new TTree("ERepSimProperties","ERepSim Properties");
    PropertyTree->Branch("Property",&Property);
    PropertyTree->Branch("PropertyString",&PropertyString);

    DataTree = new TTree("ERepSimEvents","Energy Response");
    DataTree->Branch("RunId",&RunId,"RunId/I");
    DataTree->Branch("EventId",&EventId,"EventId/I");
    DataTree->Branch("HitSensorId",&HitSensorId);
    DataTree->Branch("HitX",&HitX);
    DataTree->Branch("HitY",&HitY);
    DataTree->Branch("HitZ",&HitZ);
    DataTree->Branch("HitTime",&HitTime);
    DataTree->Branch("HitTimeWidth",&HitTimeWidth);
    DataTree->Branch("HitCharge",&HitCharge);
    DataTree->Branch("HitSegmentBegin",&HitSegmentBegin);
    DataTree->Branch("HitSegmentEnd",&HitSegmentEnd);
    DataTree->Branch("HitContribBegin",&HitContribBegin);
    DataTree->Branch("HitContribEnd",&HitContribEnd);

    DataTree->Branch("SegmentIds",&SegmentIds);
    DataTree->Branch("SegmentPDG",&SegmentPDG);
    DataTree->Branch("SegmentEnergy",&SegmentEnergy);
    DataTree->Branch("SegmentX1",&SegmentX1);
    DataTree->Branch("SegmentY1",&SegmentY1);
    DataTree->Branch("SegmentZ1",&SegmentZ1);
    DataTree->Branch("SegmentX2",&SegmentX2);
    DataTree->Branch("SegmentY2",&SegmentY2);
    DataTree->Branch("SegmentZ2",&SegmentZ2);
    DataTree->Branch("SegmentT",&SegmentT);

    DataTree->Branch("ContribIds",&ContribIds);
    DataTree->Branch("ContribPDG",&ContribPDG);
    DataTree->Branch("ContribMomentum",&ContribMomentum);
}

void ERepSim::Output::Reset() {
    RunId = -1;
    EventId = -1;
    HitSensorId.clear();
    HitX.clear();
    HitY.clear();
    HitZ.clear();
    HitTime.clear();
    HitTimeWidth.clear();
    HitCharge.clear();
    HitSegmentBegin.clear();
    HitSegmentEnd.clear();
    HitContribBegin.clear();
    HitContribEnd.clear();
    SegmentIds.clear();
    SegmentEnergy.clear();
    SegmentPDG.clear();
    SegmentX1.clear();
    SegmentY1.clear();
    SegmentZ1.clear();
    SegmentX2.clear();
    SegmentY2.clear();
    SegmentZ2.clear();
    SegmentT.clear();
    ContribIds.clear();
    ContribPDG.clear();
    ContribMomentum.clear();
}

void ERepSim::Output::Fill() {
    DataTree->Fill();
}

void ERepSim::Output::Write() {
    DataTree->Write();
    PropertyTree->Fill();
    PropertyTree->Write();
}