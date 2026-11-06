#include <string>
#include <Kokkos_Core.hpp>

#include <fstream>

class SvgBuffer{

public:

    int defaultLineSize(){
        std::string bufferDefault = "<line x1=\"\" y1=\"\" x2=\"\" y2=\"\" stroke-width=\"1\" stroke=\"oklch(255 255 255)\"/>\n";
        return bufferDefault.length();
    }

    KOKKOS_INLINE_FUNCTION
    int computeBufferSize(float x1, float y1, float x2, float y2, int bufferDefault) const{
        int variableSize = digits(x1) + digits(x2) + digits(y1) + digits(y2);
        return bufferDefault + variableSize;
    }

    KOKKOS_INLINE_FUNCTION
    void writeLine(char* out, float x1, float x2, float y1, float y2) const
    {
        out = write(out, "<line x1=\"");
        out = writeFloat(out, x1);

        out = write(out, "\" y1=\"");
        out = writeFloat(out, y1);

        out = write(out, "\" x2=\"");
        out = writeFloat(out, x2);

        out = write(out, "\" y2=\"");
        out = writeFloat(out, y2);

        out = write(out, "\" stroke-width=\"1\" stroke=\"oklch(255 255 255)\"/>\n");
    }

    void finaliseSvg(auto h_buffer, std::ofstream &file, float minX, float maxX, float minY, float maxY)
    {

        if (file.is_open()) {
            file << "<svg xmlns=\"http://www.w3.org/2000/svg\" "
                << "viewBox=\""
                << minX << " "
                << minY << " "
                << maxX - minX << " "
                << maxY - minY
                << "\">\n";
            file << h_buffer.data();
            file << "</svg>" << std::endl;
            file.close();
        }
    }

private: 

    KOKKOS_INLINE_FUNCTION
    int digits(float value) const {

        double x = static_cast<double>(value);
        bool negative = Kokkos::signbit(x);

        if (negative) x = -x;

        double rounded = Kokkos::floor(x * 10.0 + 0.5);
        double integerPart = Kokkos::floor(rounded/10.0);

        int count = 3;

        while (integerPart >= 10.0) {
            integerPart /= 10.0;
            ++count;
        }

        if (negative) ++count;

        return count;
    }

    KOKKOS_INLINE_FUNCTION
    char* write(char* out, const char* text) const
    {
        while (*text != '\0') {
            *out++ = *text++;
        }

        return out;
    }  

    KOKKOS_INLINE_FUNCTION
    char* writeFloat(char* out, float x) const
    {
        int scaled;

        if (x >= 0.0)
            scaled = static_cast<int>(x * 10.0 + 0.5);
        else
            scaled = static_cast<int>(x * 10.0 - 0.5);

        // Évite "-0.0"
        if (scaled == 0) {
            *out++ = '0';
            *out++ = '.';
            *out++ = '0';
            return out;
        }

        if (scaled < 0) {
            *out++ = '-';
            scaled = -scaled;
        }

        int integerPart = scaled / 10;
        int decimalPart = scaled % 10;

        if (integerPart == 0) {
            *out++ = '0';
        }
        else {
            char tmp[32];
            int n = 0;

            while (integerPart != 0) {
                tmp[n++] = static_cast<char>('0' + integerPart % 10);
                integerPart /= 10;
            }

            for (int i = n - 1; i >= 0; --i) {
                *out++ = tmp[i];
            }
        }

        // Point + exactement 1 chiffre
        *out++ = '.';
        *out++ = static_cast<char>('0' + decimalPart);

        return out;
}
};