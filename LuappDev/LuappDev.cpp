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
	};
}
