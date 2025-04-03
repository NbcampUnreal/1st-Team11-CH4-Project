#include "NormalLevelSequenceActor.h"
#include "EngineUtils.h"
#include "LevelSequencePlayer.h"
#include "ElvenRing/Boss/Boss.h"
#include "ElvenRing/Character/ElvenRingCharacter.h"


void ANormalLevelSequenceActor::BeginPlay()
{
	Super::BeginPlay();

	CurrentLevelSequence = GetSequence();
}



void ANormalLevelSequenceActor::StartSequence()
{
	// 1. 모든 플레이어를 hidden 처리
	SetAllPlayerHidden();

	// 2. 4번째 인자로 반환받을 시퀀스 액터 선언
	ALevelSequenceActor* OutActor = nullptr;

	// 3. 시퀀스를 재생할 플레이어 선언
	ULevelSequencePlayer* LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), CurrentLevelSequence, FMovieSceneSequencePlaybackSettings(), OutActor);

	// 4. 시퀀스가 끝날 때 호출할 함수 바인드
	LevelSequencePlayer->OnFinished.AddDynamic(this, &ANormalLevelSequenceActor::OnSequenceEnded);

	// 5. 시퀀스 재생
	if (IsValid(LevelSequencePlayer) && IsValid(OutActor))
	{
		LevelSequencePlayer->Play();
	}
}



void ANormalLevelSequenceActor::OnSequenceEnded()
{
	// 1. 모든 플레이어 Visible 처리
	SetAllPlayerUnhidden();

	// 2. 보스를 약간의 지연시간 후 전투 상태로 전환
	SetBossBattleModeWithDelay();
}



void ANormalLevelSequenceActor::SetAllPlayerHidden()
{
	for (AElvenRingCharacter* Player : TActorRange<AElvenRingCharacter>(GetWorld()))
	{
		if (IsValid(Player))
		{
			if (!Player->IsHidden())
			{
				Player->SetActorHiddenInGame(true);		
			}
		}
	}
}



void ANormalLevelSequenceActor::SetAllPlayerUnhidden()
{
	for (AElvenRingCharacter* Player : TActorRange<AElvenRingCharacter>(GetWorld()))
	{
		if (IsValid(Player))
		{
			if (Player->IsHidden())
			{
				Player->SetActorHiddenInGame(false);		
			}
		}
	}
}



void ANormalLevelSequenceActor::SetBossBattleModeWithDelay()
{
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]
	{
		for (ABoss* Boss : TActorRange<ABoss>(GetWorld()))
		{
			if (IsValid(Boss))
			{
				Boss->SetBossBattleMode();
			}
		}
	}), BattleModeDelay, false);
}