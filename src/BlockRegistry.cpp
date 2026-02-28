#include "BlockRegistry.h"

BlockRegistry::BlockRegistry()
{
    
    Blocks.push_back(Block({0,0}, {2,0}, {3,0}, {3,0}, {3,0} ,{3,0})); //Grass
    
    Blocks.push_back(Block({1,0})); //Stone
    
    Blocks.push_back(Block({2,0})); //Dirt
    
    Blocks.push_back(Block({0,1})); //Cobblestone
    
    //Blocks.push_back(Block({}))
    
}