#pragma once



class Block
{

public:
    float* verts;
    int id;
    bool isSolid;
    bool isTransparent;

    int faceBackTex = 1;
    int faceFrontTex = 1;

    int faceRightTex = 1;
    int faceLeftTex = 1;

    int faceDownTex = 0;
    int faceUpTex = 2;

    Block(int _id, bool _isSolid, bool _isTransparent)
    {
        id = _id;
        isSolid = _isSolid;
        isTransparent = _isTransparent;


        float vertices[] = {
            // Position          // UV    // Light
              -0.5f, -0.5f, -0.5f,  0,  15,  1.0f, // Back
               0.5f, -0.5f, -0.5f,  15, 15,  1.0f,
               0.5f,  0.5f, -0.5f,  15, 00,  1.0f,
               0.5f,  0.5f, -0.5f,  15, 00,  1.0f,
              -0.5f,  0.5f, -0.5f,  00, 00,  1.0f,
              -0.5f, -0.5f, -0.5f,  00, 15,  1.0f,

              -0.5f, -0.5f,  0.5f,  00, 15,  1.0f, // Front
               0.5f, -0.5f,  0.5f,  15, 15,  1.0f,
               0.5f,  0.5f,  0.5f,  15, 00,  1.0f,
               0.5f,  0.5f,  0.5f,  15, 00,  1.0f,
              -0.5f,  0.5f,  0.5f,  00, 00,  1.0f,
              -0.5f, -0.5f,  0.5f,  00, 15,  1.0f,

              -0.5f,  0.5f,  0.5f,  00, 00,  1.0f, // Right
              -0.5f,  0.5f, -0.5f,  15, 00,  1.0f,
              -0.5f, -0.5f, -0.5f,  15, 15,  1.0f,
              -0.5f, -0.5f, -0.5f,  15, 15,  1.0f,
              -0.5f, -0.5f,  0.5f,  00, 15,  1.0f,
              -0.5f,  0.5f,  0.5f,  00, 00,  1.0f,

               0.5f,  0.5f,  0.5f,  00, 00,  1.0f, // Left
               0.5f,  0.5f, -0.5f,  15, 00,  1.0f,
               0.5f, -0.5f, -0.5f,  15, 15,  1.0f,
               0.5f, -0.5f, -0.5f,  15, 15,  1.0f,
               0.5f, -0.5f,  0.5f,  00, 15,  1.0f,
               0.5f,  0.5f,  0.5f,  00, 00,  1.0f,

              -0.5f, -0.5f, -0.5f,  00, 15,  1.0f, // Bottom
               0.5f, -0.5f, -0.5f,  15, 15,  1.0f,
               0.5f, -0.5f,  0.5f,  15, 00,  1.0f,
               0.5f, -0.5f,  0.5f,  15, 00,  1.0f,
              -0.5f, -0.5f,  0.5f,  00, 00,  1.0f,
              -0.5f, -0.5f, -0.5f,  00, 15,  1.0f,

              -0.5f,  0.5f, -0.5f,  00, 15,  1.0f, // Top
               0.5f,  0.5f, -0.5f,  15, 15,  1.0f,
               0.5f,  0.5f,  0.5f,  15, 00,  1.0f,
               0.5f,  0.5f,  0.5f,  15, 00,  1.0f,
              -0.5f,  0.5f,  0.5f,  00, 00,  1.0f,
              -0.5f,  0.5f, -0.5f,  00, 15,  1.0f
        };
        

        for (int k = 0; k < 6; k++)
        {
            vertices[3 + (6 * k)] = vertices[3 + (6 * k)] + (16 * 1);
            vertices[39 + (6 * k)] = vertices[39 + (6 * k)] + (16 * 1);
            vertices[75 + (6 * k)] = vertices[75 + (6 * k)] + (16 * 1);
            vertices[111 + (6 * k)] = vertices[111 + (6 * k)] + (16 * 1);
            vertices[147 + (6 * k)] = vertices[147 + (6 * k)] + (16 * 0);
            vertices[183 + (6 * k)] = vertices[183 + (6 * k)] + (16 * 2);
        }

        verts = new float[216];

        verts = vertices;
    }

    ~Block()
    {
        delete[] verts;
    }

    void Render()
    {

    }
};