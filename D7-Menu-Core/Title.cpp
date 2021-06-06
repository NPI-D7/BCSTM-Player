#include "title.hpp"
#include "smdh.hpp"
#include "utils.hpp"
#include "log.hpp"

std::string CardStatus = "NotInserted";
std::string CardTypeStatus = "None";
extern Log cachelog;
bool checked = false;
FS_CardType type;

bool D7TM::checkCardSlot()
{
	bool isinsert;
	FSUSER_CardSlotIsInserted(&isinsert);
	return isinsert;
}

void D7TM::CardLoop()
{
	
	if(checkCardSlot())
	{
		checked = true;
		CardStatus = "Inserted";
		FSUSER_GetCardType(&type);
		if (type == CARD_CTR)
		{
			CardTypeStatus = "CTR";
		}
		else if (type == CARRD_NTR)
		{
			CardTypeStatus == "TWL(Not Supportet)";
		}
	}
	else 
	{
		CardStatus = "NotInserted";
		CardTypeStatus = "None";
		checked = true;
	}
	if (checked)
	{
		checked = false;
	}

}
/*static C2D_Image loadIconTex(smdh_s* smdh){
    C3D_Tex* tex                          = new C3D_Tex;
	static const Tex3DS_SubTexture subt3x = {48, 48, 0.0f, 48 / 64.0f, 48 / 64.0f, 0.0f};
	C3D_TexInit(tex, 64, 64, GPU_RGB565);

	u16* dest = (u16*)tex->data + (64 - 48) * 64;
	u16* src  = (u16*)smdh->bigIconData;
	for (int j = 0; j < 48; j += 8)
	{
		std::copy(src, src + 48 * 8, dest);
		src += 48 * 8;
		dest += 64 * 8;
	}
	
	return C2D_Image{tex, &subt3x};
}
*/
Title::~Title(void){
    if (m_Card != CARD_TWL && m_Icon.tex) {
        C3D_TexDelete(m_Icon.tex);
    }
}

bool Title::LoadFromCache(const uint64_t& _id, const std::string _title, const std::string& code, const uint8_t& mt)
    {
		cachelog.Write("Loading Title: " + _id);
        m_id = _id;
        m_Media = (FS_MediaType)mt;
        m_Name.assign(_title);
		//m_Author.assign(_author);
        m_prodCode.assign(code);
		cachelog.Write(std::to_string(m_id));
		cachelog.Write(m_Name + std::to_string((u32)(m_id)) + std::to_string((u32)(m_id >> 32)));
		//smdh_s* smdh = loadSMDH(lowid(), highid(), m_Media);
    	//if (smdh == NULL){
        //	return false;
    	//}

		//m_Icon     = loadIconTex(smdh);
		//delete smdh;
        return true;
    }

bool Title::load(u64 id, FS_MediaType media) {
    bool titleload = false;
	cachelog.Write("Loading Title: " + id);
    m_id = id;
    m_Media = media;
	cachelog.Write(std::to_string(m_id));
    smdh_s* smdh = loadSMDH(lowid(), highid(), m_Media);
    if (smdh == NULL){
        return false;
    }
	char tmp[16];
    AM_GetTitleProductCode(media, id, tmp);
	m_prodCode = tmp;
    m_Name   = UTF16toUTF8((char16_t*)smdh->applicationTitles[1].shortDescription);
	//m_Author = UTF16toUTF8((char16_t*)smdh->applicationTitles[1].publisher);
	titleload = true;
	//m_Icon     = loadIconTex(smdh);
	delete smdh;
	return titleload;
}

u32 Title::highid(void)
{
	return (u32)(m_id >> 32);
}

u32 Title::lowid(void)
{
	return (u32)m_id;
}

u64 Title::ID(void)
{
	return (u64)highid() << 32 | lowid();
}

FS_MediaType Title::mediatype(void)
{
	return m_Media;
}

// Get the Icon from the SMDH.
 
C2D_Image Title::icon(void)
{
	return m_Icon;
}

// get the name of the Title from the SMDH.
std::string Title::name(void)
{
	return m_Name;
}

/*std::string Title::author(void)
{
	return m_Author;
}*/
