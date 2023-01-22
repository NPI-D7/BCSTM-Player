#pragma once
#include <3ds.h>
#include <string>

class ResultDecoder {
public:
  ResultDecoder() {}
  ~ResultDecoder() {}
  void Load(Result rescode);
  void Load(std::string rescode);
  std::string GetCode();
  std::string GetLevel();
  int GetLevelInt();
  std::string GetModule();
  int GetModuleInt();
  std::string GetDescription();
  int GetDescriptionInt();
  std::string GetSummary();
  int GetSummaryInt();
  void WriteLog();
  void SetLogFile(std::string logfile = "sdmc:/3ds/ResDec.txt");

private:
  Result m_rescode = 0;
  std::string logfilename = "sdmc:/3ds/ResDec.txt";
};