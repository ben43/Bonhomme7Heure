// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Components/ArrowComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BONHOMME7HEURE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGrabber();

	UFUNCTION(BlueprintImplementableEvent,Category = "ActorHit")
		void ActorHit();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;



private:

	UPROPERTY(EditAnywhere)
		float Reach = 200.f;
	UPROPERTY(EditAnywhere)
		float GrabberMaxMass = 0.0f;

	UPhysicsHandleComponent* physichandler = nullptr;
	UInputComponent* InputComponent = nullptr;
	UArrowComponent* playerArrow = nullptr;

	UFUNCTION(BlueprintCallable)
		void Grab();

	UFUNCTION(BlueprintCallable)
		void Release();


	void FindPhysicsHandle();
	void FindArrowComponent();
	void SetupInputComponent();

	//FVector playerViewLocation;
	//FVector playerViewRotation;

	FHitResult GetFirstPhysicBodyInReach();

	FVector GetPlayerViewLocation();

	FVector GetLineTraceEnd();


};
