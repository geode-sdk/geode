#pragma once

#include <Geode/DefaultInclude.hpp>
#include <memory>

namespace geode::utils {

    template <class FunctionType>
    class MiniFunction;

    template <class Ret, class... Args>
    class MiniFunctionStateBase {
    public:
        virtual ~MiniFunctionStateBase() = default;
        virtual Ret call(Args... args) const = 0;
        virtual MiniFunctionStateBase* clone() const = 0;
    };

    template <class Type, class Ret, class... Args>
    class MiniFunctionState final : public MiniFunctionStateBase<Ret, Args...> {
    public:
        Type m_func;

        explicit MiniFunctionState(Type func) : m_func(func) {}

        Ret call(Args... args) const override {
            return const_cast<Type&>(m_func)(args...);
        }

        MiniFunctionStateBase<Ret, Args...>* clone() const override {
            return new MiniFunctionState(*this);
        }
    };

    template <class Type, class Ret, class... Args>
    class MiniFunctionStatePointer final : public MiniFunctionStateBase<Ret, Args...> {
    public:
        Type m_func;

        explicit MiniFunctionStatePointer(Type func) : m_func(func) {}

        Ret call(Args... args) const override {
            return const_cast<Type&>(*m_func)(args...);
        }

        MiniFunctionStateBase<Ret, Args...>* clone() const override {
            return new MiniFunctionStatePointer(*this);
        }
    };

    template <class Type, class Ret, class Class, class... Args>
    class MiniFunctionStateMemberPointer final : public MiniFunctionStateBase<Ret, Class, Args...> {
    public:
        Type m_func;

        explicit MiniFunctionStateMemberPointer(Type func) : m_func(func) {}

        Ret call(Class self, Args... args) const override {
            return const_cast<Type&>(self->*m_func)(args...);
        }

        MiniFunctionStateBase<Ret, Class, Args...>* clone() const override {
            return new MiniFunctionStateMemberPointer(*this);
        }
    };
    
    template <class Callable, class Ret, class... Args>
    concept MiniFunctionCallable = requires(Callable&& func, Args... args) {
        { func(args...) } -> std::same_as<Ret>;
    };

    template <class Ret, class... Args>
    class MiniFunction<Ret(Args...)> {
    public:
        using FunctionType = Ret(Args...);
        using StateType = MiniFunctionStateBase<Ret, Args...>;

    private:
        StateType* m_state;

    public:
        MiniFunction() : m_state(nullptr) {}

        MiniFunction(std::nullptr_t) : MiniFunction() {}

        MiniFunction(MiniFunction const& other) :
            m_state(other.m_state ? other.m_state->clone() : nullptr) {}

        MiniFunction(MiniFunction&& other) : m_state(other.m_state) {
            other.m_state = nullptr;
        }

        ~MiniFunction() {
            delete m_state;
        }

        template <class Callable>
        requires(MiniFunctionCallable<Callable, Ret, Args...> && !std::is_same_v<std::decay_t<Callable>, MiniFunction<FunctionType>>)
        MiniFunction(Callable&& func) :
            m_state(new MiniFunctionState<std::decay_t<Callable>, Ret, Args...>(std::forward<Callable>(func))) {}

        template <class FunctionPointer> 
        requires(!MiniFunctionCallable<FunctionPointer, Ret, Args...> && std::is_pointer_v<FunctionPointer> && std::is_function_v<std::remove_pointer_t<FunctionPointer>>)
        MiniFunction(FunctionPointer func) :
            m_state(new MiniFunctionStatePointer<FunctionPointer, Ret, Args...>(func)) {}

        template <class MemberFunctionPointer> 
        requires(std::is_member_function_pointer_v<MemberFunctionPointer>)
        MiniFunction(MemberFunctionPointer func) :
            m_state(new MiniFunctionStateMemberPointer<MemberFunctionPointer, Ret, Args...>(func)) {}

        MiniFunction& operator=(MiniFunction const& other) {
            delete m_state;
            m_state = other.m_state ? other.m_state->clone() : nullptr;
            return *this;
        }

        MiniFunction& operator=(MiniFunction&& other) {
            delete m_state;
            m_state = other.m_state;
            other.m_state = nullptr;
            return *this;
        }

        Ret operator()(Args... args) const {
            if (!m_state) return Ret();
            return m_state->call(args...);
        }

        explicit operator bool() const {
            return m_state;
        }
    };
}
