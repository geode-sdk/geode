#pragma once

#include "json.hpp"

struct json_check_failure : public std::exception {
    std::string m_info;
    const char* what() const throw() {
        return m_info.c_str();
    }
    json_check_failure(std::string const& info) : m_info(info) {}
};

struct json_check {
    nlohmann::json m_json;
    bool m_continue = true;
    bool m_branched = false;
    bool m_hasBranch= false;
    bool m_required = false;
    std::string m_key = "";
    std::string m_hierarchy = "";
    std::string m_types = "";
    std::set<std::string_view>& m_knownKeys;

    nlohmann::json const& get_json() {
        return m_key.size() ? m_json[m_key] : m_json;
    }

    json_check(
        std::set<std::string_view>& knownKeys,
        nlohmann::json const& json
    ) : m_knownKeys(knownKeys), m_json(json) {}

    json_check(
        std::set<std::string_view>& knownKeys,
        json_check const& other
    ) : json_check(knownKeys, other.m_json) {}

    json_check& has(std::string_view const& key) {
        if (!m_continue) return *this;
        m_knownKeys.insert(key);
        m_continue = m_json.contains(key);
        m_key = key;
        return *this;
    }
    json_check& needs(std::string_view const& key) {
        if (!m_continue) return *this;
        if (!m_json.contains(key))
            throw json_check_failure(
                "[mod.json]" + m_hierarchy + " is missing required key \"" + std::string(key) + "\""
            );
        m_knownKeys.insert(key);
        m_key = key;
        m_required = true;
        return *this;
    }
    template<typename T>
    json_check& as() {
        if (!m_continue) return *this;
        auto json = get_json();
        auto keyName = m_key.size() ? "[self]" : m_key;
        if constexpr (std::is_same_v<T, std::string>) {
            m_types += "string, ";
            if (!json.is_string()) {
                if (m_required) {
                    throw json_check_failure("[mod.json]" + m_hierarchy + "." + keyName + " is not a string");
                } else {
                    if (json.is_null()) {
                        m_continue = false;
                    } else {
                        throw json_check_failure("[mod.json]" + m_hierarchy + "." + keyName + " is not a string nor null");
                    }
                }
            }
        } else
        if constexpr (std::is_same_v<T, int>) {
            m_types += "int, ";
            if (!json.is_number_integer()) {
                if (m_required) {
                    throw json_check_failure("[mod.json]" + m_hierarchy + "." + keyName + " is not a int");
                } else {
                    if (json.is_null()) {
                        m_continue = false;
                    } else {
                        throw json_check_failure("[mod.json]" + m_hierarchy + "." + keyName + " is not a int nor null");
                    }
                }
            }
        } else
        if constexpr (std::is_same_v<T, bool>) {
            m_types += "bool, ";
            if (!json.is_boolean()) {
                if (m_required) {
                    throw json_check_failure("[mod.json]" + m_hierarchy + "." + keyName + " is not a boolean");
                } else {
                    if (json.is_null()) {
                        m_continue = false;
                    } else {
                        throw json_check_failure("[mod.json]" + m_hierarchy + "." + keyName + " is not a boolean nor null");
                    }
                }
            }
        } else
        if constexpr (std::is_same_v<T, nlohmann::json::object_t>) {
            m_types += "object, ";
            if (!json.is_object()) {
                if (m_required) {
                    throw json_check_failure("[mod.json]" + m_hierarchy + "." + keyName + " is not a object");
                } else {
                    if (json.is_null()) {
                        m_continue = false;
                    } else {
                        throw json_check_failure("[mod.json]" + m_hierarchy + "." + keyName + " is not a object nor null");
                    }
                }
            }
        } else
        if constexpr (std::is_same_v<T, nlohmann::json::array_t>) {
            m_types += "array, ";
            if (!json.is_array()) {
                if (m_required) {
                    throw json_check_failure("[mod.json]" + m_hierarchy + "." + keyName + " is not a array");
                } else {
                    if (json.is_null()) {
                        m_continue = false;
                    } else {
                        throw json_check_failure("[mod.json]" + m_hierarchy + "." + keyName + " is not a array nor null");
                    }
                }
            }
        } else
            static_assert(!std::is_same_v<T, T>, "Unimplemented type for json_check");
        return *this;
    }
    template<typename T>
    json_check& is(std::function<void(nlohmann::json const&)> branch) {
        if (!m_continue) return *this;
        m_hasBranch = true;
        try { this->as<T>(); } catch(json_check_failure&) { return *this; }
        branch(get_json());
        m_continue = false;
        m_branched = true;
        return *this;
    }
    json_check& is_ok() {
        if (m_hasBranch && !m_branched) {
            throw json_check_failure("[mod.json]" + m_hierarchy + "." + m_key + " is not one of " + m_types.substr(0, m_types.size() - 2));
        }
        return *this;
    }
    json_check& validate(std::function<bool(nlohmann::json const&)> predicate) {
        if (!m_continue) return *this;
        if (!predicate(get_json())) {
            throw json_check_failure("[mod.json]" + m_hierarchy + "." + m_key + " is invalidly formatted");
        }
        return *this;
    }
    template<typename T>
    json_check& into(T& var) {
        if (!m_continue) return *this;
        var = get_json().get<T>();
        return *this;
    }
    template<typename T>
    json_check& into_if(T& var) {
        return this->is<T>([&var](nlohmann::json const& json) -> void {
            std::set<std::string_view> knownKeys;
            json_check(knownKeys, json).as<T>().into(var);
        });
    }
    template<typename T>
    json_check& into_as(T& var) {
        return this->as<T>().into(var);
    }
    json_check& into(std::function<void(nlohmann::json const&)> var) {
        if (!m_continue) return *this;
        var(get_json());
        return *this;
    }
    json_check& each(std::function<void(std::string const&, json_check)> func) {
        if (!m_continue) return *this;
        for (auto const& [key, val] : get_json().items()) {
            std::set<std::string_view> knownKeys;
            auto c = json_check(knownKeys, *this);
            c.step();
            c.m_key = key;
            c.step();
            func(key, c);
        }
        return *this;
    }
    json_check& each(std::function<void(json_check)> func) {
        if (!m_continue) return *this;
        size_t ix = 0;
        for (auto const& val : get_json()) {
            std::set<std::string_view> knownKeys;
            auto c = json_check(knownKeys, val);
            c.m_hierarchy = m_hierarchy;
            if (m_key.size()) c.m_hierarchy += "." + m_key;
            c.m_hierarchy += "." + std::to_string(ix);
            func(c);
            ix++;
        }
        return *this;
    }
    json_check& step() {
        if (!m_continue) return *this;
        if (m_key.size()) {
            this->m_hierarchy += "." + m_key;
            this->m_json = get_json();
            m_key = "";
        }
        return *this;
    }

    template<typename T>
    T get() {
        if (!m_continue) return T();
        return this->get_json().get<T>();
    }
};

template<typename T>
struct json_assign_required : json_check {
    json_assign_required(
        std::set<std::string_view>& knownKeys,
        nlohmann::json& json,
        std::string_view const& key,
        T& var
    ) : json_check(knownKeys, json) {
        this->needs(key).template as<T>().into(var);
    }
};

template<typename T>
struct json_assign_optional : json_check {
    json_assign_optional(
        std::set<std::string_view>& knownKeys,
        nlohmann::json& json,
        std::string_view const& key,
        T& var
    ) : json_check(knownKeys, json) {
        this->has(key).template as<T>().into(var);
    }
};

struct json_check_unknown {
    json_check_unknown(
        std::set<std::string_view>& knownKeys,
        nlohmann::json& json,
        std::string const& hierarchy
    ) {
        for (auto& [key, _] : json.items()) {
            if (!knownKeys.count(key)) {
                InternalMod::get()->log() << Severity::Warning
                    << "[mod.json]" << hierarchy
                    << " contains unknown key \"" << key << "\"";
                // todo: log as warning for mod
            }
        }
    }
};
