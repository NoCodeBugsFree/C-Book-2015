// Fill out your copyright notice in the Description page of Project Settings.

#include "GoldenEgg.h"
#include "MyHUD.h"
#include "Avatar.h"
#include "Spell.h"


void AMyHUD::DrawHUD()
{
	Super::DrawHUD();

	Dims.X = Canvas->SizeX;
	Dims.Y = Canvas->SizeY;

	DrawMessages();

	DrawHealthBar();

	DrawWidgets();
}

void AMyHUD::AddMessage(FMessage Message)
{
	Messages.Add(Message);
}

void AMyHUD::ClearWidgets()
{
	Widgets.Empty();
}

void AMyHUD::AddWidget(FMyWidget Widget)
{
	// find the pos of the widget based on the grid. draw the icons..
	FVector2D Start(200, 200);
	FVector2D Pad(12, 12);

	Widget.Size = FVector2D(100, 100);
	Widget.Position = Start;

	// compute the position here
	for (int32 Count = 0; Count < Widgets.Num(); Count++)
	{
		// Move the position to the right a bit.
		Widget.Position.X += Widget.Size.X + Pad.X;

		// If there is no more room to the right then jump to the next line
		if (Widget.Position.X + Widget.Size.X > Dims.X )
		{
			Widget.Position.X = Start.X;
			Widget.Position.Y += Widget.Size.Y + Pad.Y;
		}
	}
	Widgets.Add(Widget);
}

void AMyHUD::MouseClicked()
{
	FVector2D Mouse;
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerController->GetMousePosition(Mouse.X, Mouse.Y);

		// Clear handle on last held widget
		HeldWidget = nullptr;

		// go and see if mouse XY click pos hits any widgets
		for (int32 c = 0; c < Widgets.Num(); c++)
		{
			if (Widgets[c].HitScreen(Mouse))
			{
				HeldWidget = &Widgets[c]; // save widget
				return;					  // stop checking
			}
		}
	}
}

void AMyHUD::MouseMoved()
{
	static FVector2D LastMouse;
	FVector2D ThisMouse, dMouse;
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerController->GetMousePosition(ThisMouse.X, ThisMouse.Y);
		dMouse = ThisMouse - LastMouse;
		// see if the last mouse has been held down for
		// more than 0 seconds. If it has been held down, 
		// then drag can commence

		// APlayerController::GetInputKeyTimeDown
		// Returns how long the given key/button has been down.
		// Returns 0 if it's up or it just went down this frame.
		float Time = PlayerController->GetInputKeyTimeDown(EKeys::LeftMouseButton);
		if (Time > 0.f && HeldWidget)
		{
			// the mouse is being held down
			// move the widget by displacement amt
			HeldWidget->Position.X = dMouse.X;
			HeldWidget->Position.Y = dMouse.Y;
		}
		LastMouse = dMouse;
	}
}

void AMyHUD::MouseRightClicked()
{
	/** set to run whenever the right mouse button is clicked on the icon:  */
	FVector2D MousePosition;
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y);
		for (int32 iCount = 0; iCount < Widgets.Num(); iCount++)
		{
			if (Widgets[iCount].HitScreen(MousePosition))
			{
				AAvatar* Avatar = Cast<AAvatar>(PlayerController->GetPawn());
				if (Avatar)
				{
					if (Widgets[iCount].BP_Spell)
					{
						Avatar->CastSpell(Widgets[iCount].BP_Spell);
					}
				}
			}
		}
	}
}

void AMyHUD::DrawMessages()
{
	for (int32 Count = Messages.Num() - 1; Count >= 0; Count--)
	{
		// draw the background box the right size for the message
		float OutputWidth = 0.f;
		float OutputHeight = 0.f;
		float Pad = 10.f;

		GetTextSize(Messages[Count].Message, OutputWidth, OutputHeight, HUDFont);

		float MessageHeight = OutputHeight + 2.f * Pad;
		float X = 0.f;
		float Y = Count * MessageHeight;
		// light blue backing
		static FLinearColor Color = FLinearColor(0.f, 0.f, 1.f, 0.3f);
		DrawRect(Color, X, Y, Canvas->SizeX, MessageHeight);

		//DrawTexture(Messages[Count].Texture2D, X, Y, 100.f, 50.f, 0.f, 0.f, 1.f, 1.f);
		
		// draw our message using the HUDFont
		DrawText(Messages[Count].Message, Messages[Count].Color, 100.f + X + Pad, Y + Pad, HUDFont);
		// reduce lifetime by the time that passed since last frame.

		Messages[Count].Time -= GetWorld()->GetDeltaSeconds();

		// if the message's time is up, remove it
		if (Messages[Count].Time < 0)
		{
			Messages.RemoveAt(Count);
		}
	}
}

void AMyHUD::DrawHealthBar()
{
	AAvatar* Avatar = Cast<AAvatar>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (Avatar)
	{
		float BarWidth = 200.f;
		float BarHeight = 50.f;
		float BarPad = 12.f;
		float BarMargin = 50.f;
		float PercentHealthPoints = Avatar->CurrentHealth / Avatar->MaxHealth;
		DrawRect(FLinearColor(0, 0, 0, 1),
			Canvas->SizeX - BarWidth - BarMargin - BarPad,
			Canvas->SizeY - BarHeight - BarMargin - BarPad,
			BarWidth + 2 * BarPad, BarHeight + 2 * BarPad
		);

		DrawRect(FLinearColor(1 - PercentHealthPoints, PercentHealthPoints, 0, 1),
			Canvas->SizeX - BarWidth - BarMargin,
			Canvas->SizeY - BarHeight - BarMargin,
			BarWidth * PercentHealthPoints, BarHeight
		);
	}
}

void AMyHUD::DrawWidgets()
{
	for (int32 Count = 0; Count < Widgets.Num(); Count++)
	{
		// draw particular widget
		DrawTexture(
			Widgets[Count].Icon.Texture2D, Widgets[Count].Position.X,
			Widgets[Count].Position.Y, Widgets[Count].Size.X, Widgets[Count].Size.Y,
			0, 0, 1, 1);
		DrawText(Widgets[Count].Icon.Name, FLinearColor::Yellow, Widgets[Count].Position.X, Widgets[Count].Position.Y, HUDFont, 1.f);
	}
}
