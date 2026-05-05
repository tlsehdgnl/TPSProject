#include "MazeGenerator.h"
#include "Kismet/KismetMathLibrary.h"

// 생성자
AMazeGenerator::AMazeGenerator()
{
    // Tick을 사용하지 않음 (미로는 시작 시 한 번만 생성)
    PrimaryActorTick.bCanEverTick = false;
}

// 게임 시작 시 호출
void AMazeGenerator::BeginPlay()
{
    Super::BeginPlay();

    // 미로 배열 초기화
    InitMaze();

    // (1,1) 위치부터 미로 생성 시작 (외곽 벽 보존)
    GenerateMaze(1, 1);

    // 생성된 미로 데이터를 기반으로 액터 배치
    SpawnMaze();
}

// 미로 배열 초기화 함수
void AMazeGenerator::InitMaze()
{
    // 세로 크기만큼 2차원 배열 생성
    Maze.SetNum(Height);

    for (int32 y = 0; y < Height; y++)
    {
        // 가로 크기 설정
        Maze[y].SetNum(Width);

        for (int32 x = 0; x < Width; x++)
        {
            // false = 벽, true = 길
            // 처음에는 전부 벽으로 초기화
            Maze[y][x] = false;
        }
    }
}

// DFS(깊이 우선 탐색)를 이용한 미로 생성 함수
void AMazeGenerator::GenerateMaze(int32 X, int32 Y)
{
    // 현재 위치를 길로 설정
    Maze[Y][X] = true;

    // 이동 방향 (상, 하, 좌, 우)
    TArray<FIntPoint> Directions = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}
    };

    // 방향 배열을 랜덤으로 섞어서
    // 미로가 매번 다르게 생성되도록 함
    for (int32 i = 0; i < Directions.Num(); ++i)
    {
        int32 RandIndex = FMath::RandRange(0, Directions.Num() - 1);
        Directions.Swap(i, RandIndex);
    }

    // 모든 방향에 대해 탐색
    for (auto D : Directions)
    {
        // 두 칸 앞을 기준으로 다음 셀 계산
        int32 NX = X + D.X * 2;
        int32 NY = Y + D.Y * 2;

        // 미로 범위 체크 (외곽 벽 유지)
        if (NX > 0 && NX < Width - 1 && NY > 0 && NY < Height - 1)
        {
            // 아직 방문하지 않은 셀이라면
            if (!Maze[NY][NX])
            {
                // 현재 위치와 다음 위치 사이의 벽을 길로 변경
                Maze[Y + D.Y][X + D.X] = true;

                // 다음 위치에서 재귀적으로 미로 생성
                GenerateMaze(NX, NY);
            }
        }
    }
}

// 미로 데이터를 실제 액터로 배치하는 함수
void AMazeGenerator::SpawnMaze()
{
    UWorld* World = GetWorld();
    if (!World) return;

    // 미로 시작 위치
    FVector Origin = GetActorLocation();

    for (int32 y = 0; y < Height; y++)
    {
        for (int32 x = 0; x < Width; x++)
        {
            // 셀 위치 계산
            FVector Pos = Origin + FVector(x * CellSize, y * CellSize, 0.f);

            if (Maze[y][x]) // 길인 경우
            {
                if (FloorClass)
                {
                    // 바닥 액터 생성
                    World->SpawnActor<AActor>(FloorClass, Pos, FRotator::ZeroRotator);
                }
            }
            else // 벽인 경우
            {
                if (WallClass)
                {
                    // 벽 액터 생성
                    World->SpawnActor<AActor>(WallClass, Pos, FRotator::ZeroRotator);
                }
            }
        }
    }
}
