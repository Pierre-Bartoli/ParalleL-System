#include <fstream>
#include <sstream>
#include <vector>
#include "../graphics/math/Vec3.hpp"

class SvgWriter{
public:

    /**
     * Init the SvgWriter to write an svg that draw the path
     */
    SvgWriter(std::ofstream& Svgfile);

    /**
     * Write the path
     */
    void writePath(std::vector<Vec3> positions);

private:

    /**
     * End
     */
    void end(float minX, float minY, float maxX, float maxY);

    std::ofstream& m_svgfile;
    std::ostringstream m_content;
};