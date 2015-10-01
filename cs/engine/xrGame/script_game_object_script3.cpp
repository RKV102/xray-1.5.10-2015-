////////////////////////////////////////////////////////////////////////////
//	Module 		: script_game_object_script3.cpp
//	Created 	: 25.09.2003
//  Modified 	: 29.06.2004
//	Author		: Dmitriy Iassenev
//	Description : XRay Script game object script export
////////////////////////////////////////////////////////////////////////////

#include "pch_script.h"
#include "script_game_object.h"
#include "alife_space.h"
#include "script_entity_space.h"
#include "movement_manager_space.h"
#include "pda_space.h"
#include "memory_space.h"
#include "cover_point.h"
#include "script_hit.h"
#include "script_binder_object.h"
#include "script_ini_file.h"
#include "script_sound_info.h"
#include "script_monster_hit_info.h"
#include "script_entity_action.h"
#include "action_planner.h"
#include "PhysicsShell.h"
#include "helicopter.h"
#include "HangingLamp.h"
#include "holder_custom.h"
#include "script_zone.h"
#include "relation_registry.h"
#include "GameTask.h"
#include "car.h"
#include "ZoneCampfire.h"
#include "physicobject.h"
#include "artefact.h"

using namespace luabind;

class_<CScriptGameObject> &script_register_game_object2(class_<CScriptGameObject> &instance)
{
	instance
		.def("add_sound",					(u32 (CScriptGameObject::*)(LPCSTR,u32,ESoundTypes,u32,u32,u32))(&CScriptGameObject::add_sound))
		.def("add_sound",					(u32 (CScriptGameObject::*)(LPCSTR,u32,ESoundTypes,u32,u32,u32,LPCSTR))(&CScriptGameObject::add_sound))
		.def("remove_sound",				&CScriptGameObject::remove_sound)
		.def("set_sound_mask",				&CScriptGameObject::set_sound_mask)
		.def("play_sound",					(void (CScriptGameObject::*)(u32))(&CScriptGameObject::play_sound))
		.def("play_sound",					(void (CScriptGameObject::*)(u32,u32))(&CScriptGameObject::play_sound))
		.def("play_sound",					(void (CScriptGameObject::*)(u32,u32,u32))(&CScriptGameObject::play_sound))
		.def("play_sound",					(void (CScriptGameObject::*)(u32,u32,u32,u32))(&CScriptGameObject::play_sound))
		.def("play_sound",					(void (CScriptGameObject::*)(u32,u32,u32,u32,u32))(&CScriptGameObject::play_sound))
		.def("play_sound",					(void (CScriptGameObject::*)(u32,u32,u32,u32,u32,u32))(&CScriptGameObject::play_sound))
		.def("binded_object",				&CScriptGameObject::binded_object)
		.def("set_previous_point",			&CScriptGameObject::set_previous_point)
		.def("set_start_point",				&CScriptGameObject::set_start_point)
		.def("get_current_point_index",		&CScriptGameObject::get_current_patrol_point_index)
		.def("path_completed",				&CScriptGameObject::path_completed)
		.def("patrol_path_make_inactual",	&CScriptGameObject::patrol_path_make_inactual)
		.def("enable_memory_object",		&CScriptGameObject::enable_memory_object)
		.def("active_sound_count",			(int (CScriptGameObject::*)())(&CScriptGameObject::active_sound_count))
		.def("active_sound_count",			(int (CScriptGameObject::*)(bool))(&CScriptGameObject::active_sound_count))
		.def("best_cover",					&CScriptGameObject::best_cover)
		.def("safe_cover",					&CScriptGameObject::safe_cover)
		.def("spawn_ini",					&CScriptGameObject::spawn_ini)
		.def("memory_visible_objects",		&CScriptGameObject::memory_visible_objects, return_stl_iterator)
		.def("memory_sound_objects",		&CScriptGameObject::memory_sound_objects, return_stl_iterator)
		.def("memory_hit_objects",			&CScriptGameObject::memory_hit_objects, return_stl_iterator)
		.def("not_yet_visible_objects",		&CScriptGameObject::not_yet_visible_objects, return_stl_iterator)
		.def("visibility_threshold",		&CScriptGameObject::visibility_threshold)
		.def("enable_vision",				&CScriptGameObject::enable_vision)
		.def("vision_enabled",				&CScriptGameObject::vision_enabled)
		.def("set_sound_threshold",			&CScriptGameObject::set_sound_threshold)
		.def("restore_sound_threshold",		&CScriptGameObject::restore_sound_threshold)

		// sight manager
		.def("set_sight",					(void (CScriptGameObject::*)(SightManager::ESightType sight_type, const Fvector *vector3d, u32 dwLookOverDelay))(&CScriptGameObject::set_sight))
		.def("set_sight",					(void (CScriptGameObject::*)(SightManager::ESightType sight_type, bool torso_look, bool path))(&CScriptGameObject::set_sight))
		.def("set_sight",					(void (CScriptGameObject::*)(SightManager::ESightType sight_type, const Fvector &vector3d, bool torso_look))(&CScriptGameObject::set_sight))
		.def("set_sight",					(void (CScriptGameObject::*)(SightManager::ESightType sight_type, const Fvector *vector3d))(&CScriptGameObject::set_sight))
		.def("set_sight",					(void (CScriptGameObject::*)(CScriptGameObject *object_to_look))(&CScriptGameObject::set_sight))
		.def("set_sight",					(void (CScriptGameObject::*)(CScriptGameObject *object_to_look, bool torso_look))(&CScriptGameObject::set_sight))
		.def("set_sight",					(void (CScriptGameObject::*)(CScriptGameObject *object_to_look, bool torso_look, bool fire_object))(&CScriptGameObject::set_sight))
		.def("set_sight",					(void (CScriptGameObject::*)(CScriptGameObject *object_to_look, bool torso_look, bool fire_object, bool no_pitch))(&CScriptGameObject::set_sight))
//		.def("set_sight",					(void (CScriptGameObject::*)(const MemorySpace::CMemoryInfo *memory_object, bool	torso_look))(&CScriptGameObject::set_sight))

		// object handler
		.def("set_item",					(void (CScriptGameObject::*)(MonsterSpace::EObjectAction ))(&CScriptGameObject::set_item))
		.def("set_item",					(void (CScriptGameObject::*)(MonsterSpace::EObjectAction, CScriptGameObject *))(&CScriptGameObject::set_item))
		.def("set_item",					(void (CScriptGameObject::*)(MonsterSpace::EObjectAction, CScriptGameObject *, u32))(&CScriptGameObject::set_item))
		.def("set_item",					(void (CScriptGameObject::*)(MonsterSpace::EObjectAction, CScriptGameObject *, u32, u32))(&CScriptGameObject::set_item))

		.def("bone_position",				&CScriptGameObject::bone_position)

		.def("is_body_turning",				&CScriptGameObject::is_body_turning)

		//////////////////////////////////////////////////////////////////////////
		// Space restrictions
		//////////////////////////////////////////////////////////////////////////
		.def("add_restrictions",			&CScriptGameObject::add_restrictions)
		.def("remove_restrictions",			&CScriptGameObject::remove_restrictions)
		.def("remove_all_restrictions",		&CScriptGameObject::remove_all_restrictions)
		.def("in_restrictions",				&CScriptGameObject::in_restrictions)
		.def("out_restrictions",			&CScriptGameObject::out_restrictions)
		.def("base_in_restrictions",		&CScriptGameObject::base_in_restrictions)
		.def("base_out_restrictions",		&CScriptGameObject::base_out_restrictions)
		.def("accessible",					&CScriptGameObject::accessible_position)
		.def("accessible",					&CScriptGameObject::accessible_vertex_id)
		.def("accessible_nearest",			&CScriptGameObject::accessible_nearest, out_value(_3))

		//////////////////////////////////////////////////////////////////////////
		.def("enable_attachable_item",		&CScriptGameObject::enable_attachable_item)
		.def("attachable_item_enabled",		&CScriptGameObject::attachable_item_enabled)
		.def("weapon_strapped",				&CScriptGameObject::weapon_strapped)
		.def("weapon_unstrapped",			&CScriptGameObject::weapon_unstrapped)

		//////////////////////////////////////////////////////////////////////////
		//inventory owner
		//////////////////////////////////////////////////////////////////////////

		.enum_("EPdaMsg")
		[
			value("dialog_pda_msg",			int(ePdaMsgDialog)),
			value("info_pda_msg",			int(ePdaMsgInfo)),
			value("no_pda_msg",				int(ePdaMsgMax))
		]

		.def("give_info_portion",			&CScriptGameObject::GiveInfoPortion)
		.def("disable_info_portion",		&CScriptGameObject::DisableInfoPortion)
		.def("give_game_news",				(void (CScriptGameObject::*)(LPCSTR,LPCSTR,LPCSTR,int,int))(&CScriptGameObject::GiveGameNews))
		.def("give_game_news",				(void (CScriptGameObject::*)(LPCSTR,LPCSTR,LPCSTR,int,int,int))(&CScriptGameObject::GiveGameNews))

		.def("give_talk_message",			(void (CScriptGameObject::*)(LPCSTR,LPCSTR,LPCSTR))(&CScriptGameObject::AddIconedTalkMessage_old))//old version, must remove!
		.def("give_talk_message2",			(void (CScriptGameObject::*)(LPCSTR,LPCSTR,LPCSTR,LPCSTR))(&CScriptGameObject::AddIconedTalkMessage))

		.def("has_info",					&CScriptGameObject::HasInfo)
		.def("dont_has_info",				&CScriptGameObject::DontHasInfo)
		.def("get_info_time",				&CScriptGameObject::GetInfoTime)

		.def("get_task_state",				&CScriptGameObject::GetGameTaskState)
		.def("set_task_state",				&CScriptGameObject::SetGameTaskState)
		.def("give_task",					&CScriptGameObject::GiveTaskToActor,		adopt(_2))
		.def("get_task",					&CScriptGameObject::GetTask)

		.def("is_talking",					&CScriptGameObject::IsTalking)
		.def("stop_talk",					&CScriptGameObject::StopTalk)
		.def("enable_talk",					&CScriptGameObject::EnableTalk)
		.def("disable_talk",				&CScriptGameObject::DisableTalk)
		.def("is_talk_enabled",				&CScriptGameObject::IsTalkEnabled)

		.def("enable_trade",				&CScriptGameObject::EnableTrade)
		.def("disable_trade",				&CScriptGameObject::DisableTrade)
		.def("is_trade_enabled",			&CScriptGameObject::IsTradeEnabled)
		.def("enable_inv_upgrade",			&CScriptGameObject::EnableInvUpgrade)
		.def("disable_inv_upgrade",			&CScriptGameObject::DisableInvUpgrade)
		.def("is_inv_upgrade_enabled",		&CScriptGameObject::IsInvUpgradeEnabled)

		.def("inventory_for_each",			&CScriptGameObject::ForEachInventoryItems)
		.def("drop_item",					&CScriptGameObject::DropItem)
		.def("drop_item_and_teleport",		&CScriptGameObject::DropItemAndTeleport)
		.def("transfer_item",				&CScriptGameObject::TransferItem)
		.def("transfer_money",				&CScriptGameObject::TransferMoney)
		.def("give_money",					&CScriptGameObject::GiveMoney)
		.def("money",						&CScriptGameObject::Money)
		.def("make_item_active",			&CScriptGameObject::MakeItemActive)

		.def("switch_to_trade",				&CScriptGameObject::SwitchToTrade)
		.def("switch_to_upgrade",			&CScriptGameObject::SwitchToUpgrade)
		.def("switch_to_talk",				&CScriptGameObject::SwitchToTalk)
		.def("run_talk_dialog",				&CScriptGameObject::RunTalkDialog)
		.def("allow_break_talk_dialog",		&CScriptGameObject::AllowBreakTalkDialog)
		
		.def("hide_weapon",					&CScriptGameObject::HideWeapon)
		.def("restore_weapon",				&CScriptGameObject::RestoreWeapon)
		
		.def("weapon_is_grenadelauncher",	&CScriptGameObject::Weapon_IsGrenadeLauncherAttached)
		.def("weapon_is_scope",				&CScriptGameObject::Weapon_IsScopeAttached)
		.def("weapon_is_silencer",			&CScriptGameObject::Weapon_IsSilencerAttached)

		.def("weapon_grenadelauncher_status",	&CScriptGameObject::Weapon_GrenadeLauncher_Status)
		.def("weapon_scope_status",				&CScriptGameObject::Weapon_Scope_Status)
		.def("weapon_silencer_status",			&CScriptGameObject::Weapon_Silencer_Status)

		.def("allow_sprint",				&CScriptGameObject::AllowSprint)

		.def("set_start_dialog",			&CScriptGameObject::SetStartDialog)
		.def("get_start_dialog",			&CScriptGameObject::GetStartDialog)
		.def("restore_default_start_dialog",&CScriptGameObject::RestoreDefaultStartDialog)

		.def("goodwill",					&CScriptGameObject::GetGoodwill)
		.def("set_goodwill",				&CScriptGameObject::SetGoodwill)
		.def("change_goodwill",				&CScriptGameObject::ChangeGoodwill)

		.def("general_goodwill",			&CScriptGameObject::GetAttitude)
		.def("set_relation",				&CScriptGameObject::SetRelation)
		
		.def("community_goodwill",			&CScriptGameObject::GetCommunityGoodwill_obj)
		.def("set_community_goodwill",		&CScriptGameObject::SetCommunityGoodwill_obj)

		.def("sympathy",					&CScriptGameObject::GetSympathy)
		.def("set_sympathy",				&CScriptGameObject::SetSympathy)

		//////////////////////////////////////////////////////////////////////////
		.def("profile_name",				&CScriptGameObject::ProfileName)
		.def("character_name",				&CScriptGameObject::CharacterName)
		.def("character_icon",				&CScriptGameObject::CharacterIcon)
		.def("character_rank",				&CScriptGameObject::CharacterRank)
		.def("set_character_rank",			&CScriptGameObject::SetCharacterRank)
		.def("character_reputation",		&CScriptGameObject::CharacterReputation)
		.def("change_character_reputation",	&CScriptGameObject::ChangeCharacterReputation)
		.def("character_community",			&CScriptGameObject::CharacterCommunity)
		.def("set_character_community",		&CScriptGameObject::SetCharacterCommunity)

		.def("get_actor_relation_flags",	&CScriptGameObject::get_actor_relation_flags)
		.def("set_actor_relation_flags",	&CScriptGameObject::set_actor_relation_flags)
		.def("sound_voice_prefix",	&CScriptGameObject::sound_voice_prefix)

		.enum_("ACTOR_RELATIONS")
		[
			value("relation_attack",						int(RELATION_REGISTRY::ATTACK)),
			value("relation_fight_help_monster",			int(RELATION_REGISTRY::FIGHT_HELP_MONSTER)),
			value("relation_fight_help_human",				int(RELATION_REGISTRY::FIGHT_HELP_HUMAN)),
			value("relation_kill",							int(RELATION_REGISTRY::KILL))
		]

		.enum_("CLSIDS")
		[
			value("no_pda_msg",				int(ePdaMsgMax))
		]

		//CustomZone
		.def("enable_anomaly",              &CScriptGameObject::EnableAnomaly)
		.def("disable_anomaly",             &CScriptGameObject::DisableAnomaly)
		.def("get_anomaly_power",			&CScriptGameObject::GetAnomalyPower)
		.def("set_anomaly_power",			&CScriptGameObject::SetAnomalyPower)

		//HELICOPTER
		.def("get_helicopter",              &CScriptGameObject::get_helicopter)
		.def("get_car",						&CScriptGameObject::get_car)
		.def("get_hanging_lamp",            &CScriptGameObject::get_hanging_lamp)
		.def("get_physics_shell",			&CScriptGameObject::get_physics_shell)
		.def("get_holder_class",			&CScriptGameObject::get_custom_holder)
		.def("get_current_holder",			&CScriptGameObject::get_current_holder)
		
		//car
		.def("attach_vehicle", &CScriptGameObject::attach_actor_Vehicle)
		.def("detach_vehicle", &CScriptGameObject::detach_actor_Vehicle)
		.def("get_actor_car", &CScriptGameObject::GetActorCar)

		//usable object
		.def("set_tip_text",				&CScriptGameObject::SetTipText)
		.def("set_tip_text_default",		&CScriptGameObject::SetTipTextDefault)
		.def("set_nonscript_usable",		&CScriptGameObject::SetNonscriptUsable)

		// Script Zone
		.def("active_zone_contact",			&CScriptGameObject::active_zone_contact)
		.def("inside",						(bool (CScriptGameObject::*)(const Fvector &, float) const)(&CScriptGameObject::inside))
		.def("inside",						(bool (CScriptGameObject::*)(const Fvector &) const)(&CScriptGameObject::inside))
		.def("set_fastcall",				&CScriptGameObject::set_fastcall)
		.def("set_const_force",				&CScriptGameObject::set_const_force)
		.def("info_add",					&CScriptGameObject::info_add)
		.def("info_clear",					&CScriptGameObject::info_clear)
		.def("is_inv_box_empty",			&CScriptGameObject::IsInvBoxEmpty)
		// monster jumper
		.def("jump",						&CScriptGameObject::jump)

		.def("make_object_visible_somewhen",&CScriptGameObject::make_object_visible_somewhen)

		.def("buy_condition",				(void (CScriptGameObject::*)(CScriptIniFile*,LPCSTR))(&CScriptGameObject::buy_condition))
		.def("buy_condition",				(void (CScriptGameObject::*)(float,float))(&CScriptGameObject::buy_condition))
		.def("show_condition",				&CScriptGameObject::show_condition)
		.def("sell_condition",				(void (CScriptGameObject::*)(CScriptIniFile*,LPCSTR))(&CScriptGameObject::sell_condition))
		.def("sell_condition",				(void (CScriptGameObject::*)(float,float))(&CScriptGameObject::sell_condition))
		.def("buy_supplies",				&CScriptGameObject::buy_supplies)

		.def("sound_prefix",				(LPCSTR (CScriptGameObject::*)() const)(&CScriptGameObject::sound_prefix))
		.def("sound_prefix",				(void (CScriptGameObject::*)(LPCSTR))(&CScriptGameObject::sound_prefix))

		.def("location_on_path",			&CScriptGameObject::location_on_path)

		.def("wounded",						(bool (CScriptGameObject::*)() const)(&CScriptGameObject::wounded))
		.def("wounded",						(void (CScriptGameObject::*)(bool))(&CScriptGameObject::wounded))

		.def("iterate_inventory",			&CScriptGameObject::IterateInventory)
		.def("iterate_inventory_box",		&CScriptGameObject::IterateInventoryBox)
		.def("mark_item_dropped",			&CScriptGameObject::MarkItemDropped)
		.def("marked_dropped",				&CScriptGameObject::MarkedDropped)
		.def("unload_magazine",				&CScriptGameObject::UnloadMagazine)

		.def("sight_params",				&CScriptGameObject::sight_params)

		.def("movement_enabled",			&CScriptGameObject::enable_movement)
		.def("movement_enabled",			&CScriptGameObject::movement_enabled)

		.def("critically_wounded",			&CScriptGameObject::critically_wounded)
		.def("get_campfire",				&CScriptGameObject::get_campfire)
		.def("get_artefact",				&CScriptGameObject::get_artefact)
		.def("get_physics_object",			&CScriptGameObject::get_physics_object)
		.def("aim_time",					(void (CScriptGameObject::*) (CScriptGameObject*, u32))&CScriptGameObject::aim_time)
		.def("aim_time",					(u32 (CScriptGameObject::*) (CScriptGameObject*))&CScriptGameObject::aim_time)

		.def("special_danger_move",			(void (CScriptGameObject::*) (bool))&CScriptGameObject::special_danger_move)
		.def("special_danger_move",			(bool (CScriptGameObject::*) ())&CScriptGameObject::special_danger_move)

		.def("sniper_update_rate",			(void (CScriptGameObject::*) (bool))&CScriptGameObject::sniper_update_rate)
		.def("sniper_update_rate",			(bool (CScriptGameObject::*) () const)&CScriptGameObject::sniper_update_rate)

		.def("sniper_fire_mode",			(void (CScriptGameObject::*) (bool))&CScriptGameObject::sniper_fire_mode)
		.def("sniper_fire_mode",			(bool (CScriptGameObject::*) () const)&CScriptGameObject::sniper_fire_mode)

		.def("aim_bone_id",					(void (CScriptGameObject::*) (LPCSTR))&CScriptGameObject::aim_bone_id)
		.def("aim_bone_id",					(LPCSTR (CScriptGameObject::*) () const)&CScriptGameObject::aim_bone_id)

		.def("actor_look_at_point",			&CScriptGameObject::ActorLookAtPoint)
		.def("enable_level_changer",		&CScriptGameObject::enable_level_changer)
		.def("is_level_changer_enabled",	&CScriptGameObject::is_level_changer_enabled)

		.def("set_level_changer_invitation",&CScriptGameObject::set_level_changer_invitation)
		.def("start_particles",				&CScriptGameObject::start_particles)
		.def("stop_particles",				&CScriptGameObject::stop_particles)

		// NEW functions /////////////////////////////////////////////

		.def("is_actor_outdoors", &CScriptGameObject::IsActorOutdoors)
		.def("get_total_weight", &CScriptGameObject::GetTotalWeight)
		.def("weight", &CScriptGameObject::Weight)

		.def("switch_projector", &CScriptGameObject::SwitchProjector)
		.def("projector_is_on", &CScriptGameObject::ProjectorIsOn)

		// functions for CInventoryItem class
		.def("set_inventory_item_flags", &CScriptGameObject::SetIIFlags)
		.def("get_inventory_item_flags", &CScriptGameObject::GetIIFlags)
		.def("get_ammo_box_curr", &CScriptGameObject::GetAmmoBoxCurr)
		.def("get_ammo_box_size", &CScriptGameObject::GetAmmoBoxSize)
		.def("set_ammo_box_curr", &CScriptGameObject::SetAmmoBoxCurr)

		// functions for CInventoryOwner class
		.def("iterate_belt", &CScriptGameObject::IterateBelt)
		.def("item_on_belt", &CScriptGameObject::ItemOnBelt)
		.def("item_in_ruck", &CScriptGameObject::ItemInRuck)
		.def("is_on_belt", &CScriptGameObject::IsOnBelt)
		.def("is_in_ruck", &CScriptGameObject::IsInRuck)
		.def("is_in_slot", &CScriptGameObject::IsInSlot)
		.def("move_to_ruck", &CScriptGameObject::MoveToRuck)
		.def("move_to_belt", &CScriptGameObject::MoveToBelt)
		.def("move_to_slot", &CScriptGameObject::MoveToSlot)
		.def("belt_count", &CScriptGameObject::BeltSize)
		.def("ruck_count", &CScriptGameObject::RuckSize)

		.def("get_visual_name", &CScriptGameObject::GetVisualName)
		

		.def("attach_vehicle", &CScriptGameObject::AttachVehicle)
		.def("detach_vehicle", &CScriptGameObject::DetachVehicle)

		.def("heal_wounds", &CScriptGameObject::HealWounds)
		.def("get_visual_ini", &CScriptGameObject::GetVisualIni)

		.def("open_inventory_box", &CScriptGameObject::OpenInventoryBox)
		.def("get_bone_name", &CScriptGameObject::GetBoneName)

		.def("get_silencer_status", &CScriptGameObject::SilencerState)
		.def("get_gl_status", &CScriptGameObject::GrenadeLauncherState)
		.def("get_scope_status", &CScriptGameObject::ScopeState)
	;return	(instance);
}
