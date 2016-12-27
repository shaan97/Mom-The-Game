// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "MazePiece.h"
#include "L_Piece.generated.h"

/**
 * 
 */
UCLASS()
class MOM_REGEN_API AL_Piece : public AMazePiece
{
	GENERATED_BODY()
	
public:
	AL_Piece() : AMazePiece() {
		setMesh();
	}

	virtual bool setMesh() override {
		// TODO : Adjust this so that it (1) Uses our meshes created by Kevin and (2) will work upon product launch (relative paths maybe?)
		static ConstructorHelpers::FObjectFinder<UStaticMesh> applyMesh(TEXT("StaticMesh'/Game/MazePieceMesh/Corner.Corner'"));
		if (applyMesh.Object) {
			static_mesh->SetStaticMesh(applyMesh.Object);
			return true;
		}

		return false;
	}
	
	
};

