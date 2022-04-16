#include <Time.hpp>


namespace rnd7 {
    class Clock {
    public:
        Clock() {};

        virtual ~Clock() {};

        
        virtual Time getCurrentTime() const { return Time{}; };

        Time getElapsedTime() const;

        Time restart();

    protected:

        Time m_startTime; ///< Time of last reset, in microseconds
    };

}
