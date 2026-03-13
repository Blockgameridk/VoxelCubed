#include "World.h"

void World::addChunk(int cx, int cy)
{
    Chunk c = Chunk(cx,cy);
    // ==chunks[{0,0}] = Chunk(cx,cy);
    chunks.push_back(c);
    
}

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
    
    Inventory[4] = {64,6};
    
    worldMaterial = LoadMaterialDefault();
    
    worldMaterial.maps[MATERIAL_MAP_ALBEDO].texture = terrain;
    
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
                    Blocks[flatten1D(x, y, z)] = 5;
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
    
    for (int x = -4; x < 4; x++) {
        for (int y = -4; y < 4; y++) {
            addChunk(x,y);
        }
    }
    
    generateMesh();
    
}

World::~World()
{
    delete[] Blocks;
    
    delete[] Lighting;
}

int World::getLightValue(int x, int y, int z)
{
    if (x < CHUNK_SIZE && z < CHUNK_SIZE && y < CHUNK_HEIGHT && x >= 0 && y >= 0 && z >= 0) {
        return Lighting[flatten1D(x, y, z)];
    } else {
        return 15;
    }
}

int World::GetBlock(int x, int y, int z) {
    if (x < CHUNK_SIZE && z < CHUNK_SIZE && y < CHUNK_HEIGHT && x >= 0 && y >= 0 && z >= 0) {
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


        pos = Vector3Add(ray.position, Vector3Add(pos, { 0.5,0.5,0.5 }));

        Vector3 roundedPos = { floorf(pos.x), floorf(pos.y), floorf(pos.z) };
        
       // std::cout << std::format("Position: X: {}, Y: {}, Z: {}", roundedPos.x,roundedPos.y,roundedPos.z) << std::endl;
        
        if (GetBlock(roundedPos.x, roundedPos.y,roundedPos.z) > 0 && isBlockExposed(roundedPos.x,roundedPos.y,roundedPos.z) && !blockDestroyed) {
            setBlock(roundedPos.x, roundedPos.y, roundedPos.z, 0);
            
            blockDestroyed = true;
        }
        
        rayDistance += 0.0001f;
        
        
    }
    
    updateLighting();
    
    generateMesh();
    
    return 0;
}

int World::placeBlockFromRay(Ray ray, int id) {
    
    float maxDistance = 5;
    
    float rayDistance = 0;
    
    bool blockDestroyed = false;
    
    while (rayDistance < maxDistance) {
        Vector3 pos = Vector3Scale(ray.direction, rayDistance);
        
        
        pos = Vector3Add(ray.position, Vector3Add(pos,{0.5,0.5,0.5}));
        
        Vector3 roundedPos = { floorf(pos.x), floorf(pos.y), floorf(pos.z) };
        
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
    
    generateMesh();
    
    return 0;
}

void World::renderWorld()
{
    // Matrix matrix = MatrixTranslate(0 ,0,0);
    
    // DrawMesh(worldMesh,worldMaterial,matrix);
    
    for (Chunk c : chunks) {
        Vector2 chunkCoords = c.getPosition();
        
        Matrix matrix = MatrixTranslate(chunkCoords.x * CHUNK_SIZE,0,chunkCoords.y * CHUNK_SIZE);
        
        DrawMesh(worldMesh,worldMaterial,matrix);
    }
    
}

void addTriangles(std::vector<float>* Vertices, float x, float y, float z, Vector3 v1, Vector3 v2, Vector3 v3) {
    Vertices->push_back(x + v1.x);
    
    Vertices->push_back(y + v1.y);
    
    Vertices->push_back(z + v1.z);
    
    Vertices->push_back(x + v2.x);
    
    Vertices->push_back(y + v2.y);
    
    Vertices->push_back(z + v2.z);
    
    Vertices->push_back(x + v3.x);
    
    Vertices->push_back(y + v3.y);
    
    Vertices->push_back(z + v3.z);
    
}

void addQuad(std::vector<float>* Vertices, float x, float y, float z, Vector3 v1, Vector3 v2, Vector3 v3, Vector3 v4) {
    addTriangles(Vertices,x,y,z,v1,v2,v3);
    
    addTriangles(Vertices,x,y,z,v4,v1,v3);
}

void addTriangleUVS(std::vector<float>* UVS, Vector2 uv1, Vector2 uv2, Vector2 uv3) {
    UVS->push_back(uv1.x);
    
    UVS->push_back(uv1.y);
    
    UVS->push_back(uv2.x);
    
    UVS->push_back(uv2.y);
    
    UVS->push_back(uv3.x);
    
    UVS->push_back(uv3.y);
    
    
}

void addColorTriangle(std::vector<unsigned char>* COLORS, Color c1, Color c2, Color c3) {
    COLORS->push_back(c1.r);
    
    COLORS->push_back(c1.g);
    
    COLORS->push_back(c1.b);
    
    COLORS->push_back(c1.a);
    
    COLORS->push_back(c2.r);
    
    COLORS->push_back(c2.g);
    
    COLORS->push_back(c2.b);
    
    COLORS->push_back(c2.a);
    
    COLORS->push_back(c3.r);
    
    COLORS->push_back(c3.g);
    
    COLORS->push_back(c3.b);
    
    COLORS->push_back(c3.a);
}

void addQuadUVS(std::vector<float>* UVS, Vector2 uv1, Vector2 uv2, Vector2 uv3, Vector2 uv4) {
    
    addTriangleUVS(UVS,uv1,uv2,uv3);
    
    addTriangleUVS(UVS,uv4,uv1,uv3);
    
}

void addQuadColors(std::vector<unsigned char>* COLORS, Color c1, Color c2, Color c3, Color c4) {
    addColorTriangle(COLORS,c1,c2,c3);
    
    addColorTriangle(COLORS,c4,c1,c3);
}

Color lightLevelToColor(int lightLevel) {
    return Color(lightLevel * 17, lightLevel * 17, lightLevel * 17, 255);
}

void World::generateMesh()
{
    std::vector<float> Vertices;
    
    std::vector<float> UVS;
    
    std::vector<unsigned char> Colors;
    
    int triangleCount = 0;
    
    if (meshExists != false) {
        
        UnloadMesh(worldMesh);
        
        meshExists = false;
        
    }
    
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            for (int y =0; y < CHUNK_HEIGHT; y++) {
                int i = GetBlock(x,y,z);
                
                if (i < 1) continue;
                
                Block block = blockRegistries.getBlockData(i);
                
                if (isBlockTranslucent(x,y + 1,z)) {
                    
                    BlockUV uv = block.GetFaceUV(0);
                    
                    int light = getLightValue(x, y + 1,z);
                    
                    float t1 = uv.U;
                    
                    float t2 = uv.V;
                    
                    triangleCount+=2;
                    
                    addQuad(&Vertices, x,y,z, {-0.5,0.5,-0.5}, {-0.5,0.5,0.5}, {0.5,0.5,0.5}, {0.5,0.5,-0.5});
                    
                    addQuadUVS(&UVS, calculateUV(0.0f, 1.0f, t1, t2), calculateUV(1.0f,1.0f,t1,t2), calculateUV(1.0f,0.0f,t1,t2), calculateUV(0.0f,0.0f,t1,t2));
                    
                    addQuadColors(&Colors,lightLevelToColor(light),lightLevelToColor(light),lightLevelToColor(light),lightLevelToColor(light));
                    
                }
                
                if (isBlockTranslucent(x,y - 1,z)) {
                    
                    BlockUV uv = block.GetFaceUV(1);
                    
                    int light = getLightValue(x, y - 1,z);
                    
                    float t1 = uv.U;
                    
                    float t2 = uv.V;
                    
                    triangleCount+=2;
                    
                    addQuad(&Vertices, x,y,z, {-0.5,-0.5,-0.5}, {0.5,-0.5,-0.5}, {0.5,-0.5,0.5}, {-0.5,-0.5,0.5});
                    
                    addQuadUVS(&UVS, calculateUV(1.0f, 0.0f, t1, t2), calculateUV(0.0f,0.0f,t1,t2), calculateUV(0.0f,1.0f,t1,t2), calculateUV(1.0f,1.0f,t1,t2));
                    
                    addQuadColors(&Colors,lightLevelToColor(light),lightLevelToColor(light),lightLevelToColor(light),lightLevelToColor(light));
                    
                }
                
                if (isBlockTranslucent(x,y,z + 1)) {
                    
                    BlockUV uv = block.GetFaceUV(4);
                    
                    int light = getLightValue(x, y,z + 1);
                    
                    float t1 = uv.U;
                    
                    float t2 = uv.V;
                    
                    triangleCount+=2;
                    
                    addQuad(&Vertices, x,y,z, {-0.5,-0.5,0.5}, {0.5,-0.5,0.5}, {0.5,0.5,0.5}, {-0.5,0.5,0.5});
                    
                    addQuadUVS(&UVS, calculateUV(0.0f, 1.0f, t1, t2), calculateUV(1.0f,1.0f,t1,t2), calculateUV(1.0f,0.0f,t1,t2), calculateUV(0.0f,0.0f,t1,t2));
                    
                    addQuadColors(&Colors,lightLevelToColor(light),lightLevelToColor(light),lightLevelToColor(light),lightLevelToColor(light));
                }
                
                if (isBlockTranslucent(x,y,z - 1)) {
                    
                    BlockUV uv = block.GetFaceUV(5);
                    
                    int light = getLightValue(x, y,z - 1);
                    
                    float t1 = uv.U;
                    
                    float t2 = uv.V;
                    
                    triangleCount+=2;
                    
                    addQuad(&Vertices, x,y,z, {-0.5,-0.5,-0.5}, {-0.5,0.5,-0.5}, {0.5,0.5,-0.5}, {0.5,-0.5,-0.5});
                    
                    addQuadUVS(&UVS, calculateUV(1.0f, 1.0f, t1, t2), calculateUV(1.0f,0.0f,t1,t2), calculateUV(0.0f,0.0f,t1,t2), calculateUV(0.0f,1.0f,t1,t2));
                    
                    addQuadColors(&Colors,lightLevelToColor(light),lightLevelToColor(light),lightLevelToColor(light),lightLevelToColor(light));
                }
                
                if (isBlockTranslucent(x - 1,y,z)) {
                    
                    BlockUV uv = block.GetFaceUV(2);
                    
                    int light = getLightValue(x - 1, y,z );
                    
                    float t1 = uv.U;
                    
                    float t2 = uv.V;
                    
                    triangleCount+=2;
                    
                    addQuad(&Vertices, x,y,z, {-0.5,-0.5,-0.5}, {-0.5,-0.5,0.5}, {-0.5,0.5,0.5}, {-0.5,0.5,-0.5});
                    
                    addQuadUVS(&UVS, calculateUV(0.0f, 1.0f, t1, t2), calculateUV(1.0f,1.0f,t1,t2), calculateUV(1.0f,0.0f,t1,t2), calculateUV(0.0f,0.0f,t1,t2));
                    
                    addQuadColors(&Colors,lightLevelToColor(light),lightLevelToColor(light),lightLevelToColor(light),lightLevelToColor(light));
                }
                
                if (isBlockTranslucent(x + 1,y,z)) {
                    
                    BlockUV uv = block.GetFaceUV(3);
                    
                    int light = getLightValue(x + 1, y,z);
                    
                    float t1 = uv.U;
                    
                    float t2 = uv.V;
                    
                    triangleCount+=2;
                    
                    addQuad(&Vertices, x,y,z, {0.5,-0.5,-0.5}, {0.5,0.5,-0.5}, {0.5,0.5,0.5}, {0.5,-0.5,0.5});
                    
                    addQuadUVS(&UVS, calculateUV(1.0f, 1.0f, t1, t2), calculateUV(1.0f,0.0f,t1,t2), calculateUV(0.0f,0.0f,t1,t2), calculateUV(0.0f,1.0f,t1,t2));
                    
                    addQuadColors(&Colors,lightLevelToColor(light),lightLevelToColor(light),lightLevelToColor(light),lightLevelToColor(light));
                }
                
                
                //bool bottom = GetBlock(x,y - 1,z) < 1;
                
                //bool front = GetBlock(x,y,z+1) < 1;
                
                //bool back = GetBlock(x,y,z-1) < 1;
                
               // bool left = GetBlock(x - 1,y,z) < 1;
                
                //bool right = GetBlock(x + 1,y,z) < 1;
                
                
            }
        }
    }
    
    //float* verts = Vertices.data();
    
    worldMesh = { 0 };
    
    worldMesh.triangleCount = triangleCount;
    
    worldMesh.vertexCount = triangleCount * 3;
    
    worldMesh.vertices = (float *)MemAlloc(worldMesh.vertexCount*3*sizeof(float));;
    
    worldMesh.texcoords = (float *)MemAlloc(worldMesh.vertexCount*2*sizeof(float));;
    
    worldMesh.colors = (unsigned char *)MemAlloc(worldMesh.vertexCount*4*sizeof(unsigned char));;
    
    std::copy(Vertices.begin(), Vertices.end(), worldMesh.vertices);
    
    std::copy(UVS.begin(), UVS.end(), worldMesh.texcoords);
    
    std::copy(Colors.begin(), Colors.end(), worldMesh.colors);

    UploadMesh(&worldMesh, false);
    
    meshExists = true;
}

Vector3 World::getBlockVectorFromRay(Ray ray)
{
    float maxDistance = 5;

    float rayDistance = 0;

    bool blockDestroyed = false;

    while (rayDistance < maxDistance) {
        Vector3 pos = Vector3Scale(ray.direction, rayDistance);


        pos = Vector3Add(ray.position, Vector3Add(pos, { 0.5,0.5,0.5 }));

        Vector3 roundedPos = { floorf(pos.x), floorf(pos.y), floorf(pos.z) };
        //std::cout << std::format("Position: X: {}, Y: {}, Z: {}", roundedPos.x,roundedPos.y,roundedPos.z) << std::endl;

        if (GetBlock(roundedPos.x, roundedPos.y, roundedPos.z) > 0 && isBlockExposed(roundedPos.x, roundedPos.y, roundedPos.z) && !blockDestroyed) {
            //setBlock(normal.x, normal.y, normal.z, Inventory[currentSlot].id);
            return { roundedPos.x,roundedPos.y,roundedPos.z };
            blockDestroyed = true;
        }

        rayDistance += 0.001f;


    }

    // updateLighting();

    return { INFINITY, INFINITY, INFINITY };
}

void World::updateLighting()
{
        for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            for (int y = CHUNK_HEIGHT - 1; y >= 0; y--) {
                Lighting[x + (y * CHUNK_SIZE) + (z * CHUNK_SIZE * CHUNK_HEIGHT)] = 7;
            }
            
            for (int y = CHUNK_HEIGHT - 1; y >= 0; y--) {
                if (GetBlock(x,y,z) < 1 || isBlockTranslucent(x,y,z)) {
                
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
    if (x < CHUNK_SIZE && z < CHUNK_SIZE && y < CHUNK_HEIGHT && x >= 0 && y >= 0 && z >= 0) {
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
    
    if (top || bottom || front || back || left || right) {
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
                
                //int index = x + (y * CHUNK_SIZE) + (z * CHUNK_SIZE * CHUNK_HEIGHT);
                
                file << x << " " << y << " " << z << " " << i << std::endl;
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
                    
                    setBlock(nums[0],nums[1],nums[2],nums[3]);
                    
                    nums.clear();
        }
    }
    // Check for eof
    if (file.eof())
        std::cout << "Reached end of file." << std::endl;
    else
        std::cerr << "Error: File reading failed!" << std::endl;
    
    updateLighting(); 
    
    generateMesh();
    
    file.close();
}
