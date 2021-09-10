// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BulletHitInterface.h"
#include "Enemy.generated.h"

UCLASS()
class SHOOTER_API AEnemy : public ACharacter, public IBulletHitInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent)
		void ShowHealthBar();
	void ShowHealthBar_Implementation();

	UFUNCTION(BlueprintImplementableEvent)
		void HideHealthBar();

	void Die();

	void PlayHitMontage(FName Section, float PlayRate = 1.f);

	void ResetHitReactTimer();

	UFUNCTION(BlueprintCallable)
		void StoreHitNumber(UUserWidget* HitNumber, FVector Location);

	UFUNCTION()
		void DestroyHitNumber(UUserWidget* HitNumber);

	void UpdateHitNumbers();

	// called when something overlaps with the agro sphere
	UFUNCTION()
		void AgroSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
		void SetStunned(bool Stunned);

	UFUNCTION()
		void CombatRangeOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void CombatRangeEndOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
		void PlayAttackMontage(FName Section, float PlayRate);

	UFUNCTION(BlueprintPure)
		FName GetAttackSectionName();

	UFUNCTION()
		void OnLeftWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnRightWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// activate/deactivate collision for weapon boxes
	UFUNCTION(BlueprintCallable)
		void ActivateLeftWeapon();
	UFUNCTION(BlueprintCallable)
		void DeactivateLeftWeapon();
	UFUNCTION(BlueprintCallable)
		void ActivateRightWeapon();
	UFUNCTION(BlueprintCallable)
		void DeactivateRightWeapon();

	void DoDamage(class AShooterCharacter* Victim);
	void SpawnBlood(AShooterCharacter* Victim, FName SocketName);

	// attemp to stun character
	void StunCharacter(AShooterCharacter* Victim);

	void ResetCanAttack();

	UFUNCTION(BlueprintCallable)
	void FinishDeath();

	UFUNCTION()
	void DestroyEnemy();

private:

	// particles to spawn when hit by bullets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		class UParticleSystem* ImpactParticles;

	// sound to polay when hit by bullets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		class USoundCue* ImpactSound;

	// current health of the enemy
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
		float Health;

	// maximum health of the enemy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		float MaxHealth;

	// name of the head bone
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		FString HeadBone;

	// time to display health bar once shot
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		float HealthBarDisplayTime;

	FTimerHandle HealthBarTimer;

	// montage containing hit and death animations
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		UAnimMontage* HitMontage;

	FTimerHandle HitReactTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		float HitReactTimeMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		float HitReactTimeMax;

	bool bCanHitReact;

	// map to store hitNumber widgets and their hit locations
	UPROPERTY(VisibleAnywhere, Category = Combat, meta = (AllowPrivateAccess = "true"))
		TMap<UUserWidget*, FVector> HitNumbers;

	// time before a hitNumber is removed from the screen
	UPROPERTY(EditAnywhere, Category = Combat, meta = (AllowPrivateAccess = "true"))
		float HitNumberDestroyTime;

	// Behavior tree for the AI Character
	UPROPERTY(EditAnywhere, Category = "Behavior Tree", meta = (AllowPrivateAccess = "true"))
		class UBehaviorTree* BehaviorTree;

	// point for the enemy to move to
	UPROPERTY(EditAnywhere, Category = "Behavior Tree", meta = (AllowPrivateAccess = "true", MakeEditWidget = "true"))
		FVector PatrolPoint;

	// second point for the enemy to move to
	UPROPERTY(EditAnywhere, Category = "Behavior Tree", meta = (AllowPrivateAccess = "true", MakeEditWidget = "true"))
		FVector PatrolPoint2;

	class AEnemyController* EnemyController;

	// overlap sphere for when the enemy becomes hostile
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* AgroSphere;

	// true when playing the get hit animation
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		bool bStunned;

	// chance of being stunned. 0: no stun chance, 1: 100% stun chance
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		float StunChance;

	// true when in attack range time to attack!
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		bool bInAttackRange;

	// overlap sphere for when the enemy becomes hostile
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* CombatRangeSphere;

	// montage containing different attacks
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		UAnimMontage* AttackMontage;

	// the four attack montage section names
	FName AttackLFast;
	FName AttackRFast;
	FName AttackL;
	FName AttackR;

	// collision volume for the left weapon
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* LeftWeaponCollision;

	// collision volume for the right weapon
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* RightWeaponCollision;

	// base damage for enemy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		float BaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		FName LeftWeaponSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		FName RighttWeaponSocket;

	// true when enemy can attack
	UPROPERTY(VisibleAnywhere, Category = Combat, meta = (AllowPrivateAccess = "true"))
		bool bCanAttack;

	FTimerHandle AttackWaitTimer;

	// minimum wait time between attacks
	UPROPERTY(EditAnywhere, Category = Combat, meta = (AllowPrivateAccess = "true"))
		float AttackWaitTime;

	// death anim montage for the enemy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		UAnimMontage* DeathMontage;

	bool bDying;

	FTimerHandle DeathTimer;

	// time after death until destroy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		float DeathTime;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BulletHit_Implementation(FHitResult HitResult) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	FORCEINLINE FString GetHeadBone() const { return HeadBone; }

	UFUNCTION(BlueprintImplementableEvent)
		void ShowHitNumber(int32 Damage, FVector HitLocation, bool bHeadShot);

	FORCEINLINE UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }
};
