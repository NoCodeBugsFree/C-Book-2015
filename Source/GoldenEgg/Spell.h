// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Spell.generated.h"

UCLASS()
class GOLDENEGG_API ASpell : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpell();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Parents this spell to a caster actor
	void SetCaster(AActor* caster);

	// Runs each frame. override the Tick function to deal damage
	// to anything in ProxBox each frame.
	virtual void Tick(float DeltaSeconds) override;

	// Length of time the spell has been alive in the level
	float TimeAlive;

protected:



private:
	
	// box defining volume of damage
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* CollisionBox;

	// the particle visualization of the spell
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* ParticleClass;

	// How much damage the spell does per second
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	float DamagePerSecond = 1.f;

	// How long the spell lasts
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	float Duration = 5.f;

	// The original caster of the spell (so player doesn't hit self)
	UPROPERTY()
	AActor* Caster;
	
};
