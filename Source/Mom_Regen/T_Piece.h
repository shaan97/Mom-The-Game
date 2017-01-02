// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MazePiece.h"
#include "T_Piece.generated.h"

/**
 * 
 */
UCLASS()
class MOM_REGEN_API AT_Piece : public AMazePiece
{
	GENERATED_BODY()
	
public:
	AT_Piece() : AMazePiece() {
		setMesh();
	}
	virtual bool setMesh() override {
		// TODO : Adjust this so that it (1) Uses our meshes created by Kevin and (2) will work upon product launch (relative paths maybe?)
		static ConstructorHelpers::FObjectFinder<UStaticMesh> applyMesh(TEXT("StaticMesh'/Game/MazePieceMesh/TShapeSimple.TShapeSimple'"));
		if (applyMesh.Object) {
			static_mesh->SetStaticMesh(applyMesh.Object);
			return true;
		}

		return false;
	}
	
	
};

