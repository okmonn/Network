#ifndef OKMONN_VECTOR
#define OKMONN_VECTOR

#include <cmath>
#include <cstdint>

namespace okmonn {
	template <typename T>
	struct Vector2 {
	public:
		/*�R���X�g���N�^*/
		Vector2() {
			x = y = T(0);
		}
		/*�R���X�g���N�^
		.X�v�f
		.Y�v�f*/
		Vector2(const T& x, const T& y) :
			x(x), y(y) {}
		/*�R���X�g���N�^
		.�v�f*/
		Vector2(const T& i) {
			(*this) = Vector2(i, i);
		}
		/*�R�s�[�R���X�g���N�^*/
		Vector2(const Vector2&) = default;

	public:
		/*�傫���̎擾
		return �x�N�g���̑傫��*/
		T Dimensions(void) const {
			return std::sqrt(std::pow(x, T(2)) + std::pow(y, T(2)));
		}
		/*���K��
		return 1.0�ȉ��̒l*/
		Vector2 Normalization(void) const {
			return (*this) / Dimensions();
		}

		/*���Z�q�I�[�o�[���[�h*/
	public:
		void operator=(const T& i) {
			x = y = i;
		}
		void operator+=(const T& i) {
			(*this) = Vector2(x + i, y + i);
		}
		void operator-=(const T& i) {
			(*this) = Vector2(x - i, y - i);
		}
		void operator*=(const T& i) {
			(*this) = Vector2(x * i, y * i);
		}
		void operator/=(const T& i) {
			(*this) = Vector2(x / i, y / i);
		}
		void operator=(const Vector2& vec) {
			x = vec.x; y = vec.y;
		}
		void operator+=(const Vector2& vec) {
			(*this) = Vector2(x + vec.x, y + vec.y);
		}
		void operator-=(const Vector2& vec) {
			(*this) = Vector2(x - vec.x, y - vec.y);
		}
		void operator*=(const Vector2& vec) {
			(*this) = Vector2(x * vec.x, y * vec.y);
		}
		void operator/=(const Vector2& vec) {
			(*this) = Vector2(x / vec.x, y / vec.y);
		}
		Vector2 operator+(const T& i) const {
			return Vector2(x + i, y + i);
		}
		Vector2 operator-(const T& i) const {
			return Vector2(x - i, y - i);
		}
		Vector2 operator*(const T& i) const {
			return Vector2(x * i, y * i);
		}
		Vector2 operator/(const T& i) const {
			return Vector2(x / i, y / i);
		}
		Vector2 operator+(const Vector2& vec) const {
			return Vector2(x + vec.x, y + vec.y);
		}
		Vector2 operator-(const Vector2& vec) const {
			return Vector2(x - vec.x, y - vec.y);
		}
		Vector2 operator*(const Vector2& vec) const {
			return Vector2(x * vec.x, y * vec.y);
		}
		Vector2 operator/(const Vector2& vec) const {
			return Vector2(x / vec.x, y / vec.y);
		}
		Vector2 operator-(void) const {
			return Vector2(-x, -y);
		}
		bool operator==(const T& i) const {
			return (x == i && y == i);
		}
		bool operator!=(const T& i) const {
			return !((*this) == i);
		}
		bool operator==(const Vector2& vec) const {
			return (x == vec.x && y == vec.y);
		}
		bool operator!=(const Vector2& vec) const {
			return !((*this) == vec);
		}

	public:
		/*X,Y�v�f*/
		T x, y;
	};
	/*����
	.�x�N�g��1
	.�x�N�g��2
	return ����*/
	template <typename T>
	T Dot(const Vector2<T>& vec1, const Vector2<T>& vec2) {
		return (vec1.x * vec2.x) + (vec1.y * vec2.y);
	}
	/*�O��
	.�x�N�g��1
	.�x�N�g��2
	return �O��*/
	template <typename T>
	T Cross(const Vector2<T>& vec1, const Vector2<T>& vec2) {
		return (vec1.x * vec2.y) - (vec1.y * vec2.x);
	}

	using Vec2 = Vector2<int32_t>;
	using Vec2f = Vector2<float>;

	template <typename T>
	struct Vector3 {
	public:
		/*�R���X�g���N�^*/
		Vector3() {
			x = y = z = T(0);
		}
		/*�R���X�g���N�^
		.X�v�f
		.Y�v�f
		.Z�v�f*/
		Vector3(const T& x, const T& y, const T& z) :
			x(x), y(y), z(z) {}
		/*�R���X�g���N�^
		.�v�f*/
		Vector3(const T& i) {
			(*this) = Vector3(i, i, i);
		}
		/*�R���X�g���N�^
		.Vector2*/
		Vector3(const Vector2<T>& vec) {
			(*this) = Vector3(vec.x, vec.y, T(0));
		}
		/*�R���X�g���N�^
		.Vector2
		.Z�v�f*/
		Vector3(const Vector2<T>& vec, const T& z) {
			(*this) = Vector3(vec.x, vec.y, z);
		}

	public:
		/*�傫���̎擾
		return �x�N�g���̑傫��*/
		T Dimensions(void) const {
			return std::sqrt(std::pow(x, T(2)) + std::pow(y, T(2)) + std::pow(z, T(2)));
		}
		/*���K��
		return 1.0�ȉ��̒l*/
		Vector3 Normalization(void) const {
			return (*this) / Dimensions();
		}

		/*���Z�q�I�[�o�[���[�h*/
	public:
		void operator=(const T& i) {
			x = y = z = i;
		}
		void operator+=(const T& i) {
			(*this) = Vector3(x + i, y + i, z + i);
		}
		void operator-=(const T& i) {
			(*this) = Vector3(x - i, y - i, z - i);
		}
		void operator*=(const T& i) {
			(*this) = Vector3(x * i, y * i, z * i);
		}
		void operator/=(const T& i) {
			(*this) = Vector3(x / i, y / i, z / i);
		}
		void operator=(const Vector2<T>& vec) {
			x = vec.x; y = vec.y;
		}
		void operator+=(const Vector2<T>& vec) {
			(*this) = Vector2<T>(x + vec.x, y + vec.y);
		}
		void operator-=(const Vector2<T>& vec) {
			(*this) = Vector2<T>(x - vec.x, y - vec.y);
		}
		void operator*=(const Vector2<T>& vec) {
			(*this) = Vector2<T>(x * vec.x, y * vec.y);
		}
		void operator/=(const Vector2<T>& vec) {
			(*this) = Vector2<T>(x / vec.x, y / vec.y);
		}
		void operator=(const Vector3& vec) {
			x = vec.x; y = vec.y; z = vec.z;
		}
		void operator+=(const Vector3& vec) {
			(*this) = Vector3(x + vec.x, y + vec.y, z + vec.z);
		}
		void operator-=(const Vector3& vec) {
			(*this) = Vector3(x - vec.x, y - vec.y, z - vec.z);
		}
		void operator*=(const Vector3& vec) {
			(*this) = Vector3(x * vec.x, y * vec.y, z * vec.z);
		}
		void operator/=(const Vector3& vec) {
			(*this) = Vector3(x / vec.x, y / vec.y, z / vec.z);
		}
		Vector3 operator+(const T& i) const {
			return Vector3(x + i, y + i, z + i);
		}
		Vector3 operator-(const T& i) const {
			return Vector3(x - i, y - i, z - i);
		}
		Vector3 operator*(const T& i) const {
			return Vector3(x * i, y * i, z * i);
		}
		Vector3 operator/(const T& i) const {
			return Vector3(x / i, y / i, z / i);
		}
		Vector3 operator+(const Vector2<T>& vec) const {
			return Vector3(x + vec.x, y + vec.y, z);
		}
		Vector3 operator-(const Vector2<T>& vec) const {
			return Vector3(x - vec.x, y - vec.y, z);
		}
		Vector3 operator*(const Vector2<T>& vec) const {
			return Vector3(x * vec.x, y * vec.y, z);
		}
		Vector3 operator/(const Vector2<T>& vec) const {
			return Vector3(x / vec.x, y / vec.y, z);
		}
		Vector3 operator+(const Vector3& vec) const {
			return Vector3(x + vec.x, y + vec.y, z + vec.z);
		}
		Vector3 operator-(const Vector3& vec) const {
			return Vector3(x - vec.x, y - vec.y, z - vec.z);
		}
		Vector3 operator*(const Vector3& vec) const {
			return Vector3(x * vec.x, y * vec.y, z * vec.z);
		}
		Vector3 operator/(const Vector3& vec) const {
			return Vector3(x / vec.x, y / vec.y, z / vec.z);
		}
		Vector3 operator-(void) const {
			return Vector3(-x, -y, -z);
		}
		bool operator==(const T& i) const {
			return (x == i && y == i && z == i);
		}
		bool operator!=(const T& i) const {
			return !((*this) == i);
		}
		bool operator==(const Vector3& vec) const {
			return (x == vec.x && y == vec.y && z == vec.z);
		}
		bool operator!=(const Vector3& vec) const {
			return !((*this) == vec);
		}

	public:
		/*X�v�f*/
		T x;
		/*Y�v�f*/
		T y;
		/*Z�v�f*/
		T z;
	};
	/*����
	.�x�N�g��1
	.�x�N�g��2*/
	template <typename T>
	T Dot(const Vector3<T>& vec1, const Vector3<T>& vec2) {
		return (vec1.x * vec2.x) + (vec1.y * vec2.y) + (vec1.z * vec2.z);
	}
	/*�O��
	.�x�N�g��1
	.�x�N�g��2
	return �O��*/
	template <typename T>
	Vector3<T> Cross(const Vector3<T>& vec1, const Vector3<T>& vec2) {
		return Vector3<T>((vec1.y * vec2.z) - (vec1.z * vec2.y),
			(vec1.z * vec2.x) - (vec1.z * vec2.z),
			(vec1.x * vec2.y) - (vec1.y * vec2.x));
	}

	using Vec3 = Vector3<int32_t>;
	using Vec3f = Vector3<float>;

	template <typename T>
	struct Vector4 {
	public:
		/*�R���X�g���N�^*/
		Vector4() {
			x = y = z = w = T(0);
		}
		/*�R���X�g���N�^
		.X�v�f
		.Y�v�f
		.Z�v�f
		.W�v�f*/
		Vector4(const T& x, const T& y, const T& z, const T& w) :
			x(x), y(y), z(z), w(w) {}
		/*�R���X�g���N�^
		.�v�f*/
		Vector4(const T& i) {
			(*this) = i;
		}
		/*�R���X�g���N�^
		.Vector4*/
		Vector4(const Vector4& vec) {
			(*this) = vec;
		}
		/*�R���X�g���N�^
		.Vector3*/
		Vector4(const Vector3<T>& vec) {
			(*this) = Vector4(vec.x, vec.y, vec.z, T(0));
		}
		/*�R���X�g���N�^
		.Vector3
		.W�v�f*/
		Vector4(const Vector3<T>& vec, const T& w) {
			(*this) = Vector4(vec.x, vec.y, vec.z, w);
		}


		/*���Z�q�I�[�o�[���[�h*/
	public:
		void operator=(const T& i) {
			x = y = z = w = i;
		}
		void operator+=(const T& i) {
			(*this) = Vector4(x + i, y + i, z + i, w + i);
		}
		void operator-=(const T& i) {
			(*this) = Vector4(x - i, y - i, z - i, w - i);
		}
		void operator*=(const T& i) {
			(*this) = Vector4(x * i, y * i, z * i, w * i);
		}
		void operator/=(const T& i) {
			(*this) = Vector4(x / i, y / i, z / i, w / i);
		}
		void operator=(const Vector3<T>& vec) {
			x = vec.x; y = vec.y; z = vec.z;
		}
		void operator+=(const Vector3<T>& vec) {
			(*this) = Vector4(x + vec.x, y + vec.y, z + vec.z, w);
		}
		void operator-=(const Vector3<T>& vec) {
			(*this) = Vector4(x - vec.x, y - vec.y, z - vec.z, w);
		}
		void operator*=(const Vector3<T>& vec) {
			(*this) = Vector4(x * vec.x, y * vec.y, z * vec.z, w);
		}
		void operator/=(const Vector3<T>& vec) {
			(*this) = Vector4(x / vec.x, y / vec.y, z / vec.z, w);
		}
		void operator=(const Vector4& vec) {
			x = vec.x; y = vec.y; z = vec.z; w = vec.w;
		}
		void operator+=(const Vector4& vec) {
			(*this) = Vector4(x + vec.x, y + vec.y, z + vec.z, w + vec.w);
		}
		void operator-=(const Vector4& vec) {
			(*this) = Vector4(x - vec.x, y - vec.y, z - vec.z, w - vec.w);
		}
		void operator*=(const Vector4& vec) {
			(*this) = Vector4(x * vec.x, y * vec.y, z * vec.z, w * vec.w);
		}
		void operator/=(const Vector4& vec) {
			(*this) = Vector4(x / vec.x, y / vec.y, z / vec.z, w / vec.w);
		}
		Vector4 operator+(const T& i) const {
			return Vector4(x + i, y + i, z + i, w + i);
		}
		Vector4 operator-(const T& i) const {
			return Vector4(x - i, y - i, z - i, w - i);
		}
		Vector4 operator*(const T& i) const {
			return Vector4(x * i, y * i, z * i, w * i);
		}
		Vector4 operator/(const T& i) const {
			return Vector4(x / i, y / i, z / i, w / i);
		}
		Vector4 operator+(const Vector3<T>& vec) const {
			return Vector4(x + vec.x, y + vec.y, z + vec.z, w);
		}
		Vector4 operator-(const Vector3<T>& vec) const {
			return Vector4(x - vec.x, y - vec.y, z - vec.z, w);
		}
		Vector4 operator*(const Vector3<T>& vec) const {
			return Vector4(x * vec.x, y * vec.y, z * vec.z, w);
		}
		Vector4 operator/(const Vector3<T>& vec) const {
			return Vector4(x / vec.x, y / vec.y, z / vec.z, w);
		}
		Vector4 operator+(const Vector4& vec) const {
			return Vector4(x + vec.x, y + vec.y, z + vec.z, w + vec.w);
		}
		Vector4 operator-(const Vector4& vec) const {
			return Vector4(x - vec.x, y - vec.y, z - vec.z, w - vec.w);
		}
		Vector4 operator*(const Vector4& vec) const {
			return Vector4(x * vec.x, y * vec.y, z * vec.z, w * vec.w);
		}
		Vector4 operator/(const Vector4& vec) const {
			return Vector4(x / vec.x, y / vec.y, z / vec.z, w / vec.w);
		}
		Vector4 operator-(void) const {
			return Vector4(-x, -y, -z, -w);
		}
		bool operator==(const T& i) const {
			return (x == i && y == i && z == i && w == i);
		}
		bool operator!=(const T& i) const {
			return !((*this) == i);
		}
		bool operator==(const Vector4& vec) const {
			return (x == vec.x && y == vec.y && z == vec.z && w == vec.w);
		}
		bool operator!=(const Vector4& vec) const {
			return !((*this) == vec);
		}

	public:
		/*X�v�f*/
		T x;
		/*Y�v�f*/
		T y;
		/*Z�v�f*/
		T z;
		/*W�v�f*/
		T w;
	};

	using Vec4 = Vector4<std::int32_t>;
	using Vec4f = Vector4<float>;
}

#endif
