// Fill out your copyright notice in the Description page of Project Settings.

#include "GoldenEgg.h"
#include "Monster.h"
#include "Avatar.h"
#include "MeleeWeapon.h"
#include "PickupItem.h"
#include "Bullet.h"

// Sets default values
AMonster::AMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Speed = 20;
	HitPoints = 100;
	Experience = 0;
	BP_Loot = nullptr;
	BaseAttackDamage = 1;
	AttackTimeout = 1.5f;
	TimeSinceLastStrike = 0;

	SightSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sight Sphere"));
	SightSphere->SetupAttachment(RootComponent);
	SightSphere->SetSphereRadius(2500.f);

	SwordRangeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sword Range Sphere"));
	SwordRangeSphere->SetupAttachment(RootComponent);
	SwordRangeSphere->SetSphereRadius(200.f);

	BulletRangeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Bullet Range Sphere"));
	BulletRangeSphere->SetupAttachment(RootComponent);
	BulletRangeSphere->SetSphereRadius(1500.f);
}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMonster::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	KnockBack *= 0.5f;
	
	AAvatar* Avatar = Cast<AAvatar>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (Avatar)
	{
		// move the monster towards the player
		FVector PlayerLocation = Avatar->GetActorLocation();
		FVector ToPlayer = PlayerLocation - GetActorLocation();

		// FVector::Size() - The length of this vector.
		DistanceToPlayer = ToPlayer.Size();
		if (DistanceToPlayer > SightSphere->GetScaledSphereRadius())
		{
			return;
		}

		// normalizes the vector
		ToPlayer /= DistanceToPlayer;
		
		// At least face the target
		// Gets you the rotator to turn something
		// that looks in the `toPlayer` direction
		FRotator ToPlayerRotation = ToPlayer.Rotation();
		ToPlayerRotation.Pitch = 0; // off the Pitch
		RootComponent->SetWorldRotation(ToPlayerRotation);

		if (IsInSwordAttackRangeOfPlayer())
		{
			// Perform the attack
			if (!TimeSinceLastStrike)
			{
				//Attack(Avatar); // Amim BP doing this job
			}
			TimeSinceLastStrike += DeltaTime;
			if (TimeSinceLastStrike > AttackTimeout)
			{
				TimeSinceLastStrike = 0;
			}

			return;
		}

		else if (IsInSightRange())
		{
			// not in attack range, so walk towards player
			AddMovementInput(ToPlayer, Speed * DeltaTime);
		}
	}
	
}

// Called to bind functionality to input
void AMonster::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

void AMonster::SwordSwung()
{
	if (MeleeWeapon)
	{
		MeleeWeapon->Swing();
	}
}

float AMonster::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	
	// Call the base class - this will tell us how much damage to apply  
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (ActualDamage > 0.f)
	{
		HitPoints -= ActualDamage;
		// If the damage depletes our health set our lifespan to zero - which will destroy the actor  
		if (HitPoints <= 0.f)
		{
			DestroyMonster();
		}
	}
	return ActualDamage;
}

void AMonster::DestroyMonster_Implementation()
{
	// spawn loot
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;
	
	FVector SpawnLocation = GetMesh()->GetComponentLocation();
	SpawnLocation += FVector(0, 0, 200);
	FRotator SpawnRotation = GetMesh()->GetComponentRotation();

	APickupItem* Loot = GetWorld()->SpawnActor<APickupItem>(BP_Loot, SpawnLocation, SpawnRotation, SpawnParams);
	
	// play death animation

	// destroy weapon
	if (MeleeWeapon)
	{
		MeleeWeapon->Destroy();
	}
	// destroy self
	Destroy();
}

void AMonster::Attack(AActor* ActorToAttack)
{
	if (MeleeWeapon)
	{
		// code for the melee weapon swing, if
		// a melee weapon is used
		MeleeWeapon->Swing();
	}
	else if (BulletClass)
	{
		// If a blueprint for a bullet to use was assigned,
		// then use that. Note we wouldn't execute this code
		// bullet firing code if a MeleeWeapon was equipped
		FVector ForwardVector = GetActorForwardVector();
		FVector NozzleLocation = GetMesh()->GetBoneLocation("RightHand");

		// move it fwd of the monster so it
		// doesn't collide with the monster model
		NozzleLocation += ForwardVector * 155;
		FVector ToOpponent = ActorToAttack->GetActorLocation() - NozzleLocation;
		ToOpponent.Normalize();

		// SpawnActor
		FRotator SpawnRotation = RootComponent->GetComponentRotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		ABullet* Bullet = GetWorld()->SpawnActor<ABullet>(BulletClass, NozzleLocation, SpawnRotation, SpawnParams);

		if (Bullet)
		{
			Bullet->Firer = this;
			Bullet->GetSphereComponent()->AddImpulse(ForwardVector * BulletLaunchImpulse);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AMonster::Attack - ERROR HERE"));
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, "monster: no bullet actor could be spawned. is the bullet overlapping something ? ");

			}
		}
	}
}

void AMonster::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (MeleeWeaponClass)
	{
		MeleeWeapon = GetWorld()->SpawnActor<AMeleeWeapon>(MeleeWeaponClass, FVector(), FRotator());
		if (MeleeWeapon)
		{
			const USkeletalMeshSocket* Socket = GetMesh()->GetSocketByName("RightHandSocket");
			Socket->AttachActor(MeleeWeapon, GetMesh());
			MeleeWeapon->SetWeaponHolder(this);
			MeleeWeapon->SetOwner(this);
		}
	}
}

