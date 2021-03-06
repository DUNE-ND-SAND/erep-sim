#ifndef ERepSimResponseTPC_hxx_seen
#define ERepSimResponseTPC_hxx_seen

#include "ERepSimResponseBase.hxx"

#include <TG4HitSegment.h>

namespace ERepSim {
    class ResponseTPC;
}



class ERepSim::ResponseTPC: public ERepSim::ResponseBase {
public:
    ResponseTPC(const char* modelName,
                const char* volumeName,
                int id);
    virtual ~ResponseTPC();

    virtual void Initialize();

    virtual void Process(const TG4Event* event,
                         const TG4HitSegmentContainer& segments);

    virtual void Reset();

private:
    /// Forage information out of the geometry.
    bool RecurseGeometry();

    //Iterates over segment
    void AddTrack(const TG4Event* event, const TG4HitSegment& seg, int segId);

    //Generates a certain amount of electrons at a given position of a segment
    void GenerateElectrons(const TG4Event* event, const TG4HitSegment& seg, int segId,
                           TLorentzVector generationPoint, double segLength);

    //Drifts the electrons towards anode (MM)
    void DriftElectrons(const TG4Event* event, const TG4HitSegment& seg, int segId,
                        int sensId, double nbElectrons,
                        TLorentzVector generationPoint);

    //Computes the MM amplification
    void MMAmplification(const TG4Event* event,
                         const TG4HitSegment& seg, int segId,
                         int sensId, double nbElectrons,
                         double avg_t, double sigma_t,
                         TLorentzVector generationPoint);

    //Applies charge spreading due to resistive pads
    void SpreadCharge(const TG4Event* event, const TG4HitSegment& seg, int segId,
                      int sensId, double nbElectrons,
                      double avg_t, double sigma_t,
                      TLorentzVector generationPoint);

    //Adding a hit in a given pad
    void AddHit(const TG4Event* event, const TG4HitSegment& seg, int segId,
                int sensId, double nbElectrons,
                double avg_t, double sigma_t,
                TLorentzVector generationPoint);

    /// Turn a plane, pad_y and pad_z indexes into a sensor id (or a cube id).
    int GetSensorId(int plane, int pad_y, int pad_z);

    void GetPadInfo(int sensId, int &plane, int& pad_y, int& pad_z);

    //Returns drift time in ns
    double GetDriftTime(double dist);

    //Returns longitudinal spread time in ns
    double GetSigmaDriftTime(double dist);

    //Returns longitudinal spread in mm
    double GetLongitudinalSpread(double dist);
    //Returns 3d length between two points
    double Length(const TLorentzVector& A, const TLorentzVector& B);

    // The volume defining the drift region for this TPC.
    const char* fVolumeName;

    // Selects which of the 3 tpcs
    int fTpcId;

    // Pad sizes (hardcoded)
    double fPadSizeY;
    double fPadSizeZ;

    // Dimensions of the tpc
    double fXmin;
    double fXmax;
    double fYmin;
    double fYmax;
    double fZmin;
    double fZmax;
    double fCathodeX;

    // Number of pads
    int fNbPadsY;
    int fNbPadsZ;

    // Step size for electron generation along tracks
    double fStepSize;
    // Ionisation coefficient of gas
    double fWI;
    // MM gain
    double fGainMM;
};

#endif

// Local Variables:
// mode:c++
// c-basic-offset:4
// compile-command:"$(git rev-parse --show-toplevel)/build/erep-build.sh force"
// End:
