// Copyright 2015 Vagen Ayrapetyan

#pragma once

#include "GameFramework/WheeledVehicle.h"
#include "RageBaseCar.generated.h"



// Base For All Cars

class UPhysicalMaterial;
class UCameraComponent;
class USpringArmComponent;
class UTextRenderComponent;
class UInputComponent;
class UActionComponent;

UCLASS(config=Game)
class ARageBaseCar : public AWheeledVehicle
{
	GENERATED_BODY()


public:
	ARageBaseCar();



	virtual void BeginPlay() override;

	virtual void Tick(float Delta) override;



private:

	// Engine Sound Component
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UAudioComponent* EngineSoundComponent;


	// Slippery Material instance 
	UPhysicalMaterial* SlipperyMaterial;
	// Non Slippery Material instance 
	UPhysicalMaterial* NonSlipperyMaterial;


	// Returns EngineSoundComponent subobject 
	FORCEINLINE virtual UAudioComponent* GetEngineSoundComponent() const { return EngineSoundComponent; }



public:


	// Are we on a 'slippery' surface 
	bool bIsLowFriction;



	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rage Status")
		float MaxHealth = 100;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Rage Status")
		float Health = 50;

	UPROPERTY(Replicated)
		bool bDead;


	// Default inventory items
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
		TArray<TSubclassOf<class AItem> >  DefaultInventoryItems;


	// Default Main Weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		TSubclassOf<class AWeapon> DefaultMainWeapon;

	// Default Alt Weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		TSubclassOf<class AWeapon> DefaultAltWeapon;


	// Main Weapon Ref
	UPROPERTY(Replicated,EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	class AWeapon* MainWeapon;

	// Alt Weapon Ref
	UPROPERTY(Replicated,EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	class AWeapon* AltWeapon;

	// Ult Weapon Ref
	UPROPERTY(Replicated,EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	class AWeapon* UltWeapon;


	// Equip Default Weapon
	virtual void EquipDefaultWeapons(){};



	// Take Damage override
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)override;




	// Get Health Percent
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TheCar")
		float GetHealthPercent();



	// Update the physics material used by the vehicle mesh 
	virtual void UpdatePhysicsMaterial();



	// Engine Audio RPM Name
	static const FName EngineAudioRPM;


	// Get Move Speed
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TheCar")
	int32 GetMoveSpeed();

	// Get Current Gear State
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TheCar")
	int32 GetGearState();

	


};
