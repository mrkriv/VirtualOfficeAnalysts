#pragma once

#include "Voice.h"
#include "OnlineSubsystemUtils.h"
#include "Sound/SoundWaveProcedural.h"
#include "GameFramework/Character.h"
#include "VoiceCharacter.generated.h"

UCLASS()
class VR_API AVoiceCharecter : public ACharacter
{
	GENERATED_BODY()

	TArray<uint8> sendBuff;
	bool bEnable;
public:
	AVoiceCharecter();

	TSharedPtr<class IVoiceCapture> VoiceCapture;

	UPROPERTY()
	USoundWaveProcedural* SoundStreaming;
	UPROPERTY()
	UAudioComponent* AudioComponent;

	UFUNCTION(BlueprintCallable, Category = "VoiceCapture")
	void EnableVoiceCapture();

	UFUNCTION(BlueprintCallable, Category = "VoiceCapture")
	void DisableVoiceCapture();

	UFUNCTION(BlueprintCallable, Category = "VoiceCapture")
	bool IsVoiceCapture();

	UFUNCTION(BlueprintCallable, Category = "VoiceCapture")
	bool GetVoiceData(TArray<uint8> &buffer);

	UFUNCTION(BlueprintCallable, Category = "VoiceCapture")
	void PlayVoiceData(TArray<uint8> buffer);

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
