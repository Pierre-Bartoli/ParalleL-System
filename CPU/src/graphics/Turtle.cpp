#include <Turtle.hpp>
#include <math.h>
#include <iostream>

Turtle::Turtle()
{
    m_currentPosition = Vec3{};
    m_rotation        = Mat3{};
    m_rotation.setIdentity();
}

void Turtle::step(float value)
{
    // compute the direction with the rotation matrix
    Vec3 direction = m_rotation.mutliply(Vec3(value, 0, 0));
    // then apply in current positions
    m_currentPosition.add(direction);

    // Store the position
    positions.push_back(m_currentPosition);
}

void Turtle::rotate(float angle)
{
    // compute rotation of axe z
    Mat3 rotationZ{};
    rotationZ.setRotationZ(angle);

    m_rotation.mutliply(rotationZ);
}

void Turtle::savePosition()
{
    positionStack.push(m_currentPosition);
    rotationStack.push(m_rotation);
}

void Turtle::usePosition()
{
    m_currentPosition = positionStack.top();
    positionStack.pop();
    positions.push_back(m_currentPosition); // don't forget to store the position, to reset the last position

    m_rotation = rotationStack.top();
    rotationStack.pop();
}

std::vector<Vec3> Turtle::getPath()
{
    return positions;
}