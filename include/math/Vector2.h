#ifndef	VECTOR2_H
#define VECTOR2_H

#include <math/GenericMath.h>


/*Vector*/

template<typename Type>
struct alignas(4) TVector2
{
public:

	union {
		struct { Type X, Y; };
		struct { Type R, G; };
		struct { Type S, T; };
	};


public:
	TVector2() = default;
	~TVector2() = default;
	TVector2(const TVector2 & inVec2) = default;
	
	TVector2(Type inX, Type inY);

	TVector2(Type Scalar);

	template<typename U>
	TVector2(const TVector3<U> & inVec3);
	explicit TVector2(Type const* Pointer);

	Type Length() const;

	inline void Normalize();
	inline TVector2 GetNormalized() const;

	const size_t Hash() const;

	/*Operators*/
	TVector2 operator+(const TVector2& Other) const;
	TVector2 operator+(Type Scalar) const;

	TVector2 operator-(const TVector2& Other) const;
	TVector2 operator-(Type Scalar) const;

	TVector2 operator*(const TVector2 & Other) const;
	TVector2 operator*(Type Scalar) const;

	TVector2 operator/(const TVector2 & Other) const;
	TVector2 operator/(Type Scalar) const;

	
	template <typename U>
	TVector3<U> Cross(const TVector2 & Other) const;

	template <typename U>
	TVector3<U> Cross(const TVector2 & A, const TVector2 & B);


	Type Dot(const TVector2 & Other) const;
	Type Dot(const TVector2 & A, const TVector2 & B);

	TVector2 & operator=(const TVector2& Other) = default;

	bool operator==(const TVector2& Other) const;
	bool operator!=(const TVector2& Other) const;

	TVector2 operator - () const;

	void Set(Type InX, Type InY);

	Type & operator[](int32 i);
	Type const & operator[](int32 i) const;

};


template<typename T>
struct std::hash<TVector2<T>>
{
	const size_t operator()(const TVector2<T> & key) const;
};


#include <math/Vector2.inl>


#endif 




