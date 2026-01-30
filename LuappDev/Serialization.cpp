#include <doctest/doctest.h>

#include <luaver.h>
#include <luapp/luapp_serialization.h>

namespace LuappDev
{
    template<class S>
    struct SeriUC
    {
        int i = 0;

        static constexpr bool UserClassMetaMethods = true;

        static int Serialize(S L)
        {
            auto* th = L.template CheckUserClass<SeriUC>(1);
            L.Push("SeriUC");
            L.Push(th->i);
            return 2;
        }
        static int Deserialize(S L)
        {
            L.template NewUserClass<SeriUC>(L.template Check<int>(1));
            return 1;
        }
    };
    static_assert(lua::userdata::SerializeCpp<lua::State, SeriUC<lua::State>>);

    TEST_CASE_TEMPLATE("Serialization", US, lua::UniqueState, ExtUniqueState)
    {
        using S = US::Base;
        SUBCASE("table") {
            std::stringstream val{};

            {
                US L{};

                L.DoStringT("return {5, 'abc', function() return 7 end}");

                lua::serialization::LuaSerializer vs{lua::serialization::StreamIO{val}, S{L}};
                vs.SerializeVariable(-1);
            }
            val.seekp(std::ios_base::beg);
            {
                US L{};

                lua::serialization::LuaSerializer vs{lua::serialization::StreamIO{val}, S{L}};
                vs.DeserializeVariable();
                L.SetGlobal("i");

                L.DoStringT("assert(i[1] == 5)\n"
                    "assert(i[2] == 'abc')\n"
                    "assert(i[3]() == 7)\n");
            }
        }

        if constexpr (S::Capabilities::NativeIntegers)
        {
            SUBCASE("integer")
            {
                US L{};
                std::stringstream str{};
                lua::serialization::LuaSerializer vs{lua::serialization::StreamIO{str}, S{L}};

                L.Push(5);
                CHECK(L.IsInteger(-1));
                vs.SerializeVariable(-1);

                L.SetTop(0);
                str.seekp(std::ios_base::beg);

                vs.DeserializeVariable();
                CHECK(L.IsInteger(-1));
                CHECK(L.template Check<int>(-1) == 5);
            }
        }

        SUBCASE("userclass")
        {
            US L{};
            std::stringstream str{};
            lua::serialization::LuaSerializer vs{lua::serialization::StreamIO{str}, S{L}};

            using UC = SeriUC<S>;
            L.template NewUserClass<UC>(500);
            vs.SerializeVariable(-1);

            str.seekp(std::ios_base::beg);

            vs.GetUserdataDeserializer = [](std::string_view s) -> lua::CFunction
            {
                if (s == "SeriUC")
                    return &S::template CppToCFunction<UC::Deserialize>;
                return nullptr;
            };

            vs.DeserializeVariable();
            CHECK(!L.RawEqual(1, 2));
            CHECK(L.template CheckUserClass<UC>(1)->i == L.template CheckUserClass<UC>(2)->i);
        }

        SUBCASE("stack")
        {
            US L{};
            std::stringstream str{};
            lua::serialization::LuaSerializer vs{lua::serialization::StreamIO{str}, S{L}};

            L.Push(5);
            L.Push("foo");
            L.NewTable();
            L.PushValue(3);
            vs.SerializeStack();

            L.SetTop(0);
            str.seekp(std::ios_base::beg);

            vs.DeserializeStack();
            CHECK(L.template Check<int>(1) == 5);
            CHECK(L.template Check<std::string_view>(2) == "foo");
            CHECK(L.IsTable(3));
            CHECK(L.RawEqual(3, 4));
        }

        SUBCASE("state")
        {
            {
                std::stringstream val{};
                {
                    US L{};

                    L.DoStringT("function foo() return 42 end\n"
                        "x = {}\n"
                        "x.y = x\n"
                        "x[foo] = 5\n"
                        "x[5] = foo\n"
                        "x[-5.5] = foo\n"
                        "x[x] = foo\n"
                        "y = x\n");

                    L.PushSerializedRegistry();
                    L.Push("bar");
                    L.GetGlobal("x");
                    L.SetTableRaw(-3);
                    L.SetTop(0);


                    lua::serialization::LuaSerializer vs{lua::serialization::StreamIO{val}, S{L}};
                    vs.IsGlobalSkipped = [](std::string_view v) { return v == "y"; };
                    vs.SerializeState();
                }
                val.seekp(std::ios_base::beg);
                {
                    US L{};

                    lua::serialization::LuaSerializer vs{lua::serialization::StreamIO{val}, S{L}};
                    vs.DeserializeState();

                    L.DoStringT("assert(foo() == 42)\n"
                        "assert(x == x.y)\n"
                        "assert(x[foo] == 5)\n"
                        "assert(x[5] == foo)\n"
                        "assert(x[-5.5] == foo)\n"
                        "assert(x[x] == foo)\n"
                        "assert(y == nil)\n");

                    L.PushSerializedRegistry();
                    L.Push("bar");
                    L.GetTableRaw(-2);
                    L.GetGlobal("x");
                    CHECK(L.RawEqual(-1, -2));
                }
            }
        }

        if constexpr (S::Capabilities::UpvalueId)
        {
            SUBCASE("upvalue")
            {
                {
                    std::stringstream val{};
                    {
                        US L{};

                        L.DoStringT("local x = 0\n"
                            "function foo()\n"
                            "return x\n"
                            "end\n"
                            "function inc()\n"
                            "x = x + 1\n"
                            "end\n"
                            "assert(foo() == 0)\n"
                            "inc()\n"
                            "assert(foo() == 1)\n");

                        lua::serialization::LuaSerializer vs{lua::serialization::StreamIO{val}, S{L}};
                        vs.SerializeState();
                    }
                    val.seekp(std::ios_base::beg);
                    {
                        US L{};

                        lua::serialization::LuaSerializer vs{lua::serialization::StreamIO{val}, S{L}};
                        vs.DeserializeState();

                        L.DoStringT("assert(foo() == 1)\n"
                            "inc()\n"
                            "assert(foo() == 2)\n");
                    }
                }
            }
        }
    }
}