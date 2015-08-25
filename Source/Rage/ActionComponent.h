// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "ActionComponent.generated.h"


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



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RAGE_API UActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UActionComponent();


	/*

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "")
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)

	UFUNCTION(BlueprintImplementableEvent, Category = " ")

	UFUNCTION(BlueprintCallable, Category = " ")

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = " ")

	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		TSubclassOf<class AWeapon> DefaultMainWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		class AWeapon* MainWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		TSubclassOf<class AWeapon> DefaultAltWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		class AWeapon* AltWeapon;


	/*
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
		TArray<FWeaponSlot> WeaponMainList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
		TArray<FWeaponSlot> WeaponAltList;
	*/
	class ARageBaseCar* TheCar;


	virtual void BeginPlay() override;
	

	// BindExternal Calling

	

	void ActionDown();
	void ActionUp();

	void AltActionDown();
	void AltActionUp();

	void BoostDown();
	void BoostUp();


protected:

	// Callabale action Events
	UFUNCTION(BlueprintImplementableEvent, Category = " Main Events ")
		void BP_ActionDown();
	UFUNCTION(BlueprintImplementableEvent, Category = " Main Events ")
		void BP_ActionUp();



	UFUNCTION(BlueprintImplementableEvent, Category = " Main Events ")
		void BP_AltActionDown();
	UFUNCTION(BlueprintImplementableEvent, Category = " Main Events ")
		void BP_AltActionUp();

	UFUNCTION(BlueprintImplementableEvent, Category = " Main Events ")
		void BP_BoostDown();
	UFUNCTION(BlueprintImplementableEvent, Category = " Main Events ")
		void BP_BoostUp();
		
	
};



/*

UENUM(BlueprintType)
enum class EQuestCheckStatusType : uint8
{
None 		UMETA(DisplayName = "None"),
Hunt 		UMETA(DisplayName = "Hunt"),
Position 	UMETA(DisplayName = "Location"),
Inventory	UMETA(DisplayName = "Inventory"),
Time 		UMETA(DisplayName = "Time"),
};

USTRUCT(BlueprintType)
struct FBasicQuestData
{
GENERATED_USTRUCT_BODY()
public:

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Data")
FName SmartNodePath;

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Data")
int32 QuestStageNumber;

FBasicQuestData()
{
}
FBasicQuestData(FName newSmartNodePath, int32 newQuestStageNumber)
{
SmartNodePath = newSmartNodePath;
QuestStageNumber = newQuestStageNumber;

}
};
*/