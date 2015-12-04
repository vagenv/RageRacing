// Copyright 2015 Vagen Ayrapetyan

#include "Rage.h"
#include "Engine.h"

#include "Projectile.h"
#include "Vehicle/RageBaseCar.h"

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
	CollisionComp->IgnoreActorWhenMoving(ThePlayer, true);
	CollisionComp->IgnoreActorWhenMoving(TheWeapon, true);
	CollisionComp->IgnoreActorWhenMoving(this, true);
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

	// Set default radial Damage Curve
	FRichCurve* RadialDamageCurveData = RadialDamageCurve.GetRichCurve();
	if (RadialDamageCurveData)
	{
		RadialDamageCurveData->AddKey(0, 90);
		RadialDamageCurveData->AddKey(380, 40);
		RadialDamageCurveData->AddKey(900, 0);
	}

	// Set default Radial Impulse Curve
	FRichCurve* RadialImpulseCurveData = RadialImpulseCurve.GetRichCurve();
	if (RadialImpulseCurveData)
	{
		RadialImpulseCurveData->AddKey(0, 15000);
		RadialImpulseCurveData->AddKey(900, 9000);
	}

}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (Role>=ROLE_Authority)
	{
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
	
		//printg(OtherActor->GetName());
		Explode();
	}

}

void AProjectile::Explode()
{

	const FVector Loc = GetActorLocation();
	for (TActorIterator<AActor> aItr(GetWorld()); aItr; ++aItr)
	{
		if ((aItr && aItr->GetRootComponent() && aItr->GetRootComponent()->IsSimulatingPhysics()) )
		{
			float distance = GetDistanceTo(*aItr);

			if (distance<AffectArea && aItr )
			{

				FVector dir = aItr->GetActorLocation() - Loc;
				dir.Normalize();
				ARageBaseCar* TheChar = Cast<ARageBaseCar>(*aItr);

				//If Player apply damage
				if (TheChar)
				{
					// BP Explosion Hit Enemy
					BP_Explode_HitEnemy(TheChar, RadialDamageCurve.GetRichCurve()->Eval(distance));

					// Apply Damage to Character
					UGameplayStatics::ApplyDamage(TheChar, RadialDamageCurve.GetRichCurve()->Eval(distance),ThePlayer->GetController(), this, ExplosionDamageType);

				
					if (TheChar && TheChar->GetActorEnableCollision())
						TheChar->GetMesh()->AddImpulse(dir*RadialImpulseCurve.GetRichCurve()->Eval(distance),NAME_None,false);
					//	Cast<UPrimitiveComponent>(aItr->GetRootComponent())->AddImpulse(dir*RadialImpulseCurve.GetRichCurve()->Eval(distance));

					//printr("Apply explosion Dmg :  " + FString::FromInt(RadialDamageCurve.GetRichCurve()->Eval(distance)));
				}
				else 
				// Apply impulse on physics actors
				if (aItr->GetRootComponent() && aItr->GetRootComponent()->Mobility == EComponentMobility::Movable
					&&aItr->GetRootComponent()->IsSimulatingPhysics() && Cast<UPrimitiveComponent>(aItr->GetRootComponent()))
				{
						Cast<UPrimitiveComponent>(aItr->GetRootComponent())->AddImpulse(dir*RadialImpulseCurve.GetRichCurve()->Eval(distance));
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