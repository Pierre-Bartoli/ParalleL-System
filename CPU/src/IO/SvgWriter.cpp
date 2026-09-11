#include <SvgWriter.hpp>

SvgWriter::SvgWriter(std::ofstream& svgfile):m_svgfile(svgfile)
{}

void SvgWriter::writePath(std::vector<Vec3> positions){

    float minX = positions[0].getX(), 
          minY = positions[0].getY(), 
          maxX = positions[0].getX(), 
          maxY = positions[0].getY();

    for (int i = 0; i < positions.size(); i++)
    {
        float x1 = 0;
        float y1 = 0;

        if(i > 1)
        {
            x1 = positions[i - 1].getX();
            y1 = positions[i - 1].getY();
        }

        float x2 = positions[i].getX();
        float y2 = positions[i].getY();

        m_content << "<line"
            << " x1=\"" << x1 << "\""
            << " y1=\"" << y1 << "\""
            << " x2=\"" << x2 << "\""
            << " y2=\"" << y2 << "\""
            << " stroke-width=\"1\""
            << " stroke=\"oklch(255 255 255)\""
            << "/>"
            << std::endl;

        if(minX > x2)
            minX = x2;
        if(minY > y2)
            minY = y2;
        if(maxX < x2)
            maxX = x2;
        if(maxY < y2)
            maxY = y2;
    }

    end(minX, minY, maxX, maxY);
    
}

void SvgWriter::end(float minX, float minY, float maxX, float maxY){
    m_svgfile << "<svg xmlns=\"http://www.w3.org/2000/svg\" "
            << "viewBox=\""
            << minX << " "
            << minY << " "
            << maxX - minX << " "
            << maxY - minY
            << "\">\n";
    m_svgfile << m_content.str();
    m_svgfile << "</svg>" << std::endl;
}