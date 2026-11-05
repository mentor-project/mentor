#pragma once

#include "Base/Protocol/Model.hpp"
#include <concepts>
#include <nlohmann/json.hpp>
#include <optional>

namespace mentor::codec
{

std::optional<TestInfo> decodeTestInfo(const nlohmann::json &) noexcept;
std::optional<ChoiceQuestion> decodeChoiceQuestion(const nlohmann::json &) noexcept;
std::optional<EssayQuestion> decodeEssayQuestion(const nlohmann::json &) noexcept;

template <typename Info>
concept IsTestInfo = std::same_as<Info, TestInfo>;

template <typename Question>
concept IsQuestion = std::same_as<Question, ChoiceQuestion> ||
                     std::same_as<Question, EssayQuestion>;

template <typename Info>
std::optional<Info> decode(const nlohmann::json &j) noexcept
    requires IsTestInfo<Info>
{
    return decodeTestInfo(j);
}

template <typename Question>
std::optional<Question> decode(const nlohmann::json &j) noexcept
    requires IsQuestion<Question>
{
    // every question should at least have id, stem and type
    if (!j.contains("id") || !j.contains("stem") || !j.contains("type"))
        return std::nullopt;

    if constexpr (std::is_same_v<ChoiceQuestion, Question>)
        return decodeChoiceQuestion(j);
    else
        return decodeEssayQuestion(j);
}

} // namespace mentor::codec
