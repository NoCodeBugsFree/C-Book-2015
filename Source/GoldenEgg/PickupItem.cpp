// Fill out your copyright notice in the Description page of Project Settings.

#include "GoldenEgg.h"
#include "PickupItem.h"
#include "Avatar.h"
#include "MyHUD.h"
#include "Spell.h"

// Sets default values
APickupItem::APickupItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pickup Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetSimulatePhysics(true);

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	CollisionSphere->SetSphereRadius(200.f);
	CollisionSphere->SetupAttachment(RootComponent);
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &APickupItem::OnOverlapBegin);
	//CollisionSphere->SetCollisionObjectType(ECC_Pawn);

	Icon = CreateDefaultSubobject<UTexture2D>(TEXT("Icon"));
	Name = "UNKNOWN_ITEM";
	Quantity = 0;
}

// Called when the game starts or when spawned
void APickupItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickupItem::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void APickupItem::OnOverlapBegin_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	if ((OtherActor != nullptr) && (OtherActor != this))
	{
		// Return if not a Avatar
		AAvatar* Avatar = Cast<AAvatar>(OtherActor);
		if (Avatar)
		{
			APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
			if (PlayerController)
			{
				AMyHUD* MyHUD = Cast<AMyHUD>(PlayerController->GetHUD());
				if (MyHUD)
				{
					FString MessageToSet = FString("Picked up ") + FString::FromInt(Quantity) + FString(" ") + Name;
					FMessage Msg = FMessage(Icon, MessageToSet, 5.f, FColor::Red);
					MyHUD->AddMessage(Msg);

					Avatar->Pickup(this);

					Destroy();
				}
			}
		}
	}
}

