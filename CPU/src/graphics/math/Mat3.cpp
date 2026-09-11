#include <Mat3.hpp>
#include <math.h>
#include <iostream>

Mat3::Mat3()
{
    for (short i = 0; i < 3; ++i) {
            for (short j = 0; j < 3; ++j) {
                m_data[i][j] = 0;
            }
    }
}

void Mat3::mutliply(const Mat3& a)
{
    float result[3][3] = {0};

    for (short i = 0; i < 3; ++i) {
        for (short j = 0; j < 3; ++j) {
            for (short k = 0; k < 3; ++k) {
                result[i][j] += m_data[i][k] * a.m_data[k][j];
            }
        }
    }

    for (short i = 0; i < 3; ++i) {
        for (short j = 0; j < 3; ++j) {
            m_data[i][j] = result[i][j];
        }
    }
}

Mat3 Mat3::mutliplied(const Mat3& a)
{
    Mat3 result{};

    for (short i = 0; i < 3; ++i) {
        for (short j = 0; j < 3; ++j) {
            for (short k = 0; k < 3; ++k) {
                result.m_data[i][j] += m_data[i][k] * a.m_data[k][j];
            }
        }
    }

    return result;
}

Vec3 Mat3::mutliply(const Vec3& v){
    Vec3 out(m_data[0][0] * v.getX() + m_data[0][1] * v.getY() + m_data[0][2] * v.getZ(),
             m_data[1][0] * v.getX() + m_data[1][1] * v.getY() + m_data[1][2] * v.getZ(),
             m_data[2][0] * v.getX() + m_data[2][1] * v.getY() + m_data[2][2] * v.getZ());

    return out;
}

void Mat3::setIdentity()
{
    for (short i = 0; i < 3; ++i) {
            for (short j = 0; j < 3; ++j) {
                m_data[i][j] = 0;
            }
    }

    m_data[0][0] = 1;
    m_data[1][1] = 1;
    m_data[2][2] = 1;
}

void Mat3::setRotationX(float angle)
{
    setIdentity();
    m_data[1][1] = cosf(angle);
    m_data[1][2] = -sinf(angle);
    m_data[2][1] = sinf(angle);
    m_data[2][2] = cosf(angle);
}

void Mat3::setRotationY(float angle)
{
    setIdentity();
    m_data[0][0] = cosf(angle);
    m_data[0][2] = sinf(angle);
    m_data[2][0] = -sinf(angle);
    m_data[2][2] = cosf(angle);
}

void Mat3::setRotationZ(float angle)
{
    setIdentity();
    m_data[0][0] = cosf(angle);
    m_data[0][1] = -sinf(angle);
    m_data[1][0] = sinf(angle);
    m_data[1][1] = cosf(angle);
}