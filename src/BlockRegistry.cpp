#include "BlockRegistry.h"

BlockRegistry::BlockRegistry()
{
    
    Blocks.push_back(Block({0,0}, {2,0}, {3,0}, {3,0}, {3,0} ,{3,0}, false)); //Grass
    
    Blocks.push_back(Block({1,0}, false)); //Stone
    
    Blocks.push_back(Block({2,0}, false)); //Dirt
    
    Blocks.push_back(Block({0,1}, false)); //Cobblestone
    
    Blocks.push_back(Block({1,1}, false)); //Bedrock
    
    Blocks.push_back(Block({1,3}, true)); //Glass
    
    //Blocks.push_back(Block({}))
    
}