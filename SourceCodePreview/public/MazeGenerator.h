#pragma once   // 헤더 파일 중복 포함 방지

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MazeGenerator.generated.h"

// 미로를 자동으로 생성하는 액터 클래스
UCLASS()
class TPSPROJECT_API AMazeGenerator : public AActor
{
    GENERATED_BODY()

public:
    // 생성자
    AMazeGenerator();

protected:
    // 게임 시작 시 한 번 호출되는 함수
    virtual void BeginPlay() override;

public:

    /* ====== 미로 설정 값 ====== */

    // 미로의 가로 크기 (셀 개수)
    UPROPERTY(EditAnywhere, Category = "Maze")
    int32 Width = 20;

    // 미로의 세로 크기 (셀 개수)
    UPROPERTY(EditAnywhere, Category = "Maze")
    int32 Height = 20;

    // 한 칸(Cell)의 실제 월드 크기
    UPROPERTY(EditAnywhere, Category = "Maze")
    float CellSize = 200.f;

    // 벽으로 사용할 액터 클래스 (에디터에서 지정)
    UPROPERTY(EditAnywhere, Category = "Maze")
    TSubclassOf<AActor> WallClass;

    // 바닥(길)으로 사용할 액터 클래스 (에디터에서 지정)
    UPROPERTY(EditAnywhere, Category = "Maze")
    TSubclassOf<AActor> FloorClass;

private:
    /* ====== 내부 미로 데이터 ====== */

    // 미로 구조를 저장하는 2차원 배열
    // true  : 길
    // false : 벽
    TArray<TArray<bool>> Maze;

    /* ====== 내부 동작 함수 ====== */

    // 미로 배열을 초기화 (전부 벽으로 설정)
    void InitMaze();

    // DFS(깊이 우선 탐색)를 이용해 미로 생성
    void GenerateMaze(int32 X, int32 Y);

    // 미로 데이터 기반으로 벽/바닥 액터를 실제로 생성
    void SpawnMaze();
};
