// Fill out your copyright notice in the Description page of Project Settings.

#include "GoldenEgg.h"
#include "MeleeWeapon.h"
#include "Monster.h"

// Sets default values
AMeleeWeapon::AMeleeWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AttackDamage = 1;
	bIsSwinging = false;
	WeaponHolder = nullptr;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);
	


	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeapon::Prox);
	
}

// Called when the game starts or when spawned
void AMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();
	WeaponMesh->OnComponentHit.AddDynamic(this, &AMeleeWeapon::OnSwordHit);
}

// Called every frame
void AMeleeWeapon::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AMeleeWeapon::Swing()
{
	ThingsHit.Empty();
	bIsSwinging = true;
}

void AMeleeWeapon::SetWeaponHolder(AMonster* WeaponHolderToSet)
{
	WeaponHolder = WeaponHolderToSet;
}

void AMeleeWeapon::OnSwordHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Error, TEXT("OnSwordHit"));
	GEngine->AddOnScreenDebugMessage(0, 3, FColor::Red, FString("OnSwordHit"));
}

void AMeleeWeapon::Rest()
{
	ThingsHit.Empty();
	bIsSwinging = false;
}

void AMeleeWeapon::Prox_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// don't hit non root components
	if (OtherComp != OtherActor->GetRootComponent())
	{
		return;
	}

	// avoid hitting things while sword isn't swinging,
	// avoid hitting yourself, and
	// avoid hitting the same OtherActor twice
	if (bIsSwinging && OtherActor != WeaponHolder && !ThingsHit.Contains(OtherActor))
	{
		if (OtherActor)
		{
			if (WeaponHolder)
			{
				OtherActor->TakeDamage(AttackDamage + WeaponHolder->GetBaseAttackDamage(), FDamageEvent(), nullptr, this);
				ThingsHit.Add(this);
			}
		}
	}
}

