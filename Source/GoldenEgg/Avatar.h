// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Avatar.generated.h"

class APickupItem;
class ASpell;

UCLASS()
class GOLDENEGG_API AAvatar : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAvatar();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	float CurrentHealth = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	float MaxHealth = 100.f;

	// A map for the player's backpack
	TMap<FString, int> Backpack;

	// The icons for the items in the backpack, lookup by string
	TMap<FString, UTexture2D*> Icons;

	/** to remember the blueprint of the spell that an item casts, by item name  */
	TMap<FString, TSubclassOf<ASpell>> Spells;

	// a flag alerting us the UI is showing
	bool bIsInventoryShowing;

	UFUNCTION()
	void Pickup(APickupItem *Item);

	UFUNCTION()
	void MouseClicked();

	UFUNCTION()
	void MouseRightClicked();

	UFUNCTION()
	void CastSpell(TSubclassOf<ASpell> BP_Spell);

	/**  Arrow Component for Spell Casting */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA")
	UArrowComponent* SpellArrowComponent;
	
private:

	virtual float TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	FVector KnockBack;

	UFUNCTION()
	void ToggleInventory();

	UFUNCTION()
	void MoveForward(float Amount);

	UFUNCTION()
	void MoveRight(float Amount);

	UFUNCTION()
	void Yaw(float Amount);

	UFUNCTION()
	void Pitch(float Amount);
	
};
