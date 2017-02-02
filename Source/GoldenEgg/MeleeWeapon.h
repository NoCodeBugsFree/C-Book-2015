// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MeleeWeapon.generated.h"

class AMonster;

UCLASS()
class GOLDENEGG_API AMeleeWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMeleeWeapon();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AAA")
	void Prox(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void Swing();

	void SetWeaponHolder(AMonster* WeaponHolderToSet);

	void OnSwordHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:


private:

	/** The amount of damage attacks by this weapon do */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	float AttackDamage;

	/** A list of things the melee weapon already hit this swing  Ensures each thing sword passes thru only gets hit once*/
	TArray<AActor*> ThingsHit;

	/**  prevents damage from occurring in frames where  the sword is not swinging */
	bool bIsSwinging;

	/** "Stop hitting yourself" - used to check if the actor holding the weapon is hitting himself  */
	AMonster* WeaponHolder;

	/** bounding box that determines when melee weapon hit  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxComponent;
	
	/** Weapon mesh  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* WeaponMesh;

	

	void Rest();

};
