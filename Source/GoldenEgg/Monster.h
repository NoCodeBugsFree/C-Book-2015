// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Monster.generated.h"

class AMeleeWeapon;
class ABullet;
class APickupItem;

UCLASS()
class GOLDENEGG_API AMonster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMonster();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	FORCEINLINE float GetBaseAttackDamage() { return BaseAttackDamage; }

	UFUNCTION(BlueprintCallable, Category = "AAA")
	bool IsInSwordAttackRangeOfPlayer() const {
		return DistanceToPlayer < SwordRangeSphere->GetScaledSphereRadius();
	}

	UFUNCTION(BlueprintCallable, Category = "AAA")
	bool IsInBulletAttackRangeOfPlayer() const {
		return DistanceToPlayer < BulletRangeSphere->GetScaledSphereRadius() && !IsInSwordAttackRangeOfPlayer();
	}

	UFUNCTION(BlueprintCallable, Category = "AAA")
	bool IsInSightRange() const {
		return DistanceToPlayer < SightSphere->GetScaledSphereRadius() && !IsInSwordAttackRangeOfPlayer();
	}

	UFUNCTION(BlueprintCallable, Category = "AAA")
	void SwordSwung();

	FVector KnockBack;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AAA")
	void DestroyMonster();
	void DestroyMonster_Implementation();

	UFUNCTION(BlueprintCallable, Category = "AAA")
	float GetHealthBarPercent() const { return HitPoints / 100.f; }

protected:



private:
	
	void Attack(AActor* ActorToAttack);

	/** The blueprint of the bullet class the monster uses  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ABullet> BulletClass;

	/** Thrust behind bullet launches  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	float BulletLaunchImpulse;

	float DistanceToPlayer;

	virtual void PostInitializeComponents() override;

	/** The MeleeWeapon class the monster uses If this is not set, he uses a melee attack */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMeleeWeapon> MeleeWeaponClass;

	/** The MeleeWeapon instance (set if the character is using a melee weapon  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	AMeleeWeapon* MeleeWeapon;

	/** Monster's Speed  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	float Speed;

	/** Monster's HP  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	float HitPoints = 100.f;

	/** Experience gained for defeating */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	int32 Experience;

	/** Blueprint of the type of item dropped by the monster */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APickupItem> BP_Loot;

	/** The amount of damage attacks do */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	float BaseAttackDamage;

	/** Amount of time the monster needs to rest in seconds between attacking*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	float AttackTimeout;

	/** Experience gained for defeating */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	float TimeSinceLastStrike;

	/** Range for his sight */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	USphereComponent* SightSphere;

	/** Range for his attack. Visualizes as a sphere in editor */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	USphereComponent* SwordRangeSphere;

	/** Range for his attack. Visualizes as a sphere in editor */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	USphereComponent* BulletRangeSphere;

};
