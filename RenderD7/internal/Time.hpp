namespace rnd7 {
class Time {
    public:

        Time();

        float asSeconds() const;

        int asMilliseconds() const;

        long asMicroseconds() const;

        static const Time Zero_;

    private:

        friend Time seconds(float);

        friend Time milliseconds(int);

        friend Time microseconds(long);

        explicit Time(long microseconds);

    private:

        long m_microseconds; ///< Time value stored as microseconds
    };

    Time seconds(float amount);

    Time milliseconds(int amount);

    Time microseconds(long amount);

    bool operator==(Time left, Time right);

    bool operator!=(Time left, Time right);

    bool operator<(Time left, Time right);

    bool operator>(Time left, Time right);
    bool operator<=(Time left, Time right);

    bool operator>=(Time left, Time right);

    Time operator-(Time right);
    Time operator+(Time left, Time right);

    Time &operator+=(Time &left, Time right);

    Time operator-(Time left, Time right);

    Time &operator-=(Time &left, Time right);

    Time operator*(Time left, float right);

    Time operator*(Time left, long right);

    Time operator*(float left, Time right);

    Time operator*(long left, Time right);

    Time &operator*=(Time &left, float right);

    Time &operator*=(Time &left, long right);

    Time operator/(Time left, float right);

    Time operator/(Time left, long right);

    Time &operator/=(Time &left, float right);

    Time &operator/=(Time &left, long right);

    float operator/(Time left, Time right);

    Time operator%(Time left, Time right);

    Time &operator%=(Time &left, Time right);

} 
