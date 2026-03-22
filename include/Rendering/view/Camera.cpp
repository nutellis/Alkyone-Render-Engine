//
// Created by Nutellis on 09-Mar-26.
//

#include "rendering/view/Camera.h"

Frustrum::Frustrum()
{
    //FrustrumBox = new BoundingBox();
    //BoundingBox = new AxisAlignedBoundingBox();
    nearPlane = 0.0f;
    farPlane = 0.0f;
    ratio = 0.0f;
    fieldOfView = 0.0f;

}

Frustrum::~Frustrum()
{
    //delete FrustrumBox;
    //delete BoundingBox;
}
// void Frustrum::CalculateFrustrumCorners(Camera* Camera, float Offset)
// {
// 	float TanHalfVFOV = SMath::Tan(DegreesToRadians(FieldOfView) * 0.5f);
//
// 	float HNear = (2 * TanHalfVFOV * NearPlane) + Offset;
// 	float HFar = (2 * TanHalfVFOV * FarPlane) + Offset;
//
// 	float WNear = HNear * Ratio;
// 	float WFar = HFar * Ratio;
//
// 	Vector3f NearCenter = Camera->GetPosition() + (Camera->Front * NearPlane);
// 	Vector3f FarCenter = Camera->GetPosition() + (Camera->Front * FarPlane);
//
// 	Vector3f RightVector = Camera->Right * (WNear * 0.5f);
// 	Vector3f UpVector = Camera->Up * (HNear * 0.5f);
//
// 	std::vector<Vector3f> FrustrumCorners = std::vector<Vector3f>(8);
// 	// Near Plane
// 	FrustrumCorners.push_back(NearCenter + UpVector - RightVector); //top left
// 	FrustrumCorners.push_back(NearCenter + UpVector + RightVector); //top right
// 	FrustrumCorners.push_back(NearCenter - UpVector - RightVector); //bottom left
// 	FrustrumCorners.push_back(NearCenter - UpVector + RightVector); //bottom right
//
// 	RightVector = Camera->Right * (WFar * 0.5f);
// 	UpVector = Camera->Up * (HFar * 0.5f);
//
// 	// Far Plane
// 	FrustrumCorners.push_back(FarCenter + UpVector - RightVector); //top left
// 	FrustrumCorners.push_back(FarCenter + UpVector + RightVector); //top right
// 	FrustrumCorners.push_back(FarCenter - UpVector - RightVector); //bottom left
// 	FrustrumCorners.push_back(FarCenter - UpVector + RightVector); //bottom right
//
// 	//FrustrumBox->CalculateFromCorners(FrustrumCorners);
// 	//BoundingBox->CalculateFromCorners(FrustrumCorners);
// }
//
// void Frustrum::CalculateFrustrumPlanes(Camera* Camera)
// {
// 	auto MakePlane = [](const Vector3f& A, const Vector3f& B, const Vector3f& C)
// 	{
// 		Plane newPlane;
// 		Vector3f normal = Normalize(Cross(B - A, C - A));
// 		newPlane.normal = normal;
// 		newPlane.d = -Dot(normal, A);
//
// 		return newPlane;
// 	};
// 	Planes.clear();
// 	// 1. Left Plane
// 	Planes.push_back(MakePlane(FrustrumBox->GetCorner(NEAR_LEFT_BOTTOM), FrustrumBox->GetCorner(FAR_LEFT_TOP), FrustrumBox->GetCorner(NEAR_LEFT_TOP)));
// 	// 2. Right Plane
// 	Planes.push_back(MakePlane(FrustrumBox->GetCorner(NEAR_RIGHT_TOP), FrustrumBox->GetCorner(FAR_RIGHT_BOTTOM), FrustrumBox->GetCorner(NEAR_RIGHT_BOTTOM)));
// 	// 3. Top Plane
// 	Planes.push_back(MakePlane(FrustrumBox->GetCorner(NEAR_LEFT_TOP), FrustrumBox->GetCorner(FAR_RIGHT_TOP), FrustrumBox->GetCorner(NEAR_RIGHT_TOP)));
// 	// 4. Bottom Plane
// 	Planes.push_back(MakePlane(FrustrumBox->GetCorner(NEAR_RIGHT_BOTTOM), FrustrumBox->GetCorner(FAR_LEFT_BOTTOM), FrustrumBox->GetCorner(NEAR_LEFT_BOTTOM)));
// 	// 5. Near Plane
// 	Planes.push_back(MakePlane(FrustrumBox->GetCorner(NEAR_LEFT_BOTTOM), FrustrumBox->GetCorner(NEAR_RIGHT_TOP), FrustrumBox->GetCorner(NEAR_RIGHT_BOTTOM)));
// 	// 6. Far Plane
// 	Planes.push_back(MakePlane(FrustrumBox->GetCorner(FAR_RIGHT_BOTTOM), FrustrumBox->GetCorner(FAR_LEFT_TOP), FrustrumBox->GetCorner(FAR_LEFT_BOTTOM)));
// }


//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//									CAMERA COMPONENT
//----------------------------------------------------------------------------------------
Camera::Camera(Vector3f up, float yaw, float pitch) : front(Vector3f(0.0f, 0.0f, -1.0f)), right(Vector3f(1.0f, 0.0f, 0.0f)), movementSpeed(1.0f), mouseSensitivity(0.5f), zoom(60.0f)
{
	up = up;
	worldUp = up;

	yaw = yaw;
	pitch = pitch;

	mouseSensitivity = 0.5f;
	movementSpeed = 1.0f;
	zoom = 60.0f;

	frustrum = new Frustrum();
	frustrum->fieldOfView = 60.0f;
	frustrum->nearPlane = 0.5f;
	frustrum->farPlane = 1000.0f;
	frustrum->ratio = 16.0f / 9.0f;

	//RotateCamera(Parent->ObjectRotation.X, Parent->ObjectRotation.Y);

	//UpdateCameraVectors();

	//Frustrum.CalculateFrustrumCorners(this);

}

Camera::Camera(float upX, float upY, float upZ, float yaw, float pitch)
	: front(Vector3f(0.0f, 0.0f, -1.0f)), movementSpeed(1.0f), mouseSensitivity(0.5f), zoom(60.0f)
{
	up = Vector3f(upX, upY, upZ);
	yaw = yaw;
	pitch = pitch;
	// UpdateCameraVectors();
}

Camera::~Camera()
{
}

bool Camera::Initialize()
{
	return true;
}

void Camera::Terminate()
{
}

void Camera::Update()
{
}

void Camera::SetProjection(ProjectionType Type)
{
	if (Type == ORTHOGRAPHIC)
	{
		//Projection = Ortho();
	}
	else if (Type == PERSPECTIVE)
	{
		//Projection == Perspective();
	}
}


Matrix4f Camera::GetViewMatrix() const
{
	return view;
}

Matrix4f Camera::GetProjectionMatrix() const
{
	return projection;
}

void Camera::UpdateCameraVectors()
{
	// float Yaw = DegreesToRadians(Parent->ObjectRotation.Y - 90.0f);
	// float Pitch = DegreesToRadians(Parent->ObjectRotation.X);
	//
	// Front.X = SMath::Cos(Pitch) * SMath::Cos(Yaw);
	// Front.Y = SMath::Sin(Pitch);
	// Front.Z = SMath::Sin(Yaw) * SMath::Cos(Pitch);
	//
	// Front = Normalize(Front);
	// Right = Normalize(Cross(Front,WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	// Up = Normalize(Cross(Right,Front));
}

void Camera::ProcessKeyboard(CameraMovement direction, float deltaTime)
{
	//
	// float velocity = MovementSpeed * deltaTime;
	// if (direction == FORWARD)
	// 	Parent->ObjectPosition += (Front * velocity);
	// if (direction == BACKWARD)
	// 	Parent->ObjectPosition -= (Front * velocity);
	// if (direction == LEFT)
	// 	Parent->ObjectPosition -= (Front.Cross(WorldUp) * velocity);
	// if (direction == RIGHT)
	// 	Parent->ObjectPosition += (Front.Cross(WorldUp) * velocity);
	// if (direction == UP)
	// 	Parent->ObjectPosition += (WorldUp * velocity);
	// if (direction == DOWN)
	// 	Parent->ObjectPosition -= (WorldUp * velocity);


}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
	// xoffset *= MouseSensitivity;
	// yoffset *= MouseSensitivity;
	//
	// float rotationSpeed = 0.4f;
	// xoffset = rotationSpeed * 0.3f * xoffset;
	// yoffset = rotationSpeed * 0.3f * -yoffset;
	//
	// Parent->ObjectRotation.Y += xoffset;
	// Parent->ObjectRotation.X += yoffset;
	//
	// const float pitchLimit = 89.0f;
	// if (constrainPitch) {
	// 	if (Parent->ObjectRotation.X > pitchLimit) Parent->ObjectRotation.X = pitchLimit;
	// 	if (Parent->ObjectRotation.X < -pitchLimit) Parent->ObjectRotation.X = -pitchLimit;
	// }
}

void Camera::ProcessMouseScroll(float yoffset)
{
	zoom -= (float)yoffset;
	if (zoom < 1.0f)
		zoom = 1.0f;
	if (zoom > 45.0f)
		zoom = 45.0f;
}

// void Camera::AdjustPlanesBasedOnObjects(std::vector<AxisAlignedBoundingBox> objects)
// {
// 	if (objects.empty()) {
// 		return;
// 	}
//
// 	// get the union of all objects
// 	AxisAlignedBoundingBox objectUnion = BoundingHelper::UnionAABB(objects);
//
// 	Union->RenderDebugBoundingBox(1, Vector4f(1.0, 0.0f, 0.0f, 0.1f), RCamera->Projection, RCamera->View);
//
// 	float newNearPlane = FLT_MAX;
// 	float newFarPlane = -FLT_MAX;
//
// 	// for (int i = 0; i < 8; ++i) {
// 	//
// 	// 	Vector3f PointToCam = objectUnion.Corners[i] - GetPosition();
// 	// 	float Z = Dot(PointToCam, this->front);
// 	//
// 	// 	// find boundary values
// 	// 	if (Z > newFarPlane) newFarPlane = Z;
// 	// 	if (Z < newNearPlane) newNearPlane = Z;
// 	// }
//
// 	newNearPlane = SMath::Max(newNearPlane, zNearMin);
// 	newFarPlane = SMath::Max(newFarPlane, zNearMin + 1.0f);
//
// 	frustrum->nearPlane = newNearPlane;
// 	frustrum->farPlane = newFarPlane;
//
// 	UpdateCamera();
// }

void Camera::UpdateCamera(uint32 width, uint32 height) {

	if (width != 0 && height != 0) {
		frustrum->ratio = (float)width / (float)height;
	}
	
	UpdateCameraVectors();

	// frustrum->CalculateFrustrumCorners(this);
	// frustrum->CalculateFrustrumPlanes(this);

	projection = Perspective(frustrum->fieldOfView, frustrum->ratio, frustrum->nearPlane, frustrum->farPlane);

	view = LookAt(parent->GetPosition(), parent->GetPosition() + front, worldUp);

}

void Camera::SetDefaults()
{
	const float YAW = 0.0f;
	const float PITCH = 0.0f;
	const float SPEED = 2.5f;
	const float SENSITIVTY = 0.2f;
	const float FOV = 60.0f;
}

Matrix4f Camera::LookAt(const Vector3f& eye, const Vector3f& center, const Vector3f& up = Vector3f(0.0f, 1.0f, 0.0f))
{
	Vector3f F = Normalize(center - eye);
	Vector3f R = Normalize(Cross(F, up));
	Vector3f U = (Cross(R,F));

	Matrix4f Result = Matrix4f::IDENTITY;

	Result[0][0] = R.X;
	Result[1][0] = R.Y;
	Result[2][0] = R.Z;

	Result[0][1] = U.X;
	Result[1][1] = U.Y;
	Result[2][1] = U.Z;

	Result[0][2] = -F.X;
	Result[1][2] = -F.Y;
	Result[2][2] = -F.Z;

	Result[3][0] = -Dot(R, eye);
	Result[3][1] = -Dot(U, eye);
	Result[3][2] = Dot(F, eye);

	return Result;
}

Matrix4f Camera::Ortho(
	const float& left, const float& right,
	const float& bottom, const float& top,
	const float& zNear = 0.1f, const float& zFar = 1000.0f
)
{
	Matrix4f Result = Matrix4f::IDENTITY;

	Result[0][0] = 2.0f / (right - left);
	Result[1][1] = 2.0f / (top - bottom);
	Result[2][2] = -2.0f / (zFar - zNear);

	Result[3][0] = -(right + left) / (right - left);
	Result[3][1] = -(top + bottom) / (top - bottom);
	Result[3][2] = - (zFar + zNear) / (zFar - zNear);

	return Result;
}

// Centered ortho
Matrix4f Camera::OrthoCentered(
	const float& width,
	const float& height,
	const float& zNear = 0.1f,
	const float& zFar = 100.0f
)
{
	Matrix4f Result = Matrix4f::IDENTITY;

	Result[0][0] = 2.0f / width;
	Result[1][1] = 2.0f / height;
	Result[2][2] = -2.0f / (zFar - zNear);
	Result[3][2] = - (zFar + zNear) / (zFar - zNear);

	return Result;
}

Matrix4f Camera::Perspective(
	const float& fieldOfView,
	const float& aspectRatio,
	const float& zNear = 0.1f,
	const float& zFar = 100.0f
)
{
	Matrix4f Result = Matrix4f::ZERO;
	if (zNear <= 0) {
		// Handle invalid ZNear value
	}
	else {
		float F = SMath::Tan(DegreesToRadians(fieldOfView) * 0.5f);

		Result[0][0] = 1.0f / (aspectRatio * F);
		Result[1][1] = 1.0f / F;
		Result[2][2] = -(zFar + zNear) / (zFar - zNear);
		Result[2][3] = -1.0f;
		Result[3][2] = -(2.0f * zFar * zNear) / (zFar - zNear);
	}
	return Result;

	//// create Frustrum
	//Matrix4f VPM = Projection * View;

	//CameraFrustrum.LeftPlane = (Vector4f(VPM.GetVector(3))
	//	+ Vector4f(VPM.GetVector(0))
	//	).GetNormalized();  // Left Plane

	//CameraFrustrum.RightPlane = (Vector4f(VPM.GetVector(3))
	//	- Vector4f(VPM.GetVector(0))
	//	).GetNormalized();  // Right Plane

	//CameraFrustrum.BottomPlane = (Vector4f(VPM.GetVector(3))
	//	+ Vector4f(VPM.GetVector(1))
	//	).GetNormalized();  // Bottom Plane

	//CameraFrustrum.TopPlane = (Vector4f(VPM.GetVector(3))
	//	- Vector4f(VPM.GetVector(1))
	//	).GetNormalized();  // Top Plane

	//CameraFrustrum.NearPlane = (Vector4f(VPM.GetVector(3))
	//	+ Vector4f(VPM.GetVector(2))
	//	).GetNormalized();  // Near Plane

	//CameraFrustrum.FarPlane = (Vector4f(VPM.GetVector(3)) -
	//	Vector4f(VPM.GetVector(2))
	//	).GetNormalized(); // Far Plane

}