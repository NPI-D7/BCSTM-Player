#include "TitleManager.hpp"
#include <memory>

namespace Cache {
void Create(std::vector<std::shared_ptr<Title>> t, const std::string &path,
            int countall);
bool Read(std::vector<std::shared_ptr<Title>> t, const std::string &path,
          bool nand);
} // namespace Cache