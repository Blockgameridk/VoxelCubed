#pragma once

const int CHUNK_SIZE = 16;
        
const int CHUNK_HEIGHT = 256;

inline int flatten1D(int x, int y, int z) {//Takes 3D Voxel Coordinates and Flatten it based on the Chunk Size and Height
    return x + (y * CHUNK_SIZE) + (z * CHUNK_SIZE * CHUNK_HEIGHT);
}
