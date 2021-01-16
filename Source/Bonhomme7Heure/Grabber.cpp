// Fill out your copyright notice in the Description page of Project Settings.

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/InputComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Grabber.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features 
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// ...

	FindPhysicsHandle();

	FindArrowComponent();
	
}

void UGrabber::FindArrowComponent()
{
	for (TObjectIterator<UArrowComponent> Itr; Itr; ++Itr) 
	{
		if(Itr->GetName() == "GrabberOrigin")
		{
			playerArrow = *Itr;
		}
	
	}

	if(!playerArrow)
	{
		UE_LOG(LogTemp, Error, TEXT("The main character doesnt have the grabber origin so the grabber will therefor not work."))
	}
	
}

//This find the physic handle component for the grabber and stores it in a variable declared in the header file.
//This component will handle the physics of the handler
void UGrabber::FindPhysicsHandle()
{
	physichandler = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(!physichandler)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s does not have a physic handler, or the game will either crash or the grabber will not work."), *GetOwner()->GetName());
	}
}



// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Move The object we are holding with us
	if (!physichandler) { return; } //prevent crash when there is no physic handler

	//Make the object follow the player
	if(physichandler->GrabbedComponent)
	{
		physichandler->SetTargetLocation(GetLineTraceEnd());
	}
}


void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber pressed."));

	// ray-cast and return the body that was touched by the it.
	FHitResult HitResult = GetFirstPhysicBodyInReach();

	//Give the reference to the actor or component that was struck by the ray-cast
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();

	AActor* ActorHit = HitResult.GetActor();

	//Get the range of the ray-cast for the handler to grab it at the good range
	FVector LineTraceEnd = GetLineTraceEnd();



	//Attach the actor if it is a valid actor
	if(ActorHit)
	{
		UPrimitiveComponent* ActorPrimitiveComponent = ActorHit->FindComponentByClass<UPrimitiveComponent>();

		//ActorPrimitiveComponent->SetSimulatePhysics(true);

		float ActorMass = ActorPrimitiveComponent->GetMass();

		//ActorHit();

		if (ActorMass < GrabberMaxMass) 
		{

			if (!physichandler) { return; } //verify if there is a physichandler to avoid crash

			physichandler->GrabComponentAtLocation(
				ComponentToGrab,
				NAME_None,
				LineTraceEnd
			);
		}
	}




}

void UGrabber::Release()
{
	

	physichandler->ReleaseComponent();
}


FHitResult UGrabber::GetFirstPhysicBodyInReach()
{
	FVector playerViewLocation = GetPlayerViewLocation();
	FRotator playerViewRotation;
	//FVector StartingPoint = playerViewLocation + playerViewRotation.Vector();
	FVector LineTraceEnd = GetLineTraceEnd();

	FString playerViewLocationString = playerViewLocation.ToString();

	UE_LOG(LogTemp, Warning, TEXT("The player view location is : %s"),*playerViewLocationString);

	FHitResult Hit;

	//Ray-cast to the distance set by the reach variable.
	FCollisionQueryParams TraceParams(
	FName(TEXT("")), // This is the trace tag, not sure what it means ...
	false, //Decide if we use complex collision
	GetOwner() // This are the ignored actors
	);

	//Detect what we just hit
	
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,				//out the object hit
		playerViewLocation,		//Start of the line, in this case the FVector of the camera emplacement, so the "eye" of the player
		LineTraceEnd,			//End of the line.
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),	//This get the target info to compare with the one below
		TraceParams	//Refer to the parameters we said we want and compare them with the target hit parameter
	);

	//Store the actor we just hit someone
	AActor* ActorHit = Hit.GetActor();

	if(ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s was hit by the grabber"), *ActorHit->GetName());
	}

	DrawDebugLine(
		GetWorld(),
		playerViewLocation,
		LineTraceEnd,
		FColor(0, 255, 0),
		false,
		4.f,
		0,
		5.f
	);
	return Hit;

}

//Get the LineTraceEnd (0) and the playerViewLocation (1)
FVector UGrabber::GetPlayerViewLocation()
{
	FVector playerViewLocation;
	FRotator playerViewRotation;

	//Get player location and rotation
	
	//This get the arrow component Location and Rotation and use it as the grabber base. 
	playerViewRotation = playerArrow->GetComponentRotation() ;

	playerViewLocation = playerArrow->GetComponentLocation(); //+ GetOwner()->GetActorLocation();        //playerArrow->GetComponentLocation() + GetOwner()->GetActorLocation();

	FString playerViewLocationString = playerViewLocation.ToString();

	UE_LOG(LogTemp, Warning, TEXT("The playerViewLocation in the GetPlayerViewLocation function: %s"),*playerViewLocationString);

	return playerViewLocation;

}

//Get the lineTraceEnd
FVector UGrabber::GetLineTraceEnd()
{
	FRotator playerViewRotation = playerArrow->GetComponentRotation();
	FVector playerViewLocation = playerArrow->GetComponentLocation();

	FVector lineTraceEnd = playerViewLocation + playerViewRotation.Vector() * Reach;

	return lineTraceEnd;
}


