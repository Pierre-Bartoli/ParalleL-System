#pragma once

class Vec3
{
public:

    /**
     * Init a zero vector
     */
    Vec3();

    /**
     * Init a vector with the given parameter
     * @param x coordinate
     * @param y coordinate
     * @param z coordinate
     */
    Vec3(float x, float y, float z);

    /**
     * Add to v and return new instance of vector
     * @param v added to this
     */
    Vec3 added(const Vec3& v);
    /**
     * Add this to v by modifying this
     * @param v the vector that is added to this
     */
    void add(const Vec3& v);


    /**
     * Scale by an amount and return new instance of vector
     * @param scale amount 
     */
    Vec3 scaled(float scale);
    /**
     * Scale this by an amount by modifying this
     * @param scale amount
     */
    void scale(float scale);

    /**
     * Make dot product of this to v
     * @param v the vector that is dotted to this
     */
    float dot(const Vec3& v);

    /**
     * Make cross product of this to v and return new instance of vector
     * @param v crossed to this
     */
    Vec3 crossed(const Vec3& v);
    /**
     * Make cross product of this to v by modifying this
     * @param v the vector that is crossed to this
     */
    void cross(const Vec3& v);

    /**
     * Return the norm of this
     */
    float norm();

    /**
     * Return a new vector of this normalised
     */
    Vec3 normalised();
    /**
     * Normalise the vector
     */
    void normalise();

    // Getters
    float getX() const;
    float getY() const;
    float getZ() const;

    void toString();

private:
    float m_x, m_y, m_z;
};