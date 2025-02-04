// Fill out your copyright notice in the Description page of Project Settings.

#pragma once



#include "MazePiece.h"
#include "Vert_Line.generated.h"

/**
 * 
 */
UCLASS()
class MOM_REGEN_API AVert_Line : public AMazePiece
{
	GENERATED_BODY()
	
public:
	AVert_Line() : AMazePiece() {
		setMesh();
	}

	virtual bool setMesh() override {
		// TODO : Adjust this so that it (1) Uses our meshes created by Kevin and (2) will work upon product launch (relative paths maybe?)
		static ConstructorHelpers::FObjectFinder<UStaticMesh> applyMesh(TEXT("StaticMesh'/Game/MazePieceMesh/OpenCylinderSimple.OpenCylinderSimple'"));
		if (applyMesh.Object) {
			static_mesh->SetStaticMesh(applyMesh.Object);
			return true;
		}

		return false;
	}
	
	
};

