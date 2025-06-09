#pragma once

#include "CoreMinimal.h"
#include "PHUInt8.generated.h"


USTRUCT(BlueprintType)
struct FPHUInt8
{
GENERATED_BODY()

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
    uint8 Value;
public:
    DECLARE_DELEGATE_TwoParams(FOnValueChanged, const uint8& /*OldValue*/, const uint8& /*NewValue*/);
    FOnValueChanged OnValueChanged;

    FPHUInt8() : Value(0)
    {
    }

    explicit FPHUInt8(uint8 In) : Value(In)
    {
    }

    void SetValue(const uint8& NewValue)
    {
        if (Value != NewValue)
        {
            uint8 OldValue = Value;
            Value = NewValue;
            OnValueChanged.ExecuteIfBound(OldValue, NewValue);
        }
    }

    const uint8& GetValue() const
    {
        return Value;
    }

    FPHUInt8& operator=(const uint8& NewValue)
    {
        SetValue(NewValue);
        return *this;
    }

    operator const uint8&() const
    {
        return Value;
    }
};
