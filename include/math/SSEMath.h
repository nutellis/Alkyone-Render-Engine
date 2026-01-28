#ifndef	SSEMATH_H
#define SSEMATH_H


#include <emmintrin.h>

using VectorRegister = __m128;

//ADDITIONS
#define VectorAddF(VecA, VecB)			_mm_add_ps(VecA,VecB)
#define VectorAddI(VecA, VecB)			_mm_add_epi32(VecA,VecB)


#define VectorSub(VecA, VecB)			_mm_sub_ps(VecA,VecB)
#define VectorMultiply( VecA, VecB )	_mm_mul_ps( VecA, VecB )
#define VectorMultiplyAdd( VecA, VecB, Vec3 )	_mm_add_ps( _mm_mul_ps(VecA, VecB), Vec3 )
#define VectorDivide( VecA, VecB )		_mm_div_ps( VecA, VecB )

#define SHUFFLEMASK(A0,A1,B2,B3) ( (A0) | ((A1)<<2) | ((B2)<<4) | ((B3)<<6) )

#define ReplicateVector(Vec,Index) _mm_shuffle_ps(Vec,Vec, _MM_SHUFFLE(Index,Index,Index,Index))

#define ShuffleSelfVector(Vec,Index0,Index1,Index2,Index3) _mm_shuffle_ps(Vec,Vec, _MM_SHUFFLE(Index0,Index1,Index2,Index3))
#define ShuffleVectors(VecA,VecB,Index0,Index1,Index2,Index3) _mm_shuffle_ps(VecA,VecB, _MM_SHUFFLE(Index0,Index1,Index2,Index3))


#define CreateVector(x,y,z,w)			_mm_set_ps(w,z,y,x) 

#define VectorZero()					_mm_setzero_ps()

#define VectorOne()						_mm_set1_ps(1)

#define VectorSetFloat(F)				_mm_set1_ps(F)

#define VectorMin( VecA, VecB )			_mm_min_ps( VecA, VecB )

#define VectorMax( VecA, VecB )			_mm_max_ps( VecA, VecB )

#define VectorMoveLower(VecA,VecB)		_mm_move_ss( VecA, VecB )
//#define VectorAbs( Vec )				_mm_and_ps(Vec,)

inline void AddMatrix(void *Result,const void *Matrix1, const void *Matrix2) 
{

	const VectorRegister *A = (const VectorRegister *) Matrix1;
	const VectorRegister *B = (const VectorRegister *) Matrix2;
	VectorRegister *R = (VectorRegister *) Result;
	
	R[0] = VectorAddF(A[0], B[0]);
	R[1] = VectorAddF(A[1], B[1]);
	R[2] = VectorAddF(A[2], B[2]);
	R[3] = VectorAddF(A[3], B[3]);

}

inline void MultiplyMatrix(void *Result, const void *Matrix1, const void *Matrix2)
{

	const VectorRegister *A = (const VectorRegister *)Matrix1;
	const VectorRegister *B = (const VectorRegister *)Matrix2;

	VectorRegister *R = (VectorRegister *)Result;
	VectorRegister Temp, R0, R1, R2, R3;

	//First Row
	Temp = VectorMultiply(ReplicateVector(A[0], 0), B[0]);
	Temp = VectorMultiplyAdd(ReplicateVector(A[0], 1), B[1], Temp);
	Temp = VectorMultiplyAdd(ReplicateVector(A[0], 2), B[2], Temp);
	R0 = VectorMultiplyAdd(ReplicateVector(A[0], 3), B[3], Temp);

	//Second Row
	Temp = VectorMultiply(ReplicateVector(A[1], 0), B[0]);
	Temp = VectorMultiplyAdd(ReplicateVector(A[1], 1), B[1], Temp);
	Temp = VectorMultiplyAdd(ReplicateVector(A[1], 2), B[2], Temp);
	R1 = VectorMultiplyAdd(ReplicateVector(A[1], 3), B[3], Temp);

	//Third Row
	Temp = VectorMultiply(ReplicateVector(A[2], 0), B[0]);
	Temp = VectorMultiplyAdd(ReplicateVector(A[2], 1), B[1], Temp);
	Temp = VectorMultiplyAdd(ReplicateVector(A[2], 2), B[2], Temp);
	R2 = VectorMultiplyAdd(ReplicateVector(A[2], 3), B[3], Temp);

	//Fourth Row
	Temp = VectorMultiply(ReplicateVector(A[3], 0), B[0]);
	Temp = VectorMultiplyAdd(ReplicateVector(A[3], 1), B[1], Temp);
	Temp = VectorMultiplyAdd(ReplicateVector(A[3], 2), B[2], Temp);
	R3 = VectorMultiplyAdd(ReplicateVector(A[3], 3), B[3], Temp);

	R[0] = R0;
	R[1] = R1;
	R[2] = R2;
	R[3] = R3;
}

inline void print(__m128 var, const char* text)
{
	float *val = (float*)&var;
	//printf("%s: %f %f %f %f \n", text,
		//val[0], val[1], val[2], val[3]);
}

inline void MultiplyQuat(void *Result, const void *Quat1, const void *Quat2)
{

	const VectorRegister *A = (const VectorRegister *)Quat1;
	const VectorRegister *B = (const VectorRegister *)Quat2;

	VectorRegister *R = (VectorRegister *)Result;
	VectorRegister Temp, Q1, Q2, CrossQ1Q2, Q, W;

	W = VectorMultiply(A[0], B[0]);
	Temp = VectorSub(W,ReplicateVector(W, 2));
	Temp = VectorSub(Temp, ReplicateVector(W, 1));
	Temp = VectorSub(Temp, ReplicateVector(W, 0));
	W = ReplicateVector(Temp, 3);
		
	Q1 = VectorMultiply(ReplicateVector(A[0], 3), B[0]); 
	Q2 = VectorMultiply(ReplicateVector(B[0], 3), A[0]);
	CrossQ1Q2 = VectorSub(
		VectorMultiply(ShuffleSelfVector(A[0],3, 0, 2, 1), ShuffleSelfVector(B[0],3, 1, 0, 2)),
		VectorMultiply(ShuffleSelfVector(A[0], 3, 1, 0, 2), ShuffleSelfVector(B[0], 3, 0, 2, 1))
	);

	Q = VectorAddF(VectorAddF(Q1, Q2), CrossQ1Q2);
	W = VectorMoveLower(ShuffleSelfVector(Q,0,1,2,3),W);
	*R = ShuffleSelfVector(W,0, 1, 2, 3);
}

inline void CrossProduct(void *Result, const void *Vector1,const void *Vector2)
{
	const VectorRegister *A = (const VectorRegister *)Vector1;
	const VectorRegister *B = (const VectorRegister *)Vector2;
	VectorRegister *R = (VectorRegister *)Result;

	*R = VectorSub(
		VectorMultiply(ShuffleSelfVector(A[0], 3, 0, 2, 1), ShuffleSelfVector(B[0], 3, 1, 0, 2)),
		VectorMultiply(ShuffleSelfVector(A[0], 3, 1, 0, 2), ShuffleSelfVector(B[0], 3, 0, 2, 1))
	);
}



#endif 

