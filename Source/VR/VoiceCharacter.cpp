#include "VR.h"
#include "AudioDevice.h"
#include "VoiceCharacter.h"

#define SEND_BUFFER_SIZE 500

AVoiceCharecter::AVoiceCharecter()
{
	VoiceCapture = FVoiceModule::Get().CreateVoiceCapture();
}

void AVoiceCharecter::BeginPlay()
{
	Super::BeginPlay();

	SoundStreaming = NewObject<USoundWaveProcedural>();
	SoundStreaming->SampleRate = 16000;
	SoundStreaming->NumChannels = 1;
	SoundStreaming->Duration = INDEFINITELY_LOOPING_DURATION;
	SoundStreaming->SoundGroup = SOUNDGROUP_Voice;
	SoundStreaming->bLooping = false;
	SoundStreaming->bProcedural = true;
	SoundStreaming->Pitch = 1.00f;
	SoundStreaming->Volume = 5.f;

	AudioComponent = UGameplayStatics::SpawnSoundAttached(SoundStreaming, RootComponent, TEXT("VoiceCaptureAudioComponent"));
	AudioComponent->bAllowSpatialization = true;
	AudioComponent->VolumeMultiplier = 5.0f;

	DisableVoiceCapture();
}

void AVoiceCharecter::EnableVoiceCapture()
{
	if (!bEnable)
	{
		bEnable = VoiceCapture.IsValid();
		if (bEnable)
			VoiceCapture->Start();
	}
}

void AVoiceCharecter::DisableVoiceCapture()
{
	bEnable = false;

	if (VoiceCapture.IsValid())
		VoiceCapture->Stop();
}

bool AVoiceCharecter::IsVoiceCapture()
{
	return bEnable;
}

bool AVoiceCharecter::GetVoiceData(TArray<uint8> &buffer)
{
	if (!VoiceCapture.IsValid())
		return false;

	if (sendBuff.Num() == 0)
	{
		uint32 available = 0;
		EVoiceCaptureState::Type CaptureState = VoiceCapture->GetCaptureState(available);

		if (CaptureState == EVoiceCaptureState::Ok && available > 0)
		{
			uint32 size = 0;
			sendBuff.Reset();
			sendBuff.SetNumUninitialized(available);
			VoiceCapture->GetVoiceData(sendBuff.GetData(), available, size);
		}
	}

	if (sendBuff.Num() == 0)
		return false;

	if (&sendBuff == nullptr)
		return false;

	buffer.Reset();
	buffer.SetNumUninitialized(SEND_BUFFER_SIZE);
	FMemory::Memcpy(buffer.GetData(), sendBuff.GetData(), SEND_BUFFER_SIZE);

	sendBuff.RemoveAt(0, SEND_BUFFER_SIZE, false);
	sendBuff.Reset();

	return true;
}

void AVoiceCharecter::PlayVoiceData(TArray<uint8> buffer)
{
	if (SoundStreaming != nullptr)
		SoundStreaming->QueueAudio(buffer.GetData(), buffer.Num());
}

void AVoiceCharecter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AVoiceCharecter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
