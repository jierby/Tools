//关键

#include "Engine.h"
#include "Async/Async.h"

//任务一
class TEST_API Add_Test :public FNonAbandonableTask
{
public:
	//friend class FAutoDeleteAsyncTask<FTestTask>;
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FTestTask, STATGROUP_ThreadPoolAsyncTasks);
	}
	Add_Test(int32& InResult, FCriticalSection& InCriticalSection) :Result(InResult), CriticalSection(InCriticalSection) {};
	void DoWork()
	{
		for (int i = 0; i < 10000; ++i)
		{
			//锁
			FScopeLock Lock(&CriticalSection);
			Result += i;
			UE_LOG(LogTemp, Warning, TEXT("+++ %d"), Result);
		}
	};
	int32& Result;
	FCriticalSection& CriticalSection;
};


//任务二
class TEST_API Sub_Test :public FNonAbandonableTask
{
public:
	//friend class FAutoDeleteAsyncTask<FTestTask>;
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FTestTask, STATGROUP_ThreadPoolAsyncTasks);
	}
	Sub_Test(int32& InResult, FCriticalSection& InCriticalSection) :Result(InResult), CriticalSection(InCriticalSection) {};
	void DoWork()
	{
		for (int i = 0; i < 10000; ++i)
		{
			//锁
			FScopeLock Lock(&CriticalSection);
			Result -= i;
			UE_LOG(LogTemp, Warning, TEXT("---  %d"), Result);
		}
	};
	int32& Result;
	FCriticalSection& CriticalSection;
};



UCLASS()
class TEST_API AMyQueue : public AActor
{
	GENERATED_BODY()

public:
	//锁
	static FCriticalSection CriticalSection;//在CPP中初始化：FCriticalSection AMyQueue::CriticalSection;
	static AMyQueue* Myqueue;//在CPP中初始化：AMyQueue* AMyQueue::Myqueue = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int a = 9;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 Result = 0;//异步操作变量
	UFUNCTION(BlueprintCallable)
		static AMyQueue* GetInstance(int _a)
	{
		if (Myqueue == nullptr) Myqueue = NewObject<AMyQueue>();
		UE_LOG(LogTemp, Warning, TEXT("Instance:a=%d"), Myqueue->a);
		Myqueue->a = _a;
		UE_LOG(LogTemp, Warning, TEXT("Instance:a=%d"), Myqueue->a);
		return Myqueue;
	}
	//异步任务一
	UFUNCTION(BlueprintCallable)
	void StartTask1() 
	{
		//创建任务
		FAutoDeleteAsyncTask<Add_Test>* AddAsyncTask = new FAutoDeleteAsyncTask<Add_Test>(Result, CriticalSection);
		//开始任务
		AddAsyncTask->StartBackgroundTask();

	}
	//异步任务二
	UFUNCTION(BlueprintCallable)
	void StartTask2() 
	{
		FAutoDeleteAsyncTask<Sub_Test>* SubAsyncTask = new FAutoDeleteAsyncTask<Sub_Test>(Result, CriticalSection);
		SubAsyncTask->StartBackgroundTask();
	}

	UFUNCTION(BlueprintCallable)
		void StartTaskMulti() {
		FAutoDeleteAsyncTask<Add_Test>* AddAsyncTask = new FAutoDeleteAsyncTask<Add_Test>(Result, CriticalSection);
		FAutoDeleteAsyncTask<Sub_Test>* SubAsyncTask = new FAutoDeleteAsyncTask<Sub_Test>(Result, CriticalSection);
		AddAsyncTask->StartBackgroundTask();
		SubAsyncTask->StartBackgroundTask();
	}

	UFUNCTION(BlueprintCallable)
		void TestQueue(FString Item) { GEngine->AddOnScreenDebugMessage(1, 5, FColor::Green, Item); UE_LOG(LogTemp, Warning, TEXT("%s"), *Item); return; };
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
	AMyQueue() {};
	~AMyQueue() {};

};
