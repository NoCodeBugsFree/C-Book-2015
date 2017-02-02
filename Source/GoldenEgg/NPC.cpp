// Fill out your copyright notice in the Description page of Project Settings.

#include "GoldenEgg.h"
#include "NPC.h"
#include "Avatar.h"
#include "MyHUD.h"
#include "PickupItem.h"


// Sets default values
ANPC::ANPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetupAttachment(RootComponent);
	CollisionSphere->SetSphereRadius(200.f);
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ANPC::OnOverlapBegin);
	
	NPCMessage = "Hi, I'm Owen!";
}

// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANPC::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void ANPC::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

void ANPC::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Other Actor is the actor that triggered the event. Check that is not ourself.  
	if ((OtherActor != nullptr) && (OtherActor != this))
	{
		 AAvatar* Avatar = Cast<AAvatar>(OtherActor);
		 if (Avatar)
		 {
			 // SpawnActor
			 FString PickupItemName;
			 auto World = GetWorld();
			 if (World)
			 {
				 FActorSpawnParameters SpawnParams;
				 SpawnParams.Owner = this;
				 SpawnParams.Instigator = Instigator;

				 FVector SpawnLocation;
				 FRotator SpawnRotation;

				 APickupItem* Spawned = World->SpawnActor<APickupItem>(PickupItem, SpawnLocation, SpawnRotation, SpawnParams);
				 if (Spawned)
				 {
					 PickupItemName = Spawned->GetPickUpName();
					 Avatar->Pickup(Spawned);
					 Spawned->Destroy();
				 }
			 }

			 APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
			 if (PlayerController)
			 {
				 AMyHUD* MyHUD = Cast<AMyHUD>(PlayerController->GetHUD());
				 if (MyHUD)
				 {
					 if (PickupItemName.Len() > 0)
					 {
						 NPCMessage = NPCMessage + TEXT(" I gave you ") + PickupItemName;
					 }

					 MyHUD->AddMessage(FMessage(Face, NPCMessage, 5.f, FColor::Red));
				 }
			 }

			 
		 }
	}
}

