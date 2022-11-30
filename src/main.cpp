#include <array>
#include <string_view>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <fmt/color.h>
#include <vector>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <utility>
#include <map>

namespace fs = std::filesystem;

std::vector<fs::path> include_paths {"./", "/usr/lib/gcc/x86_64-pc-linux-gnu/12/include/g++-v12/"};

class IncludedFile;
std::map<std::string, IncludedFile> included_files; 

void fatalerror(std::string_view str) {
    fmt::print(fmt::emphasis::bold, "includecounter: ");
    fmt::print(fmt::emphasis::bold | fg(fmt::terminal_color::red), "fatal error: ");
    fmt::print("{}", str);
    std::abort();
}

class IncludedFile {
    std::vector<std::string> files;
    std::size_t nlines {};
    std::string name;
    IncludedFile* parent;
    int depth;

public:

    IncludedFile(std::string str, IncludedFile* parent, int depth = 0) : name(str), parent(parent), depth(depth) {
        for (const auto& path : include_paths) {
            auto p = path / str;
            if (!fs::exists(p)) continue;

            std::ifstream stream {p};
            // Read file
            std::string line;
            while (std::getline(stream, line)) {
                incrementLines();
                if (!line.starts_with("#include")) continue;
                std::size_t start = line.find("\"");
                if (start == std::string::npos) start = line.find("<");
                if (start == std::string::npos) continue;
                start++;
                std::size_t end = line.find("\"", start);
                if (end == std::string::npos) end = line.find(">", start);
                if (end == std::string::npos) continue;
                std::string included_file = line.substr(start, end-start);
                if (!included_files.contains(included_file)) {
                    included_files.emplace(included_file, IncludedFile{included_file, this, depth+1});
                    files.push_back(included_file);
                }
            }
        }
    }

    void incrementLines() {
        nlines++;
        if (parent) parent->incrementLines();
    }

    void print() {
        for (int i=0;i<depth; i++) fmt::print("  ");
        fmt::print(fmt::emphasis::bold, "include -> {} {}\n", name, fmt::styled(nlines, fmt::fg(fmt::terminal_color::green)));
        for (auto& filestr : files) {
            included_files.find(filestr)->second.print();
        }
    }
};

int main(int argc, char ** argv) {
    for (int i=1; i<argc; i++) {
        std::string_view arg = argv[i];
        std::string_view value = arg;
        value.remove_prefix(2);

        if (value.empty()) value = i < argc-1 ? argv[++i] : "";
        
        if (arg.starts_with("-I")) include_paths.push_back(value);
        else {
            if (!fs::is_regular_file(arg)) fatalerror(fmt::format("File '{}' not found\n", arg));
            IncludedFile file {arg.data(), 0};
            file.print();
        }
    }
    
    return 0;

}