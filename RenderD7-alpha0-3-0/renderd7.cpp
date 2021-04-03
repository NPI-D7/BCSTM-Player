#include "renderd7.hpp"
#include "log.hpp"
#include <regex>

#define RGBA8(r, g, b, a) ((((r) & 0xFF) << 0) | (((g) & 0xFF) << 8) | (((b) & 0xFF) << 16) | (((a) & 0xFF) << 24))
#define D7_NOTHING C2D_Color32(0, 0, 0, 0)
Log renderd7log;
float animtime;
bool isndspinit = false;
bool running = true;
std::stack<std::unique_ptr<RenderD7::Scene>> RenderD7::Scene::scenes;

u32 d7_hDown;
u32 d7_hHeld;
u32 d7_hUp;
touchPosition d7_touch;

C2D_TextBuf TextBuf;
C2D_Font Font;

// Fps Calc
static float current_fps = 0.0f;
static unsigned int frames = 0;
static u64 last_time = 0;
float d11framerate = 0;
//-----------------

bool currentScreen = false;

C3D_RenderTarget* Top;
C3D_RenderTarget* TopRight;
C3D_RenderTarget* Bottom;

#define DSEVENBLACK C2D_Color32(0, 0 ,0, 255)
#define DSEVENWHITE C2D_Color32(255, 255, 255, 255)
RenderD7::SpriteSheetAnimation::SpriteSheetAnimation()
{
    renderd7log.Write("SpriteSheetAnimation createt!");
}
RenderD7::SpriteSheetAnimation::~SpriteSheetAnimation()
{
    
}
bool RenderD7::DrawImageFromSheet(RenderD7::Sheet* sheet, size_t index, float x, float y, float scaleX, float scaleY)
{
    if (sheet->spritesheet != nullptr)
    {
		if (C2D_SpriteSheetCount(sheet->spritesheet) >= index)
        {
			return C2D_DrawImageAt(C2D_SpriteSheetGetImage(sheet->spritesheet, index), x, y, 0.5f, nullptr, scaleX, scaleY);
		}
	}
    return false;
}
void RenderD7::Init::NdspFirm(bool useit)
{
    if (useit)
    {
        if ( access( "sdmc:/3ds/dspfirm.cdc", F_OK ) != -1 ) 
        {
            ndspInit();
            isndspinit = true;
        }
        else
        {
            renderd7log.Write("RenderD7: SoundEngine Error! ndspfirm not found!");
        }
    }
}
void RenderD7::Exit::NdspFirm()
{
    if (isndspinit)
    {
        ndspExit();
    }
}
void RenderD7::Msg::Display(std::string titletxt, std::string txt, C3D_RenderTarget *target)
{
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, DSEVENBLACK);
	C2D_TargetClear(Bottom, DSEVENBLACK);
    RenderD7::ClearTextBufs();
	RenderD7::OnScreen(Top);
	RenderD7::DrawRect(0, 0, 400, 240, RenderD7::Color::Hex("#111111"));
	RenderD7::OnScreen(Bottom);
	RenderD7::DrawRect(0, 0, 320, 240, RenderD7::Color::Hex("#111111"));
	RenderD7::OnScreen(target);
	RenderD7::DrawRect(0, 0, 400, 26, RenderD7::Color::Hex("#333333", 200));
    RenderD7::DrawText(5, 2, 0.7f, DSEVENWHITE, titletxt);
	RenderD7::DrawText(5, 30, 0.6f, DSEVENWHITE, txt);
	
	C3D_FrameEnd(0);
}
void RenderD7::SetupLog()
{
    renderd7log.Init("sdmc:/Flappy-Bird-RenderD7.log");
}
void RenderD7::SpriteSheetAnimation::Setup(RenderD7::Sheet *sheet, size_t imagecount, size_t startimage, float frame_begin, float frame_finish)
{
    
    D_totaltime = frame_begin;
    renderd7log.Write("frame_begin success");
    this->images = imagecount;
    renderd7log.Write("imagecount success");
    this->sheet = sheet;
    renderd7log.Write("sheet success");
    this->time = frame_finish;
    renderd7log.Write("frame_finish success");
    RenderD7::SpriteSheetAnimation::FromSheet(this->sheet, startimage);
}
void RenderD7::SpriteSheetAnimation::Play(float timespeed)
{
    D_totaltime += timespeed;
    if (D_totaltime >= time)
    {
        D_totaltime -= time;
        imgs++;
        if (imgs == images)
        {
            imgs = 0;
        }
    }
    RenderD7::SpriteSheetAnimation::FromSheet(sheet, imgs);
    //RenderD7::SpriteSheetAnimation::Draw();
    

}

void RenderD7::Error::DisplayError(std::string toptext, std::string errortext)
{
    RenderD7::ClearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, DSEVENBLACK);
	C2D_TargetClear(Bottom, DSEVENBLACK);
    RenderD7::DrawTextCentered(0, 0, 0.7f, DSEVENWHITE, toptext, 400);
	RenderD7::DrawTextCentered(0, 100, 0.6f, DSEVENWHITE, errortext, 400);
	C3D_FrameEnd(0);
    for (int i = 0; i < 60*20; i++) {
		gspWaitForVBlank();
	}
}
u32 RenderD7::Color::Hex(const std::string color, u8 a)
{
    if (color.length() < 7 || std::regex_search(color.substr(1), std::regex("[^0-9A-Fa-f]"))) { // invalid color.
		return D7_NOTHING;
	}
    int r = std::stoi(color.substr(1, 2), nullptr, 16);
	int g = std::stoi(color.substr(3, 2), nullptr, 16);
	int b = std::stoi(color.substr(5, 2), nullptr, 16);
	return RGBA8(r, g, b, a);
}

void RenderD7::Scene::doDraw() {
	if(!RenderD7::Scene::scenes.empty())
		RenderD7::Scene::scenes.top()->Draw();
}

void RenderD7::Scene::doLogic(u32 hDown, u32 hHeld, u32 hUp, touchPosition touch) {
	if(!RenderD7::Scene::scenes.empty())
		RenderD7::Scene::scenes.top()->Logic(hDown, hHeld, hUp, touch);
}

void RenderD7::Scene::Load(std::unique_ptr<Scene> scene)
{ 
	Scene::scenes.push(std::move(scene));
}

void RenderD7::Scene::Back() {
	if(RenderD7::Scene::scenes.size() > 0)
		RenderD7::Scene::scenes.pop();
}

void RenderD7::OnScreen(C3D_RenderTarget *target)
{
    C2D_SceneBegin(target);
}

bool RenderD7::MainLoop()
{
    if (!aptMainLoop()) return false;
    hidScanInput();
	d7_hDown = hidKeysDown();
	d7_hUp = hidKeysUp();
	d7_hHeld = hidKeysHeld();
	hidTouchRead(&d7_touch);

    RenderD7::ClearTextBufs();
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
	C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
    
    return running;
}


RenderD7::Sheet::Sheet()
{

}
RenderD7::Sheet::~Sheet()
{

}

Result RenderD7::Sheet::Load(const char *path)
{
	this->spritesheet = C2D_SpriteSheetLoad(path);
	return 0;
}

void RenderD7::Sheet::Free()
{
	C2D_SpriteSheetFree(this->spritesheet);
}

RenderD7::Sprite::Sprite()
{

}
RenderD7::Sprite::~Sprite()
{
	
}
void RenderD7::Sprite::FromSheet(RenderD7::Sheet *sheet, size_t index)
{
	C2D_SpriteFromSheet(&this->sprite, sheet->spritesheet, index);
}
bool RenderD7::Sprite::Draw()
{
	return C2D_DrawSprite(&this->sprite);
}
void RenderD7::Sprite::SetCenter(float x, float y)
{
	C2D_SpriteSetCenter(&this->sprite, x, y);
}
void RenderD7::Sprite::SetPos(float x, float y)
{
	C2D_SpriteSetPos(&this->sprite, x, y);
}
void RenderD7::Sprite::SetRotation(float rotation)
{
	C2D_SpriteSetRotation(&this->sprite, rotation);
}
void RenderD7::Sprite::Rotate(float speed)
{
	C2D_SpriteRotateDegrees(&this->sprite, speed);
}
float RenderD7::Sprite::getHeigh()
{
	return this->sprite.params.pos.h;
}
float RenderD7::Sprite::getWidth()
{
	return this->sprite.params.pos.w;
}
float RenderD7::Sprite::getPosX()
{
	return this->sprite.params.pos.x;
}
float RenderD7::Sprite::getPosY()
{
	return this->sprite.params.pos.y;
}

void RenderD7::ClearTextBufs(void)
{
	C2D_TextBufClear(TextBuf);
}

void frameloop()
{
	frames++;
	u64 delta_time = osGetTime() - last_time;
	if (delta_time >= 1000) {
		current_fps = frames/(delta_time/1000.0f)+1;
		frames = 0;
		last_time = osGetTime();
	}
	d11framerate = current_fps;
}
float getframerate()
{
    frameloop();
	return d11framerate;
}

bool RenderD7::DrawRect(float x, float y, float w, float h, u32 color)
{
	return C2D_DrawRectSolid(x, y, 0.5f, w, h, color);
}

void RenderD7::DrawTextCentered(float x, float y, float size, u32 color, std::string Text, int maxWidth, int maxHeight, C2D_Font fnt) {
	float lineHeight, widthScale;

	// Check for the lineHeight.
	if (fnt != nullptr) {
		lineHeight = RenderD7::GetTextHeight(size, " ", fnt);
	} else {
		lineHeight = RenderD7::GetTextHeight(size, " ");
	}

	int line = 0;
	while(Text.find('\n') != Text.npos) {
		if (maxWidth == 0) {
			// Do the widthScale.
			if (fnt != nullptr) {
				widthScale = RenderD7::GetTextWidth(size, Text.substr(0, Text.find('\n')), fnt);
			} else {
				widthScale = RenderD7::GetTextWidth(size, Text.substr(0, Text.find('\n')));
			}

		} else {
			// Do the widthScale 2.
			if (fnt != nullptr) {
				widthScale = std::min((float)maxWidth, RenderD7::GetTextWidth(size, Text.substr(0, Text.find('\n')), fnt));
			} else {
				widthScale = std::min((float)maxWidth, RenderD7::GetTextWidth(size, Text.substr(0, Text.find('\n'))));
			}

		}

		if (fnt != nullptr) {
			RenderD7::DrawText((currentScreen ? 200 : 160)+x-(widthScale/2), y+(lineHeight*line), size, color, Text.substr(0, Text.find('\n')), maxWidth, maxHeight, fnt);
		} else {
			RenderD7::DrawText((currentScreen ? 200 : 160)+x-(widthScale/2), y+(lineHeight*line), size, color, Text.substr(0, Text.find('\n')), maxWidth, maxHeight);
		}

		Text = Text.substr(Text.find('\n')+1);
		line++;
	}

	if (maxWidth == 0) {
		// Do the next WidthScale.
		if (fnt != nullptr) {
			widthScale = RenderD7::GetTextWidth(size, Text.substr(0, Text.find('\n')), fnt);
		} else {
			widthScale = RenderD7::GetTextWidth(size, Text.substr(0, Text.find('\n')));
		}

	} else {
		// And again.
		if (fnt != nullptr) {
			widthScale = std::min((float)maxWidth, RenderD7::GetTextWidth(size, Text.substr(0, Text.find('\n')), fnt));
		} else {
			widthScale = std::min((float)maxWidth, RenderD7::GetTextWidth(size, Text.substr(0, Text.find('\n'))));
		}

	}
	if (fnt != nullptr) {
		RenderD7::DrawText((currentScreen ? 200 : 160)+x-(widthScale/2), y+(lineHeight*line), size, color, Text.substr(0, Text.find('\n')), maxWidth, maxHeight, fnt);
	} else {
		RenderD7::DrawText((currentScreen ? 200 : 160)+x-(widthScale/2), y+(lineHeight*line), size, color, Text.substr(0, Text.find('\n')), maxWidth, maxHeight);
	}
}

// Draw String or Text.
void RenderD7::DrawText(float x, float y, float size, u32 color, std::string Text, int maxWidth, int maxHeight, C2D_Font fnt) {
	C2D_Text c2d_text;

	if (fnt != nullptr) {
		C2D_TextFontParse(&c2d_text, fnt, TextBuf, Text.c_str());
	} else {
		C2D_TextFontParse(&c2d_text, Font, TextBuf, Text.c_str());
	}

	C2D_TextOptimize(&c2d_text);

	float heightScale;
	if (maxHeight == 0) {
		heightScale = size;
	} else {
		
		if (fnt != nullptr) {
			heightScale = std::min(size, size*(maxHeight/RenderD7::GetTextHeight(size, Text, fnt)));
		} else {
			heightScale = std::min(size, size*(maxHeight/RenderD7::GetTextHeight(size, Text)));
		}
	}

	if (maxWidth == 0) {
		C2D_DrawText(&c2d_text, C2D_WithColor, x, y, 0.5f, size, heightScale, color);
	} else {
		if (fnt != nullptr) {
			C2D_DrawText(&c2d_text, C2D_WithColor, x, y, 0.5f, std::min(size, size*(maxWidth/RenderD7::GetTextWidth(size, Text, fnt))), heightScale, color);
		} else {
			C2D_DrawText(&c2d_text, C2D_WithColor, x, y, 0.5f, std::min(size, size*(maxWidth/RenderD7::GetTextWidth(size, Text))), heightScale, color);
		}
	}
}

// Get String or Text Width.
float RenderD7::GetTextWidth(float size, std::string Text, C2D_Font fnt) {
	float width = 0;
	if (fnt != nullptr) {
		GetTextSize(size, &width, NULL, Text, fnt);
	} else {
		GetTextSize(size, &width, NULL, Text);
	}
	return width;
}

// Get String or Text Size.
void RenderD7::GetTextSize(float size, float *width, float *height, std::string Text, C2D_Font fnt) {
	C2D_Text c2d_text;
	if (fnt != nullptr) {
		C2D_TextFontParse(&c2d_text, fnt, TextBuf, Text.c_str());
	} else {
		C2D_TextFontParse(&c2d_text, Font, TextBuf, Text.c_str());
	}
	C2D_TextGetDimensions(&c2d_text, size, size, width, height);
}


// Get String or Text Height.
float RenderD7::GetTextHeight(float size, std::string Text, C2D_Font fnt) {
	float height = 0;
	if (fnt != nullptr) {
		GetTextSize(size, NULL, &height, Text.c_str(), fnt);
	} else {
		GetTextSize(size, NULL, &height, Text.c_str());
	}
	return height;
}

Result RenderD7::loadFont(C2D_Font &fnt, const char* Path) {
	fnt = C2D_FontLoad(Path); // Only load if found.
	return 0;
}

// Unload a Font.
Result RenderD7::unloadFont(C2D_Font &fnt) {
	if (fnt != nullptr) {
		C2D_FontFree(fnt); // Make sure to only unload if not nullptr.
	}
	return 0;
}

bool RenderD7::DrawCircle(float x, float y, float radius, u32 color)
{
	return C2D_DrawCircleSolid(x, y, 0.5f, radius, color);
}

Result RenderD7::Init::Main()
{
    gfxInitDefault();
    aptInit();
    romfsInit();
    cfguInit();
    osSetSpeedupEnable(true);
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	Top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	TopRight = C2D_CreateScreenTarget(GFX_TOP, GFX_RIGHT);
	Bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
	TextBuf = C2D_TextBufNew(4096);
	Font = C2D_FontLoadSystem(CFG_REGION_USA);
	return 0;

}
void RenderD7::Exit::Main()
{
    C2D_TextBufDelete(TextBuf);
	C2D_Fini();
	C3D_Fini();
    aptExit();
    romfsExit();
    cfguExit();
}

std::string RenderD7::GetFramerate()
{
	frameloop();
	return (std::to_string(d11framerate).substr(0, 2));
}

void RenderD7::DrawTObjects(std::vector<RenderD7::TObject> tobjects, u32 color, u32 txtcolor, int selection, u32 selbgcolor, u32 selcolor)
{
	for(int i = 0; i < (int)tobjects.size(); i++)
	{
		if (selection == i)
		{
			RenderD7::DrawRect(tobjects[i].x - 2, tobjects[i].y - 2, tobjects[i].w + 4, tobjects[i].h + 4, selbgcolor);
			RenderD7::DrawRect(tobjects[i].x, tobjects[i].y, tobjects[i].w, tobjects[i].h, color);
			RenderD7::DrawRect(tobjects[i].x, tobjects[i].y, tobjects[i].w, tobjects[i].h, selcolor);
			RenderD7::DrawText(tobjects[i].x + (tobjects[i].w/2) - RenderD7::GetTextHeight(tobjects[i].txtsize , tobjects[i].text) + tobjects[i].correctx, tobjects[i].y + (tobjects[i].h/2) - RenderD7::GetTextHeight(tobjects[i].txtsize, tobjects[i].text) + tobjects[i].correcty, tobjects[i].txtsize, txtcolor, tobjects[i].text);
		}
		else
		{
			RenderD7::DrawRect(tobjects[i].x, tobjects[i].y - 1, tobjects[i].w, tobjects[i].h, color);
			RenderD7::DrawText(tobjects[i].x + (tobjects[i].w/2) - RenderD7::GetTextHeight(tobjects[i].txtsize , tobjects[i].text) + tobjects[i].correctx, tobjects[i].y + (tobjects[i].h/2) - RenderD7::GetTextHeight(tobjects[i].txtsize, tobjects[i].text) + tobjects[i].correcty, tobjects[i].txtsize, txtcolor, tobjects[i].text);
		}
		
	}
}
void RenderD7::ExitApp()
{
	running = false;
}

bool RenderD7::touchTObj(touchPosition touch, RenderD7::TObject button)
{
   if (touch.px >= button.x && touch.px <= (button.x + button.w) && touch.py >= button.y && touch.py <= (button.y + button.h))	return true;
    else	return false;
}

void RenderD7::DrawSTObject(std::vector<RenderD7::TObject> tobject, int tobjectindex, u32 color, u32 txtcolor)
{
	RenderD7::DrawRect(tobject[tobjectindex].x, tobject[tobjectindex].y, tobject[tobjectindex].w, tobject[tobjectindex].h, color);
	RenderD7::DrawText(tobject[tobjectindex].x + (tobject[tobjectindex].w/2) - RenderD7::GetTextHeight(tobject[tobjectindex].txtsize , tobject[tobjectindex].text) + tobject[tobjectindex].correctx, tobject[tobjectindex].y + (tobject[tobjectindex].h/2) - RenderD7::GetTextHeight(tobject[tobjectindex].txtsize, tobject[tobjectindex].text) + tobject[tobjectindex].correcty, tobject[tobjectindex].txtsize, txtcolor, tobject[tobjectindex].text);
}

bool RenderD7::NameIsEndingWith(const std::string &name, const std::vector<std::string> &extensions) {
	if (name.substr(0, 2) == "._") return false;

	if (name.size() == 0) return false;

	if (extensions.size() == 0) return true;

	for(int i = 0; i < (int)extensions.size(); i++) {
		const std::string ext = extensions.at(i);
		if (strcasecmp(name.c_str() + name.size() - ext.size(), ext.c_str()) == 0) return true;
	}

	return false;
}

bool dirEntryPredicate(const RenderD7::DirContent &lhs, const RenderD7::DirContent &rhs) {
	if (!lhs.isDir && rhs.isDir) return false;
	if (lhs.isDir && !rhs.isDir) return true;

	return strcasecmp(lhs.name.c_str(), rhs.name.c_str()) < 0;
}

void RenderD7::GetDirContentsExt(std::vector<RenderD7::DirContent> &dircontent, const std::vector<std::string> &extensions) {
	struct stat st;

	dircontent.clear();

	DIR *pdir = opendir(".");

	if (pdir != nullptr) {
		while(true) {
			RenderD7::DirContent dirEntry;

			struct dirent *pent = readdir(pdir);
			if (pent == NULL) break;

			stat(pent->d_name, &st);
			dirEntry.name = pent->d_name;
			dirEntry.isDir = (st.st_mode & S_IFDIR) ? true : false;

			if (dirEntry.name.compare(".") != 0 && (dirEntry.isDir || RenderD7::NameIsEndingWith(dirEntry.name, extensions))) {
				dircontent.push_back(dirEntry);
			}
		}

		closedir(pdir);
	}

	sort(dircontent.begin(), dircontent.end(), dirEntryPredicate);
}

void RenderD7::GetDirContents(std::vector<RenderD7::DirContent> &dircontent) {
	RenderD7::GetDirContentsExt(dircontent, {});
}