#pragma once


template <typename Type>
const TVector4<Type> TVector4<Type>::ZERO(0.0f, 0.0f, 0.0f, 0.0f);

template <typename Type>
const TVector4<Type> TVector4<Type>::ONE(1.0f, 1.0f, 1.0f, 1.0f);

template <typename Type>
void TVector4<Type>::Print() const
{
	// std::cout << "X: " << X << "\t" << "Y: " << Y << "\t" << "Z: " << Z << "\n";
}

//Constructors
template <typename Type>
inline TVector4<Type>::TVector4() : X(0), Y(0), Z(0), W(0)
{}

template <typename Type>
inline TVector4<Type>::TVector4(const TVector4<Type> & inVec4) : X(inVec4.X), Y(inVec4.Y), Z(inVec4.Z), W(inVec4.W)
{}

template <typename Type>
inline TVector4<Type>::TVector4(const Type inX, const Type inY, const Type inZ, const Type inW) : X(inX), Y(inY), Z(inZ), W(inW)
{}

template<typename Type>
inline TVector4<Type>::TVector4(const Type inX, const Type inY, const Type inZ) : X(inX), Y(inY), Z(inZ), W(0)
{}

template<typename Type>
template <typename U>
inline  TVector4<Type>::TVector4(const TVector4<U>& Other)
	: X(static_cast<Type>(Other.X)),
	Y(static_cast<Type>(Other.Y)),
	Z(static_cast<Type>(Other.Z)),
	W(static_cast<Type>(Other.W)) {
}

//Vector3
template <typename Type>
template <typename U>
inline TVector4<Type>::TVector4(const TVector3<U> & inVec3, const Type inW) : 
	X(static_cast<Type>(inVec3.X)), Y(static_cast<Type>(inVec3.Y)), Z(static_cast<Type>(inVec3.Z)), W(inW)
{}

//Vector3
template <typename Type>
template <typename U>
inline TVector4<Type>::TVector4(const TVector3<U> & inVec3) :
	X(static_cast<Type>(inVec3.X)), Y(static_cast<Type>(inVec3.Y)), Z(static_cast<Type>(inVec3.Z)), W(0.0f)
{}

//Vector2
template <typename Type>
template <typename U>
inline TVector4<Type>::TVector4(const TVector2<U> & inVec2,const Type inZ, const Type inW) :
	X(static_cast<Type>(inVec2.X)), Y(static_cast<Type>(inVec2.Y)), Z(inZ), W(inW)
{}

template <typename Type>
template <typename U>
inline TVector4<Type>::TVector4(const Type inX, const Type inY, const TVector2<U> & inVec2) :
	X(inX), Y(inY), Z(static_cast<Type>(inVec2.X)), W(static_cast<Type>(inVec2.Y))
{}

template <typename Type>
template <typename U>
inline TVector4<Type>::TVector4(const TVector2<U> & inVec2xy, const TVector2<U> & inVec2zw) :
	X(static_cast<Type>(inVec2xy.X)), Y(static_cast<Type>(inVec2xy.Y)),
	Z(static_cast<Type>(inVec2zw.X)), W(static_cast<Type>(inVec2zw.Y))
{}


//Scalar
template <typename Type>
inline TVector4<Type>::TVector4(Type Scalar) : X(Scalar), Y(Scalar), Z(Scalar), W(Scalar)
{}

template <typename Type>
inline const TVector4<Type> TVector4<Type>::Zeros()
{
	return TVector4<Type>(0, 0, 0, 0);
}

template <typename Type>
inline const TVector4<Type> TVector4<Type>::Ones()
{
	return TVector4<Type>(1, 1, 1, 1);
}

template <typename Type>
inline TVector3<Type> TVector4<Type>::XYZ() const
{
	return TVector3(X, Y, Z);
}

template <typename Type>
inline TVector2<Type> TVector4<Type>::XY() const
{
	return TVector2(X, Y);
}

//Functions
template <typename Type>
inline Type TVector4<Type>::Length() const
{
	return SMath::Sqrt((X*X) + (Y*Y) + (Z*Z) + (W*W));
}

template <typename Type>
inline void TVector4<Type>::Normalize()
{
	//SSE EQUIVALENT FIX

	float invLength = 1 / Length();
	X = X * invLength;
	Y = Y * invLength;
	Z = Z * invLength;
	W = W * invLength;
}

template <typename Type>
inline TVector4<Type> TVector4<Type>::GetNormalized() const
{
	TVector4<Type> Result(*this);
	Result.Normalize();
	return Result;
}


template <typename Type>
inline void TVector4<Type>::Set(const Type InX, const Type InY, const Type InZ, const Type inW)
{
	X = InX;
	Y = InY;
	Z = InZ;
	W = inW;
}

template<typename Type>
inline size_t TVector4<Type>::Hash()
{
	return std::hash<TVector4<Type>>(&this);
}


//Operators + Cross/Dot products
template <typename Type>
inline Type & TVector4<Type>::operator[](int32 i)
{
	assert(i >= 0 && i < 4);
	return  (&X)[i];
}

template <typename Type>
inline Type const & TVector4<Type>::operator[](int32 i) const
{
	assert(i >= 0 && i < 4);
	return  (&X)[i];
}

template <typename Type>
inline TVector4<Type> TVector4<Type>::operator+(const TVector4<Type> & Other) const
{
	return TVector4<Type>(X + Other.X, Y + Other.Y, Z + Other.Z, W + Other.W);
}

template <typename Type>
inline TVector4<Type> TVector4<Type>::operator+(const Type Scalar) const
{
	return TVector4<Type>(X + Scalar, Y + Scalar, Z + Scalar, W + Scalar);
}

template <typename Type>
inline TVector4<Type> TVector4<Type>::operator-(const TVector4<Type>& Other) const
{
	return TVector4<Type>(X - Other.X, Y - Other.Y, Z - Other.Z, W - Other.W);
}

template <typename Type>
inline TVector4<Type> TVector4<Type>::operator-(const Type Scalar) const
{
	return TVector4<Type>(X - Scalar, Y - Scalar, Z - Scalar, W - Scalar);
}

template <typename Type>
inline TVector4<Type> TVector4<Type>::operator*(const TVector4<Type> & Other) const
{
	return TVector4<Type>(X*Other.X, Y*Other.Y, Z*Other.Z, W*Other.W);
}

template <typename Type>
inline TVector4<Type> TVector4<Type>::operator*(const Type Scalar) const
{
	return TVector4<Type>(X * Scalar, Y * Scalar, Z * Scalar, W* Scalar);
}

template <typename Type>
inline TVector4<Type> TVector4<Type>::operator/(const TVector4<Type> & Other) const
{
	return TVector4(X / Other.X, Y / Other.Y, Z / Other.Z, W / Other.W);
}

template <typename Type>
inline TVector4<Type> TVector4<Type>::operator/(const Type Scalar) const
{
	return TVector4<Type>(X / Scalar, Y / Scalar, Z / Scalar, W / Scalar);
}

template <typename Type>
inline TVector4<Type> & TVector4<Type>::operator=(const TVector4<Type>& Other)
{
	X = Other.X;
	Y = Other.Y;
	Z = Other.Z;
	W = Other.W;

	return *this;
}

template <typename Type>
inline bool TVector4<Type>::operator==(const TVector4<Type>& Other) const
{
	return X == Other.X && Y == Other.Y && Z == Other.Z && W == Other.W;
}

template <typename Type>
inline bool TVector4<Type>::operator!=(const TVector4<Type>& Other) const
{
	return X != Other.X || Y != Other.Y || Z != Other.Z || W != Other.W;
}


template <typename Type>
TVector4<Type> TVector4<Type>::Cross(const TVector4<Type>& Other) const
{
	return TVector4<Type>(Y*Other.Z - Z * Other.Y,
						 Z*Other.X - X * Other.Z,
						 X*Other.Y - Y * Other.X,
						 W);
}

template <typename Type>
static TVector4<Type> TVector4<Type>::Cross(const TVector4<Type>& A, const TVector4<Type>& B)
{
	return A.Cross(B);
}

//fix
template <typename Type>
inline Type TVector4<Type>::Dot(const TVector4<Type>& Other) const
{
	return(X*Other.X + Y * Other.Y + Z * Other.Z);
}

template <typename Type>
inline Type TVector4<Type>::Dot(const TVector4<Type>& A, const TVector4<Type>& B)
{
	return A.Dot(B);
}

template <typename Type>
inline TVector4<Type> TVector4<Type>::operator - () const
{
	return TVector4(-X, -Y, -Z, -W);
}

template<typename T>
inline const size_t std::hash<TVector4<T>>::operator()(const TVector4<T>& key) const
{
	return std::hash<T>()(key.X) ^ std::hash<T>()(key.Y) ^ std::hash<T>()(key.Z) ^ std::hash<T>()(key.W);
}


/*
//---------------------------------------------------------------------------------
//
//
//							int32 4 element vector (Vector4i)
//
//
//---------------------------------------------------------------------------------

inline Vector4i::Vector4i() : TVector4(10)
{

	//VectorRegister = _mm_load_si128(reinterpret_cast<__m128i *>( &X ) );


}


inline Vector4i::Vector4i(const Vector4i & Other)
{

}


//Operators & Cross/Dot products
inline int32 & Vector4i::operator[](int32 i)
{
	assert(i >= 0 && i < VectorLength);
	//return  static_cast<int32>(&VectorRegister[0]);
}

inline int32 const& Vector4i::operator[](int32 i) const
{
	assert(i >= 0 && i < VectorLength);
	return  (&X)[i];
}*/