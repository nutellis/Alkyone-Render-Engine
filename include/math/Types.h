#pragma once


	template <typename Type> struct TVector4;
	template <typename Type> struct TVector3;
	template <typename Type> struct TVector2;
	template <typename Type> struct TMatrix4;

	//Vector2

	typedef TVector2<float> Vector2f;
	typedef TVector2<int32> Vector2i;
	typedef TVector2<uint8> Vector2b;


	//Vector3 

	typedef TVector3<float> Vector3f;
	typedef TVector3<int32> Vector3i;
	typedef TVector3<uint8> Vector3b;

	//Vector4

	typedef TVector4<float> Vector4f;
	typedef TVector4<int32> Vector4i;
	typedef TVector4<uint8> Vector4b;

	//Matrix4
	typedef TMatrix4<float> Matrix4f;

	//TODO: Add Matrix 3 and 2
