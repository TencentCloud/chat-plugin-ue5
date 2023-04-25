// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE5_PluginCharacter.h"
#include "UE5_PluginProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TencentCloudChat.h"


class SendMessageCallback : public V2TIMSendCallback {
  public:

	 void OnSuccess(const V2TIMMessage &message) override{
        UE_LOG(LogTemp,Log,TEXT("=== SendCallback OnSuccess ======"));
		
    };
    void OnError(int error_code, const V2TIMString &error_message) override {
        UE_LOG(LogTemp,Log,TEXT("=== SendCallback OnError error code:%d ======"),error_code);
    };
    void OnProgress(uint32_t progress) override {
      UE_LOG(LogTemp,Log,TEXT("=== SendCallback OnProgress progress:%d ======"),progress);
    };
  };
 class LoginCallback : public V2TIMCallback {
    
    public:
  
      void OnSuccess() override {
        UE_LOG(LogTemp,Log,TEXT("<== login OnSuccess"));
		V2TIMString textmessage = static_cast<V2TIMString>("message from ue5");
		V2TIMString groupid = static_cast<V2TIMString>("@TGS#1F33BWOM6");
		V2TIMString rec = static_cast<V2TIMString>("");
		V2TIMMessage msg = TencentCloudChat::CreateTextMessage(textmessage);
		SendMessageCallback* sb = new SendMessageCallback();
		TencentCloudChat::SendMessage(msg, "",groupid, V2TIM_PRIORITY_DEFAULT,false,V2TIMOfflinePushInfo(),sb);
      };
      void OnError(int error_code, const V2TIMString &error_message) override {
        UE_LOG(LogTemp,Log,TEXT("<== login failed OnError ======: %d"), error_code);
      };
  };

//////////////////////////////////////////////////////////////////////////
// AUE5_PluginCharacter
 
  
    
AUE5_PluginCharacter::AUE5_PluginCharacter()
{
	// Character doesnt have a rifle at start
	bHasRifle = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

}

void AUE5_PluginCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	V2TIMSDKConfig Config;
	Config.logLevel = V2TIM_LOG_DEBUG;
	#if PLATFORM_ANDROID
		Config.initPath = static_cast<V2TIMString>("/sdcard/Android/data/com.YourCompany.UE5_Plugin/files/UnrealGame/UE5_Plugin");
		Config.logPath = static_cast<V2TIMString>("/sdcard/Android/data/com.YourCompany.UE5_Plugin/files/UnrealGame/UE5_Plugin");
	#endif
	bool initSuccess = TencentCloudChat::InitSDK(0,Config); // replace 0 with your sdkappid
	if(initSuccess){
		UE_LOG(LogTemp,Log,TEXT("TencentCloudChat Log============================================================================================= Init OnSuccess in work "));
		 V2TIMString userid = static_cast<V2TIMString>("your login userid");// your userid 
		 V2TIMString userSig = static_cast<V2TIMString>("your usersig "); // your usersig . you can get it here https://console.cloud.tencent.com/im/tool-usersig 
		LoginCallback* login_callback_ = new LoginCallback();
		TencentCloudChat::Login(userid,userSig,login_callback_);

		// FMessageDialog::Open(EAppMsgType::Ok, text);
	}
	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

}

//////////////////////////////////////////////////////////////////////////// Input

void AUE5_PluginCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AUE5_PluginCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AUE5_PluginCharacter::Look);
	}
}


void AUE5_PluginCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
		UE_LOG(LogTemp,Log,TEXT("<== move ==========================================================================="));
		
	}
}

void AUE5_PluginCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AUE5_PluginCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool AUE5_PluginCharacter::GetHasRifle()
{
	return bHasRifle;
}