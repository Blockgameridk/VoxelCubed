#include "MainMenuScene.h"

MainMenuScene::MainMenuScene(std::shared_ptr<SceneManager> sm)
{
    _sm = sm;
    
    ctx = InitNuklear(fontSize);
    
    
}
void MainMenuScene::Tick()
{
   UpdateNuklear(ctx);
   
   if (nk_begin(ctx, "Title", nk_rect(GetScreenWidth() / 2,GetScreenHeight() / 2,220,440),  NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_CLOSABLE)) {
    nk_layout_row_static(ctx, 50.0f, 100.0f, 1);
    
    if (nk_button_label(ctx,"Singleplayer")) {
        startGame = true;
    }
    
   }
   
   nk_end(ctx);
   
   if (startGame == true) {
     _sm->SwitchScene("GameScene")  ;
     return;
   }
   
   BeginDrawing();
   
   ClearBackground(WHITE);
   
   DrawNuklear(ctx);
   
   EndDrawing();
        //struct nk_style *style = &ctx->style;
    
    //nk_style_push_color(ctx, &style->window.background, nk_rgba(0,0,0,0));
    
    //nk_style_push_style_item(ctx, &style->window.fixed_background, nk_style_item_color(nk_rgba(0,0,0,0)));
}
void MainMenuScene::Exit()
{
    UnloadNuklear(ctx);
}