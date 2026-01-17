#include <doctest/doctest.h>

#include <luaver.h>
#include <DtorTest.h>

namespace LuappDev
{

    namespace cls
    {
        template<class S>
        class IntHolderOp
        {
            lua::Integer i;

            static int get(S L)
            {
                auto t = L.template CheckUserClass<IntHolderOp>(1);
                L.Push(t->i);
                return 1;
            }
            static int set(S L)
            {
                auto t = L.template CheckUserClass<IntHolderOp>(1);
                t->i = L.CheckInt(2);
                return 0;
            }

        public:
            explicit IntHolderOp(lua::Integer i) : i(i) {}

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

            static constexpr std::array LuaMethods{
                S::FuncReference::template GetRef<get>("Get"),
                S::FuncReference::template GetRef<set>("Set"),
            };
        };

        template<class S>
        class IntHolderLua
        {
            lua::Integer i;

            static int get(S L)
            {
                auto t = L.template CheckUserClass<IntHolderLua>(1);
                L.Push(t->i);
                return 1;
            }
            static int set(S L)
            {
                auto t = L.template CheckUserClass<IntHolderLua>(1);
                t->i = L.CheckInt(2);
                return 0;
            }

        public:
            explicit IntHolderLua(lua::Integer i) : i(i) {}

            static constexpr std::array LuaMethods{
                S::FuncReference::template GetRef<get>("Get"),
                S::FuncReference::template GetRef<set>("Set"),
            };

            static int Equals(S L)
            {
                auto t = L.template CheckUserClass<IntHolderLua>(1);
                auto o = L.template CheckUserClass<IntHolderLua>(2);
                L.Push(t->i == o->i);
                return 1;
            }
            static int LessThan(S L)
            {
                auto t = L.template CheckUserClass<IntHolderLua>(1);
                auto o = L.template CheckUserClass<IntHolderLua>(2);
                L.Push(t->i < o->i);
                return 1;
            }
            static int LessOrEquals(S L)
            {
                auto t = L.template CheckUserClass<IntHolderLua>(1);
                auto o = L.template CheckUserClass<IntHolderLua>(2);
                L.Push(t->i <= o->i);
                return 1;
            }
            static int Add(S L)
            {
                auto t = L.template CheckUserClass<IntHolderLua>(1);
                auto o = L.template CheckUserClass<IntHolderLua>(2);
                L.template NewUserClass<IntHolderLua>(t->i + o->i);
                return 1;
            }
            static int Subtract(S L)
            {
                auto t = L.template CheckUserClass<IntHolderLua>(1);
                auto o = L.template CheckUserClass<IntHolderLua>(2);
                L.template NewUserClass<IntHolderLua>(t->i - o->i);
                return 1;
            }
            static int Multiply(S L)
            {
                auto t = L.template CheckUserClass<IntHolderLua>(1);
                auto o = L.template CheckUserClass<IntHolderLua>(2);
                L.template NewUserClass<IntHolderLua>(t->i * o->i);
                return 1;
            }
            static int Divide(S L)
            {
                auto t = L.template CheckUserClass<IntHolderLua>(1);
                auto o = L.template CheckUserClass<IntHolderLua>(2);
                L.template NewUserClass<IntHolderLua>(t->i / o->i);
                return 1;
            }
            static int Pow(S L)
            {
                auto t = L.template CheckUserClass<IntHolderLua>(1);
                auto o = L.template CheckUserClass<IntHolderLua>(2);
                L.template NewUserClass<IntHolderLua>(static_cast<lua::Integer>(std::pow(t->i, o->i)));
                return 1;
            }
            static int UnaryMinus(S L)
            {
                auto t = L.template CheckUserClass<IntHolderLua>(1);
                L.template NewUserClass<IntHolderLua>(-t->i);
                return 1;
            }
            static int Concat(S L)
            {
                auto t = L.template CheckUserClass<IntHolderLua>(1);
                auto o = L.template CheckUserClass<IntHolderLua>(2);
                L.Push(t->i);
                L.Push(o->i);
                L.Concat(2);
                return 1;
            }
            static int NewIndex(S L)
            {
                auto t = L.template CheckUserClass<IntHolderLua>(1);
                if (L.CheckStringView(2) != "i")
                    throw lua::LuaException{"invalid key"};
                t->i = L.CheckInt(3);
                return 0;
            }
            static int Index(S L)
            {
                auto t = L.template CheckUserClass<IntHolderLua>(1);
                if (L.CheckStringView(2) != "i")
                    throw lua::LuaException{"invalid key"};
                L.Push(t->i);
                return 1;
            }
            static int Call(S L)
            {
                auto t = L.template CheckUserClass<IntHolderLua>(1);
                auto o = L.CheckInt(2);
                L.template NewUserClass<IntHolderLua>(t->i * o);
                return 1;
            }
            static int Modulo(S L)
            {
                auto t = L.template CheckUserClass<IntHolderLua>(1);
                auto o = L.template CheckUserClass<IntHolderLua>(2);
                L.template NewUserClass<IntHolderLua>(t->i % o->i);
                return 1;
            }
            static int Length(S L)
            {
                auto t = L.template CheckUserClass<IntHolderLua>(1);
                L.Push(t->i);
                L.ToString(-1);
                auto l = L.RawLength(-1);
                L.Push(static_cast<lua::Integer>(l));
                return 1;
            }
            static int IntegerDivide(S L)
            {
                auto t = L.template CheckUserClass<IntHolderLua>(1);
                auto o = L.template CheckUserClass<IntHolderLua>(2);
                L.template NewUserClass<IntHolderLua>(t->i / o->i + 1);
                return 1;
            }
            static int BitwiseAnd(S L)
            {
                auto t = L.template CheckUserClass<IntHolderLua>(1);
                auto o = L.template CheckUserClass<IntHolderLua>(2);
                L.template NewUserClass<IntHolderLua>(t->i & o->i);
                return 1;
            }
            static int BitwiseOr(S L)
            {
                auto t = L.template CheckUserClass<IntHolderLua>(1);
                auto o = L.template CheckUserClass<IntHolderLua>(2);
                L.template NewUserClass<IntHolderLua>(t->i | o->i);
                return 1;
            }
            static int BitwiseXOr(S L)
            {
                auto t = L.template CheckUserClass<IntHolderLua>(1);
                auto o = L.template CheckUserClass<IntHolderLua>(2);
                L.template NewUserClass<IntHolderLua>(t->i ^ o->i);
                return 1;
            }
            static int BitwiseNot(S L)
            {
                auto t = L.template CheckUserClass<IntHolderLua>(1);
                L.template NewUserClass<IntHolderLua>(~t->i);
                return 1;
            }
            static int ShiftLeft(S L)
            {
                auto t = L.template CheckUserClass<IntHolderLua>(1);
                auto o = L.template CheckUserClass<IntHolderLua>(2);
                L.template NewUserClass<IntHolderLua>(t->i << o->i);
                return 1;
            }
            static int ShiftRight(S L)
            {
                auto t = L.template CheckUserClass<IntHolderLua>(1);
                auto o = L.template CheckUserClass<IntHolderLua>(2);
                L.template NewUserClass<IntHolderLua>(t->i >> o->i);
                return 1;
            }
            static int ToString(S L)
            {
                auto t = L.template CheckUserClass<IntHolderLua>(1);
                L.Push(std::format("IntHolder {}", t->i));
                return 1;
            }
        };

        template<class S>
        class InheritanceTestB
        {
        protected:
            lua::Integer i;

            static int geti(S L)
            {
                auto t = L.template CheckUserClass<InheritanceTestB>(1);
                L.Push(t->i);
                return 1;
            }
            static int seti(S L)
            {
                auto t = L.template CheckUserClass<InheritanceTestB>(1);
                t->i = L.CheckInt(2);
                return 0;
            }

        public:
            using BaseClass = InheritanceTestB;

            explicit InheritanceTestB(lua::Integer i) : i(i) {}
            virtual ~InheritanceTestB() = default;

            static constexpr std::array LuaMethods{
                S::FuncReference::template GetRef<geti>("GetI"),
                S::FuncReference::template GetRef<seti>("SetI"),
            };

            InheritanceTestB operator+(const InheritanceTestB& o) const { return InheritanceTestB{i + o.i}; }
            InheritanceTestB operator-() const { return InheritanceTestB{-i}; }
        };
        template<class S>
        class InheritanceTestD : public InheritanceTestB<S>
        {
            lua::Number j;
            static int getj(S L)
            {
                auto t = L.template CheckUserClass<InheritanceTestD>(1);
                L.Push(t->j);
                return 1;
            }
            static int setj(S L)
            {
                auto t = L.template CheckUserClass<InheritanceTestD>(1);
                t->j = L.CheckNumber(2);
                return 0;
            }

        public:
            InheritanceTestD(lua::Integer i, lua::Number j) : InheritanceTestB<S>(i), j(j) {}

            static constexpr std::array LuaMethods{
                S::FuncReference::template GetRef<InheritanceTestB<S>::geti>("GetI"),
                S::FuncReference::template GetRef<InheritanceTestB<S>::seti>("SetI"),
                S::FuncReference::template GetRef<getj>("GetJ"),
                S::FuncReference::template GetRef<setj>("SetJ"),
            };

            InheritanceTestD operator+(const InheritanceTestD& o) const { return {InheritanceTestB<S>::i + o.i, j + o.j}; }
        };
        template<class S>
        class InheritanceTestF : public InheritanceTestB<S>
        {
            std::function<void()> f;

        public:
            explicit InheritanceTestF(std::function<void()> f) : InheritanceTestB<S>(42), f(std::move(f)) {}

            ~InheritanceTestF() override { f(); }
        };

        template<class S>
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
                else if constexpr (S::Capabilities::Uservalues)
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
                else if constexpr (S::Capabilities::Uservalues)
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
                S::Capabilities::Uservalues ? (S::Capabilities::ArbitraryUservalues ? 3 : 1) : 0;

            static constexpr std::array LuaMethods{
                S::FuncReference::template GetRef<get>("Get"),
                S::FuncReference::template GetRef<set>("Set"),
                S::FuncReference::template GetRef<n>("n"),
            };
        };
    }


    TEST_CASE_TEMPLATE("UserClass", US, lua::UniqueState, ExtUniqueState)
    {
        using S = US::Base;

        using IntHolderOp = cls::IntHolderOp<S>;
        using IntHolderLua = cls::IntHolderLua<S>;
        using InheritanceTestB = cls::InheritanceTestB<S>;
        using InheritanceTestD = cls::InheritanceTestD<S>;
        using InheritanceTestF = cls::InheritanceTestF<S>;

        US L{};
        CHECK_EQ(0, L.GetTop());

        L.template NewUserClass<IntHolderOp>(5);
        L.SetGlobal("i");
        L.template NewUserClass<IntHolderOp>(7);
        CHECK(L.template OptionalUserClass<IntHolderLua>(-1) == nullptr);
        AssertRegex("LuappDev::cls::IntHolderOp<lua::decorator::State<[:, [:alnum:]]+>>: 0x[0-9a-f]+", L.ConvertToString(1));
        AssertRegex("<Userdata (class )?LuappDev::cls::IntHolderOp<lua::decorator::State<[:, [:alnum:]]+>> 0x[0-9a-f]+>", L.ToDebugString(1));
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
        if constexpr (S::Capabilities::NativeIntegers)
        {
            L.DoStringT("j:Set(4); i:Set(2); local k=i+j; k:Set(6);\n"
                        "assert((i&k):Get()==2);\n"
                        "assert((i|j):Get()==6);\n"
                        "assert((i~k):Get()==4);\n"
                        "assert((~i):Get()==~2);\n"
                        "assert((j<<i):Get()==16);\n"
                        "assert((j>>i):Get()==1);\n");
        }

        L.template NewUserClass<IntHolderLua>(5);
        CHECK(L.template OptionalUserClass<IntHolderOp>(-1) == nullptr);
        L.SetGlobal("i");
        L.template NewUserClass<IntHolderLua>(7);
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
        if constexpr (S::Capabilities::MetatableLengthModulo)
        {
            L.DoStringT("local a = i+j; a.i=4;\n"
                        "assert((j%a).i==2);\n"
                        "assert(#j==2)\n");
        }
        if constexpr (S::Capabilities::NativeIntegers)
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

        L.template NewUserClass<InheritanceTestD>(5, 10.0);
        L.template CheckUserClass<InheritanceTestB>(-1);
        L.template CheckUserClass<InheritanceTestD>(-1);
        CHECK(L.template OptionalUserClass<InheritanceTestF>(-1) == nullptr);
        CHECK(L.template OptionalUserClass<IntHolderLua>(-1) == nullptr);
        L.SetGlobal("inhd");

        L.template NewUserClass<InheritanceTestB>(5);
        L.template CheckUserClass<InheritanceTestB>(-1);
        CHECK(L.template OptionalUserClass<InheritanceTestF>(-1) == nullptr);
        CHECK(L.template OptionalUserClass<InheritanceTestD>(-1) == nullptr);
        CHECK(L.template OptionalUserClass<IntHolderLua>(-1) == nullptr);
        L.SetGlobal("inhb");

        L.DoStringT("assert(inhd:GetI()==5);\n"
                    "assert(inhd:GetJ()==10);\n"
                    "assert(inhb:GetI()==5);\n"
                    "assert(inhb.GetJ == nil);\n");

        if constexpr (S::Capabilities::Uservalues)
        {
            L.template NewUserClass<cls::UservalueTest<S>>();
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
            US L2{};
            L2.template NewUserClass<cls::DtorTest>([&closed]() { closed = true; });
            // closing the state forces everything to get gcd
        }
        CHECK(closed);
        closed = false;
        {
            US L2{};
            L2.template NewUserClass<InheritanceTestF>([&closed]() { closed = true; });
            L2.template CheckUserClass<InheritanceTestF>(-1);
            L2.template CheckUserClass<InheritanceTestB>(-1);
            CHECK(L2.template OptionalUserClass<InheritanceTestD>(-1) == nullptr);
            CHECK(L2.template OptionalUserClass<IntHolderLua>(-1) == nullptr);
            // closing the state forces everything to get gcd
        }
        CHECK(closed);
    }
}