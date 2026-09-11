#include <utility>
#include <stack>
#include <vector>
#include "math/Vec3.hpp"
#include "math/Mat3.hpp"

class Turtle{

public:

    /**
     * Create a turtle, it move throught command, basically only rembembers the last position 
     */
    Turtle();

    /**
     * Make the turtle step
     * @param step distance of the turtle
     */
    void step(float value);
    /**
     * Make rotate the turtle in Z axis
     * @param angle of rotation
     */
    void rotate(float value);  
    /**
     * Stack the last position
     */
    void savePosition();
    /**
     * Put the turtle in the position of the stack and pops it
     */
    void usePosition(); 

    /**
     * Get the path to draw it
     */
    std::vector<Vec3> getPath();

private:

    Vec3 m_currentPosition;
    Mat3 m_rotation;
    std::stack<Vec3> positionStack;
    std::stack<Mat3> rotationStack;

    std::vector<Vec3> positions;

};