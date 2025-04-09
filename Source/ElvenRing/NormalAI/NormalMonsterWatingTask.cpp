#include "ElvenRing/NormalAI/NormalMonsterWatingTask.h"
#include "ElvenRing/NormalAI/NormalMonster.h"
#include "ElvenRing/NormalAI/NormalAIController.h"  // 컨트롤러 헤더 추가
#include "ElvenRing/NormalAI/Grux_AnimInstance.h"

#include "GameFramework/Actor.h"
#include "Animation/AnimInstance.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UNormalMonsterWatingTask::UNormalMonsterWatingTask()
{
	bNotifyTick = true; 
	NodeName = TEXT("Normal Monster Waiting Task");
}

EBTNodeResult::Type UNormalMonsterWatingTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ANormalAIController* AIController = Cast<ANormalAIController>(OwnerComp.GetAIOwner());
	ANormalMonster* NormalMonster = Cast<ANormalMonster>(AIController->GetPawn());
	if (!NormalMonster)
	{
		UE_LOG(LogTemp, Warning, TEXT("NormalMonster가 nullptr입니다!"));
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	
	if (!BlackboardComp)
	{
		UE_LOG(LogTemp, Error, TEXT("BlackboardComp가 nullptr입니다!"));
		return EBTNodeResult::Failed;
	}
	return EBTNodeResult::InProgress; // Task 진행 중으로 설정
}

void UNormalMonsterWatingTask::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return;

	bool bPlayerDetected = BlackboardComp->GetValueAsBool(TEXT("PlayerDetectedKey"));
	// UE_LOG(LogTemp, Warning, TEXT("TickTask 실행 중... PlayerDetectedKey = %s"), 
	// 	   bPlayerDetected ? TEXT("true") : TEXT("false"));

	if (bPlayerDetected)
	{
		UE_LOG(LogTemp, Warning, TEXT("플레이어 감지됨! Task 종료."));
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UNormalMonsterWatingTask::UpdateWaiting(ANormalMonster* NormalMonster)
{
	UAnimInstance* AnimInstance = NormalMonster->GetMesh()->GetAnimInstance();
	UGrux_AnimInstance* GruxAnimInstance = Cast<UGrux_AnimInstance>(AnimInstance);

	if (!GruxAnimInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("GruxAnimInstance가 nullptr입니다!"));
		return;
	}

	GruxAnimInstance->IsWaiting = true; // 시작 시 대기 상태로 변경
	UE_LOG(LogTemp, Warning, TEXT("애니메이션 블루프린트: IsWaiting 활성화"));

	// 함수 내부에서 사용할 타이머 핸들 선언
	FTimerHandle WaitingLoopTimerHandle;

	// 일정 시간마다 반복 실행
	NormalMonster->GetWorld()->GetTimerManager().SetTimer(
		WaitingLoopTimerHandle,
		[GruxAnimInstance]()
		{
			GruxAnimInstance->IsWaiting = !GruxAnimInstance->IsWaiting; // 상태 반전
			UE_LOG(LogTemp, Warning, TEXT("애니메이션 블루프린트: IsWaiting 변경 → %s"),
			       GruxAnimInstance->IsWaiting ? TEXT("true") : TEXT("false"));
		},
		GruxAnimInstance->IsWaiting ? 9.0f : 7.0f, true
	);
}
