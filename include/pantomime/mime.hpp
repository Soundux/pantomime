#pragma once
#include <set>
#include <vector>
#include <string>
#include <optional>
#include <filesystem>

namespace pantomime
{
    struct mime
    {
        const std::string type;
        const std::string sub_type;

      public:
        const std::set<std::string> extensions;

      public:
        const std::optional<bool> compressible;
        const std::optional<std::string> charset;

      public:
        static std::vector<mime> all();

      public:
        static std::optional<mime> get(const std::string &mime);
        static std::optional<mime> from(const std::filesystem::path &file);
    };
} // namespace pantomime