#pragma once

#include "Globals.h"

class Chunk {
  private:
        
        int _X = 0;
        
        int _Y = 0;
        
    public:
        int* Blocks = new int[CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT];
        
        int* Lighting = new int[CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT];
    
        Chunk(int X, int Y) : _X(X), _Y(Y) {
            for (int x = 0; x < CHUNK_SIZE; x++) {
                for (int y = 0; y < CHUNK_HEIGHT; y++) {
                    for (int z = 0; z < CHUNK_SIZE; z++) {
                        
                        int index = flatten1D(x,y,z);
                        
                        Blocks[index] = 0;
                        
                        if (y < 24) {
                            Blocks[index] = 2;
                        }
                        
                    }
                }
            }
        }
        
        void generateMesh(Chunk Left, Chunk Right, Chunk Front, Chunk Back) { //Remember to remind me to never touch voxel game coding again
            
        }
        
        Vector2 getPosition() {
            return {(float)_X,(float)_Y};
        }
};