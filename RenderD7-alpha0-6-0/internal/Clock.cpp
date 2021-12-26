#include "Clock.hpp"

namespace rnd7 {

////////////////////////////////////////////////////////////
    Time Clock::getElapsedTime() const {
        return getCurrentTime() - m_startTime;
    }


////////////////////////////////////////////////////////////
    Time Clock::restart() {
        Time now = getCurrentTime();
        Time elapsed = now - m_startTime;
        m_startTime = now;

        return elapsed;
    }

}
