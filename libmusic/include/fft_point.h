/**
 * @file        fft_point.h
 * @brief       FftPoint class definition
 * @addtogroup  libmusic
 * @{
 */

#include "lmtypes.h"


class FftPoint {

private:
    FftPoint();

public:
    uint32_t      sampleNumber;   /** index in FFT frequency domain array */
    amplitude_t   magnitude;      /** magnitude value in polar notation */

    /**
     * Constructor
     *
     * @param   sn          sample number, corresponding index in FFT frequency
     *                      domain array
     * @param   magnitude   magnitude value in polar notation
     */
    FftPoint(uint32_t sn, amplitude_t m) : sampleNumber(sn), magnitude(m) {}

    friend bool operator<(const FftPoint& point1, const FftPoint& point2)
    {
        return (point1.magnitude < point2.magnitude);
    }
};

/** @} */
