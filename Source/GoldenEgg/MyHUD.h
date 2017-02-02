// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

class ASpell;
/**
 * 
 */

USTRUCT(BlueprintType)
struct FMessage {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA")
	FString Message = "BLANK MESSAGE";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA")
	float Time = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA")
	UTexture2D* Texture2D;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA")
	FColor Color = FColor::Red;

	FMessage()
	{
		Time = 5.f;
		Color = FColor::White;
	}

	FMessage(UTexture2D* Texture2DToSet, FString MessageToSet, float TimeToSet, FColor ColorToSet)
	{
		Texture2D = Texture2DToSet;
		Message = MessageToSet;
		Time = TimeToSet;
		Color = ColorToSet;
	}
};

USTRUCT(BlueprintType)
struct FMyIcon {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA")
	FString Name = "ICON NAME";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA")
	UTexture2D* Texture2D;

	FMyIcon() 
	{
		Name = "UNKNOWN ICON";
		Texture2D = nullptr;
	}

	FMyIcon(FString &NameToSet, UTexture2D* Texture2DToSet)
	{
		Name = NameToSet;
		Texture2D = Texture2DToSet;
	}

};

USTRUCT(BlueprintType)
struct FMyWidget {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA")
	TSubclassOf<ASpell> BP_Spell;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA")
	FMyIcon Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA")
	FVector2D Position;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA")
	FVector2D Size;

	FMyWidget()
	{
		Icon = FMyIcon();
	}

	FMyWidget(FMyIcon IconToSet) 
	{
		Icon = IconToSet;
	}

	float Left() { return Position.X; }
	float Right() { return Position.X + Size.X; }
	float Top() { return Position.Y; }
	float Bottom() { return Position.Y + Size.Y; }

	bool HitScreen(FVector2D P)
	{
		// +--+ top (0)
		// |  | 
		// +--+ bottom (2) (bottom > top)
		// L  R
		return P.X > Left() && P.X < Right() && P.Y > Top() && P.Y < Bottom();
	}
};


UCLASS()
class GOLDENEGG_API AMyHUD : public AHUD
{
	GENERATED_BODY()
	
public:

	/** Add message to Array */
	UFUNCTION()
	void AddMessage(FMessage Message);

	/** Clear All Widgets */
	UFUNCTION()
	void ClearWidgets();

	/** Add new Widget */
	UFUNCTION()
	void AddWidget(FMyWidget Widget);

	/** Mouse Clicked on Widget */
	UFUNCTION()
	void MouseClicked();

	/** Mouse Clicked on Widget */
	UFUNCTION()
	void MouseMoved();

	/** Mouse Right Clicked on Widget */
	UFUNCTION()
	void MouseRightClicked();

protected:



private:
	//
	/** Screen Dimension  */
	FVector2D Dims;

	/** The Widgets  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	TArray<FMyWidget> Widgets;

	/** The list of messages  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	TArray<FMessage> Messages;
	
	/** The font used to render the text to the HUD.  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	UFont* HUDFont;

	/** Hold The last touched Widget in memory  */
	FMyWidget* HeldWidget;

	/** Add this function to be able to draw to the HUD  */
	virtual void DrawHUD() override;

	/** Draw Messages  */
	UFUNCTION()
	void DrawMessages();

	/** Draw Health Bar */
	UFUNCTION()
	void DrawHealthBar();

	/** Draw Widgets */
	UFUNCTION()
	void DrawWidgets();

};
