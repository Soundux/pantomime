#include "db.hpp"
#include "mime.hpp"

#include <algorithm>
#include <filesystem>

namespace pantomime
{
    std::vector<mime> mime::all()
    {
        std::vector<mime> rtn;
        std::transform(mimes.begin(), mimes.end(), std::back_inserter(rtn), [](const auto &item) { return item.second; });

        return rtn;
    }

    std::optional<mime> mime::get(const std::string &mime)
    {
        if (mimes.count(mime))
        {
            return mimes.at(mime);
        }

        return std::nullopt;
    }

    std::optional<mime> mime::from(const std::filesystem::path &file)
    {
        if (!file.has_extension())
        {
            return std::nullopt;
        }

        auto extension = file.extension().string().substr(1);
        auto mime = std::find_if(mimes.begin(), mimes.end(), [&extension](const auto &item) { return item.second.extensions.count(extension); });

        if (mime != mimes.end())
        {
            return mime->second;
        }

        return std::nullopt;
    }
} // namespace pantomime