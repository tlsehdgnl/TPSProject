# 🎮 Hunter in Maze (TPS Project)

## 📌 프로젝트 소개

Unreal Engine 5 기반으로 개발한 3인칭 슈팅 게임입니다.
플레이어는 제한 시간 내에 **랜덤으로 생성되는 미로**에서
도망가는 적을 추격하여 최대한 많은 점수를 획득해야 합니다.

---

## 🚀 핵심 기능

### 🧩 Procedural Maze Generation

* DFS(Recursive Backtracking) 알고리즘 기반 미로 생성
* 게임 시작 시마다 새로운 맵 생성
* Runtime에서 동적 생성

### 🤖 Enemy AI (Distance-based FSM)

* 플레이어와의 거리 기반 상태 전환
* 일정 거리 이하 → 도망(Flee)
* NavMesh 기반 이동 처리
* Squared Distance 계산으로 최적화

### 🔫 Shooting System

* 마우스 입력 기반 연사(Full Auto)
* 발사 쿨타임 제어 (Fire Rate Control)
* 입력 이벤트 기반 처리 (Triggered / Started)

### ⏱️ Time Attack System

* 제한 시간 내 점수 획득
* GameState 기반 타이머 관리
* UI 실시간 바인딩

---

## 🧠 시스템 구조

* Game Manager: 게임 상태 및 루프 관리
* Maze System (C++): 미로 생성 및 배치
* Enemy AI (C++): 거리 기반 FSM
* Player: 이동 및 무기 시스템
* UI System: 타이머 / 점수 / 상태 표시

---

## 🛠️ 사용 기술

* Unreal Engine 5
* C++
* Blueprint
* NavMesh
* Git

---

## 🎮 조작 방법

* 이동: WASD
* 달리기: Shift
* 조준: Ctrl
* 발사: 마우스 좌클릭
* 무기 변경: 1 / 2

---

## 📷 플레이 영상

👉 (여기에 영상 또는 GIF 넣기)

---

## 💡 기술적 도전과 해결

### 1. 미로 생성 최적화

DFS 기반 생성 과정에서
불필요한 연산을 줄이고, 한 번만 생성되도록 Tick 비활성화

### 2. AI 이동 문제

도망 위치가 NavMesh 밖으로 나가는 문제 발생
→ `ProjectPointToNavigation` 사용으로 해결

### 3. 연사 구현

입력 이벤트 처리 방식 변경 (Started → Triggered)
→ 버튼 유지 시 연속 발사 가능하도록 개선

---

## 👥 역할 분담

* 신동휘

  * Enemy AI 시스템
  * UI 설계 및 로직
  * 미로 알고리즘 개발
  * 미로 생성 시스템 개발

* 윤태웅

  * UI 데이터 연동
  * 무기 및 탄약 시스템

---

## 📌 회고

제한된 시간과 인원으로 인해
핵심 재미(Core Fun)에 집중하는 방향으로 개발을 진행했습니다.

특히 **절차적 생성과 AI 로직 구현 과정에서 많은 문제 해결 경험**을 얻을 수 있었습니다.
