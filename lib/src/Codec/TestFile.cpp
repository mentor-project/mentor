#include "Codec/TestFile.hpp"
#include "Codec/Model.hpp"

namespace mentor::codec
{

std::optional<kernel::TestFile> decodeTestFile(const nlohmann::json &j)
{
    if (!j.contains("questions"))
        return std::nullopt;

    auto ti = codec::decode<TestInfo>(j);
    if (!ti.has_value())
        return std::nullopt;

    kernel::TestFile tf;
    tf.info      = ti.value();
    tf.questions = j["questions"];
    return tf;
}

} // namespace mentor::codec
