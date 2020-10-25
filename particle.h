#ifndef PARTICLE_H
#define PARTICLE_H

#include <iostream>
#include <cmath>
#include "point.h"
#include "velocity.h"

template <class T>
class Particle {
    void calRadius() { mRadius = std::log( mMass ) * 3; }

public:
    typedef Particle<T>     ParticleT;
    typedef Point<T>        PointT;
    typedef Velocity<T>     VelocityT;

    //Particle(): mPos(), mVelocity(), mMass( 0 ), mRadius( 0 ){}
    Particle( const PointT& p = PointT(), const T& m = 0, const VelocityT& v = VelocityT() ):
        mPos( p ), mMass( m ), mVelocity( v )
    { calRadius(); }

    // Functions
    bool isNearParticle( const ParticleT& p ) const { return true; }

    // Getters
    T getEdgeX( const T& angle = 0 ) const { return mPos.getX() + ( mRadius * std::cos( angle ) ); }
    T getEdgeY( const T& angle = 0 ) const { return mPos.getY() + ( mRadius * std::sin( angle ) ); }

    const PointT& getPos() const { return mPos; }
    const VelocityT& getVelocity() const { return mVelocity; }
    const T& getRadius() const { return mRadius; }
    const T& getMass() const { return mMass; }

    // Unconst Getters
    PointT& getPos(){ return mPos; }
    VelocityT& getVelocity(){ return mVelocity; }

    // Setters
    void setMass( const T& m ){ mMass = std::abs( m ); calRadius(); }
    void setRadius( const T& r ){ mRadius = std::abs( r ); }

private:
    PointT mPos;
    VelocityT mVelocity;
    T mRadius;
    T mMass;
};

template <class T>
class Collision {
    const Particle<T>* mParticle1;
    const Particle<T>* mParticle2;
    T mAngle;
};

template <class T>
class Gravitation {
public:
    static constexpr double G = 6.67e-11;

    typedef Gravitation<T> GravitationT;
    typedef Particle<T> ParticleT;
    typedef Point<T> PointT;

    Gravitation(): mP1( nullptr ), mP2( nullptr ){}
    Gravitation( const ParticleT& p1, const ParticleT& p2 ):
        mP1( &p1 ), mP2( &p2 )
    { calculate(); }

    Gravitation( const GravitationT& ) = delete;
    void operator= ( const GravitationT& ) = delete;

    void operator()( const ParticleT& p1, const ParticleT& p2 ){ mP1 = &p1; mP2 = &p2; }
    T getAccel1() const { return calculate( *mP1, *mP2 ); }
    T getAccel2() const { return calculate( *mP2, *mP1 ); }
    T getAngle1() const { return mP1->getPos().getDirectionToPoint( mP2->getPos() ); }
    T getAngle2() const { return mP2->getPos().getDirectionToPoint( mP1->getPos() ); }

private:
    const ParticleT* mP1;
    const ParticleT* mP2;

    T calculate( const ParticleT& p1, const ParticleT& p2 ) const { return ( G * p1.getMass() ) / p1.getPos().distance( p2.getPos() );  }
};

#endif // PARTICLE_H
