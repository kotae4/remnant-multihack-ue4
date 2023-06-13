#include "pch.h"
#include "UObject.h"
#include "UClass.h"
#include "BucketUObjects.h"
#include "../Offsets_UE4.h"
#include "../../../Utils/Logger.h"
#include "../../../Utils/Memory.h"

namespace UnrealEngine4
{
	FString UObject::GetFullName(const UObject* stopOuter)
	{
		FString retString;
		((void(__thiscall*)(void* object, FString* retString, const UObject* stopOuter))(Memory::ProcessBaseAddress + UOBJECT_GETFULLNAME_OFFSET))(this, &retString, stopOuter);
		return retString;
	}

	bool UObject::IsA(const UClass* targetClass, bool verboseLogging)
	{
		if (targetClass == nullptr)
			return false;
		if (targetClass->Super == nullptr)
			return false;
		// walk class hierarchy of current object
		try
		{
			for (UStruct* super = this->Class; super; super = super->Super)
			{
				if (super == nullptr)
					break;
				// compare class to targetClass
				if (verboseLogging)
					LOG_TRACEW(L"-----'{}'", super->GetFullName(NULL).c_str());
				if (super == targetClass)
				{
					return true;
				}
			}
		}
		catch (...) { LOG_ERROR("Exception caught on UObject->IsA (this: [{:#x}] target: [{:#x}])", (uintptr_t)this, (uintptr_t)targetClass); }
		return false;
	}

	UObject* UObject::GetTypedOuter(const UClass* targetClass)
	{
		if (targetClass == nullptr)
			return NULL;
		for (UObject* outer = this->Outer; outer; outer = outer->Outer)
		{
			if (outer == nullptr) break;

			if (outer->Class == targetClass)
				return outer;
			else if (outer->Class)
			{
				for (UStruct* super = outer->Class; super; super = super->Super)
				{
					if (super == nullptr) break;
					else if (super == targetClass)
					{
						return outer;
					}
				}
			}
		}
		return NULL;
	}

	UClass* UObject::StaticClass()
	{
		return UObject::GetStaticClass("Class /Script/CoreUObject.Object");
	}

	// Caches results to save time on subsequent calls
	std::map<std::string, UClass*> UObject::nameToClassMap;
	UClass* UObject::GetStaticClass(const char* className)
	{
		LOG_INFO("Attempting to get static class '{}'", className);
		if ((className == nullptr) || (className == "")) return NULL;
		std::map<std::string, UClass*>::iterator it = nameToClassMap.find(className);
		if (it == nameToClassMap.end())
		{
			LOG_INFO("--Iterating GObjects...");
			// not found in cache map
			for (int bucketIndex = 0; bucketIndex < GObjects->NumBuckets; bucketIndex++)
			{
				for (int entityIndex = 0; ((entityIndex < OBJECT_BUCKET_SIZE) && (((bucketIndex * OBJECT_BUCKET_SIZE) + entityIndex) < GObjects->NumObjects)); entityIndex++)
				{
					UObject* obj = GObjects->Buckets[bucketIndex][entityIndex].Object;
					if ((obj) && (obj->GetFullName(NULL).ToString().compare(className) == 0))
					{
						LOG_INFO("----Found static class in GObjects");
						// TO-DO:
						// check that insert succeeds (.second is bool succeeded)
						return (nameToClassMap.insert(std::map<std::string, UClass*>::value_type(std::string(className), ((UClass*)obj)))).first->second;
					}
				}
			}
		}
		else
		{
			LOG_INFO("--Found static class from cache");
			// found
			return (*it).second;
		}
		return NULL;
	}
}