#include "Time.hpp"


namespace rnd7 {
////////////////////////////////////////////////////////////
    const Time Time::Zero_;


////////////////////////////////////////////////////////////
    Time::Time() :
            m_microseconds(0) {
    }


////////////////////////////////////////////////////////////
    float Time::asSeconds() const {
        return m_microseconds / 1000000.f;
    }


////////////////////////////////////////////////////////////
    int Time::asMilliseconds() const {
        return static_cast<int>(m_microseconds / 1000);
    }


////////////////////////////////////////////////////////////
    long Time::asMicroseconds() const {
        return m_microseconds;
    }


////////////////////////////////////////////////////////////
    Time::Time(long microseconds) :
            m_microseconds(microseconds) {
    }


////////////////////////////////////////////////////////////
    Time seconds(float amount) {
        return Time(static_cast<long>(amount * 1000000));
    }


////////////////////////////////////////////////////////////
    Time milliseconds(int amount) {
        return Time(static_cast<long>(amount) * 1000);
    }


////////////////////////////////////////////////////////////
    Time microseconds(long amount) {
        return Time(amount);
    }


////////////////////////////////////////////////////////////
    bool operator==(Time left, Time right) {
        return left.asMicroseconds() == right.asMicroseconds();
    }


////////////////////////////////////////////////////////////
    bool operator!=(Time left, Time right) {
        return left.asMicroseconds() != right.asMicroseconds();
    }


////////////////////////////////////////////////////////////
    bool operator<(Time left, Time right) {
        return left.asMicroseconds() < right.asMicroseconds();
    }


////////////////////////////////////////////////////////////
    bool operator>(Time left, Time right) {
        return left.asMicroseconds() > right.asMicroseconds();
    }


////////////////////////////////////////////////////////////
    bool operator<=(Time left, Time right) {
        return left.asMicroseconds() <= right.asMicroseconds();
    }


////////////////////////////////////////////////////////////
    bool operator>=(Time left, Time right) {
        return left.asMicroseconds() >= right.asMicroseconds();
    }


////////////////////////////////////////////////////////////
    Time operator-(Time right) {
        return microseconds(-right.asMicroseconds());
    }


////////////////////////////////////////////////////////////
    Time operator+(Time left, Time right) {
        return microseconds(left.asMicroseconds() + right.asMicroseconds());
    }


////////////////////////////////////////////////////////////
    Time &operator+=(Time &left, Time right) {
        return left = left + right;
    }


////////////////////////////////////////////////////////////
    Time operator-(Time left, Time right) {
        return microseconds(left.asMicroseconds() - right.asMicroseconds());
    }


////////////////////////////////////////////////////////////
    Time &operator-=(Time &left, Time right) {
        return left = left - right;
    }


////////////////////////////////////////////////////////////
    Time operator*(Time left, float right) {
        return seconds(left.asSeconds() * right);
    }


////////////////////////////////////////////////////////////
    Time operator*(Time left, long right) {
        return microseconds(left.asMicroseconds() * right);
    }


////////////////////////////////////////////////////////////
    Time operator*(float left, Time right) {
        return right * left;
    }


////////////////////////////////////////////////////////////
    Time operator*(long left, Time right) {
        return right * left;
    }


////////////////////////////////////////////////////////////
    Time &operator*=(Time &left, float right) {
        return left = left * right;
    }


////////////////////////////////////////////////////////////
    Time &operator*=(Time &left, long right) {
        return left = left * right;
    }


////////////////////////////////////////////////////////////
    Time operator/(Time left, float right) {
        return seconds(left.asSeconds() / right);
    }


////////////////////////////////////////////////////////////
    Time operator/(Time left, long right) {
        return microseconds(left.asMicroseconds() / right);
    }


////////////////////////////////////////////////////////////
    Time &operator/=(Time &left, float right) {
        return left = left / right;
    }


////////////////////////////////////////////////////////////
    Time &operator/=(Time &left, long right) {
        return left = left / right;
    }


////////////////////////////////////////////////////////////
    float operator/(Time left, Time right) {
        return left.asSeconds() / right.asSeconds();
    }


////////////////////////////////////////////////////////////
    Time operator%(Time left, Time right) {
        return microseconds(left.asMicroseconds() % right.asMicroseconds());
    }


////////////////////////////////////////////////////////////
    Time &operator%=(Time &left, Time right) {
        return left = left % right;
    }

}
