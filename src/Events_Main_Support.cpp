
#include "ActorManipulation.h"
#include "Events.h"
#include "Utility.h"

namespace Events
{
#define EvalProcessing()   \
	if (!CanProcess()) \
		return;

	/// <summary>
	/// initializes important variables, which need to be initialized every time a game is loaded
	/// </summary>
	void Main::InitializeCompatibilityObjects()
	{
		EvalProcessing();
		// now that the game was loaded we can try to initialize all our variables we conuldn't before
		if (!initialized) {
			// if we are in com mode, try to find the needed items. If we cannot find them, deactivate comp mode
			initialized = true;
		}
	}

	/// <summary>
	/// Calculates the cooldowns of an actor for a specific effect
	/// </summary>
	void Main::CalcActorCooldowns(std::shared_ptr<ActorInfo> acinfo, AlchemicEffect effect, int dur)
	{
		EvalProcessing();
		if ((effect & AlchemicEffect::kHealth).IsValid()) {
			acinfo->SetDurHealth(CalcPotionDuration(dur));
		}
		if ((effect & AlchemicEffect::kMagicka).IsValid()) {
			acinfo->SetDurMagicka(CalcPotionDuration(dur));
		}
		if ((effect & AlchemicEffect::kStamina).IsValid()) {
			acinfo->SetDurStamina(CalcPotionDuration(dur));
		}
		if ((effect & AlchemicEffect::kAnyRegen).IsValid()) {
			acinfo->SetDurRegeneration(CalcRegenerationDuration(dur));
		}
		if ((effect & AlchemicEffect::kAnyFortify).IsValid()) {
			acinfo->SetDurFortify(CalcFortifyDuration(dur));
		}
	}

	/// <summary>
	/// Calculates poison effects based on [combatdata], [target], and [tcombatdata]
	/// </summary>
	/// <param name="combatdata">combatdata of the actor using poison</param>
	/// <param name="target">target</param>
	/// <param name="tcombatdata">combatdata of the target</param>
	/// <returns>valid poison effects</returns>
	AlchemicEffect Main::CalcPoisonEffects(uint32_t combatdata, RE::Actor* target, uint32_t tcombatdata)
	{
		LOG_4("{}[Events] [CalcPoisonEffects]");
		AlchemicEffect effects = 0;
		effects |= AlchemicEffect::kDamageResist |
		           AlchemicEffect::kResistMagic |
		           AlchemicEffect::kPoisonResist |
		           AlchemicEffect::kResistDisease |
		           AlchemicEffect::kParalysis |
		           AlchemicEffect::kFear |
		           AlchemicEffect::kFrenzy |
		           AlchemicEffect::kCarryWeight |
		           AlchemicEffect::kReflectDamage |
		           AlchemicEffect::kSpeedMult |
		           AlchemicEffect::kFortifyHealth |
		           AlchemicEffect::kHealRate |
		           AlchemicEffect::kHealRateMult |
		           AlchemicEffect::kHealth;
		if (tcombatdata & static_cast<uint32_t>(Utility::CurrentCombatStyle::Spellsword)) {
			effects |= AlchemicEffect::kOneHanded |
			           AlchemicEffect::kMeleeDamage |
			           AlchemicEffect::kWeaponSpeedMult |
			           AlchemicEffect::kAttackDamageMult |
			           AlchemicEffect::kCriticalChance |
			           AlchemicEffect::kFortifyMagicka |
			           AlchemicEffect::kMagickaRate |
			           AlchemicEffect::kMagickaRateMult |
			           AlchemicEffect::kFortifyStamina |
			           AlchemicEffect::kStaminaRate |
			           AlchemicEffect::kStaminaRateMult |
			           AlchemicEffect::kStamina;
		}
		if (tcombatdata & static_cast<uint32_t>(Utility::CurrentCombatStyle::Staffsword)) {
			effects |= AlchemicEffect::kOneHanded |
			           AlchemicEffect::kMeleeDamage |
			           AlchemicEffect::kWeaponSpeedMult |
			           AlchemicEffect::kAttackDamageMult |
			           AlchemicEffect::kCriticalChance |
			           AlchemicEffect::kFortifyMagicka |
			           AlchemicEffect::kMagickaRate |
			           AlchemicEffect::kMagickaRateMult |
			           AlchemicEffect::kFortifyStamina |
			           AlchemicEffect::kStaminaRate |
			           AlchemicEffect::kStaminaRateMult |
			           AlchemicEffect::kStamina |
			           AlchemicEffect::kMagicka;
		}
		if (tcombatdata & static_cast<uint32_t>(Utility::CurrentCombatStyle::OneHandedShield)) {
			effects |= AlchemicEffect::kOneHanded |
			           AlchemicEffect::kBlock |
			           AlchemicEffect::kWeaponSpeedMult |
			           AlchemicEffect::kAttackDamageMult |
			           AlchemicEffect::kCriticalChance |
			           AlchemicEffect::kFortifyStamina |
			           AlchemicEffect::kStaminaRate |
			           AlchemicEffect::kStaminaRateMult |
			           AlchemicEffect::kStamina;
		}
		if (tcombatdata & static_cast<uint32_t>(Utility::CurrentCombatStyle::TwoHanded)) {
			effects |= AlchemicEffect::kTwoHanded |
			           AlchemicEffect::kBlock |
			           AlchemicEffect::kMeleeDamage |
			           AlchemicEffect::kWeaponSpeedMult |
			           AlchemicEffect::kAttackDamageMult |
			           AlchemicEffect::kCriticalChance |
			           AlchemicEffect::kFortifyStamina |
			           AlchemicEffect::kStaminaRate |
			           AlchemicEffect::kStaminaRateMult |
			           AlchemicEffect::kStamina;
		}
		if (tcombatdata & static_cast<uint32_t>(Utility::CurrentCombatStyle::Ranged)) {
			effects |= AlchemicEffect::kArchery |
			           AlchemicEffect::kWeaponSpeedMult |
			           AlchemicEffect::kAttackDamageMult |
			           AlchemicEffect::kBowSpeed |
			           AlchemicEffect::kCriticalChance |
			           AlchemicEffect::kFortifyStamina |
			           AlchemicEffect::kStaminaRate |
			           AlchemicEffect::kStaminaRateMult |
			           AlchemicEffect::kStamina;
		}
		if (tcombatdata & static_cast<uint32_t>(Utility::CurrentCombatStyle::DualWield)) {
			effects |= AlchemicEffect::kOneHanded |
			           AlchemicEffect::kWeaponSpeedMult |
			           AlchemicEffect::kAttackDamageMult |
			           AlchemicEffect::kCriticalChance |
			           AlchemicEffect::kFortifyStamina |
			           AlchemicEffect::kStaminaRate |
			           AlchemicEffect::kStaminaRateMult |
			           AlchemicEffect::kStamina;
		}
		if (tcombatdata & static_cast<uint32_t>(Utility::CurrentCombatStyle::HandToHand)) {
			effects |= AlchemicEffect::kUnarmedDamage |
			           AlchemicEffect::kAttackDamageMult |
			           AlchemicEffect::kFortifyStamina |
			           AlchemicEffect::kStaminaRate |
			           AlchemicEffect::kStaminaRateMult |
			           AlchemicEffect::kStamina;
		}
		if (tcombatdata & static_cast<uint32_t>(Utility::CurrentCombatStyle::DualStaff)) {
			effects |= AlchemicEffect::kFortifyMagicka |
			           AlchemicEffect::kMagickaRate |
			           AlchemicEffect::kMagickaRateMult |
			           AlchemicEffect::kMagicka;
		}
		if (tcombatdata & static_cast<uint32_t>(Utility::CurrentCombatStyle::Mage)) {
			effects |= AlchemicEffect::kFortifyMagicka |
			           AlchemicEffect::kMagickaRate |
			           AlchemicEffect::kMagickaRateMult |
			           AlchemicEffect::kMagicka;
		}
		// magic related stuff
		if (tcombatdata & static_cast<uint32_t>(Utility::CurrentCombatStyle::MagicAlteration)) {
			effects |= AlchemicEffect::kAlteration;
		}
		if (tcombatdata & static_cast<uint32_t>(Utility::CurrentCombatStyle::MagicConjuration)) {
			effects |= AlchemicEffect::kConjuration;
		}
		if (tcombatdata & static_cast<uint32_t>(Utility::CurrentCombatStyle::MagicDestruction)) {
			effects |= AlchemicEffect::kDestruction;
		}
		if (tcombatdata & static_cast<uint32_t>(Utility::CurrentCombatStyle::MagicIllusion)) {
			effects |= AlchemicEffect::kIllusion;
		}
		if (tcombatdata & static_cast<uint32_t>(Utility::CurrentCombatStyle::MagicRestoration)) {
			effects |= AlchemicEffect::kRestoration;
		}
		// resistance values based on our expected damage type
		if (combatdata != 0) {
			if (combatdata & static_cast<uint32_t>(Utility::CurrentCombatStyle::MagicDamageFire)) {
				effects |= AlchemicEffect::kResistFire;
			}
			if (combatdata & static_cast<uint32_t>(Utility::CurrentCombatStyle::MagicDamageFrost)) {
				effects |= AlchemicEffect::kResistFrost;
			}
			if (combatdata & static_cast<uint32_t>(Utility::CurrentCombatStyle::MagicDamageShock)) {
				effects |= AlchemicEffect::kResistShock;
			}
		}
		// light and heavy armor
		uint32_t armordata = Utility::GetArmorData(target);
		if (armordata & static_cast<uint32_t>(Utility::CurrentArmor::LightArmor))
			effects |= AlchemicEffect::kLightArmor;
		if (armordata & static_cast<uint32_t>(Utility::CurrentArmor::HeavyArmor))
			effects |= AlchemicEffect::kHeavyArmor;

		return effects;
	}

	/// <summary>
	/// Calculates all fortify effects that an actor is equitable for, based on their and their targets combat data
	/// </summary>
	/// <param name="acinfo">actoringo object</param>
	/// <param name="combatdata">combatdata of [acinfo]</param>
	/// <param name="tcombatdata">combatdata of target</param>
	/// <returns></returns>
	AlchemicEffect Main::CalcFortifyEffects(std::shared_ptr<ActorInfo> acinfo, uint32_t combatdata, uint32_t tcombatdata)
	{
		LOG_4("{}[Events] [CalcFortifyEffects]");
		AlchemicEffect effects = 0;
		effects |= AlchemicEffect::kDamageResist |
		           AlchemicEffect::kResistMagic |
		           AlchemicEffect::kPoisonResist |
		           AlchemicEffect::kResistDisease |
		           AlchemicEffect::kReflectDamage;

		if (combatdata & static_cast<uint32_t>(Utility::CurrentCombatStyle::Spellsword)) {
			effects |= AlchemicEffect::kOneHanded |
			           AlchemicEffect::kMeleeDamage |
			           AlchemicEffect::kSpeedMult |
			           AlchemicEffect::kWeaponSpeedMult |
			           AlchemicEffect::kAttackDamageMult |
			           AlchemicEffect::kCriticalChance |
			           AlchemicEffect::kFortifyHealth |
			           AlchemicEffect::kFortifyMagicka |
			           AlchemicEffect::kFortifyStamina;
		}
		if (combatdata & static_cast<uint32_t>(Utility::CurrentCombatStyle::Staffsword)) {
			effects |= AlchemicEffect::kOneHanded |
			           AlchemicEffect::kMeleeDamage |
			           AlchemicEffect::kSpeedMult |
			           AlchemicEffect::kWeaponSpeedMult |
			           AlchemicEffect::kAttackDamageMult |
			           AlchemicEffect::kCriticalChance |
			           AlchemicEffect::kFortifyHealth |
			           AlchemicEffect::kFortifyMagicka |
			           AlchemicEffect::kFortifyStamina;
		}
		if (combatdata & static_cast<uint32_t>(Utility::CurrentCombatStyle::OneHandedShield)) {
			effects |= AlchemicEffect::kOneHanded |
			           AlchemicEffect::kBlock |
			           AlchemicEffect::kSpeedMult |
			           AlchemicEffect::kWeaponSpeedMult |
			           AlchemicEffect::kAttackDamageMult |
			           AlchemicEffect::kCriticalChance |
			           AlchemicEffect::kFortifyHealth |
			           AlchemicEffect::kFortifyStamina;
		}
		if (combatdata & static_cast<uint32_t>(Utility::CurrentCombatStyle::OneHanded)) {
			effects |= AlchemicEffect::kOneHanded |
			           AlchemicEffect::kBlock |
			           AlchemicEffect::kMeleeDamage |
			           AlchemicEffect::kSpeedMult |
			           AlchemicEffect::kWeaponSpeedMult |
			           AlchemicEffect::kAttackDamageMult |
			           AlchemicEffect::kCriticalChance |
			           AlchemicEffect::kFortifyHealth |
			           AlchemicEffect::kFortifyStamina;
		}
		if (combatdata & static_cast<uint32_t>(Utility::CurrentCombatStyle::TwoHanded)) {
			effects |= AlchemicEffect::kTwoHanded |
			           AlchemicEffect::kBlock |
			           AlchemicEffect::kMeleeDamage |
			           AlchemicEffect::kSpeedMult |
			           AlchemicEffect::kWeaponSpeedMult |
			           AlchemicEffect::kAttackDamageMult |
			           AlchemicEffect::kCriticalChance |
			           AlchemicEffect::kFortifyHealth |
			           AlchemicEffect::kFortifyStamina;
		}
		if (combatdata & static_cast<uint32_t>(Utility::CurrentCombatStyle::Ranged)) {
			effects |= AlchemicEffect::kArchery |
			           AlchemicEffect::kWeaponSpeedMult |
			           AlchemicEffect::kAttackDamageMult |
			           AlchemicEffect::kBowSpeed |
			           AlchemicEffect::kCriticalChance |
			           AlchemicEffect::kFortifyStamina;
		}
		if (combatdata & static_cast<uint32_t>(Utility::CurrentCombatStyle::DualWield)) {
			effects |= AlchemicEffect::kOneHanded |
			           AlchemicEffect::kSpeedMult |
			           AlchemicEffect::kWeaponSpeedMult |
			           AlchemicEffect::kAttackDamageMult |
			           AlchemicEffect::kCriticalChance |
			           AlchemicEffect::kFortifyHealth |
			           AlchemicEffect::kFortifyStamina;
		}
		if (combatdata & static_cast<uint32_t>(Utility::CurrentCombatStyle::HandToHand)) {
			effects |= AlchemicEffect::kUnarmedDamage |
			           AlchemicEffect::kSpeedMult |
			           AlchemicEffect::kAttackDamageMult |
			           AlchemicEffect::kFortifyHealth |
			           AlchemicEffect::kFortifyStamina;
		}
		if (combatdata & static_cast<uint32_t>(Utility::CurrentCombatStyle::DualStaff)) {
			effects |= AlchemicEffect::kFortifyMagicka;
		}
		if (combatdata & static_cast<uint32_t>(Utility::CurrentCombatStyle::Mage)) {
			effects |= AlchemicEffect::kFortifyMagicka;
		}
		// magic related stuff
		if (combatdata & static_cast<uint32_t>(Utility::CurrentCombatStyle::MagicAlteration)) {
			effects |= AlchemicEffect::kAlteration;
		}
		if (combatdata & static_cast<uint32_t>(Utility::CurrentCombatStyle::MagicConjuration)) {
			effects |= AlchemicEffect::kConjuration;
		}
		if (combatdata & static_cast<uint32_t>(Utility::CurrentCombatStyle::MagicDestruction)) {
			effects |= AlchemicEffect::kDestruction;
		}
		if (combatdata & static_cast<uint32_t>(Utility::CurrentCombatStyle::MagicIllusion)) {
			effects |= AlchemicEffect::kIllusion;
		}
		if (combatdata & static_cast<uint32_t>(Utility::CurrentCombatStyle::MagicRestoration)) {
			effects |= AlchemicEffect::kRestoration;
		}
		// resistance values based on their enemies expected damage type
		if (tcombatdata != 0) {
			if (tcombatdata & static_cast<uint32_t>(Utility::CurrentCombatStyle::MagicDamageFire)) {
				effects |= AlchemicEffect::kResistFire;
			}
			if (tcombatdata & static_cast<uint32_t>(Utility::CurrentCombatStyle::MagicDamageFrost)) {
				effects |= AlchemicEffect::kResistFrost;
			}
			if (tcombatdata & static_cast<uint32_t>(Utility::CurrentCombatStyle::MagicDamageShock)) {
				effects |= AlchemicEffect::kResistShock;
			}
		}
		// light and heavy armor
		uint32_t armordata = Utility::GetArmorData(acinfo->GetActor());
		if (armordata & static_cast<uint32_t>(Utility::CurrentArmor::LightArmor))
			effects |= AlchemicEffect::kLightArmor;
		if (armordata & static_cast<uint32_t>(Utility::CurrentArmor::HeavyArmor))
			effects |= AlchemicEffect::kHeavyArmor;

		// shield potions
		effects |= AlchemicEffect::kShield;
		return effects;
	}

	
	/// <summary>
	/// Calculates all regeneration effects that an actor is equitable for, based on their combat data
	/// </summary>
	/// <param name="combatdata">combatdata of the actor</param>
	/// <returns>valid regeneration effects</returns>
	AlchemicEffect Main::CalcRegenEffects(uint32_t combatdata)
	{
		LOG_4("{}[Events] [CalcRegenEffects]");
		AlchemicEffect effects = 0;
		effects |= AlchemicEffect::kHealRate |
		           AlchemicEffect::kHealRateMult;

		if (combatdata &
			(static_cast<uint32_t>(Utility::CurrentCombatStyle::Spellsword) |
				static_cast<uint32_t>(Utility::CurrentCombatStyle::Staffsword))) {
			effects |= AlchemicEffect::kMagickaRate |
			           AlchemicEffect::kMagickaRateMult |
			           AlchemicEffect::kStaminaRate |
			           AlchemicEffect::kStaminaRateMult;
		}
		if (combatdata &
			(static_cast<uint32_t>(Utility::CurrentCombatStyle::OneHandedShield) |
				static_cast<uint32_t>(Utility::CurrentCombatStyle::TwoHanded) |
				static_cast<uint32_t>(Utility::CurrentCombatStyle::Ranged) |
				static_cast<uint32_t>(Utility::CurrentCombatStyle::DualWield) |
				static_cast<uint32_t>(Utility::CurrentCombatStyle::HandToHand))) {
			effects |= AlchemicEffect::kStaminaRate |
			           AlchemicEffect::kStaminaRateMult;
		}
		if (combatdata &
			(static_cast<uint32_t>(Utility::CurrentCombatStyle::Mage) |
				static_cast<uint32_t>(Utility::CurrentCombatStyle::DualStaff))) {
			effects |= AlchemicEffect::kMagickaRate |
			           AlchemicEffect::kMagickaRateMult;
		}
		return effects;
	}

	/// <summary>
	/// Calculates all regeneration effects that an actor is equitable for, based on their combat data
	/// </summary>
	/// <param name="acinfo"></param>
	/// <param name="combatdata"></param>
	/// <returns></returns>
	AlchemicEffect Main::CalcRegenEffects(std::shared_ptr<ActorInfo> /*acinfo*/, uint32_t combatdata)
	{
		return CalcRegenEffects(combatdata);
	}

	/// <summary>
	/// Processes the item distribution for an actor
	/// </summary>
	/// <param name="acinfo"></param>
	void Main::ProcessDistribution(std::shared_ptr<ActorInfo> acinfo)
	{
		// check wether this charackter maybe a follower
		if (acinfo->GetLastDistrTime() == 0.0f || RE::Calendar::GetSingleton()->GetDaysPassed() - acinfo->GetLastDistrTime() > 1) {
			if (!Distribution::ExcludedNPC(acinfo) && acinfo->IsDead() == false) {
				// begin with compatibility mode removing items before distributing new ones
				if (Settings::Debug::_CompatibilityRemoveItemsBeforeDist) {
					auto items = ACM::GetAllPotions(acinfo);
					auto it = items.begin();
					while (it != items.end()) {
						acinfo->RemoveItem(*it, 1);
						LOG1_1("{}[Events] [ProcessDistribution] Removed item {}", Utility::PrintForm(*it));
						it++;
					}
					items = ACM::GetAllPoisons(acinfo);
					it = items.begin();
					while (it != items.end()) {
						acinfo->RemoveItem(*it, 1);
						LOG1_1("{}[Events] [ProcessDistribution] Removed item {}", Utility::PrintForm(*it));
						it++;
					}
					items = ACM::GetAllFood(acinfo);
					it = items.begin();
					while (it != items.end()) {
						acinfo->RemoveItem(*it, 1);
						LOG1_1("{}[Events] [ProcessDistribution] Removed item {}", Utility::PrintForm(*it));
						it++;
					}
				}

				// if we have characters that should not get items, the function
				// just won't return anything, but we have to check for standard factions like CurrentFollowerFaction
				auto items = Distribution::GetDistrItems(acinfo);
				if (acinfo->IsDead()) {
					return;
				}
				if (items.size() > 0) {
					for (int i = 0; i < items.size(); i++) {
						if (items[i] == nullptr) {
							continue;
						}
						acinfo->AddItem(items[i], 1);
						LOG2_4("{}[Events] [ProcessDistribution] added item {} to actor {}", Utility::PrintForm(items[i]), Utility::PrintForm(acinfo));
					}
					acinfo->SetLastDistrTime(RE::Calendar::GetSingleton()->GetDaysPassed());
				}
			}
		}
	}

	/// <summary>
	/// Registers an NPC for handling
	/// </summary>
	/// <param name="actor"></param>
	void Main::RegisterNPC(RE::Actor* actor)
	{
		EvalProcessing();
		// exit if the actor is unsafe / not valid
		if (Utility::ValidateActor(actor) == false)
			return;
		LOG1_1("{}[Events] [RegisterNPC] Trying to register new actor for potion tracking: {}", Utility::PrintForm(actor));
		std::shared_ptr<ActorInfo> acinfo = data->FindActor(actor);
		LOG1_1("{}[Events] [RegisterNPC] Found: {}", Utility::PrintForm(acinfo));
		// if actor was dead, exit
		if (acinfo->GetDead()) {
			LOG_1("{}[Events] [RegisterNPC] Actor already dead");
			return;
		}
		// reset object to account for changes to underlying objects
		acinfo->Reset(actor);
		if (acinfo->IsValid() == false) {
			LOG_1("{}[Events] [RegisterNPC] Actor reset failed");
			return;
		}
		// find out whether to insert the actor, if yes insert him into the temp insert list
		sem.acquire();
		if (!acset.contains(acinfo)) {
			acset.insert(acinfo);
		} else {
			sem.release();
			LOG_1("{}[Events] [RegisterNPC] Actor already registered");
			return;
		}
		sem.release();

		ProcessDistribution(acinfo);
		EvalProcessing();
		if (actor->IsDead())
			return;

		LOG_1("{}[Events] [RegisterNPC] finished registering NPC");
	}

	/// <summary>
	/// Unregisters an NPC form handling
	/// </summary>
	/// <param name="actor"></param>
	void Main::UnregisterNPC(RE::Actor* actor)
	{
		EvalProcessing();
		// exit if actor is unsafe / not valid
		if (Utility::ValidateActor(actor) == false)
			return;
		LOG1_1("{}[Events] [UnregisterNPC] Unregister NPC from potion tracking: {}", Utility::PrintForm(actor));
		std::shared_ptr<ActorInfo> acinfo = data->FindActor(actor);
		sem.acquire();
		acset.erase(acinfo);
		acinfo->SetDurHealth(0);
		acinfo->SetDurMagicka(0);
		acinfo->SetDurStamina(0);
		acinfo->SetDurFortify(0);
		acinfo->SetDurRegeneration(0);
		sem.release();
		LOG_1("{}[Events] [UnregisterNPC] Unregistered NPC");
	}

	/// <summary>
	/// Unregisters an NPC from handling
	/// </summary>
	/// <param name="acinfo"></param>
	void Main::UnregisterNPC(std::shared_ptr<ActorInfo> acinfo)
	{
		EvalProcessing();
		LOG1_1("{}[Events] [UnregisterNPC] Unregister NPC from potion tracking: {}", acinfo->GetName());
		sem.acquire();
		acset.erase(acinfo);
		acinfo->SetDurHealth(0);
		acinfo->SetDurMagicka(0);
		acinfo->SetDurStamina(0);
		acinfo->SetDurFortify(0);
		acinfo->SetDurRegeneration(0);
		sem.release();
	}

	/// <summary>
	/// Unregisters an NPC from handling
	/// </summary>
	/// <param name="acinfo"></param>
	void Main::UnregisterNPC(RE::FormID formid)
	{
		EvalProcessing();
		LOG1_1("{}[Events] [UnregisterNPC] Unregister NPC from potion tracking: {}", Utility::GetHex(formid));
		sem.acquire();
		auto itr = acset.begin();
		while (itr != acset.end()) {
			if (std::shared_ptr<ActorInfo> acinfo = itr->lock()) {
				if (acinfo->GetFormIDBlank() == formid) {
					acset.erase(itr);
					break;
				}
			} else {
				// weak pointer is expired, so remove it while we are on it
				acset.erase(itr);
			}
			itr++;
		}
		sem.release();
	}

	bool Main::IsDead(RE::Actor* actor)
	{
		return actor == nullptr || deads.contains(actor->GetHandle()) || actor->IsDead();
	}

	bool Main::IsDeadEventFired(RE::Actor* actor)
	{
		return actor == nullptr || deads.contains(actor->GetHandle());
	}

	void Main::SetDead(RE::Actor* actor)
	{
		if (actor != nullptr)
			deads.insert(actor->GetHandle());
	}

	int Main::CalcPotionDuration(int dur)
	{
		return dur == 0 ?
		           1000 :
		       dur * 1000 > Settings::_MaxDuration ?
		           Settings::_MaxDuration :
		           dur * 1000;
	}

	int Main::CalcFortifyDuration(int dur)
	{
		return dur == 0 ?
		           1000 :
		       dur * 1000 > Settings::_MaxFortifyDuration ?
		           Settings::_MaxFortifyDuration :
		           dur * 1000;
	}

	int Main::CalcRegenerationDuration(int dur)
	{
		return dur == 0 ?
		           1000 :
		       dur * 1000 > Settings::_MaxFortifyDuration ?
		           Settings::_MaxFortifyDuration :
		           dur * 1000;
	}

	float Main::CalcFoodDuration(int dur)
	{
		static RE::Calendar* calendar = RE::Calendar::GetSingleton();
		// if duration is zero (vanilla food) set cooldown to at least 4 minutes
		return dur <= 0 ?
		           calendar->GetDaysPassed() + 240 * calendar->GetTimescale() / 60 / 60 / 24 :
		           calendar->GetDaysPassed() + dur * calendar->GetTimescale() / 60 / 60 / 24;
	}


	/// <summary>
	/// Removes all distributable alchemy items from all actors in the game on loading a game
	/// </summary>
	void Main::RemoveItemsOnStartup()
	{
		EvalProcessing();
		std::this_thread::sleep_for(5s);

		auto begin = std::chrono::steady_clock::now();

		auto datahandler = RE::TESDataHandler::GetSingleton();
		auto actors = datahandler->GetFormArray<RE::Actor>();
		RE::Actor* actor = nullptr;

		RE::TESObjectCELL* cell = nullptr;
		std::vector<RE::TESObjectCELL*> cs;
		const auto& [hashtable, lock] = RE::TESForm::GetAllForms();
		{
			const RE::BSReadLockGuard locker{ lock };
			if (hashtable) {
				for (auto& [id, form] : *hashtable) {
					EvalProcessing();
					if (form) {
						actor = form->As<RE::Actor>();
						if (Utility::ValidateActor(actor)) {
							std::shared_ptr<ActorInfo> acinfo = data->FindActor(actor);
							auto items = ACM::GetAllPotions(acinfo);
							auto it = items.begin();
							while (it != items.end()) {
								if (Settings::Debug::_CompatibilityRemoveItemsStartup_OnlyExcluded && !(Distribution::excludedItems()->contains((*it)->GetFormID()))) {
									it++;
									continue;
								}
								actor->RemoveItem(*it, 1, RE::ITEM_REMOVE_REASON::kRemove, nullptr, nullptr);
								LOG1_1("{}[Events] [RemoveItemsOnStartup] Removed item {}", Utility::PrintForm(*it));
								it++;
							}
							items = ACM::GetAllPoisons(acinfo);
							it = items.begin();
							while (it != items.end()) {
								if (Settings::Debug::_CompatibilityRemoveItemsStartup_OnlyExcluded && !(Distribution::excludedItems()->contains((*it)->GetFormID()))) {
									it++;
									continue;
								}
								actor->RemoveItem(*it, 1, RE::ITEM_REMOVE_REASON::kRemove, nullptr, nullptr);
								LOG1_1("{}[Events] [RemoveItemsOnStartup] Removed item {}", Utility::PrintForm(*it));
								it++;
							}
							items = ACM::GetAllFood(acinfo);
							it = items.begin();
							while (it != items.end()) {
								if (Settings::Debug::_CompatibilityRemoveItemsStartup_OnlyExcluded && !(Distribution::excludedItems()->contains((*it)->GetFormID()))) {
									it++;
									continue;
								}
								actor->RemoveItem(*it, 1, RE::ITEM_REMOVE_REASON::kRemove, nullptr, nullptr);
								LOG1_1("{}[Events] [RemoveItemsOnStartup] Removed item {}", Utility::PrintForm(*it));
								it++;
							}
						}
					}
				}
			}
		}
		LogConsole("Finished Thread RemoveItemsOnStartup");

		PROF1_1("{}[Events] [RemoveItemsOnStartup] execution time: {} µs", std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin).count()));
	}

}
