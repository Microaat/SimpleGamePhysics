#ifndef POINT_H
#define POINT_H

#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <cmath>

#ifndef UNCONST_FN
#define UNCONST_FN(CLASS, FN, TYPE) const_cast<TYPE>( static_cast<const CLASS*>(this)->FN )
#endif

template <class T>
class Point {
    T mX;
    T mY;

public:
    typedef Point<T>            PointT;
    typedef unsigned short int  ushort;

    Point(): mX( 0 ), mY( 0 ){}
    Point( const T& x, const T& y ):
        mX( x ), mY( y ){}
    Point( const PointT & ) = default;

    // Functions
    std::string str() const {
        std::ostringstream ss;
        ss << getX() << ", " << getY();
        return ss.str();
    }

    // This function returns the direction of another point relative to this point.
    T getDirectionToPoint( const PointT& p ) const {
        T run = p.mX - mX; T rise = p.mY - mY;
        T angle = std::atan( rise / run ) * 180.0 / std::atan(-1);

        return ( rise > 0 )?
                  ( run > 0 )? angle : 180 + angle
                  : ( run < 0 )? 360 + angle : 270 - angle;
    }

    // Getters
    const T& getX() const { return mX; }
    const T& getY() const { return mY; }
    T& getX(){ return mX; }
    T& getY(){ return mY; }

    // Setters
    void setX( const T& value ) { mX = value; }
    void setY( const T& value ) { mY = value; }
    void setPos( const T& x, const T& y ){ mX = x; mY = y; }

    // Overloaded Operators
    const T& operator[] ( ushort pos ) const {
        if( pos > 1 ) throw std::out_of_range( "Position is out of range" );
        return ( pos == 0 ) ? getX() : getY();
    }
    T& operator[] ( ushort pos ){ return UNCONST_FN( Point, operator[]( pos ), T& ); }

    // For std::cout
    friend std::ostream & operator<< ( std::ostream & out, const PointT & point ){
        return ( out << '(' << point.str() << ')' );
    }

    T distance( const PointT& p ) const {
        return std::sqrt( std::pow( p.mX - mX, 2 ) + std::pow( p.mY - mY, 2 ) );
    }
};

#endif // POINT_H
