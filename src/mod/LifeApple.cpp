#include "mod/LifeApple.h"

#include "ll/api/mod/RegisterHelper.h"

namespace life_apple {

LifeApple& LifeApple::getInstance() {
    static LifeApple instance;
    return instance;
}

bool LifeApple::load() { return true; }

bool LifeApple::enable() { return true; }

bool LifeApple::disable() { return true; }

} // namespace life_apple

LL_REGISTER_MOD(life_apple::LifeApple, life_apple::LifeApple::getInstance());

#include "ll/api/memory/Hook.h"

#include "mc/network/ServerNetworkHandler.h"
#include "mc/server/ServerPlayer.h"

#include "mc/nbt/CompoundTag.h"
#include "mc/world/attribute/SharedAttributes.h"

#include "mc/dataloadhelper/DataLoadHelper.h"
#include "mc/world/attribute/Attribute.h"
#include "mc/world/attribute/BaseAttributeMap.h"


#include "ll/api/memory/Hook.h"
#include "mc/world/actor/monster/Shulker.h"
#include "mc/world/actor/player/Player.h"

#include "mc/nbt/CompoundTag.h"
#include "mc/platform/UUID.h"
#include "mc/world/Container.h"
#include "mc/world/attribute/AttributeModifier.h"
#include "mc/world/attribute/AttributeModifierOperation.h"
#include "mc/world/attribute/AttributeOperands.h"
#include "mc/world/attribute/MutableAttributeWithContext.h"
#include "mc/world/attribute/SharedAttributes.h"
#include "mc/world/item/VanillaItemNames.h"

#include "mc/deps/core/string/HashedString.h"
#include "mc/world/attribute/AttributeModificationContext.h"


#include "mc/world/attribute/AttributeInstance.h"

static const mce::UUID healthBoostApple = mce::UUID::fromString("bf459e5c-7724-419d-be69-8fef8eefad84");


LL_AUTO_TYPE_INSTANCE_HOOK(
    PlayerEat,
    ll::memory::HookPriority::Normal,
    Player,
    &Player::eat,
    void,
    ::ItemStack const& instance
) {
    origin(instance);

    if (instance.getFullNameHash() != VanillaItemNames::Apple()) {
        return;
    }

    auto health = getMutableAttribute(SharedAttributes::HEALTH());
    if (!health.mInstance) {
        return;
    }

    auto it = std::find_if(
        health.mInstance->mModifierList->begin(),
        health.mInstance->mModifierList->end(),
        [](auto const& modifier) { return modifier.mId == healthBoostApple; }
    );

    float newCount = it != health.mInstance->mModifierList->end() ? it->mAmount + 1.0f : 1.0f;

    if (newCount > 20.0f) {
        return;
    }

    AttributeModifier modifier(
        healthBoostApple,
        "healthBoostApple",
        newCount,
        int(AttributeModifierOperation::OperationAddition),
        int(AttributeOperands::OperandMax),
        true
    );

    if (it != health.mInstance->mModifierList->end()) {
        health.mInstance->updateModifier(modifier, health.mContext);
    } else {
        health.mInstance->addModifier(modifier, health.mContext);
    }
}


LL_AUTO_TYPE_STATIC_HOOK(
    SharedAttributesLoadAttirbute,
    HookPriority::Normal,
    SharedAttributes,
    &SharedAttributes::_loadAttribute,
    void,
    ::AttributeInstance&            instance,
    const ::CompoundTag*            tag,
    ::AttributeModificationContext& context
) {
    origin(instance, tag, context);
    if (tag && *instance.mAttribute->mName == *SharedAttributes::HEALTH().mName) {
        instance.mCurrentValue = tag->as<CompoundTag>()["Current"];
    }
}