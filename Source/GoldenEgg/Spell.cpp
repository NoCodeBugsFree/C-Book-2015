// Fill out your copyright notice in the Description page of Project Settings.

#include "GoldenEgg.h"
#include "Spell.h"
#include "Monster.h"


// Sets default values
ASpell::ASpell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// The Particles are the root component, and the ProxBox
	// is a child of the Particle system.
	// If it were the other way around, scaling the ProxBox
	// would also scale the Particles, which we don't want
	ParticleClass = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	SetRootComponent(ParticleClass);
	
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(RootComponent);

	Duration = 3.f;
	DamagePerSecond = 1.f;
	TimeAlive = 0.f;
}

// Called when the game starts or when spawned
void ASpell::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASpell::SetCaster(AActor* caster)
{
	Caster = caster;
	AttachToComponent(caster->GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
}

// Called every frame
void ASpell::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	// search the proxbox for all actors in the volume.
	TArray<AActor*> AllOverlapingActors;
	CollisionBox->GetOverlappingActors(AllOverlapingActors);

	// damage each actor the box overlaps
	for (int iCounter = 0; iCounter < AllOverlapingActors.Num(); iCounter++)
	{
		// don't damage the spell caster
		if (AllOverlapingActors[iCounter] != Caster)
		{
			// Only apply the damage if the box is overlapping
			// the actors ROOT component.
			// This way damage doesn't get applied for simply
			// overlapping the SightSphere of a monster
			AMonster* Monster = Cast<AMonster>(AllOverlapingActors[iCounter]);
			if (Monster && CollisionBox->IsOverlappingComponent(Monster->GetCapsuleComponent()))
			{
				Monster->TakeDamage(DamagePerSecond * DeltaTime, FDamageEvent(), 0, this);
			}

			// to damage other class types, try a checked cast here..

		}
		TimeAlive += DeltaTime;
		if (TimeAlive > Duration)
		{
			Destroy();
		}
	}

}

