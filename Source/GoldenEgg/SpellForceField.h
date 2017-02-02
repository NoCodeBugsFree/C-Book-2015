// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Spell.h"
#include "SpellForceField.generated.h"

/**
 * 
 */
UCLASS()
class GOLDENEGG_API ASpellForceField : public ASpell
{
	GENERATED_BODY()
	
public:

	virtual void BeginPlay() override;

	ASpellForceField();

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

protected:

private:
	
	/**  A forcefield will deflect attacks  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* ForceBox;
};
