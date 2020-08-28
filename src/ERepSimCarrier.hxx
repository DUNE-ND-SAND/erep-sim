#ifndef ERepSimCarrier_hxx_seen
#define ERepSimCarrier_hxx_seen

#include <TLorentzVector.h>

#include <memory>
#include <map>

namespace ERepSim {
    class Carrier;
}
class TG4HitSegment;

/// This describes a photon, electron, electron cloud, or something else that
/// is generated by an energy deposition in the detector, and then transported
/// to a sensor.  The initial position is the place the energy was deposited,
/// and the final position is "where" the signal was detected.  In fact, the
/// ending point should be the position that the signal is represented
/// (e.g. the end, or center, of a scintillating bar), so it ignores things
/// like long fibers outside of the detector.
///
/// The constructor takes an integer sensor identifier.  This is how the
/// response simulation distinguishes between sensors, but the value is
/// entirely up to the user.  The only thing that the response simulation
/// cares about is that each sensor has a different id, and that the
/// identifiers don't change (within an event).
class ERepSim::Carrier {
public:
    /// The container of carriers.
    typedef std::vector<std::shared_ptr<ERepSim::Carrier>> Container;

    /// The type of a map from the sensor identifier to the carriers
    /// associated with that sensor.
    typedef std::map<int,ERepSim::Carrier::Container> Map;

    Carrier(int sensorId, int segmentId,
            const TG4HitSegment* hitSegment = NULL);
    virtual ~Carrier();

    const TLorentzVector& GetInitialPosition() const {return fInitialPosition;}
    const TLorentzVector& GetFinalPosition() const {return fFinalPosition;}
    const TLorentzVector& GetPositionSigma() const {return fPositionSigma;}

    int GetSensorId() const {return fSensorId;}
    int GetSegmentId() const {return fSegmentId;}
    const TG4HitSegment* GetSegment() const {return fHitSegment;}

    /// The is the point at which the carrier started.  It is typically the
    /// average position of the segment that created the carrier.
    void SetInitialPosition(double x, double y, double z, double t) {
        fInitialPosition.SetX(x);
        fInitialPosition.SetY(y);
        fInitialPosition.SetZ(z);
        fInitialPosition.SetT(t);
    }

    /// Set the final position of the carrier.  The final position will be the
    /// position of the eventual hit.
    void SetFinalPosition(double x, double y, double z, double t) {
        fFinalPosition.SetX(x);
        fFinalPosition.SetY(y);
        fFinalPosition.SetZ(z);
        fFinalPosition.SetT(t);
    }

    /// The is the point at which the carrier started.  It is typically the
    /// average position of the segment that created the carrier.
    void SetPositionSigma(double x, double y, double z, double t) {
        fPositionSigma.SetX(x);
        fPositionSigma.SetY(y);
        fPositionSigma.SetZ(z);
        fPositionSigma.SetT(t);
    }

    /// Get or set the charge.
    double GetCharge() const {return fCharge;}
    void SetCharge(double q) {fCharge = q;}

private:

    /// The identifier for the sensor that is being hit.  The sensor should be
    /// "located" at the final position.
    int fSensorId;

    /// The index of the segment that created this carrier
    int fSegmentId;

    /// THe segment that created this carrier.
    const TG4HitSegment* fHitSegment;

    /// The initial position of the carrier
    TLorentzVector fInitialPosition;

    /// The final position of the carrier
    TLorentzVector fFinalPosition;

    /// The width of the carrier distribution at the final position.  This
    /// only manages the dispersion, and any correlations between the
    /// dimensions need to be handled in the code.  This is likely to be zero.
    TLorentzVector fPositionSigma;

    /// The "charge" of the carrier.  For a lot of applications, this should
    /// be ignored (e.g. photon counting), but allows this to be used to
    /// describe stuff like electron clouds in a TPC.  The default value is
    /// 1.0
    double fCharge;
};

#endif
