#pragma once

#include "raylib.h"

struct BlockUV {
    float U;

    float V;
};


class Block{
    
    private:
        BlockUV _TopUV;
        
        BlockUV _FrontUV;
        
        BlockUV _BackUV;
        
        BlockUV _BottomUV;
        
        BlockUV _LeftUV;
        
        BlockUV _RightUV;
        
        bool _translucent;
    
    public:
        Block(BlockUV TopUV, BlockUV BottomUV, BlockUV LeftUV, BlockUV RightUV, BlockUV FrontUV, BlockUV BackUV, bool translucent)  {//TOP // BOTTOM //LEFT // RIGHT // FRONT //BACk
            _TopUV = TopUV;
            
            _BottomUV = BottomUV;
            
            _LeftUV = LeftUV;
            
            _RightUV = RightUV;
            
            _FrontUV = FrontUV;
            
            _BackUV = BackUV;
            
            _translucent = translucent;
        }
        
        Block(BlockUV uv, bool translucent) {
            _TopUV = uv;
            
            _BottomUV = uv;
            
            _LeftUV = uv;
            
            _RightUV = uv;
            
            _FrontUV = uv;
            
            _BackUV = uv;
            
            _translucent = translucent;
        }
        
        inline BlockUV GetFaceUV(int faceID) { // TOP // BOTTOM // LEFT // RIGHT // FRONT // BACK
            switch(faceID) {
                case 0:
                    return _TopUV;
                    
                    break;
                    
                case 1:
                    return _BottomUV;
                    
                    break;
                    
                case 2:
                    return _LeftUV;
                    
                    break;
                    
                case 3:
                    return _RightUV;
                    
                    break;
                    
                case 4:
                    return _FrontUV;
                    
                    break;
                    
                case 5:
                
                    return _BackUV;
                    
                    break;
            }
        }
        
        inline bool GetTranslucentState() {
            return _translucent;
        }
    
};