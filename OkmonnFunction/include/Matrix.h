#ifndef OKMONN_MATRIX
#define OKMONN_MATRIX

#include "Vector.h"

namespace okmonn {
    struct Matrix3x4 {
    public:
        /*コンストラクタ*/
        Matrix3x4() {
            for (std::uint32_t y = 0U; y < 3; ++y) {
                for (std::uint32_t x = 0U; x < 4; ++x) {
                    if (y != x) {
                        m[y][x] = 0.0f;
                    }
                    else {
                        m[y][x] = 1.0f;
                    }
                }
            }
        }
        /*コンストラクタ
        .ベクトル1
        .ベクトル2
        .ベクトル3
        .ベクトル4*/
        Matrix3x4(const Vec3f& vec1, const Vec3f& vec2, const Vec3f& vec3, const Vec3f& vec4) :
            _11(vec1.x), _21(vec2.x), _31(vec3.x), _41(vec4.x),
            _12(vec1.y), _22(vec2.y), _32(vec3.y), _42(vec4.y),
            _13(vec1.z), _23(vec2.z), _33(vec3.z), _43(vec4.z) {}
        /*コンストラクタ
        .各要素*/
        Matrix3x4(const float& _11, const float& _12, const float& _13,
            const float& _21, const float& _22, const float& _23,
            const float& _31, const float& _32, const float& _33,
            const float& _41, const float& _42, const float& _43) :
            _11(_11), _21(_21), _31(_31), _41(_41),
            _12(_12), _22(_22), _32(_32), _42(_42),
            _13(_13), _23(_23), _33(_33), _43(_43) {}

    public:
        /*平行移動行列
        .移動ベクトル*/
        void Translation(const Vec3f& vec) {
            _11 = 1.0f; _21 = 0.0f; _31 = 0.0f; _41 = vec.x;
            _12 = 0.0f; _22 = 1.0f; _32 = 0.0f; _42 = vec.y;
            _13 = 0.0f; _23 = 0.0f; _33 = 1.0f; _43 = vec.z;
        }
        /*拡縮行列
        .拡縮ベクトル*/
        void Scale(const Vec3f& scale) {
            _11 = scale.x; _21 = 0.0f;    _31 = 0.0f;    _41 = 0.0f;
            _12 = 0.0f;    _22 = scale.y; _32 = 0.0f;    _42 = 0.0f;
            _13 = 0.0f;    _23 = 0.0f;    _33 = scale.z; _43 = 0.0f;
        }
        /*X軸回転行列
        .回転ラジアン*/
        void RotationX(const float& rad) {
            _11 = 1.0f; _21 = 0.0f;          _31 = 0.0f;           _41 = 0.0f;
            _12 = 0.0f; _22 = std::cos(rad); _32 = -std::sin(rad); _42 = 0.0f;
            _13 = 0.0f; _23 = std::sin(rad); _33 = std::cos(rad);  _43 = 0.0f;
        }
        /*Y軸回転行列
        .回転ラジアン*/
        void RotationY(const float& rad) {
            _11 = std::cos(rad);  _21 = 0.0f; _31 = std::sin(rad); _41 = 0.0f;
            _12 = 0.0f;           _22 = 1.0f; _32 = 0.0f;          _42 = 0.0f;
            _13 = -std::sin(rad); _23 = 0.0f; _33 = std::cos(rad); _43 = 0.0f;
        }
        /*Z軸回転行列
        .回転ラジアン*/
        void RotationZ(const float& rad) {
            _11 = std::cos(rad); _21 = -std::sin(rad); _31 = 0.0f; _41 = 0.0f;
            _12 = std::sin(rad); _22 = std::cos(rad);  _32 = 0.0f; _42 = 0.0f;
            _13 = 0.0f;          _23 = 0.0f;           _33 = 1.0f; _43 = 0.0f;
        }

        /*演算子オーバーロード*/
    public:
        void operator*=(const Matrix3x4& mat) {
            for (std::uint32_t y = 0U; y < 3; ++y) {
                for (std::uint32_t x = 0U; x < 3; ++x) {
                    for (std::uint32_t i = 0U; i < 3; ++i) {
                        m[y][x] += m[y][i] * mat.m[i][x];
                    }
                }
            }
            for (std::uint32_t i = 0U; i < 3; ++i) {
                m[i][3] = m[i][3] + mat.m[i][3];
            }
        }
        Matrix3x4 operator*(const Matrix3x4& mat) const {
            Matrix3x4 tmp;
            for (std::uint32_t y = 0U; y < 3; ++y) {
                for (std::uint32_t x = 0U; x < 3; ++x) {
                    tmp.m[y][x] = 0.0f;
                    for (std::uint32_t i = 0U; i < 3; ++i) {
                        tmp.m[y][x] += m[y][i] * mat.m[i][x];
                    }
                }
            }
            for (std::uint32_t i = 0U; i < 3; ++i) {
                tmp.m[i][3] = m[i][3] + mat.m[i][3];
            }
            return tmp;
        }

    public:
        /*各要素*/
        union {
            struct {
                float _11, _21, _31, _41;
                float _12, _22, _32, _42;
                float _13, _23, _33, _43;
            };
            float m[3][4];
        };
    };

    struct Matrix4x4 {
    public:
        /*コンストラクタ*/
        Matrix4x4() {
            for (std::uint32_t y = 0U; y < 4; ++y) {
                for (std::uint32_t x = 0U; x < 4; ++x) {
                    if (y != x) {
                        m[y][x] = 0.0f;
                    }
                    else {
                        m[y][x] = 1.0f;
                    }
                }
            }
        }
        /*コンストラクタ
        .ベクトル1
        .ベクトル2
        .ベクトル3
        .ベクトル4*/
        Matrix4x4(const Vec4f& vec1, const Vec4f& vec2, const Vec4f& vec3, const Vec4f& vec4) :
            _11(vec1.x), _21(vec2.x), _31(vec3.x), _41(vec4.x),
            _12(vec1.y), _22(vec2.y), _32(vec3.y), _42(vec4.y),
            _13(vec1.z), _23(vec2.z), _33(vec3.z), _43(vec4.z),
            _14(vec1.w), _24(vec2.w), _34(vec3.w), _44(vec4.w) {}
        /*コンストラクタ
        .各要素*/
        Matrix4x4(const float& _11, const float& _12, const float& _13, const float& _14,
            const float& _21, const float& _22, const float& _23, const float& _24,
            const float& _31, const float& _32, const float& _33, const float& _34,
            const float& _41, const float& _42, const float& _43, const float& _44) :
            _11(_11), _21(_21), _31(_31), _41(_41),
            _12(_12), _22(_22), _32(_32), _42(_42),
            _13(_13), _23(_23), _33(_33), _43(_43),
            _14(_14), _24(_24), _34(_34), _44(_44) {}

    public:
        /*平行移動行列
        .移動ベクトル*/
        void Translation(const Vec3f& vec) {
            _11 = 1.0f;  _21 = 0.0f;  _31 = 0.0f;  _41 = 0.0f;
            _12 = 0.0f;  _22 = 1.0f;  _32 = 0.0f;  _42 = 0.0f;
            _13 = 0.0f;  _23 = 0.0f;  _33 = 1.0f;  _43 = 0.0f;
            _14 = vec.x; _24 = vec.y; _34 = vec.z; _44 = 1.0f;
        }
        /*拡縮行列
        .拡縮ベクトル*/
        void Scale(const Vec3f& scale) {
            _11 = scale.x; _21 = 0.0f;    _31 = 0.0f;    _41 = 0.0f;
            _12 = 0.0f;    _22 = scale.y; _32 = 0.0f;    _42 = 0.0f;
            _13 = 0.0f;    _23 = 0.0f;    _33 = scale.z; _43 = 0.0f;
            _14 = 0.0f;    _24 = 0.0f;    _34 = 0.0f;    _44 = 1.0f;
        }
        /*X軸回転行列
        .回転ラジアン*/
        void RotationX(const float& rad) {
            _11 = 1.0f; _21 = 0.0f;           _31 = 0.0f;          _41 = 0.0f;
            _12 = 0.0f; _22 =  std::cos(rad); _32 = std::sin(rad); _42 = 0.0f;
            _13 = 0.0f; _23 = -std::sin(rad); _33 = std::cos(rad); _43 = 0.0f;
            _14 = 0.0f; _24 = 0.0f;           _34 = 0.0f;          _44 = 1.0f;
        }
        /*Y軸回転行列
        .回転ラジアン*/
        void RotationY(const float& rad) {
            _11 = std::cos(rad); _21 = 0.0f; _31 = -std::sin(rad); _41 = 0.0f;
            _12 = 0.0f;          _22 = 1.0f; _32 =  0.0f;          _42 = 0.0f;
            _13 = std::sin(rad); _23 = 0.0f; _33 =  std::cos(rad); _43 = 0.0f;
            _14 = 0.0f;          _24 = 0.0f; _34 =  0.0f;          _44 = 1.0f;
        }
        /*Z軸回転行列
        .回転ラジアン*/
        void RotationZ(const float& rad) {
            _11 =  std::cos(rad); _21 = std::sin(rad); _31 = 0.0f; _41 = 0.0f;
            _12 = -std::sin(rad); _22 = std::cos(rad); _32 = 0.0f; _42 = 0.0f;
            _13 =  0.0f;          _23 = 0.0f;          _33 = 1.0f; _43 = 0.0f;
            _14 =  0.0f;          _24 = 0.0f;          _34 = 0.0f; _44 = 1.0f;
        }

        /*演算子オーバーロード*/
    public:
        Matrix4x4 operator*(const Matrix4x4& mat) {
            Matrix4x4 tmp;
            for (std::uint32_t y = 0U; y < 4; ++y) {
                for (std::uint32_t x = 0U; x < 4; ++x) {
                    tmp.m[y][x] = 0.0f;
                    for (std::uint32_t i = 0U; i < 4; ++i) {
                        tmp.m[y][x] += m[y][i] * mat.m[i][x];
                    }
                }
            }
            return tmp;
        }

    public:
        /*各要素*/
        union {
            struct {
                float _11, _21, _31, _41;
                float _12, _22, _32, _42;
                float _13, _23, _33, _43;
                float _14, _24, _34, _44;
            };
            float m[4][4];
        };
    };
}

#endif
