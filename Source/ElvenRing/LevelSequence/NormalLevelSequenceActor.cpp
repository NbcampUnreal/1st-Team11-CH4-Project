#include "NormalLevelSequenceActor.h"
#include "EngineUtils.h"
#include "LevelSequencePlayer.h"
#include "ElvenRing/ElvenRing.h"
#include "ElvenRing/Boss/Boss.h"
#include "ElvenRing/Character/ElvenRingCharacter.h"
#include "ElvenRing/Core/ElvenringGameInstance.h"
#include "ElvenRing/UI/UIManager.h"


void ANormalLevelSequenceActor::BeginPlay()
{
	Super::BeginPlay();

	CurrentLevelSequence = GetSequence();
}



void ANormalLevelSequenceActor::StartSequence()
{
	// 모든 플레이어를 hidden 처리
	SetAllPlayerHidden();

	// 4번째 인자로 반환받을 시퀀스 액터 선언
	ALevelSequenceActor* OutActor = nullptr;

	// 시퀀스를 재생할 플레이어 지정
	LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), CurrentLevelSequence, FMovieSceneSequencePlaybackSettings(), OutActor);

	// 시퀀스가 끝날 때 호출할 함수 바인드
	LevelSequencePlayer->OnFinished.AddDynamic(this, &ANormalLevelSequenceActor::OnSequenceEnded);

	// 시퀀스 재생
	if (IsValid(LevelSequencePlayer) && IsValid(OutActor))
	{
		LevelSequencePlayer->Play();
	}
}



void ANormalLevelSequenceActor::OnSequenceEnded()
{
	// 모든 플레이어 Visible 처리
	SetAllPlayerUnhidden();

	// 재생하는 LevelSequence 타입에 따라 로직 처리
	FTimerHandle TimerHandle;
	switch (SequenceType)
	{
	case ESequenceType::Spawn:
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ANormalLevelSequenceActor::OnSpawnSequenceEnded, SpawnSequenceDelegateDelay, false);
		break;
	case ESequenceType::Phase:
		OnPhaseSequenceEnded();
		break;
	case ESequenceType::Dead:
		UElvenringGameInstance* Instance = Cast<UElvenringGameInstance>(GetGameInstance());
		Instance->GetUIManager()->ShowMessage("Enemy Defeated !", EMessageType::SystemMessage);
		break;
	}
}



void ANormalLevelSequenceActor::SetAllPlayerHidden()
{
	for (AElvenRingCharacter* Player : TActorRange<AElvenRingCharacter>(GetWorld()))
	{
		if (IsValid(Player))
		{
			LOG(TEXT("Player Hidden"));
			Player->ToggleInput(false);
		}
	}
}



void ANormalLevelSequenceActor::SetAllPlayerUnhidden()
{
	for (AElvenRingCharacter* Player : TActorRange<AElvenRingCharacter>(GetWorld()))
	{
		if (IsValid(Player))
		{
			LOG(TEXT("Player Unhidden"));
			Player->ToggleInput(true);
		}
	}
}



void ANormalLevelSequenceActor::OnSpawnSequenceEnded()
{
	for (ABoss* Boss : TActorRange<ABoss>(GetWorld()))
	{
		if (IsValid(Boss))
		{
			Boss->OnSpawnSequenceEnded();
			return;
		}
	}
}

void ANormalLevelSequenceActor::OnPhaseSequenceEnded()
{
	for (ABoss* Boss : TActorRange<ABoss>(GetWorld()))
	{
		if (IsValid(Boss))
		{
			Boss->OnPhaseSequenceEnded();
			return;
		}
	}
}
