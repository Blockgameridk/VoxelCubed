#pragma once

#include "Block.h"

#include <vector>

class BlockRegistry {
    private:
    
    std::vector<Block> Blocks;
    
    public:
    
    BlockRegistry();
    //~BlockRegistry();
    
    Block getBlockData(int id) {
        
        return Blocks[id];
        
    }
    
    
};
