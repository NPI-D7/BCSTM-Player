#include <3ds.h>
#include <algorithm>
#include <citro2d.h>
#include <string>

class Title {
    public:
    Title(void) = default;
    ~Title(void);
    bool load(u64 id, FS_MediaType mediatype);
    u32 highid(void);
    u32 lowid(void);
    u64 ID(void);
    FS_MediaType mediatype(void);
    std::string name(void);
    std::string author(void);
    C2D_Image icon(void);

    private:
    u64 m_id;
    FS_MediaType m_Media;
    FS_CardType m_Card;
    C2D_Image m_Icon;
    std::string m_Name;
    std::string m_Author;
};