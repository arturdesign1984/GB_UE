#include "Cannon.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"

// Sets default values
ACannon::ACannon()
{
 	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = SceneComp;

	CannonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CannonMesh"));
	CannonMesh->SetupAttachment(SceneComp);

	ProjectileSpawnComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ProjectileSpawnComponent->SetupAttachment(CannonMesh);

}

void ACannon::Fire()
{
	if (!bReadyToFire && Ammo <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Can't shooting");
		return;
	}
	if (CannonType == ECannonType::FireProjectile)
	{
		int NumTimesToFire = 3;
		int BulletInQueye = 0;
		if (nextShoot) {
			GetWorld()->GetTimerManager().SetTimer(BurstShooting, [this, &BulletInQueye, &NumTimesToFire]()
				{
					nextShoot = false;
					if (BulletInQueye <= NumTimesToFire) {
						Shoot();
						BulletInQueye += 1;
						GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("BulletInQueye: %d"), BulletInQueye));
					}
					else
					{
						GetWorldTimerManager().ClearTimer(BurstShooting);
						GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Queue ended")));
						nextShoot = true;
					}
				}, 0.3f, true);
			Ammo -= 1;
		}
	}
	else if (CannonType == ECannonType::FireTrace)
	{
		int NumTimesToFire = 3;
		int BulletInQueye = 0;
		if (nextShoot) {
			GetWorld()->GetTimerManager().SetTimer(BurstShooting, [this, &BulletInQueye, &NumTimesToFire]()
				{
					nextShoot = false;
					if (BulletInQueye <= NumTimesToFire) {
						Shoot();
						BulletInQueye += 1;
						GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("BulletInQueye: %d"), BulletInQueye));
					}
					else
					{
						GetWorldTimerManager().ClearTimer(BurstShooting);
						GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Queue ended")));
						nextShoot = true;
					}
				}, 0.3f, true);
			Ammo -= 1;
		}
	}
	bReadyToFire = false;
	if (Ammo <= 0)
		GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACannon::Reload, FireRate, false);
}

void ACannon::FireSpecial()
{
	if (!bReadyToFire && Ammo <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "No ammo");
		return;
	}
	if (CannonSpecialType == ECannonSpecialType::SpecialFire)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Ammo: %d FireSpecial"), Ammo));
		Ammo -= 1;
	}
	bReadyToFire = false;
	if (Ammo <= 0)
		GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACannon::Reload, FireRate, false);
}

bool ACannon::IsReadyToFire()
{
		return nextShoot ? bReadyToFire = true : bReadyToFire = false;
}

void ACannon::Reload()
{
	if (!bReadyToFire && Ammo <= 0)
	{
		bReadyToFire = true;
		Ammo = 30;
	}
}

void ACannon::BeginPlay()
{
	Super::BeginPlay();
	
	bReadyToFire = true;
	Ammo = 30;
}

void ACannon::Shoot()
{
	if (CannonType == ECannonType::FireProjectile)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Purple, FString::Printf(TEXT("Ammo: %d FireProjectile"), Ammo));
	}
	if (CannonType == ECannonType::FireTrace)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, FString::Printf(TEXT("Ammo: %d FireTrace"), Ammo));
	}
}


