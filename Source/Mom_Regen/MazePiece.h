// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "maze_globals.h"

#include "GameFramework/Actor.h"
#include "MazePiece.generated.h"

UCLASS(abstract)
class MOM_REGEN_API AMazePiece : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* static_mesh;

public:	
	// Sets default values for this actor's properties
	AMazePiece();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual bool setMesh() PURE_VIRTUAL(AMazePiece::setMesh(), return false;);
	
	
};

