#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <cstdlib>

namespace fs = std::filesystem;

void execute_command(const std::string& command, const fs::path& dir) {
    std::string full_command = "cd /d \"" + dir.string() + "\" && " + command;
    std::system(full_command.c_str());
}

void sweep(const fs::path& dir, const std::string& command, int depth, int max_depth) {
    if (depth > max_depth && max_depth != -1) return;

    for (const auto& entry : fs::directory_iterator(dir)) {
        if (entry.is_directory()) {
            execute_command(command, entry.path());
            sweep(entry.path(), command, depth + 1, max_depth);
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: sweep [-d depth] command [args...]" << std::endl;
        return 1;
    }

    int arg_offset = 1;
    int max_depth = -1;

    if (std::string(argv[1]) == "-d") {
        if (argc < 4) {
            std::cout << "Error: -d option requires a depth value and a command." << std::endl;
            return 1;
        }
        max_depth = std::stoi(argv[2]);
        arg_offset = 3;
    }

    std::string command;
    for (int i = arg_offset; i < argc; ++i) {
        command += argv[i];
        if (i < argc - 1) command += " ";
    }

    sweep(fs::current_path(), command, 1, max_depth);

    return 0;
}