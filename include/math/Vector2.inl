/*Implementations*/

template <typename Type>
inline TVector2<Type>::TVector2()
{}

template <typename Type>
inline TVector2<Type>::TVector2(Type inX, Type inY) :X(inX), Y(inY)
{}

template <typename Type>
inline  TVector2<Type>::TVector2(Type Scalar) : X(Scalar), Y(Scalar)
{}


template <typename Type>
template <typename U>
inline  TVector2<Type>::TVector2(const TVector3<U> & inVec3) : X((inVec3.X)), Y(static_cast<Type>(inVec3.Y))
{}

template <typename Type>
inline TVector2<Type>::TVector2(Type const *const Pointer)
{
	if (Pointer != nullptr)
	{
		X = static_cast<Type>(Pointer[0]);
		Y = static_cast<Type>(Pointer[1]);
	}
}

template <typename Type>
inline Type  TVector2<Type>::Length() const
{
	return SMath::Sqrt((X*X) + (Y*Y));
}
template <typename Type>
inline void TVector2<Type>::Normalize()
{
	//SSE EQUIVALENT FIX

	Type invLength = 1 / Length();
	X = X * invLength;
	Y = Y * invLength;
}
template <typename Type>
inline TVector2<Type> TVector2<Type>::GetNormalized() const
{
	TVector2 Result(*this);
	Result.Normalize();
	return Result;
}

template<typename Type>
inline const size_t TVector2<Type>::Hash() const
{
	return std::hash<TVector2<Type>>{}(*this);
}

template <typename Type>
inline TVector2<Type> TVector2<Type>::operator+(const TVector2& Other) const
{
	return TVector2(X + Other.X, Y + Other.Y);
}

template <typename Type> 
inline TVector2<Type> TVector2<Type>::operator+(Type FConst) const
{
	return TVector2(X + FConst, Y + FConst);
}

template <typename Type> 
inline TVector2<Type> TVector2<Type>::operator-(const TVector2& Other) const
{
	return TVector2(X - Other.X, Y - Other.Y);
}

template <typename Type> 
inline TVector2<Type> TVector2<Type>::operator-(Type Scalar) const
{
	return TVector2(X - Scalar, Y - Scalar);
}

template <typename Type> 
inline TVector2<Type> TVector2<Type>::operator*(const TVector2 & Other) const
{
	return TVector2(X*Other.X, Y*Other.Y);
}

template <typename Type> 
inline TVector2<Type> TVector2<Type>::operator*(Type Scalar) const
{
	return TVector2(X * Scalar, Y * Scalar);
}

template <typename Type> 
inline TVector2<Type> TVector2<Type>::operator/(const TVector2 & Other) const
{
	return TVector2(X / Other.X, Y / Other.Y);
}

template <typename Type> 
inline TVector2<Type> TVector2<Type>::operator/(Type Scalar) const
{
	return TVector2(X / Scalar, Y / Scalar);
}

template <typename Type>
inline TVector2<Type>& TVector2<Type>::operator=(const TVector2& Other)
{
	this->X = Other.X;
	this->Y = Other.Y;

	return *this;
}

template <typename Type>
inline bool TVector2<Type>::operator==(const TVector2<Type>& Other) const
{
	return X == Other.X && Y == Other.Y;
}

template <typename Type>
inline bool TVector2<Type>::operator!=(const TVector2<Type>& Other) const
{
	return X != Other.X || Y != Other.Y;
}

template <typename Type>
template <typename U>
inline TVector3<U> TVector2<Type>::Cross(const TVector2<Type> & Other) const
{
	return(0, 0, X*Other.Y - Y * Other.X);
}

template <typename Type>
template <typename U>
inline TVector3<U> TVector2<Type>::Cross(const TVector2<Type> & A, const TVector2<Type> & B)
{
	return TVector3<Type>(A, 0).Cross(B);
}


template <typename Type>
inline Type TVector2<Type>::Dot(const TVector2<Type>& Other) const
{
	return(X*Other.X + Y * Other.Y);
}

template <typename Type>
inline Type TVector2<Type>::Dot(const TVector2<Type>& A, const TVector2<Type>& B)
{
	return A.Dot(B);
}

template <typename Type> 
inline TVector2<Type> TVector2<Type>::operator - () const
{
	return TVector2(-X, -Y);
}

template <typename Type>
inline void TVector2<Type>::Set(Type InX, Type InY)
{
	X = InX;
	Y = InY;
}

template <typename Type>
inline Type & TVector2<Type>::operator[](int32 i)
{
	assert(i >= 0 && i < 2);
	return  (&X)[i];
}

template <typename Type>
inline Type const & TVector2<Type>::operator[](int32 i) const
{
	assert(i >= 0 && i < 2);
	return  (&X)[i];
}


template<typename T>
inline const size_t std::hash<TVector2<T>>::operator()(const TVector2<T>& key) const
{
	return std::hash<T>()(key.X) ^ std::hash<T>()(key.Y);
}
