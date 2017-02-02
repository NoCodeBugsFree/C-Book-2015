// Fill out your copyright notice in the Description page of Project Settings.

#include "GoldenEgg.h"
#include "Avatar.h"
#include "PickupItem.h"
#include "MyHUD.h"
#include "Spell.h"

// Sets default values
AAvatar::AAvatar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpellArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("Spell Arrow Component"));
	SpellArrowComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AAvatar::BeginPlay()
{
	Super::BeginPlay();
	

}

// Called every frame
void AAvatar::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	// apply knockback vector
	AddMovementInput(KnockBack, 1.f);

	// half the size of the knockback each frame
	KnockBack *= 0.5f;
}

// Called to bind functionality to input
void AAvatar::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	check(InputComponent);
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAxis("MoveForward", this, &AAvatar::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AAvatar::MoveRight);
	InputComponent->BindAxis("Turn", this, &AAvatar::Yaw);
	InputComponent->BindAxis("LookUp", this, &AAvatar::Pitch);

	InputComponent->BindAction("Inventory", IE_Pressed, this, &AAvatar::ToggleInventory);
	InputComponent->BindAction("MouseClickedLMB", IE_Pressed, this, &AAvatar::MouseClicked);
	InputComponent->BindAction("MouseClickedRMB", IE_Pressed, this, &AAvatar::MouseRightClicked);
}

void AAvatar::Pickup(APickupItem *Item)
{
	if (Item)
	{
		if (Backpack.Find(Item->Name))
		{
			// the item was already in the pack.. increase qty of it
			Backpack[Item->Name] += Item->Quantity;
		}
		else
		{
			// the item wasn't in the pack before, add it in now
			Backpack.Add(Item->Name, Item->Quantity);

			// record ref to the texture the first time it is picked up
			Icons.Add(Item->Name, Item->Icon);
		}

		// the spell associated with the item
		Spells.Add(Item->Name, Item->Spell);
	}
}

void AAvatar::MouseClicked()
{
	APlayerController * PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		AMyHUD* MyHUD = Cast<AMyHUD>(PlayerController->GetHUD());
		if (MyHUD)
		{
			MyHUD->MouseClicked();
		}
	}
}

void AAvatar::MouseRightClicked()
{
	if (bIsInventoryShowing)
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			AMyHUD* MyHUD = Cast<AMyHUD>(PlayerController->GetHUD());
			if (MyHUD)
			{
				MyHUD->MouseRightClicked();
			}
		}
	}
}

void AAvatar::CastSpell(TSubclassOf<ASpell> BP_Spell)
{
	if (BP_Spell)
	{
		// instantiate the spell and attach to character
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;
		
		FVector SpawnLocation = SpellArrowComponent->GetComponentLocation();
		FRotator SpawnRotation = SpellArrowComponent->GetComponentRotation();
					
		ASpell* Spell = GetWorld()->SpawnActor<ASpell>(BP_Spell, SpawnLocation, SpawnRotation, SpawnParams);
		if (Spell)
		{
			Spell->SetCaster(this);
		}
		else
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Yellow, FString("can't cast ") + BP_Spell->GetName());
			}
		}
	}
}

float AAvatar::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// Call the base class - this will tell us how much damage to apply  
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (ActualDamage > 0.f)
	{
		CurrentHealth -= ActualDamage;
		// If the damage depletes our health set our lifespan to zero - which will destroy the actor  
		if (CurrentHealth <= 0.f)
		{
			SetLifeSpan(0.001f);
		}
	}

	// add some knockback that gets applied over a few frames
	if (DamageCauser)
	{
		KnockBack = GetActorLocation() - DamageCauser->GetActorLocation();
		KnockBack.Normalize();
		KnockBack *= DamageAmount * 500; // knockback proportional to damage
	}

	return ActualDamage;
}

void AAvatar::ToggleInventory()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController)
	{
		AMyHUD* MyHUD = Cast<AMyHUD>(PlayerController->GetHUD());
		if (MyHUD)
		{
			// If inventory is displayed, undisplay it.
			if (bIsInventoryShowing)
			{
				MyHUD->ClearWidgets();
				bIsInventoryShowing = false;
				PlayerController->bShowMouseCursor = false;
				return;
			}
			// Otherwise, display the player's inventory
			else
			{
				bIsInventoryShowing = true;
				PlayerController->bShowMouseCursor = true;
				for (auto it = Backpack.CreateIterator(); it; ++it)
				{
					// Combine string name of the item, with qty eg Cow x 5
					FString Fs = it->Key + FString(" ") + FString::FromInt(it->Value);
					UTexture2D* Texture2D;
					if (Icons.Find(it->Key))
					{
						Texture2D = Icons[it->Key];
						FMyWidget MyWidget(FMyIcon(Fs, Texture2D));
						MyWidget.BP_Spell = Spells[it->Key];
						MyHUD->AddWidget(MyWidget);
					}
				}
			}
		}
	}
}

void AAvatar::MoveForward(float Amount)
{
	if (Controller && Amount && !bIsInventoryShowing)
	{
		AddMovementInput(GetActorForwardVector(), Amount);
	}
}

void AAvatar::MoveRight(float Amount)
{
	if (Controller && Amount && !bIsInventoryShowing)
	{
		AddMovementInput(GetActorRightVector(), Amount);
	}
}

void AAvatar::Yaw(float Amount)
{
	if (bIsInventoryShowing)
	{
		// when the inventory is showing 
		// pass the input to the HUD
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			AMyHUD* MyHUD = Cast<AMyHUD>(PlayerController->GetHUD());
			if (MyHUD)
			{
				MyHUD->MouseMoved();
			}
		}
		return;
	}
	if (Controller && Amount && !bIsInventoryShowing)
	{
		AddControllerYawInput(Amount * 50.f * GetWorld()->GetDeltaSeconds());
	}
}

void AAvatar::Pitch(float Amount)
{
	if (bIsInventoryShowing)
	{
		// when the inventory is showing 
		// pass the input to the HUD
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			AMyHUD* MyHUD = Cast<AMyHUD>(PlayerController->GetHUD());
			if (MyHUD)
			{
				MyHUD->MouseMoved();
			}
		}
		return;
	}
	if (Controller && Amount && !bIsInventoryShowing)
	{
		AddControllerPitchInput(Amount * 50.f * GetWorld()->GetDeltaSeconds());
	}
}

