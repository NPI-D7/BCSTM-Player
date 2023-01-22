#include <ResultDecoder.hpp>
#include <fstream>
#include <map>
#include <sstream>

static std::map<int, std::string> modules = {
    {0, "common"},
    {1, "kernel"},
    {2, "util"},
    {3, "file server"},
    {4, "loader server"},
    {5, "tcb"},
    {6, "os"},
    {7, "dbg"},
    {8, "dmnt"},
    {9, "pdn"},
    {10, "gsp"},
    {11, "i2c"},
    {12, "gpio"},
    {13, "dd"},
    {14, "codec"},
    {15, "spi"},
    {16, "pxi"},
    {17, "fs"},
    {18, "di"},
    {19, "hid"},
    {20, "cam"},
    {21, "pi"},
    {22, "pm"},
    {23, "pm_low"},
    {24, "fsi"},
    {25, "srv"},
    {26, "ndm"},
    {27, "nwm"},
    {28, "soc"},
    {29, "ldr"},
    {30, "acc"},
    {31, "romfs"},
    {32, "am"},
    {33, "hio"},
    {34, "updater"},
    {35, "mic"},
    {36, "fnd"},
    {37, "mp"},
    {38, "mpwl"},
    {39, "ac"},
    {40, "http"},
    {41, "dsp"},
    {42, "snd"},
    {43, "dlp"},
    {44, "hio_low"},
    {45, "csnd"},
    {46, "ssl"},
    {47, "am_low"},
    {48, "nex"},
    {49, "friends"},
    {50, "rdt"},
    {51, "applet"},
    {52, "nim"},
    {53, "ptm"},
    {54, "midi"},
    {55, "mc"},
    {56, "swc"},
    {57, "fatfs"},
    {58, "ngc"},
    {59, "card"},
    {60, "cardnor"},
    {61, "sdmc"},
    {62, "boss"},
    {63, "dbm"},
    {64, "config"},
    {65, "ps"},
    {66, "cec"},
    {67, "ir"},
    {68, "uds"},
    {69, "pl"},
    {70, "cup"},
    {71, "gyroscope"},
    {72, "mcu"},
    {73, "ns"},
    {74, "news"},
    {75, "ro"},
    {76, "gd"},
    {77, "card spi"},
    {78, "ec"},
    {79, "web browser"},
    {80, "test"},
    {81, "enc"},
    {82, "pia"},
    {83, "act"},
    {84, "vctl"},
    {85, "olv"},
    {86, "neia"},
    {87, "npns"},
    {90, "avd"},
    {91, "l2b"},
    {92, "mvd"},
    {93, "nfc"},
    {94, "uart"},
    {95, "spm"},
    {96, "qtm"},
    {97, "nfp"},
    {254, "application"},
};

static std::map<int, std::string> levels = {
    {0, "Success"},       {1, "Info"},       {25, "Status"},
    {26, "Temporary"},    {27, "Permanent"}, {28, "Usage"},
    {29, "Reinitialize"}, {30, "Reset"},     {31, "Fatal"},
};

static std::map<int, std::string> summaries = {
    {0, "Success"},
    {1, "Nothing happened"},
    {2, "Would block"},
    {3, "Out of resource"},
    {4, "Not found"},
    {5, "Invalid state"},
    {6, "Not supported"},
    {7, "Invalid argument"},
    {8, "Wrong argument"},
    {9, "Canceled"},
    {10, "Status changed"},
    {11, "Internal"},
    {63, "Invalid result value"},
};

static std::map<int, std::string> desccommon = {
    {0, "Success"},
    {1000, "Invalid selection"},
    {1001, "Too large"},
    {1002, "Not authorized"},
    {1003, "Already done"},
    {1004, "Invalid size"},
    {1005, "Invalid enum value"},
    {1006, "Invalid combination"},
    {1007, "No data"},
    {1008, "Busy"},
    {1009, "Misaligned address"},
    {1010, "Misaligned size"},
    {1011, "Out of memory"},
    {1012, "Not implemented"},
    {1013, "Invalid address"},
    {1014, "Invalid pointer"},
    {1015, "Invalid handle"},
    {1016, "Not initialized"},
    {1017, "Already initialized"},
    {1018, "Not found"},
    {1019, "Cancel requested"},
    {1020, "Already exists"},
    {1021, "Out of range"},
    {1022, "Timeout"},
    {1023, "Invalid result value"},
};

static std::map<int, std::string> desckernel = {
    {2, "Invalid memory permissions."},
};

static std::map<int, std::string> descos = {
    {10, "Not enough memory."},
    {26, "Session closed by remote."},
    {47, "Invalid command header."},
};

// Need to Fix The Range based Values
static std::map<int, std::string> descfs = {
    {101, "Archive not mounted or mount-point not found."},
    {120, "Title or object not found."},
    {141, "Gamecard not inserted."},
    {230, "Invalid open flags or permissions."},
    {391, "NCCH hash check failed."},
    {302, "RSA or AES-MAC verification failed."},
    {395, "RomFS or Savedata hash check failed."},
    {630, "Command not allowed, or missing permissions."},
    {702, "Invalid path."},
    {761, "Incorrect ExeFS read size."},
    {100, "[Media] not found."},
    {180, "Exists already."},
    {200, "Not enough space."},
    {220, "Invalidated archive."},
    {230, "Unacceptable or write protected."},
    {340, "0x01"},
    {360, "Bad format."},
    {390, "Verification failure."},
    {400, "0x01"},
    {600, "Out of resources."},
    {630, "Access denied."},
    {661, "0x01"},
    {700, "Invalid argument."},
    {730, "Not initialized."},
    {750, "Already initialized."},
    {760, "Not supported."},
    {780, "0x01"},
};

static std::map<int, std::string> descsrv = {
    {5, "Invalid string length (service name length is zero or longer than 8 "
        "chars)."},
    {6, "Access to service denied (requested a service the application does "
        "not have access to)."},
    {7, "String size does not match contents (service name contains unexpected "
        "null byte)."},
};

static std::map<int, std::string> descnwm = {
    {2, "This error usually indicates the wifi chipset in the console is dying "
        "or dead."},
};

static std::map<int, std::string> descam = {
    {4, "Invalid ticket version."},
    {32, "Empty CIA."},
    {37, "Invalid NCCH."},
    {39, "Invalid title version."},
    {43, "Database doesn\"t exist, or it failed to open."},
    {44, "Trying to uninstall system-app."},
    {106, "Invalid signature/CIA. Usually happens when developer UNITINFO is "
          "enabled in Luma3DS."},
    {393, "Invalid database."},
};

static std::map<int, std::string> deschttp = {
    {105, "Request timed out."},
};

static std::map<int, std::string> descnim = {
    {1, "Invalid string IPC paramater (non null terminated at its indicated "
        "length)."},
    {12, "Invalid country code returned by CFG module reading config save "
         "0xB0000."},
    {13, "Zero string length console serial number or '000000000000000' "
         "returned by CFG's SecureInfoGetSerialNo."},
    {18, "General data reading error of NIM's .dat files from its system save, "
         "bad data or bad data lengths."},
    {22, "General invalid data or length of data returned from nintendo "
         "servers. (Only applicable for some operations)"},
    {25, "IntegrityVerificationSeed is waiting on servers to be synced into "
         "console. Can't processed with online services without sync being "
         "completed first over IPC request."},
    {26, "Unavailable/unaccessable IntegrityVerificationSeed on Nintendo "
         "servers. May happen if NIM is told to import "
         "IntegrityVerificationSeed from servers at any time other than after "
         "the successful System Transfer reboot."},
    {27, "Invalid country language code returned by CFG module reading config "
         "save 0xA0002."},
    {37, "Service is in Standby Mode. (eShop ban? General service is down? "
         "This caused by a server response flag on account information. "
         "Account is not referring to NNID.)"},
    {39, "HTTP Status non 200. (Only applicable for some operations)"},
    {40, "General XML read/write error while processing Auto Delivery XMLs."},
    {41, "General XML read/write error while processing Auto Delivery XMLs. "
         "(Stubbed virtual call was called)"},
    {58,
     "Invalid NPNS token returned by CFG module reading config save 0xF0006."},
    {67, "HTTP Status 404 while trying to download a game's seed."},
    {68, "HTTP Status 503 while trying to download a game's seed."},
};

static std::map<int, std::string> descmvd = {
    {271, "Invalid configuration."},
};

static std::map<int, std::string> descqtm = {
    {8, "Camera is already in use or busy."},
};

// Need to Fix The Range based Values
static std::map<int, std::string> descapplication = {
    {0, "The application raised an error. Please consult the application's "
        "source code or ask the author for assistance with it."},
    {1024, "0x01"},
};

void ResultDecoder::Load(Result rescode) { this->m_rescode = rescode; }
void ResultDecoder::Load(std::string rescode) {
  std::stringstream ss;
  ss << rescode;
  ss >> std::hex >> this->m_rescode;
}
std::string ResultDecoder::GetCode() {
  std::stringstream ss;
  ss << std::hex << m_rescode;
  std::string reshex(ss.str());
  return reshex;
}
std::string ResultDecoder::GetLevel() {
  std::string res = levels.at(this->GetLevelInt()) + " (" +
                    std::to_string(this->GetLevelInt()) + ")";
  return res;
}
int ResultDecoder::GetLevelInt() { return R_LEVEL(m_rescode); }
std::string ResultDecoder::GetModule() {
  std::string res = modules.at(this->GetModuleInt()) + " (" +
                    std::to_string(this->GetModuleInt()) + ")";
  return res;
}
int ResultDecoder::GetModuleInt() { return R_MODULE(m_rescode); }
std::string ResultDecoder::GetDescription() {
  std::string res = "Desc Not Implemented!";
  switch (this->GetModuleInt()) {
  case 0:
    res = desccommon.at(this->GetDescriptionInt()) + " (" +
          std::to_string(this->GetDescriptionInt()) + ")";
    break;
  case 1:
    if ((desckernel.find(this->GetDescriptionInt()) == desckernel.end())) {
      res = desccommon.at(this->GetDescriptionInt()) + " (" +
            std::to_string(this->GetDescriptionInt()) + ")";
    } else {
      res = desckernel.at(this->GetDescriptionInt()) + " (" +
            std::to_string(this->GetDescriptionInt()) + ")";
    }
    break;
  case 6:
    if ((descos.find(this->GetDescriptionInt()) == descos.end())) {
      res = desccommon.at(this->GetDescriptionInt()) + " (" +
            std::to_string(this->GetDescriptionInt()) + ")";
    } else {
      res = descos.at(this->GetDescriptionInt()) + " (" +
            std::to_string(this->GetDescriptionInt()) + ")";
    }
    break;
  case 17:
    if ((descfs.find(this->GetDescriptionInt()) == descfs.end())) {
      res = desccommon.at(this->GetDescriptionInt()) + " (" +
            std::to_string(this->GetDescriptionInt()) + ")";
    } else {
      res = descfs.at(this->GetDescriptionInt()) + " (" +
            std::to_string(this->GetDescriptionInt()) + ")";
    }
    break;
  case 25:
    if ((descsrv.find(this->GetDescriptionInt()) == descsrv.end())) {
      res = desccommon.at(this->GetDescriptionInt()) + " (" +
            std::to_string(this->GetDescriptionInt()) + ")";
    } else {
      res = descsrv.at(this->GetDescriptionInt()) + " (" +
            std::to_string(this->GetDescriptionInt()) + ")";
    }
    break;
  case 27:
    if ((descnwm.find(this->GetDescriptionInt()) == descnwm.end())) {
      res = desccommon.at(this->GetDescriptionInt()) + " (" +
            std::to_string(this->GetDescriptionInt()) + ")";
    } else {
      res = descnwm.at(this->GetDescriptionInt()) + " (" +
            std::to_string(this->GetDescriptionInt()) + ")";
    }
    break;
  case 32:
    if ((descam.find(this->GetDescriptionInt()) == descam.end())) {
      res = desccommon.at(this->GetDescriptionInt()) + " (" +
            std::to_string(this->GetDescriptionInt()) + ")";
    } else {
      res = descam.at(this->GetDescriptionInt()) + " (" +
            std::to_string(this->GetDescriptionInt()) + ")";
    }
    break;
  case 40:
    if ((deschttp.find(this->GetDescriptionInt()) == deschttp.end())) {
      res = desccommon.at(this->GetDescriptionInt()) + " (" +
            std::to_string(this->GetDescriptionInt()) + ")";
    } else {
      res = deschttp.at(this->GetDescriptionInt()) + " (" +
            std::to_string(this->GetDescriptionInt()) + ")";
    }
    break;
  case 52:
    if ((descnim.find(this->GetDescriptionInt()) == descnim.end())) {
      res = desccommon.at(this->GetDescriptionInt()) + " (" +
            std::to_string(this->GetDescriptionInt()) + ")";
    } else {
      res = descnim.at(this->GetDescriptionInt()) + " (" +
            std::to_string(this->GetDescriptionInt()) + ")";
    }
    break;
  case 92:
    if ((descmvd.find(this->GetDescriptionInt()) == descmvd.end())) {
      res = desccommon.at(this->GetDescriptionInt()) + " (" +
            std::to_string(this->GetDescriptionInt()) + ")";
    } else {
      res = descmvd.at(this->GetDescriptionInt()) + " (" +
            std::to_string(this->GetDescriptionInt()) + ")";
    }
    break;
  case 96:
    if ((descqtm.find(this->GetDescriptionInt()) == descqtm.end())) {
      res = desccommon.at(this->GetDescriptionInt()) + " (" +
            std::to_string(this->GetDescriptionInt()) + ")";
    } else {
      res = descqtm.at(this->GetDescriptionInt()) + " (" +
            std::to_string(this->GetDescriptionInt()) + ")";
    }
    break;
  case 254:
    if ((descapplication.find(this->GetDescriptionInt()) ==
         descapplication.end())) {
      res = desccommon.at(this->GetDescriptionInt()) + " (" +
            std::to_string(this->GetDescriptionInt()) + ")";
    } else {
      res = descapplication.at(this->GetDescriptionInt()) + " (" +
            std::to_string(this->GetDescriptionInt()) + ")";
    }
    break;

  default:
    res = desccommon.at(this->GetDescriptionInt()) + " (" +
          std::to_string(this->GetDescriptionInt()) + ")";
    break;
  }
  return res;
}
int ResultDecoder::GetDescriptionInt() { return R_DESCRIPTION(m_rescode); }
std::string ResultDecoder::GetSummary() {
  std::string res = summaries.at(this->GetSummaryInt()) + " (" +
                    std::to_string(this->GetSummaryInt()) + ")";
  return res;
}

int ResultDecoder::GetSummaryInt() { return R_SUMMARY(m_rescode); }

void ResultDecoder::WriteLog() {
  std::ofstream out(logfilename, std::ios::app);
  out << "--------------------\n";
  out << "| Error: " << GetCode() << "\n";
  out << "|-------------------\n";
  out << "| Module: " << GetModule() << "\n";
  out << "|-------------------\n";
  out << "| Level: " << GetLevel() << "\n";
  out << "|-------------------\n";
  out << "| Summary: " << GetSummary() << "\n";
  out << "|-------------------\n";
  out << "| Description: " << GetDescription() << "\n";
  out << "--------------------\n";
  out.close();
}

void ResultDecoder::SetLogFile(std::string logfile) { logfilename = logfile; }