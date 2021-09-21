
#if 0
namespace fwmath
{
/*><><><><><><><><> TYPES <><><><><><><><><><><><><><><><><*/
    template<int N, typename T>
    struct generic_vec;

    template<typename T>
    struct generic_vec<2, T>
    {
        union { T x, r, s; };
        union { T y, g, t; };
    };

    template<typename T>
    struct generic_vec<3, T>
    {
        union { T x, r, s; };
        union { T y, g, t; };
        union { T z, b, p; };
    };

    template<typename T>
    struct generic_vec<4, T>
    {
        union { T x, r, s; };
        union { T y, g, t; };
        union { T z, b, p; };
        union { T w, a, q; };
    };

/*><><><><><><><><> OPERATORS <><><><><><><><><><><><><><><><><*/

    template<int N, typename T>
    constexpr bool operator==(const generic_vec<N, T> &a, const generic_vec<N, T> &b);

    template<int N, typename T>
    constexpr bool operator!=(const generic_vec<N, T> &a, const generic_vec<N, T> &b);

    template<int N, typename T>
    constexpr generic_vec<N, T> operator+(const generic_vec<N, T> &a, const generic_vec<N, T> &b);

    template<int N, typename T>
    constexpr generic_vec<N, T> operator-(const generic_vec<N, T> &a, const generic_vec<N, T> &b);

    template<int N, typename T>
    constexpr generic_vec<N, T> operator*(const generic_vec<N, T> &a, const generic_vec<N, T> &b);

    template<int N, typename T>
    constexpr generic_vec<N, T> operator/(const generic_vec<N, T> &a, const generic_vec<N, T> &b);

    template<int N, typename T>
    constexpr generic_vec<N, T> operator+(const generic_vec<N, T> &a, const T &b);

    template<int N, typename T>
    constexpr generic_vec<N, T> operator-(const generic_vec<N, T> &a, const T &b);

    template<int N, typename T>
    constexpr generic_vec<N, T> operator*(const generic_vec<N, T> &a, const T &b);

    template<int N, typename T>
    constexpr generic_vec<N, T> operator/(const generic_vec<N, T> &a, const T &b);

    template<int N, typename T>
    constexpr generic_vec<N, T> operator-(const generic_vec<N, T> &a);

    template<int N, typename T>
    T length(const generic_vec<N, T> &a);

    template<int N, typename T>
    generic_vec<N, T> norm(const generic_vec<N, T> &a);

    template<int N, typename T>
    constexpr generic_vec<N, T> cross(const generic_vec<N, T> &a, const generic_vec<N, T> &b);

    template<int N, typename T>
    constexpr T dot(const generic_vec<N, T> &a, const generic_vec<N, T> &b);

    template<int N, typename T>
    T &at(generic_vec<N, T> &v, int i);

    template<int N, typename T>
    const T &at(const generic_vec<N, T> &v, int i);

    template<typename E>
    inline generic_vec<3, E> rotate_vec(const generic_vec<3, E> &original, const generic_vec<3, E> &axis, E amount);

/*><><><><><><><><> COMMON TYPEDEFS <><><><><><><><><><><><><><><><><*/

    using vec2  = generic_vec<2, float>;
    using ivec2 = generic_vec<2, int>;
    using vec3  = generic_vec<3, float>;
    using ivec3 = generic_vec<3, int>;
    using vec4  = generic_vec<4, float>;
    using ivec4 = generic_vec<4, int>;
    
    template<int A, int B, typename T>
    using generic_mat = generic_vec<A, generic_vec<B, T>>;

    using mat2   = generic_mat<2, 2, float>;
    using mat2x2 = generic_mat<2, 2, float>;
    using mat2x3 = generic_mat<2, 3, float>;
    using mat2x4 = generic_mat<2, 4, float>;

    using mat3   = generic_mat<3, 3, float>;
    using mat3x2 = generic_mat<3, 2, float>;
    using mat3x3 = generic_mat<3, 3, float>;
    using mat3x4 = generic_mat<3, 4, float>;

    using mat4   = generic_mat<4, 4, float>;
    using mat4x2 = generic_mat<4, 2, float>;
    using mat4x3 = generic_mat<4, 3, float>;
    using mat4x4 = generic_mat<4, 4, float>;

    template<typename T> T iden();

    template<typename E, int N, int M>
    generic_mat<M, N, E> transp(const generic_mat<N, M, E> &m);

    template<int N, int M, typename E>
    generic_mat<M, N, E> operator*(const generic_mat<M, N, E> &a, const generic_mat<N, M, E> &b);

/*><><><><><><><><> GEOMETRIC FUNCTIONS <><><><><><><><><><><><><><><><><*/

    template<typename T>
    constexpr T pi = static_cast<T>(3.14159265358979);

    template<typename E>
    generic_mat<4, 4, E> persp(const E &fov, const E &aspect, const E &near, const E &far);
    
    template<typename E>
    generic_mat<4, 4, E> transl(const generic_vec<3, E> &delta);

    template<typename E>
    generic_mat<4, 4, E> lookat(
        const generic_vec<3, E> &from,
        const generic_vec<3, E> &target,
        const generic_vec<3, E> &up);
}


/*><><><><><><><><> IMPLEMENTATION <><><><><><><><><><><><><><><><><*/
// incomplete, operators on vec2/vec4 are missing, matrix/matrix and matrix/vector mult are missing...
// overall, TODO
#ifdef FWDRAW_IMPL

namespace fwdraw
{

}

namespace fwmath
{
    template<int N, typename T>
    inline constexpr bool operator !=(const generic_vec<N, T> &a, const generic_vec<N, T> &b)
    { return !(a == b); }

    template<typename T>
    inline constexpr bool operator ==(const generic_vec<3, T> &a, const generic_vec<3, T> &b)
    { return a.x == b.x && a.y == b.y && a.z == b.z; }

    template<typename T>
    inline constexpr bool operator ==(const generic_vec<2, T> &a, const generic_vec<2, T> &b)
    { return a.x == b.x && a.y == b.y; }

#define VEC3_OP(O) \
    template<typename T> \
    inline constexpr generic_vec<3, T> operator O(const generic_vec<3, T> &a, const generic_vec<3, T> &b) \
    { return { a.x O b.x, a.y O b.y, a.z O b.z }; }

#define VEC3_OP_LIT(O) \
    template<typename T> \
    inline constexpr generic_vec<3, T> operator O(const generic_vec<3, T> &a, const T &b) \
    { return { a.x O b, a.y O b, a.z O b }; }

    template<typename T>
    inline constexpr generic_vec<3, T> operator -(const generic_vec<3, T> &a)
    { return { -a.x, -a.y, -a.z }; }

    VEC3_OP(+) VEC3_OP_LIT(+)
    VEC3_OP(-) VEC3_OP_LIT(-)
    VEC3_OP(*) VEC3_OP_LIT(*)
    VEC3_OP(/) VEC3_OP_LIT(/)

    template<typename T>
    inline constexpr generic_vec<3, T> cross(const generic_vec<3, T> &a, const generic_vec<3, T> &b)
    {
        return {
            a.y * b.z - b.y * a.z,
            a.z * b.x - b.z * a.x,
            a.x * b.y - b.x * a.y,
        };
    }

    template<typename T>
    inline constexpr T dot(const generic_vec<3, T> &a, const generic_vec<3, T> &b)
    { return a.x * b.x + a.y * b.y + a.z * b.z; }

    template<typename T>
    inline T length(const generic_vec<3, T> &a)
    { return std::sqrt(a.x*a.x + a.y*a.y + a.z*a.z); }

    template<int N, typename T>
    inline generic_vec<N, T> norm(const generic_vec<N, T> &a)
    { return a / length(a); }

    template<int N, typename T>
    inline T &at(generic_vec<N, T> &v, int i) { return *((&v.x) + i); }

    template<int N, typename T>
    inline const T &at(const generic_vec<N, T> &v, int i) { return *((&v.x) + i); }

    template<> inline float iden<float>() { return 1.f; }
    template<> inline vec2 iden<vec2>() { return { 1.f, 0.f }; }
    template<> inline vec3 iden<vec3>() { return { 1.f, 0.f, 0.f }; }
    template<> inline vec4 iden<vec4>() { return { 1.f, 0.f, 0.f, 0.f }; }
    template<> inline mat2 iden<mat2>()
    {
        return {
            {1.f, 0.f},
            {0.f, 1.f},
        };
    }
    template<> inline mat3 iden<mat3>()
    {
        return {
            {1.f, 0.f, 0.f},
            {0.f, 1.f, 0.f},
            {0.f, 0.f, 1.f},
        };
    }
    template<> inline mat4 iden<mat4>()
    {
        return {
            {1.f, 0.f, 0.f, 0.f},
            {0.f, 1.f, 0.f, 0.f},
            {0.f, 0.f, 1.f, 0.f},
            {0.f, 0.f, 0.f, 1.f},
        };
    }

    template<typename E>
    inline generic_mat<4, 4, E> transp(const generic_mat<4, 4, E> &m)
    {
        return {
            { m.x.x, m.y.x, m.z.x, m.w.x },
            { m.x.y, m.y.y, m.z.y, m.w.y },
            { m.x.z, m.y.z, m.z.z, m.w.z },
            { m.x.w, m.y.w, m.z.w, m.w.w },
        };
    }

    template<typename E>
    inline generic_mat<4, 4, E> persp(const E &fov, const E &aspect, const E &near, const E &far)
    {
        float thfov = std::tan(fov * 0.5);
        auto m = iden<generic_mat<4, 4, E>>();
        
        m.x.x = 1 / (aspect * thfov);
        m.y.y = 1 / (1 * thfov);
        m.z.z = -(far + near) / (far - near);
        m.w.z = -(2 * far * near) / (far - near);
        m.z.w = -1;
        m.w.w = 0;
        return m;
    }
    
    template<typename E>
    inline generic_mat<4, 4, E> transl(const generic_vec<3, E> &delta)
    {
        return {
            { 1, 0, 0, 0 }, // delta.x
            { 0, 1, 0, 0 }, // delta.y
            { 0, 0, 1, 0 }, // delta.z
            { delta.x, delta.y, delta.z, 1 },
        };
    }

    template<typename E>
    inline generic_mat<4, 4, E> lookat(
        const generic_vec<3, E> &from,
        const generic_vec<3, E> &target,
        const generic_vec<3, E> &up)
    {
        auto zaxis = norm(from - target);
        auto xaxis = norm(cross(up, zaxis));
        auto yaxis = cross(zaxis, xaxis);

        return generic_mat<4, 4, E>
        {
            xaxis.x,           yaxis.x,           zaxis.x,           0,
            xaxis.y,           yaxis.y,           zaxis.y,           0,
            xaxis.z,           yaxis.z,           zaxis.z,           0,
            dot(xaxis, -from), dot(yaxis, -from), dot(zaxis, -from), 1,
        };
    }

    template<typename E>
    inline generic_mat<4, 4, E> operator*(const generic_mat<4, 4, E> &a, const generic_mat<4, 4, E> &b)
    {
        return generic_mat<4, 4, E>
        {
          { a.x.x * b.x.x + a.x.y * b.y.x + a.x.z * b.z.x + a.x.w * b.w.x,
            a.x.x * b.x.y + a.x.y * b.y.y + a.x.z * b.z.y + a.x.w * b.w.y,
            a.x.x * b.x.z + a.x.y * b.y.z + a.x.z * b.z.z + a.x.w * b.w.z,
            a.x.x * b.x.w + a.x.y * b.y.w + a.x.z * b.z.w + a.x.w * b.w.w, },

          { a.y.x * b.x.x + a.y.y * b.y.x + a.y.z * b.z.x + a.y.w * b.w.x,
            a.y.x * b.x.y + a.y.y * b.y.y + a.y.z * b.z.y + a.y.w * b.w.y,
            a.y.x * b.x.z + a.y.y * b.y.z + a.y.z * b.z.z + a.y.w * b.w.z,
            a.y.x * b.x.w + a.y.y * b.y.w + a.y.z * b.z.w + a.y.w * b.w.w, },

          { a.z.x * b.x.x + a.z.y * b.y.x + a.z.z * b.z.x + a.z.w * b.w.x,
            a.z.x * b.x.y + a.z.y * b.y.y + a.z.z * b.z.y + a.z.w * b.w.y,
            a.z.x * b.x.z + a.z.y * b.y.z + a.z.z * b.z.z + a.z.w * b.w.z,
            a.z.x * b.x.w + a.z.y * b.y.w + a.z.z * b.z.w + a.z.w * b.w.w, },

          { a.w.x * b.x.x + a.w.y * b.y.x + a.w.z * b.z.x + a.w.w * b.w.x,
            a.w.x * b.x.y + a.w.y * b.y.y + a.w.z * b.z.y + a.w.w * b.w.y,
            a.w.x * b.x.z + a.w.y * b.y.z + a.w.z * b.z.z + a.w.w * b.w.z,
            a.w.x * b.x.w + a.w.y * b.y.w + a.w.z * b.z.w + a.w.w * b.w.w, },
        };
    }

    template<typename E>
    inline generic_vec<3, E> rotate_vec(const generic_vec<3, E> &original, const generic_vec<3, E> &axis, E amount)
    {
        return original * std::cos(amount)
             + cross(axis, original) * std::sin(amount)
             + axis * dot(axis, original) * (1 - std::sin(amount));
    }
}

#endif
#endif
