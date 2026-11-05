#include "Codec/TestFile.hpp"
#include "Codec/Model.hpp"

namespace mentor::codec
{

std::optional<TestFile> decodeTestFile(const nlohmann::json &j) noexcept
{
    if (!j.contains("questions"))
        return std::nullopt;

    auto ti = codec::decode<TestInfo>(j);
    if (!ti)
        return std::nullopt;

    TestFile tf;
    tf.info      = *ti;
    tf.questions = j["questions"];
    return tf;
}

} // namespace mentor::codec
