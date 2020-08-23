#include "EncSim.h"

static EncSim* This; // hack, allows one instance only

EncSim::EncSim(unsigned pinA, unsigned pinB, unsigned pinZ)
{
    This = this;
    this->A = pinA;
    this->B = pinB;
    this->Z = pinZ;
}

EncSim& EncSim::begin(/*unsigned pinA, unsigned pinB, int pinZ*/)
{
    //CCM_CSCMR1 &= ~CCM_CSCMR1_PERCLK_CLK_SEL; // set PIT clock to 150MHz

    phaseA.begin(A);
    phaseB.begin(B);
    if (Z < UINT32_MAX)
    {
        pinMode(Z, OUTPUT);
        digitalWriteFast(Z, LOW);
    }

    // defaults
    setFrequency(100);         // Quadrature signal with
    setPhase(90);              // 50Hz and 90° phase
    setTotalBounceDuration(0); // 5ms bouncing
    setBounceDurationMax(300); // bounce peaks between
    setBounceDurationMin(30);  // 30 and 300µs
    setPeriod(100);            // index pulse every 100 steps

    current = 0; // reset counter
    return *this;
}

void EncSim::moveAbsAsync(int _target)
{
    if (_target == current)
        return;

    target = _target;
    direction = (target >= current) ? 1 : -1;

    if (!running)
        mainTimer.begin([] { This->pitISR(); }, T[current & 1]);
    running = true;
}

// start relative move and return immediately
void EncSim::moveRelAsync(int delta)
{
    moveAbsAsync(current + delta);
}

// move to absolute position (blocking)
void EncSim::moveAbs(int target)
{
    moveAbsAsync(target);
    while (isRunning())
    {
        delay(10);
    }
}

// move to relative position (blocking)
void EncSim::moveRel(int delta)
{
    moveAbs(current + delta);
}

// stop movement
void EncSim::stop()
{
    mainTimer.end();
    running = false;
}

bool EncSim::isRunning()
{
    return running;
}

EncSim& EncSim::setFrequency(float f_Hz)
{
    frequency = f_Hz;
    return setCountRate(frequency, phase);
}

EncSim& EncSim::setPeriod(unsigned p)
{
    period = p;
    return *this;
}

EncSim& EncSim::setPhase(float deg)
{
    phase = deg;
    return setCountRate(frequency, phase);
}

EncSim& EncSim::setCountRate(float f_Hz, float phase_deg)
{
    constexpr float minPhase = 5.f;  // deg
    constexpr float maxPhase = 90.f; // deg
    constexpr float minFreq = 0.2f;  // Hz
    constexpr float maxFreq = 2E6f;  // Hz

    float freq = std::max(minFreq, std::min(maxFreq, f_Hz));
    float halfPeriod = 1'000'000 / freq; //µs
    float phase = max(minPhase, min(maxPhase, phase_deg));

    T[0] = halfPeriod * (phase / 90.0);
    T[1] = 2.0f * halfPeriod - T[0];

    frequency = 2'000'000 / (T[0] + T[1]);

    //Serial.printf("%5f %5f %5f %5f \n", halfPeriod, T[0], T[1], frequency);
    return *this;
}

EncSim& EncSim::setTotalBounceDuration(unsigned microseconds)
{
    phaseA.totalBounceTime = microseconds;
    phaseB.totalBounceTime = microseconds;
    return *this;
}

EncSim& EncSim::setBounceDurationMin(unsigned microseconds)
{
    phaseA.minBounceTime = microseconds;
    phaseB.minBounceTime = microseconds;
    return *this;
}

EncSim& EncSim::setBounceDurationMax(unsigned microseconds)
{
    phaseA.maxBounceTime = microseconds;
    phaseB.maxBounceTime = microseconds;
    return *this;
}

void EncSim::printSettings(Stream& s)
{
    float phase = 180.0 * T[0] / (T[1] + T[0]);

    s.printf("  freq:      %.0f Hz\n", frequency);
    s.printf("  phase:     %.0f\u00B0\n", phase);
    s.printf("  period:    %d steps\n", period);
    s.printf("  btot:      %d \u00B5s\n", phaseA.totalBounceTime);
    s.printf("  bmin:      %d \u00B5s\n", phaseA.minBounceTime);
    s.printf("  bmax:      %d \u00B5s\n", phaseA.maxBounceTime);
    s.println();
}