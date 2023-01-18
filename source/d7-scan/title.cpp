#include "title.hpp"
#include "smdh.hpp"
#include "utils.hpp"

std::string CardStatus = "NotInserted";
std::string CardTypeStatus = "None";

bool checked = false;
FS_CardType type;
bool loadicon = true;

void D7TM::IconLoading(bool do_) { loadicon = do_; }

static bool IsValidName(std::string name) {
  if (name == "???") {
    return false;
  }

  else if (name == "") {
    return false;
  }

  return true;
}

bool D7TM::checkCardSlot() {
  bool isinsert;
  FSUSER_CardSlotIsInserted(&isinsert);
  return isinsert;
}

void D7TM::CardLoop() {

  if (checkCardSlot()) {
    checked = true;
    CardStatus = "Inserted";
    FSUSER_GetCardType(&type);
    CardTypeStatus = type ? "TWL" : "CTR";
  } else {
    CardStatus = "NotInserted";
    CardTypeStatus = "None";
    checked = true;
  }
  if (checked) {
    checked = false;
  } //__
}

Title::~Title(void) {}

bool Title::LoadFromCache(const uint64_t &_id, std::string _title,
                          std::string _author, FS_MediaType mt) {
  m_id = _id;
  m_Media = mt;
  m_Name = _title;
  m_Author = _author;
  return true;
}

bool Title::load(u64 id, FS_MediaType media) {
  bool titleload = false;
  m_id = id;
  m_Media = media;
  m_Name = "Null";
  m_Author = "Null";
  smdh_s *smdh = loadSMDH(lowid(), highid(), m_Media);
  if (smdh == NULL) {
    return false;
  }
  m_Name = UTF16toUTF8((char16_t *)smdh->applicationTitles[1].shortDescription);
  if (!IsValidName(m_Name)) {
    delete smdh;
    return false;
  }

  m_Author = UTF16toUTF8((char16_t *)smdh->applicationTitles[1].publisher);
  titleload = true;
  // m_3Icon = GetIcon(smdh);
  delete smdh;
  return titleload;
}

u32 Title::highid(void) { return (u32)(m_id >> 32); }

u32 Title::lowid(void) { return (u32)m_id; }

u64 Title::ID(void) { return (u64)highid() << 32 | lowid(); }

FS_MediaType Title::mediatype(void) { return m_Media; }

// get the name of the Title from the SMDH.
std::string Title::name(void) { return m_Name; }

std::string Title::author(void) { return m_Author; }

std::string Title::MediaTP(void) {
  std::string mt_ = "Unknown";

  switch (this->m_Media) {
  case MEDIATYPE_SD:
    return "SD";
    break;
  case MEDIATYPE_NAND:
    return "NAND";
    break;
  case MEDIATYPE_GAME_CARD:
    return "Game Card";
    break;
  default:
    return "Unknown";
    break;
  }
  return mt_;
}