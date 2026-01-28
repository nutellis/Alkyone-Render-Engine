
#ifndef	VECTOR4_H
#define VECTOR4_H

#include <GenericMath.h>

/*4 element generic Vector*/
template <typename Type> 

#pragma pack(push, 1)
struct TVector4
{
public:
	union {
		struct {  Type X, Y, Z, W; };
		struct {  Type R, G, B, A; };
		struct {  Type S, T, P, Q; };
	};

public:
	TVector4();

	TVector4(const Type inX, const Type inY, const Type inZ,const Type inW);

	TVector4(const Type inX, const Type inY, const Type inZ);

	TVector4(const TVector4 & inVec4);

	template <typename U>
	TVector4(const TVector4<U>& Other);

	template<typename U>
	TVector4(const TVector3<U> & inVec3, const Type inW);
	
	template<typename U>
	TVector4(const TVector3<U> & inVec3);
	
	template<typename U>
	TVector4(const TVector2<U> & inVec2, const Type inZ, const Type inW);
	
	template<typename U>
	TVector4(const Type inX, const Type inY, const TVector2<U> & inVec2);
	
	template<typename U>
	TVector4(const TVector2<U> & inVec2xy, const TVector2<U> & inVec2zw);

	TVector4(const Type Scalar);

	const TVector4 Zeros();

	const TVector4 Ones();


	TVector3<Type> XYZ() const;

	TVector2<Type> XY() const;

	Type Length() const;

	void Normalize();

	TVector4 GetNormalized() const;

	void Set(const Type InX, const Type InY, const Type InZ, const Type inW);

	size_t Hash();

	//Operators
	Type & operator[](int32 i);

	Type const & operator[](int32 i) const;

	TVector4 operator+(const TVector4 & Other) const;
	TVector4 operator+(const Type Scalar) const;

	TVector4 operator-(const TVector4 & Other) const;
	TVector4 operator-(const Type Scalar) const;

	TVector4 operator*(const TVector4 & Other) const;
	TVector4 operator*(const Type Scalar) const;

	TVector4 operator/(const TVector4 & Other) const;
	TVector4 operator/(const Type Scalar) const;

	//No need for Cross product here
	TVector4 Cross(const TVector4& Other) const;
	static TVector4 Cross(const TVector4&A, const TVector4&B);

	Type Dot(const TVector4 & Other) const;
	Type Dot(const TVector4 & A, const TVector4 & B);

	TVector4& operator=(const TVector4& Other);

	bool operator==(const TVector4& Other) const;
	bool operator!=(const TVector4& Other) const;

	TVector4 operator - () const;

public:
	static const TVector4 ZERO;

	static const TVector4 ONE;

	void Print() const;


};
#pragma pack(pop)

template<typename T>
struct std::hash<TVector4<T>>
{
	const size_t operator()(const TVector4<T> & key) const;
};


/*

/*SSE specific int32 4 element Vector
struct Vector4i : TVector4<int32>
{
	Vector4i();

	Vector4i(const Vector4i & Other);

	int32 & operator[](int32 i);

	int32 const & operator[](int32 i) const;

	inline Vector4i Vector4i::operator+(const Vector4i & Other)
	{
		VectorRegister = _mm_add_epi32(VectorRegister, (const __m128i &)Other);

		std::cout << X << "\t" << Y << "\t" << "\t" << Z << "\t" << W;

	}


private:
	__m128i VectorRegister;



	/*template <typename Type>
	inline TVector4<Type> TVector4<Type>::operator+(const Type Scalar) const
	{
		return TVector4<Type>(X + Scalar, Y + Scalar, Z + Scalar, W + Scalar);
	}


};
*/
#include <math/Vector4.inl>

#endif 


