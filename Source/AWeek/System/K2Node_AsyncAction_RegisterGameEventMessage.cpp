// Fill out your copyright notice in the Description page of Project Settings.


#include "K2Node_AsyncAction_RegisterGameEventMessage.h"

#include "AsyncAction_RegisterGameEventMessage.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintFunctionNodeSpawner.h"

namespace UK2Node_AsyncAction_RegisterGameEventMessagesName
{
	static FName PayloadPinName = "Payload";
	static FName PayloadTypePinName = "PayloadType";
}

void UK2Node_AsyncAction_RegisterGameEventMessage::PostReconstructNode()
{
	Super::PostReconstructNode();

	RefreshOutputPin();
}

void UK2Node_AsyncAction_RegisterGameEventMessage::PinDefaultValueChanged(UEdGraphPin* Pin)
{
	Super::PinDefaultValueChanged(Pin);

	// PayloadType이 변경됐을때 갱신
	if (Pin == GetPayloadTypePin())
	{
		RefreshOutputPin();
	}
}

void UK2Node_AsyncAction_RegisterGameEventMessage::GetPinHoverText(const UEdGraphPin& Pin, FString& HoverTextOut) const
{
	Super::GetPinHoverText(Pin, HoverTextOut);
}

void UK2Node_AsyncAction_RegisterGameEventMessage::GetMenuActions(
	FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	// UK2Node_AsyncAction 부모 를래스 GetMenuAction에서 타입만 바꾼거 
	struct GetMenuActions_Utils
	{
		static void SetNodeFunc(UEdGraphNode* NewNode, bool /*bIsTemplateNode*/, TWeakObjectPtr<UFunction> FunctionPtr)
		{
			UK2Node_AsyncAction_RegisterGameEventMessage* AsyncTaskNode = CastChecked<UK2Node_AsyncAction_RegisterGameEventMessage>(NewNode);
			if (FunctionPtr.IsValid())
			{
				UFunction* Func = FunctionPtr.Get();
				FObjectProperty* ReturnProp = CastFieldChecked<FObjectProperty>(Func->GetReturnProperty());
						
				AsyncTaskNode->ProxyFactoryFunctionName = Func->GetFName();
				AsyncTaskNode->ProxyFactoryClass        = Func->GetOuterUClass();
				AsyncTaskNode->ProxyClass               = ReturnProp->PropertyClass;
			}
		}
	};

	UClass* NodeClass = GetClass();
	ActionRegistrar.RegisterClassFactoryActions<UAsyncAction_RegisterGameEventMessage>(FBlueprintActionDatabaseRegistrar::FMakeFuncSpawnerDelegate::CreateLambda([NodeClass](const UFunction* FactoryFunc)->UBlueprintNodeSpawner*
	{
		UBlueprintNodeSpawner* NodeSpawner = UBlueprintFunctionNodeSpawner::Create(FactoryFunc);
		check(NodeSpawner != nullptr);
		NodeSpawner->NodeClass = NodeClass;

		TWeakObjectPtr<UFunction> FunctionPtr = MakeWeakObjectPtr(const_cast<UFunction*>(FactoryFunc));
		NodeSpawner->CustomizeNodeDelegate = UBlueprintNodeSpawner::FCustomizeNodeDelegate::CreateStatic(GetMenuActions_Utils::SetNodeFunc, FunctionPtr);

		return NodeSpawner;
	}) );
}

void UK2Node_AsyncAction_RegisterGameEventMessage::AllocateDefaultPins()
{
	Super::AllocateDefaultPins();

	// WildCard 형식으로 Pin 생성 Payload 타입이 변경되므로
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, UK2Node_AsyncAction_RegisterGameEventMessagesName::PayloadPinName);
}

bool UK2Node_AsyncAction_RegisterGameEventMessage::HandleDelegates(
	const TArray<FBaseAsyncTaskHelper::FOutputPinAndLocalVariable>& VariableOutputs, UEdGraphPin* ProxyObjectPin,
	UEdGraphPin*& InOutLastThenPin, UEdGraph* SourceGraph, FKismetCompilerContext& CompilerContext)
{
	return Super::HandleDelegates(VariableOutputs, ProxyObjectPin, InOutLastThenPin, SourceGraph, CompilerContext);
}

void UK2Node_AsyncAction_RegisterGameEventMessage::RefreshOutputPin()
{
	UEdGraphPin* PayloadPin = GetPayloadPin();
	UEdGraphPin* PayloadTypePin = GetPayloadTypePin();

	// 현재 Payload 타입과 출력하고자하는 PayloadType이 다르면 갱신
	if (PayloadPin->PinType.PinSubCategoryObject != PayloadTypePin->DefaultObject)
	{
		// 에디터 상에서 Payload가 분해돼서 사용되는 중이면 Recombine 후 타입 변경
		if (PayloadPin->SubPins.Num() > 0)
		{
			GetSchema()->RecombinePin(PayloadPin);
		}

		PayloadPin->PinType.PinSubCategoryObject = PayloadTypePin->DefaultObject;
		PayloadPin->PinType.PinCategory = (PayloadTypePin->DefaultObject == nullptr)?
		UEdGraphSchema_K2::PC_Wildcard : UEdGraphSchema_K2::PC_Struct;
	}
}

UEdGraphPin* UK2Node_AsyncAction_RegisterGameEventMessage::GetPayloadPin() const
{
	UEdGraphPin* PayloadPin = FindPinChecked(UK2Node_AsyncAction_RegisterGameEventMessagesName::PayloadPinName);
	check(PayloadPin->Direction == EGPD_Output)

	return PayloadPin;
}

UEdGraphPin* UK2Node_AsyncAction_RegisterGameEventMessage::GetPayloadTypePin() const
{
	UEdGraphPin* PayloadTypePin = FindPinChecked(UK2Node_AsyncAction_RegisterGameEventMessagesName::PayloadTypePinName);
	check(PayloadTypePin->Direction == EGPD_Input)

	return PayloadTypePin;
}
