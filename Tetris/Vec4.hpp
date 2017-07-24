#ifndef Vec4_h
#define Vec4_h

template<typename T>
struct Vec4
{
public:
    T x, y, z, w;
    
public:
    Vec4()
    : x(T())
    , y(T())
    , z(T())
    , w(T())
    {
    }
    
    constexpr Vec4(T a, T b, T c, T d)
    : x(a)
    , y(b)
    , z(c)
    , w(d)
    {
    }
    
    ~Vec4() = default;
    
    Vec4 operator*(T a)
    {
        return{ x * a, y * a, z * a, w * a };
    }
    
    Vec4 operator+(const Vec4& a)
    {
        return{ x + a.x, y + a.y, z + a.z, w + a.w };
    }
    
    const Vec4 operator+(const Vec4& a) const
    {
        return{ x + a.x, y + a.y, z + a.z, w + a.w };
    }
    
    Vec4& operator+=(Vec4 a)
    {
        x += a.x;
        y += a.y;
        z += a.z;
        w += a.w;
        return *this;
    }
    
    const Vec4& operator+=(Vec4 a) const
    {
        x += a.x;
        y += a.y;
        z += a.z;
        w += a.w;
        return *this;
    }
};

using Vec4i = Vec4<int>;
using Vec4f = Vec4<float>;

#endif /* Vec4_h */
