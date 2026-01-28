//#ifndef	QUAT_H
//#define QUAT_H
//
//#include<Vector3.h>
//
//
////r = (cos(theta / 2), n sin(theta / 2))
//
//	/*Quat*/
//	 MVS_ALIGN(16) struct Quat
//	{
//	public:
//		float X;
//		float Y;
//		float Z;
//
//		/*Scalar Value*/
//		float W;
//
//	public:
//		Quat() { }
//
//		Quat(float inX, float inY, float inZ, float InW);
//
//		Quat(float inV);
//
//		Quat(Vector inV, float W);
//
//
//		/*Operators*/
//
//		Quat operator+(const Quat& Other) const;
//		Quat operator+(float FConst) const;
//
//		Quat operator-(const Quat& Other) const;
//		Quat operator-(float FConst) const;
//
//		Quat operator*(const Quat& Other) const;
//		Quat operator*(float FConst) const;
//
//		//Quat operator/(const Quat& Other) const;
//		Quat operator/(float FConst) const;
//
//		float operator|(const Quat& Other) const;
//		Quat& operator=(const Quat& Other);
//
//		bool operator==(const Quat& Other) const;
//		bool operator!=(const Quat& Other) const;
//
//		void Set(float InX, float InY, float InZ, float InW);
//
//		Quat(const Vector & Axis, float Angle);
//
//		//Angle is in Radians
//		//inline Quat QuatFromAxisAngle(const Vector& Axis, float Angle);
//
//		inline Vector QuatToVector()
//		{
//			return Vector(X, Y, Z);
//		}
//
//	};
//
//	/*Implementations*/
//	inline Quat::Quat(float inX, float inY, float inZ, float inW) :X(inX), Y(inY), Z(inZ), W(inW)
//	{}
//
//	inline Quat::Quat(float inV) : X(inV), Y(inV), Z(inV), W(1)
//	{}
//
//	inline Quat::Quat(Vector inV, float inW)
//	{
//		X = inV.X; Y = inV.Y; Z = inV.Z; W = inW;
//	}
//
//	inline Quat Quat::operator+(const Quat& Other) const
//	{
//		return (Quat(X + Other.X, Y + Other.Y, Z + Other.Z, W + Other.W));
//	}
//
//	inline Quat Quat::operator+(float FConst) const
//	{
//		return Quat(X + FConst, Y + FConst, Z + FConst, W);
//	}
//
//	inline Quat Quat::operator-(const Quat& Other) const
//	{
//		return Quat(X - Other.X, Y - Other.Y, Z - Other.Z, W - Other.W);
//	}
//
//	inline Quat Quat::operator-(float FConst) const
//	{
//		return Quat(X - FConst, Y - FConst, Z - FConst, W);
//	}
//	
//	inline Quat Quat::operator*(const Quat & Other) const
//	{
//
//		//fix
//
//		return Quat(X*Other.X, Y*Other.Y, Z*Other.Z,W*Other.W);
//	
//	}
//
//	inline Quat Quat::operator*(float FConst) const
//	{
//		return Quat(X * FConst, Y * FConst, Z * FConst, W*FConst);
//	}
//
//	/*
//	inline Quat Quat::operator/(const Quat & Other) const
//	{
//		return Quat(X/Other.X, Y/Other.Y, Z/Other.Z);
//	}*/
//
//	inline Quat Quat::operator/(float FConst) const
//	{
//		const float Scale = 1.0f / FConst;
//		return Quat(X*Scale, Y*Scale, Z*Scale, W*Scale);
//	}
//
//	inline Quat& Quat::operator=(const Quat& Other)
//	{
//		this->X = Other.X;
//		this->Y = Other.Y;
//		this->Z = Other.Z;
//		this->W = Other.W;
//
//		return *this;
//	}
//
//	inline bool Quat::operator==(const Quat& Other) const
//	{
//		return X == Other.X && Y == Other.Y && Z == Other.Z && W == Other.W;
//	}
//
//	inline bool Quat::operator!=(const Quat& Other) const
//	{
//		return X != Other.X || Y != Other.Y || Z != Other.Z || W != Other.W;
//	}
//
//	inline float Quat::operator|(const Quat& Other) const
//	{
//		return X * Other.X + Y * Other.Y + Z * Other.Z + W * Other.W;
//	}
//
//	inline void Quat::Set(float InX, float InY, float InZ, float InW)
//	{
//		X = InX;
//		Y = InY;
//		Z = InZ;
//		W = InW;
//	}
//
//	inline Quat::Quat(const Vector & Axis, float Angle)
//	{
//		float sinAngle = SMath::Sin(Angle * 0.5f);
//		Vector NormAxis = Axis.GetNormalized();
//
//		W = SMath::Cos(Angle * 0.5f);
//		X = NormAxis.X * sinAngle;
//		Y = NormAxis.Y * sinAngle;
//		Z = NormAxis.Z * sinAngle;
//	}
//
//
//	//FVector FQuat::GetRotationAxis() const
//
//	//FQuat::RotateVector(FVector V) const
//	// V' = V + 2w(Q x V) + (2Q x (Q x V))
//	// refactor:
//	// V' = V + w(2(Q x V)) + (Q x (2(Q x V)))
//	// T = 2(Q x V);
//	// V' = V + w*(T) + (Q x T)
///*
//	FORCEINLINE float FQuat::GetAngle() const
//	{
//		return 2.f * FMath::Acos(W);
//	}*/
//
//#endif 