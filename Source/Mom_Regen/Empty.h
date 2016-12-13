// Fill out your copyright notice in the Description page of Project Settings.

#pragma once



#include "MazePiece.h"
#include "Empty.generated.h"

/**
 * 
 */
UCLASS()
class MOM_REGEN_API AEmpty : public AMazePiece
{
	GENERATED_BODY()

public:
	AEmpty() : AMazePiece() {
		setMesh();
	}

	virtual bool setMesh() override {
		// TODO : Adjust this so that it (1) Uses our meshes created by Kevin and (2) will work upon product launch (relative paths maybe?)
		static ConstructorHelpers::FObjectFinder<UStaticMesh> applyMesh(TEXT("/Game/Geometry/Meshes/1M_Cube"));
		if (applyMesh.Object) {
			static_mesh->SetStaticMesh(applyMesh.Object);
			return true;
		}

		return false;
	}
	
	
};

