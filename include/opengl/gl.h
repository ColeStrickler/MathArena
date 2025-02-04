#ifndef GLMANAGER_H
#define GLMANAGER_H
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include <string>
#include <iostream>
#include <deque>
#include <functional> // for std::bind
#include "renderer.h"
#define DEFAULT_WINDOW_WIDTH 1200U
#define DEFAULT_WINDOW_HEIGHT 900U
#define DEFAULT_MOVE_SPEED_ 40.0f
#define DEFAULT_MOUSE_SENSITIVITY 0.10f
#define DEFAULT_OPENGL_VERSION_MAJOR 4
#define DEFAULT_OPENGL_VERSION_MINOR 5
#define MOUSE_CLICK_OBJ_SEL_THRESHOLD 10.0f

static float ViewDistance = 2000.0f;

class GLManager;

typedef struct GlStats
{
    float m_FramesPerSecond;
    std::deque<float> m_PrevTimes;
    float m_MemoryUsage;
    float m_DrawCalls;
    float m_GPUMemoryUsage;
    float m_NumTriangles;
}GlStats;



class Camera
{
public:
    Camera(float screen_height, float screen_width, float speed, GLManager *manager);
    // Camera();
    ~Camera();
    void ChangeScreenDimensions(float width, float height);
    void ChangeViewDistance(float distance);
    bool SetMVP(ShaderProgram *prog);
    std::string FetchLog();
    void SetPosition(glm::vec3 pos);
    void DumpLog();
    void ChangeMoveSpeed(float speed);
    glm::vec3 GetPosition() const;
    glm::mat4 GetViewMatrix();
    glm::mat4 GetProjectionMatrix();
    glm::mat4 GetOrthoProjectionMatrix() { return m_Ortho; }
    void CameraHandleMouseMovement(float xoffset, float yoffset);
    void CameraHandleKey_W();
    void CameraHandleKey_A();
    void CameraHandleKey_S();
    void CameraHandleKey_D();
    void CameraHandleKey_SPACE();
    void UpdateCameraVectors();

    float GetScreenWidth() const {return m_ScreenWidth;}
    float GetScreenHeight() const {return m_ScreenHeight;}
private:
    // glm::mat4 m_Model; the model matrix will be set by the rendered objects
    glm::vec3 m_CameraPos;
    glm::vec3 m_CameraFront;
    glm::vec3 m_CameraUp;
    glm::vec3 m_CameraRight;
    glm::vec3 m_WorldUp;
    glm::mat4 m_View;
    glm::mat4 m_Projection;
    glm::mat4 m_Ortho;
    float m_Pitch;
    float m_Yaw;
    float m_MouseSensitivity;
    float m_MoveSpeed;
    float m_ScreenWidth;
    float m_ScreenHeight;
    std::string m_Log;
    GLManager *m_Manager;
};

// This class is used to manage the OpenGL state machine
class GLManager
{
public:
    GLManager();
    ~GLManager();
    static void SetWindowSize(GLFWwindow *window, int width, int height);
    GLFWwindow *GetWindow();
    void SetDepthTesting(bool enable);
    void SetStencilTesting(bool enable);
    bool UpdateCameraMVP(ShaderProgram *prog);
    void RegisterKeyCallback(int key, std::function<void()> callback);
    Camera *GetCamera() { return &m_Camera; }
    static std::unordered_map<int, std::function<void()>> m_KeyCallbacks;
    void CalcDeltaTime()
    {
        float curr = glfwGetTime();
        m_DeltaTime = curr - m_LastTime;
        m_LastTime = curr;
    };
    float GetDeltaTime() { return m_DeltaTime; }

    void PerFrame(); // put computation needing to be handled per frame here
private:
    float m_DeltaTime; // time between each frame rendering
    float m_LastTime;
    Camera m_Camera;
    static GLFWwindow *window;
    static int window_width;
    static int window_height;
    std::string m_glVersion;
    void GLFW_Init();
    void GLAD_Init();
    static void MouseScrollCallback(GLFWwindow *window, double xposIn, double yposIn);
    static void HandleAllKeyCallbacks(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void MouseClickCallback(GLFWwindow *window, int button, int action, int mods);
    void HandleToggleCursorHidden();
    void HandleToggleCursorVisible();
    void HandleShowGlStats();
    void DisplayGlStats();
    void UpdateStats();
    void HandleDisplayProfilerStatistics();
    void HandleDisplayChunkManagementOptions();
    std::unordered_map<int, int> m_KeyPressed;
    bool m_ViewLock;
    bool m_bCursorHidden;
    bool m_bShowGlStats;
    GlStats m_GlStats;


    // mouse pos
    static bool m_FirstMouse;
    static float m_LastX;
    static float m_LastY;
};
#endif