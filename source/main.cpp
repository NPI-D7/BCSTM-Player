#include "BCSTM.hpp"
#include "Menu.hpp"
#include <ResultDecoder.hpp>
#include <citro3d.h>
#include <stdio.h>

// #include "TitleManager.hpp"

// RenderD7::Image img;

void InitColors() {
  nlc::color_storage::Add(nlc::color_t("#ffffff"), "white");
  nlc::color_storage::Add(nlc::color_t("#eeeeee"), "style_white");
  nlc::color_storage::Add(nlc::color_t("#111111"), "style_black");
  nlc::color_storage::Add(nlc::color_t("#222222"), "style_black2");
  nlc::color_storage::Add(nlc::color_t("#333333"), "style_black3");
  nlc::color_storage::Add(nlc::color_t("#666666"), "style_black4");
  nlc::color_storage::Add(nlc::color_t("#cccccc"), "style_grey");
  nlc::color_storage::Add(nlc::color_t("#00ff00"), "green");
}

bool is3dsx = false;

static void getCurrentUsage() {
  u64 id;
  APT_GetProgramID(&id);
  is3dsx = (id != 0x0004000007893300);
}

bool splash = true;

void DoSplash() {
  while (splash) {
    nlc::nr::DrawBeg();
    nlc::nr2::DrawOnScreen();

    nlc::nr2::DrawRectSolid(0, 0, 400, 240, getcol("style_white"));
    nlc::nr2::DrawRectSolid(0, 0, 400, 26, getcol("style_black"));
    nlc::nr2::DrawText(5, 2, 0.7f, 0xffffffff, "BCSTM-Player -> Loading...");

    nlc::nr2::DrawOnScreen(1);
    nlc::nr2::DrawRectSolid(0, 0, 320, 240, getcol("style_black2"));
    C3D_FrameEnd(0);
    gspWaitForVBlank();
  }
}

extern BCSTM player;
bool exit_ = false;

void Bcstm_Loop() {
  while (true) {
    if (player.IsLoadet())
      player.tick();
    nlc::worker::sleep(1 * 100);
  }
  player.stop();
}

int main() {
  nlc::napp app("BCSTM-Player");
  //  nlc::ntrace::init("sdmc:/bcstm.trace");
  app.InitNdsp();
  nlc::nr::Init();

  getCurrentUsage();
  nlc::worker::push(InitColors, "colors");
  nlc::worker::push(DoSplash, "splash");
  nlc::nr2::AddFont("romfs:/roboto_regular.bcfnt", "sans");
  nlc::nr2::AddFont("romfs:/roboto_bold.bcfnt", "sans_bold");
  nlc::nr2::AddFont("romfs:/roboto_medium.bcfnt", "sans_medium");
  nlc::nr2::AddFont("romfs:/roboto_medium_italic.bcfnt", "sans_medil");
  std::filesystem::create_directories(
      std::filesystem::path("sdmc:/BCSTM-Player/cache/"));
  nlc::lang::load(app.GetSysLangKey());
  aptSetSleepAllowed(false);

  nlc::worker::push(Bcstm_Loop, "bcstm_loop");
  splash = false;
  nlc::scene::Load(std::make_unique<MMM>());

  while (app.Running() && !exit_) {
    hidScanInput();
    nlc::nr::DrawBeg();
    nlc::scene::doDraw();
    nlc::scene::doLogic();
    nlc::nr2::DrawOnScreen(0);
    if (!app.IsNdspLoadet())
      nlc::nr2::DrawText(5, 155, 0.6f, getcol("style_black"),
                         "<sdmc:/3ds/dspfirm.cdc> not found!\nThis means that "
                         "audio does not work!\nTo Fix this: Go "
                         "to\nRosalinamenu->Miscellaneous options...->Dump DSP "
                         "Firmware\nAnd Restart this App",
                         0, 0, "sans_medil");
    nlc::nr2::DrawOnScreen(1);
    // nlc::nr2::DrawText(0, 0, 0.7, nlc::color_storage::Get("white"),
    //                    std::to_string(C3D_GetProcessingTime()) + "ms");
    nlc::nr::DrawEnd();
    // player.tick();
  }
  // exit_ = true;
  //  nlc::ntrace::exit();
  // player.stop();
  nlc::nr2::UnloadFonts();
  nlc::nr::Exit();
  return 0;
}
