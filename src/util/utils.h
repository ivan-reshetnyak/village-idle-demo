#pragma once

#include <memory>
#include <string>


namespace utils {
std::unique_ptr<char[]> LoadText(const std::string& fileName);
}
