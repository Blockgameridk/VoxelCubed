#pragma once

#include "raylib.h"

#include "rlgl.h"

#include "Scene.h"

#include "SceneManager.h"

#include "vector"

#include "format"

#include "FastNoiseLite.h"

#include "BlockRegistry.h"

#include <fstream>

#include <sstream>

#include "raymath.h"

struct InventorySlot {
    int stack;
    
    int id;
};

class World {
    private:
    Texture2D terrain = LoadTexture("resources/terrain.png");
        
        
        FastNoiseLite noiseLite;
        
    public:
        InventorySlot* Inventory = new InventorySlot[10];
    
        std::unique_ptr<Camera> playerCam; //Replace this with a playerController class to prevent far land issues at greater distances
    
        BlockRegistry blockRegistries = BlockRegistry();
    
        const int CHUNK_SIZE = 64;
        
        const int CHUNK_HEIGHT = 256;
    
        int* Blocks = new int[CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT];
        
        int* Lighting = new int[CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT];
        
        inline int flatten1D(int x, int y, int z) {//Takes 3D Voxel Coordinates and Flatten it based on the Chunk Size and Height
           return x + (y * CHUNK_SIZE) + (z * CHUNK_SIZE * CHUNK_HEIGHT);
        }
        
        World();
        ~World();
        
        int getLightValue(int x, int y, int z);
    int GetBlock(int x, int y, int z);
    
    void CalculatePlayerCollisions();

    void calculateUV(float u, float v, float t1, float t2) {
        
        float baseU = t1 * (16.0f / 256.0f);
        
        float baseV = t2 * (16.0f / 256.0f);
        
        rlTexCoord2f(baseU + u * (16.0f / 256.0f), baseV + v * (16.0f / 256.0f));
    }
    
    void DrawCubeTexture(Texture2D texture, Vector3 position, float width, float height, float length, Color color,bool top, bool bottom, bool left, bool right, bool front, bool back, int id)
    {
        float x = position.x;
        float y = position.y;
        float z = position.z;
        
        Block block = blockRegistries.getBlockData(id - 1);
        
        // Set desired texture to be enabled while drawing following vertex data
        rlSetTexture(texture.id);

        // Vertex data transformation can be defined with the commented lines,
        // but in this example we calculate the transformed vertex data directly when calling rlVertex3f()
        //rlPushMatrix();
            // NOTE: Transformation is applied in inverse order (scale -> rotate -> translate)
            //rlTranslatef(2.0f, 0.0f, 0.0f);
            //rlRotatef(45, 0, 1, 0);
            //rlScalef(2.0f, 2.0f, 2.0f);

           // rlBegin(RL_QUADS);
                // Front Face
                
                if (front) {
                
                BlockUV uvs = block.GetFaceUV(4);
                
                float t1 = uvs.U;
                
                float t2 = uvs.V;
                
                if (getLightValue(x,y,z + 1) >= 0) {
                    
                    float light = getLightValue(x,y,z + 1);
                    
                    rlColor4ub(light * 17, light * 17, light * 17, 255);
                    
                }
                //int randomLight = GetRandomValue(1,15);
                
                //rlColor4ub(randomLight * 17, randomLight * 17, randomLight * 17, 255);
                
                rlNormal3f(0.0f, 0.0f, 1.0f);       // Normal Pointing Towards Viewer
                //rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x - width/2, y - height/2, z + length/2);  // Bottom Left Of The Texture and Quad
               // rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x + width/2, y - height/2, z + length/2);  // Bottom Right Of The Texture and Quad
                //rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x + width/2, y + height/2, z + length/2);  // Top Right Of The Texture and Quad
                //rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x - width/2, y + height/2, z + length/2);  // Top Left Of The Texture and Quad
                
                calculateUV(0.0f, 1.0f, t1,t2); rlVertex3f(x - width/2, y - height/2, z + length/2);  // Bottom Left Of The Texture and Quad
                calculateUV(1.0f, 1.0f, t1, t2); rlVertex3f(x + width/2, y - height/2, z + length/2);  // Bottom Right Of The Texture and Quad
                calculateUV(1.0f, 0.0f, t1, t2); rlVertex3f(x + width/2, y + height/2, z + length/2);  // Top Right Of The Texture and Quad
                calculateUV(0.0f, 0.0f, t1, t2); rlVertex3f(x - width/2, y + height/2, z + length/2);  // Top Left Of The Texture and Quad
                }
                if (back) {
                BlockUV uvs = block.GetFaceUV(5);
                
                float t1 = uvs.U;
                
                float t2 = uvs.V;
                
                    if (getLightValue(x,y,z - 1) >= 0) {
                    
                    float light = getLightValue(x,y,z - 1);
                    
                    rlColor4ub(light * 17, light * 17, light * 17, 255);
                    
                }
                //int randomLight = GetRandomValue(1,15);
                
                //rlColor4ub(randomLight * 17, randomLight * 17, randomLight * 17, 255);
                // Back Face
                rlNormal3f(0.0f, 0.0f, - 1.0f);     // Normal Pointing Away From Viewer
                //rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x - width/2, y - height/2, z - length/2);  // Bottom Right Of The Texture and Quad
                //rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x - width/2, y + height/2, z - length/2);  // Top Right Of The Texture and Quad
                //rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x + width/2, y + height/2, z - length/2);  // Top Left Of The Texture and Quad
                //rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x + width/2, y - height/2, z - length/2);  // Bottom Left Of The Texture and Quad
                
                calculateUV(1.0f, 1.0f,t1,t2); rlVertex3f(x - width/2, y - height/2, z - length/2);  // Bottom Right Of The Texture and Quad
                calculateUV(1.0f, 0.0f,t1,t2); rlVertex3f(x - width/2, y + height/2, z - length/2);  // Top Right Of The Texture and Quad
                calculateUV(0.0f, 0.0f,t1,t2); rlVertex3f(x + width/2, y + height/2, z - length/2);  // Top Left Of The Texture and Quad
                calculateUV(0.0f, 1.0f,t1,t2); rlVertex3f(x + width/2, y - height/2, z - length/2);  // Bottom Left Of The Texture and Quad
                }
                // Top Face
                if (top) {
                BlockUV uvs = block.GetFaceUV(0);
                
                float t1 = uvs.U;
                
                float t2 = uvs.V;
                
                if (getLightValue(x,y + 1,z) >= 0) {
                    
                    float light = getLightValue(x,y + 1,z);
                    
                    rlColor4ub(light * 17, light * 17, light * 17, 255);
                    
                }
                rlNormal3f(0.0f, 1.0f, 0.0f);       // Normal Pointing Up
                calculateUV(0.0f, 0.0f,t1,t2); rlVertex3f(x - width/2, y + height/2, z - length/2);  // Top Left Of The Texture and Quad
                calculateUV(0.0f, 1.0f,t1,t2); rlVertex3f(x - width/2, y + height/2, z + length/2);  // Bottom Left Of The Texture and Quad
                calculateUV(1.0f, 1.0f,t1,t2); rlVertex3f(x + width/2, y + height/2, z + length/2);  // Bottom Right Of The Texture and Quad
                calculateUV(1.0f, 0.0f,t1,t2); rlVertex3f(x + width/2, y + height/2, z - length/2);  // Top Right Of The Texture and Quad
                }
                // Bottom Face
                rlColor4ub(255, 255, 255, 255);
                if (bottom) {
                    
                BlockUV uvs = block.GetFaceUV(1);
                
                float t1 = uvs.U;
                
                float t2 = uvs.V;
                
                    
                if (getLightValue(x,y - 1,z) >= 0) {
                    
                    float light = getLightValue(x,y - 1,z);
                    
                    rlColor4ub(light * 17, light * 17, light * 17, 255);
                    
                }
                    
                rlNormal3f(0.0f, - 1.0f, 0.0f);     // Normal Pointing Down
                calculateUV(1.0f, 0.0f,t1,t2); rlVertex3f(x - width/2, y - height/2, z - length/2);  // Top Right Of The Texture and Quad
                calculateUV(0.0f, 0.0f,t1,t2); rlVertex3f(x + width/2, y - height/2, z - length/2);  // Top Left Of The Texture and Quad
                calculateUV(0.0f, 1.0f,t1,t2); rlVertex3f(x + width/2, y - height/2, z + length/2);  // Bottom Left Of The Texture and Quad
                calculateUV(1.0f, 1.0f,t1,t2); rlVertex3f(x - width/2, y - height/2, z + length/2);  // Bottom Right Of The Texture and Quad
                }
                // Right face
                if (right) {
                    
                BlockUV uvs = block.GetFaceUV(3);
                
                float t1 = uvs.U;
                
                float t2 = uvs.V;
                
                    
                if (getLightValue(x + 1,y,z) >= 0) {
                    
                    float light = getLightValue(x + 1,y,z);
                    
                    rlColor4ub(light * 17, light * 17, light * 17, 255);
                    
                }
                rlNormal3f(1.0f, 0.0f, 0.0f);       // Normal Pointing Right
                calculateUV(1.0f, 1.0f,t1,t2); rlVertex3f(x + width/2, y - height/2, z - length/2);  // Bottom Right Of The Texture and Quad
                calculateUV(1.0f, 0.0f,t1,t2); rlVertex3f(x + width/2, y + height/2, z - length/2);  // Top Right Of The Texture and Quad
                calculateUV(0.0f, 0.0f,t1,t2); rlVertex3f(x + width/2, y + height/2, z + length/2);  // Top Left Of The Texture and Quad
                calculateUV(0.0f, 1.0f,t1,t2); rlVertex3f(x + width/2, y - height/2, z + length/2);  // Bottom Left Of The Texture and Quad
                }
                // Left Face
                if (left) {
                    
                BlockUV uvs = block.GetFaceUV(2);
                
                float t1 = uvs.U;
                
                float t2 = uvs.V;
                
                if (getLightValue(x - 1,y,z) >= 0) {
                    
                    float light = getLightValue(x - 1,y,z);
                    
                    rlColor4ub(light * 17, light * 17, light * 17, 255);
                    
                }
                rlNormal3f( - 1.0f, 0.0f, 0.0f);    // Normal Pointing Left
                calculateUV(0.0f, 1.0f,t1,t2); rlVertex3f(x - width/2, y - height/2, z - length/2);  // Bottom Left Of The Texture and Quad
                calculateUV(1.0f, 1.0f,t1,t2); rlVertex3f(x - width/2, y - height/2, z + length/2);  // Bottom Right Of The Texture and Quad
                calculateUV(1.0f, 0.0f,t1,t2); rlVertex3f(x - width/2, y + height/2, z + length/2);  // Top Right Of The Texture and Quad
                calculateUV(0.0f, 0.0f,t1,t2); rlVertex3f(x - width/2, y + height/2, z - length/2);  // Top Left Of The Texture and Quad
                }
           // rlEnd();
        //rlPopMatrix();

        rlSetTexture(0);
}

int removeBlockFromRay(Ray ray);

int placeBlockFromRay(Ray ray, int id);

void renderWorld();

Vector3 getBlockVectorFromRay(Ray ray);

void updateLighting();

void setBlock(int x, int y, int z, int id);

bool isBlockExposed(int x, int y, int z);

void save();

void load();

};