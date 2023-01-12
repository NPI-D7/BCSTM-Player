#include "BCSTM.hpp"
#include "Menu.hpp"
#include <citro3d.h>
#include <stdio.h>
//#include "TitleManager.hpp"

// RenderD7::Image img;

void InitColors() {
  nlc::color_storage::Add(nlc::color_t("#ffffff"), "white");
  nlc::color_storage::Add(nlc::color_t("#eeeeee"), "style_white");
  nlc::color_storage::Add(nlc::color_t("#111111"), "style_black");
  nlc::color_storage::Add(nlc::color_t("#222222"), "style_black2");
  nlc::color_storage::Add(nlc::color_t("#333333"), "style_black3");
  nlc::color_storage::Add(nlc::color_t("#666666"), "style_black4");
  nlc::color_storage::Add(nlc::color_t("#cccccc"), "style_grey");
}

extern BCSTM player;
bool exit_ = false;

void Bcstm_Loop() {
  while (!exit_) {
    player.tick();
    nlc::worker::sleep(1 * 1000);
  }
  return;
}

int main() {
  nlc::napp app("BCSTM-Player");
  //  nlc::ntrace::init("sdmc:/bcstm.trace");
  app.InitNdsp();
  InitColors();
  nlc::nr::Init();
  nlc::nr2::AddFont("romfs:/roboto_regular.bcfnt", "sans");
  nlc::nr2::AddFont("romfs:/roboto_bold.bcfnt", "sans_bold");
  nlc::nr2::AddFont("romfs:/roboto_medium.bcfnt", "sans_medium");
  nlc::nr2::AddFont("romfs:/roboto_medium_italic.bcfnt", "sans_medil");
  std::filesystem::create_directories(
      std::filesystem::path("sdmc:/BCSTM-Player/cache/"));
  nlc::lang::load(app.GetSysLangKey());
  aptSetSleepAllowed(false);

  nlc::worker::push(Bcstm_Loop, "bcstm_loop");
  nlc::scene::Load(std::make_unique<MMM>());

  while (app.Running() && !exit_) {
    hidScanInput();
    nlc::nr::DrawBeg();
    nlc::scene::doDraw();
    nlc::scene::doLogic();
    nlc::nr2::DrawOnScreen(1);
    nlc::nr2::DrawText(0, 0, 0.7, nlc::color_storage::Get("white"),
                       std::to_string(C3D_GetProcessingTime()) + "ms");
    nlc::nr::DrawEnd();
  }
  exit_ = true;
  // nlc::ntrace::exit();
  nlc::nr2::UnloadFonts();
  nlc::nr::Exit();
  player.stop();
  return 0;
}
