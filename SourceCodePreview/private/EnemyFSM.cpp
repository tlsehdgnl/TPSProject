// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "TPSCharacter.h"
#include "Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "TPSProject.h"
#include "Components/CapsuleComponent.h"
#include "EnemyAnim.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"

// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), ATPSCharacter::StaticClass());
	target = Cast<ATPSCharacter>(actor);
	me = Cast<AEnemy>(GetOwner());

	anim = Cast<UEnemyAnim>(me->GetMesh()->GetAnimInstance());

	ai = Cast<AAIController>(me->GetController());
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//FString logMsg = UEnum::GetValueAsString(mState);
	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, logMsg);

	switch (mState)
	{
	case EEnemyState::Idle:
		IdleState();
		break;
	case EEnemyState::Move:
		MoveState();
		break;
	case EEnemyState::Attack:
		AttackState();
		break;
	case EEnemyState::Damage:
		DamageState();
		break;
	case EEnemyState::Die:
		DieState();
		break;
	}
}

void UEnemyFSM::IdleState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > idleDelayTime) {
		mState = EEnemyState::Move;
		currentTime = 0;

		anim->animState = mState;

		GetRandomPositionInNavMesh(me->GetActorLocation(), 4000, randomPos);
	}
}

void UEnemyFSM::MoveState()
{
	// 플레이어 위치
	FVector playerPos = target->GetActorLocation();

	// 적(자기 자신) 위치
	FVector myPos = me->GetActorLocation();

	/* ====== 도망 방향 계산 ====== */

	// 플레이어 → 적 방향의 반대 벡터
	// 즉, 플레이어로부터 멀어지는 방향
	FVector fleeDir = (myPos - playerPos).GetSafeNormal();

	// 도망갈 거리 (값이 클수록 더 멀리 도망감)
	float fleeDistance = 100.0f;

	// 도망 목표 위치 = 현재 위치 + 도망 방향 * 거리
	FVector fleePos = myPos + fleeDir * fleeDistance;

	/* ====== NavMesh 위의 유효한 위치인지 확인 ====== */

	// 네비게이션 시스템 가져오기
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	FNavLocation navLoc;

	// 계산된 도망 위치가 NavMesh 위에 있는지 검사
	bool valid = ns->ProjectPointToNavigation(fleePos, navLoc);

	if (valid)
	{
		// NavMesh 위라면 해당 위치로 이동
		ai->MoveToLocation(navLoc.Location, 30.0f);
	}
	else
	{
		// NavMesh 위가 아니라면
		// 반경 내에서 랜덤한 도망 위치를 다시 탐색
		GetRandomPositionInNavMesh(myPos, 800, fleePos);

		// 랜덤으로 찾은 위치로 이동
		ai->MoveToLocation(fleePos);
	}

	/* ====== 상태 전환 조건 ====== */

	// 플레이어와의 현재 거리 계산
	float distance = FVector::Distance(playerPos, myPos);

	// 충분히 멀어졌다면 도망 상태 종료
	if (distance > 1001.0f)
	{
		// Idle 상태로 전환
		mState = EEnemyState::Idle;

		// 애니메이션 상태도 함께 변경
		anim->animState = mState;
	}
}



void UEnemyFSM::AttackState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > attackDelayTime) {
		//PRINT_LOG(TEXT("Attack!!!!!"));
		currentTime = 0;
		anim->bAttackPlay = true;
	}

	float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());
	if (distance > attackRange) {
		mState = EEnemyState::Move;
		anim->animState = mState;

	}
}

void UEnemyFSM::DamageState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > damageDelayTime) {
		mState = EEnemyState::Idle;
		currentTime = 0;
		anim->animState = mState;
	}
}

void UEnemyFSM::DieState()
{
	if (anim->bDieDone == false) {
		return;
	}

	FVector P0 = me->GetActorLocation();
	FVector vt = FVector::DownVector * dieSpeed * GetWorld()->DeltaTimeSeconds;
	FVector P = P0 + vt;
	me->SetActorLocation(P);

	if (P.Z < -200.0f) {
		me->Destroy();
	}
}

void UEnemyFSM::OnDamageProcess()
{
	//me->Destroy();
	hp--;
	if (hp > 0) {
		mState = EEnemyState::Damage;

		int32 index = FMath::RandRange(0, 2);
		FString sectionName = FString::Printf(TEXT("Damage%d"), index);
		anim->PlayDamageAnim(FName(*sectionName));

	}
	else {
		mState = EEnemyState::Die;
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		anim->PlayDamageAnim(TEXT("Die"));
	}
	anim->animState = mState;

	ai->StopMovement();
}

bool UEnemyFSM::GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest)
{
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation loc;
	bool result = ns->GetRandomReachablePointInRadius(centerLocation, radius, loc);
	dest = loc.Location;
	return result;
}