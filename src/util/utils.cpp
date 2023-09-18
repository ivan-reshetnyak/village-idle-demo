#include "pch.h"

#include <cstdio>

#include "util/utils.h"


namespace utils {
std::unique_ptr<char[]> LoadText(const std::string &fileName)
{
    FILE* file;
    std::unique_ptr<char[]> mem = nullptr;
    errno_t err;

    if ((err = fopen_s(&file, fileName.c_str(), "r")) == 0) {
        size_t len;

        fseek(file, 0, SEEK_END);
        len = ftell(file);

        mem = std::unique_ptr<char[]>(new char[len + 1]);
        if (mem) {
            fseek(file, 0, SEEK_SET);
            len = fread(mem.get(), 1, len, file);
            mem[len] = 0;
        }
        fclose(file);
    }
    return std::move(mem);
}
}
