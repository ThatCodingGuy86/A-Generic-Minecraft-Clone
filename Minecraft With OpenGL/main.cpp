#if defined(_WIN32)
#define PLATFORM_NAME "Windows"
#elif defined(_WIN64)
#define PLATFORM_NAME "Windows"
#elif defined(__CYGWIN__) && !defined(_WIN32)
#define PLATFORM_NAME "Windows"
#elif defined(__linux__)
#define PLATFORM_NAME "Linux"
#else
#endif

#if !defined(PLATFORM_NAME)
#error Platform not supported.
#endif



#include "util.h"
#include <string>
#include "perlin_noise.h"
#include <array>
#include "Chunk.h"

// - Camera variables -
glm::vec3 cameraPos = glm::vec3(0.0f, 15.0f, 0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

double yaw = -90.0f;
double pitch = 0.0f;

// Keeps mouse movement outside of the window when created from causing the camera to move
bool firstMouse = true;


// Yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing
// to the right so it's initially rotated a bit to the left
double lastX = SCR_WIDTH / 2.0;
double lastY = SCR_HEIGHT / 2.0;
float fov = 45.0f;

// Time calculation variables
double deltaTime = 0.0f;
double lastFrame = 0.0f;
double FPS = 0.0f;
double previousTime = glfwGetTime();
int frameCount = 0;

// Wireframe mode toggle
bool isLine = false;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // When pressing 1, toggles between wireframe mode and fill mode
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
        isLine = !isLine;
}

int main()
{

    // Initilize GLFW & OpenGL, then create window
    int initReturn = init();
    if (initReturn != 0)
    {
        return initReturn;
    }

    glfwSetKeyCallback(window, key_callback);

    // Build and compile shader
    Shader shader("assets/shaders/shader.vert", "assets/shaders/shader.frag");

    // Initilize text lib
    Text();


    // - Chunk stuff -

    std::vector<Chunk*> chunkArray;

    const int chunkRenderSize = 10;
    size_t chunkArrSize = 0;

    unsigned int chunkVAOs[chunkRenderSize * chunkRenderSize];
    unsigned int chunkVBOs[chunkRenderSize * chunkRenderSize];

    // Initialize chunk VAOs, VBOs, and EBOs
    glGenVertexArrays(chunkRenderSize * chunkRenderSize, chunkVAOs);
    glGenBuffers(chunkRenderSize * chunkRenderSize, chunkVBOs);

    // Generate chunks
    int i = 0;
    for (int y = 0; y < chunkRenderSize; y++)
    {
        for (int x = 0; x < chunkRenderSize; x++)
        {
            // Try/catch for exceptions thrown while generating chunks
            try
            {
                chunkArray.push_back(new Chunk(i, 2353));
            }
            catch(std::exception e)
            {
                std::stringstream error;

                error << "[MAIN_CHUNKGEN/FATAL]: " << e.what() << "\n";
                err(error.str());

                std::cout << "[MAIN_FERR_MSG]: " << "Failed to generate chunk " << i << ".\n";

                return (e.what()[0] + e.what()[1]) * -1;
            }
            

            glBindVertexArray(chunkVAOs[i]);
            glBindBuffer(GL_ARRAY_BUFFER, chunkVBOs[i]);

            std::vector<float> chunkVerts;

            chunkArray[i]->genChunkMesh(chunkVerts, chunkArrSize);


            glBufferData(GL_ARRAY_BUFFER, chunkVerts.size() * sizeof(float), &chunkVerts.front(), GL_STATIC_DRAW);
            
            // XYZ attribute
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            // UV attribute
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);

            // Light attribute
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(5 * sizeof(float)));
            glEnableVertexAttribArray(2);
            i++;
        }
    }


    // - Load assets -

    stbi_set_flip_vertically_on_load(true);

    // Load font atlas
    Texture textAtlas("assets/fonts/font.png", true);

    // Load texture atlas
    Texture texture1("assets/textures/atlas.png", true);

    // Set up shader
    shader.use();
    shader.setInt("texture1", 0);

    // Load text shaders
    Shader textShader("assets/shaders/tShader.vert", "assets/shaders/tShader.frag");


    // - Render loop -
    while (!glfwWindowShouldClose(window))
    {
        // - Input -
        processInput(window);


        // - Render -
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Wireframe mode stuff
        if (isLine)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }


        // Bind texture
        texture1.use(0);

        // Activate shader
        shader.use();

        // Pass projection matrix to shader
        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
        shader.setMat4("projection", projection);

        // Camera/view transformation matrix
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        shader.setMat4("view", view);

        // Set the texture size to the.. well, texture size.. what else did you think it was going to be?
        shader.setVec2("texSize", 256, 256);

        glm::mat4 model = glm::mat4(1.0f); // Make sure to initialize matrix to identity matrix first

        // Render chunks
        int i = 0;
        for (int y = 0; y < chunkRenderSize; y++)
        {
            for (int x = 0; x < chunkRenderSize; x++)
            {
                // Bind the hopefully correct VAO from the chunk VAO's
                glBindVertexArray(chunkVAOs[i]);
                // Set up model matrix
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(x * 16, 0, y * 16));
                shader.setMat4("model", model);

                // Draw chunk
                glDrawArrays(GL_TRIANGLES, 0, (GLsizei)chunkArray[x * chunkRenderSize + y]->chunkMeshSize);
                i++;
            }
        }

        float r = 0;
        float g = 0;
        float b = 0;

        // If FPS < 10, draw FPS as red
        // Else, if FPS < 30, draw FPS as orange
        // Else, if FPS < 50, draw FPS as yellow
        // Else, if FPS >= 51, draw FPS as white

        // Bit convoluted but I can't think of a way to use a switch statement for this that isn't
        // more convoluted


        if (FPS < 10)
        {
            r = 255; g = 0; b = 0;
        }
        else if (FPS < 30)
        {
            r = 255; g = 116; b = 0;
        }
        else if (FPS < 50)
        {
            r = 231; g = 251; b = 34;
        }
        else if (FPS >= 51)
        {
            r = 255; g = 255; b = 255;
        }

        // Render FPS text
        RenderText(textAtlas, textShader, "FPS:" + std::to_string((int)FPS), 0, 550, 2, glm::vec3(r / 255, g / 255, b / 255));

        // Text rendering test
        //const char* t = "!\"#$%&'()*+`-./\n0123456789:;<=>?\n@ABCDEFGHIJKLMNO\nPQRSTUVWXYZ[\\]^_\n`abcdefghijklmno\npqrstuvwxyz{|}~";
        //RenderText(textAtlas, textShader, t, 50, 50, 1, glm::vec3(50, 50, 50));

        // Swap buffers
        glfwSwapBuffers(window);

        // Time stuff
        double currentFrame = glfwGetTime();
        frameCount++;
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        if (currentFrame - previousTime >= 1.0)
        {
            // Display the frame count
            FPS = frameCount;

            frameCount = 0;
            previousTime = currentFrame;
        }

        // Poll IO 
        glfwPollEvents();
    }

    // Clean up GLFW resources & exit program
    glfwTerminate();
    return 0;
}

// Query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    double cameraSpeed = 6.5 * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += (float)cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= (float)cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * (float)cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * (float)cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        cameraPos += (float)cameraSpeed * cameraUp;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cameraPos -= (float)cameraSpeed * cameraUp;

}

// Resize callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions
    glViewport(0, 0, width, height);
    SCR_HEIGHT = height;
    SCR_WIDTH = width;
}

// Mouse move callback
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    double xoffset = xpos - lastX;
    double yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians((float)yaw)) * cos(glm::radians((float)pitch));
    front.y = sin(glm::radians((float)pitch));
    front.z = sin(glm::radians((float)yaw)) * cos(glm::radians((float)pitch));
    cameraFront = glm::normalize(front);
}

// Mouse scroll callback
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    // This used to be a zooming feature, but I've removed it because it's not really in the style of Minecraft

    //fov -= (float)yoffset;
    //if (fov < 1.0f)
    //    fov = 1.0f;
    //if (fov > 45.0f)
    //    fov = 45.0f;
}