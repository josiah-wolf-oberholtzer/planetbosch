#ifndef PLANETBOSCH_FREQUENCY_SHIFTER
#define PLANETBOSCH_FREQUENCY_SHIFTER

#include "DSP.h"
#include "Hilbert.h"
#include <math.h>

namespace planetbosch {

class FrequencyShifter {
public:
  FrequencyShifter() {}
  ~FrequencyShifter() {}

  inline void Init(float sample_rate) {
    hilbert_.Init(sample_rate);
    phase_       = 0.f;
    frequency_   = 0.f;
    sample_rate_ = sample_rate;
  }

  float Process(float in) {
    float out[2];
    hilbert_.Process(in, &out[0]);
    phase_ += frequency_ * TWOPI_F / sample_rate_;
    // wrap the phase to avoid losing precision
    if (phase_ > TWOPI_F) {
      phase_ -= TWOPI_F;
    } else {
      phase_ += TWOPI_F;
    }
    return (out[0] * cosf(phase_)) + (out[1] * sinf(phase_));
  }

  inline void SetFrequency(float frequency) { frequency_ = frequency; }

private:
  planetbosch::Hilbert hilbert_;
  float                phase_;
  float                frequency_;
  float                sample_rate_;
};

} // namespace planetbosch

#endif
