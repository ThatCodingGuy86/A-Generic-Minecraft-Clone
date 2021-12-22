#pragma once
#include <glm/glm.hpp>


static const float voxelData[(6 * 6) * 6] = {
 // Position // UV    // Light
    // Back face
    0,  0,  0,  00, 15,  0.65f, // Bottom-left
    1,  0,  0,  15, 15,  0.65f, // bottom-right    
    1,  1,  0,  15, 00,  0.65f, // top-right              
    1,  1,  0,  15, 00,  0.65f, // top-right
    0,  1,  0,  00, 00,  0.65f, // top-left
    0,  0,  0,  00, 15,  0.65f, // bottom-left                
    // Front face
    0,  0,  1,  00, 15,  0.85f, // bottom-left
    1,  1,  1,  15, 00,  0.85f, // top-right
    1,  0,  1,  15, 15,  0.85f, // bottom-right        
    1,  1,  1,  15, 00,  0.85f, // top-right
    0,  0,  1,  00, 15,  0.85f, // bottom-left
    0,  1,  1,  00, 00,  0.85f, // top-left        
    // Left face
    0,  1,  1,  15, 00,  0.7f, // top-right
    0,  0,  0,  00, 15,  0.7f, // bottom-left
    0,  1,  0,  00, 00,  0.7f, // top-left       
    0,  0,  0,  15, 15,  0.7f, // bottom-left
    0,  1,  1,  00, 00,  0.7f, // top-right
    0,  0,  1,  00, 15,  0.7f, // bottom-right
    // Right face
    1,  1,  1,  00, 00,  0.7f, // top-left
    1,  1,  0,  15, 00,  0.7f, // top-right      
    1,  0,  0,  15, 15,  0.7f, // bottom-right          
    1,  0,  0,  15, 15,  0.7f, // bottom-right
    1,  0,  1,  00, 15,  0.7f, // bottom-left
    1,  1,  1,  00, 00,  0.7f, // top-left
    // Bottom face          
    0,  0,  0,  00, 15,  0.4f, // top-right
    1,  0,  1,  15, 00,  0.4f, // bottom-left
    1,  0,  0,  15, 15,  0.4f, // top-left        
    1,  0,  1,  15, 00,  0.4f, // bottom-left
    0,  0,  0,  00, 15,  0.4f, // top-right
    0,  0,  1,  00, 00,  0.4f, // bottom-right
    // Top face
    0,  1,  0,  00, 15,  0.85f, // top-left
    1,  1,  0,  15, 15,  0.85f, // top-right
    1,  1,  1,  15, 00,  0.85f, // bottom-right                 
    1,  1,  1,  15, 00,  0.85f, // bottom-right
    0,  1,  1,  00, 00,  0.85f, // bottom-left  
    0,  1,  0,  00, 15,  0.85f  // top-left 
};