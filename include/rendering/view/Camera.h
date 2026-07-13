//
// Created by Nutellis on 09-Mar-26.
//

#ifndef ALKYONERENDERENGINE_CAMERA_H
#define ALKYONERENDERENGINE_CAMERA_H

#include "core/PODTypes.h"
#include "math/ParabolaMath.h"
#include "scene/Node.h"

class BoundingBox;
class AxisAlignedBoundingBox;

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

enum ProjectionType {
    ORTHOGRAPHIC,
    PERSPECTIVE
};

struct Plane {
    Float3 normal{};
    float d{};
};


struct Frustrum {
public:

    //BoundingBox * FrustrumBox;
   // AxisAlignedBoundingBox* BoundingBox;

    float nearPlane;
    float farPlane;
    float ratio;
    float fieldOfView;

    std::vector<Plane> planes; //make this array? arent the planes always 6?

    Frustrum();
    ~Frustrum();
   // void CalculateFrustrumCorners(PCameraComponent* Camera, float Offset = 0.0);
   // void CalculateFrustrumPlanes(PCameraComponent* Camera);

};

// should inherit from node
class Camera : protected Node
{
public:

    Camera() = delete;
    // Constructor with vectors
    //Camera(Vector3f up = Vector3f(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

    Camera(Node* parent, Float3 up = Float3(0.0f, 1.0f, 0.0f), float yaw = 0.0f, float pitch = 0.0f);

    Camera(Float3 up, float yaw, float pitch);
    // Constructor with scalar values
    Camera(float upX, float upY, float upZ, float yaw, float pitch);

    ~Camera() override;

    bool Initialize() override;
    void Terminate() override;
    void Update() override;


    void SetProjection(ProjectionType Type);

    [[nodiscard]] Matrix4f GetViewMatrix() const;

    [[nodiscard]] Matrix4f GetProjectionMatrix() const;

    // Calculates the front vector from the Camera's (updated) Euler Angles
    void UpdateCameraVectors();

    //void AdjustPlanesBasedOnObjects(std::vector<AxisAlignedBoundingBox>Objects);
    //void AdjustPlanesBasedOnObjects(TArray<PAxisAlignedBoundingBox*> Objects, PCameraComponent* RCamera);

    void UpdateCamera(uint32 width = 0, uint32 height = 0);
    void SetDefaults();
    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(CameraMovement direction, float deltaTime);

    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);

    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yOffset);

    // should this be here??
   // void SetupShaderCamera(Shader* ActiveShader);

    static Matrix4f LookAt(const Float3& eye, const Float3& center, const Float3& up);
    static Matrix4f Ortho(const float& left, const float& right, const float& bottom, const float& top, const float& zNear,
                   const float& zFar);
    static Matrix4f OrthoCentered(const float& width, const float& height, const float& zNear, const float& zFar);
    static Matrix4f Perspective(const float& fieldOfView, const float& aspectRatio, const float& zNear, const float& zFar);


    Matrix4f view;
    Matrix4f projection;
    Frustrum * frustrum;


    // Camera Attributes
    Float3 front;
    Float3 up;
    Float3 right;
    Float3 worldUp;


    // Euler Angles
    float yaw;
    float pitch;

    // Camera options
    float movementSpeed;
    float mouseSensitivity;

    //degrees
    float zoom;

    //set true for active camera
    bool isActiveCamera;
private:
    const float zNearMin = 0.5f;
    const float zFarMax = 2000.f;
};


#endif //ALKYONERENDERENGINE_CAMERA_H