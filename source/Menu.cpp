// Menu.cpp
#include "Menu.hpp"
#include "BCSTM.hpp"
#include <log.hpp>
//#include "TitleManager.hpp"

#ifdef V_STRING
#else
#define V_STRING ""
#endif

bool touchTObj(touchPosition touch, TObject button) {
  if (touch.px >= button.x && touch.px <= (button.x + button.w) &&
      touch.py >= button.y && touch.py <= (button.y + button.h))
    return true;
  else
    return false;
}

BCSTM player;

extern bool exit_;

bool playing = false;
std::string currentlypl;

nlc::image background;

// RenderD7::Checkbox box{40, 60, 20};

void clearCache() {
  remove("sdmc:/BCSTM-Player/cache/sd");
  remove("sdmc:/BCSTM-Player/cache/nand");
}
void DrawFMBG() {
  nlc::nr2::DrawRectSolid(0, 49, 400, 18, getcol("style_grey"));
  nlc::nr2::DrawRectSolid(0, 85, 400, 18, getcol("style_grey"));
  nlc::nr2::DrawRectSolid(0, 121, 400, 18, getcol("style_grey"));
  nlc::nr2::DrawRectSolid(0, 157, 400, 18, getcol("style_grey"));
  nlc::nr2::DrawRectSolid(0, 193, 400, 18, getcol("style_grey"));
}

Log llg;

MMM::MMM() {
  llg.Init("sdmc:/log.lg");
  llg.Write(std::string(__FILE__) + ":" + std::to_string(__LINE__));
  background.LoadFile("romfs:/gfx/bg.png");
}
void MMM::Draw(void) const {
  nlc::nr2::DrawOnScreen(0);
  nlc::nr2::DrawRectSolid(0, 0, 400, 240, getcol("style_white"));
  // img.Draw(0, 0);
  nlc::nr2::DrawRectSolid(0, 0, 400, 26, getcol("style_black"));
  nlc::nr2::DrawText(5, 2, 0.7f, getcol("white"),
                     "BCSTM-Player->" + nlc::lang::get("MAINMENU"), 0, 0,
                     "sans_bold");
  nlc::nr2::DrawText(
      5, 30, 0.7f, getcol("style_black"),
      "\uE003: Stop Player!\n\uE002 or \uE001: Back to MainMenu");
  // std::vector<std::string> tsks = nlc::worker::GetTasks();
  // for (size_t i = 0; i < tsks.size(); i++) {
  //   nlc::nr2::DrawText(5, 30 + i * 30, 0.7f, getcol("style_black"), tsks[i]);
  // }

  if (playing) {
    nlc::nr2::DrawText(5, 218, 0.7f, getcol("style_black"),
                       "Playing: " + currentlypl);
  }
  nlc::nr2::DrawOnScreen(1);
  nlc::nr2::DrawRectSolid(0, 0, 320, 240, getcol("style_black2"));
  int hsel__ = 0;
  for (auto const &it : buttons) {
    if (hsel__ == Selection)
      nlc::nr2::DrawRectSolid(it.x - 2, it.y - 2, it.w + 4, it.h + 4,
                              getcol("style_black4"));
    nlc::nr2::DrawRectSolid(it.x, it.y, it.w, it.h, getcol("style_black3"));
    nlc::nr2::DrawText(it.ftx + it.x + it.w / 4, it.fty + it.y + it.h / 4, 0.7f,
                       getcol("white"), it.name, 0, 0, "sans_bold");
    hsel__++;
  }
}
void MMM::Logic() {
  touchPosition touch_;
  hidTouchRead(&touch_);
  if (((hidKeysDown() & KEY_DOWN && Selection < 4 && Selection != 3))) {
    Selection++;
  }
  if (((hidKeysDown() & KEY_UP && Selection > 0 && Selection != 4))) {
    Selection--;
  }
  if ((hidKeysDown() & KEY_A && Selection == 0)) {
    nlc::scene::Load(std::make_unique<Browse>());
  }
  // if (hidKeysDown() & KEY_A && Selection == 1)
  //{
  //     nlc::scene::Load(std::make_unique<Titles>());
  // }
  if ((hidKeysDown() & KEY_A && Selection == 1)) {
    nlc::scene::Load(std::make_unique<Credits>());
  }
  if ((hidKeysDown() & KEY_A && Selection == 2)) {
    exit_ = true;
  }
  if ((hidKeysDown() & KEY_TOUCH && touchTObj(touch_, buttons[0]))) {
    nlc::scene::Load(std::make_unique<Browse>());
  }
  // if (hidKeysDown() & KEY_TOUCH && touchTObj(touch_, buttons[1]))
  //{
  //     nlc::scene::Load(std::make_unique<Titles>());
  // }
  if ((hidKeysDown() & KEY_TOUCH && touchTObj(touch_, buttons[1]))) {
    nlc::scene::Load(std::make_unique<Credits>());
  }
  if ((hidKeysDown() & KEY_TOUCH && touchTObj(touch_, buttons[2]))) {
    exit_ = true;
  }
  if (hidKeysDown() & KEY_START) {
    exit_ = true;
  }
  if (hidKeysDown() & KEY_Y) {
    if (playing) {
      currentlypl.clear();
      player.stop();
      playing = false;
    }
  }
}

Browse::Browse() {
  dir = "sdmc:/";
  dircontent = nlc::fsys::GetDirContentsExt(dir, {"bcstm"});
  this->changeddir = false;
}

void Browse::Draw(void) const {
  // nlc::ntrace::trace_begin("Browse", "Draw");
  nlc::nr2::DrawOnScreen(0);
  nlc::nr2::DrawRectSolid(0, 0, 400, 240, getcol("style_white"));
  nlc::nr2::DrawRectSolid(0, 0, 400, 26, getcol("style_black2"));
  nlc::nr2::DrawText(5, 2, 0.7f, getcol("white"), "BCSTM-Player->FileManager",
                     0, 0, "sans_bold");
  DrawFMBG();
  nlc::nr2::DrawText(5, 216, 0.7f, getcol("style_black"), dir, 0, 0,
                     "sans_medil");
  std::string dirs;
  std::vector<std::string> dlst;
  for (int i = this->dirsel < 9 ? 0 : this->dirsel - 9;
       (int)dircontent.size() &&
       i < ((this->dirsel < 9) ? 10 : this->dirsel + 1);
       i++) {
    if (i == dirsel) {
      dlst.push_back("> " + dircontent[i].name + "\n");
    } else {
      dlst.push_back(dircontent[i].name + "\n");
    }
  }
  // for (int i = 0;
  //      i < (((int)dircontent.size() < 10) ? 10 - (int)dircontent.size() : 0);
  //      i++) {
  //   dirs += "\n\n";
  // }
  for (auto const &it : dlst) {
    dirs += it;
  }

  nlc::nr2::DrawText(10, 28, 0.7f, getcol("style_black"), dirs.c_str(), 0, 0,
                     "sans");
  nlc::nr2::DrawOnScreen(1);
  nlc::nr2::DrawRectSolid(0, 0, 320, 240, getcol("style_white"));
  if (playing) {
    nlc::nr2::DrawText(5, 218, 0.7f, getcol("style_black"),
                       "Playing: " + currentlypl);
  }
  nlc::nr2::DrawText(5, 2, 0.7f, getcol("style_black"),
                     "DirContents: " + std::to_string(dircontent.size()), 0, 0,
                     "sans_medium");
  nlc::nr2::DrawText(5, 2, 0.7f, getcol("style_black"),
                     "\nLoop: " + player.GetLoop(), 0, 0, "sans_medium");
  // nlc::ntrace::trace_end("Browse", "Draw");
}

void Browse::Logic() {
  if (this->changeddir) {

    this->dircontent.clear();
    std::vector<nlc::fsys::DirEntry> temp =
        nlc::fsys::GetDirContentsExt(dir, {"bcstm"});

    for (uint i = 0; i < temp.size(); i++) {
      this->dircontent.push_back(temp[i]);
    }

    this->changeddir = false;
  }
  if (hidKeysDown() & KEY_A) {
    if (this->dircontent.size() > 0) {
      if (this->dircontent[dirsel]._is_dir) {
        chdir(this->dircontent[this->dirsel].name.c_str());
        if (dir.substr(dir.length() - 1, 1) != "/")
          dir += "/";
        dir += this->dircontent[this->dirsel].name;
        this->dircontent.clear();
        this->dirsel = 0;
        this->changeddir = true;
      } else {
        if (nlc::st::NameIsEndingWith(this->dircontent[this->dirsel].name,
                                      {"bcstm"})) {
          playing = false;
          player.stop();
          player.openFromFile(this->dircontent[this->dirsel].name);
          player.play();
          currentlypl = this->dircontent[this->dirsel].name;
          playing = true;
        }
      }
    } else if (this->dircontent.size() == 0) {
      // RenderD7::Msg::Display("BCSTM_Player->Error", "What are you trying to
      // do?\nThis Directory is empty.", Top);
    }
  }
  if (hidKeysDown() & KEY_B) {

    if (strcmp(dir.c_str(), "sdmc:/") == 0 || strcmp(dir.c_str(), "/") == 0) {
      nlc::scene::Back();
    } else {
      dir = nlc::fsys::GetParentPath(dir, "sdmc:/");
      dirsel = 0;
      changeddir = true;
    }
  }
  if (hidKeysDown() & KEY_X) {
    nlc::scene::Load(std::make_unique<MMM>());
  }
  if (hidKeysDown() & KEY_UP && dirsel > 0) {
    dirsel--;
  }
  if (hidKeysDown() & KEY_DOWN && dirsel < (int)dircontent.size() - 1) {
    dirsel++;
  }
  if (hidKeysDown() & KEY_LEFT && dirsel - 6 > 0) {
    dirsel -= 6;
  }
  if (hidKeysDown() & KEY_RIGHT && dirsel + 6 < (int)dircontent.size() - 1) {
    dirsel += 6;
  }
}

Credits::Credits() {}
void Credits::Draw(void) const {
  nlc::nr2::DrawOnScreen(0);
  nlc::nr2::DrawRectSolid(0, 0, 400, 240, getcol("style_white"));
  // img.Draw(0, 0);
  nlc::nr2::DrawRectSolid(0, 0, 400, 26, getcol("style_black2"));
  nlc::nr2::DrawRectSolid(0, 240, 400, -26, getcol("style_black2"));
  std::string stdzeitverschwendung = "Version: 1.2.1";
  std::string stdzeitverschwendung2 = "nightly: " V_STRING;
  nlc::nr2::DrawText(0, 2, 0.7f, getcol("white"), "BCSTM-Player->Credits", 0, 0,
                     "sans_bold");
  nlc::nr2::DrawText(5, 218, 0.7f, getcol("white"), stdzeitverschwendung, 0, 0,
                     "sans_bold");
  nlc::nr2::DrawTextRight(395, 218, 0.7f, getcol("white"),
                          stdzeitverschwendung2, 0, 0, "sans_bold");
  nlc::nr2::DrawText(5, 30, 0.7f, nlc::color_t("#000000").GetRGBA(),
                     "- Tobi-D7\n- devkitpro\n- citro2d\n- citro3d\n- nlc-libs",
                     0, 0, "sans_medium");
  nlc::nr2::DrawOnScreen(1);
  nlc::nr2::DrawRectSolid(0, 0, 320, 240, getcol("style_black2"));
  // RenderD7::DrawText(5, 2, 0.6f, nlc::color_t("#FFFFFF"), "Framerate: " +
  // RenderD7::GetFramerate());
  // RenderD7::DrawCheckbox(box);
  // RenderD7::DrawNFRect(20, 20, 50, 50, C2D_Color32(255, 255, 255, 255), 3);
}

void Credits::Logic() {
  if (hidKeysDown() & KEY_B) {
    nlc::scene::Back();
  }
}
// int abc = 0;
// Titles::Titles()
//{
//     TitleManager::ScanSD("sdmc:/BCSTM-Player/");
//     maxtitles = (int)TitleManager::sdtitles.size();
//     /*TitleManager::ScanNand("sdmc:/BCSTM-Player/");
//     abc = (int)TitleManager::nandtitles.size();*/
// }
//
// void Titles::Draw(void) const
//{
//     RenderD7::OnScreen(Top);
//     nlc::nr2::DrawRectSolid(0, 0, 400, 240, getcol("style_white"));
//     DrawFMBG();
//     std::string titles;
//     for (int i = this->selection < 9 ? 0 : this->selection -
//     9;TitleManager::sdtitles.size() && i < ((this->selection < 9) ? 10 :
//     this->selection + 1); i++)
//     {
//         if (i == selection)
//         {
//             titles += "> " + TitleManager::sdtitles[i]->name() + "\n";
//         }
//         else
//         {
//             titles += TitleManager::sdtitles[i]->name() + "\n";
//         }
//     }
//     for (uint i = 0; i < ((TitleManager::sdtitles.size() < 10) ? 10 -
//     TitleManager::sdtitles.size() : 0); i++) {
//		titles += "\n\n";
//	}
//
//     RenderD7::DrawText(10, 30, 0.6f, getcol("style_black"),
//     titles.c_str()); RenderD7::OnScreen(Bottom); RenderD7::DrawText(5, 2,
//     0.7f, getcol("style_black"), "Titles:\nNand: " + std::to_string(abc) +
//     " SD: " + std::to_string(maxtitles));
//     //C2D_DrawImageAt(TitleManager::sdtitles[selection]->icon(), 30, 30,
//     0.5f, nullptr);
// }
//
// void Titles::Logic()
//{
//     if (hidKeysDown() & KEY_B)
//     {
//         nlc::scene::Back();
//     }
//     if (hidKeysDown() & KEY_A)
//     {
//         RenderD7::Msg::Display("BCSTM-Player", "Mounting romfs of " +
//         TitleManager::sdtitles[selection]->name(), Top); romfsUnmount("h");
//         romfsMountFromTitle(TitleManager::sdtitles[selection]->ID(),
//         TitleManager::sdtitles[selection]->mediatype(), "h");
//     }
//     if (hidKeysDown() & KEY_DOWN && selection <
//     (int)TitleManager::sdtitles.size())
//     {
//         selection++;
//     }
//     if (hidKeysDown() & KEY_UP && selection > 0)
//     {
//         selection--;
//     }
// }
//
// RomfsBrowse::RomfsBrowse()
//{
//     RenderD7::Msg::Display("BCSTM-Player", "Loading Directory: h:/", Top);
//     chdir("romfs:/");
//     std::vector<nlc::fsys::DirEntry> temp;
//     RenderD7::GetDirContentsExt(temp, {"bcstm"});
//     for (uint i = 0; i < temp.size(); i++)
//     {
//         this->dircontent.push_back(temp[i]);
//     }
//     this->changeddir = false;
// }
//
// void RomfsBrowse::Draw(void) const
//{
//     RenderD7::OnScreen(Top);
//     char path[PATH_MAX];
//     getcwd(path, PATH_MAX);
//     nlc::nr2::DrawRectSolid(0, 0, 400, 240, getcol("style_white"));
//     nlc::nr2::DrawRectSolid(0, 0, 400, 26, getcol("style_black2"));
//     RenderD7::DrawText(5, 2, 0.7f, nlc::color_t("#FFFFFF"),
//     "BCSTM-Player->FileManager"); DrawFMBG(); RenderD7::DrawTextCentered(30,
//     216, 0.7f, getcol("style_black"), path, 390); std::string dirs; for
//     (int i = this->dirsel < 9 ? 0 : this->dirsel - 9; (int)dircontent.size()
//     && i < ((this->dirsel < 9) ? 10 : this->dirsel + 1); i++)
//     {
//         if (i == dirsel)
//         {
//             dirs += "> " + this->dircontent[i].name + "\n";
//         }
//         else
//         {
//             dirs += this->dircontent[i].name + "\n";
//         }
//     }
//     for (uint i = 0; i < ((this->dircontent.size() < 10) ? 10 -
//     this->dircontent.size() : 0); i++) {
//		dirs += "\n\n";
//	}
//
//     RenderD7::DrawText(10, 30, 0.6f, getcol("style_black"), dirs.c_str());
//     RenderD7::OnScreen(Bottom);
//     nlc::nr2::DrawRectSolid(0, 0, 320, 240, getcol("style_white"));
//     if(playing)
//     {
//         RenderD7::DrawText(5, 218, 0.7f, getcol("style_black"), "Playing: "
//         + currentlypl);
//     }
// }
//
// void RomfsBrowse::Logic()
//{
//     if (this->changeddir) {
//         RenderD7::Msg::Display("BCSTM-Player", "Loading Directory", Top);
//
//		this->dircontent.clear();
//		std::vector<nlc::fsys::DirEntry> temp;
//		RenderD7::GetDirContentsExt(temp, {"bcstm"});
//
//		for(uint i = 0; i < temp.size(); i++) {
//			this->dircontent.push_back(temp[i]);
//		}
//
//		this->changeddir = false;
//	}
//     if (hidKeysDown() & KEY_A)
//     {
//         char path[PATH_MAX];
//		getcwd(path, PATH_MAX);
//         if (this->dircontent.size() > 0)
//         {
//			if (this->dircontent[dirsel].isDir)
//             {
//				chdir(this->dircontent[this->dirsel].name.c_str());
//				this->dirsel = 0;
//				this->changeddir = true;
//			}
//             else
//             {
//                 if
//                 (RenderD7::NameIsEndingWith(this->dircontent[this->dirsel].name,
//                 {"bcstm"}))
//                 {
//                     playing = false;
//                     player.stop();
//                     player.openFromFile(this->dircontent[this->dirsel].name);
//                     player.play();
//                     currentlypl = this->dircontent[this->dirsel].name;
//                     playing = true;
//                 }
//			}
//		}
//         else if (this->dircontent.size() == 0)
//         {
//             RenderD7::Msg::Display("BCSTM_Player->Error", "What are you
//             trying to do?\nThis Directory is empty.", Top);
//         }
//     }
//     if (hidKeysDown() & KEY_B)
//     {
//         char path[PATH_MAX];
//		getcwd(path, PATH_MAX);
//
//		if (strcmp(path, "romfs:/") == 0 || strcmp(path, "/") == 0)
//         {
//			nlc::scene::Back();
//		}
//         else
//         {
//			chdir("..");
//			dirsel = 0;
//			changeddir = true;
//		}
//     }
//     if (hidKeysDown() & KEY_X)
//     {
//         nlc::scene::Load(std::make_unique<MMM>());
//     }
//     if (hidKeysDown() & KEY_UP && dirsel > 0)
//     {
//         dirsel--;
//     }
//     if (hidKeysDown() & KEY_DOWN && dirsel < (int)dircontent.size() - 1)
//     {
//         dirsel++;
//     }
//     if (hidKeysDown() & KEY_LEFT && dirsel - 6 > 0)
//     {
//         dirsel -= 6;
//     }
//     if (hidKeysDown() & KEY_RIGHT && dirsel + 6 < (int)dircontent.size() - 1)
//     {
//         dirsel += 6;
//     }
// }
//