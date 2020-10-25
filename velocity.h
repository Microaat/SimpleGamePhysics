#ifndef VELOCITY_H
#define VELOCITY_H

#include <iostream>
#include <limits>
#include <type_traits>
#include <cmath>

// The angle class main will primarily be working with degrees. Arithmetics with this class
// should be done with degrees and not radians.
template <class T>
class Angle {
    T mDegree;
    T mRadian;

    // Determine if degree is been set
    void cleanAngle( bool degree = true ){
        if( degree ) mRadian = degreeToRadian( mDegree );
        else mDegree = radianToDegree( mRadian );
        if( std::fmod( mDegree, 360.0 ) < 0 ) mDegree += 360.0;
        if( std::fmod( mRadian, 2 * PI ) < 0 ) mRadian += 2 * PI;
    }

public:
    typedef Angle<T> AngleT;
    static constexpr double PI = std::atan( -1 );

    Angle( const T& value = 0 ): mDegree( value ){ cleanAngle(); }
    Angle( const AngleT& ) = default;

    // Getters
    const T& getDegree() const { return mDegree; }
    const T& getRadian() const { return mRadian; }

    // Get the value from trigonometric functions
    T cos() const { return std::cos( mRadian ); }
    T sin() const { return std::sin( mRadian ); }
    T tan() const { return std::tan( mRadian ); }

    // Setters
    void setDegree( const T& value ){ mDegree = value; cleanAngle(); }
    void setRadian( const T& value ){ mRadian = value; cleanAngle( false ); }
    void addDegree( const T& value ){ setDegree( mDegree + value ); }
    void addRadian( const T& value ){ setRadian( mRadian + value ); }

    // Reference Angles
    void ref90(){ addDegree( 90 ); }
    void ref180(){ addDegree( 180 ); }
    void ref270(){ addDegree( 270 ); }

    // Static Methods
    static T degreeToRadian( const T& angle ) { return angle * ( PI / 180.0 ); }
    static T radianToDegree( const T& radian ) { return radian * ( 180.0 / PI ); }

    // Overloaded Operators
    explicit operator T() const { return mDegree; }
    AngleT& operator= ( const AngleT& a ){ setDegree( a.mDegree ); return *this; }
    bool operator== ( const AngleT& a ) const {
        if( std::is_integral<T>::value ) return mDegree == a.mDegree;
        return std::abs( mDegree - a.mDegree ) <= std::numeric_limits<T>::epsilon() * std::abs( mDegree );
    }
    bool operator!= ( const AngleT& a ) const { return !( *this == a ); }
    bool operator< ( const AngleT& a ) const { return mDegree < a.mDegree; }
    bool operator> ( const AngleT& a ) const { return mDegree > a.mDegree; }
    bool operator<= ( const AngleT& a ) const { return mDegree <= a.mDegree; }
    bool operator>= ( const AngleT& a ) const { return mDegree >= a.mDegree; }

    AngleT& operator+= ( const AngleT& a ){ return ( *this = mDegree + a.mDegree ); }
    AngleT& operator-= ( const AngleT& a ){ return ( *this = mDegree - a.mDegree ); }
    AngleT& operator*= ( const AngleT& a ){ return ( *this = mDegree * a.mDegree ); }
    AngleT& operator/= ( const AngleT& a ){ return ( *this = mDegree / a.mDegree ); }

    AngleT& operator+ ( const AngleT& a ) const { return AngleT( *this ) += a ; }
    AngleT& operator- ( const AngleT& a ) const { return AngleT( *this ) -= a ; }
    AngleT& operator* ( const AngleT& a ) const { return AngleT( *this ) *= a ; }
    AngleT& operator/ ( const AngleT& a ) const { return AngleT( *this ) /= a ; }
};

template <class T>
class Speed {
    T mSpeed;

public:
    typedef Speed<T> SpeedT;

    Speed( const T& s = 0 ): mSpeed( s ){}
    Speed( const SpeedT& ) = default;

    // Functions
    void makePositive() { mSpeed *= ( mSpeed < 0 ) ? -1 : 1; }

    // Getters
    const T& getSpeed() const { return mSpeed; }

    // Setters
    void setSpeed( const T& s ){ mSpeed = s; }

    // Overloaded Operators
    operator T() const { return mSpeed; } // This converts from Speed to the defined primitive type of the template.
    bool operator== ( const SpeedT& s ) const { return s.mSpeed == mSpeed; }
    bool operator!= (const SpeedT& s ) const { return !( s == *this ); }
    bool operator< ( const SpeedT& s ) const { return mSpeed < s.mSpeed; }
    bool operator> ( const SpeedT& s ) const { return mSpeed > s.mSpeed; }
    bool operator<= ( const SpeedT& s ) const { return mSpeed <= s.mSpeed; }
    bool operator>= ( const SpeedT& s ) const { return mSpeed >= s.mSpeed; }

    SpeedT& operator+= ( const SpeedT& s ){ mSpeed += s.mSpeed; return *this; }
    SpeedT& operator-= ( const SpeedT& s ){ mSpeed -= s.mSpeed; return *this; }
    SpeedT& operator*= ( const SpeedT& s ){ mSpeed *= s.mSpeed; return *this; }
    SpeedT& operator/= ( const SpeedT& s ){ mSpeed /= s.mSpeed; return *this; }

    SpeedT operator+ ( SpeedT& s ) const { return ( SpeedT( *this ) += s.mSpeed ); }
    SpeedT operator- ( const SpeedT& s ) const { return ( SpeedT( *this ) -= s.mSpeed ); }
    SpeedT operator* ( const SpeedT& s ) const { return ( SpeedT( *this ) *= s.mSpeed ); }
    SpeedT operator/ ( const SpeedT& s ) const { return ( SpeedT( *this ) /= s.mSpeed ); }
};

template <class T>
class Velocity {
    T mSpeed;
    T mAngle;

    void cleanSpeed(){
        if( mSpeed < 0 ){
            mSpeed *= -1;
            addAngle( 180 );
        }
    }
    void cleanAngle(){ if( std::fmod( mAngle, 360.0 ) < 0 ) mAngle += 360.0; }

public:
    typedef Velocity<T> VelocityT;

    static constexpr double PI = std::acos( -1 );

    Velocity(): mSpeed( 0 ), mAngle( 0 ){}
    Velocity( const T& speed, const T& angle ):
        mSpeed( speed ), mAngle( angle ){}
    Velocity( const Velocity<T> & ) = default;

    bool isSameSpeed( const T& speed ){
        if( std::is_integral<T>::value ) return mSpeed == speed;
        return std::abs( mSpeed - speed ) <= std::numeric_limits<T>::epsilon() * std::abs( mSpeed );
    }
    bool isSameAngle( const T& angle ){
        if( std::is_integral<T>::value ) return mAngle == angle;
        return std::abs( mAngle - angle ) <= std::numeric_limits<T>::epsilon() * std::abs( mAngle );
    }

    void angleReflect(){ mAngle += 2 * mAngle; }

    // Getters
    const T& getSpeed() const { return mSpeed; }
    const T& getAngle() const { return mAngle; }
    T getXComponent() const { return mSpeed * std::cos( angleToRadian( mAngle ) ); }
    T getYComponent() const { return mSpeed * std::sin( angleToRadian( mAngle ) ); }

    // Setters
    void setSpeed( const T& speed ){ mSpeed = speed; cleanSpeed(); }
    void addSpeed( const T& speed ){ mSpeed += speed; cleanSpeed(); }
    void setAngle( const T& angle ){ mAngle = angle; cleanAngle(); }
    void addAngle( const T& angle ){ mAngle += angle; cleanAngle(); }

    // Resetters
    void resetSpeed(){ mSpeed = 0; }
    void resetAngle(){ mAngle = 0; }
    void resetVelocity(){ mSpeed = 0; mAngle = 0; }

    // Comparison Operators
    bool operator ==( const VelocityT & rhs ){ return isSameSpeed( rhs.mSpeed ) && isSameAngle( rhs.mAngle ); }
    bool operator !=( const VelocityT & rhs ){ return !( *this == rhs ); }

    // For std::cout
    friend std::ostream & operator<< ( std::ostream & out, const VelocityT& v ){
        return ( out << '(' << v.mSpeed << ", " << v.mAngle << ')' );
    }

    // Static Functions
    static T angleToRadian( const T& angle ) { return angle * ( PI / 180.0 ); }
    static T radianToAngle( const T& radian ) { return radian * ( 180.0 / PI ); }
};

#endif // VELOCITY_H
