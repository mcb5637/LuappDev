#include <doctest/doctest.h>

#include <DtorTest.h>
#include <iostream>
#include <luaver.h>

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

            static constexpr std::array LuaMetaMethods{
                S::FuncReference::template GetRef<get>("Get"),
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
            using InheritsFrom = std::tuple<InheritanceTestB<S>>;

            InheritanceTestD(lua::Integer i, lua::Number j) : InheritanceTestB<S>(i), j(j) {}

            static constexpr std::array LuaMethods{
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
            using InheritsFrom = std::tuple<InheritanceTestB<S>>;

            static constexpr std::array<typename S::FuncReference, 0> LuaMethods{};

            explicit InheritanceTestF(std::function<void()> f) : InheritanceTestB<S>(42), f(std::move(f)) {}

            ~InheritanceTestF() override { f(); }
        };

        template<class S>
        class Accumulate
        {
            lua::Number n = 0.0;

        protected:
            static int Add(S L)
            {
                auto* th = L.template CheckUserClass<Accumulate>(1);
                th->n += L.CheckNumber(2);
                return 0;
            }
            static int Get(S L)
            {
                auto* th = L.template CheckUserClass<Accumulate>(1);
                L.Push(th->n);
                return 1;
            }
            static int Virt(S L)
            {
                [[maybe_unused]] auto* th = L.template CheckUserClass<Accumulate>(1);
                L.Push(5);
                return 1;
            }
        public:

            static constexpr std::array LuaMethods{
                S::FuncReference::template GetRef<Add>("Add"),
                S::FuncReference::template GetRef<Get>("Get"),
                S::FuncReference::template GetRef<Virt>("Virt"),
            };
        };
        template<class S>
        class Append
        {
            std::string s;

        protected:
            static int AddS(S L)
            {
                auto* th = L.template CheckUserClass<Append>(1);
                th->s += L.CheckStringView(2);
                return 0;
            }
            static int GetS(S L)
            {
                auto* th = L.template CheckUserClass<Append>(1);
                L.Push(th->s);
                return 1;
            }
        public:

            static constexpr std::array LuaMethods{
                S::FuncReference::template GetRef<AddS>("AddS"),
                S::FuncReference::template GetRef<GetS>("GetS"),
            };
        };

        template<class S>
        class MultiInheritance : public Accumulate<S>, public Append<S>
        {
            std::function<void()> f;
            static int Virt(S L)
            {
                [[maybe_unused]] auto* th = L.template CheckUserClass<MultiInheritance>(1);
                L.Push(10);
                return 1;
            }

        public:
            explicit MultiInheritance(std::function<void()> f) : f(std::move(f)) {}
            ~MultiInheritance()
            {
                f();
            }

            static constexpr std::array LuaMethods{
                S::FuncReference::template GetRef<Virt>("Virt"),
            };

            using InheritsFrom = std::tuple<Accumulate<S>, Append<S>>;
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
        using MultiInheritance = cls::MultiInheritance<S>;

        static_assert(lua::userdata::HasLuaMethods<IntHolderOp>);
        static_assert(lua::userdata::HasLuaMetaMethods<IntHolderOp>);
        static_assert(!lua::userdata::EquatableCpp<S, IntHolderOp>);
        static_assert(lua::userdata::EquatableOp<IntHolderOp>);
        static_assert(!lua::userdata::LessThanCpp<S, IntHolderOp>);
        static_assert(lua::userdata::LessThanOp<IntHolderOp>);
        static_assert(!lua::userdata::LessThanEqualsCpp<S, IntHolderOp>);
        static_assert(lua::userdata::LessThanEqualsOp<IntHolderOp>);
        static_assert(!lua::userdata::AddCpp<S, IntHolderOp>);
        static_assert(lua::userdata::AddOp<IntHolderOp>);
        static_assert(!lua::userdata::SubtractCpp<S, IntHolderOp>);
        static_assert(lua::userdata::SubtractOp<IntHolderOp>);
        static_assert(!lua::userdata::MultiplyCpp<S, IntHolderOp>);
        static_assert(lua::userdata::MultiplyOp<IntHolderOp>);
        static_assert(!lua::userdata::DivideCpp<S, IntHolderOp>);
        static_assert(lua::userdata::DivideOp<IntHolderOp>);
        static_assert(!lua::userdata::IntegerDivideCpp<S, IntHolderOp>);
        static_assert(!lua::userdata::ModuloCpp<S, IntHolderOp>);
        static_assert(!lua::userdata::PowCpp<S, IntHolderOp>);
        static_assert(!lua::userdata::UnaryMinusCpp<S, IntHolderOp>);
        static_assert(lua::userdata::UnaryMinusOp<IntHolderOp>);
        static_assert(!lua::userdata::BitwiseAndCpp<S, IntHolderOp>);
        static_assert(lua::userdata::BitwiseAndOp<IntHolderOp>);
        static_assert(!lua::userdata::BitwiseOrCpp<S, IntHolderOp>);
        static_assert(lua::userdata::BitwiseOrOp<IntHolderOp>);
        static_assert(!lua::userdata::BitwiseXOrCpp<S, IntHolderOp>);
        static_assert(lua::userdata::BitwiseXOrOp<IntHolderOp>);
        static_assert(!lua::userdata::BitwiseNotCpp<S, IntHolderOp>);
        static_assert(lua::userdata::BitwiseNotOp<IntHolderOp>);
        static_assert(!lua::userdata::ShiftLeftCpp<S, IntHolderOp>);
        static_assert(lua::userdata::ShiftLeftOp<IntHolderOp>);
        static_assert(!lua::userdata::ShiftRightCpp<S, IntHolderOp>);
        static_assert(lua::userdata::ShiftRightOp<IntHolderOp>);
        static_assert(!lua::userdata::LengthCpp<S, IntHolderOp>);
        static_assert(!lua::userdata::ConcatCpp<S, IntHolderOp>);
        static_assert(!lua::userdata::NewIndexCpp<S, IntHolderOp>);
        static_assert(!lua::userdata::CallCpp<S, IntHolderOp>);
        static_assert(!lua::userdata::IndexCpp<S, IntHolderOp>);
        static_assert(!lua::userdata::ToStringCpp<S, IntHolderOp>);


        static_assert(lua::userdata::HasLuaMethods<IntHolderLua>);
        static_assert(!lua::userdata::HasLuaMetaMethods<IntHolderLua>);
        static_assert(lua::userdata::EquatableCpp<S, IntHolderLua>);
        static_assert(!lua::userdata::EquatableOp<IntHolderLua>);
        static_assert(lua::userdata::LessThanCpp<S, IntHolderLua>);
        static_assert(!lua::userdata::LessThanOp<IntHolderLua>);
        static_assert(lua::userdata::LessThanEqualsCpp<S, IntHolderLua>);
        static_assert(!lua::userdata::LessThanEqualsOp<IntHolderLua>);
        static_assert(lua::userdata::AddCpp<S, IntHolderLua>);
        static_assert(!lua::userdata::AddOp<IntHolderLua>);
        static_assert(lua::userdata::SubtractCpp<S, IntHolderLua>);
        static_assert(!lua::userdata::SubtractOp<IntHolderLua>);
        static_assert(lua::userdata::MultiplyCpp<S, IntHolderLua>);
        static_assert(!lua::userdata::MultiplyOp<IntHolderLua>);
        static_assert(lua::userdata::DivideCpp<S, IntHolderLua>);
        static_assert(!lua::userdata::DivideOp<IntHolderLua>);
        static_assert(lua::userdata::IntegerDivideCpp<S, IntHolderLua>);
        static_assert(lua::userdata::ModuloCpp<S, IntHolderLua>);
        static_assert(lua::userdata::PowCpp<S, IntHolderLua>);
        static_assert(lua::userdata::UnaryMinusCpp<S, IntHolderLua>);
        static_assert(!lua::userdata::UnaryMinusOp<IntHolderLua>);
        static_assert(lua::userdata::BitwiseAndCpp<S, IntHolderLua>);
        static_assert(!lua::userdata::BitwiseAndOp<IntHolderLua>);
        static_assert(lua::userdata::BitwiseOrCpp<S, IntHolderLua>);
        static_assert(!lua::userdata::BitwiseOrOp<IntHolderLua>);
        static_assert(lua::userdata::BitwiseXOrCpp<S, IntHolderLua>);
        static_assert(!lua::userdata::BitwiseXOrOp<IntHolderLua>);
        static_assert(lua::userdata::BitwiseNotCpp<S, IntHolderLua>);
        static_assert(!lua::userdata::BitwiseNotOp<IntHolderLua>);
        static_assert(lua::userdata::ShiftLeftCpp<S, IntHolderLua>);
        static_assert(!lua::userdata::ShiftLeftOp<IntHolderLua>);
        static_assert(lua::userdata::ShiftRightCpp<S, IntHolderLua>);
        static_assert(!lua::userdata::ShiftRightOp<IntHolderLua>);
        static_assert(lua::userdata::LengthCpp<S, IntHolderLua>);
        static_assert(lua::userdata::ConcatCpp<S, IntHolderLua>);
        static_assert(lua::userdata::NewIndexCpp<S, IntHolderLua>);
        static_assert(lua::userdata::CallCpp<S, IntHolderLua>);
        static_assert(lua::userdata::IndexCpp<S, IntHolderLua>);
        static_assert(lua::userdata::ToStringCpp<S, IntHolderLua>);

        static_assert(!lua::userdata::InheritsDefined<IntHolderLua>);
        static_assert(lua::userdata::InheritsDefined<InheritanceTestD>);
        static_assert(lua::userdata::InheritsDefined<InheritanceTestF>);

        US L{};
        CHECK_EQ(0, L.GetTop());

        L.template NewUserClass<IntHolderOp>(5);
        L.SetGlobal("i");
        L.template NewUserClass<IntHolderOp>(7);
        CHECK(L.template OptionalUserClass<IntHolderLua>(-1) == nullptr);
        AssertRegex("LuappDev::cls::IntHolderOp<lua::decorator::State<[:, [:alnum:]]+>>: 0x[0-9a-f]+", L.ConvertToString(1));
        AssertRegex("<Userdata (class )?LuappDev::cls::IntHolderOp<lua::decorator::State<[:, [:alnum:]]+>> 0x[0-9a-f]+>", L.ToDebugString(1));
        CHECK(L.GetMetaField(1, "Get"));
        CHECK(L.IsCFunction(-1));
        L.SetTop(1);
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
        CHECK_NOTHROW(L.template CheckUserClass<InheritanceTestB>(-1));
        CHECK_NOTHROW(L.template CheckUserClass<InheritanceTestD>(-1));
        CHECK(L.template OptionalUserClass<InheritanceTestF>(-1) == nullptr);
        CHECK(L.template OptionalUserClass<IntHolderLua>(-1) == nullptr);
        L.SetGlobal("inhd");

        CHECK_NOTHROW(L.template NewUserClass<InheritanceTestB>(5));
        CHECK_NOTHROW(L.template CheckUserClass<InheritanceTestB>(-1));
        CHECK(L.template OptionalUserClass<InheritanceTestF>(-1) == nullptr);
        CHECK(L.template OptionalUserClass<InheritanceTestD>(-1) == nullptr);
        CHECK(L.template OptionalUserClass<IntHolderLua>(-1) == nullptr);
        L.SetGlobal("inhb");

        CHECK_NOTHROW(L.DoStringT("assert(inhd:GetI()==5);\n"
                    "assert(inhd:GetJ()==10);\n"
                    "assert(inhb:GetI()==5);\n"
                    "assert(inhb.GetJ == nil);\n"));

        if constexpr (S::Capabilities::Uservalues)
        {
            L.template NewUserClass<cls::UservalueTest<S>>();
            L.SetGlobal("uvt");
            CHECK_NOTHROW(L.DoStringT("if uvt:n()==3 then\n"
                        "uvt:Set(1,2,3)\n"
                        "local a,b,c = uvt:Get()\n"
                        "assert(a==3)\n"
                        "assert(b==2)\n"
                        "assert(c==1)\n"
                        "elseif uvt:n()==1 then\n"
                        "uvt:Set({1})\n"
                        "assert(uvt:Get()[1]==1)\n"
                        "end"));
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
            CHECK_NOTHROW(L2.template CheckUserClass<InheritanceTestF>(-1));
            CHECK_NOTHROW(L2.template CheckUserClass<InheritanceTestB>(-1));
            CHECK(L2.template OptionalUserClass<InheritanceTestD>(-1) == nullptr);
            CHECK(L2.template OptionalUserClass<IntHolderLua>(-1) == nullptr);
            // closing the state forces everything to get gcd
        }
        CHECK(closed);
        closed = false;
        {
            US L2{};
            L2.template NewUserClass<MultiInheritance>([&closed]() { closed = true; });
            CHECK_NOTHROW(L2.template CheckUserClass<MultiInheritance>(-1));
            L2.SetGlobal("i");
            CHECK_NOTHROW(L2.DoStringT("assert(i:Get() == 0)\n"
                        "i:Add(42)\n"
                        "assert(i:Get() == 42)\n"
                        "assert(i:GetS() == '')\n"
                        "i:AddS('foo')\n"
                        "assert(i:GetS() == 'foo')\n"
                        "assert(i:Virt() == 10)\n"));
        }
        CHECK(closed);
    }
}