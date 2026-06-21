#ifndef PMATH_H
#define PMATH_H


#include <math/GenericMath.h>

#include <math/SSEMath.h>

#include <math/Vector2.h>
#include <math/Vector3.h>
#include <math/Vector4.h>

#include <math/Matrix4.h>


/**
	 * Converts radians to degrees.
	 * @param	RadVal			Value in radians.
	 * @return					Value in degrees.
	 */
template<class T>
static  auto RadiansToDegrees(T const& RadVal) -> decltype(RadVal * (180.f / PI))
{
	return RadVal * (180.f / PI);
}

/**
 * Converts degrees to radians.
 * @param	DegVal			Value in degrees.
 * @return					Value in radians.
 */
template<class T>
static  auto DegreesToRadians(T const& DegVal) -> decltype(DegVal * (PI / 180.f))
{
	return DegVal * (PI / 180.f);
}



//colum-major. USE FALSE WHEN CALLING OPENGL.
// ¯\_(ツ)_/¯
template <typename T>
static TMatrix4<T> Translate(const TVector3<T> Translation, TMatrix4<T> const& Source)
{
	TMatrix4<T> Result = Source;
	Result[3] = Source[0] * Translation.X + Source[1] * Translation.Y + Source[2] * Translation.Z + Source[3];

	return Result;
}

template <typename T>
static TMatrix4<T> Translate(const TVector4<T> Translation, TMatrix4<T> const& Source)
{
	TMatrix4<T> Result = Source;
	Result[3][0] += Translation.X;
	Result[3][1] += Translation.Y;
	Result[3][2] += Translation.Z;

	return Result;
}



template <typename T>
static inline TMatrix4<T> Scale(const TVector3<T> Scale,TMatrix4<T> const & Source)
{
	TMatrix4<T> Result = Source;

	Result[0][0] *= Scale.X;
	Result[1][1] *= Scale.Y;
	Result[2][2] *= Scale.Z;

	return Result;

}
template <typename T>
static inline TMatrix4<T> Scale(const float Scale, TMatrix4<T> const & Source)
{
	TMatrix4<T> Result = Source;

	Result[0][0] *= Scale;
	Result[1][1] *= Scale;
	Result[2][2] *= Scale;

	return Result;

}

template <typename T>
static TMatrix4<T> Rotate(TMatrix4<T> const& M, T Angle, TVector3<T> const& InVector)
{
	T a = DegreesToRadians(Angle);
	const T cosTheta = SMath::Cos(a);
	const T sinTheta = SMath::Sin(a);
	const T t = T(1) - cosTheta;
	TMatrix4<T> Rotate = TMatrix4<T>::IDENTITY;
	TMatrix4<T> Result;

	TVector3<T> NormalizedRotationAxis = InVector.GetNormalized();

	Rotate[0][0] = t * NormalizedRotationAxis.X * NormalizedRotationAxis.X + cosTheta;
	Rotate[0][1] = t * NormalizedRotationAxis.X * NormalizedRotationAxis.Y + sinTheta * NormalizedRotationAxis.Z;
	Rotate[0][2] = t * NormalizedRotationAxis.X * NormalizedRotationAxis.Z - sinTheta * NormalizedRotationAxis.Y;

	Rotate[1][0] = t * NormalizedRotationAxis.X * NormalizedRotationAxis.Y - sinTheta * NormalizedRotationAxis.Z;
	Rotate[1][1] = t * NormalizedRotationAxis.Y * NormalizedRotationAxis.Y + cosTheta;
	Rotate[1][2] = t * NormalizedRotationAxis.Y * NormalizedRotationAxis.Z + sinTheta * NormalizedRotationAxis.X;

	Rotate[2][0] = t * NormalizedRotationAxis.X * NormalizedRotationAxis.Z + sinTheta * NormalizedRotationAxis.Y;
	Rotate[2][1] = t * NormalizedRotationAxis.Y * NormalizedRotationAxis.Z - sinTheta * NormalizedRotationAxis.X;
	Rotate[2][2] = t * NormalizedRotationAxis.Z * NormalizedRotationAxis.Z + cosTheta;

	Result[0] = M[0] * Rotate[0][0] + M[1] * Rotate[0][1] + M[2] * Rotate[0][2];
	Result[1] = M[0] * Rotate[1][0] + M[1] * Rotate[1][1] + M[2] * Rotate[1][2];
	Result[2] = M[0] * Rotate[2][0] + M[1] * Rotate[2][1] + M[2] * Rotate[2][2];
	Result[3] = M[3];
	return Result;
}

template <typename T>
static TMatrix4<T> Rotate(TVector3<T> RotationAxis, T Angle, TMatrix4<T> const & Source)
{
	float a = DegreesToRadians(Angle);
	const float cosTheta = SMath::Cos(a);
	const float sinTheta = SMath::Sin(a);
	const float t = 1 - cosTheta;
	TMatrix4<T> Rotate = TMatrix4<T>::IDENTITY;

	TVector3<T> NormalizedRotationAxis = RotationAxis.GetNormalized();

	Rotate[0][0] = t * NormalizedRotationAxis.X * NormalizedRotationAxis.X + cosTheta;
	Rotate[1][0] = t * NormalizedRotationAxis.X * NormalizedRotationAxis.Y + sinTheta * NormalizedRotationAxis.Z;
	Rotate[2][0] = t * NormalizedRotationAxis.X * NormalizedRotationAxis.Z - sinTheta * NormalizedRotationAxis.Y;

	Rotate[0][1] = t * NormalizedRotationAxis.X * NormalizedRotationAxis.Y - sinTheta * NormalizedRotationAxis.Z;
	Rotate[1][1] = t * NormalizedRotationAxis.Y * NormalizedRotationAxis.Y + cosTheta;
	Rotate[2][1] = t * NormalizedRotationAxis.Y * NormalizedRotationAxis.Z + sinTheta * NormalizedRotationAxis.X;

	Rotate[0][2] = t * NormalizedRotationAxis.X * NormalizedRotationAxis.Z + sinTheta * NormalizedRotationAxis.Y;
	Rotate[1][2] = t * NormalizedRotationAxis.Y * NormalizedRotationAxis.Z - sinTheta * NormalizedRotationAxis.X;
	Rotate[2][2] = t * NormalizedRotationAxis.Z * NormalizedRotationAxis.Z + cosTheta;

	return Rotate * Source;
}

template <typename T>
static TMatrix4<T> Rotate(float Angle, TVector3<T> const & InVector)
{
	return Rotate(Matrix4f::IDENTITY, Angle, InVector);
}

template <typename T>
static TMatrix4<T> Rotate(TVector3<T> Rotator, TMatrix4<T> const& Source) {

	TMatrix4<T> RotatorX = Rotate(Float3(1.0f, 0.0f, 0.0f), Rotator.X, TMatrix4<T>::IDENTITY);
	TMatrix4<T> RotatorY = Rotate(Float3(0.0f, 1.0f, 0.0f), Rotator.Y, TMatrix4<T>::IDENTITY);
	TMatrix4<T> RotatorZ = Rotate(Float3(0.0f, 0.0f, 1.0f), Rotator.Z, TMatrix4<T>::IDENTITY);
	return RotatorZ * RotatorY * RotatorX * Source;
}

template <typename T>
static TVector3<T> QuatToEuler(TVector4<T> Quaternion)
{
	TVector3<T> Result = TVector3<T>::ZERO;

	Result.X = atan2(2 * (Quaternion.W * Quaternion.X + Quaternion.Y * Quaternion.Z), 1 - 2 * (Quaternion.X * Quaternion.X + Quaternion.Y * Quaternion.Y));
	float theta = 2 * (Quaternion.W * Quaternion.Y - Quaternion.Z * Quaternion.X);

	if (std::abs(theta) <= 1.0f)
	{
		Result.Y = std::copysign(3.14159265f / 2.0f, theta);
	} else
	{
		Result.Y = asin(theta);
	}
	Result.Z =  atan2(2 *(Quaternion.W * Quaternion.Z + Quaternion.X * Quaternion.Y), 1 - 2 * (Quaternion.Y * Quaternion.Y + Quaternion.Z * Quaternion.Z));

	return Result;
}

template <typename Type>
static TVector4<Type> Normalize(TVector4<Type> inVector)
{
	TVector4<Type> Result = TVector4<Type>::ZERO;

	float invLength = static_cast<Type>(1) / inVector.Length();
	Result.X = inVector.X * invLength;
	Result.Y = inVector.Y * invLength;
	Result.Z = inVector.Z * invLength;
	Result.W = inVector.W * invLength;

	return Result;
}
template <typename Type>
static TVector3<Type> Normalize(TVector3<Type> inVector)
{
	TVector3<Type> Result = TVector3<Type>::ZERO;

	Type invLength = static_cast<Type>(1) / inVector.Length();
	Result.X = inVector.X * invLength;
	Result.Y = inVector.Y * invLength;
	Result.Z = inVector.Z * invLength;

	return Result;
}

template <typename Type>
static TVector2<Type> Normalize(TVector2<Type> inVector)
{
	TVector2<Type> Result = TVector2<Type>::ZERO;

	Type invLength = static_cast<Type>(1) / inVector.Length();
	Result.X = inVector.X * invLength;
	Result.Y = inVector.Y * invLength;

	return Result;
}

template <typename Type>
static TVector3<Type> Cross(const TVector3<Type>& A, const TVector3<Type>& B)
{
	return TVector3<Type>(
		A.Y * B.Z - A.Z * B.Y,
		A.Z * B.X - A.X * B.Z,
		A.X * B.Y - A.Y * B.X
	);
}


template <typename Type>
static Type Dot(const TVector3<Type>& A, const TVector3<Type>& B)
{
	TVector3<Type> Temp = A * B;

	return(Temp.X + Temp.Y + Temp.Z);
}

template <typename Type>
static TMatrix4<Type> Inverse(const TMatrix4<Type>& M)
{
	Type Coef00 = M[2][2] * M[3][3] - M[3][2] * M[2][3];
	Type Coef02 = M[1][2] * M[3][3] - M[3][2] * M[1][3];
	Type Coef03 = M[1][2] * M[2][3] - M[2][2] * M[1][3];

	Type Coef04 = M[2][1] * M[3][3] - M[3][1] * M[2][3];
	Type Coef06 = M[1][1] * M[3][3] - M[3][1] * M[1][3];
	Type Coef07 = M[1][1] * M[2][3] - M[2][1] * M[1][3];

	Type Coef08 = M[2][1] * M[3][2] - M[3][1] * M[2][2];
	Type Coef10 = M[1][1] * M[3][2] - M[3][1] * M[1][2];
	Type Coef11 = M[1][1] * M[2][2] - M[2][1] * M[1][2];

	Type Coef12 = M[2][0] * M[3][3] - M[3][0] * M[2][3];
	Type Coef14 = M[1][0] * M[3][3] - M[3][0] * M[1][3];
	Type Coef15 = M[1][0] * M[2][3] - M[2][0] * M[1][3];

	Type Coef16 = M[2][0] * M[3][2] - M[3][0] * M[2][2];
	Type Coef18 = M[1][0] * M[3][2] - M[3][0] * M[1][2];
	Type Coef19 = M[1][0] * M[2][2] - M[2][0] * M[1][2];

	Type Coef20 = M[2][0] * M[3][1] - M[3][0] * M[2][1];
	Type Coef22 = M[1][0] * M[3][1] - M[3][0] * M[1][1];
	Type Coef23 = M[1][0] * M[2][1] - M[2][0] * M[1][1];

	TVector4<Type> Fac0(Coef00, Coef00, Coef02, Coef03);
	TVector4<Type> Fac1(Coef04, Coef04, Coef06, Coef07);
	TVector4<Type> Fac2(Coef08, Coef08, Coef10, Coef11);
	TVector4<Type> Fac3(Coef12, Coef12, Coef14, Coef15);
	TVector4<Type> Fac4(Coef16, Coef16, Coef18, Coef19);
	TVector4<Type> Fac5(Coef20, Coef20, Coef22, Coef23);

	TVector4<Type> Vec0(M[1][0], M[0][0], M[0][0], M[0][0]);
	TVector4<Type> Vec1(M[1][1], M[0][1], M[0][1], M[0][1]);
	TVector4<Type> Vec2(M[1][2], M[0][2], M[0][2], M[0][2]);
	TVector4<Type> Vec3(M[1][3], M[0][3], M[0][3], M[0][3]);

	TVector4<Type> Inv0(Vec1 * Fac0 - Vec2 * Fac1 + Vec3 * Fac2);
	TVector4<Type> Inv1(Vec0 * Fac0 - Vec2 * Fac3 + Vec3 * Fac4);
	TVector4<Type> Inv2(Vec0 * Fac1 - Vec1 * Fac3 + Vec3 * Fac5);
	TVector4<Type> Inv3(Vec0 * Fac2 - Vec1 * Fac4 + Vec2 * Fac5);

	TVector4<Type> SignA(+1, -1, +1, -1);
	TVector4<Type> SignB(-1, +1, -1, +1);

	TMatrix4<Type> Inverse(Inv0 * SignA, Inv1 * SignB, Inv2 * SignA, Inv3 * SignB);

	TVector4<Type> Row0(Inverse[0][0], Inverse[1][0], Inverse[2][0], Inverse[3][0]);

	TVector4<Type> Dot0(M[0] * Row0);
	Type Dot1 = (Dot0.X + Dot0.Y) + (Dot0.Z + Dot0.W);

	Type OneOverDeterminant = static_cast<Type>(1) / Dot1;

	return Inverse * OneOverDeterminant;
}

static float Lerp(float A, float B, float Alpha)
{
	return A + (B - A) * Alpha;
}

static float RandomFloat() // [0-1]
{
	return float(rand()) / float(RAND_MAX);
}

static TVector2<float> ConcentricSampleDisk()
{
	float RadialDistance, Theta;
	float Uniform1 = RandomFloat();
	float Uniform2 = RandomFloat();

	// Map uniform random numbers to $[-1,1]^2$
	float SampleX = 2 * Uniform1 - 1;
	float SampleY = 2 * Uniform2 - 1;

	// Handle degeneracy at the origin (avoid div by zero)
	if (SampleX == 0.0 && SampleY == 0.0)
	{
		return Float2(0, 0);
	}
	// Map square to $(r,\theta)$
	if (SampleX >= -SampleY)
	{
		if (SampleX > SampleY)
		{ // Handle first region of disk
			RadialDistance = SampleX;
			if (SampleY > 0.0)
				Theta = SampleY / RadialDistance;
			else
				Theta = 8.0f + SampleY / RadialDistance;
		}
		else
		{ // Handle second region of disk
			RadialDistance = SampleY;
			Theta = 2.0f - SampleX / RadialDistance;
		}
	}
	else
	{
		if (SampleX <= SampleY)
		{ // Handle third region of disk
			RadialDistance = -SampleX;
			Theta = 4.0f - SampleY / RadialDistance;
		}
		else
		{ // Handle fourth region of disk
			RadialDistance = -SampleY;
			Theta = 6.0f + SampleX / RadialDistance;
		}
	}
	Theta *= float(PI) / 4.0f;
	return TVector2<float>(RadialDistance * cosf(Theta), RadialDistance * sinf(Theta));
}


static TVector3<float> CosineSampleHemisphere()
{
	TVector3<float> CosineHemisphere(ConcentricSampleDisk(), 0);
	CosineHemisphere.Z = SMath::Sqrt(SMath::Max(0.f, 1.f - CosineHemisphere.X * CosineHemisphere.X - CosineHemisphere.Y * CosineHemisphere.Y));
	return CosineHemisphere;
}

static TVector3<float> UniformSampleHemisphere()
{
	float Uniform1 = RandomFloat();
	float Uniform2 = RandomFloat();

	float Phi = 2.0f * PI * Uniform1;
	float CosTheta = Uniform2;
	float SinTheta = SMath::Sqrt(SMath::Max(0.f, 1.f - CosTheta * CosTheta));

	return TVector3<float>(SinTheta * SMath::Cos(Phi), SinTheta * SMath::Sin(Phi), CosTheta);
}

#endif //!PMATH
