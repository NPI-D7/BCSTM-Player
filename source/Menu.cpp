
#include "Menu.hpp"
#include "BCSTM.hpp"

BCSTM player;

extern RenderD7::Sheet sheet;
bool playing = false;
std::string currentlypl;

void DrawFMBG()
{
    RenderD7::DrawRect(0, 49, 400, 18, RenderD7::Color::Hex("#CCCCCC"));
    RenderD7::DrawRect(0, 85, 400, 18, RenderD7::Color::Hex("#CCCCCC"));
    RenderD7::DrawRect(0, 121, 400, 18, RenderD7::Color::Hex("#CCCCCC"));
    RenderD7::DrawRect(0, 157, 400, 18, RenderD7::Color::Hex("#CCCCCC"));
    RenderD7::DrawRect(0, 193, 400, 18, RenderD7::Color::Hex("#CCCCCC"));
}
void MMM::Draw(void) const
{
    
    RenderD7::OnScreen(Top);
    RenderD7::DrawRect(0, 0, 400, 240, RenderD7::Color::Hex("#EEEEEE"));
    RenderD7::DrawRect(0, 0, 400, 26, RenderD7::Color::Hex("#222222"));
    RenderD7::DrawText(5, 2, 0.7f, RenderD7::Color::Hex("#FFFFFF"), "BCSTM-Player->MainMenu");
    RenderD7::DrawText(5, 30, 0.7f, RenderD7::Color::Hex("#222222"), "\uE003: Stop Player!\n\uE002 or \uE001: Back to MainMenu");
    if (playing)
    {
        RenderD7::DrawText(5, 218, 0.7f, RenderD7::Color::Hex("#111111"), "Playing: " + currentlypl);
    }
    RenderD7::OnScreen(Bottom);
    RenderD7::DrawRect(0, 0, 320, 240, RenderD7::Color::Hex("#222222"));
    RenderD7::DrawTObjects(this->buttons, RenderD7::Color::Hex("#333333"), RenderD7::Color::Hex("#FFFFFF"), this->Selection, RenderD7::Color::Hex("#444444"), RenderD7::Color::Hex("#333333", 100));
    
}
void MMM::Logic(u32 hDown, u32 hHeld, u32 hUp, touchPosition touch)
{
    if((hDown & KEY_DOWN && Selection < 2))
    {
        Selection ++;
    }
    if ((hDown & KEY_UP && Selection > 0))
    {
        Selection--;
    }
    if ((hDown & KEY_A && Selection == 0))
    {
        RenderD7::Scene::Load(std::make_unique<Browse>());
    }
    if (hDown & KEY_A && Selection == 1)
    {
        RenderD7::Scene::Load(std::make_unique<Credits>());
    }
    if ((hDown & KEY_A && Selection == 2))
    {
        RenderD7::ExitApp();
    }
    if (hDown & KEY_START)
    {
        RenderD7::ExitApp();
    }
    if (RenderD7::touchTObj(touch, buttons[2]))
    {
        RenderD7::ExitApp();
    }
    if (RenderD7::touchTObj(touch, buttons[0]))
    {
        RenderD7::Scene::Load(std::make_unique<Browse>());
    }
    if (RenderD7::touchTObj(touch, buttons[1]))
    {
        RenderD7::Scene::Load(std::make_unique<Credits>());
    }
    if(hDown & KEY_Y)
    {
        if (playing)
        {
            player.stop();
        }
    }
    
}

Browse::Browse()
{
    RenderD7::Msg::Display("BCSTM-Player", "Loading Directory", Top);
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
		dirs += "\n";
	}

    RenderD7::DrawText(10, 30, 0.6f, RenderD7::Color::Hex("#111111"), dirs.c_str());
    RenderD7::OnScreen(Bottom);
    RenderD7::DrawRect(0, 0, 320, 240, RenderD7::Color::Hex("#EEEEEE"));
    if(playing)
    {
        RenderD7::DrawText(5, 218, 0.7f, RenderD7::Color::Hex("#111111"), "Playing: " + currentlypl);
    }
    
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
    
}

void Credits::Draw(void) const
{
    RenderD7::OnScreen(Top);
    RenderD7::DrawRect(0, 0, 400, 240, RenderD7::Color::Hex("#EEEEEE"));
    RenderD7::DrawRect(0, 0, 400, 26, RenderD7::Color::Hex("#222222"));
    RenderD7::DrawRect(0, 240, 400, -26, RenderD7::Color::Hex("#222222"));
    RenderD7::DrawTextCentered(40, 2, 0.7f, RenderD7::Color::Hex("#FFFFFF"), "BCSTM-Player->Credits", 400);
    RenderD7::DrawTextCentered(40, 218, 0.7f, RenderD7::Color::Hex("#FFFFFF"), "Version: 1.0.0", 400);
    RenderD7::DrawText(5, 30, 0.7f, RenderD7::Color::Hex("#000000"), "- Tobi-D7\n- devkitpro\n- citro2d\n- citro3d\n\nThis Project was made\nwith RenderD7.");
    RenderD7::OnScreen(Bottom);
    RenderD7::DrawRect(0, 0, 320, 240, RenderD7::Color::Hex("#222222"));
    RenderD7::DrawImageFromSheet(&sheet, 0, 15, 70);

}

void Credits::Logic(u32 hDown, u32 hHeld, u32 hUp, touchPosition touch)
{
    if (hDown & KEY_B)
    {
        RenderD7::Scene::Back();
    }
}
