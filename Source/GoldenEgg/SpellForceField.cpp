// Fill out your copyright notice in the Description page of Project Settings.

#include "GoldenEgg.h"
#include "SpellForceField.h"
#include "Monster.h"

void ASpellForceField::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(TimeAlive);
}

ASpellForceField::ASpellForceField()
{
	ForceBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Force Box"));
	ForceBox->SetupAttachment(RootComponent);
	TimeAlive = 3.f;
}

void ASpellForceField::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	TArray<UPrimitiveComponent*> EnemiesToPush;
	ForceBox->GetOverlappingComponents(EnemiesToPush);
	if (EnemiesToPush.Num())
	{
		for (int32 iCount = 0; iCount < EnemiesToPush.Num(); iCount++)
		{
			AMonster* Monster = Cast<AMonster>(EnemiesToPush[iCount]->GetOwner());
			if (Monster)
			{
				FVector PlayerLocation = UGameplayStatics::GetPlayerPawn(this, 0)->GetActorLocation();
				
				Monster->KnockBack = Monster->GetActorLocation() - PlayerLocation;
				Monster->KnockBack.Normalize();
				Monster->KnockBack *= 2;
				Monster->AddMovementInput(Monster->KnockBack, 1.f);
			}
		}
	}
}
