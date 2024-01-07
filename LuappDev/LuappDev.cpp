#include "pch.h"
#include "CppUnitTest.h"

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
		static int ToString(lua::State L) {
			auto t = L.GetUserData<IntHolderLua>(1);
			L.Push(std::format("IntHolder {}", t->i));
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

	bool RegexMatch(const char* p, std::string_view data) {
		std::regex pattern{ p, std::regex_constants::ECMAScript };
		return std::regex_match(data.begin(), data.end(), pattern);
	}
	void AssertRegex(const char* p, std::string_view data) {
		Assert::IsTrue(RegexMatch(p, data), ToString(std::format("expected '{}', found '{}'", p, data)).c_str());
	}

	TEST_CLASS(LuappDev)
	{
	public:
		TEST_METHOD(CreateDelete)
		{
			lua::State L{};

			lua::UniqueState closer{ L };

			Assert::AreEqual(L.GetState(), closer.GetState());
			Assert::AreEqual(typeid(DtorTest).name(), typename_details::type_name<DtorTest>());

			bool closed = false;
			{
				closer.NewUserData<DtorTest>([&closed]() { closed = true; });

				Assert::IsFalse(closed);

				lua::UniqueState c2{ std::move(closer) };
				Assert::AreEqual(L.GetState(), c2.GetState());
#pragma warning( push )
#pragma warning( disable : 26800 )
				Assert::AreEqual(closer.GetState(), static_cast<lua_State*>(nullptr));
#pragma warning( pop )

				Assert::IsFalse(closed);
			}

			Assert::IsTrue(closed);
		}

		TEST_METHOD(StackAccess) {
			lua::UniqueState L{};

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
			Assert::AreEqual(2, L.ToAbsoluteIndex(-1));
			Assert::AreEqual(1, L.ToAbsoluteIndex(-2));
			Assert::AreEqual(L.REGISTRYINDEX, L.ToAbsoluteIndex(L.REGISTRYINDEX));

			Assert::AreEqual(lua::Integer{ 1 }, L.CheckInteger(1));
			Assert::IsTrue(L.CheckBool(2));

			L.Push();
			L.Insert(2);
			Assert::AreEqual(3, L.GetTop());
			Assert::AreEqual(lua::Integer{ 1 }, L.CheckInteger(1));
			Assert::IsTrue(L.IsNil(2));
			Assert::IsTrue(L.CheckBool(3));

			L.Push("");
			L.Replace(2);
			Assert::AreEqual(3, L.GetTop());
			Assert::AreEqual(lua::Integer{ 1 }, L.CheckInteger(1));
			Assert::AreEqual("", L.CheckString(2));
			Assert::IsTrue(L.CheckBool(3));

			L.Copy(1, 2);
			Assert::AreEqual(3, L.GetTop());
			Assert::AreEqual(lua::Integer{ 1 }, L.CheckInteger(1));
			Assert::AreEqual(lua::Integer{ 1 }, L.CheckInteger(2));
			Assert::IsTrue(L.CheckBool(3));

			L.Remove(2);
			Assert::AreEqual(2, L.GetTop());
			Assert::AreEqual(lua::Integer{ 1 }, L.CheckInteger(1));
			Assert::IsTrue(L.CheckBool(2));

			L.Pop(2);
			Assert::AreEqual(0, L.GetTop());
		}

		template<class S>
		void BaseDatatypes_T() {
			S L{};
			Assert::AreEqual(0, L.GetTop());

			Assert::IsTrue(L.IsNone(1));
			Assert::IsTrue(L.IsNoneOrNil(1));
			Assert::IsTrue(L.Type(1) == lua::LType::None);

			L.Push();
			Assert::IsTrue(L.IsNil(1));
			Assert::IsTrue(L.IsNoneOrNil(1));
			Assert::IsTrue(L.Type(1) == lua::LType::Nil);
			Assert::AreEqual(std::string_view{ "nil" }, L.ConvertToString(1));
			Assert::AreEqual(std::string{ "nil" }, L.ToDebugString(1));
			L.SetTop(0);

			L.Push(1);
			Assert::IsTrue(L.IsNumber(1));
			if constexpr (S::Capabilities::NativeIntegers)
				Assert::IsTrue(L.IsInteger(1));

			Assert::AreEqual(lua::Integer{ 1 }, L.CheckInteger(1));
			Assert::AreEqual(1.0, L.CheckNumber(1));
			Assert::IsTrue(L.Type(1) == lua::LType::Number);
			Assert::AreEqual(std::string_view{ "1" }, L.ConvertToString(1));
			Assert::AreEqual(std::string{ "1" }, L.ToDebugString(1));
			L.SetTop(0);

			L.Push(1.3);
			Assert::IsTrue(L.IsNumber(1));
			if constexpr (S::Capabilities::NativeIntegers)
				Assert::IsFalse(L.IsInteger(1));
			else
				Assert::AreEqual(lua::Integer{ 1 }, L.CheckInteger(1));
			Assert::AreEqual(1.3, L.CheckNumber(1));
			Assert::IsTrue(L.Type(1) == lua::LType::Number);
			Assert::AreEqual(std::string_view{ "1.3" }, L.ConvertToString(1));
			Assert::AreEqual(std::string{ "1.3" }, L.ToDebugString(1));
			L.SetTop(0);

			L.Push("a");
			Assert::IsTrue(L.IsString(1));
			Assert::AreEqual("a", L.CheckString(1));
			Assert::AreEqual(std::string_view{ "a" }, L.CheckStringView(1));
			Assert::IsTrue(L.Type(1) == lua::LType::String);
			Assert::AreEqual(std::string_view{ "a" }, L.ConvertToString(1));
			Assert::AreEqual(std::string{ "\"a\"" }, L.ToDebugString(1));
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
			Assert::AreEqual(std::string_view{ "true" }, L.ConvertToString(1));
			Assert::AreEqual(std::string{ "true" }, L.ToDebugString(1));
			L.SetTop(0);

			L.PushLightUserdata(&L);
			Assert::IsTrue(L.IsUserdata(1));
			Assert::IsTrue(L.IsLightUserdata(1));
			Assert::AreEqual(static_cast<void*>(&L), L.ToUserdata(1));
			Assert::IsTrue(L.Type(1) == lua::LType::LightUserdata);
			AssertRegex("userdata: 0x[0-9a-f]+", L.ConvertToString(1));
			AssertRegex("<LightUserdata 0x[0-9a-f]+>", L.ToDebugString(1));
			L.SetTop(0);

			if constexpr (S::Capabilities::Uservalues)
			{
				if constexpr (S::Capabilities::ArbitraryUservalues) {
					L.NewUserdata(sizeof(int), 1);
				}
				else {
					L.NewUserdata(sizeof(int));
				}
				Assert::IsTrue(L.IsUserdata(1));
				Assert::IsFalse(L.IsLightUserdata(1));
				Assert::IsTrue(L.Type(1) == lua::LType::Userdata);
				AssertRegex("userdata: 0x[0-9a-f]+", L.ConvertToString(1));
				AssertRegex("<Userdata unknown type 0x[0-9a-f]+>", L.ToDebugString(1));
				L.SetTop(1);

				L.NewTable();
				L.PushValue(2);
				L.SetUserValue(1);
				L.GetUserValue(1);
				Assert::IsTrue(L.IsTable(3));
				Assert::IsTrue(L.RawEqual(2, 3));
				L.SetTop(0);
			}
		}
		TEST_METHOD(BaseDatatypes) {
			BaseDatatypes_T<lua::UniqueState>();
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
		static int foo_up(lua::State L) {
			Assert::AreEqual(1, L.Debug_GetStackDepth());
			L.PushValue(1);
			L.PushValue(L.Upvalueindex(1));
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
			lua::UniqueState L{};
			Assert::AreEqual(0, L.GetTop());

			L.Push(&foo_raw);
			Assert::IsTrue(L.IsFunction(1));
			Assert::IsTrue(L.IsCFunction(1));
			Assert::IsTrue(L.Type(1) == lua::LType::Function);

			L.Push(5);
			Assert::AreEqual(1, L.TCall(1, 1));
			Assert::AreEqual(1, L.GetTop());
			Assert::AreEqual(lua::Integer{ 42 + 5 }, L.CheckInteger(1));
			L.SetTop(0);

			L.Push<foo>();
			Assert::IsTrue(L.IsFunction(1));
			Assert::IsTrue(L.IsCFunction(1));
			Assert::IsTrue(L.Type(1) == lua::LType::Function);

			L.Push(5);
			Assert::AreEqual(1, L.TCall(1, 1));
			Assert::AreEqual(1, L.GetTop());
			Assert::AreEqual(lua::Integer{ 42 + 5 }, L.CheckInteger(1));
			L.SetTop(0);

			L.Push(44);
			L.Push<foo_up>(1);
			L.Push(5);
			Assert::AreEqual(1, L.TCall(1, L.MULTIRET));
			Assert::AreEqual(1, L.GetTop());
			Assert::AreEqual(lua::Integer{ 44 + 5 }, L.CheckInteger(1));
			L.SetTop(0);

			L.Push<foo>();

			L.Push(5);
			L.Push("bar");
			Assert::AreEqual(2, L.TCall(2, 2));
			Assert::AreEqual(lua::Integer{ 42 + 5 }, L.CheckInteger(1));
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

			L.SetTop(0);
			struct A {
				int i;

				int P(lua::State L) {
					L.Push(i + L.CheckInt(1) + L.CheckInt(L.Upvalueindex(2)));
					return 1;
				}
				int PC(lua::State L) const {
					L.Push(i + 1 + L.CheckInt(1) + L.CheckInt(L.Upvalueindex(2)));
					return 1;
				}
				int G(lua::State L) {
					L.Push(i);
					return 1;
				}
				int GC(lua::State L) const {
					L.Push(i + 1);
					return 1;
				}
			};
			A a{ 5 };
			L.Push(3);
			L.Push<A, &A::P>(a, 1);
			L.Push(4);
			L.TCall(1, 1);
			Assert::AreEqual(12, L.CheckInt(1));
			L.SetTop(0);

			L.Push(3);
			L.Push<A, &A::PC>(a, 1);
			L.Push(4);
			L.TCall(1, 1);
			Assert::AreEqual(13, L.CheckInt(1));
			L.SetTop(0);

			std::array<lua::FuncReference, 2> toregobj{ {
					lua::FuncReference::GetRef<A, &A::G>(a, "G"),
					lua::FuncReference::GetRef<A, &A::GC>(a, "GC"),
			} };
			L.RegisterGlobalLib(toregobj, "AFuncs");
			Assert::AreEqual(0, L.GetTop());
			L.GetGlobal("AFuncs");
			Assert::IsTrue(L.IsTable(1));
			L.GetTableRaw(1, "G");
			Assert::IsTrue(L.IsFunction(2));
			AssertRegex("<function C  AFuncs\\.G \\(defined in: C:0x[0-9a-f]+\\)>", L.ToDebugString(2));
			L.TCall(0, 1);
			Assert::IsTrue(L.IsNumber(2));
			Assert::AreEqual(5.0, *L.ToNumber(2));
			L.SetTop(1);
			L.GetTableRaw(1, "GC");
			Assert::IsTrue(L.IsFunction(2));
			L.TCall(0, 1);
			Assert::IsTrue(L.IsNumber(2));
			Assert::AreEqual(6.0, *L.ToNumber(2));
			L.SetTop(1);
		}

		template<class S>
		void Tables_T() {
			S L{};
			Assert::AreEqual(0, L.GetTop());

			L.NewTable();
			Assert::AreEqual(1, L.GetTop());
			Assert::IsTrue(L.IsTable(1));
			AssertRegex("<table 0x[0-9a-f]+>", L.ToDebugString(1));

			L.Push("a");
			L.Push(42);
			L.SetTable(1);
			Assert::AreEqual(1, L.GetTop());
			L.Push("a");
			L.GetTable(1);
			Assert::AreEqual(lua::Integer{ 42 }, L.CheckInteger(2));
			L.Pop(1);
			Assert::AreEqual(1, L.GetTop());

			L.Push("a");
			L.Push(43);
			L.SetTableRaw(1);
			Assert::AreEqual(1, L.GetTop());
			L.Push("a");
			L.GetTableRaw(1);
			Assert::AreEqual(lua::Integer{ 43 }, L.CheckInteger(2));
			L.Pop(1);
			Assert::AreEqual(1, L.GetTop());

			L.Push(44);
			L.SetTableRaw(1, "a");
			Assert::AreEqual(1, L.GetTop());
			L.GetTableRaw(1, "a");
			Assert::AreEqual(lua::Integer{ 44 }, L.CheckInteger(2));
			L.Pop(1);
			Assert::AreEqual(1, L.GetTop());

			L.Push("a");
			L.SetTableRaw(1, 1);
			L.GetTableRaw(1, 1);
			Assert::AreEqual("a", L.CheckString(2));
			L.Pop(1);
			Assert::AreEqual(1, L.GetTop());

			Assert::IsFalse(L.GetMetatable(1));
			L.NewTable();
			L.PushValue(2);
			Assert::IsTrue(L.SetMetatable(1));
			Assert::IsTrue(L.GetMetatable(1));
			Assert::AreEqual(L.ToPointer(2), L.ToPointer(3));
			Assert::IsTrue(L.RawEqual(2, 3));
			L.SetTop(0);

			L.Push("abc");
			L.SetGlobal("xyz");
			Assert::AreEqual(0, L.GetTop());
			L.GetGlobal("xyz");
			Assert::AreEqual(1, L.GetTop());
			Assert::AreEqual("abc", L.ToString(1));
			L.SetTop(0);

			L.GetSubTable("glob");
			Assert::AreEqual(1, L.GetTop());
			Assert::IsTrue(L.IsTable(1));
			L.GetSubTable("foo", 1);
			Assert::AreEqual(2, L.GetTop());
			Assert::IsTrue(L.IsTable(2));
			L.GetTableRaw(1, "foo");
			Assert::IsTrue(L.RawEqual(2, 3));
			L.Pop(2);
			L.GetGlobal("glob");
			Assert::IsTrue(L.RawEqual(1, 2));
			L.SetTop(0);

			L.DoStringT("local t = {5,6,7}; setmetatable(t, {__len=function() return 5; end}); return t;");
			Assert::AreEqual(3u, L.RawLength(1));

			if constexpr (S::Capabilities::MetatableLengthModulo) {
				L.ObjLength(1);
				Assert::AreEqual(S::Capabilities::MetatableLengthOnTables ? 5 : 3, L.CheckInt(2));
			}
			L.SetTop(0);

			L.DoStringT("local t = {}; t.t = {t=t}; return t;");
			AssertRegex("\\{\n\t\\[\"t\"\\] = \\{\n\t\t\\[\"t\"\\] = <table, recursion 0x[0-9a-f]+>,\n\t\\},\n\\}", L.ToDebugString(1, 10));
		}
		TEST_METHOD(Tables) {
			Tables_T<lua::UniqueState>();
		}

		template<class S>
		void Operators_T() {
			try {
				lua::UniqueState L{};
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
				Assert::AreEqual(lua::Integer{ 15 }, L.CheckInteger(2));

				L.Arithmetic(lua::ArihmeticOperator::Subtract);
				Assert::AreEqual(lua::Integer{ -10 }, L.CheckInteger(1));

				L.Push(2);
				L.Arithmetic(lua::ArihmeticOperator::Multiply);
				Assert::AreEqual(lua::Integer{ -20 }, L.CheckInteger(1));

				L.Push(2);
				L.Arithmetic(lua::ArihmeticOperator::Divide);
				Assert::AreEqual(lua::Integer{ -10 }, L.CheckInteger(1));

				L.Arithmetic(lua::ArihmeticOperator::UnaryNegation);
				Assert::AreEqual(lua::Integer{ 10 }, L.CheckInteger(1));

				L.Push(4);
				L.Arithmetic(lua::ArihmeticOperator::Modulo);
				Assert::AreEqual(lua::Integer{ 2 }, L.CheckInteger(1));

				L.Push(4);
				L.Arithmetic(lua::ArihmeticOperator::Pow);
				Assert::AreEqual(lua::Integer{ 16 }, L.CheckInteger(1));
				L.Pop(1);

				if constexpr (S::Capabilities::NativeIntegers) {
					L.Push(4);
					L.Push(6);
					L.Arithmetic(lua::ArihmeticOperator::BitwiseAnd);
					Assert::AreEqual(lua::Integer{ 4 }, L.CheckInteger(1));

					L.Push(2);
					L.Arithmetic(lua::ArihmeticOperator::BitwiseOr);
					Assert::AreEqual(lua::Integer{ 6 }, L.CheckInteger(1));

					L.Push(2);
					L.Arithmetic(lua::ArihmeticOperator::BitwiseXOr);
					Assert::AreEqual(lua::Integer{ 4 }, L.CheckInteger(1));

					L.Push(2);
					L.Arithmetic(lua::ArihmeticOperator::ShiftLeft);
					Assert::AreEqual(lua::Integer{ 4 << 2 }, L.CheckInteger(1));

					L.Push(2);
					L.Arithmetic(lua::ArihmeticOperator::ShiftRight);
					Assert::AreEqual(lua::Integer{ 4 }, L.CheckInteger(1));

					L.Arithmetic(lua::ArihmeticOperator::BitwiseNot);
					Assert::AreEqual(~lua::Integer{ 4 }, L.CheckInteger(1));
					L.Pop(1);
				}

				L.Push(16);
				L.Push("abc");
				L.Concat(2);
				Assert::AreEqual("16abc", L.CheckString(1));
			}
			catch (const lua::LuaException& e) {
				auto m = ToString(e.what());
				Assert::Fail(m.c_str());
			}
		}
		TEST_METHOD(Operators) {
			Operators_T<lua::UniqueState>();
		}

		template<class S>
		void RunLua_T() {
			S L{};
			Assert::AreEqual(0, L.GetTop());

			L.DoStringT("function fool(a, b) return a+b end");
			Assert::AreEqual(0, L.GetTop());
			L.GetGlobal("fool");
			Assert::IsTrue(L.IsFunction(1));
			Assert::IsFalse(L.IsCFunction(1));
			if (L.Version() == 5.0)
				Assert::AreEqual(std::string{ "<function Lua global fool (defined in: [string \"function fool(a, b) return a+b end\"]:1)>" }, L.ToDebugString(1));
			else
				Assert::AreEqual(std::string{ "<function Lua  fool (defined in: [string \"function fool(a, b) return a+b end\"]:1)>" }, L.ToDebugString(1));

			L.Push(5);
			L.Push(42);
			L.Push("abc");
			L.TCall(3, 1);
			Assert::AreEqual(lua::Integer{ 5 + 42 }, L.CheckInteger(1));
			L.Pop(1);

			L.RegisterFuncs(toRegister);
			L.DoStringT("return foo(6, 666)");
			Assert::AreEqual(lua::Integer{ 6 + 42 }, L.CheckInteger(1));
			L.SetTop(0);

			Assert::IsTrue(L.DoString("error('number is 6')") != lua::ErrorCode::Success);
			Assert::IsTrue(L.CheckStringView(1).find("number is 6") != std::string_view::npos);

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

			L.SetTop(0);
			L.GetGlobal("fool");
			Assert::IsTrue(L.IsFunction(1));
			Assert::IsFalse(L.IsCFunction(1));
			auto f = L.Dump();
			Assert::IsFalse(f.empty());
			L.SetTop(0);

			Assert::IsTrue(L.LoadBuffer(f.c_str(), f.length(), nullptr) == lua::ErrorCode::Success);
			L.Push(7);
			L.Push(42);
			L.Push("abc");
			L.TCall(3, 1);
			Assert::AreEqual(lua::Integer{ 7 + 42 }, L.CheckInteger(1));
			L.SetTop(0);

			if constexpr (S::Capabilities::Environments) {
				L.DoString("glob = 5; function aglob() return glob; end; return aglob;");
				Assert::IsTrue(L.IsFunction(1));
				L.PushValue(1);
				L.TCall(0, 1);
				Assert::AreEqual(5.0, *L.ToNumber(2));
				L.Pop(1);

				L.GetEnvironment(1);
				L.PushGlobalTable();
				Assert::IsTrue(L.RawEqual(2, 3));
				L.Pop(2);

				L.NewTable();
				L.Push(42);
				L.SetTableRaw(2, "glob");
				L.SetEnvironment(1);
				L.TCall(0, 1);
				Assert::AreEqual(42.0, *L.ToNumber(1));
				L.SetTop(0);
			}
		}
		TEST_METHOD(RunLua) {
			RunLua_T<lua::UniqueState>();
		}

		TEST_METHOD(Userdata) {
			lua::UniqueState L{};
			Assert::AreEqual(0, L.GetTop());

			L.NewUserData<IntHolderOp>(5);
			L.SetGlobal("i");
			L.NewUserData<IntHolderOp>(7);
			AssertRegex("class LuappDev::IntHolderOp: 0x[0-9a-f]+", L.ConvertToString(1));
			AssertRegex("<Userdata class LuappDev::IntHolderOp 0x[0-9a-f]+>", L.ToDebugString(1));
			L.Pop(1);
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
				if constexpr (lua::State::Capabilities::NativeIntegers) {
					L.DoStringT("j:Set(4); i:Set(2); local k=i+j; k:Set(6);\n"
						"assert((i&k):Get()==2);\n"
						"assert((i|j):Get()==6);\n"
						"assert((i~k):Get()==4);\n"
						"assert((~i):Get()==~2);\n"
						"assert((j<<i):Get()==16);\n"
						"assert((j>>i):Get()==1);\n");
				}
			}
			catch (const lua::LuaException& e) {
				auto m = ToString(e.what());
				Assert::Fail(m.c_str());
			}

			L.NewUserData<IntHolderLua>(5);
			L.SetGlobal("i");
			L.NewUserData<IntHolderLua>(7);
			Assert::AreEqual(std::string_view{"IntHolder 7"}, L.ConvertToString(1));
			L.Pop(1);
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
				if constexpr (lua::State::Capabilities::MetatableLengthModulo) {
					L.DoStringT("local a = i+j; a.i=4;\n"
						"assert((j%a).i==2);\n"
						"assert(#j==2)\n");
				}
				if constexpr (lua::State::Capabilities::NativeIntegers) {
					L.DoStringT("assert((j//i):Get()==3);\n"
						"j:Set(4); i:Set(2); local k=i+j; k:Set(6);\n"
						"assert((i&k):Get()==2);\n"
						"assert((i|j):Get()==6);\n"
						"assert((i~k):Get()==4);\n"
						"assert((~i):Get()==~2);\n"
						"assert((j<<i):Get()==16);\n"
						"assert((j>>i):Get()==1);\n");
				}
			}
			catch (const lua::LuaException& e) {
				auto m = ToString(e.what());
				Assert::Fail(m.c_str());
			}

			bool closed = false;
			{
				lua::UniqueState L2{};
				L2.NewUserData<DtorTest>([&closed]() { closed = true; });
				// closing the state forces everything to get gcd
			}
			Assert::IsTrue(closed);
		}

		TEST_METHOD(Iterate) {
			lua::UniqueState L{};
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
				Assert::AreEqual(lua::Integer{ k + 4 }, L.CheckInteger(-1));
				++i;
			}
			Assert::AreEqual(2, i);
			Assert::AreEqual(1, L.GetTop());
		}

		static int black_magic(lua::State L) {
			lua::DebugInfo i{};
			Assert::AreEqual(3, L.Debug_GetStackDepth()); // black_magic, foo, main chunk
			Assert::IsTrue(L.Debug_GetStack(1, i, lua::DebugInfoOptions::Name, false));
			Assert::AreEqual("foo", i.Name);
			Assert::AreEqual(std::string_view{ "foo" }, std::string_view{ L.Debug_GetNameForStackFunc(1) });
			int num = 0;
			for (const auto& l : L.Debug_Locals(1)) {
				if (l.Name == std::string_view{ "l" }) {
					Assert::AreEqual(lua::Integer{ 1 }, L.CheckInteger(-1));
					L.Push(42);
					Assert::AreEqual("l", L.Debug_SetLocal(1, l.LocalNum));
					++num;
				}
				else if (l.Name == std::string{ "x" }) {
					Assert::AreEqual(lua::Integer{ 5 }, L.CheckInteger(-1));
					++num;
				}
				else if (l.Name == std::string{ "y" }) {
					Assert::AreEqual(lua::Integer{ 6 }, L.CheckInteger(-1));
					++num;
				}
				L.Pop(1);
			}
			Assert::AreEqual(3, num);
			L.Debug_PushDebugInfoFunc(i);
			for (const auto& u : L.Debug_Upvalues(-1)) {
				if (u.Name == std::string_view{ "upv" }) {
					Assert::AreEqual(lua::Integer{ 0 }, L.CheckInteger(-1));
					L.Push(4242);
					Assert::AreEqual("upv", L.Debug_SetUpvalue(-3, u.UpvalNum));
					++num;
				}
				else if (u.Name == std::string{ "u2" }) {
					Assert::AreEqual(lua::Integer{ 4 }, L.CheckInteger(-1));
					++num;
				}
				else if (u.Name == std::string{ "u3" }) {
					Assert::AreEqual(lua::Integer{ 5 }, L.CheckInteger(-1));
					++num;
				}
				L.Pop(1);
			}
			Assert::AreEqual(6, num);
			return 0;
		}
		TEST_METHOD(LocalAcess) {
			lua::UniqueState L{};
			Assert::AreEqual(0, L.GetTop());
			Assert::AreEqual(0, L.Debug_GetStackDepth());

			L.Push("black_magic");
			L.Push<black_magic>();
			L.SetGlobal();

			L.DoStringT("local upv = 0; local u2=4; local u3=5; function foo() local y = 6; local l = 1; local x = 5; black_magic(); u3,u2=u2,u3; return l,upv; end; local a,b = foo(); return a,b;");
			Assert::AreEqual(2, L.GetTop());
			Assert::AreEqual(lua::Integer{ 42 }, L.CheckInteger(1));
			Assert::AreEqual(lua::Integer{ 4242 }, L.CheckInteger(2));
		}

		static void hook_magic(lua::State L, lua::ActivationRecord ar) {
			if (ar.Matches(lua::HookEvent::Return)) {
				auto inf = L.Debug_GetInfoFromAR(ar, lua::DebugInfoOptions::Name);
				if (inf.Name && inf.Name == std::string_view{ "bar" }) {
					L.Debug_GetStack(1, inf, lua::DebugInfoOptions::Name, false);
					Assert::AreEqual("foo", inf.Name);
					for (const auto& l : L.Debug_Locals(inf)) {
						if (l.Name == std::string_view{ "l" }) {
							Assert::AreEqual(lua::Integer{ 1 }, L.CheckInteger(-1));
							L.Push(42);
							Assert::AreEqual("l", L.Debug_SetLocal(1, l.LocalNum));
						}
						L.Pop(1);
					}
					L.Debug_PushDebugInfoFunc(inf);
					for (const auto& u : L.Debug_Upvalues(-1)) {
						if (u.Name == std::string_view{ "upv" }) {
							Assert::AreEqual(lua::Integer{ 0 }, L.CheckInteger(-1));
							L.Push(4242);
							Assert::AreEqual("upv", L.Debug_SetUpvalue(-3, u.UpvalNum));
						}
						L.Pop(1);
					}
					L.Pop(1);
				}
			}
		}

		template<class S>
		void Hook_T() {
			S L{};
			Assert::AreEqual(0, L.GetTop());

			if constexpr (S::Capabilities::JIT)
				L.SetJITMode(S::JITMode::On);

			L.Debug_SetHook<hook_magic>(lua::HookEvent::Call | lua::HookEvent::Return, 0);

			L.DoStringT("local upv = 0; function bar() end; function foo() local l = 1; bar(); return l,upv; end; local a,b = foo(); return a,b;");
			Assert::AreEqual(2, L.GetTop());
			Assert::AreEqual(lua::Integer{ 42 }, L.CheckInteger(1));
			Assert::AreEqual(lua::Integer{ 4242 }, L.CheckInteger(2));
		}
		TEST_METHOD(Hook) {
			Hook_T<lua::UniqueState>();
		}

		static void hook_checkline(lua::State L, lua::ActivationRecord ar) {
			int t = L.GetTop();
			if (ar.Matches(lua::HookEvent::Line) && L.Debug_GetNameForStackFunc(0) == "test") {
				L.PushLightUserdata(&hook_checkline);
				L.GetTableRaw(L.REGISTRYINDEX);
				auto* exp = static_cast<int*>(L.ToUserdata(-1));
				Assert::AreEqual(*exp, ar.Line());
				++*exp;
			}
			L.SetTop(t);
		}
		static int activate_hookline(lua::State L) {
			L.Debug_SetHook<hook_checkline>(lua::HookEvent::Line | lua::HookEvent::Call | lua::HookEvent::Return, 0);
			return 0;
		}
		TEST_METHOD(HookLine) {
			lua::UniqueState L{};
			Assert::AreEqual(0, L.GetTop());

			int exp = L.Version() == 5.0 ? 8 : 7;

			L.PushLightUserdata(&hook_checkline);
			L.PushLightUserdata(&exp);
			L.SetTableRaw(L.REGISTRYINDEX);
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
			Assert::AreEqual(13, exp);
		}
	};
}
