#include <doctest/doctest.h>

#include <luaver.h>

namespace LuappDev
{
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
        template<class S>
        static int run_foo(S L)
        {
            L.PushValue(1);
            L.Push(42);
            L.Arithmetic(lua::ArihmeticOperator::Add);
            return 1;
        }
        template<class S>
        static int foo_up(S L)
        {
            CHECK_EQ(1, L.Debug_GetStackDepth());
            L.PushValue(1);
            L.PushValue(S::Upvalueindex(1));
            L.Arithmetic(lua::ArihmeticOperator::Add);
            return 1;
        }
        template<class S>
        static int ex(S L)
        {
            auto i = L.CheckInt(1);
            throw lua::LuaException{std::format("number is {}", i)};
        }
        static double run_api(double a, double b)
        {
            return a + b;
        }
        static double api2()
        {
            return 42.0;
        }
        static void api3(double a)
        {
            CHECK(a == 5.0);
        }
        static std::tuple<double, std::string_view> api4(int i)
        {
            return {static_cast<double>(i) + 0.5, "foo"};
        }
    } // namespace funcs

    TEST_CASE_TEMPLATE("Functions", US, lua::UniqueState, ExtUniqueState)
    {
        using S = US::Base;

        US L{};
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

        L.template Push<funcs::run_foo<S>>();
        CHECK(L.IsFunction(1));
        CHECK(L.IsCFunction(1));
        CHECK(L.Type(1) == lua::LType::Function);

        L.Push(5);
        CHECK_EQ(1, L.TCall(1, 1));
        CHECK_EQ(1, L.GetTop());
        CHECK_EQ(lua::Integer{42 + 5}, L.CheckInteger(1));
        L.SetTop(0);

        L.Push(44);
        L.template Push<funcs::foo_up<S>>(1);
        L.Push(5);
        CHECK_EQ(1, L.TCall(1, L.MULTIRET));
        CHECK_EQ(1, L.GetTop());
        CHECK_EQ(lua::Integer{44 + 5}, L.CheckInteger(1));
        L.SetTop(0);

        L.template Push<funcs::run_foo<S>>();

        L.Push(5);
        L.Push("bar");
        CHECK_EQ(2, L.TCall(2, 2));
        CHECK_EQ(lua::Integer{42 + 5}, L.CheckInteger(1));
        CHECK(L.IsNil(2));
        L.SetTop(0);

        L.template Push<S::template AutoTranslateAPI<funcs::run_api>>();
        L.Push(5.0);
        L.Push(10.05);
        L.TCall(2, 1);
        CHECK(L.template Check<double>(1) == 15.05);
        L.SetTop(0);

        L.template Push<S::template AutoTranslateAPI<funcs::api2>>();
        L.TCall(0, 1);
        CHECK(L.template Check<double>(1) == 42.0);
        L.SetTop(0);

        L.template Push<S::template AutoTranslateAPI<funcs::api3>>();
        L.Push(5.0);
        L.TCall(1, 0);
        L.SetTop(0);

        L.template Push<funcs::api4>();
        L.Push(5);
        L.TCall(1, 2);
        CHECK(L.template Check<double>(1) == 5.5);
        CHECK(L.template Check<std::string_view>(2) == "foo");
        L.SetTop(0);

        L.template Push<funcs::ex<S>>();
        L.Push(5);
        CHECK(L.PCall(1, 0) == lua::ErrorCode::Runtime);
        CHECK(L.CheckStringView(1).find("number is 5") != std::string_view::npos);
        L.SetTop(0);
        bool ok = false;
        try
        {
            L.template Push<funcs::ex<S>>();
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

            [[nodiscard]] int P(S L) // NOLINT(*-make-member-function-const)
            {
                L.Push(i + L.CheckInt(1) + L.CheckInt(S::Upvalueindex(2)));
                return 1;
            }
            [[nodiscard]] int PC(S L) const
            {
                L.Push(i + 1 + L.CheckInt(1) + L.CheckInt(S::Upvalueindex(2)));
                return 1;
            }
            [[nodiscard]] int G(S L) // NOLINT(*-make-member-function-const)
            {
                L.Push(i);
                return 1;
            }
            [[nodiscard]] int GC(S L) const
            {
                L.Push(i + 1);
                return 1;
            }

            [[nodiscard]] double API(double a) // NOLINT(*-make-member-function-const)
            {
                return static_cast<double>(i) + a;
            }
            [[nodiscard]] double APIC(double a) const
            {
                return static_cast<double>(i) + a + 1.0;
            }
            [[nodiscard]] std::tuple<double, int, std::string_view> PARAMS(double a, std::string_view s, int x) const
            {
                return {static_cast<double>(i) + a, x + i, s};
            }
        };
        A a{5};
        L.Push(3);
        L.template Push<A, &A::P>(a, 1);
        L.Push(4);
        L.TCall(1, 1);
        CHECK_EQ(12, L.CheckInt(1));
        L.SetTop(0);

        L.Push(3);
        L.template Push<A, &A::PC>(a, 1);
        L.Push(4);
        L.TCall(1, 1);
        CHECK_EQ(13, L.CheckInt(1));
        L.SetTop(0);

        L.template Push<&A::API>(a);
        L.Push(5);
        L.TCall(1, 1);
        CHECK_EQ(10, L.CheckInt(1));
        L.SetTop(0);

        L.template Push<&A::APIC>(a);
        L.Push(5);
        L.TCall(1, 1);
        CHECK_EQ(11, L.CheckInt(1));
        L.SetTop(0);

        std::array toregobj{
            S::FuncReference::template GetRef<A, &A::G>(a, "G"),
            S::FuncReference::template GetRef<A, &A::GC>(a, "GC"),
            S::FuncReference::template GetRef<&A::API>(a, "API"),
            S::FuncReference::template GetRef<&A::APIC>(a, "APIC"),
            S::FuncReference::template GetRef<&A::PARAMS>(a, "PARAMS"),
        };
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
        L.GetTableRaw(1, "API");
        L.Push(1);
        L.TCall(1, 1);
        CHECK(L.CheckNumber(-1) == 6.0);
        L.SetTop(1);
        L.GetTableRaw(1, "APIC");
        L.Push(1);
        L.TCall(1, 1);
        CHECK(L.CheckNumber(-1) == 7.0);
        L.SetTop(1);
        L.GetTableRaw(1, "APIC");
        {
            auto [r] = L.template TCall<double>(5.0);
            CHECK(r == 11.0);
        }
        CHECK(L.GetTop() == 2);
        L.SetTop(1);
        L.GetTableRaw(1, "PARAMS");
        {
            auto [r, i, s] = L.template TCall<double, int, std::string_view>(5.0, "foo", 1);
            CHECK(r == 10.0);
            CHECK(i == 6);
            CHECK(s == "foo");
        }
        CHECK(L.GetTop() == 4);
        L.SetTop(1);
    }
}