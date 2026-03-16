#include "minibricks/db.h"

#include <fstream>
#include <sstream>
#include <utility>

namespace minibricks {
    
namespace {
std::string ReadFile(const std::string& path) {
    std::ifstream input(path);
    if (!input) throw SqliteError("failed to open file: " + path);
    std::ostringstream buffer;
    buffer << input.rdbuf();
    return buffer.str();
}

}

}