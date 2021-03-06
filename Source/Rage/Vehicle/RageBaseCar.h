// Copyright 2015-2016 Vagen Ayrapetyan

#pragma once

#include "GameFramework/WheeledVehicle.h"
//#include "Components/WidgetComponent.h"
#include "RageBaseCar.generated.h"


// Template classes
class UPhysicalMaterial;
class UCameraComponent;
class USpringArmComponent;
class UTextRenderComponent;
class UInputComponent;
class UWidgetComponent;

UCLASS(config=Game)
class ARageBaseCar : public AWheeledVehicle
{
	GENERATED_BODY()




public:
	ARageBaseCar();

	// Begin Play
	virtual void BeginPlay() override;

	// Post Begin Play
	virtual void PostBeginPlay();

	// Tick/Update
	virtual void Tick(float Delta) override;



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//								Character Data


	// Maximum Health
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status")
		float MaxHealth = 100;

	// Current Health
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Status")
		float Health = 50;


	// Character Name 
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Character")
		FString CharacterName;

	// Character Color
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Character")
		FLinearColor CharacterColor = FLinearColor::White;

	// Material Properties
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Character")
		FVector AdditonalColorProperties = FVector(1);

	// Randomise Color on Start
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		bool bRandomizeCharacterColorOnStart = false;


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//								Character Components


	// Character Status Widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
		UWidgetComponent* CharacterStatusWidget;


	// Main Weapon Ref
	UPROPERTY(ReplicatedUsing = OnRep_WeaponsUpdated, EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	class AWeapon* MainWeapon;

	// Alt Weapon Ref
	UPROPERTY(ReplicatedUsing = OnRep_WeaponsUpdated, EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	class AWeapon* AltWeapon;

	// Ult Weapon Ref
	UPROPERTY(ReplicatedUsing = OnRep_WeaponsUpdated, EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	class AWeapon* UltWeapon;


	// Default inventory items
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
		TArray<TSubclassOf<class AItem> >  DefaultInventoryItems;


	// Default Main Weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		TSubclassOf<class AWeapon> DefaultMainWeapon;

	// Default Alt Weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		TSubclassOf<class AWeapon> DefaultAltWeapon;

	// Local Player Controller/ The one who holds camera
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	class APlayerController* TheLocalController;


	// Equip Default Weapon
	virtual void EquipDefaultWeapons(){};

	// Current Weapon Updated
	UFUNCTION()
		virtual void OnRep_WeaponsUpdated(){};


	// Curve with scale-to-distance ratio of widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
		FRuntimeFloatCurve CharacterStatusWidget_ZoomCurve;

	// Default offset of widget 
	FVector CharacterStatusWidget_DefaultOffset=FVector(0);



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			
	//									Death/ Revive / TakeDamage/AddHealth
		
	// Is Character Dead
	UPROPERTY(Replicated)
		bool bDead;

	// Can Character Revive after death
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
		bool bCanRevive = false;

	// Character Revive Delay
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
		float ReviveTime= 5;

	// Death Event
	virtual void Death();

	// Global Death Event
	UFUNCTION(NetMulticast, Reliable)
		void Global_Death();
		virtual void Global_Death_Implementation();

	// Blueprint Death
	UFUNCTION(BlueprintImplementableEvent, Category = "Status")
		void BP_Death();

	// Revive
	virtual void Revive();
	// Global Death Event
	UFUNCTION(NetMulticast, Reliable)
		void Global_Revive();
		virtual void Global_Revive_Implementation();

	// Blueprint Revive
	UFUNCTION(BlueprintImplementableEvent, Category = "Status")
		void BP_Revive();


	// Add Health to Character
	UFUNCTION(BlueprintCallable, Category = "Status")
		virtual void AddHealth(float AddValue);

	// Take Damage override
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)override;


	// Get Health Percent
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Status")
		float GetHealthPercent();



	// Get Move Speed
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "State")
	int32 GetMoveSpeed();

	// Get Current Gear State
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "State")
	int32 GetGearState();


	// Function that rotates the widget to camera
	void RotateCharacterStatusWidget();


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//							Legacy from Vechile tempalte

	// Update the physics material used by the vehicle mesh 
	virtual void UpdatePhysicsMaterial();

	// Are we on a 'slippery' surface 
	bool bIsLowFriction;


	// Engine Audio RPM Name
	static const FName EngineAudioRPM;


private:

	// Engine Sound Component
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UAudioComponent* EngineSoundComponent;

	// Slippery Material instance 
	UPROPERTY()
		UPhysicalMaterial* SlipperyMaterial;

	// Non Slippery Material instance 
	UPROPERTY()
		UPhysicalMaterial* NonSlipperyMaterial;
public:


	// Returns EngineSoundComponent subobject 
	FORCEINLINE virtual UAudioComponent* GetEngineSoundComponent() const { return EngineSoundComponent; }



};
