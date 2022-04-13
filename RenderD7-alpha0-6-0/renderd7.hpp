#pragma once
#include <3ds.h>
#include <citro2d.h>
#include <citro3d.h>
#include <memory>
#include <stack>
#include <string>
#include <functional>
#include <map>
#include <vector>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <cstring>
#include <random>
#include <sys/stat.h>
#include <algorithm>
#include <iostream>
#include "external/lodepng.h"
#include "external/fs.h"
#include <codecvt>
#include "lang.hpp"
#include "parameter.hpp"
#include "thread.hpp"
#include "ini.hpp"
#include "stringtool.hpp"
#include "Clock.hpp"

#define RENDERD7VSTRING "0.7.0"
#define CHANGELOG "0.6.2:  \n0.6.10: rewrite Threadsystem, Improve framerate\n0.6.02: Fix Code in lang.hpp\nadd Draw Text Left Function.\nadd changelog\n0.6.01: add Threading system."
#define DEFAULT_CENTER 0.5f

extern C3D_RenderTarget* Top;
extern C3D_RenderTarget* TopRight;
extern C3D_RenderTarget* Bottom;

extern u32 d7_hDown;
extern u32 d7_hHeld;
extern u32 d7_hUp;
extern touchPosition d7_touch;

extern std::string dspststus;

/// RenderD7
namespace RenderD7
{
    enum kbd{
         SWKBD,
         BKBD
    };
    enum kbd_type
    {
         NUMPAD,
         STANDARD
    };
    struct TObject
    {
        int x; //Position X
        int y; //Position Y
        int w; //Button Width
        int h; //Button Height
        std::string text = ""; //Text
        float correctx = 0; //Correct X Position
        float correcty = 0; //Correct Y Position
        float txtsize = 0.7f;  //Set Text Size
    };
    /// Set current RenderScreen
    /// \param target The RenderTarget Top, Bottom
    void OnScreen(C3D_RenderTarget *target);
    /** The Spritesheet Class */
    class Sheet
    {
        public:
        /// Construct sheet
        Sheet();
        // Deconstruct sheet
        ~Sheet();
        /// Load a Sritesheet
        /// \param path Path to the Spritesheet (.t3x)
        Result Load(const char *path);
        /// Unload the Spritesheet
        void Free();
        /// The Spritesheet
        C2D_SpriteSheet spritesheet;
    };
    /// Image Class
    class Image
    {
        public:
        /// Load Image from Png
        /// \param path path to png file
        void LoadPng(const std::string path);
        /// Load the Image from buffer
        /// \param buffer the frame buffer
        void LoadPFromBuffer(const std::vector<u8> &buffer);
        /// Draw the Image directly
        /// \param x The x position
        /// \param y the y position
        /// \param scaleX x scale from 0.0 to 1.0
        /// \param scaleY y scale from 0.0 to 1.0
        bool Draw(float x, float y, float scaleX = 1.0f, float scaleY = 1.0f);
        /// \brief Get The Image
        /// \return C2D_Image
        C2D_Image Get(){return this->img;}
        /// \param img this is the C2D_Image
        C2D_Image img;
        /// \param loadet whether the image is loadet or not
        bool loadet = false;
    };
    /// Sprite Class
    class Sprite
    {
        public:
        /// \brief Construct Sprite
        Sprite();
        /// \brief Deconstruct Sprite
        ~Sprite();
        /// \brief Load a Sprite From SpriteSheet
        /// \param sheet the Sheet to load from.(RenderD7::Sheet)
        /// \param index the number of the Sprite in the Sheet
        void FromSheet(RenderD7::Sheet *sheet, size_t index);
        /// \brief Load a Sprite From SpriteSheet
        /// \param img the Image to load from.(RenderD7::Image)
        void FromImage(RenderD7::Image *img);
        bool Draw();
        void SetCenter(float x, float y);
        void SetPos(float x, float y);
        void SetScale(float x, float y);
        void SetRotation(float rotation);
        void Rotate(float speed);
        float getWidth();
        float getHeigh();
        float getPosX();
        float getPosY();
        private:
        C2D_ImageTint tint;
        C2D_Sprite sprite;
    };

    class Scene {
    public:
        static std::stack<std::unique_ptr<Scene>> scenes;
    	virtual ~Scene() {}
    	virtual void Logic(u32 hDown, u32 hHeld, u32 hUp, touchPosition touch) = 0;
    	virtual void Draw() const = 0;
        //virtual void Ovl() const = 0;
        static void Load(std::unique_ptr<Scene> scene);
        static void Back();
        static void doDraw();
        static void doLogic(u32 hDown, u32 hHeld, u32 hUp, touchPosition touch);
        //static void HandleOvl();
    };

    class RSettings : public RenderD7::Scene
    {
    private:
        std::string rd7srstate = "false";
        std::vector<RenderD7::TObject> buttons = 
        {
            {20, 35, 120, 35, "RD7SR", -11, 10},
            {20, 85, 120, 35, "", 0, 9},
            {20, 135, 120, 35, "", -8, 10},
            {20, 185, 120, 35, "", 8, 10},
            {180, 35, 120, 35, "", -15, 10},
            {180, 85, 120, 35, "", -15, 10},
            {180, 135, 120, 35, "", -15, 10},
            {180, 185, 120, 35, "", -15, 10}
        };
    public:
        RSettings();
        void Draw(void) const override;
        ~RSettings();
        void Logic(u32 hDown, u32 hHeld, u32 hUp, touchPosition touch) override;
    };

    void LoadSettings();

    /*class Ovl {
        public:
        virtual ~Ovl(){}
        virtual void Draw() const = 0;
    };
    void AddOvl(RenderD7::Ovl overlay);*/
    namespace Color
    {
        struct rgba
        {
            u8 r, g, b, a;
        };
        class RGBA{
            public:
            RGBA(u8 r, u8 g, u8 b, u8 a) : m_r(r),m_g(g),m_b(b),m_a(a){}
            u32 toRGBA() const {return (m_r << 24) | (m_g << 16) | (m_b << 8) | m_a;}
            
            u8 m_r, m_g ,m_b, m_a;
        };
        u32 Hex(const std::string color, u8 a = 255);
    }
    int GetRandomInt(int b, int e);
    void DrawMetrikOvl();
    bool DrawImageFromSheet(RenderD7::Sheet* sheet, size_t index, float x, float y, float scaleX = 1.0, float scaleY = 1.0);
    namespace Error
    {
        void DisplayError(std::string toptext, std::string errortext, int timesec = 3);
        void DisplayFatalError(std::string toptext, std::string errortext);
    }
    namespace Init
    {
        Result Main(std::string app_name = "RD7Game");
        Result Reload();
        void NdspFirm(bool useit = false);
    }
    namespace Exit
    {
        void Main();
        void NdspFirm();
    }
    namespace Msg
    {
        void Display(std::string titletxt, std::string subtext, C3D_RenderTarget *target);
        void DisplayWithProgress(std::string titletext, std::string subtext, float current, float total, u32 prgbarcolor);
    }
    
    namespace Convert
    {
        inline float StringtoFloat(std::string inp){return std::atof(inp.c_str());}
        inline int StringtoInt(std::string inp){return std::atoi(inp.c_str());}
        inline bool FloatToBool(float inp){if(inp == 1)return true; else return false;}
    }
    namespace FS
    {
        bool FileExist(const std::string& path);
    }
    bool IsNdspInit();
    void SetupLog(void);
    std::string GetFramerate();
    bool MainLoop();
    void ExitApp();
    
    void ClearTextBufs(void);
   
    bool DrawRect(float x, float y, float w, float h, u32 color);
    bool DrawNFRect(float p1x, float p1y, float w, float h, u32 color, float scale = 1);
    bool DrawPx(float x, float y, u32 color);
    void DrawTextCentered(float x, float y, float size, u32 color, std::string Text, int maxWidth = 0, int maxHeight = 0, C2D_Font fnt = nullptr);
	void DrawText(float x, float y, float size, u32 color, std::string Text, int maxWidth = 0, int maxHeight = 0, C2D_Font fnt = nullptr);
    void DrawTextLeft(float x, float y, float size, u32 color, std::string Text, int maxWidth = 0, int maxHeight = 0, C2D_Font fnt = nullptr);
	float GetTextWidth(float size, std::string Text, C2D_Font fnt = nullptr);
	void GetTextSize(float size, float *width, float *height, std::string Text, C2D_Font fnt = nullptr);
	float GetTextHeight(float size, std::string Text, C2D_Font fnt = nullptr);
    Result loadFont(C2D_Font &fnt, const char * Path = "");
	Result unloadFont(C2D_Font &fnt);
    bool DrawCircle(float x, float y, float radius, u32 color);
    bool DrawImage(C2D_Image img, float x, float y, float scaleX = 1.0f, float scaleY = 1.0f);
    void FrameEnd();
    void ToggleRD7SR();
    bool IsRD7SR();

    class SpriteSheetAnimation : public RenderD7::Sprite
    {
        public:
        SpriteSheetAnimation();
        ~SpriteSheetAnimation();
        void Setup(RenderD7::Sheet *sheet, size_t imagecount, size_t startimage, float frame_begin, float frame_finish);
        void Play(float timespeed);
        private:
        size_t images;
        size_t imgs = 0;
        float D_totaltime;
        RenderD7::Sheet *sheet;
        float time;
    };
    

    struct TLBtn
    {
        int x; //Position X
        int y; //Position Y
        int w; //Button Width
        int h; //Button Height
    };

    struct ScrollList1 
    {
        std::string Text = "";
    };

    struct ScrollList2 
    {
        float x;
        float y;
        float w;
        float h;
        std::string Text = "";
    };
    /*enum ListType
    {
        ONE,
        TWO
    };*/
    void DrawList1(RenderD7::ScrollList1 &l, float txtsize, C3D_RenderTarget *t);
    void DrawTObjects(std::vector<RenderD7::TObject> tobjects, u32 color, u32 txtcolor,  int selection = -1, u32 selbgcolor = RenderD7::Color::Hex("#2D98AF"), u32 selcolor = RenderD7::Color::Hex("#000000"));
    void DrawSTObject(std::vector<RenderD7::TObject> tobject, int tobjectindex, u32 color, u32 txtcolor);
    bool touchTObj(touchPosition touch, RenderD7::TObject button);
    void DrawTLBtns(std::vector<RenderD7::TLBtn> btns, u32 color,  int selection = -1, u32 selbgcolor = RenderD7::Color::Hex("#2D98AF"), u32 selcolor = RenderD7::Color::Hex("#000000"));
    struct DirContent
    {
        std::string name;
        std::string path;
        bool isDir;
    };
    struct Checkbox
    {
        float x, y, s;
        bool is_chexked = false;
        u32 outcol, incol, chcol;
    };
    void DrawCheckbox(Checkbox box);
    /*class Console
    {
         public:
           Console();
           Console(int x, int y, int w, int h, int a = 255);
           Console(int x, int y, int w, int h, RenderD7::Color::rgba col);
           Console(int x, int y, int w, int h, std::string name, RenderD7::Color::rgba col = {255, 255, 255, 255}, RenderD7::Color::rgba barcol = {0, 0, 0, 255}, RenderD7::Color::rgba outlinecol = {222, 222, 222, 255});
           void On(C3D_RenderTarget *t_cscreen);
           bool Update();
           ~Console();
         private:
           std::vector<std::string> m_lines;
           int x, y, w, h;
           std::string m_name = "";
           C3D_RenderTarget *cscreen;
           bool m_nconsole = false;
           bool m_mconsole = false;
           RenderD7::Color::rgba color = {255, 255, 255, 255};
           RenderD7::Color::rgba outlinecol = {222, 222, 222, 255};
           RenderD7::Color::rgba barcolor = {0, 0, 0, 255};
    };*/

    bool NameIsEndingWith(const std::string &name, const std::vector<std::string> &extensions);
    void GetDirContentsExt(std::vector<RenderD7::DirContent> &dircontent, const std::vector<std::string> &extensions);
    void GetDirContents(std::vector<RenderD7::DirContent> &dircontent);

} /// RenderD7
