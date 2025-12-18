#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <format>
#include <functional>
#include <regex>

#ifdef LUA50
#include "luapp/luapp50.h"
#endif
#ifdef LUA51
#include "luapp/luapp51.h"
#endif
#ifdef LUAJIT
#include "luapp/luappjit.h"
#endif
#ifdef LUA52
#include "luapp/luapp52.h"
#endif
#ifdef LUA53
#include "luapp/luapp53.h"
#endif
#ifdef LUA54
#include "luapp/luapp54.h"
#endif

#if defined(_MSC_VER) && !defined(__llvm__) && !defined(__INTEL_COMPILER)
std::string_view demangle(std::string_view v)
{
    return v;
}
#else
#include <cxxabi.h>
std::string demangle(std::string_view v)
{
    int status = 0;
    const std::unique_ptr<char, decltype([](char* c) {std::free(c);})> p{
        abi::__cxa_demangle(v.data(), nullptr, nullptr, &status)
    };
    CHECK(status == 0);
    return std::string{p.get()};
}
#endif

namespace LuappDev
{
    class IntHolderOp
    {
        lua::Integer i;

        static int get(lua::State L)
        {
            auto t = L.CheckUserClass<IntHolderOp>(1);
            L.Push(t->i);
            return 1;
        }
        static int set(lua::State L)
        {
            auto t = L.CheckUserClass<IntHolderOp>(1);
            t->i = L.CheckInt(2);
            return 0;
        }

    public:
        IntHolderOp(lua::Integer i) : i(i) {}

        auto operator<=>(const IntHolderOp&) const noexcept = default;

        IntHolderOp operator+(const IntHolderOp& o) const noexcept { return IntHolderOp{this->i + o.i}; }
        IntHolderOp operator-(const IntHolderOp& o) const noexcept { return IntHolderOp{this->i - o.i}; }
        IntHolderOp operator*(const IntHolderOp& o) const noexcept { return IntHolderOp{this->i * o.i}; }
        IntHolderOp operator/(const IntHolderOp& o) const noexcept { return IntHolderOp{this->i / o.i}; }
        IntHolderOp operator-() const noexcept { return IntHolderOp{-this->i}; }

        IntHolderOp operator&(const IntHolderOp& o) const noexcept { return IntHolderOp{this->i & o.i}; }
        IntHolderOp operator|(const IntHolderOp& o) const noexcept { return IntHolderOp{this->i | o.i}; }
        IntHolderOp operator^(const IntHolderOp& o) const noexcept { return IntHolderOp{this->i ^ o.i}; }
        IntHolderOp operator~() const noexcept { return IntHolderOp{~this->i}; }
        IntHolderOp operator<<(const IntHolderOp& o) const noexcept { return IntHolderOp{this->i << o.i}; }
        IntHolderOp operator>>(const IntHolderOp& o) const noexcept { return IntHolderOp{this->i >> o.i}; }

        static constexpr std::array<lua::FuncReference, 2> LuaMethods{{
            lua::FuncReference::GetRef<get>("Get"),
            lua::FuncReference::GetRef<set>("Set"),
        }};
    };

    class IntHolderLua
    {
        lua::Integer i;

        static int get(lua::State L)
        {
            auto t = L.CheckUserClass<IntHolderLua>(1);
            L.Push(t->i);
            return 1;
        }
        static int set(lua::State L)
        {
            auto t = L.CheckUserClass<IntHolderLua>(1);
            t->i = L.CheckInt(2);
            return 0;
        }

    public:
        IntHolderLua(lua::Integer i) : i(i) {}

        static constexpr std::array<lua::FuncReference, 2> LuaMethods{{
            lua::FuncReference::GetRef<get>("Get"),
            lua::FuncReference::GetRef<set>("Set"),
        }};

        static int Equals(lua::State L)
        {
            auto t = L.CheckUserClass<IntHolderLua>(1);
            auto o = L.CheckUserClass<IntHolderLua>(2);
            L.Push(t->i == o->i);
            return 1;
        }
        static int LessThan(lua::State L)
        {
            auto t = L.CheckUserClass<IntHolderLua>(1);
            auto o = L.CheckUserClass<IntHolderLua>(2);
            L.Push(t->i < o->i);
            return 1;
        }
        static int LessOrEquals(lua::State L)
        {
            auto t = L.CheckUserClass<IntHolderLua>(1);
            auto o = L.CheckUserClass<IntHolderLua>(2);
            L.Push(t->i <= o->i);
            return 1;
        }
        static int Add(lua::State L)
        {
            auto t = L.CheckUserClass<IntHolderLua>(1);
            auto o = L.CheckUserClass<IntHolderLua>(2);
            L.NewUserClass<IntHolderLua>(t->i + o->i);
            return 1;
        }
        static int Substract(lua::State L)
        {
            auto t = L.CheckUserClass<IntHolderLua>(1);
            auto o = L.CheckUserClass<IntHolderLua>(2);
            L.NewUserClass<IntHolderLua>(t->i - o->i);
            return 1;
        }
        static int Multiply(lua::State L)
        {
            auto t = L.CheckUserClass<IntHolderLua>(1);
            auto o = L.CheckUserClass<IntHolderLua>(2);
            L.NewUserClass<IntHolderLua>(t->i * o->i);
            return 1;
        }
        static int Divide(lua::State L)
        {
            auto t = L.CheckUserClass<IntHolderLua>(1);
            auto o = L.CheckUserClass<IntHolderLua>(2);
            L.NewUserClass<IntHolderLua>(t->i / o->i);
            return 1;
        }
        static int Pow(lua::State L)
        {
            auto t = L.CheckUserClass<IntHolderLua>(1);
            auto o = L.CheckUserClass<IntHolderLua>(2);
            L.NewUserClass<IntHolderLua>(static_cast<lua::Integer>(std::pow(t->i, o->i)));
            return 1;
        }
        static int UnaryMinus(lua::State L)
        {
            auto t = L.CheckUserClass<IntHolderLua>(1);
            L.NewUserClass<IntHolderLua>(-t->i);
            return 1;
        }
        static int Concat(lua::State L)
        {
            auto t = L.CheckUserClass<IntHolderLua>(1);
            auto o = L.CheckUserClass<IntHolderLua>(2);
            L.Push(t->i);
            L.Push(o->i);
            L.Concat(2);
            return 1;
        }
        static int NewIndex(lua::State L)
        {
            auto t = L.CheckUserClass<IntHolderLua>(1);
            if (L.CheckStringView(2) != "i")
                throw lua::LuaException{"invalid key"};
            t->i = L.CheckInt(3);
            return 0;
        }
        static int Index(lua::State L)
        {
            auto t = L.CheckUserClass<IntHolderLua>(1);
            if (L.CheckStringView(2) != "i")
                throw lua::LuaException{"invalid key"};
            L.Push(t->i);
            return 1;
        }
        static int Call(lua::State L)
        {
            auto t = L.CheckUserClass<IntHolderLua>(1);
            auto o = L.CheckInt(2);
            L.NewUserClass<IntHolderLua>(t->i * o);
            return 1;
        }
        static int Modulo(lua::State L)
        {
            auto t = L.CheckUserClass<IntHolderLua>(1);
            auto o = L.CheckUserClass<IntHolderLua>(2);
            L.NewUserClass<IntHolderLua>(t->i % o->i);
            return 1;
        }
        static int Length(lua::State L)
        {
            auto t = L.CheckUserClass<IntHolderLua>(1);
            L.Push(t->i);
            L.ToString(-1);
            auto l = L.RawLength(-1);
            L.Push(static_cast<lua::Integer>(l));
            return 1;
        }
        static int IntegerDivide(lua::State L)
        {
            auto t = L.CheckUserClass<IntHolderLua>(1);
            auto o = L.CheckUserClass<IntHolderLua>(2);
            L.NewUserClass<IntHolderLua>(t->i / o->i + 1);
            return 1;
        }
        static int BitwiseAnd(lua::State L)
        {
            auto t = L.CheckUserClass<IntHolderLua>(1);
            auto o = L.CheckUserClass<IntHolderLua>(2);
            L.NewUserClass<IntHolderLua>(t->i & o->i);
            return 1;
        }
        static int BitwiseOr(lua::State L)
        {
            auto t = L.CheckUserClass<IntHolderLua>(1);
            auto o = L.CheckUserClass<IntHolderLua>(2);
            L.NewUserClass<IntHolderLua>(t->i | o->i);
            return 1;
        }
        static int BitwiseXOr(lua::State L)
        {
            auto t = L.CheckUserClass<IntHolderLua>(1);
            auto o = L.CheckUserClass<IntHolderLua>(2);
            L.NewUserClass<IntHolderLua>(t->i ^ o->i);
            return 1;
        }
        static int BitwiseNot(lua::State L)
        {
            auto t = L.CheckUserClass<IntHolderLua>(1);
            L.NewUserClass<IntHolderLua>(~t->i);
            return 1;
        }
        static int ShiftLeft(lua::State L)
        {
            auto t = L.CheckUserClass<IntHolderLua>(1);
            auto o = L.CheckUserClass<IntHolderLua>(2);
            L.NewUserClass<IntHolderLua>(t->i << o->i);
            return 1;
        }
        static int ShiftRight(lua::State L)
        {
            auto t = L.CheckUserClass<IntHolderLua>(1);
            auto o = L.CheckUserClass<IntHolderLua>(2);
            L.NewUserClass<IntHolderLua>(t->i >> o->i);
            return 1;
        }
        static int ToString(lua::State L)
        {
            auto t = L.CheckUserClass<IntHolderLua>(1);
            L.Push(std::format("IntHolder {}", t->i));
            return 1;
        }
    };
    class DtorTest
    {
        std::function<void()> f;

    public:
        DtorTest(std::function<void()> f) : f(std::move(f)) {}

        ~DtorTest() { f(); }
    };
    class InheritanceTestB
    {
    protected:
        lua::Integer i;

        static int geti(lua::State L)
        {
            auto t = L.CheckUserClass<InheritanceTestB>(1);
            L.Push(t->i);
            return 1;
        }
        static int seti(lua::State L)
        {
            auto t = L.CheckUserClass<InheritanceTestB>(1);
            t->i = L.CheckInt(2);
            return 0;
        }

    public:
        using BaseClass = InheritanceTestB;

        InheritanceTestB(lua::Integer i) : i(i) {}
        virtual ~InheritanceTestB() = default;

        static constexpr std::array<lua::FuncReference, 2> LuaMethods{{
            lua::FuncReference::GetRef<geti>("GetI"),
            lua::FuncReference::GetRef<seti>("SetI"),
        }};

        InheritanceTestB operator+(const InheritanceTestB& o) const { return {i + o.i}; }
        InheritanceTestB operator-() const { return {-i}; }
    };
    class InheritanceTestD : public InheritanceTestB
    {
        lua::Number j;
        static int getj(lua::State L)
        {
            auto t = L.CheckUserClass<InheritanceTestD>(1);
            L.Push(t->j);
            return 1;
        }
        static int setj(lua::State L)
        {
            auto t = L.CheckUserClass<InheritanceTestD>(1);
            t->j = L.CheckNumber(2);
            return 0;
        }

    public:
        InheritanceTestD(lua::Integer i, lua::Number j) : InheritanceTestB(i), j(j) {}

        static constexpr std::array<lua::FuncReference, 4> LuaMethods{{
            lua::FuncReference::GetRef<geti>("GetI"),
            lua::FuncReference::GetRef<seti>("SetI"),
            lua::FuncReference::GetRef<getj>("GetJ"),
            lua::FuncReference::GetRef<setj>("SetJ"),
        }};

        InheritanceTestD operator+(const InheritanceTestD& o) const { return {i + o.i, j + o.j}; }
    };
    class InheritanceTestF : public InheritanceTestB
    {
        std::function<void()> f;

    public:
        InheritanceTestF(std::function<void()> f) : InheritanceTestB(42), f(std::move(f)) {}

        ~InheritanceTestF() override { f(); }
    };
    template <class S>
    class UservalueTest
    {
        static int get(S L)
        {
            L.template CheckUserClass<UservalueTest>(1);
            if constexpr (S::Capabilities::ArbitraryUservalues)
            {
                for (int i = 0; i < NumberUserValues; ++i)
                    L.GetUserValue(1, i + 1);
            }
            else if constexpr (lua::State::Capabilities::Uservalues)
            {
                L.GetUserValue(1);
            }
            return NumberUserValues;
        }
        static int set(S L)
        {
            L.template CheckUserClass<UservalueTest>(1);
            if constexpr (S::Capabilities::ArbitraryUservalues)
            {
                for (int i = 0; i < NumberUserValues; ++i)
                    L.SetUserValue(1, i + 1);
            }
            else if constexpr (lua::State::Capabilities::Uservalues)
            {
                L.SetUserValue(1);
            }
            return 0;
        }
        static int n(S L)
        {
            L.Push(NumberUserValues);
            return 1;
        }

    public:
        static constexpr int NumberUserValues =
            lua::State::Capabilities::Uservalues ? (lua::State::Capabilities::ArbitraryUservalues ? 3 : 1) : 0;

        static constexpr std::array<lua::FuncReference, 3> LuaMethods{{
            lua::FuncReference::GetRef<get>("Get"),
            lua::FuncReference::GetRef<set>("Set"),
            lua::FuncReference::GetRef<n>("n"),
        }};
    };

    bool RegexMatch(const char* p, std::string_view data)
    {
        std::regex pattern{p, std::regex_constants::ECMAScript};
        return std::regex_match(data.begin(), data.end(), pattern);
    }
    void AssertRegex(const char* p, std::string_view data)
    {
        CHECK_MESSAGE(RegexMatch(p, data), std::format("expected '{}', found '{}'", p, data));
    }

    namespace funcs
    {

        static int foo_raw(lua_State* l)
        {
            lua::State L{l};
            L.PushValue(1);
            L.Push(42);
            L.Arithmetic(lua::ArihmeticOperator::Add);
            return 1;
        }
        static int foo(lua::State L)
        {
            L.PushValue(1);
            L.Push(42);
            L.Arithmetic(lua::ArihmeticOperator::Add);
            return 1;
        }
        static int foo_up(lua::State L)
        {
            CHECK_EQ(1, L.Debug_GetStackDepth());
            L.PushValue(1);
            L.PushValue(lua::State::Upvalueindex(1));
            L.Arithmetic(lua::ArihmeticOperator::Add);
            return 1;
        }
        static int ex(lua::State L)
        {
            auto i = L.CheckInt(1);
            throw lua::LuaException{std::format("number is {}", i)};
        }
        static constexpr std::array<lua::FuncReference, 1> toRegister{{
            lua::FuncReference::GetRef<foo>("foo"),
        }};

    } // namespace funcs

    TEST_CASE("CreateDelete")
    {
        lua::State L{};

        lua::UniqueState closer{L};

        CHECK(L.GetState() == closer.GetState());
        CHECK_EQ(demangle(typeid(DtorTest).name()), typename_details::type_name<DtorTest>());

        bool closed = false;
        {
            closer.NewUserClass<DtorTest>([&closed]() { closed = true; });

            CHECK(!closed);

            lua::UniqueState c2{std::move(closer)};
            CHECK(L.GetState() == c2.GetState());
            CHECK(closer.GetState() == static_cast<lua_State*>(nullptr));

            CHECK(!closed);
        }

        CHECK(closed);
    }

    TEST_CASE("StackAccess")
    {
        lua::UniqueState L{};

        CHECK(0 == L.GetTop());

        L.Push(1);
        L.Push(true);
        CHECK(2 == L.GetTop());
        CHECK(L.IsNumber(1));
        CHECK(L.IsBoolean(2));

        L.PushValue(2);
        CHECK(3 == L.GetTop());
        CHECK(L.IsBoolean(3));
        L.SetTop(2);
        CHECK(2 == L.GetTop());
        CHECK(2 == L.ToAbsoluteIndex(-1));
        CHECK(1 == L.ToAbsoluteIndex(-2));
        CHECK(L.REGISTRYINDEX == L.ToAbsoluteIndex(L.REGISTRYINDEX));

        CHECK(lua::Integer{1} == L.CheckInteger(1));
        CHECK(L.CheckBool(2));

        L.Push();
        L.Insert(2);
        CHECK(3 == L.GetTop());
        CHECK(lua::Integer{1} == L.CheckInteger(1));
        CHECK(L.IsNil(2));
        CHECK(L.CheckBool(3));

        L.Push("");
        L.Replace(2);
        CHECK(3 == L.GetTop());
        CHECK(lua::Integer{1} == L.CheckInteger(1));
        CHECK(std::string_view("") == L.CheckString(2));
        CHECK(L.CheckBool(3));

        L.Copy(1, 2);
        CHECK(3 == L.GetTop());
        CHECK(lua::Integer{1} == L.CheckInteger(1));
        CHECK(lua::Integer{1} == L.CheckInteger(2));
        CHECK(L.CheckBool(3));

        L.Remove(2);
        CHECK(2 == L.GetTop());
        CHECK(lua::Integer{1} == L.CheckInteger(1));
        CHECK(L.CheckBool(2));

        L.Pop(2);
        CHECK(0 == L.GetTop());
    }

    template <class S>
    void BaseDatatypes_T()
    {
        S L{};
        CHECK(0 == L.GetTop());

        CHECK(L.IsNone(1));
        CHECK(L.IsNoneOrNil(1));
        CHECK(L.Type(1) == lua::LType::None);

        L.Push();
        CHECK(L.IsNil(1));
        CHECK(L.IsNoneOrNil(1));
        CHECK(L.Type(1) == lua::LType::Nil);
        CHECK(std::string_view{"nil"} == L.ConvertToString(1));
        CHECK(std::string{"nil"} == L.ToDebugString(1));
        L.SetTop(0);

        L.Push(1);
        CHECK(L.IsNumber(1));
        if constexpr (S::Capabilities::NativeIntegers)
            CHECK(L.IsInteger(1));

        CHECK(lua::Integer{1} == L.CheckInteger(1));
        CHECK(1.0 == L.CheckNumber(1));
        CHECK(L.Type(1) == lua::LType::Number);
        CHECK(std::string_view{"1"} == L.ConvertToString(1));
        CHECK(std::string{"1"} == L.ToDebugString(1));
        L.SetTop(0);

        L.Push(1.3);
        CHECK(L.IsNumber(1));
        if constexpr (S::Capabilities::NativeIntegers)
            CHECK(!L.IsInteger(1));
        else
            CHECK(lua::Integer{1} == L.CheckInteger(1));
        CHECK(1.3 == L.CheckNumber(1));
        CHECK(L.Type(1) == lua::LType::Number);
        CHECK(std::string_view{"1.3"} == L.ConvertToString(1));
        CHECK(std::string{"1.3"} == L.ToDebugString(1));
        L.SetTop(0);

        L.Push("a");
        CHECK(L.IsString(1));
        CHECK(std::string_view{"a"} == L.CheckString(1));
        CHECK(std::string_view{"a"} == L.CheckStringView(1));
        CHECK(L.Type(1) == lua::LType::String);
        CHECK(std::string_view{"a"} == L.ConvertToString(1));
        CHECK(std::string{"\"a\""} == L.ToDebugString(1));
        L.SetTop(0);

        L.PushFString("a %d", 42);
        CHECK(L.IsString(1));
        CHECK_EQ(std::string_view{"a 42"}, L.CheckString(1));
        CHECK_EQ(std::string_view{"a 42"}, L.CheckStringView(1));
        CHECK(L.Type(1) == lua::LType::String);
        L.SetTop(0);

        L.Push(true);
        CHECK(L.IsBoolean(1));
        CHECK_EQ(true, L.CheckBool(1));
        CHECK(L.Type(1) == lua::LType::Boolean);
        CHECK_EQ(std::string_view{"true"}, L.ConvertToString(1));
        CHECK_EQ(std::string{"true"}, L.ToDebugString(1));
        L.SetTop(0);

        L.PushLightUserdata(&L);
        CHECK(L.IsUserdata(1));
        CHECK(L.IsLightUserdata(1));
        CHECK_EQ(static_cast<void*>(&L), L.ToUserdata(1));
        CHECK(L.Type(1) == lua::LType::LightUserdata);
        AssertRegex("userdata: 0x[0-9a-f]+", L.ConvertToString(1));
        AssertRegex("<LightUserdata 0x[0-9a-f]+>", L.ToDebugString(1));
        L.SetTop(0);

        if constexpr (S::Capabilities::Uservalues)
        {
            if constexpr (S::Capabilities::ArbitraryUservalues)
            {
                L.NewUserdata(sizeof(int), 1);
            }
            else
            {
                L.NewUserdata(sizeof(int));
            }
            CHECK(L.IsUserdata(1));
            CHECK(!L.IsLightUserdata(1));
            CHECK(L.Type(1) == lua::LType::Userdata);
            AssertRegex("userdata: 0x[0-9a-f]+", L.ConvertToString(1));
            AssertRegex("<Userdata unknown type 0x[0-9a-f]+>", L.ToDebugString(1));
            L.SetTop(1);

            L.NewTable();
            L.PushValue(2);
            L.SetUserValue(1);
            L.GetUserValue(1);
            CHECK(L.IsTable(3));
            CHECK(L.RawEqual(2, 3));
            L.SetTop(0);
        }
    }
    TEST_CASE("BaseDatatypes") { BaseDatatypes_T<lua::UniqueState>(); }

    TEST_CASE("Functions")
    {
        lua::UniqueState L{};
        CHECK_EQ(0, L.GetTop());

        L.Push(&funcs::foo_raw);
        CHECK(L.IsFunction(1));
        CHECK(L.IsCFunction(1));
        CHECK(L.Type(1) == lua::LType::Function);

        L.Push(5);
        CHECK_EQ(1, L.TCall(1, 1));
        CHECK_EQ(1, L.GetTop());
        CHECK_EQ(lua::Integer{42 + 5}, L.CheckInteger(1));
        L.SetTop(0);

        L.Push<funcs::foo>();
        CHECK(L.IsFunction(1));
        CHECK(L.IsCFunction(1));
        CHECK(L.Type(1) == lua::LType::Function);

        L.Push(5);
        CHECK_EQ(1, L.TCall(1, 1));
        CHECK_EQ(1, L.GetTop());
        CHECK_EQ(lua::Integer{42 + 5}, L.CheckInteger(1));
        L.SetTop(0);

        L.Push(44);
        L.Push<funcs::foo_up>(1);
        L.Push(5);
        CHECK_EQ(1, L.TCall(1, L.MULTIRET));
        CHECK_EQ(1, L.GetTop());
        CHECK_EQ(lua::Integer{44 + 5}, L.CheckInteger(1));
        L.SetTop(0);

        L.Push<funcs::foo>();

        L.Push(5);
        L.Push("bar");
        CHECK_EQ(2, L.TCall(2, 2));
        CHECK_EQ(lua::Integer{42 + 5}, L.CheckInteger(1));
        CHECK(L.IsNil(2));
        L.SetTop(0);

        L.Push<funcs::ex>();
        L.Push(5);
        CHECK(L.PCall(1, 0) == lua::ErrorCode::Runtime);
        CHECK(L.CheckStringView(1).find("number is 5") != std::string_view::npos);
        L.SetTop(0);
        bool ok = false;
        try
        {
            L.Push<funcs::ex>();
            L.Push(5);
            L.TCall(1, 0);
            L.SetTop(0);
        }
        catch (const lua::LuaException& e)
        {
            if (std::string_view{e.what()}.find("number is 5") != std::string_view::npos)
                ok = true;
        }
        CHECK(ok);

        L.SetTop(0);
        struct A
        {
            int i;

            [[nodiscard]] int P(lua::State L) // NOLINT(*-make-member-function-const)
            {
                L.Push(i + L.CheckInt(1) + L.CheckInt(lua::State::Upvalueindex(2)));
                return 1;
            }
            [[nodiscard]] int PC(lua::State L) const
            {
                L.Push(i + 1 + L.CheckInt(1) + L.CheckInt(lua::State::Upvalueindex(2)));
                return 1;
            }
            [[nodiscard]] int G(lua::State L) // NOLINT(*-make-member-function-const)
            {
                L.Push(i);
                return 1;
            }
            [[nodiscard]] int GC(lua::State L) const
            {
                L.Push(i + 1);
                return 1;
            }
        };
        A a{5};
        L.Push(3);
        L.Push<A, &A::P>(a, 1);
        L.Push(4);
        L.TCall(1, 1);
        CHECK_EQ(12, L.CheckInt(1));
        L.SetTop(0);

        L.Push(3);
        L.Push<A, &A::PC>(a, 1);
        L.Push(4);
        L.TCall(1, 1);
        CHECK_EQ(13, L.CheckInt(1));
        L.SetTop(0);

        std::array<lua::FuncReference, 2> toregobj{{
            lua::FuncReference::GetRef<A, &A::G>(a, "G"),
            lua::FuncReference::GetRef<A, &A::GC>(a, "GC"),
        }};
        L.RegisterGlobalLib(toregobj, "AFuncs");
        CHECK_EQ(0, L.GetTop());
        L.GetGlobal("AFuncs");
        CHECK(L.IsTable(1));
        L.GetTableRaw(1, "G");
        CHECK(L.IsFunction(2));
        AssertRegex(R"(<function C  AFuncs\.G \(defined in: C:0x[0-9a-f]+\)>)", L.ToDebugString(2));
        L.TCall(0, 1);
        CHECK(L.IsNumber(2));
        CHECK_EQ(5.0, *L.ToNumber(2));
        L.SetTop(1);
        L.GetTableRaw(1, "GC");
        CHECK(L.IsFunction(2));
        L.TCall(0, 1);
        CHECK(L.IsNumber(2));
        CHECK_EQ(6.0, *L.ToNumber(2));
        L.SetTop(1);
    }


    template <class S>
    void Tables_T()
    {
        S L{};
        CHECK_EQ(0, L.GetTop());

        L.NewTable();
        CHECK_EQ(1, L.GetTop());
        CHECK(L.IsTable(1));
        AssertRegex("<table 0x[0-9a-f]+>", L.ToDebugString(1));

        L.Push("a");
        L.Push(42);
        L.SetTable(1);
        CHECK_EQ(1, L.GetTop());
        L.Push("a");
        L.GetTable(1);
        CHECK_EQ(lua::Integer{42}, L.CheckInteger(2));
        L.Pop(1);
        CHECK_EQ(1, L.GetTop());

        L.Push("a");
        L.Push(43);
        L.SetTableRaw(1);
        CHECK_EQ(1, L.GetTop());
        L.Push("a");
        L.GetTableRaw(1);
        CHECK_EQ(lua::Integer{43}, L.CheckInteger(2));
        L.Pop(1);
        CHECK_EQ(1, L.GetTop());

        L.Push(44);
        L.SetTableRaw(1, "a");
        CHECK_EQ(1, L.GetTop());
        L.GetTableRaw(1, "a");
        CHECK_EQ(lua::Integer{44}, L.CheckInteger(2));
        L.Pop(1);
        CHECK_EQ(1, L.GetTop());

        L.Push("a");
        L.SetTableRaw(1, 1);
        L.GetTableRaw(1, 1);
        CHECK_EQ(std::string_view{"a"}, L.CheckString(2));
        L.Pop(1);
        CHECK_EQ(1, L.GetTop());

        CHECK(!L.GetMetatable(1));
        L.NewTable();
        L.PushValue(2);
        CHECK(L.SetMetatable(1));
        CHECK(L.GetMetatable(1));
        CHECK_EQ(L.ToPointer(2), L.ToPointer(3));
        CHECK(L.RawEqual(2, 3));
        L.SetTop(0);

        L.Push("abc");
        L.SetGlobal("xyz");
        CHECK_EQ(0, L.GetTop());
        L.GetGlobal("xyz");
        CHECK_EQ(1, L.GetTop());
        CHECK_EQ(std::string_view{"abc"}, L.ToString(1));
        L.SetTop(0);

        L.GetSubTable("glob");
        CHECK_EQ(1, L.GetTop());
        CHECK(L.IsTable(1));
        L.GetSubTable("foo", 1);
        CHECK_EQ(2, L.GetTop());
        CHECK(L.IsTable(2));
        L.GetTableRaw(1, "foo");
        CHECK(L.RawEqual(2, 3));
        L.Pop(2);
        L.GetGlobal("glob");
        CHECK(L.RawEqual(1, 2));
        L.SetTop(0);

        L.DoStringT("local t = {5,6,7}; setmetatable(t, {__len=function() return 5; end}); return t;");
        CHECK_EQ(3u, L.RawLength(1));

        if constexpr (S::Capabilities::MetatableLengthModulo)
        {
            L.ObjLength(1);
            CHECK_EQ(S::Capabilities::MetatableLengthOnTables ? 5 : 3, L.CheckInt(2));
        }
        L.SetTop(0);

        L.DoStringT("local t = {}; t.t = {[1]=t}; return t;");
        AssertRegex("\\{\n\tt = \\{\n\t\t\\[1\\] = <table, recursion 0x[0-9a-f]+>,\n\t\\},\n\\}",
                    L.ToDebugString(1, 10));
    }
    TEST_CASE("Tables") { Tables_T<lua::UniqueState>(); }

    template <class S>
    void Operators_T()
    {
        S L{};
        CHECK_EQ(0, L.GetTop());

        L.Push(5);
        L.Push(5);
        L.Push(10);
        CHECK(L.LessThan(1, 3));
        CHECK(L.Compare(1, 3, lua::ComparisonOperator::LessThan));
        CHECK(L.Compare(1, 3, lua::ComparisonOperator::LessThanOrEquals));
        CHECK(L.Equal(1, 2));
        CHECK(L.RawEqual(1, 2));
        CHECK(L.Compare(1, 2, lua::ComparisonOperator::Equals));
        CHECK(L.Compare(1, 2, lua::ComparisonOperator::LessThanOrEquals));

        L.Arithmetic(lua::ArihmeticOperator::Add);
        CHECK_EQ(lua::Integer{15}, L.CheckInteger(2));

        L.Arithmetic(lua::ArihmeticOperator::Subtract);
        CHECK_EQ(lua::Integer{-10}, L.CheckInteger(1));

        L.Push(2);
        L.Arithmetic(lua::ArihmeticOperator::Multiply);
        CHECK_EQ(lua::Integer{-20}, L.CheckInteger(1));

        L.Push(2);
        L.Arithmetic(lua::ArihmeticOperator::Divide);
        CHECK_EQ(lua::Integer{-10}, L.CheckInteger(1));

        L.Arithmetic(lua::ArihmeticOperator::UnaryNegation);
        CHECK_EQ(lua::Integer{10}, L.CheckInteger(1));

        L.Push(4);
        L.Arithmetic(lua::ArihmeticOperator::Modulo);
        CHECK_EQ(lua::Integer{2}, L.CheckInteger(1));

        L.Push(4);
        L.Arithmetic(lua::ArihmeticOperator::Pow);
        CHECK_EQ(lua::Integer{16}, L.CheckInteger(1));
        L.Pop(1);

        if constexpr (S::Capabilities::NativeIntegers)
        {
            L.Push(4);
            L.Push(6);
            L.Arithmetic(S::ArihmeticOperator::BitwiseAnd);
            CHECK_EQ(lua::Integer{4}, L.CheckInteger(1));

            L.Push(2);
            L.Arithmetic(S::ArihmeticOperator::BitwiseOr);
            CHECK_EQ(lua::Integer{6}, L.CheckInteger(1));

            L.Push(2);
            L.Arithmetic(S::ArihmeticOperator::BitwiseXOr);
            CHECK_EQ(lua::Integer{4}, L.CheckInteger(1));

            L.Push(2);
            L.Arithmetic(S::ArihmeticOperator::ShiftLeft);
            CHECK_EQ(lua::Integer{4 << 2}, L.CheckInteger(1));

            L.Push(2);
            L.Arithmetic(S::ArihmeticOperator::ShiftRight);
            CHECK_EQ(lua::Integer{4}, L.CheckInteger(1));

            L.Arithmetic(S::ArihmeticOperator::BitwiseNot);
            CHECK_EQ(~lua::Integer{4}, L.CheckInteger(1));
            L.Pop(1);
        }

        L.Push(16);
        L.Push("abc");
        L.Concat(2);
        CHECK_EQ(std::string_view{"16abc"}, L.CheckString(1));
    }
    TEST_CASE("Operators") { Operators_T<lua::UniqueState>(); }

    template <class S>
    void RunLua_T()
    {
        S L{};
        CHECK_EQ(0, L.GetTop());

        L.DoStringT("function fool(a, b) return a+b end");
        CHECK_EQ(0, L.GetTop());
        L.GetGlobal("fool");
        CHECK(L.IsFunction(1));
        CHECK(!L.IsCFunction(1));
        if (L.Version() == 5.0)
            CHECK_EQ(
                std::string{
                    "<function Lua global fool (defined in: [string \"function fool(a, b) return a+b end\"]:1)>"},
                L.ToDebugString(1));
        else
            CHECK_EQ(
                std::string{"<function Lua  fool (defined in: [string \"function fool(a, b) return a+b end\"]:1)>"},
                L.ToDebugString(1));

        L.Push(5);
        L.Push(42);
        L.Push("abc");
        L.TCall(3, 1);
        CHECK_EQ(lua::Integer{5 + 42}, L.CheckInteger(1));
        L.Pop(1);

        L.RegisterFuncs(funcs::toRegister);
        L.DoStringT("return foo(6, 666)");
        CHECK_EQ(lua::Integer{6 + 42}, L.CheckInteger(1));
        L.SetTop(0);

        CHECK(L.DoString("error('number is 6')") != lua::ErrorCode::Success);
        CHECK(L.CheckStringView(1).find("number is 6") != std::string_view::npos);

        L.SetTop(0);
        bool ok = false;
        try
        {
            L.DoStringT("error('number is 6')");
        }
        catch (const lua::LuaException& e)
        {
            if (std::string_view{e.what()}.find("number is 6") != std::string_view::npos)
                ok = true;
        }
        CHECK(ok);

        L.SetTop(0);
        L.GetGlobal("fool");
        CHECK(L.IsFunction(1));
        CHECK(!L.IsCFunction(1));
        auto f = L.Dump();
        CHECK(!f.empty());
        L.SetTop(0);

        CHECK(L.LoadBuffer(f.c_str(), f.length(), nullptr) == lua::ErrorCode::Success);
        L.Push(7);
        L.Push(42);
        L.Push("abc");
        L.TCall(3, 1);
        CHECK_EQ(lua::Integer{7 + 42}, L.CheckInteger(1));
        L.SetTop(0);

        if constexpr (S::Capabilities::Environments)
        {
            L.DoString("glob = 5; function aglob() return glob; end; return aglob;");
            CHECK(L.IsFunction(1));
            L.PushValue(1);
            L.TCall(0, 1);
            CHECK_EQ(5.0, *L.ToNumber(2));
            L.Pop(1);

            L.GetEnvironment(1);
            L.PushGlobalTable();
            CHECK(L.RawEqual(2, 3));
            L.Pop(2);

            L.NewTable();
            L.Push(42);
            L.SetTableRaw(2, "glob");
            L.SetEnvironment(1);
            L.TCall(0, 1);
            CHECK_EQ(42.0, *L.ToNumber(1));
            L.SetTop(0);
        }
    }
    TEST_CASE("RunLua") { RunLua_T<lua::UniqueState>(); }

    TEST_CASE("UserClass")
    {
        lua::UniqueState L{};
        CHECK_EQ(0, L.GetTop());

        L.NewUserClass<IntHolderOp>(5);
        L.SetGlobal("i");
        L.NewUserClass<IntHolderOp>(7);
        CHECK(L.OptionalUserClass<IntHolderLua>(-1) == nullptr);
        AssertRegex("LuappDev::IntHolderOp: 0x[0-9a-f]+", L.ConvertToString(1));
        AssertRegex("<Userdata (class )?LuappDev::IntHolderOp 0x[0-9a-f]+>", L.ToDebugString(1));
        L.Pop(1);
        L.SetGlobal("j");

        L.DoStringT("assert(i:Get()==5);\n"
                    "j:Set(10);\n"
                    "assert(j:Get()==10);\n"
                    "assert((-i):Get()==-5);\n"
                    "assert((i+j):Get()==15);\n"
                    "assert((i-j):Get()==-5);\n"
                    "assert((i*j):Get()==50);\n"
                    "assert((j/i):Get()==2);\n"
                    "assert(i<=j);\n"
                    "assert(i<j);\n"
                    "assert(i~=j);\n"
                    "assert(j>i);\n"
                    "assert(j>=i);\n"
                    "assert(i==i);\n");
        if constexpr (lua::State::Capabilities::NativeIntegers)
        {
            L.DoStringT("j:Set(4); i:Set(2); local k=i+j; k:Set(6);\n"
                        "assert((i&k):Get()==2);\n"
                        "assert((i|j):Get()==6);\n"
                        "assert((i~k):Get()==4);\n"
                        "assert((~i):Get()==~2);\n"
                        "assert((j<<i):Get()==16);\n"
                        "assert((j>>i):Get()==1);\n");
        }

        L.NewUserClass<IntHolderLua>(5);
        CHECK(L.OptionalUserClass<IntHolderOp>(-1) == nullptr);
        L.SetGlobal("i");
        L.NewUserClass<IntHolderLua>(7);
        CHECK_EQ(std::string_view{"IntHolder 7"}, L.ConvertToString(1));
        L.Pop(1);
        L.SetGlobal("j");

        L.DoStringT("assert(i:Get()==5);\n"
                    "j:Set(10);\n"
                    "assert(j:Get()==10);\n"
                    "assert((-i):Get()==-5);\n"
                    "assert((i+j):Get()==15);\n"
                    "assert((i-j):Get()==-5);\n"
                    "assert((i*j):Get()==50);\n"
                    "assert((j/i):Get()==2);\n"
                    "assert(i<=j);\n"
                    "assert(i<j);\n"
                    "assert(i~=j);\n"
                    "assert(j>i);\n"
                    "assert(j>=i);\n"
                    "assert(i==i);\n");
        L.DoStringT("assert((i^j):Get()==5^10);\n"
                    "assert(i.i==5);\n"
                    "local k=i+j; k.i=3; assert(k:Get()==3);\n"
                    "assert((i..j)=='510');\n"
                    "assert(i(6).i==5*6);\n");
        if constexpr (lua::State::Capabilities::MetatableLengthModulo)
        {
            L.DoStringT("local a = i+j; a.i=4;\n"
                        "assert((j%a).i==2);\n"
                        "assert(#j==2)\n");
        }
        if constexpr (lua::State::Capabilities::NativeIntegers)
        {
            L.DoStringT("assert((j//i):Get()==3);\n"
                        "j:Set(4); i:Set(2); local k=i+j; k:Set(6);\n"
                        "assert((i&k):Get()==2);\n"
                        "assert((i|j):Get()==6);\n"
                        "assert((i~k):Get()==4);\n"
                        "assert((~i):Get()==~2);\n"
                        "assert((j<<i):Get()==16);\n"
                        "assert((j>>i):Get()==1);\n");
        }

        L.NewUserClass<InheritanceTestD>(5, 10.0);
        L.CheckUserClass<InheritanceTestB>(-1);
        L.CheckUserClass<InheritanceTestD>(-1);
        CHECK(L.OptionalUserClass<InheritanceTestF>(-1) == nullptr);
        CHECK(L.OptionalUserClass<IntHolderLua>(-1) == nullptr);
        L.SetGlobal("inhd");

        L.NewUserClass<InheritanceTestB>(5);
        L.CheckUserClass<InheritanceTestB>(-1);
        CHECK(L.OptionalUserClass<InheritanceTestF>(-1) == nullptr);
        CHECK(L.OptionalUserClass<InheritanceTestD>(-1) == nullptr);
        CHECK(L.OptionalUserClass<IntHolderLua>(-1) == nullptr);
        L.SetGlobal("inhb");

        L.DoStringT("assert(inhd:GetI()==5);\n"
                    "assert(inhd:GetJ()==10);\n"
                    "assert(inhb:GetI()==5);\n"
                    "assert(inhb.GetJ == nil);\n");

        if constexpr (lua::State::Capabilities::Uservalues)
        {
            L.NewUserClass<UservalueTest<lua::State>>();
            L.SetGlobal("uvt");
            L.DoStringT("if uvt:n()==3 then\n"
                        "uvt:Set(1,2,3)\n"
                        "local a,b,c = uvt:Get()\n"
                        "assert(a==3)\n"
                        "assert(b==2)\n"
                        "assert(c==1)\n"
                        "elseif uvt:n()==1 then\n"
                        "uvt:Set({1})\n"
                        "assert(uvt:Get()[1]==1)\n"
                        "end");
        }

        bool closed = false;
        {
            lua::UniqueState L2{};
            L2.NewUserClass<DtorTest>([&closed]() { closed = true; });
            // closing the state forces everything to get gcd
        }
        CHECK(closed);
        closed = false;
        {
            lua::UniqueState L2{};
            L2.NewUserClass<InheritanceTestF>([&closed]() { closed = true; });
            L2.CheckUserClass<InheritanceTestF>(-1);
            L2.CheckUserClass<InheritanceTestB>(-1);
            CHECK(L2.OptionalUserClass<InheritanceTestD>(-1) == nullptr);
            CHECK(L2.OptionalUserClass<IntHolderLua>(-1) == nullptr);
            // closing the state forces everything to get gcd
        }
        CHECK(closed);
    }

    TEST_CASE("Iterate")
    {
        lua::UniqueState L{};
        CHECK_EQ(0, L.GetTop());

        L.DoStringT("return {[1]=5,[2]=6,[4]=8}");
        CHECK_EQ(1, L.GetTop());
        int i = 0;
        for (auto k : L.Pairs(1))
        {
            CHECK(k == lua::LType::Number);
            CHECK_EQ(L.CheckInt(-2) + 4, L.CheckInt(-1));
            ++i;
        }
        CHECK_EQ(3, i);
        CHECK_EQ(1, L.GetTop());
        i = 0;
        for (auto k : L.IPairs(1))
        {
            CHECK_EQ(lua::Integer{k + 4}, L.CheckInteger(-1));
            ++i;
        }
        CHECK_EQ(2, i);
        CHECK_EQ(1, L.GetTop());
    }

    static int black_magic(lua::State L)
    {
        lua::DebugInfo i{};
        CHECK_EQ(3, L.Debug_GetStackDepth()); // black_magic, foo, main chunk
        CHECK(L.Debug_GetStack(1, i, lua::DebugInfoOptions::Name, false));
        CHECK_EQ(std::string_view{"foo"}, i.Name);
        CHECK_EQ(std::string_view{"foo"}, std::string_view{L.Debug_GetNameForStackFunc(1)});
        int num = 0;
        for (const auto& l : L.Debug_Locals(1))
        {
            if (l.Name == std::string_view{"l"})
            {
                CHECK_EQ(lua::Integer{1}, L.CheckInteger(-1));
                L.Push(42);
                CHECK_EQ(std::string_view{"l"}, L.Debug_SetLocal(1, l.LocalNum));
                ++num;
            }
            else if (l.Name == std::string{"x"})
            {
                CHECK_EQ(lua::Integer{5}, L.CheckInteger(-1));
                ++num;
            }
            else if (l.Name == std::string{"y"})
            {
                CHECK_EQ(lua::Integer{6}, L.CheckInteger(-1));
                ++num;
            }
            L.Pop(1);
        }
        CHECK_EQ(3, num);
        L.Debug_PushDebugInfoFunc(i);
        for (const auto& u : L.Debug_Upvalues(-1))
        {
            if (u.Name == std::string_view{"upv"})
            {
                CHECK_EQ(lua::Integer{0}, L.CheckInteger(-1));
                L.Push(4242);
                CHECK_EQ(std::string_view{"upv"}, L.Debug_SetUpvalue(-3, u.UpvalNum));
                ++num;
            }
            else if (u.Name == std::string{"u2"})
            {
                CHECK_EQ(lua::Integer{4}, L.CheckInteger(-1));
                ++num;
            }
            else if (u.Name == std::string{"u3"})
            {
                CHECK_EQ(lua::Integer{5}, L.CheckInteger(-1));
                ++num;
            }
            L.Pop(1);
        }
        CHECK_EQ(6, num);
        return 0;
    }
    TEST_CASE("LocalAccess")
    {
        lua::UniqueState L{};
        CHECK_EQ(0, L.GetTop());
        CHECK_EQ(0, L.Debug_GetStackDepth());

        L.Push("black_magic");
        L.Push<black_magic>();
        L.SetGlobal();

        L.DoStringT("local upv = 0; local u2=4; local u3=5; function foo() local y = 6; local l = 1; local x = 5; "
                    "black_magic(); u3,u2=u2,u3; return l,upv; end; local a,b = foo(); return a,b;");
        CHECK_EQ(2, L.GetTop());
        CHECK_EQ(lua::Integer{42}, L.CheckInteger(1));
        CHECK_EQ(lua::Integer{4242}, L.CheckInteger(2));
    }

    static void hook_magic(lua::State L, lua::ActivationRecord ar)
    {
        if (ar.Matches(lua::HookEvent::Return))
        {
            auto inf = L.Debug_GetInfoFromAR(ar, lua::DebugInfoOptions::Name);
            if (inf.Name && inf.Name == std::string_view{"bar"})
            {
                L.Debug_GetStack(1, inf, lua::DebugInfoOptions::Name, false);
                CHECK_EQ(std::string_view{"foo"}, inf.Name);
                for (const auto& l : L.Debug_Locals(inf))
                {
                    if (l.Name == std::string_view{"l"})
                    {
                        CHECK_EQ(lua::Integer{1}, L.CheckInteger(-1));
                        L.Push(42);
                        CHECK_EQ(std::string_view{"l"}, L.Debug_SetLocal(1, l.LocalNum));
                    }
                    L.Pop(1);
                }
                L.Debug_PushDebugInfoFunc(inf);
                for (const auto& u : L.Debug_Upvalues(-1))
                {
                    if (u.Name == std::string_view{"upv"})
                    {
                        CHECK_EQ(lua::Integer{0}, L.CheckInteger(-1));
                        L.Push(4242);
                        CHECK_EQ(std::string_view{"upv"}, L.Debug_SetUpvalue(-3, u.UpvalNum));
                    }
                    L.Pop(1);
                }
                L.Pop(1);
            }
        }
    }

    template <class S>
    void Hook_T()
    {
        S L{};
        CHECK_EQ(0, L.GetTop());

        if constexpr (S::Capabilities::JIT)
            L.SetJITMode(S::JITMode::On);

        L.template Debug_SetHook<hook_magic>(lua::HookEvent::Call | lua::HookEvent::Return, 0);

        L.DoStringT("local upv = 0; function bar() end; function foo() local l = 1; bar(); return l,upv; end; local "
                    "a,b = foo(); return a,b;");
        CHECK_EQ(2, L.GetTop());
        CHECK_EQ(lua::Integer{42}, L.CheckInteger(1));
        CHECK_EQ(lua::Integer{4242}, L.CheckInteger(2));
    }
    TEST_CASE("Hook") { Hook_T<lua::UniqueState>(); }

    static void hook_checkline(lua::State L, lua::ActivationRecord ar)
    {
        int t = L.GetTop();
        if (ar.Matches(lua::HookEvent::Line) && L.Debug_GetNameForStackFunc(0) == "test")
        {
            L.PushLightUserdata(reinterpret_cast<void*>(&hook_checkline));
            L.GetTableRaw(lua::State::REGISTRYINDEX);
            auto* exp = static_cast<int*>(L.ToUserdata(-1));
            CHECK_EQ(*exp, ar.Line());
            ++*exp;
        }
        L.SetTop(t);
    }
    static int activate_hookline(lua::State L)
    {
        L.Debug_SetHook<hook_checkline>(lua::HookEvent::Line | lua::HookEvent::Call | lua::HookEvent::Return, 0);
        return 0;
    }
    TEST_CASE("HookLine")
    {
        lua::UniqueState L{};
        CHECK_EQ(0, L.GetTop());

        int exp = lua::State::Version() == 5.0 ? 8 : 7;

        L.PushLightUserdata(reinterpret_cast<void*>(&hook_checkline));
        L.PushLightUserdata(&exp);
        L.SetTableRaw(lua::State::REGISTRYINDEX);
        L.RegisterFunc<activate_hookline>("activate_hook");
        L.Debug_SetHook<hook_checkline>(lua::HookEvent::Count, 5000);
        L.DoStringT("function foo() end\n"
                    "\n"
                    "function test()\n"
                    "foo()\n"
                    "activate_hook()\n"
                    "\n"
                    "foo()\n" // 7
                    "foo()\n"
                    "foo()\n"
                    "foo()\n"
                    "foo()\n"
                    "end\n"
                    "test()\n"); // 13
        CHECK_EQ(13, exp);
    }

    template<class S>
    struct StateExt
    {
        void Push(std::pair<double, double> f)
        {
            auto* t = static_cast<S*>(this);
            t->NewTable();
            t->Push("X");
            t->Push(f.first);
            t->SetTableRaw(-3);
            t->Push("Y");
            t->Push(f.second);
            t->SetTableRaw(-3);
        }
        std::pair<double, double> CheckVec(int i)
        {
            auto* t = static_cast<S*>(this);
            i = t->ToAbsoluteIndex(i);
            t->CheckType(i, lua::LType::Table);
            t->GetTableRaw(i, "X");
            t->GetTableRaw(i, "Y");
            std::pair<double, double> r{t->CheckNumber(-2), t->CheckNumber(-1)};
            t->Pop(2);
            return r;
        }
    };
    int ExtFunc(lua::State::BindExtensions<StateExt> L)
    {
        auto v = L.CheckVec(1);
        L.Push(v.first + v.second);
        return 1;
    }
    TEST_CASE("Extension")
    {
        lua::UniqueState::BindExtensions<StateExt> L{};
        CHECK_EQ(0, L.GetTop());
        L.Push<ExtFunc>();

        L.Push(std::pair{4.2, 42.0});
        CHECK(L.GetTop() == 2);
        L.GetTableRaw(2, "X");
        CHECK(L.CheckNumber(3) == 4.2);
        L.Pop(1);
        L.GetTableRaw(2, "Y");
        CHECK(L.CheckNumber(3) == 42.0);
        L.Pop(1);

        CHECK(L.CheckVec(-1) == std::pair{4.2, 42.0});
        L.TCall(1, 1);
        CHECK(L.GetTop() == 1);
        CHECK(L.CheckNumber(1) == 4.2 + 42.0);
    }
} // namespace LuappDev
