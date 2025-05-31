#include <Geode/utils/JsonValidation.hpp>
#include <Geode/utils/ranges.hpp>

using namespace geode::prelude;

// This is used for null JsonExpectedValues (for example when doing
// `json.has("key")` where "key" doesn't exist)
static matjson::Value NULL_SCOPED_VALUE = nullptr;

class JsonExpectedValue::Impl final {
public:
    // Values shared between JsonExpectedValues related to the same JSON
    struct Shared final {
        matjson::Value originalJson;
        std::optional<std::string> error;
        std::string rootScopeName;

        Shared(matjson::Value const& json, std::string_view rootScopeName)
          : originalJson(json), rootScopeName(rootScopeName) {}
    };

    // this may be null if the JsonExpectedValue is a "null" value
    std::shared_ptr<Shared> shared;
    matjson::Value& scope;
    std::string scopeName;
    std::string key;
    std::unordered_set<std::string> knownKeys;

    Impl()
      : shared(nullptr),
        scope(NULL_SCOPED_VALUE)
    {}

    // Create a root Impl
    Impl(std::shared_ptr<Shared> shared)
      : shared(shared),
        scope(shared->originalJson),
        scopeName(shared->rootScopeName)
    {}

    // Create a derived Impl
    Impl(Impl* from, matjson::Value& scope, std::string_view key)
      : shared(from->shared),
        scope(scope),
        scopeName(fmt::format("{}.{}", from->scopeName, key)),
        key(key)
    {}
};

JsonExpectedValue::JsonExpectedValue()
  : m_impl(std::make_unique<Impl>())
{}
JsonExpectedValue::JsonExpectedValue(Impl* from, matjson::Value& scope, std::string_view key)
  : m_impl(std::make_unique<Impl>(from, scope, key))
{}
JsonExpectedValue::JsonExpectedValue(matjson::Value const& json, std::string_view rootScopeName)
  : m_impl(std::make_unique<Impl>(std::make_shared<Impl::Shared>(json, rootScopeName)))
{}
JsonExpectedValue::~JsonExpectedValue() {}

JsonExpectedValue::JsonExpectedValue(JsonExpectedValue&&) = default;
JsonExpectedValue& JsonExpectedValue::operator=(JsonExpectedValue&&) = default;

const char* JsonExpectedValue::matJsonTypeToString(matjson::Type ty) {
    switch (ty) {
        case matjson::Type::Null: return "null";
        case matjson::Type::Bool: return "bool";
        case matjson::Type::Number: return "number";
        case matjson::Type::String: return "string";
        case matjson::Type::Array: return "array";
        case matjson::Type::Object: return "object";
        default: return "unknown";
    }
}

matjson::Value const& JsonExpectedValue::getJSONRef() const {
    return m_impl->scope;
}
matjson::Value JsonExpectedValue::json() const {
    return m_impl->scope;
}
std::string JsonExpectedValue::key() const {
    return m_impl->key;
}

bool JsonExpectedValue::hasError() const {
    return !m_impl->shared || m_impl->shared->error.has_value();
}
void JsonExpectedValue::setError(std::string_view error) {
    m_impl->shared->error.emplace(fmt::format("[{}]: {}", m_impl->scopeName, error));
}

bool JsonExpectedValue::is(matjson::Type type) const {
    if (this->hasError()) return false;
    return m_impl->scope.type() == type;
}
bool JsonExpectedValue::isNull() const {
    return this->is(matjson::Type::Null);
}
bool JsonExpectedValue::isBool() const {
    return this->is(matjson::Type::Bool);
}
bool JsonExpectedValue::isNumber() const {
    return this->is(matjson::Type::Number);
}
bool JsonExpectedValue::isString() const {
    return this->is(matjson::Type::String);
}
bool JsonExpectedValue::isArray() const {
    return this->is(matjson::Type::Array);
}
bool JsonExpectedValue::isObject() const {
    return this->is(matjson::Type::Object);
}
JsonExpectedValue& JsonExpectedValue::assertIs(matjson::Type type) {
    if (this->hasError()) return *this;
    if (m_impl->scope.type() != type) {
        this->setError(
            "invalid type {}, expected {}",
            matJsonTypeToString(m_impl->scope.type()),
            matJsonTypeToString(type)
        );
    }
    return *this;
}
JsonExpectedValue& JsonExpectedValue::assertIsNull() {
    return this->assertIs(matjson::Type::Null);
}
JsonExpectedValue& JsonExpectedValue::assertIsBool() {
    return this->assertIs(matjson::Type::Bool);
}
JsonExpectedValue& JsonExpectedValue::assertIsNumber() {
    return this->assertIs(matjson::Type::Number);
}
JsonExpectedValue& JsonExpectedValue::assertIsString() {
    return this->assertIs(matjson::Type::String);
}
JsonExpectedValue& JsonExpectedValue::assertIsArray() {
    return this->assertIs(matjson::Type::Array);
}
JsonExpectedValue& JsonExpectedValue::assertIsObject() {
    return this->assertIs(matjson::Type::Object);
}
JsonExpectedValue& JsonExpectedValue::assertIs(std::initializer_list<matjson::Type> types) {
    if (this->hasError()) return *this;
    if (!std::any_of(types.begin(), types.end(), [this](matjson::Type t) { return t == m_impl->scope.type(); })) {
        this->setError(
            "invalid type {}, expected either {}",
            matJsonTypeToString(m_impl->scope.type()),
            ranges::join(ranges::map<std::vector<std::string>>(types, [](matjson::Type t) {
                return matJsonTypeToString(t);
            }), " or ")
        );
    }
    return *this;
}

JsonExpectedValue JsonExpectedValue::has(std::string_view key) {
    if (this->hasError()) {
        return JsonExpectedValue();
    }
    if (!this->assertIs(matjson::Type::Object)) {
        return JsonExpectedValue();
    }
    m_impl->knownKeys.insert(std::string(key));
    if (!m_impl->scope.contains(key)) {
        return JsonExpectedValue();
    }
    return JsonExpectedValue(m_impl.get(), m_impl->scope[key], key);
}
JsonExpectedValue JsonExpectedValue::hasNullable(std::string_view key) {
    if (this->hasError()) {
        return JsonExpectedValue();
    }
    if (!this->assertIs(matjson::Type::Object)) {
        return JsonExpectedValue();
    }
    m_impl->knownKeys.insert(std::string(key));
    if (!m_impl->scope.contains(key) || m_impl->scope[key].isNull()) {
        return JsonExpectedValue();
    }
    return JsonExpectedValue(m_impl.get(), m_impl->scope[key], key);
}
JsonExpectedValue JsonExpectedValue::needs(std::string_view key) {
    if (this->hasError()) {
        return JsonExpectedValue();
    }
    if (!this->assertIs(matjson::Type::Object)) {
        return JsonExpectedValue();
    }
    m_impl->knownKeys.insert(std::string(key));
    if (!m_impl->scope.contains(key)) {
        this->setError("missing required key {}", key);
        return JsonExpectedValue();
    }
    return JsonExpectedValue(m_impl.get(), m_impl->scope[key], key);
}
std::vector<std::pair<std::string, JsonExpectedValue>> JsonExpectedValue::properties() {
    if (this->hasError()) {
        return std::vector<std::pair<std::string, JsonExpectedValue>>();
    }
    if (!this->assertIs(matjson::Type::Object)) {
        return std::vector<std::pair<std::string, JsonExpectedValue>>();
    }
    std::vector<std::pair<std::string, JsonExpectedValue>> res;
    for (auto& [k, v] : m_impl->scope) {
        res.push_back(std::make_pair(k, JsonExpectedValue(m_impl.get(), v, k)));
    }
    return res;
}
void JsonExpectedValue::checkUnknownKeys() {
    if (this->hasError()) return;
    for (auto&& [key, _] : this->properties()) {
        if (!m_impl->knownKeys.contains(key)) {
            log::warn("{} contains unknown key \"{}\"", m_impl->scopeName, key);
        }
    }
}

size_t JsonExpectedValue::length() {
    if (this->hasError()) {
        return 0;
    }
    if (!this->assertIs(matjson::Type::Array)) {
        return 0;
    }
    return m_impl->scope.size();
}
JsonExpectedValue JsonExpectedValue::at(size_t index) {
    if (this->hasError()) {
        return JsonExpectedValue();
    }
    if (!this->assertIs(matjson::Type::Array)) {
        return JsonExpectedValue();
    }
    if (index >= m_impl->scope.size()) {
        this->setError(
            "array expected to have at least size {}, but its size was only {}",
            index + 1, m_impl->scope.size()
        );
        return JsonExpectedValue();
    }
    return JsonExpectedValue(m_impl.get(), m_impl->scope[index], std::to_string(index));
}
std::vector<JsonExpectedValue> JsonExpectedValue::items() {
    if (this->hasError()) {
        return std::vector<JsonExpectedValue>();
    }
    if (!this->assertIs(matjson::Type::Array)) {
        return std::vector<JsonExpectedValue>();
    }
    std::vector<JsonExpectedValue> res;
    size_t i = 0;
    for (auto& v : m_impl->scope) {
        res.push_back(JsonExpectedValue(m_impl.get(), v, std::to_string(i++)));
    }
    return res;
}

JsonExpectedValue::operator bool() const {
    // The shared check is because null values should evaluate to false so `obj.has("key")` evaluates to false
    return m_impl->shared && !this->hasError();
}

Result<> JsonExpectedValue::ok() {
    if (m_impl->shared && m_impl->shared->error) {
        return Err(*m_impl->shared->error);
    }
    return Ok();
}

JsonExpectedValue geode::checkJson(matjson::Value const& json, std::string_view rootScopeName) {
    return JsonExpectedValue(json, rootScopeName);
}
