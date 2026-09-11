#include <Vec3.hpp>
#include <math.h>
#include <iostream>

Vec3::Vec3()
{
    m_x = 0;
    m_y = 0;
    m_z = 0;
}

Vec3::Vec3(float x, float y, float z):m_x(x), m_y(y), m_z(z){}

Vec3 Vec3::added(const Vec3& v)
{
    Vec3 out(v.m_x + m_x, v.m_y + m_y, v.m_z + m_z);

    return out;
}

void Vec3::add(const Vec3& v)
{
    m_x += v.m_x;
    m_y += v.m_y;
    m_z += v.m_z;
}

Vec3 Vec3::scaled(float scale)
{
    Vec3 out(scale*m_x, scale*m_y, scale*m_z);

    return out;
}

void Vec3::scale(float scale)
{
    m_x *= scale;
    m_y *= scale;
    m_z *= scale;
}

float Vec3::dot(const Vec3& v)
{
    return m_x*v.m_x + m_y*v.m_y + m_z*v.m_z;
}

Vec3 Vec3::crossed(const Vec3& v)
{
    Vec3 out(m_y*v.m_z - m_z*v.m_y, 
             m_z*v.m_x - m_x*v.m_z, 
             m_x*v.m_y - m_y*v.m_x);

    return out;
}

void Vec3::cross(const Vec3& v)
{
    m_x = m_y*v.m_z - m_z*v.m_y;
    m_y = m_z*v.m_x - m_x*v.m_z;
    m_z = m_x*v.m_y - m_y*v.m_x;
}

float Vec3::norm(){
    return sqrt(m_x*m_x + m_y*m_y + m_z*m_z);
}

Vec3 Vec3::normalised()
{
    Vec3 out(m_x, m_y, m_z);
    out.scale(1.0f/out.norm());

    return out;
}

void Vec3::normalise()
{
    float normScale = this->norm();
    m_x /= normScale;
    m_y /= normScale;
    m_z /= normScale;
}

float Vec3::getX() const
{
    return m_x;
}

float Vec3::getY() const
{
    return m_y;
}

float Vec3::getZ() const
{
    return m_z;
}

void Vec3::toString()
{
    std::cout << "Vec3 : {" << m_x << ", " << m_y << ", " << m_z << "}" << std::endl;
}