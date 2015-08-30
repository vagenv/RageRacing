// Fill out your copyright notice in the Description page of Project Settings.

#include "Rage.h"
#include "Projectile.h"
#include "Engine.h"
#include "RageBaseCar.h"

AProjectile::AProjectile(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{

	//RootComponent = PCIP.CreateDefaultSubobject<USceneComponent>(this, TEXT("RootPoint"));

	Mesh = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("ProjectileMesh"));
	Mesh->bReceivesDecals = false;
	Mesh->CastShadow = false;
	RootComponent = Mesh;
	//Mesh->AttachParent = RootComponent;


	// Use a sphere as a simple collision representation
	CollisionComp = PCIP.CreateDefaultSubobject<USphereComponent>(this, TEXT("SphereComp"));
	CollisionComp->MoveIgnoreActors.Add(this);
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");			// Collision profiles are defined in DefaultEngine.ini
	CollisionComp->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);		// set up a notification for when this component overlaps something
	CollisionComp->IgnoreActorWhenMoving(TheCar, true);
	CollisionComp->IgnoreActorWhenMoving(TheWeapon, true);
	CollisionComp->AttachParent = Mesh;


	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = PCIP.CreateDefaultSubobject<UProjectileMovementComponent>(this, TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	//InitialLifeSpan = 3.0f;
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	Mesh->SetMobility(EComponentMobility::Movable);
	//ProjectileMovement->Velocity = FVector(0);
	const UWorld* theWorld = GetWorld();
	if (theWorld)
	{
		if (bAutoExplodeAfterTime)
		{
			FTimerHandle DeathHandle;
			theWorld->GetTimerManager().SetTimer(DeathHandle, this, &AProjectile::Explode, LifeTime, false);
		}


		FTimerHandle EnableDelayhandle;
		theWorld->GetTimerManager().SetTimer(EnableDelayhandle, this, &AProjectile::EnableProjectile, EnableDelay, false);
	}

}
void AProjectile::EnableProjectile()
{
	bInstantExplode = true;
}
void AProjectile::OnHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	if (!bInstantExplode)return;
	//printg(OtherActor->GetName());
	//printr("Hit Something");
	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, OtherComp->GetName());
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp != Mesh)
	{
	
		//printg(OtherComp->GetName());
		Explode();
	}

}

void AProjectile::Explode()
{

	const FVector Loc = GetActorLocation();
	for (TActorIterator<AActor> aItr(GetWorld()); aItr; ++aItr)
	{
		if ((aItr && aItr->GetRootComponent() && aItr->GetRootComponent()->IsSimulatingPhysics()) || Cast<ARageBaseCar>(*aItr))
		{
			float distance = GetDistanceTo(*aItr);
		
			//FVector::Dist(theChar->GetActorLocation(),GetActorLocation())<
			if (distance<AffectArea && aItr && aItr->GetRootComponent() && aItr->GetRootComponent()->Mobility == EComponentMobility::Movable)
			{

				FVector dir = aItr->GetActorLocation() - Loc;
				dir.Normalize();

				float DistanceMultiplier = 1;

				if (distance < InnerDamageRadius)DistanceMultiplier = 1;
				else DistanceMultiplier = 1 - (distance - InnerDamageRadius) / (AffectArea - InnerDamageRadius);

				if (Cast<ARageBaseCar>(*aItr))
				{
					Cast<ARageBaseCar>(*aItr)->GetMesh()->AddImpulse(dir*RadialImpulse*DistanceMultiplier * CarImpulseMuliplier);
					//printr(FString::FromInt(RadialImpulse*DistanceMultiplier * CarImpulseMuliplier));
				}
				else if (aItr->GetRootComponent()->IsSimulatingPhysics() && Cast<UPrimitiveComponent>(aItr->GetRootComponent()))
				{
					Cast<UPrimitiveComponent>(aItr->GetRootComponent())->AddImpulse(dir*RadialImpulse*DistanceMultiplier);
				}
			}
			
		}

	}


	BP_Explode();


	Destroy();
}
void AProjectile::InitVelocity(const FVector& ShootDirection)
{

	if (ProjectileMovement)
	{
		//printr("Init Velocity" + FVector(ShootDirection * ProjectileMovement->InitialSpeed).ToString());
		// set the projectile's velocity to the desired direction
		ProjectileMovement->Velocity = ShootDirection * ProjectileMovement->InitialSpeed;
		//printg(ProjectileMovement->Velocity.ToString());
	}
}