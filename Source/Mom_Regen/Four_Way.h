// Fill out your copyright notice in the Description page of Project Settings.

#pragma once



#include "MazePiece.h"
#include "Four_Way.generated.h"

/**
 * 
 */
UCLASS()
class MOM_REGEN_API AFour_Way : public AMazePiece
{
	GENERATED_BODY()
private:
	UPointLightComponent* light;
	long numTicks;
	const float INTENSITY = 1000;
	UParticleSystemComponent* dust;
public:
	AFour_Way() : AMazePiece(), numTicks(0) {
		PrimaryActorTick.bCanEverTick = true;
		setMesh();
		
		RootComponent = static_mesh;
		
		light = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light"));
		light->SetIntensity(INTENSITY);
		light->SetupAttachment(RootComponent);

		dust = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("dust"));
		static ConstructorHelpers::FObjectFinder<UParticleSystem> amb_dust(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Ambient_Dust.P_Ambient_Dust'"));
		if (amb_dust.Object) {
			dust->SetTemplate(amb_dust.Object);
		}
		dust->SetupAttachment(RootComponent);
		//dust->SetRelativeLocation(FVector(0));

	}

	virtual bool setMesh() override {
		// TODO : Adjust this so that it (1) Uses our meshes created by Kevin and (2) will work upon product launch (relative paths maybe?)
		static ConstructorHelpers::FObjectFinder<UStaticMesh> applyMesh(TEXT("StaticMesh'/Game/MazePieceMesh/FourWaySimple.FourWaySimple'"));
		if (applyMesh.Object) {
			static_mesh->SetStaticMesh(applyMesh.Object);
			return true;
		}

		return false;
	}

	virtual void Tick(float DeltaSeconds) override;

	
};

