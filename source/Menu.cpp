
#include "Menu.hpp"
#include "BCSTM.hpp"

BCSTM player;

extern RenderD7::Sheet sheet;
bool playing = false;
std::string currentlypl;

void DrawFMBG()
{
    RenderD7::Draw::Rect(0, 49, 400, 18, RenderD7::Color::Hex("#CCCCCC"));
    RenderD7::Draw::Rect(0, 85, 400, 18, RenderD7::Color::Hex("#CCCCCC"));
    RenderD7::Draw::Rect(0, 121, 400, 18, RenderD7::Color::Hex("#CCCCCC"));
    RenderD7::Draw::Rect(0, 157, 400, 18, RenderD7::Color::Hex("#CCCCCC"));
    RenderD7::Draw::Rect(0, 193, 400, 18, RenderD7::Color::Hex("#CCCCCC"));
}
void MMM::Draw(void) const
{
    
    RenderD7::OnScreen(Top);
    RenderD7::Draw::Rect(0, 0, 400, 240, RenderD7::Color::Hex("#EEEEEE"));
    RenderD7::Draw::Rect(0, 0, 400, 26, RenderD7::Color::Hex("#222222"));
    RenderD7::Draw::Text(5, 2, 0.7f, RenderD7::Color::Hex("#FFFFFF"), "BCSTM-Player->MainMenu");
    RenderD7::Draw::Text(5, 30, 0.7f, RenderD7::Color::Hex("#222222"), "\uE003: Stop Player!\n\uE002 or \uE001: Back to MainMenu");
    if (playing)
    {
        RenderD7::Draw::Text(5, 218, 0.7f, RenderD7::Color::Hex("#111111"), "Playing: " + currentlypl);
    }
    RenderD7::OnScreen(Bottom);
    RenderD7::Draw::Rect(0, 0, 320, 240, RenderD7::Color::Hex("#222222"));
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
	    currentlypl.clear();
            player.stop();
        }
    }
    
}

void Browse::Update(std::string path)
{
    std::vector<std::string> temp;
    RenderD7::FileSystem::GetDirectoryItems(path.c_str(), temp);
    for (const auto& content : temp)
    {
        if(RenderD7::NameIsEndingWith(this->items[this->dirsel], {"bcstm"}))
        {
            this->items.push_back(content.c_str());
        }
    } 
}

Browse::Browse()
{
    RenderD7::Msg::Display("BCSTM-Player", "Loading Directory", Top);
    Update("/");
    
    this->changeddir = false;
}

void Browse::Draw(void) const
{
    RenderD7::OnScreen(Top);
    char path[PATH_MAX];
    getcwd(path, PATH_MAX);
    RenderD7::Draw::Rect(0, 0, 400, 240, RenderD7::Color::Hex("#EEEEEE"));
    RenderD7::Draw::Rect(0, 0, 400, 26, RenderD7::Color::Hex("#222222"));
    RenderD7::Draw::Text(5, 2, 0.7f, RenderD7::Color::Hex("#FFFFFF"), "BCSTM-Player->FileManager");
    DrawFMBG();
    RenderD7::Draw::TextCentered(0, 216, 0.7f, RenderD7::Color::Hex("#111111"), path, 400);
    std::string dirs;
    for (int i = this->dirsel < 9 ? 0 : this->dirsel - 9; (int)items.size() && i < ((this->dirsel < 9) ? 10 : this->dirsel + 1); i++)
    {
        if (i == dirsel)
        {
            dirs += "> " + this->items[i] + "\n";
        }
        else
        {
            dirs += this->items[i] + "\n";
        } 
    }
    for (uint i = 0; i < ((this->items.size() < 9) ? 9 - this->items.size() : 0); i++) {
		dirs += "\n";
	}

    RenderD7::Draw::Text(10, 30, 0.6f, RenderD7::Color::Hex("#111111"), dirs.c_str());
    RenderD7::OnScreen(Bottom);
    RenderD7::Draw::Rect(0, 0, 320, 240, RenderD7::Color::Hex("#EEEEEE"));
    if(playing)
    {
        RenderD7::Draw::Text(5, 218, 0.7f, RenderD7::Color::Hex("#111111"), "Playing: " + currentlypl);
    }
    if (this->changeddir) 
    {
        dirs.clear();
    }
    
}

void Browse::Logic(u32 hDown, u32 hHeld, u32 hUp, touchPosition touch)
{
    if (this->changeddir) {
        
        
		
		this->changeddir = false;
	}
    if (hDown & KEY_A)
    {
        char path[PATH_MAX];
		getcwd(path, PATH_MAX);
        if (this->items.size() > 0) 
        {
			if (RD7_FSYS_GETINFO(this->items[this->dirsel].c_str()).type == RenderD7::FileSystem::FileType::FileType_Directory) 
            {
                RenderD7::Msg::Display("BCSTM-Player", "Loading Directory", Top);
				Update(this->items[this->dirsel]);
				this->dirsel = 0;
				this->changeddir = true;

			} 
            else 
            {
				
                if (RenderD7::NameIsEndingWith(this->items[this->dirsel], {"bcstm"}))
                {
                    playing = false;
                    player.stop();
                    player.openFromFile(this->items[this->dirsel]);
                    player.play();
                    currentlypl = this->items[this->dirsel];
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
			Update("..");
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
    if (hDown & KEY_DOWN && dirsel < (int)items.size() - 1)
    {
        dirsel++;
    }
    if (hDown & KEY_LEFT && dirsel - 6 > 0)
    {
        dirsel -= 6;
    }
    if (hDown & KEY_RIGHT && dirsel + 6 < (int)items.size() - 1)
    {
        dirsel += 6;
    }
    
}

void Credits::Draw(void) const
{
    RenderD7::OnScreen(Top);
    RenderD7::Draw::Rect(0, 0, 400, 240, RenderD7::Color::Hex("#EEEEEE"));
    RenderD7::Draw::Rect(0, 0, 400, 26, RenderD7::Color::Hex("#222222"));
    RenderD7::Draw::Rect(0, 240, 400, -26, RenderD7::Color::Hex("#222222"));
    RenderD7::Draw::TextCentered(0, 2, 0.7f, RenderD7::Color::Hex("#FFFFFF"), "BCSTM-Player->Credits", 400);
    std::string version = "Version: 1.1.0, ";
    version += V_STRING;
    RenderD7::Draw::TextCentered(0, 218, 0.7f, RenderD7::Color::Hex("#FFFFFF"), version, 400);
    RenderD7::Draw::Text(5, 30, 0.7f, RenderD7::Color::Hex("#000000"), "- Tobi-D7\n- devkitpro\n- citro2d\n- citro3d\n- RenderD7");
    RenderD7::OnScreen(Bottom);
    RenderD7::Draw::Rect(0, 0, 320, 240, RenderD7::Color::Hex("#222222"));
    RenderD7::DrawImageFromSheet(&sheet, 0, 15, 70);

}

void Credits::Logic(u32 hDown, u32 hHeld, u32 hUp, touchPosition touch)
{
    if (hDown & KEY_B)
    {
        RenderD7::Scene::Back();
    }
}
