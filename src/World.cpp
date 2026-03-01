#include "World.h"

World::World()
{
    playerCam = std::make_unique<Camera>();

    playerCam->position = {0,20,1};
    
    playerCam->target = {0,0,0};
    
    playerCam->up = {0,1,0};
    
    playerCam->fovy = 60;
    
    Inventory[0] = {64,1};
    
    Inventory[1] = {64,2};
    
    Inventory[2] = {64,3};
    
    Inventory[3] = {64,4};
    
    noiseLite.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    
   //Blocks.resize(16 * 16 * 128);
    
     for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            for (int y =0; y < CHUNK_HEIGHT; y++) {
                
                Blocks[flatten1D(x, y, z)] = 0;
                
                Lighting[flatten1D(x, y, z)] = 0;
                
                //float density = noiseLite.GetNoise((float)x * 30,(float)y * 30, (float)z * 30) + 30;
                //std::cout << density << std::endl;
                if (y < 24) {
                    Blocks[flatten1D(x, y, z)] = 2;
                }
                
                if (y == 0) {
                    Blocks[flatten1D(x, y, z)] = 4;
                }
            }
            
            for (int y = CHUNK_HEIGHT - 1; y >= 0; y--) {
                if (GetBlock(x,y,z) > 1) {
                    Blocks[flatten1D(x, y, z)] = 1;
                    
                    for (int a = y - 1; a > y - GetRandomValue(1,4); a--) {
                        Blocks[flatten1D(x, a, z)] = 3;
                    }
                    
                    break;
                }
            }
            
            for (int y = CHUNK_HEIGHT - 1; y >= 0; y--) {
                if (GetBlock(x,y,z) < 1) {
                
                Lighting[flatten1D(x, y, z)] = 15;
                
                }
                else {
                    break;
                }
                
            }
            
        }
    }
}

World::~World()
{
    delete[] Blocks;
    
    delete[] Lighting;
}

int World::getLightValue(int x, int y, int z)
{
    if (x < CHUNK_SIZE & z < CHUNK_SIZE & y < CHUNK_HEIGHT & x >= 0 & y >= 0 & z >= 0) {
        return Lighting[flatten1D(x, y, z)];
    } else {
        return 15;
    }
}

int World::GetBlock(int x, int y, int z) {
    if (x < CHUNK_SIZE & z < CHUNK_SIZE & y < CHUNK_HEIGHT & x >= 0 & y >= 0 & z >= 0) {
        return Blocks[flatten1D(x, y, z)];
    } else {
        return 0;
    }
}

void World::CalculatePlayerCollisions()
{
   
}

int World::removeBlockFromRay(Ray ray) {
    
    float maxDistance = 5;
    
    float rayDistance = 0;
    
    bool blockDestroyed = false;
    
    while (rayDistance < maxDistance) {
        Vector3 pos = Vector3Scale(ray.direction, rayDistance);
        
        
        pos = Vector3Add(ray.position, Vector3Add(pos,{0.5,0.5,0.5}));
        
        Vector3 roundedPos = Vector3(floorf(pos.x), floorf(pos.y), floorf(pos.z));
        
       // std::cout << std::format("Position: X: {}, Y: {}, Z: {}", roundedPos.x,roundedPos.y,roundedPos.z) << std::endl;
        
        if (GetBlock(roundedPos.x, roundedPos.y,roundedPos.z) > 0 && isBlockExposed(roundedPos.x,roundedPos.y,roundedPos.z) && !blockDestroyed) {
            setBlock(roundedPos.x, roundedPos.y, roundedPos.z, 0);
            
            blockDestroyed = true;
        }
        
        rayDistance += 0.0001f;
        
        
    }
    
    updateLighting();
    
    return 0;
}

int World::placeBlockFromRay(Ray ray, int id) {
    
    float maxDistance = 5;
    
    float rayDistance = 0;
    
    bool blockDestroyed = false;
    
    while (rayDistance < maxDistance) {
        Vector3 pos = Vector3Scale(ray.direction, rayDistance);
        
        
        pos = Vector3Add(ray.position, Vector3Add(pos,{0.5,0.5,0.5}));
        
        Vector3 roundedPos = Vector3(floorf(pos.x), floorf(pos.y), floorf(pos.z));
        
        RayCollision collision = GetRayCollisionBox(ray, {{roundedPos.x - 0.5f,roundedPos.y - 0.5f,roundedPos.z - 0.5f},{roundedPos.x + 0.5f, roundedPos.y + 0.5f, roundedPos.z + 0.5f}});
        
        Vector3 normal = {0,0,0};
        
        if (collision.hit) {
            normal = Vector3Add(roundedPos,collision.normal);
        }
        
        //std::cout << std::format("Position: X: {}, Y: {}, Z: {}", roundedPos.x,roundedPos.y,roundedPos.z) << std::endl;
        
        if (GetBlock(roundedPos.x, roundedPos.y,roundedPos.z) > 0 && isBlockExposed(roundedPos.x,roundedPos.y,roundedPos.z) && !blockDestroyed && GetBlock(normal.x,normal.y,normal.z) < 1) {
            setBlock(normal.x, normal.y, normal.z, id);
            
            blockDestroyed = true;
        }
        
        rayDistance += 0.0001f;
        
        
    }
    
    updateLighting();

    return 0;
}

void World::renderWorld()
{
    rlBegin(RL_QUADS);
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            for (int y =0; y < CHUNK_HEIGHT; y++) {
                int i = GetBlock(x,y,z);
                
                if (i < 1) continue;
                
                bool top = GetBlock(x,y + 1,z) < 1;
                
                bool bottom = GetBlock(x,y - 1,z) < 1;
                
                bool front = GetBlock(x,y,z+1) < 1;
                
                bool back = GetBlock(x,y,z-1) < 1;
                
                bool left = GetBlock(x - 1,y,z) < 1;
                
                bool right = GetBlock(x + 1,y,z) < 1;
                DrawCubeTexture(terrain, {(float)x,(float)y,(float)z}, 1.0f,1.0f,1.0f,WHITE,top,bottom,left,right,front,back,i);
            }
        }
    }
    rlEnd(); 
}

Vector3 World::getBlockVectorFromRay(Ray ray)
{
     float maxDistance = 5;
    
    float rayDistance = 0;
    
    bool blockDestroyed = false;
    
    while (rayDistance < maxDistance) {
        Vector3 pos = Vector3Scale(ray.direction, rayDistance);
        
        
        pos = Vector3Add(ray.position, Vector3Add(pos,{0.5,0.5,0.5}));
        
        Vector3 roundedPos = Vector3(floorf(pos.x), floorf(pos.y), floorf(pos.z));
        //std::cout << std::format("Position: X: {}, Y: {}, Z: {}", roundedPos.x,roundedPos.y,roundedPos.z) << std::endl;
        
        if (GetBlock(roundedPos.x, roundedPos.y,roundedPos.z) > 0 && isBlockExposed(roundedPos.x,roundedPos.y,roundedPos.z) && !blockDestroyed) {
            //setBlock(normal.x, normal.y, normal.z, Inventory[currentSlot].id);
            return {roundedPos.x,roundedPos.y,roundedPos.z};
            blockDestroyed = true;
        }
        
        rayDistance += 0.001f;
        
        
    }
    
   // updateLighting();
    
    return Vector3(INFINITY,INFINITY,INFINITY);
}

void World::updateLighting()
{
        for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            for (int y = CHUNK_HEIGHT - 1; y >= 0; y--) {
                Lighting[x + (y * CHUNK_SIZE) + (z * CHUNK_SIZE * CHUNK_HEIGHT)] = 7;
            }
            
            for (int y = CHUNK_HEIGHT - 1; y >= 0; y--) {
                if (GetBlock(x,y,z) < 1) {
                
                Lighting[x + (y * CHUNK_SIZE) + (z * CHUNK_SIZE * CHUNK_HEIGHT)] = 15;
                
                }
                else {
                    break;
                }
                
            }
            
        }
    }
}

void World::setBlock(int x,int y,int z, int id) {
    if (x < CHUNK_SIZE & z < CHUNK_SIZE & y < CHUNK_HEIGHT & x >= 0 & y >= 0 & z >= 0) {
         Blocks[x + (y * CHUNK_SIZE) + (z * CHUNK_SIZE * CHUNK_HEIGHT)] = id;
    } else {
        return;
    }
    //Blocks[x + (y * CHUNK_SIZE) + (z * CHUNK_SIZE * CHUNK_HEIGHT)] = id;
    
}

bool World::isBlockExposed(int x, int y, int z)
{
    bool top = GetBlock(x,y + 1,z) < 1;
                
    bool bottom = GetBlock(x,y - 1,z) < 1;
                
    bool front = GetBlock(x,y,z+1) < 1;
                
    bool back = GetBlock(x,y,z-1) < 1;
                
    bool left = GetBlock(x - 1,y,z) < 1;
                
    bool right = GetBlock(x + 1,y,z) < 1;
    
    if (top | bottom | front | back | left | right) {
        return true;
    }
    
    return false;
}

void World::save()
{
    std::ofstream file("saves/save.vsf");
    
    file << "This is a Voxel Save File. Don't fuck with it!" << std::endl;
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            for (int y =0; y < CHUNK_HEIGHT; y++) {
                int i = GetBlock(x,y,z);
                
                if (i < 1) continue;
                
                int index = x + (y * CHUNK_SIZE) + (z * CHUNK_SIZE * CHUNK_HEIGHT);
                
                file << index << " " << i << std::endl;
            }
        }
    }
}

void World::load()
{
    std::ifstream file("saves/save.vsf");
    
    if (!file.is_open())
    {
        std::cerr << "Error: Unable to open file!" << std::endl;
        return;
    }
    std::string line;
    
    int i = 0;
    
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            for (int y =0; y < CHUNK_HEIGHT; y++) {
                
                Blocks[x + (y * CHUNK_SIZE) + (z * CHUNK_SIZE * CHUNK_HEIGHT)] = 0;
                
                Lighting[x + (y * CHUNK_SIZE) + (z * CHUNK_SIZE * CHUNK_HEIGHT)] = 0;
            }
            
        }
    }
    
    while (getline(file, line)) {

        
        i++;
        
        if (i > 1) {
             std::istringstream iss(line);
                    int num;
                    
                    std::vector<int> nums;
                    
                    while(iss >> num) 
                    {
                        //std::cout << num << " ";
                        
                        nums.push_back(num);
                    }
                    
                    //std::cout << std::endl;
                    
                    Blocks[nums[0]] = nums[1];
                    
                    nums.clear();
        }
    }
    // Check for eof
    if (file.eof())
        std::cout << "Reached end of file." << std::endl;
    else
        std::cerr << "Error: File reading failed!" << std::endl;
    
    updateLighting(); 
        
    file.close();
}
