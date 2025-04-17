# ElvenRing - 소울라이크 액션 RPG 게임

## 🎮 프로젝트 개요
ElvenRing은 언리얼 엔진을 사용하여 개발된 엘든 링에서 영감을 받은 소울라이크 액션 RPG 게임입니다. 이 프로젝트는 FromSoftware 게임의 특징인 도전적인 전투 메커니즘, 분위기 있는 월드 디자인, 그리고 보스 전투 경험을 재현하는 것을 목표로 합니다.
Actor Replicatoin, RPC를 통한 기본적인 동기화를 이용해서 네트워크에 대한 이해를 할 수 있었습니다.
- 개발 기간 : 3/31 ~ 4/18(3주)
- 개발 언어 및 도구 : C++, Unreal Engine, Git

## 팀원
- 김경수 : 팀장, Game Framework
- 조선욱 : 캐릭터
- 김대영 : 보스 몬스터 AI
- 백의현 : 일반 몬스터 AI
- 김세원 : UI

## ✨ 주요 기능

### 📊 전투 시스템
* 연속 공격이 가능한 반응성 높은 근접 전투
* 회피, 달리기, 공격 등을 포함한 스태미나 기반 액션
* 방어 및 패링 메커니즘을 통한 방어적 움직임
* 체력, 스태미나, 마나 자원 관리

### 🧙 캐릭터 매매커니즘
* 회피, 구르기, 점프가 포함된 동적 이동 시스템
* 충돌 감지를 갖춘 무기 기반 전투
* 캐릭터 스탯 관리 및 진행

### 🔥 보스 전투 시스템
* 상태 기계 아키텍처를 사용한 상태 기반 보스 AI
* 일반 및 특수 공격이 포함된 복잡한 보스 공격 패턴
* 각각 고유한 전투 스타일과 메커니즘을 가진 다양한 보스 설계
* 페이즈 기반 보스 전투 시스템

### 🛠️ 기술적 특징
* 언리얼 엔진 기반 개발
* 상태 패턴을 활용한 캐릭터 및 보스 AI 설계
* 네트워크 멀티플레이어 지원
* 애니메이션 시스템과 연동된 전투 메커니즘

## 🏆 보스 구현

### Artorias
* 다양한 검 공격 패턴과 회피 기술을 가진 기사형 보스
* 특수 기술: 수평 베기, 수직 베기, 회전 공격, 점프 공격, 찌르기

### Tenebris
* 두 페이즈로 진행되며 지진, 꼬리 공격, 불 뿜기 등의 공격을 사용하는 용형 보스
* 1페이즈: 꼬리 공격, 지진 공격, 붙잡기 공격
* 2페이즈: 포효, 전방 불 뿜기, 에너지 공격

### Hanged Man
* 전기 기반 원거리 공격과 페이즈 변화 시스템을 갖춘 보스
* 특수 기술: 번개 공격, 전기 창, 천둥 공격

## 🤖 인공지능 시스템 상세

### 기본 AI 아키텍처

ElvenRing의 AI 시스템은 언리얼 엔진의 비헤이비어 트리와 블랙보드를 기반으로 구축되었습니다. 이 시스템은 다양한 적 캐릭터에 대한 지능적이고 역동적인 행동을 구현합니다.

#### 🔄 상태 시스템

일반 몬스터는 다음과 같은 상태 전환 시스템을 갖추고 있습니다:
- **대기(Waiting)**: 플레이어 감지 전 기본 상태
- **감지(Detection)**: 플레이어 감지 후 상태
- **공격(Attack)**: 공격 범위 내 플레이어 존재 시 상태
- **피격(Hit)**: 데미지 받았을 때 상태
- **사망(Death)**: HP가 0이 되었을 때 상태

### 🧟 몬스터 종류

#### 일반 몬스터 (NormalMonster)

`NormalMonster` 클래스는 기본적인 적 캐릭터를 위한 기반 클래스로 다음과 같은 특징을 제공합니다:

#### 광폭 몬스터 (RampageMonster)

`RampageMonster`는 `NormalMonster`를 확장하여 더 공격적인 행동 패턴을 구현합니다:


## 💻 기술적 구현

#### 네트워크 멀티플레이어 지원

모든 AI 액션은 네트워크 멀티플레이어를 지원하도록 구현되었습니다:

```cpp
UFUNCTION(NetMulticast, Reliable)
virtual void MulticastIsHit(bool value, FVector HitLocation, FRotator HitRotation);

UFUNCTION(NetMulticast, Reliable)
virtual void MulticastIsAttack(bool value);

UFUNCTION(NetMulticast, Reliable)
virtual void MulticastIsDeath(bool value);
```

#### UI 통합

몬스터는 HP 표시를 위한 위젯 컴포넌트를 포함합니다:

```cpp
UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI")
UWidgetComponent* HPWidgetComponent;
```

이를 통해 플레이어는 현재 몬스터의 상태를 시각적으로 확인할 수 있습니다.

## 📄 라이선스
이 프로젝트는 개인 포트폴리오 용도로 제작되었으며, 상업적 사용을 목적으로 하지 않습니다.
© 2025 ElvenRing Project
