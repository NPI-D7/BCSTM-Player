//Menu.cpp
#include "Menu.hpp"
#include "BCSTM.hpp"
#include "TitleManager.hpp"

RenderD7::Image img;
//extern RenderD7::Image img;
BCSTM player;

extern RenderD7::Sheet sheet;
bool playing = false;
std::string currentlypl;
RenderD7::Checkbox box{40, 60, 20};

void clearCache()
{
    remove("sdmc:/BCSTM-Player/cache/sd");
    remove("sdmc:/BCSTM-Player/cache/nand");
}
void DrawFMBG()
{
    RenderD7::DrawRect(0, 49, 400, 18, RenderD7::Color::Hex("#CCCCCC"));
    RenderD7::DrawRect(0, 85, 400, 18, RenderD7::Color::Hex("#CCCCCC"));
    RenderD7::DrawRect(0, 121, 400, 18, RenderD7::Color::Hex("#CCCCCC"));
    RenderD7::DrawRect(0, 157, 400, 18, RenderD7::Color::Hex("#CCCCCC"));
    RenderD7::DrawRect(0, 193, 400, 18, RenderD7::Color::Hex("#CCCCCC"));
}

MMM::MMM()
{
    img.LoadPng("romfs:/gfx/bg.png");
}
void MMM::Draw(void) const
{
    RenderD7::OnScreen(Top);
    RenderD7::DrawRect(0, 0, 400, 240, RenderD7::Color::Hex("#444444"));
    img.Draw(0, 0);
    RenderD7::DrawRect(0, 0, 400, 26, RenderD7::Color::Hex("#222222", 200));
    RenderD7::DrawText(5, 2, 0.7f, RenderD7::Color::Hex("#FFFFFF"), "BCSTM-Player->" + RenderD7::Lang::get("MAINMENU"));
    RenderD7::DrawText(5, 30, 0.7f, RenderD7::Color::Hex("#FFFFFF"), "\n\n\nNdspFirmStatus: " + dspststus);
    RenderD7::DrawText(5, 50, 0.7f, RenderD7::Color::Hex("#FFFFFF"), "\n\n\n\uE003: Stop Player!\n\uE002 or \uE001: Back to MainMenu\nCard: " + CardStatus + "\nType: " + CardTypeStatus);
    if (playing)
    {
        RenderD7::DrawText(5, 218, 0.7f, RenderD7::Color::Hex("#111111"), "Playing: " + currentlypl);
    }
    RenderD7::OnScreen(Bottom);
    RenderD7::DrawRect(0, 0, 320, 240, RenderD7::Color::Hex("#222222"));
    RenderD7::DrawTObjects(this->buttons, RenderD7::Color::Hex("#333333"), RenderD7::Color::Hex("#FFFFFF"), this->Selection, RenderD7::Color::Hex("#444444"), RenderD7::Color::Hex("#333333", 100));
    RenderD7::DrawText(5, 2, 0.6f, RenderD7::Color::Hex("#FFFFFF"), "Framerate: " + RenderD7::GetFramerate());
}
void MMM::Logic(u32 hDown, u32 hHeld, u32 hUp, touchPosition touch)
{
    if(((hDown & KEY_DOWN && Selection < 7 && Selection !=3)))
    {
        Selection ++;
    }
    if (((hDown & KEY_UP && Selection > 0 && Selection != 4)))
    {
        Selection--;
    }
    if ((hDown & KEY_RIGHT && Selection < 4))
    {
        Selection += 4;
    }
    if ((hDown & KEY_LEFT && Selection > 3))
    {
        Selection -= 4;
    }
    if ((hDown & KEY_A && Selection == 0))
    {
        RenderD7::Scene::Load(std::make_unique<Browse>());
    }
  /*  if (hDown & KEY_A && Selection == 1)
    {
        RenderD7::Scene::Load(std::make_unique<Titles>());
    }*/
    if ((hDown & KEY_A && Selection == 2))
    {
        RenderD7::Scene::Load(std::make_unique<Credits>());
    }
    if ((hDown & KEY_A && Selection == 3))
    {
        RenderD7::ExitApp();
    }
    if (hDown & KEY_START)
    {
        RenderD7::ExitApp();
    }
    if (RenderD7::touchTObj(touch, buttons[2]))
    {
        RenderD7::Scene::Load(std::make_unique<Credits>());
    }
    if (RenderD7::touchTObj(touch, buttons[0]))
    {
        RenderD7::Scene::Load(std::make_unique<Browse>());
    }
   /* if (RenderD7::touchTObj(touch, buttons[1]))
    {
        RenderD7::Scene::Load(std::make_unique<Titles>());
    }*/
    if (RenderD7::touchTObj(touch, buttons[3]))
    {
        RenderD7::ExitApp();
    }
    if(hDown & KEY_Y)
    {
        if (playing)
        {
            currentlypl.clear();
            player.stop();
            playing = false;
        }
    }
 /*   if(hDown & KEY_L)
    {
        RenderD7::Scene::Load(std::make_unique<RomfsBrowse>());
    }*/
    if (hDown & KEY_R)
    {
        RenderD7::Error::DisplayError("BCSTM-Player", "Just for fun");
    }
    if (hDown & KEY_SELECT)
    {
        clearCache();
    }
}

Browse::Browse()
{
    //RenderD7::Msg::Display("BCSTM-Player", "Loading Directory: sd:/", Top);
    this->dircontent.clear();
    chdir("sdmc:/");
    std::vector<RenderD7::DirContent> temp;
    RenderD7::GetDirContentsExt(temp, {"bcstm"});
    for (uint i = 0; i < temp.size(); i++)
    {
        this->dircontent.push_back(temp[i]);
    } 
    this->changeddir = false;
}

void Browse::Draw(void) const
{
    RenderD7::OnScreen(Top);
    char path[PATH_MAX];
    getcwd(path, PATH_MAX);
    RenderD7::DrawRect(0, 0, 400, 240, RenderD7::Color::Hex("#EEEEEE"));
    RenderD7::DrawRect(0, 0, 400, 26, RenderD7::Color::Hex("#222222"));
    RenderD7::DrawText(5, 2, 0.7f, RenderD7::Color::Hex("#FFFFFF"), "BCSTM-Player->FileManager");
    DrawFMBG();
    RenderD7::DrawTextCentered(30, 216, 0.7f, RenderD7::Color::Hex("#111111"), path, 390);
    std::string dirs;
    int contentsss;
    contentsss = (int)dircontent.size();
    for (int i = this->dirsel < 9 ? 0 : this->dirsel - 9; (int)dircontent.size() && i < ((this->dirsel < 9) ? 10 : this->dirsel + 1); i++)
    {
        if (i == dirsel)
        {
            dirs += "> " + dircontent[i].name + "\n";
        }
        else
        {
            dirs += dircontent[i].name + "\n";
        } 
    }
    for (uint i = 0; i < ((dircontent.size() < 10) ? 10 - dircontent.size() : 0); i++) {
		dirs += "\n\n";
	}

    RenderD7::DrawText(10, 30, 0.6f, RenderD7::Color::Hex("#111111"), dirs.c_str());
    RenderD7::OnScreen(Bottom);
    RenderD7::DrawRect(0, 0, 320, 240, RenderD7::Color::Hex("#EEEEEE"));
    if(playing)
    {
        RenderD7::DrawText(5, 218, 0.7f, RenderD7::Color::Hex("#111111"), "Playing: " + currentlypl);
    }
    RenderD7::DrawText(5, 2, 0.7f, RenderD7::Color::Hex("#111111"), "DirContents: " + std::to_string(contentsss));
    RenderD7::DrawText(5, 2, 0.7f, RenderD7::Color::Hex("#111111"), "\nLoop: " + player.GetLoop());
    RenderD7::DrawText(5, 2, 0.7f, RenderD7::Color::Hex("#111111"), "\n\nLoopStart: " + player.GetLoopStart());
    RenderD7::DrawText(5, 2, 0.7f, RenderD7::Color::Hex("#111111"), "\n\n\nLoopEnd: " + player.GetLoopEnd());
}

void Browse::Logic(u32 hDown, u32 hHeld, u32 hUp, touchPosition touch)
{
    if (this->changeddir) {
        RenderD7::Msg::Display("BCSTM-Player", "Loading Directory", Top);
        
		this->dircontent.clear();
		std::vector<RenderD7::DirContent> temp;
		RenderD7::GetDirContentsExt(temp, {"bcstm"});

		for(uint i = 0; i < temp.size(); i++) {
			this->dircontent.push_back(temp[i]);
		}

		this->changeddir = false;
	}
    if (hDown & KEY_A)
    {
        char path[PATH_MAX];
		getcwd(path, PATH_MAX);
        if (this->dircontent.size() > 0) 
        {
			if (this->dircontent[dirsel].isDir) 
            {
				chdir(this->dircontent[this->dirsel].name.c_str());
				this->dirsel = 0;
				this->changeddir = true;
			} 
            else 
            {
                if (RenderD7::NameIsEndingWith(this->dircontent[this->dirsel].name, {"bcstm"}) && RenderD7::IsNdspInit())
                {
                    playing = false;
                    player.stop();
                    player.openFromFile(this->dircontent[this->dirsel].name);
                    player.play();
                    currentlypl = this->dircontent[this->dirsel].name;
                    playing = true;
                }
                else
                {
                    RenderD7::Error::DisplayError("BCSTM-Player", "Ndsp is not initialisized.\nPress Start to exit!");
                }
			}
		}
        else if (this->dircontent.size() == 0)
        {
            RenderD7::Msg::Display("BCSTM_Player->Error", "What are you trying to do?\nThis Directory is empty.", Top);
        }
    }
    if (hDown & KEY_B)
    {
        char path[PATH_MAX];
		getcwd(path, PATH_MAX);

		if (strcmp(path, "sdmc:/") == 0 || strcmp(path, "/") == 0) 
        {
			RenderD7::Scene::Back();
		} 
        else 
        {
			chdir("..");
			dirsel = 0;
			changeddir = true;
		}
    }
    if (hDown & KEY_X)
    {
        RenderD7::Scene::Load(std::make_unique<MMM>());
    }
    if (hDown & KEY_UP && dirsel > 0)
    {
        dirsel--;
    }
    if (hDown & KEY_DOWN && dirsel < (int)dircontent.size() - 1)
    {
        dirsel++;
    }
    if (hDown & KEY_LEFT && dirsel - 6 > 0)
    {
        dirsel -= 6;
    }
    if (hDown & KEY_RIGHT && dirsel + 6 < (int)dircontent.size() - 1)
    {
        dirsel += 6;
    }
    if (hDown & KEY_R)
    {
        RenderD7::Error::DisplayError("BCSTM-Player", "Just for fun");
    }
}

Credits::Credits()
{
    //img.LoadPng("romfs:/gfx/bg.png");
}
void Credits::Draw(void) const
{
    RenderD7::OnScreen(Top);
    RenderD7::DrawRect(0, 0, 400, 240, RenderD7::Color::Hex("#EEEEEE"));
    //img.Draw(0, 0);
    RenderD7::DrawRect(0, 0, 400, 26, RenderD7::Color::Hex("#222222"));
    RenderD7::DrawRect(0, 240, 400, -26, RenderD7::Color::Hex("#222222"));
    std::string stdzeitverschwendung = "Version: 1.0.0";
    std::string stdzeitverschwendung2 = "nightly: " V_STRING;
    RenderD7::DrawTextCentered(40, 2, 0.7f, RenderD7::Color::Hex("#FFFFFF"), "BCSTM-Player->Credits", 400);
    RenderD7::DrawText(5, 218, 0.7f, RenderD7::Color::Hex("#FFFFFF"), stdzeitverschwendung);
    RenderD7::DrawTextLeft(395, 218, 0.7f, RenderD7::Color::Hex("#FFFFFF"), stdzeitverschwendung2);
    RenderD7::DrawText(5, 30, 0.7f, RenderD7::Color::Hex("#000000"), "- Tobi-D7\n- devkitpro\n- citro2d\n- citro3d\n\n" + RenderD7::Lang::get("TPWMR"));
    RenderD7::OnScreen(Bottom);
    RenderD7::DrawRect(0, 0, 320, 240, RenderD7::Color::Hex("#222222"));
    RenderD7::DrawImageFromSheet(&sheet, 0, 15, 70);
    RenderD7::DrawTextCentered(5, 218, 0.7f, RenderD7::Color::Hex("#FFFFFF"), "RenderD7: Version: " RENDERD7VSTRING, 390);
    RenderD7::DrawText(5, 2, 0.6f, RenderD7::Color::Hex("#FFFFFF"), "Framerate: " + RenderD7::GetFramerate());
    RenderD7::DrawCheckbox(box);
}

void Credits::Logic(u32 hDown, u32 hHeld, u32 hUp, touchPosition touch)
{
    if (hDown & KEY_B)
    {
        RenderD7::Scene::Back();
    }
}
int abc = 0;
Titles::Titles()
{
    TitleManager::ScanSD("sdmc:/BCSTM-Player/");
    maxtitles = (int)TitleManager::sdtitles.size();
    TitleManager::ScanNand("sdmc:/BCSTM-Player/");
    abc = (int)TitleManager::nandtitles.size();
}

void Titles::Draw(void) const
{
    RenderD7::OnScreen(Top);
    RenderD7::DrawRect(0, 0, 400, 240, RenderD7::Color::Hex("#EEEEEE"));
    DrawFMBG();
    std::string titles;
    for (int i = this->selection < 9 ? 0 : this->selection - 9;TitleManager::sdtitles.size() && i < ((this->selection < 9) ? 10 : this->selection + 1); i++)
    {
        if (i == selection)
        {
            titles += "> " + TitleManager::sdtitles[i]->name() + "\n";
        }
        else
        {
            titles += TitleManager::sdtitles[i]->name() + "\n";
        } 
    }
    for (uint i = 0; i < ((TitleManager::sdtitles.size() < 10) ? 10 - TitleManager::sdtitles.size() : 0); i++) {
		titles += "\n\n";
	}

    RenderD7::DrawText(10, 30, 0.6f, RenderD7::Color::Hex("#111111"), titles.c_str());
    RenderD7::OnScreen(Bottom);
    RenderD7::DrawText(5, 2, 0.7f, RenderD7::Color::Hex("#111111"), "Titles:\nNand: " + std::to_string(abc) + "SD: " + std::to_string(maxtitles));
    //C2D_DrawImageAt(TitleManager::sdtitles[selection]->icon(), 30, 30, 0.5f, nullptr);
}

void Titles::Logic(u32 hDown, u32 hHeld, u32 hUp, touchPosition touch)
{
    if (hDown & KEY_B)
    {
        RenderD7::Scene::Back();
    }
    if (hDown & KEY_A)
    {
        RenderD7::Msg::Display("BCSTM-Player", "Mounting romfs of " + TitleManager::sdtitles[selection]->name(), Top);
        romfsUnmount("h");
        romfsMountFromTitle(TitleManager::sdtitles[selection]->ID(), TitleManager::sdtitles[selection]->mediatype(), "h");
    }
    if (hDown & KEY_DOWN && selection < maxtitles)
    {
        selection++;
    }
    if (hDown & KEY_UP && selection > 0)
    {
        selection--;
    }
}

RomfsBrowse::RomfsBrowse()
{
    RenderD7::Msg::Display("BCSTM-Player", "Loading Directory: h:/", Top);
    chdir("h:/");
    std::vector<RenderD7::DirContent> temp;
    RenderD7::GetDirContentsExt(temp, {"bcstm"});
    for (uint i = 0; i < temp.size(); i++)
    {
        this->dircontent.push_back(temp[i]);
    } 
    this->changeddir = false;
}

void RomfsBrowse::Draw(void) const
{
    RenderD7::OnScreen(Top);
    char path[PATH_MAX];
    getcwd(path, PATH_MAX);
    RenderD7::DrawRect(0, 0, 400, 240, RenderD7::Color::Hex("#EEEEEE"));
    RenderD7::DrawRect(0, 0, 400, 26, RenderD7::Color::Hex("#222222"));
    RenderD7::DrawText(5, 2, 0.7f, RenderD7::Color::Hex("#FFFFFF"), "BCSTM-Player->FileManager");
    DrawFMBG();
    RenderD7::DrawTextCentered(30, 216, 0.7f, RenderD7::Color::Hex("#111111"), path, 390);
    std::string dirs;
    for (int i = this->dirsel < 9 ? 0 : this->dirsel - 9; (int)dircontent.size() && i < ((this->dirsel < 9) ? 10 : this->dirsel + 1); i++)
    {
        if (i == dirsel)
        {
            dirs += "> " + this->dircontent[i].name + "\n";
        }
        else
        {
            dirs += this->dircontent[i].name + "\n";
        } 
    }
    for (uint i = 0; i < ((this->dircontent.size() < 10) ? 10 - this->dircontent.size() : 0); i++) {
		dirs += "\n\n";
	}

    RenderD7::DrawText(10, 30, 0.6f, RenderD7::Color::Hex("#111111"), dirs.c_str());
    RenderD7::OnScreen(Bottom);
    RenderD7::DrawRect(0, 0, 320, 240, RenderD7::Color::Hex("#EEEEEE"));
    if(playing)
    {
        RenderD7::DrawText(5, 218, 0.7f, RenderD7::Color::Hex("#111111"), "Playing: " + currentlypl);
    }
}

void RomfsBrowse::Logic(u32 hDown, u32 hHeld, u32 hUp, touchPosition touch)
{
    if (this->changeddir) {
        RenderD7::Msg::Display("BCSTM-Player", "Loading Directory", Top);
        
		this->dircontent.clear();
		std::vector<RenderD7::DirContent> temp;
		RenderD7::GetDirContentsExt(temp, {"bcstm"});

		for(uint i = 0; i < temp.size(); i++) {
			this->dircontent.push_back(temp[i]);
		}

		this->changeddir = false;
	}
    if (hDown & KEY_A)
    {
        char path[PATH_MAX];
		getcwd(path, PATH_MAX);
        if (this->dircontent.size() > 0) 
        {
			if (this->dircontent[dirsel].isDir) 
            {
				chdir(this->dircontent[this->dirsel].name.c_str());
				this->dirsel = 0;
				this->changeddir = true;
			} 
            else 
            {
                if (RenderD7::NameIsEndingWith(this->dircontent[this->dirsel].name, {"bcstm"}))
                {
                    playing = false;
                    player.stop();
                    player.openFromFile(this->dircontent[this->dirsel].name);
                    player.play();
                    currentlypl = this->dircontent[this->dirsel].name;
                    playing = true;
                }
			}
		}
        else if (this->dircontent.size() == 0)
        {
            RenderD7::Msg::Display("BCSTM_Player->Error", "What are you trying to do?\nThis Directory is empty.", Top);
        }
    }
    if (hDown & KEY_B)
    {
        char path[PATH_MAX];
		getcwd(path, PATH_MAX);

		if (strcmp(path, "h:/") == 0 || strcmp(path, "/") == 0) 
        {
			RenderD7::Scene::Back();
		} 
        else 
        {
			chdir("..");
			dirsel = 0;
			changeddir = true;
		}
    }
    if (hDown & KEY_X)
    {
        RenderD7::Scene::Load(std::make_unique<MMM>());
    }
    if (hDown & KEY_UP && dirsel > 0)
    {
        dirsel--;
    }
    if (hDown & KEY_DOWN && dirsel < (int)dircontent.size() - 1)
    {
        dirsel++;
    }
    if (hDown & KEY_LEFT && dirsel - 6 > 0)
    {
        dirsel -= 6;
    }
    if (hDown & KEY_RIGHT && dirsel + 6 < (int)dircontent.size() - 1)
    {
        dirsel += 6;
    }
}
