#include <Kokkos_Core.hpp>
#include <math.h>
#include <Kokkos_Printf.hpp>
#define _USE_MATH_DEFINES

class Matrix4
{
private:
    float m[16] = {0};

public:
    
    KOKKOS_INLINE_FUNCTION
    Matrix4(){
    }

    KOKKOS_INLINE_FUNCTION
    void identity()
    {
        for (int i = 0; i < 16; ++i)
            m[i] = 0.0;

        m[0] = 1.0;
        m[5] = 1.0;
        m[10] = 1.0;
        m[15] = 1.0;
    }

    KOKKOS_INLINE_FUNCTION
    Matrix4 multiply(const Matrix4& B) const
    {
        Matrix4 C{};
        for (int i = 0; i < 16; ++i)
            C.m[i] = 0.0;

        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {

                float sum = 0.0;

                for (int k = 0; k < 4; ++k) {
                    sum += m[4 * i + k]
                        * B.m[4 * k + j];
                }

                C.m[4 * i + j] = sum;
            }
        }

        return C;
    }

    KOKKOS_INLINE_FUNCTION
    void setTransform(char symbol, float step, float rotation)
    {

        identity();

        switch (symbol)
        {
        case 'F':
            
            m[3] = step;
            break;
        
        case '+':
            m[0] = cosf(rotation);
            m[1] = -sinf(rotation);
            m[4] = sinf(rotation);
            m[5] = cosf(rotation);
            break;
        
        case '-':
            m[0] = cosf(-rotation);
            m[1] = -sinf(-rotation);
            m[4] = sinf(-rotation);
            m[5] = cosf(-rotation);
            break;
        
        default:
            break;
        }
    }

    KOKKOS_INLINE_FUNCTION
    float getMatrixAt(int i) const{
        return m[i];
    }
};

struct MatrixScan {

    using value_type = Matrix4;

    Kokkos::View<Matrix4*> input;
    Kokkos::View<Matrix4*> output;

    KOKKOS_INLINE_FUNCTION
    void init(value_type& dst) const {
        dst.identity();
    }

    KOKKOS_INLINE_FUNCTION
    void join(value_type& dst, const value_type& src) const 
    {
        dst = src.multiply(dst);
    }

    KOKKOS_INLINE_FUNCTION
    void operator()(int i, value_type& prefix, bool final) const {
        const Matrix4 current = input(i);

        prefix = prefix.multiply(current);
        
        if (final){
            output(i) = prefix;
        }

    }
};