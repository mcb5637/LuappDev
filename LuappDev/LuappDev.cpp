#include "pch.h"
#include "CppUnitTest.h"

#include <format>
#include <functional>

#ifdef LUA50
#include "luapp/luapp50.h"
#endif
#ifdef LUA51
#include "luapp/luapp51.h"
#endif
#ifdef LUA52
#include "luapp/luapp52.h"
#endif
#ifdef LUA53
#include "luapp/luapp53.h"
#define hasbit
#endif
#ifdef LUA54
#include "luapp/luapp54.h"
#define hasbit
#endif

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

template<>
static std::wstring Microsoft::VisualStudio::CppUnitTestFramework::ToString<lua_State>(const lua_State* q) {
	return ToString(static_cast<const void*>(q));
}
template<>
static std::wstring Microsoft::VisualStudio::CppUnitTestFramework::ToString<lua_State>(lua_State* q) {
	return ToString(static_cast<void*>(q));
}

namespace LuappDev
{
	class IntHolderOp {
		lua::Integer i;

		static int get(lua::State L) {
			auto t = L.GetUserData<IntHolderOp>(1);
			L.Push(t->i);
			return 1;
		}
		static int set(lua::State L) {
			auto t = L.GetUserData<IntHolderOp>(1);
			t->i = L.CheckInt(2);
			return 0;
		}

	public:

		IntHolderOp(lua::Integer i) : i(i) {}

		auto operator<=>(const IntHolderOp&) const noexcept = default;

		IntHolderOp operator+(const IntHolderOp& o) const noexcept {
			return IntHolderOp{ this->i + o.i };
		}
		IntHolderOp operator-(const IntHolderOp& o) const noexcept {
			return IntHolderOp{ this->i - o.i };
		}
		IntHolderOp operator*(const IntHolderOp& o) const noexcept {
			return IntHolderOp{ this->i * o.i };
		}
		IntHolderOp operator/(const IntHolderOp& o) const noexcept {
			return IntHolderOp{ this->i / o.i };
		}
		IntHolderOp operator-() const noexcept {
			return IntHolderOp{ -this->i };
		}

		IntHolderOp operator&(const IntHolderOp& o) const noexcept {
			return IntHolderOp{ this->i & o.i };
		}
		IntHolderOp operator|(const IntHolderOp& o) const noexcept {
			return IntHolderOp{ this->i | o.i };
		}
		IntHolderOp operator^(const IntHolderOp& o) const noexcept {
			return IntHolderOp{ this->i ^ o.i };
		}
		IntHolderOp operator~() const noexcept {
			return IntHolderOp{ ~this->i };
		}
		IntHolderOp operator<<(const IntHolderOp& o) const noexcept {
			return IntHolderOp{ this->i << o.i };
		}
		IntHolderOp operator>>(const IntHolderOp& o) const noexcept {
			return IntHolderOp{ this->i >> o.i };
		}

		static constexpr std::array<lua::FuncReference, 2> LuaMethods{ {
				lua::FuncReference::GetRef<get>("Get"),
				lua::FuncReference::GetRef<set>("Set"),
		} };
	};

	class IntHolderLua {
		lua::Integer i;

		static int get(lua::State L) {
			auto t = L.GetUserData<IntHolderLua>(1);
			L.Push(t->i);
			return 1;
		}
		static int set(lua::State L) {
			auto t = L.GetUserData<IntHolderLua>(1);
			t->i = L.CheckInt(2);
			return 0;
		}

	public:

		IntHolderLua(lua::Integer i) : i(i) {}

		static constexpr std::array<lua::FuncReference, 2> LuaMethods{ {
				lua::FuncReference::GetRef<get>("Get"),
				lua::FuncReference::GetRef<set>("Set"),
		} };

		static int Equals(lua::State L) {
			auto t = L.GetUserData<IntHolderLua>(1);
			auto o = L.GetUserData<IntHolderLua>(2);
			L.Push(t->i == o->i);
			return 1;
		}
		static int LessThan(lua::State L) {
			auto t = L.GetUserData<IntHolderLua>(1);
			auto o = L.GetUserData<IntHolderLua>(2);
			L.Push(t->i < o->i);
			return 1;
		}
		static int LessOrEquals(lua::State L) {
			auto t = L.GetUserData<IntHolderLua>(1);
			auto o = L.GetUserData<IntHolderLua>(2);
			L.Push(t->i <= o->i);
			return 1;
		}
		static int Add(lua::State L) {
			auto t = L.GetUserData<IntHolderLua>(1);
			auto o = L.GetUserData<IntHolderLua>(2);
			L.NewUserData<IntHolderLua>(t->i + o->i);
			return 1;
		}
		static int Substract(lua::State L) {
			auto t = L.GetUserData<IntHolderLua>(1);
			auto o = L.GetUserData<IntHolderLua>(2);
			L.NewUserData<IntHolderLua>(t->i - o->i);
			return 1;
		}
		static int Multiply(lua::State L) {
			auto t = L.GetUserData<IntHolderLua>(1);
			auto o = L.GetUserData<IntHolderLua>(2);
			L.NewUserData<IntHolderLua>(t->i * o->i);
			return 1;
		}
		static int Divide(lua::State L) {
			auto t = L.GetUserData<IntHolderLua>(1);
			auto o = L.GetUserData<IntHolderLua>(2);
			L.NewUserData<IntHolderLua>(t->i / o->i);
			return 1;
		}
		static int Pow(lua::State L) {
			auto t = L.GetUserData<IntHolderLua>(1);
			auto o = L.GetUserData<IntHolderLua>(2);
			L.NewUserData<IntHolderLua>(static_cast<lua::Integer>(std::pow(t->i, o->i)));
			return 1;
		}
		static int UnaryMinus(lua::State L) {
			auto t = L.GetUserData<IntHolderLua>(1);
			L.NewUserData<IntHolderLua>(-t->i);
			return 1;
		}
		static int Concat(lua::State L) {
			auto t = L.GetUserData<IntHolderLua>(1);
			auto o = L.GetUserData<IntHolderLua>(2);
			L.Push(t->i);
			L.Push(o->i);
			L.Concat(2);
			return 1;
		}
		static int NewIndex(lua::State L) {
			auto t = L.GetUserData<IntHolderLua>(1);
			if (L.CheckStringView(2) != "i")
				throw lua::LuaException{ "invalid key" };
			t->i = L.CheckInt(3);
			return 0;
		}
		static int Index(lua::State L) {
			auto t = L.GetUserData<IntHolderLua>(1);
			if (L.CheckStringView(2) != "i")
				throw lua::LuaException{ "invalid key" };
			L.Push(t->i);
			return 1;
		}
		static int Call(lua::State L) {
			auto t = L.GetUserData<IntHolderLua>(1);
			auto o = L.CheckInt(2);
			L.NewUserData<IntHolderLua>(t->i * o);
			return 1;
		}
		static int Modulo(lua::State L) {
			auto t = L.GetUserData<IntHolderLua>(1);
			auto o = L.GetUserData<IntHolderLua>(2);
			L.NewUserData<IntHolderLua>(t->i % o->i);
			return 1;
		}
		static int Length(lua::State L) {
			auto t = L.GetUserData<IntHolderLua>(1);
			L.Push(t->i);
			L.ToString(-1);
			auto l = L.RawLength(-1);
			L.Push(static_cast<lua::Integer>(l));
			return 1;
		}
		static int IntegerDivide(lua::State L) {
			auto t = L.GetUserData<IntHolderLua>(1);
			auto o = L.GetUserData<IntHolderLua>(2);
			L.NewUserData<IntHolderLua>(t->i / o->i + 1);
			return 1;
		}
		static int BitwiseAnd(lua::State L) {
			auto t = L.GetUserData<IntHolderLua>(1);
			auto o = L.GetUserData<IntHolderLua>(2);
			L.NewUserData<IntHolderLua>(t->i & o->i);
			return 1;
		}
		static int BitwiseOr(lua::State L) {
			auto t = L.GetUserData<IntHolderLua>(1);
			auto o = L.GetUserData<IntHolderLua>(2);
			L.NewUserData<IntHolderLua>(t->i | o->i);
			return 1;
		}
		static int BitwiseXOr(lua::State L) {
			auto t = L.GetUserData<IntHolderLua>(1);
			auto o = L.GetUserData<IntHolderLua>(2);
			L.NewUserData<IntHolderLua>(t->i ^ o->i);
			return 1;
		}
		static int BitwiseNot(lua::State L) {
			auto t = L.GetUserData<IntHolderLua>(1);
			L.NewUserData<IntHolderLua>(~t->i);
			return 1;
		}
		static int ShiftLeft(lua::State L) {
			auto t = L.GetUserData<IntHolderLua>(1);
			auto o = L.GetUserData<IntHolderLua>(2);
			L.NewUserData<IntHolderLua>(t->i << o->i);
			return 1;
		}
		static int ShiftRight(lua::State L) {
			auto t = L.GetUserData<IntHolderLua>(1);
			auto o = L.GetUserData<IntHolderLua>(2);
			L.NewUserData<IntHolderLua>(t->i >> o->i);
			return 1;
		}
	};
	class DtorTest {
		std::function<void()> f;
	public:
		DtorTest(std::function<void()> f) : f(f) {}

		~DtorTest() {
			f();
		}
	};

	TEST_CLASS(LuappDev)
	{
	public:
		TEST_METHOD(CreateDelete)
		{
			lua::State L{};

			lua::StateCloser closer{ L };

			Assert::AreEqual(L.GetState(), closer.GetState().GetState());
			Assert::AreEqual(typeid(DtorTest).name(), typename_details::type_name<DtorTest>());
		}

		TEST_METHOD(StackAccess) {
			lua::StateCloser cl{};
			lua::State L = cl.GetState();

			Assert::AreEqual(0, L.GetTop());

			L.Push(1);
			L.Push(true);
			Assert::AreEqual(2, L.GetTop());
			Assert::IsTrue(L.IsNumber(1));
			Assert::IsTrue(L.IsBoolean(2));

			L.PushValue(2);
			Assert::AreEqual(3, L.GetTop());
			Assert::IsTrue(L.IsBoolean(3));
			L.SetTop(2);
			Assert::AreEqual(2, L.GetTop());

			Assert::AreEqual(lua::Integer{ 1 }, L.CheckInt(1));
			Assert::IsTrue(L.CheckBool(2));

			L.Push();
			L.Insert(2);
			Assert::AreEqual(3, L.GetTop());
			Assert::AreEqual(lua::Integer{ 1 }, L.CheckInt(1));
			Assert::IsTrue(L.IsNil(2));
			Assert::IsTrue(L.CheckBool(3));

			L.Push("");
			L.Replace(2);
			Assert::AreEqual(3, L.GetTop());
			Assert::AreEqual(lua::Integer{ 1 }, L.CheckInt(1));
			Assert::AreEqual("", L.CheckString(2));
			Assert::IsTrue(L.CheckBool(3));

			L.Copy(1, 2);
			Assert::AreEqual(3, L.GetTop());
			Assert::AreEqual(lua::Integer{ 1 }, L.CheckInt(1));
			Assert::AreEqual(lua::Integer{ 1 }, L.CheckInt(2));
			Assert::IsTrue(L.CheckBool(3));

			L.Remove(2);
			Assert::AreEqual(2, L.GetTop());
			Assert::AreEqual(lua::Integer{ 1 }, L.CheckInt(1));
			Assert::IsTrue(L.CheckBool(2));

			L.Pop(2);
			Assert::AreEqual(0, L.GetTop());
		}

		TEST_METHOD(BaseDatatypes) {
			lua::StateCloser cl{};
			lua::State L = cl.GetState();
			Assert::AreEqual(0, L.GetTop());

			Assert::IsTrue(L.IsNone(1));
			Assert::IsTrue(L.IsNoneOrNil(1));
			Assert::IsTrue(L.Type(1) == lua::LType::None);

			L.Push();
			Assert::IsTrue(L.IsNil(1));
			Assert::IsTrue(L.IsNoneOrNil(1));
			Assert::IsTrue(L.Type(1) == lua::LType::Nil);
			L.SetTop(0);

			L.Push(1);
			Assert::IsTrue(L.IsNumber(1));
			Assert::AreEqual(lua::Integer{ 1 }, L.CheckInt(1));
			Assert::AreEqual(1.0, L.CheckNumber(1));
			Assert::IsTrue(L.Type(1) == lua::LType::Number);
			L.SetTop(0);

			L.Push(1.3);
			Assert::IsTrue(L.IsNumber(1));
			//Assert::AreEqual(lua::Integer{ 1 }, L.CheckInt(1));
			Assert::AreEqual(1.3, L.CheckNumber(1));
			Assert::IsTrue(L.Type(1) == lua::LType::Number);
			L.SetTop(0);

			L.Push("a");
			Assert::IsTrue(L.IsString(1));
			Assert::AreEqual("a", L.CheckString(1));
			Assert::AreEqual(std::string_view{ "a" }, L.CheckStringView(1));
			Assert::IsTrue(L.Type(1) == lua::LType::String);
			L.SetTop(0);

			L.PushFString("a %d", 42);
			Assert::IsTrue(L.IsString(1));
			Assert::AreEqual("a 42", L.CheckString(1));
			Assert::AreEqual(std::string_view{ "a 42" }, L.CheckStringView(1));
			Assert::IsTrue(L.Type(1) == lua::LType::String);
			L.SetTop(0);

			L.Push(true);
			Assert::IsTrue(L.IsBoolean(1));
			Assert::AreEqual(true, L.CheckBool(1));
			Assert::IsTrue(L.Type(1) == lua::LType::Boolean);
			L.SetTop(0);

			L.PushLightUserdata(&cl);
			Assert::IsTrue(L.IsUserdata(1));
			Assert::IsTrue(L.IsLightUserdata(1));
			Assert::AreEqual(static_cast<void*>(&cl), L.ToUserdata(1));
			Assert::IsTrue(L.Type(1) == lua::LType::LightUserdata);
			L.SetTop(0);
		}

		static int foo_raw(lua_State* l) {
			lua::State L{ l };
			L.PushValue(1);
			L.Push(42);
			L.Arithmetic(lua::ArihmeticOperator::Add);
			return 1;
		}
		static int foo(lua::State L) {
			L.PushValue(1);
			L.Push(42);
			L.Arithmetic(lua::ArihmeticOperator::Add);
			return 1;
		}
		static int ex(lua::State L) {
			auto i = L.CheckInt(1);
			throw lua::LuaException{ std::format("number is {}", i) };
		}
		static constexpr std::array<lua::FuncReference, 1> toRegister{ {
				lua::FuncReference::GetRef<foo>("foo"),
			} };

		TEST_METHOD(Functions) {
			lua::StateCloser cl{};
			lua::State L = cl.GetState();
			Assert::AreEqual(0, L.GetTop());

			L.Push(&foo_raw);
			Assert::IsTrue(L.IsFunction(1));
			Assert::IsTrue(L.IsCFunction(1));
			Assert::IsTrue(L.Type(1) == lua::LType::Function);

			L.Push(5);
			L.TCall(1, 1);
			Assert::AreEqual(lua::Integer{ 42 + 5 }, L.CheckInt(1));
			L.SetTop(0);

			L.Push<foo>();
			Assert::IsTrue(L.IsFunction(1));
			Assert::IsTrue(L.IsCFunction(1));
			Assert::IsTrue(L.Type(1) == lua::LType::Function);

			L.Push(5);
			L.TCall(1, 1);
			Assert::AreEqual(lua::Integer{ 42 + 5 }, L.CheckInt(1));
			L.SetTop(0);

			L.Push<foo>();

			L.Push(5);
			L.Push("bar");
			L.TCall(2, 2);
			Assert::AreEqual(lua::Integer{ 42 + 5 }, L.CheckInt(1));
			Assert::IsTrue(L.IsNil(2));
			L.SetTop(0);

			L.Push<ex>();
			L.Push(5);
			Assert::IsTrue(L.PCall(1, 0) == lua::ErrorCode::Runtime);
			Assert::IsTrue(L.CheckStringView(1).find("number is 5") != std::string_view::npos);
			L.SetTop(0);
			bool ok = false;
			try {
				L.Push<ex>();
				L.Push(5);
				L.TCall(1, 0);
				L.SetTop(0);
			}
			catch (const lua::LuaException& e) {
				if (std::string_view{ e.what() }.find("number is 5") != std::string_view::npos)
					ok = true;
			}
			Assert::IsTrue(ok);
		}

		TEST_METHOD(Tables) {
			lua::StateCloser cl{};
			lua::State L = cl.GetState();
			Assert::AreEqual(0, L.GetTop());

			L.NewTable();
			Assert::IsTrue(L.IsTable(1));

			L.Push("a");
			L.Push(42);
			L.SetTable(1);
			L.Push("a");
			L.GetTable(1);
			Assert::AreEqual(lua::Integer{ 42 }, L.CheckInt(2));
			L.Pop(1);

			L.Push("a");
			L.Push(43);
			L.SetTableRaw(1);
			L.Push("a");
			L.GetTableRaw(1);
			Assert::AreEqual(lua::Integer{ 43 }, L.CheckInt(2));
			L.Pop(1);

			L.Push("a");
			L.SetTableRaw(1, 1);
			L.GetTableRaw(1, 1);
			Assert::AreEqual("a", L.CheckString(2));
			L.Pop(1);

			Assert::IsFalse(L.GetMetatable(1));
			L.NewTable();
			L.PushValue(2);
			Assert::IsTrue(L.SetMetatable(1));
			Assert::IsTrue(L.GetMetatable(1));
			Assert::AreEqual(L.ToPointer(2), L.ToPointer(3));
			Assert::IsTrue(L.RawEqual(2, 3));
			L.Pop(2);
		}

		TEST_METHOD(Operators) {
			lua::StateCloser cl{};
			lua::State L = cl.GetState();
			Assert::AreEqual(0, L.GetTop());

			L.Push(5);
			L.Push(5);
			L.Push(10);
			Assert::IsTrue(L.LessThan(1, 3));
			Assert::IsTrue(L.Compare(1, 3, lua::ComparisonOperator::LessThan));
			Assert::IsTrue(L.Compare(1, 3, lua::ComparisonOperator::LessThanOrEquals));
			Assert::IsTrue(L.Equal(1, 2));
			Assert::IsTrue(L.RawEqual(1, 2));
			Assert::IsTrue(L.Compare(1, 2, lua::ComparisonOperator::Equals));
			Assert::IsTrue(L.Compare(1, 2, lua::ComparisonOperator::LessThanOrEquals));

			L.Arithmetic(lua::ArihmeticOperator::Add);
			Assert::AreEqual(lua::Integer{ 15 }, L.CheckInt(2));

			L.Arithmetic(lua::ArihmeticOperator::Subtract);
			Assert::AreEqual(lua::Integer{ -10 }, L.CheckInt(1));

			L.Push(2);
			L.Arithmetic(lua::ArihmeticOperator::Multiply);
			Assert::AreEqual(lua::Integer{ -20 }, L.CheckInt(1));

			L.Push(2);
			L.Arithmetic(lua::ArihmeticOperator::Divide);
			Assert::AreEqual(lua::Integer{ -10 }, L.CheckInt(1));

			L.Arithmetic(lua::ArihmeticOperator::UnaryNegation);
			Assert::AreEqual(lua::Integer{ 10 }, L.CheckInt(1));

			L.Push(4);
			L.Arithmetic(lua::ArihmeticOperator::Modulo);
			Assert::AreEqual(lua::Integer{ 2 }, L.CheckInt(1));

			L.Push(4);
			L.Arithmetic(lua::ArihmeticOperator::Pow);
			Assert::AreEqual(lua::Integer{ 16 }, L.CheckInt(1));
			L.Pop(1);

			L.Push(16);
			L.Push("abc");
			L.Concat(2);
			Assert::AreEqual("16abc", L.CheckString(1));
		}

		TEST_METHOD(RunLua) {
			lua::StateCloser cl{};
			lua::State L = cl.GetState();
			Assert::AreEqual(0, L.GetTop());

			L.DoStringT("function fool(a, b) return a+b end");
			Assert::AreEqual(0, L.GetTop());
			L.GetGlobal("fool");
			Assert::IsTrue(L.IsFunction(1));
			Assert::IsFalse(L.IsCFunction(1));

			L.Push(5);
			L.Push(42);
			L.Push("abc");
			L.TCall(3, 1);
			Assert::AreEqual(lua::Integer{ 5 + 42 }, L.CheckInt(1));
			L.Pop(1);

			L.RegisterFuncs(toRegister);
			L.DoStringT("return foo(6, 666)");
			Assert::AreEqual(lua::Integer{ 6 + 42 }, L.CheckInt(1));
			L.SetTop(0);

			Assert::IsTrue(L.DoString("error('number is 6')") != lua::ErrorCode::Success);
			//Assert::IsTrue(L.CheckStringView(1).find("number is 6") != std::string_view::npos);
			L.SetTop(0);
			bool ok = false;
			try {
				L.DoStringT("error('number is 6')");
			}
			catch (const lua::LuaException& e) {
				if (std::string_view{ e.what() }.find("number is 6") != std::string_view::npos)
					ok = true;
			}
			Assert::IsTrue(ok);
		}

		TEST_METHOD(Userdata) {
			lua::StateCloser cl{};
			lua::State L = cl.GetState();
			Assert::AreEqual(0, L.GetTop());

			L.NewUserData<IntHolderOp>(5);
			L.SetGlobal("i");
			L.NewUserData<IntHolderOp>(7);
			L.SetGlobal("j");

			try {
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
#ifdef hasbit
				L.DoStringT("j:Set(4); i:Set(2); local k=i+j; k:Set(6);\n"
					"assert((i&k):Get()==2);\n"
					"assert((i|j):Get()==6);\n"
					"assert((i~k):Get()==4);\n"
					"assert((~i):Get()==~2);\n"
					"assert((j<<i):Get()==16);\n"
					"assert((j>>i):Get()==1);\n");
#endif
			}
			catch (const lua::LuaException& e) {
				auto m = ToString(e.what());
				Assert::Fail(m.c_str());
			}

			L.NewUserData<IntHolderLua>(5);
			L.SetGlobal("i");
			L.NewUserData<IntHolderLua>(7);
			L.SetGlobal("j");

			try {
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
#ifndef LUA50
				L.DoStringT("local a = i+j; a.i=4;\n"
					"assert((j%a).i==2);\n"
					"assert(#j==2)\n");
#endif
#ifdef hasbit
				L.DoStringT("assert((j//i):Get()==3);\n"
					"j:Set(4); i:Set(2); local k=i+j; k:Set(6);\n"
					"assert((i&k):Get()==2);\n"
					"assert((i|j):Get()==6);\n"
					"assert((i~k):Get()==4);\n"
					"assert((~i):Get()==~2);\n"
					"assert((j<<i):Get()==16);\n"
					"assert((j>>i):Get()==1);\n");
#endif
			}
			catch (const lua::LuaException& e) {
				auto m = ToString(e.what());
				Assert::Fail(m.c_str());
			}

			bool closed = false;
			{
				lua::StateCloser cl2{};
				cl2.GetState().NewUserData<DtorTest>([&closed]() { closed = true; });
				// closing the state forces everything to get gcd
			}
			Assert::IsTrue(closed);
		}

		TEST_METHOD(Iterate) {
			lua::StateCloser cl{};
			lua::State L = cl.GetState();
			Assert::AreEqual(0, L.GetTop());

			L.DoStringT("return {[1]=5,[2]=6,[4]=8}");
			Assert::AreEqual(1, L.GetTop());
			int i = 0;
			for (auto k : L.Pairs(1)) {
				Assert::AreEqual(L.CheckInt(-2)+4, L.CheckInt(-1));
				++i;
			}
			Assert::AreEqual(3, i);
			Assert::AreEqual(1, L.GetTop());
			i = 0;
			for (auto k : L.IPairs(1)) {
				Assert::AreEqual(lua::Integer{ k + 4 }, L.CheckInt(-1));
				++i;
			}
			Assert::AreEqual(2, i);
			Assert::AreEqual(1, L.GetTop());
		}

		static int black_magic(lua::State L) {
			lua::DebugInfo i{};
			Assert::IsTrue(L.Debug_GetStack(1, i, lua::DebugInfoOptions::Name, true));
			Assert::AreEqual("foo", i.Name);
			int l = 1;
			while (const char* n = L.Debug_GetLocal(1, l)) {
				if (n == std::string_view{ "l" }) {
					Assert::AreEqual(lua::Integer{ 1 }, L.CheckInt(-1));
					L.Push(42);
					Assert::AreEqual("l", L.Debug_SetLocal(1, l));
				}
				L.Pop(1);
				++l;
			}
			int u = 1;
			while (const char* n = L.Debug_GetUpvalue(-1, u)) {
				if (n == std::string_view{ "upv" }) {
					Assert::AreEqual(lua::Integer{ 0 }, L.CheckInt(-1));
					L.Push(4242);
					Assert::AreEqual("upv", L.Debug_SetUpvalue(-3, u));
				}
				L.Pop(1);
				++u;
			}
			return 0;
		}
		TEST_METHOD(LocalAcess) {
			lua::StateCloser cl{};
			lua::State L = cl.GetState();
			Assert::AreEqual(0, L.GetTop());

			L.Push("black_magic");
			L.Push<black_magic>();
			L.SetGlobal();

			L.DoStringT("local upv = 0; function foo() local l = 1; black_magic(); return l,upv; end; local a,b = foo(); return a,b;");
			Assert::AreEqual(2, L.GetTop());
			Assert::AreEqual(lua::Integer{ 42 }, L.CheckInt(1));
			Assert::AreEqual(lua::Integer{ 4242 }, L.CheckInt(2));
		}

		static void hook_magic(lua::State L, lua::ActivationRecord ar) {
			if (L.Debug_GetEventFromAR(ar) == lua::HookEvent::Return) {
				auto inf = L.Debug_GetInfoFromAR(ar, lua::DebugInfoOptions::Name);
				if (inf.Name && inf.Name == std::string_view{ "bar" }) {
					L.Debug_GetStack(1, inf, lua::DebugInfoOptions::Name, true);
					Assert::AreEqual("foo", inf.Name);
					int l = 1;
					while (const char* n = L.Debug_GetLocal(1, l)) {
						if (n == std::string_view{ "l" }) {
							Assert::AreEqual(lua::Integer{ 1 }, L.CheckInt(-1));
							L.Push(42);
							Assert::AreEqual("l", L.Debug_SetLocal(1, l));
						}
						L.Pop(1);
						++l;
					}
					int u = 1;
					while (const char* n = L.Debug_GetUpvalue(-1, u)) {
						if (n == std::string_view{ "upv" }) {
							Assert::AreEqual(lua::Integer{ 0 }, L.CheckInt(-1));
							L.Push(4242);
							Assert::AreEqual("upv", L.Debug_SetUpvalue(-3, u));
						}
						L.Pop(1);
						++u;
					}
					L.Pop(1);
				}
			}
		}
		TEST_METHOD(Hook) {
			lua::StateCloser cl{};
			lua::State L = cl.GetState();
			Assert::AreEqual(0, L.GetTop());

			L.Debug_SetHook<hook_magic>(lua::HookEvent::Call | lua::HookEvent::Return, 0);

			L.DoStringT("local upv = 0; function bar() end; function foo() local l = 1; bar(); return l,upv; end; local a,b = foo(); return a,b;");
			Assert::AreEqual(2, L.GetTop());
			Assert::AreEqual(lua::Integer{ 42 }, L.CheckInt(1));
			Assert::AreEqual(lua::Integer{ 4242 }, L.CheckInt(2));
		}
	};
}
