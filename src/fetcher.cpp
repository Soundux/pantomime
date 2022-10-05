#include <regex>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <nlohmann/json.hpp>

int main(int argc, char **args)
{
    if (argc < 3)
    {
        std::cerr << "Too few arguments, usage: " << args[0] << R"( "file" "output")" << std::endl;
        return 1;
    }

    auto input = std::ifstream(args[1]);
    auto content = std::string{std::istreambuf_iterator<char>{input}, std::istreambuf_iterator<char>{}};

    std::ofstream out(args[2]);
    auto data = nlohmann::json::parse(content);

    out << R"hpp(#pragma once
#include <map>
#include <string>
#include "mime.hpp"

namespace pantomime
{
    inline std::map<std::string, mime> mimes =
    {
)hpp";

    for (const auto &mime : data.items())
    {
        const auto name = mime.key();
        const auto data = mime.value();

        std::smatch match;
        std::regex_match(name, match, std::regex{R"((.+)\/(.+))"});

        const auto type = match[1];
        const auto sub_type = match[2];
        const auto extensions = data.value<std::vector<std::string>>("extensions", {});

        out << "\t\t\t{\"" << mime.key() << "\", mime{\"" << type << "\", \"" << sub_type << "\", {";

        for (auto it = extensions.begin(); it != extensions.end(); ++it)
        {
            out << "\"" << *it << "\"" << (std::distance(it, extensions.end()) > 1 ? ", " : "");
        }

        out << "}, ";

        if (data.contains("compressible"))
        {
            out << data["compressible"];
        }
        else
        {
            out << "std::nullopt";
        }

        out << ", ";

        if (data.contains("charset"))
        {
            out << data["charset"];
        }
        else
        {
            out << "std::nullopt";
        }

        out << "}}," << std::endl;
    }

    out << R"hpp(   };
} //namespace pantomime)hpp";

    out.close();
    return 0;
}