// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Object/PHWarriorSkill3Object.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "Engine/DamageEvents.h"


// Sets default values
APHWarriorSkill3Object::APHWarriorSkill3Object()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerSphere"));
	//TriggerSphere->InitSphereRadius(100.f);
	//TriggerSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	TriggerSphere->SetGenerateOverlapEvents(true);
	RootComponent = TriggerSphere;
	

	FireEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("FireEffect"));
	FireEffect->SetupAttachment(TriggerSphere);
	FireEffect->SetAutoActivate(false);

	FloorEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("FloorEffect"));
	FloorEffect->SetupAttachment(TriggerSphere);
	FloorEffect->SetAutoActivate(false);
	
	InitialLifeSpan = LifeTime;
}

void APHWarriorSkill3Object::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	TriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &APHWarriorSkill3Object::OnTriggerOverlap);
	
}

void APHWarriorSkill3Object::InitializeSkill(float InDamage, AActor* InInstigator)
{
	SkillInstigator = InInstigator;
	Damage = InDamage;
}

void APHWarriorSkill3Object::OnTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	if (!HasAuthority())
	{
		return;
	}

	if (OtherActor && OtherActor != this)
	{
		UE_LOG(LogTemp, Log, TEXT("오브젝트 충돌: %s"), *OtherActor->GetName());
		// 데미지 처리
		FDamageEvent DamageEvent;
		OtherActor->TakeDamage(Damage, DamageEvent, SkillInstigator->GetInstigatorController(), this);
	}
}

// Called when the game starts or when spawned
void APHWarriorSkill3Object::BeginPlay()
{
	Super::BeginPlay();

	if (FloorEffect)
	{
		FloorEffect->Activate(true);
	}
	if (FireEffect)
	{
		FireEffect->Activate(true);
	}
}

// Called every frame
void APHWarriorSkill3Object::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

