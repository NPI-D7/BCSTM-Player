#include <3ds.h>
#include <algorithm>
#include <string>

extern std::string CardStatus;
extern std::string CardTypeStatus;

namespace D7TM {
bool checkCardSlot();
void CardLoop();
void IconLoading(bool do_);
} // namespace D7TM

class Title {
public:
  Title(void) = default;
  ~Title(void);

  bool LoadFromCache(const uint64_t &_id, std::string _title,
                     std::string _author, FS_MediaType mt);
  bool load(u64 id, FS_MediaType mediatype);
  u32 highid(void);
  u32 lowid(void);
  u64 ID(void);
  FS_MediaType mediatype(void);
  std::string MediaTP(void);
  std::string name(void);
  std::string author(void);

private:
  u64 m_id;
  FS_MediaType m_Media;
  FS_CardType m_Card;
  std::string m_Name;
  std::string m_Author;
};