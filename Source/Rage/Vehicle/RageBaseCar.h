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

	/// Main Components



	/** Audio component for the engine sound */
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAudioComponent* EngineSoundComponent;

	/*

	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* MainWeaponMeshComponent;

	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* AltWeaponMeshComponent;

		*/

public:
	ARageBaseCar();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TheCar")
		float Health_CurrentValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TheCar")
		float Health_MaxValue;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TheCar")
		float GetHealthPercent();


	/*

	UFUNCTION(BlueprintImplementableEvent, Category = " ")

	UFUNCTION(BlueprintCallable, Category = " ")

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = " ")
	
	*/


	///////////                                                                        Legacy part

	

	// Begin Actor interface
	virtual void Tick(float Delta) override;
	virtual void BeginPlay() override;

	/** Update the physics material used by the vehicle mesh */
	virtual void UpdatePhysicsMaterial();



	
	static const FName EngineAudioRPM;


	

	/* Are we on a 'slippery' surface */
	bool bIsLowFriction;
	/** Slippery Material instance */
	UPhysicalMaterial* SlipperyMaterial;
	/** Non Slippery Material instance */
	UPhysicalMaterial* NonSlipperyMaterial;


public:

	/** Returns EngineSoundComponent subobject **/
	FORCEINLINE virtual UAudioComponent* GetEngineSoundComponent() const { return EngineSoundComponent; }

};


/*


UENUM(BlueprintType)
enum class EWeaponAttachEvent : uint8
{
Action 			UMETA(DisplayName = "Action"),
AltAction 		UMETA(DisplayName = "AltAction"),
Boost 			UMETA(DisplayName = "Boost"),
};


USTRUCT(BlueprintType)
struct FWeaponSlot
{
GENERATED_USTRUCT_BODY()
public:

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Data")
TSubclassOf<class AWeapon> TheWeapon;

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Data")
FName SocketName;

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Data")
EWeaponAttachEvent ActionName;

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
float CurretAmmo;


FWeaponSlot()
{
}
};

*/