#include "ERepSimDetectorECal.hxx"
#include "ERepSimResponseBase.hxx"
#include "ERepSimDigiHit.hxx"
#include "ERepSimUnits.hxx"
#include "ERepSimDefs.hxx"

#include "ERepSimOutput.hxx"

#include <TG4Event.h>

#include <TGeoManager.h>
#include <TTree.h>

#include <iostream>

ERepSim::DetectorECal::DetectorECal(TTree* tDigitTree)
    : DetectorBase("ECal"), fDigitTree(tDigitTree) {
    Initialize();
}

ERepSim::DetectorECal::~DetectorECal() {
    std::cout << "DetectorECal::Deconstruct" << std::endl;
}

void ERepSim::DetectorECal::Initialize() {
    std::cout << "DetectorECal::Initialize" << std::endl;
    fCells = new std::vector<cell>;
}

void ERepSim::DetectorECal::Reset() {
#ifdef LOUD_AND_PROUD
    std::cout << "DetectorECal::Reset" << std::endl;
#endif
    DetectorBase::Reset();
    fHits.clear();
}

void ERepSim::DetectorECal::Accumulate(int entry, const TG4Event* event) {
    if (!fDigitTree) {
        std::cout << "DetectorECal::Accumulate -- No tree"
                  << std::endl;
        return;
    }
    fDigitTree->SetBranchAddress("cell",&fCells);
    fDigitTree->GetEntry(entry);

    double minTime = 1E+20;
    double maxTime = -1E+20;
    int generatedHits = 0;
#ifdef LOUD_AND_PROUD
    std::cout << "DetectorECal::Accumulate " << fCells->size()
              << " cells"
              << std::endl;
#endif
    for (std::vector<cell>::iterator c = fCells->begin();
         c != fCells->end(); ++c) {
        if (c->mod > 29) continue;

        // Do sensor 1:  The id is ddddd mmmmmm
        //
        // ddddd (bits 27-31 range 0-31) -- The sub detector definition (12)
        // mmmmmmm (bits 21-26, range 0-63) -- the module number.
        // llll (bits 17-20, range 0-15) -- The layer number
        // cccccccc (bits 9-16, range 0-255) -- The cell number
        // xxxxxxxx (bits 1-8, reserved)
        // e (bit 0) -- The end of the bar.
        //
        int cell_id1 = ERepSim::Def::Detector::kECal << 27;
        cell_id1 += c->mod << 21;
        cell_id1 += c->lay << 17;
        cell_id1 += c->cel << 9;
        cell_id1 += 0;
        double tAvg = 0.0;
        double ttAvg = 0.0;
        double tWidth = 0.0;
        if (c->pe_time1.size() > 0) {
            for (double t : c->pe_time1) {
                tAvg += t;
                ttAvg += t*t;
            }
            tAvg /= c->pe_time1.size();
            ttAvg /= c->pe_time1.size();
            tWidth = ttAvg - tAvg*tAvg;
            if (tWidth < 0.0) tWidth = 0.0;
            tWidth = std::sqrt(tWidth/c->pe_time1.size());
        }
        tWidth = std::max(1*unit::ns, tWidth);
        tWidth = std::min(10*unit::ns, tWidth);

        double q1 = c->adc1/4.0;
        double t1 = c->tdc1;
        if (t1 > 0 && q1 > 0) {
            ERepSim::DigiHit cell1(cell_id1);
            TVector3 pos1;
            if (c->mod < 30) {
                pos1.SetX(c->x - c->l/2.0);
                pos1.SetY(c->y);
                pos1.SetZ(c->z);
            }
            else {
                pos1.SetX(c->x);
                pos1.SetY(c->y - c->l/2.0);
                pos1.SetZ(c->z);
            }

            cell1.SetPosition(pos1.X(),pos1.Y(),pos1.Z());
            cell1.GetCharges().push_back(q1);
            minTime = std::min(t1,minTime);
            maxTime = std::max(t1,maxTime);
            cell1.GetTimes().push_back(t1);
            cell1.GetTimeWidths().push_back(tWidth);
            fHits[cell1.GetSensorId()].push_back(
                std::shared_ptr<ERepSim::DigiHit>(
                    new ERepSim::DigiHit(cell1)));
        }


        // Do sensor 2:  The id is ddddd mmmmmm
        //
        // ddddd (bits 27-31 range 0-31) -- The sub detector definition (12)
        // mmmmmmm (bits 21-26, range 0-63) -- the module number.
        // llll (bits 17-20, range 0-15) -- The layer number
        // cccccccc (bits 9-16, range 0-255) -- The cell number
        // xxxxxxxx (bits 1-8, reserved)
        // e (bit 0) -- The end of the bar.
        //
        int cell_id2 = ERepSim::Def::Detector::kECal << 27;
        cell_id2 += c->mod << 21;
        cell_id2 += c->lay << 17;
        cell_id2 += c->cel << 9;
        cell_id2 += 1;
        tAvg = 0.0;
        ttAvg = 0.0;
        tWidth = 0.0;
        if (c->pe_time2.size() > 0) {
            for (double t : c->pe_time2) {
                tAvg += t;
                ttAvg += t*t;
            }
            tAvg /= c->pe_time2.size();
            ttAvg /= c->pe_time2.size();
            tWidth = ttAvg - tAvg*tAvg;
            if (tWidth < 0.0) tWidth = 0.0;
            tWidth = std::sqrt(tWidth/c->pe_time2.size());
        }

        tWidth = std::max(1*unit::ns, tWidth);
        tWidth = std::min(10*unit::ns, tWidth);

        double q2 = c->adc2/4.0;
        double t2 = c->tdc2;
        if (t2 > 0 && q2 > 0) {
            ERepSim::DigiHit cell2(cell_id2);
            TVector3 pos2;
            if (c->mod < 30) {
                pos2.SetX(c->x + c->l/2.0);
                pos2.SetY(c->y);
                pos2.SetZ(c->z);
            }
            else {
                pos2.SetX(c->x);
                pos2.SetY(c->y + c->l/2.0);
                pos2.SetZ(c->z);
            }
            cell2.SetPosition(pos2.X(),pos2.Y(),pos2.Z());
            cell2.GetCharges().push_back(q2);
            cell2.GetTimes().push_back(t2);
            cell2.GetTimeWidths().push_back(tWidth);
            fHits[cell2.GetSensorId()].push_back(
                std::shared_ptr<ERepSim::DigiHit>(
                    new ERepSim::DigiHit(cell2)));
        }
    }

    if (fCells->size() > 0) {
        std::cout << " min " << minTime
                  << "  max "  << maxTime
                  << std::endl;
    }

    // Pack all of the ecal hit segments.
    const TG4HitSegmentDetectors& segments = event->SegmentDetectors;
    TG4HitSegmentDetectors::const_iterator detector = segments.find("EMCalSci");
    if (detector != segments.end()) {
        const TG4HitSegmentContainer&  g4Hits = detector->second;
#ifdef LOUD_AND_PROUD
        std::cout << "ERepSim::DetectorECal: Pack "
                  << g4Hits.size() << " segments"
                  << std::endl;
#endif
        for (TG4HitSegmentContainer::const_iterator s = g4Hits.begin();
             s != g4Hits.end(); ++s) {
            TLorentzVector sPos;
            TLorentzVector sPos1;
            TLorentzVector sPos2;
            int pdg = 0;
            int trackId = -1;
            double ener = s->GetEnergyDeposit();
            sPos1 = s->GetStart();
            sPos2 = s->GetStop();
            sPos = 0.5*(sPos1 + sPos2);
            if (!s->Contrib.empty()) {
                trackId = s->Contrib.front();
                if (trackId >= 0) {
                    trackId += ERepSim::Output::Get().TrajectoryIdOffset;
                }
                pdg = ERepSim::Output::Get().TrajectoryPDG[trackId];
            }
            ERepSim::Output::Get().SegmentIds.push_back(
                ERepSim::ResponseBase::GetNextSegmentIdentifier());
            ERepSim::Output::Get().SegmentTrackId.push_back(trackId);
            ERepSim::Output::Get().SegmentEnergy.push_back(ener);
            ERepSim::Output::Get().SegmentPDG.push_back(pdg);
            ERepSim::Output::Get().SegmentX1.push_back(sPos1.X());
            ERepSim::Output::Get().SegmentY1.push_back(sPos1.Y());
            ERepSim::Output::Get().SegmentZ1.push_back(sPos1.Z());
            ERepSim::Output::Get().SegmentX2.push_back(sPos2.X());
            ERepSim::Output::Get().SegmentY2.push_back(sPos2.Y());
            ERepSim::Output::Get().SegmentZ2.push_back(sPos2.Z());
            ERepSim::Output::Get().SegmentT.push_back(sPos.T());
        }
    }

#ifdef LOUD_AND_PROUD
    std::cout << "DetectorECal::Accumulate " << generatedHits
              << " hits generated" << std::endl;
#endif
}

namespace {
    struct DigiHitTimeSort {
        bool operator ()(std::shared_ptr<ERepSim::DigiHit>& lhs,
                      std::shared_ptr<ERepSim::DigiHit>& rhs) {
            return (lhs->GetTime() < rhs->GetTime());
        }
    };
}

void ERepSim::DetectorECal::Process(int entry) {
#ifdef LOUD_AND_PROUD
    std::cout << "DetectorECal::Process " << fHits.size()
              << std::endl;
#endif
    int generatedHits = 0;
    for (ERepSim::DigiHit::Map::iterator c = fHits.begin();
         c != fHits.end(); ++c) {
        ERepSim::DigiHit::Container& hits = c->second;
        std::sort(hits.begin(), hits.end(), DigiHitTimeSort());
        if (hits.empty()) continue;
        const double integrationWindow = 400*unit::ns;
        for (ERepSim::DigiHit::Container::iterator hit = hits.begin();
             hit != hits.end(); ++hit) {
            ERepSim::DigiHit::Container::iterator h = hit+1;
            while (h != hits.end()
                   && (*h)->GetTime()-(*hit)->GetTime() < integrationWindow) {
                (*hit)->GetCharges()[0] += (*h)->GetCharge();
                h = hits.erase(h);
            }
            PackDigiHit(*(*hit));
        }
    }

    std::cout << "DetectorECal::Process " << generatedHits
              << " hits generated" << std::endl;
}

// Local Variables:
// mode:c++
// c-basic-offset:4
// compile-command:"$(git rev-parse --show-toplevel)/build/erep-build.sh force"
// End:
