#include "Object.h"
#include "Camera.h"

Camera* pCamera = nullptr;

Object* pObjectLightBulbTop;
Object* pObjectLightBulbBottom;
Object* pObjectLightBulbBack;
Object* pObjectSolarPanelTop;
Object* pObjectSolarPanelBottom;

Shader* shadowMappingShader;
Shader* shadowMappingDepthShader;
Shader* lampShader;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

int objectNo = 0;

// timing
double deltaTime = 0.0f;    // time between current frame and last frame
double lastFrame = 0.0f;

float fKaValue = 0.2;
float fKdValue = 0.5;
float fnValue = 2;

const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
unsigned int depthMap, depthMapFBO;

void drawLightBulb(glm::vec3 values);
void drawSolarPanel(glm::vec3 values);

int main(int argc, char** argv)
{
    textureCount = 0;

    std::string strFullExeFileName = argv[0];
    std::string strExePath;
    const size_t last_slash_idx = strFullExeFileName.rfind('\\');
    if (std::string::npos != last_slash_idx) {
        strExePath = strFullExeFileName.substr(0, last_slash_idx);
    }

    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Simulator", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewInit();

    // Create camera
    pCamera = new Camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0, 1.0, 5.0));

    pObjectLightBulbTop = new Object(strExePath + "\\3D_Resources\\light_bulb_top.obj", 0, 0, SCR_WIDTH, SCR_HEIGHT);
    pObjectLightBulbTop->GenerateColorTexture(glm::vec3(0.2, 0.2, 0.0), 225, 256);
    pObjectLightBulbBottom = new Object(strExePath + "\\3D_Resources\\light_bulb_bottom.obj", 0, 0, SCR_WIDTH, SCR_HEIGHT);
    pObjectLightBulbBottom->GenerateColorTexture(glm::vec3(0.4, 0.4, 0.4), 225, 256);
    pObjectLightBulbBack = new Object(strExePath + "\\3D_Resources\\light_bulb_back.obj", 0, 0, SCR_WIDTH, SCR_HEIGHT);
    pObjectLightBulbBack->GenerateColorTexture(glm::vec3(0.315, 0.315, 0.315), 225, 256);
    pObjectSolarPanelTop = new Object(strExePath + "\\3D_Resources\\solar_panel_top.obj", 0, 0, SCR_WIDTH, SCR_HEIGHT);
    pObjectSolarPanelTop->GenerateColorTexture(glm::vec3(0.2, 0.2, 0.45), 225, 256);
    pObjectSolarPanelBottom = new Object(strExePath + "\\3D_Resources\\solar_panel_bottom.obj", 0, 0, SCR_WIDTH, SCR_HEIGHT);
    pObjectSolarPanelBottom->GenerateColorTexture(glm::vec3(0.3, 0.3, 0.3), 225, 256);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    shadowMappingShader = new Shader(strExePath + "\\Shading\\ShadowMapping.vs", strExePath + "\\Shading\\ShadowMapping.fs");
    shadowMappingDepthShader = new Shader(strExePath + "\\Shading\\ShadowMappingDepth.vs", strExePath + "\\Shading\\ShadowMappingDepth.fs");
    lampShader = new Shader(strExePath + "\\Shading\\Lamp.vs", strExePath + "\\Shading\\Lamp.fs");

    // configure depth map FBO
    // -----------------------
    glGenFramebuffers(1, &depthMapFBO);
    // create depth texture
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // shader configuration
    // --------------------
    shadowMappingShader->Use();
    shadowMappingShader->SetInt("diffuseTexture", 0);
    shadowMappingShader->SetInt("shadowMap", 1);

    // lighting info
    // -------------
    glm::vec3 lightPos(0.0f, 5.0f, 0.0f);

    glEnable(GL_CULL_FACE);


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        switch (objectNo)
        {
        case 0:
            drawLightBulb(glm::vec3(0.3));
            break;
        case 1:
            drawSolarPanel(glm::vec3(1.0, 3.0, 5.0));
            break;
        default:;
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    delete pCamera;
    delete pObjectLightBulbTop;
    delete pObjectLightBulbBottom;
    delete pObjectLightBulbBack;
    delete pObjectSolarPanelTop;
    delete pObjectSolarPanelBottom;


    glfwTerminate();
    return 0;
}

void drawLightBulb( glm::vec3 values )
{
    const float avg = (values.x + values.y + values.z) / 3;
    const float intensity = avg < 0.5 ? 1.0 - avg : 0.0;

    // lighting info
    // -------------
    glm::vec3 lightPos(0.0f, 1.15f, 0.25f);

    // 1. render depth of scene to texture (from light's perspective)
    glm::mat4 lightProjection, lightView;
    glm::mat4 lightSpaceMatrix;
    float near_plane = 1.0f, far_plane = 10.5f;
    lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    lightSpaceMatrix = lightProjection * lightView;

    // CUBE, SHADOW MAPPING

    // render scene from light's point of view
    shadowMappingDepthShader->Use();
    shadowMappingDepthShader->SetMat4("lightSpaceMatrix", lightSpaceMatrix);

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, pObjectLightBulbBack->getTexture());
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    pObjectLightBulbBack->render(*shadowMappingDepthShader);
    glCullFace(GL_BACK);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, pObjectLightBulbBottom->getTexture());
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    pObjectLightBulbBottom->render(*shadowMappingDepthShader);
    glCullFace(GL_BACK);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // reset viewport
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 2. render scene as normal using the generated depth/shadow map 
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shadowMappingShader->Use();
    glm::mat4 projection = pCamera->GetProjectionMatrix();
    glm::mat4 view = pCamera->GetViewMatrix();
    shadowMappingShader->SetMat4("projection", projection);
    shadowMappingShader->SetMat4("view", view);
    // set light uniforms
    shadowMappingShader->SetVec3("viewPos", pCamera->GetPosition());
    shadowMappingShader->SetVec3("lightPos", lightPos);
    shadowMappingShader->SetMat4("lightSpaceMatrix", lightSpaceMatrix);
    shadowMappingShader->SetFloat("intensity", intensity);
    shadowMappingShader->SetVec3("lightColor", glm::vec3(1.0, 1.0, 0.1));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, pObjectLightBulbBack->getTexture());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glDisable(GL_CULL_FACE);
    pObjectLightBulbBack->render(*shadowMappingShader);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, pObjectLightBulbBottom->getTexture());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glDisable(GL_CULL_FACE);
    pObjectLightBulbBottom->render(*shadowMappingShader);

    // also draw the lamp object
    lampShader->Use();
    lampShader->SetMat4("projection", pCamera->GetProjectionMatrix());
    lampShader->SetMat4("view", pCamera->GetViewMatrix());
    lampShader->SetFloat("intensity", intensity);
    lampShader->SetVec3("lightColor", glm::vec3(1.0, 1.0, 0.1));
    pObjectLightBulbTop->render(*lampShader);
}

void drawSolarPanel(glm::vec3 values)
{
    glm::vec3 triangleEnds[3];
    triangleEnds[0] = glm::vec3(0.0f, 2.0f, 0.25f);
    triangleEnds[1] = glm::vec3(1.0f, 2.0f, 0.25f);
    triangleEnds[2] = glm::vec3(0.5f, 2.0f, 0.75f);

    glm::vec3 objectPos = glm::vec3(0.0f); // Position of the object
    float maxIntensity = 0.0f;
    glm::vec3 brightestLightDir;
    glm::vec3 brightestLightPos;

    for (int i = 0; i < 3; i++) {
        glm::vec3 lightDir = glm::normalize(triangleEnds[i] - objectPos);
        float intensity = values[i] / glm::length(triangleEnds[i] - objectPos);
        if (intensity > maxIntensity) {
            maxIntensity = intensity;
            brightestLightDir = lightDir;
            brightestLightPos = triangleEnds[i];
        }
    }

    // Convert the direction vector to a rotation in degrees about the x and y axes
    glm::vec3 degrees;
    degrees.x = glm::degrees(glm::atan(brightestLightDir.y, brightestLightDir.z));
    degrees.y = glm::degrees(glm::atan(brightestLightDir.x, glm::sqrt(brightestLightDir.y * brightestLightDir.y + brightestLightDir.z * brightestLightDir.z)));
    degrees.z = 0.0f; // No rotation around the z-axis
    // Set the light position to the position of the brightest light source
    glm::vec3 lightPos = brightestLightPos;

    // 1. render depth of scene to texture (from light's perspective)
    glm::mat4 lightProjection, lightView;
    glm::mat4 lightSpaceMatrix;
    float near_plane = 1.0f, far_plane = 10.5f;
    lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    lightSpaceMatrix = lightProjection * lightView;

    // CUBE, SHADOW MAPPING

    // render scene from light's point of view
    shadowMappingDepthShader->Use();
    shadowMappingDepthShader->SetMat4("lightSpaceMatrix", lightSpaceMatrix);

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, pObjectSolarPanelTop->getTexture());
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    pObjectSolarPanelTop->render(*shadowMappingDepthShader, glm::vec3(0.0, 1.3, 0.0), glm::vec3(1.0), degrees);
    glCullFace(GL_BACK);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, pObjectSolarPanelBottom->getTexture());
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    pObjectSolarPanelBottom->render(*shadowMappingDepthShader);
    glCullFace(GL_BACK);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // reset viewport
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 2. render scene as normal using the generated depth/shadow map 
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shadowMappingShader->Use();
    glm::mat4 projection = pCamera->GetProjectionMatrix();
    glm::mat4 view = pCamera->GetViewMatrix();
    shadowMappingShader->SetMat4("projection", projection);
    shadowMappingShader->SetMat4("view", view);
    // set light uniforms
    shadowMappingShader->SetVec3("viewPos", pCamera->GetPosition());
    shadowMappingShader->SetVec3("lightPos", lightPos);
    shadowMappingShader->SetMat4("lightSpaceMatrix", lightSpaceMatrix);
    shadowMappingShader->SetFloat("intensity", 1.0);
    shadowMappingShader->SetVec3("lightColor", glm::vec3(1.0, 1.0, 0.8));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, pObjectSolarPanelTop->getTexture());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glDisable(GL_CULL_FACE);
    pObjectSolarPanelTop->render(*shadowMappingShader, glm::vec3(0.0, 1.3, 0.0), glm::vec3(1.0), degrees);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, pObjectSolarPanelBottom->getTexture());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glDisable(GL_CULL_FACE);
    pObjectSolarPanelBottom->render(*shadowMappingShader);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);    
    
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        pCamera->Reset(width, height);

        objectNo = 0;
        return;
    }
    else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        pCamera->Reset(width, height);

        objectNo = 1;
        return;
    }
    else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        pCamera->Reset(width, height);
        return;
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        pCamera->ProcessKeyboard(FORWARD, (float)deltaTime);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        pCamera->ProcessKeyboard(BACKWARD, (float)deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        pCamera->ProcessKeyboard(LEFT, (float)deltaTime);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        pCamera->ProcessKeyboard(RIGHT, (float)deltaTime);
    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
        pCamera->ProcessKeyboard(UP, (float)deltaTime);
    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
        pCamera->ProcessKeyboard(DOWN, (float)deltaTime);



    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        pCamera->Reset(width, height);

    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    pCamera->Reshape(width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    pCamera->MouseControl((float)xpos, (float)ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yOffset)
{
    pCamera->ProcessMouseScroll((float)yOffset);
}