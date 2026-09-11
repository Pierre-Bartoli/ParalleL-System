#include "Vec3.hpp"

class Mat3
{
public:

    /**
     * Creates a null matrix
     */
    Mat3();

    /**
     * Return a new matrix : this multiplied by a 
     * @param a matrix
     */
    Mat3 mutliplied(const Mat3& a);
    /**
     * Multiply this with matrix a by modifying this
     * @param a matrix
     */
    void mutliply(const Mat3& a);

    /**
     * Multiply this to v
     */
    Vec3 mutliply(const Vec3& v);

    /**
     * Set the matrix to be identity patrix
     */
    void setIdentity();

    /**
     * Set the matrix to be the rotation matrix of an angle x
     * @param angle of the rotation
     */
    void setRotationX(float angle);
    /**
     * Set the matrix to be the rotation matrix of an angle y
     * @param angle of the rotation
     */
    void setRotationY(float angle);
    /**
     * Set the matrix to be the rotation matrix of an angle z
     * @param angle of the rotation
     */
    void setRotationZ(float angle);

private:
    float m_data[3][3];
};