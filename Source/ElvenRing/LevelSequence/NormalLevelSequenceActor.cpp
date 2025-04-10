#include "NormalLevelSequenceActor.h"
#include "EngineUtils.h"
#include "LevelSequencePlayer.h"
#include "ElvenRing/ElvenRing.h"
#include "ElvenRing/Boss/Boss.h"
#include "ElvenRing/Character/ElvenRingCharacter.h"
#include "ElvenRing/Core/ElvenringGameInstance.h"
#include "ElvenRing/UI/UIManager.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


void ANormalLevelSequenceActor::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentLevelSequence = GetSequence();

	Instance = Cast<UElvenringGameInstance>(GetGameInstance());
}

void ANormalLevelSequenceActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ANormalLevelSequenceActor, bIsSequencePlaying);
}


void ANormalLevelSequenceActor::StartSequence()
{
	// 서버에서만 실행
	if (HasAuthority())
	{
		// 모든 클라이언트에게 시퀀스 재생 명령 전파
		MulticastPlaySequence();
		
		// 모든 클라이언트에게 플레이어 숨김 처리 전파
		MulticastSetAllPlayerHidden();
	}
}

void ANormalLevelSequenceActor::MulticastPlaySequence_Implementation()
{
	LOG(TEXT("Begin"));
	if (bIsSequencePlaying) return;

	bIsSequencePlaying = true;
	
	// 4번째 인자로 반환받을 시퀀스 액터 선언
	ALevelSequenceActor* OutActor = nullptr;
	LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), CurrentLevelSequence, FMovieSceneSequencePlaybackSettings(), OutActor);

	// 시퀀스가 끝날 때 호출할 함수 바인드
	LevelSequencePlayer->OnFinished.AddDynamic(this, &ANormalLevelSequenceActor::OnSequenceEnded);

	// 시퀀스 재생
	if (IsValid(LevelSequencePlayer) && IsValid(OutActor))
	{
		LevelSequencePlayer->Play();
	}
}

void ANormalLevelSequenceActor::MulticastSetAllPlayerHidden_Implementation()
{
	// UI 모두 비활성화
	if (Instance && Instance->GetUIManager())
	{
		Instance->GetUIManager()->SetActiveCharactersUI(false);
	}

	// 플레이어 객체 hidden 처리
	for (AElvenRingCharacter* Player : TActorRange<AElvenRingCharacter>(GetWorld()))
	{
		if (IsValid(Player))
		{
			Player->ToggleInput(false);
		}
	}
}

void ANormalLevelSequenceActor::OnSequenceEnded()
{
	// 서버에서만 실행
	if (HasAuthority())
	{
		// 모든 클라이언트에게 플레이어 활성화 전파
		MulticastSetAllPlayerUnhidden();

		// 서버에게 시퀀스 종료 이벤트 전파
		ServerOnSequenceEnded();
	}
}

void ANormalLevelSequenceActor::ServerOnSequenceEnded_Implementation()
{
	// 재생하는 LevelSequence 타입에 따라 로직 처리
	FTimerHandle TimerHandle;
	switch (SequenceType)
	{
	case ESequenceType::Spawn:
		// 서버에서 보스 스폰 시퀀스가 끝났음을 인지한 후 모든 클라이언트에게 스폰 이벤트 전파
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ANormalLevelSequenceActor::OnSpawnSequenceEnded, SpawnSequenceDelegateDelay, false);
		break;
	case ESequenceType::Phase:
		// 서버에서 보스 페이즈 시퀀스가 끝났음을 인지한 후 모든 클라이언트에게 페이즈 전환 이벤트 전파
		OnPhaseSequenceEnded();
		break;
	case ESequenceType::Dead:
		// 단순한 UI와 사운드 출력이므로 동기화 없이 Multicast로 출력
		Instance->GetUIManager()->ShowMessage("Enemy Defeated !", EMessageType::SystemMessage);
		USoundBase* MySound = LoadObject<USoundBase>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/ElvenRing/Resources/SFX/UI/BossKilled.BossKilled'"));
		if (MySound)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), MySound);
		}
		break;
	}
}

void ANormalLevelSequenceActor::MulticastSetAllPlayerUnhidden_Implementation()
{
	// UI 모두 활성화
	if (Instance && Instance->GetUIManager())
	{
		Instance->GetUIManager()->SetActiveCharactersUI(true);
	}

	// 플레이어 객체 활성화 처리
	for (AElvenRingCharacter* Player : TActorRange<AElvenRingCharacter>(GetWorld()))
	{
		if (IsValid(Player))
		{
			Player->ToggleInput(true);
		}
	}
}

void ANormalLevelSequenceActor::OnSpawnSequenceEnded()
{
	LOG(TEXT("Begin"));
	for (ABoss* Boss : TActorRange<ABoss>(GetWorld()))
	{
		if (IsValid(Boss))
		{
			Boss->ServerOnSpawnSequenceEnded();
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
			Boss->MulticastOnPhaseSequenceEnded();
			return;
		}
	}
}