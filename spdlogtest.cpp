#include <spdlog/spdlog.h>
#include <string>

int main()
{
    std::string s = "ok";
    spdlog::info("Test {}", s);
}
