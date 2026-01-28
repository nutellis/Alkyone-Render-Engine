#ifndef MATRIX4_H
#define MATRIX4_H

/*
#if defined(_MSC_VER)
#define ALIGNED_(x) __declspec(align(x))
#else
#if defined(__GNUC__)
#define ALIGNED_(x) __attribute__ ((aligned(x)))
#endif
#endif
*/

//FOR SIMD
//2. store the vectors strided in memory(i.e., each column contains one vector).



#include <GenericMath.h>

#include <SSEMath.h>

#include <Types.h>

//Matrix4 struct 4x4 Matrix
template <typename T>
#pragma pack(push, 1)
struct TMatrix4
{

private:
	TVector4<T> M[4];

public:
	TMatrix4();

	TMatrix4(const TMatrix4<T>& Other);

	TMatrix4(T m00, T m01, T m02, T m03, T m10, T m11, T m12, T m13, T m20, T m21, T m22, T m23, T m30, T m31, T m32, T m33);

	TMatrix4(const T Scalar);

	template<typename U>
	TMatrix4(const TVector3<U>& In0, const TVector3<U>& In1, const TVector3<U>& In2, const TVector3<U>& In3);

	template<typename T>
	TMatrix4(const TVector4<T>& In0, const TVector4<T>& In1, const TVector4<T>& In2, const TVector4<T>& In3);

	const TVector4<T>& operator[](uint8 i) const;

	TVector4<T>& operator[](uint8 i);

	TMatrix4 operator*(const TMatrix4& Other) const;

	TMatrix4 operator*(T Scalar) const;

	TVector3<T> operator*(const TVector3<T>& InVector) const;
	TVector4<T> operator*(const TVector4<T>& InVector) const;

	template<typename U>
	TMatrix4 operator*(const TVector4<U>) const;

	TMatrix4 operator+(const TMatrix4 & Other) const;

	//TMatrix4 operator+(const Vector& Other) const;

	TMatrix4 GetTransposed() const;

	//void SetIdentity();

	float Determinant() const;

	TMatrix4 Inverse() const;

	//inline TMatrix4 Translate(const Vector& Translation, const TMatrix4& Destination) const;

	//template<typename T>
	//T * value_ptr
	//(
	//	TMatrix4<T> & mat
	//)
	//{
	//	return (mat[0]);
	//}

public:
	static const TMatrix4 ZERO;

	static const TMatrix4 IDENTITY;

	void Print();
};
#pragma pack(pop) 

#include <../../Source/Matrix4.inl>

#endif //!MATRIX4_H