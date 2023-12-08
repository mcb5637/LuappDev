#include "pch.h"
#include "CppUnitTest.h"

#include "luapp/luapp50.h"

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
	TEST_CLASS(LuappDev)
	{
	public:
		TEST_METHOD(CreateDelete)
		{
			lua::State L{};

			lua::StateCloser closer{ L };

			Assert::AreEqual(L.GetState(), closer.GetState().GetState());
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

			Assert::AreEqual(1, L.CheckInt(1));
			Assert::IsTrue(L.CheckBool(2));

			L.Push();
			L.Insert(2);
			Assert::AreEqual(3, L.GetTop());
			Assert::AreEqual(1, L.CheckInt(1));
			Assert::IsTrue(L.IsNil(2));
			Assert::IsTrue(L.CheckBool(3));

			L.Push("");
			L.Replace(2);
			Assert::AreEqual(3, L.GetTop());
			Assert::AreEqual(1, L.CheckInt(1));
			Assert::AreEqual("", L.CheckString(2));
			Assert::IsTrue(L.CheckBool(3));

			L.Copy(1, 2);
			Assert::AreEqual(3, L.GetTop());
			Assert::AreEqual(1, L.CheckInt(1));
			Assert::AreEqual(1, L.CheckInt(2));
			Assert::IsTrue(L.CheckBool(3));

			L.Remove(2);
			Assert::AreEqual(2, L.GetTop());
			Assert::AreEqual(1, L.CheckInt(1));
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
			Assert::AreEqual(1, L.CheckInt(1));
			Assert::AreEqual(1.0, L.CheckNumber(1));
			Assert::IsTrue(L.Type(1) == lua::LType::Number);
			L.SetTop(0);

			L.Push(1.5);
			Assert::IsTrue(L.IsNumber(1));
			Assert::AreEqual(1, L.CheckInt(1));
			Assert::AreEqual(1.5, L.CheckNumber(1));
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
			Assert::AreEqual(42 + 5, L.CheckInt(1));
			L.SetTop(0);

			L.Push<foo>();
			Assert::IsTrue(L.IsFunction(1));
			Assert::IsTrue(L.IsCFunction(1));
			Assert::IsTrue(L.Type(1) == lua::LType::Function);

			L.Push(5);
			L.TCall(1, 1);
			Assert::AreEqual(42 + 5, L.CheckInt(1));
			L.SetTop(0);

			L.Push<foo>();

			L.Push(5);
			L.Push("bar");
			L.TCall(2, 2);
			Assert::AreEqual(42 + 5, L.CheckInt(1));
			Assert::IsTrue(L.IsNil(2));
			L.SetTop(0);
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
			Assert::AreEqual(42, L.CheckInt(2));
			L.Pop(1);

			L.Push("a");
			L.Push(43);
			L.SetTableRaw(1);
			L.Push("a");
			L.GetTableRaw(1);
			Assert::AreEqual(43, L.CheckInt(2));
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
			Assert::AreEqual(15, L.CheckInt(2));

			L.Arithmetic(lua::ArihmeticOperator::Subtract);
			Assert::AreEqual(-10, L.CheckInt(1));

			L.Push(2);
			L.Arithmetic(lua::ArihmeticOperator::Multiply);
			Assert::AreEqual(-20, L.CheckInt(1));

			L.Push(2);
			L.Arithmetic(lua::ArihmeticOperator::Divide);
			Assert::AreEqual(-10, L.CheckInt(1));

			L.Arithmetic(lua::ArihmeticOperator::UnaryNegation);
			Assert::AreEqual(10, L.CheckInt(1));

			L.Push(4);
			L.Arithmetic(lua::ArihmeticOperator::Modulo);
			Assert::AreEqual(2, L.CheckInt(1));

			L.Push(4);
			L.Arithmetic(lua::ArihmeticOperator::Pow);
			Assert::AreEqual(16, L.CheckInt(1));

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
			Assert::AreEqual(5 + 42, L.CheckInt(1));
			L.Pop(1);

			L.RegisterFuncs(toRegister);
			L.DoStringT("return foo(6, 666)");
			Assert::AreEqual(6 + 42, L.CheckInt(1));
		}
	};
}
