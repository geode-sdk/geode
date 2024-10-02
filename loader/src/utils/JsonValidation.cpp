#include <Geode/utils/JsonValidation.hpp>
#include <Geode/utils/ranges.hpp>

using namespace geode::prelude;

matjson::Value& JsonMaybeSomething::json() {
    return m_json;
}

JsonMaybeSomething::JsonMaybeSomething(
    JsonChecker& checker, matjson::Value& json, std::string const& hierarchy, bool hasValue
) :
    m_checker(checker),
    m_json(json), m_hierarchy(hierarchy), m_hasValue(hasValue) {}

bool JsonMaybeSomething::isError() const {
    return m_checker.isError() || !m_hasValue;
}

std::string JsonMaybeSomething::getError() const {
    return m_checker.getError();
}

JsonMaybeSomething::operator bool() const {
    return !isError();
}

void JsonMaybeSomething::setError(std::string const& error) {
    m_checker.m_result = error;
}

JsonMaybeValue::JsonMaybeValue(
    JsonChecker& checker, matjson::Value& json, std::string const& hierarchy, bool hasValue
) : JsonMaybeSomething(checker, json, hierarchy, hasValue) {}

JsonMaybeSomething& JsonMaybeValue::self() {
    return *static_cast<JsonMaybeSomething*>(this);
}

JsonMaybeValue& JsonMaybeValue::array() {
    this->as<value_t::Array>();
    return *this;
}

JsonMaybeObject JsonMaybeValue::obj() {
    this->as<value_t::Object>();
    return JsonMaybeObject(self().m_checker, self().m_json, self().m_hierarchy, self().m_hasValue);
}

JsonMaybeValue JsonMaybeValue::at(size_t i) {
    this->as<value_t::Array>();
    if (this->isError()) return *this;

    auto& json = self().m_json.as_array();
    if (json.size() <= i) {
        this->setError(
            self().m_hierarchy + ": has " + std::to_string(json.size()) +
            "items "
            ", expected to have at least " +
            std::to_string(i + 1)
        );
        return *this;
    }
    return JsonMaybeValue(
        self().m_checker, json.at(i), self().m_hierarchy + "." + std::to_string(i), self().m_hasValue
    );
}

typename JsonMaybeValue::template Iterator<JsonMaybeValue> JsonMaybeValue::iterate() {
    this->as<value_t::Array>();
    Iterator<JsonMaybeValue> iter;
    if (this->isError()) return iter;

    auto& json = self().m_json.as_array();
    size_t i = 0;
    for (auto& obj : json) {
        iter.m_values.emplace_back(
            self().m_checker, obj, self().m_hierarchy + "." + std::to_string(i++), self().m_hasValue
        );
    }
    return iter;
}

typename JsonMaybeValue::template Iterator<std::pair<std::string, JsonMaybeValue>> JsonMaybeValue::items() {
    this->as<value_t::Object>();
    Iterator<std::pair<std::string, JsonMaybeValue>> iter;
    if (this->isError()) return iter;

    for (auto& [k, v] : self().m_json.as_object()) {
        iter.m_values.emplace_back(
            k,
            JsonMaybeValue(self().m_checker, v, self().m_hierarchy + "." + k, self().m_hasValue)
        );
    }

    return iter;
}

JsonMaybeObject::JsonMaybeObject(
    JsonChecker& checker, matjson::Value& json, std::string const& hierarchy, bool hasValue
) : JsonMaybeSomething(checker, json, hierarchy, hasValue) {}

JsonMaybeSomething& JsonMaybeObject::self() {
    return *static_cast<JsonMaybeSomething*>(this);
}

void JsonMaybeObject::addKnownKey(std::string const& key) {
    m_knownKeys.insert(key);
}

matjson::Value& JsonMaybeObject::json() {
    return self().m_json;
}

JsonMaybeValue JsonMaybeObject::emptyValue() {
    return JsonMaybeValue(self().m_checker, self().m_json, "", false);
}

JsonMaybeValue JsonMaybeObject::has(std::string const& key) {
    this->addKnownKey(key);
    if (this->isError()) return emptyValue();
    if (!self().m_json.contains(key) || self().m_json[key].is_null()) {
        return emptyValue();
    }
    return JsonMaybeValue(self().m_checker, self().m_json[key], key, true);
}

JsonMaybeValue JsonMaybeObject::needs(std::string const& key) {
    this->addKnownKey(key);
    if (this->isError()) return emptyValue();
    if (!self().m_json.contains(key)) {
        this->setError(self().m_hierarchy + " is missing required key \"" + key + "\"");
        return emptyValue();
    }
    return JsonMaybeValue(self().m_checker, self().m_json[key], key, true);
}

// TODO: gross hack :3 (ctrl+f this comment to find the other part)
extern bool s_jsonCheckerShouldCheckUnknownKeys;
bool s_jsonCheckerShouldCheckUnknownKeys = true;
void JsonMaybeObject::checkUnknownKeys() {
    if (!s_jsonCheckerShouldCheckUnknownKeys)
        return;
    for (auto& [key, _] : self().m_json.as_object()) {
        if (!m_knownKeys.count(key)) {
            log::warn("{} contains unknown key \"{}\"", self().m_hierarchy, key);
        }
    }
}

JsonChecker::JsonChecker(matjson::Value& json) : m_json(json), m_result(std::monostate()) {}

bool JsonChecker::isError() const {
    return std::holds_alternative<std::string>(m_result);
}

std::string JsonChecker::getError() const {
    return std::get<std::string>(m_result);
}

JsonMaybeValue JsonChecker::root(std::string const& hierarchy) {
    return JsonMaybeValue(*this, m_json, hierarchy, true);
}

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
    for (auto& [k, v] : m_impl->scope.as_object()) {
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
    return m_impl->scope.as_array().size();
}
JsonExpectedValue JsonExpectedValue::at(size_t index) {
    if (this->hasError()) {
        return JsonExpectedValue();
    }
    if (!this->assertIs(matjson::Type::Array)) {
        return JsonExpectedValue();
    }
    auto& arr = m_impl->scope.as_array();
    if (arr.size() <= index) {
        this->setError(
            "array expected to have at least size {}, but its size was only {}",
            index + 1, arr.size()
        );
        return JsonExpectedValue();
    }
    return JsonExpectedValue(m_impl.get(), arr.at(index), std::to_string(index));
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
    for (auto& v : m_impl->scope.as_array()) {
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
