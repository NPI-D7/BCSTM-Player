#include <3ds.h>
#include <algorithm>
#include <citro2d.h>
#include <string>

extern std::string CardStatus;
extern std::string CardTypeStatus;

namespace D7TM
{
    bool checkCardSlot();
    void CardLoop();
}

class Title {
    public:
    Title(void) = default;
    ~Title(void);
   
    bool LoadFromCache(const uint64_t& _id, std::string _title, std::string code, const uint8_t& mt);
    bool load(u64 id, FS_MediaType mediatype);
    u32 highid(void);
    u32 lowid(void);
    u64 ID(void);
    FS_MediaType mediatype(void);
    std::string name(void);
    std::string author(void);
    std::string prodcode(void) { return m_prodCode; }
    C2D_Image icon(void);
    u16 *IconBuffer;

    private:
    u64 m_id;
    FS_MediaType m_Media;
    FS_CardType m_Card;
    C2D_Image m_Icon;
    std::string m_Name;
    std::string m_Author;
    std::string m_prodCode;
};