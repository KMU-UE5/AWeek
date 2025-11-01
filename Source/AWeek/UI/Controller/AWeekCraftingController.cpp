#include "AWeek/UI/Controller/AWeekCraftingController.h"
#include "AWeek/Components/AWeekCraftingComponent.h"
#include "AWeek/Components/AWeekInventoryComponent.h"
#include "AWeek/UI/AWeekGameUIManager.h"
#include "AWeek/Data/AWeekItemCraftingRecipe.h"

void UAWeekCraftingController::InitializeCraftingController(
    TObjectPtr<UAWeekGameUIManager> InUIManager,
    TObjectPtr<UAWeekCraftingComponent> InCraftingComponent,
    TObjectPtr<UAWeekInventoryComponent> InInventoryComponent)
{
    UIManager = InUIManager;
    CraftingComponent = InCraftingComponent;
    InventoryComponent = InInventoryComponent;

    if (!CraftingComponent || !InventoryComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("CraftingController: Invalid components!"));
    }
}

FCraftingResult UAWeekCraftingController::TryCraftRecipe(int32 RecipeIndex, int32 CraftCount)
{
    // 유효성 검사
    FCraftingResult ValidationResult = ValidateCrafting(RecipeIndex, CraftCount);
    if (ValidationResult.Result != ECraftingFailureReason::Success)
    {
        return ValidationResult;
    }

    // 실제 제작 시도
    if (CraftingComponent->TryCraftRecipe(RecipeIndex))
    {
        FAWeekCachedCraftingRecipe Recipe;
        if (CraftingComponent->GetRecipeAt(RecipeIndex, Recipe))
        {
            return FCraftingResult::MakeSuccess(
                Recipe.CraftedItemEntry.Quantity,
                Recipe.CraftedItemEntry.ItemData.TextData.Name
            );
        }
    }

    return FCraftingResult::MakeFailure(
        ECraftingFailureReason::ComponentNotInitialized,
        FText::FromString(TEXT("Crafting failed unexpectedly"))
    );
}

bool UAWeekCraftingController::CanCraftRecipe(int32 RecipeIndex) const
{
    if (!CraftingComponent)
        return false;

    return CraftingComponent->CanCraft(RecipeIndex);
}

FCraftingResult UAWeekCraftingController::ValidateCrafting(int32 RecipeIndex, int32 CraftCount) const
{
    // Component 검사
    if (!CraftingComponent || !InventoryComponent)
    {
        return FCraftingResult::MakeFailure(
            ECraftingFailureReason::ComponentNotInitialized,
            FText::FromString(TEXT("Crafting system not initialized"))
        );
    }

    // 레시피 유효성 검사
    FAWeekCachedCraftingRecipe Recipe;
    if (!CraftingComponent->GetRecipeAt(RecipeIndex, Recipe))
    {
        return FCraftingResult::MakeFailure(
            ECraftingFailureReason::InvalidRecipe,
            FText::FromString(TEXT("Invalid recipe"))
        );
    }

    // 재료 검사
    if (!CraftingComponent->CanCraft(RecipeIndex))
    {
        return FCraftingResult::MakeFailure(
            ECraftingFailureReason::InsufficientMaterials,
            FText::Format(
                FText::FromString(TEXT("Not enough materials to craft {0}")),
                Recipe.CraftedItemEntry.ItemData.TextData.Name
            )
        );
    }

    // 인벤토리 공간 검사
    if (!InventoryComponent->CanAddItem(
        Recipe.CraftedItemEntry.ItemData.ID,
        Recipe.CraftedItemEntry.ItemData.NumericData.Weight,
        Recipe.CraftedItemEntry.Quantity * CraftCount))
    {
        return FCraftingResult::MakeFailure(
            ECraftingFailureReason::InventoryFull,
            FText::FromString(TEXT("Not enough inventory space"))
        );
    }

    return FCraftingResult::MakeSuccess(0, FText::GetEmpty());
}