#pragma once

#include "BucketFNames.h"
#include "ArrayTypes.h"
#include <map>

namespace UnrealEngine4
{
	#pragma pack(1)
	class UObject
	{
	public:
		__int32 Flags; //0x0008
		__int32 Index; //0x000C
		class UClass* Class; //0x0010
		FName Name; //0x0018
		UObject* Outer; //0x0020

		// haven't mapped vfuncs yet, but declaring them so vtable will be sized in.
		virtual void Function0();
		virtual void Function1();
		virtual void Function2();
		virtual void Function3();
		virtual void Function4();
		virtual void Function5();
		virtual void Function6();
		virtual void Function7();
		virtual void Function8();
		virtual void Function9();

		FString GetFullName(const UObject* StopOuter);
		UObject* GetTypedOuter(const class UClass* targetClass);
		bool IsA(const class UClass* targetClass, bool verboseLogging = false);

		static class UClass* StaticClass();
		static class UClass* GetStaticClass(const char* className);

	private:
		static std::map<std::string, UClass*> nameToClassMap;

	}; //Size: 0x0028
	static_assert(sizeof(UObject) == 0x28);
}