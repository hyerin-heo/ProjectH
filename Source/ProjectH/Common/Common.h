#pragma once

#include "CoreMinimal.h"

#pragma  region RestAPI

#define API_URL_CREATE_ROOM     TEXT("rooms")
#define API_URL_QUICK_MATCH    TEXT("rooms/quick-match")
#define API_URL_SEARCH_ROOM    TEXT("rooms")
#define API_URL_JOIN_ROOM      TEXT("rooms/%s/join")
#define API_URL_START_GAME      TEXT("rooms/%s/start-game")
#define API_URL_DELETE_ROOM    TEXT("rooms/%s")

#pragma endregion 


#define DEFINE_CUSTOM_VARIABLE_STRUCT(T, Name) \
USTRUCT(BlueprintType) \
struct FPH##Name \
{ \
GENERATED_BODY() \
\
UPROPERTY() \
T Value; \
DECLARE_DELEGATE_TwoParams(FOnValueChanged, const T& /*OldValue*/, const T& /*NewValue*/); \
FOnValueChanged OnValueChanged; \
\
FPH##Name() : Value(0) {} \
explicit FPH##Name(T In) : Value(In) {} \
\
void SetValue(const T& NewValue) \
{ \
	if (Value != NewValue) \
	{ \
		T OldValue = Value; \
		Value = NewValue; \
		OnValueChanged.ExecuteIfBound(OldValue, NewValue); \
	} \
} \
const T& GetValue() const \
{ \
	return Value; \
} \
FPH##Name& operator=(const T& NewValue) \
{ \
	SetValue(NewValue); \
	return *this; \
} \
operator const T& () const \
{ \
	return Value; \
} \
};

// DEFINE_CUSTOM_VARIABLE_STRUCT(uint8, UInt8)
// DEFINE_CUSTOM_VARIABLE_STRUCT(float, Float)


