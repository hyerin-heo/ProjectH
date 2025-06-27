// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/SkillObject/PHProjectileSkillObject.h"

#include "Net/UnrealNetwork.h"

APHProjectileSkillObject::APHProjectileSkillObject()
{
	// Init ProjectileMovementComponent
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComponent"));
	MovementComponent->UpdatedComponent = CollisionComponent;
	MovementComponent->InitialSpeed = 3000.f;
	MovementComponent->MaxSpeed = 3000.f;
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->bShouldBounce = false;
	MovementComponent->ProjectileGravityScale = 0.f;
	MovementComponent->SetIsReplicated(true); 
}

void APHProjectileSkillObject::BeginPlay()
{
	Super::BeginPlay();
	
	MovementComponent->StopMovementImmediately();
	MovementComponent->Deactivate();
}

void APHProjectileSkillObject::Launch(const FVector& Direction, float InDamage)
{
	SetActorTickEnabled(true);
	SetActorEnableCollision(true);
	Damage = InDamage;
	MovementComponent->Velocity = Direction * MovementComponent->InitialSpeed;
	MovementComponent->Activate();
	// 클라이언트에 액터 활성화 및 초기 이동 상태를 직접 알려주는 RPC 호출
	Client_ActivateSkillObject(GetActorLocation(), GetActorRotation(), MovementComponent->Velocity, InDamage, LifeSpan, bReturnToPoolOnHit);
}

void APHProjectileSkillObject::Launch(float InDamage, float InLifeTime)
{
	Init(InLifeTime);
	SetActorTickEnabled(true);
	SetActorEnableCollision(true);
	Damage = InDamage;
	// 클라이언트에 액터 활성화 및 초기 이동 상태를 직접 알려주는 RPC 호출
	Client_ActivateSkillObject(GetActorLocation(), GetActorRotation(), MovementComponent->Velocity, InDamage, LifeSpan, bReturnToPoolOnHit);
}

void APHProjectileSkillObject::Launch(float InDamage, float InLifeTime, float InEnableCollisionTime)
{
	Init(InLifeTime);
	SetActorTickEnabled(true);
	Damage = InDamage;
	// 클라이언트에 액터 활성화 및 초기 이동 상태를 직접 알려주는 RPC 호출
	Client_ActivateSkillObject(GetActorLocation(), GetActorRotation(), MovementComponent->Velocity, InDamage, LifeSpan, bReturnToPoolOnHit);
}

void APHProjectileSkillObject::Init(float InSpeed, float InLifeTime, bool ReturnToPoolOnHit)
{
	MovementComponent->InitialSpeed = InSpeed;
	MovementComponent->MaxSpeed = InSpeed * 2.0f;
	
	LifeSpan = InLifeTime;
	LifeSpanDeltaTime = 0.f;
	bReturnToPoolOnHit = ReturnToPoolOnHit;
}

void APHProjectileSkillObject::ResetProjectile()
{
	Super::ResetProjectile();
	MovementComponent->StopMovementImmediately();
	MovementComponent->Deactivate();
}

void APHProjectileSkillObject::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(APHProjectileSkillObject, MovementComponent);
}

void APHProjectileSkillObject::Client_ActivateSkillObject_Implementation(FVector InLocation, FRotator InRotation,
	FVector InVelocity, float InDamage, float InLifeTime, bool bInReturnToPoolOnHit)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		return;
	}
	Super::Client_ActivateSkillObject_Implementation(InLocation, InRotation, InVelocity, InDamage, InLifeTime,
	                                                 bInReturnToPoolOnHit);
	
	// 이동 컴포넌트 상태 동기화
	MovementComponent->InitialSpeed = InVelocity.Size();
	MovementComponent->MaxSpeed = InVelocity.Size() * 2.0f;
	MovementComponent->Velocity = InVelocity;
	MovementComponent->Activate(); // 이동 시작
}

void APHProjectileSkillObject::Client_ResetProjectile_Implementation()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		return;
	}
	Super::Client_ResetProjectile_Implementation();
	
	MovementComponent->StopMovementImmediately();
	MovementComponent->Deactivate();
}

void APHProjectileSkillObject::Init(float InLifeTime)
{
	MovementComponent->InitialSpeed = 0.0f;
	MovementComponent->MaxSpeed = 0.0f;
	MovementComponent->StopMovementImmediately();
	MovementComponent->Deactivate();
	
	LifeSpan = InLifeTime;
	LifeSpanDeltaTime = 0.f;
	bReturnToPoolOnHit = false;
}
