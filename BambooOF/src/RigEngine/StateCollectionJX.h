#pragma once
#include "ofMain.h"

/* ----------------------------
// WIP
---------------------------- */

enum RIG_ENGINE_PART
{
	RIG_ENGINE_PART_CHARACTER_CLOTH,
	RIG_ENGINE_PART_CHARACTER_HANDL,
	RIG_ENGINE_PART_CHARACTER_HANDR,
	RIG_ENGINE_PART_CHARACTER_HEAD,
	RIG_ENGINE_PART_CHARACTER_CAPE,
	RIG_ENGINE_PART_WEAPON_ONE,
	RIG_ENGINE_PART_WEAPON_TWO,
	RIG_ENGINE_PART_WEAPON_DUALL,
	RIG_ENGINE_PART_WEAPON_DUALR,
	RIG_ENGINE_PART_HORSE_HEAD,
	RIG_ENGINE_PART_HORSE_BACK,
	RIG_ENGINE_PART_HORSE_TAIL
};

#define RIG_ENGINE_PATH_CHARACTER_CLOTH_M	"character_male_cloth"
#define RIG_ENGINE_PATH_CHARACTER_CLOTH_F	"character_female_cloth"
#define RIG_ENGINE_PATH_CHARACTER_HANDL_M	"character_male_handl"
#define RIG_ENGINE_PATH_CHARACTER_HANDL_F	"character_female_handl"
#define RIG_ENGINE_PATH_CHARACTER_HANDR_M	"character_male_handr"
#define RIG_ENGINE_PATH_CHARACTER_HANDR_F	"character_female_handr"
#define RIG_ENGINE_PATH_CHARACTER_HEAD_M	"character_male_head"
#define RIG_ENGINE_PATH_CHARACTER_HEAD_F	"character_female_head"
#define RIG_ENGINE_PATH_CHARACTER_CAPE_M	"character_male_cape"
#define RIG_ENGINE_PATH_CHARACTER_CAPE_F	"character_female_cape"
#define RIG_ENGINE_PATH_WEAPON_ONE_M		"weapon_male_one"
#define RIG_ENGINE_PATH_WEAPON_ONE_F		"weapon_female_one"
#define RIG_ENGINE_PATH_WEAPON_TWO_M		"weapon_male_two"
#define RIG_ENGINE_PATH_WEAPON_TWO_F		"weapon_female_two"
#define RIG_ENGINE_PATH_WEAPON_DUALL_M		"weapon_male_duall"
#define RIG_ENGINE_PATH_WEAPON_DUALL_F		"weapon_female_duall"
#define RIG_ENGINE_PATH_WEAPON_DUALR_M		"weapon_male_dualr"
#define RIG_ENGINE_PATH_WEAPON_DUALR_F		"weapon_female_dualr"
#define RIG_ENGINE_PATH_HORSE_HEAD			"horse_head"
#define RIG_ENGINE_PATH_HORSE_BACK			"horse_back"
#define RIG_ENGINE_PATH_HORSE_TAIL			"horse_tail"

string GetPathForRigPart(RIG_ENGINE_PART part_name, bool gender)
{
	return "";
}

// F = free hand, O = one hand, T = two hand, D = dual hand
// C = critical
enum RIG_ENGINE_STATE
{
	RIG_ENGINE_STATE_ATK_F = 1,
	RIG_ENGINE_STATE_ATK_OC,
	RIG_ENGINE_STATE_ATK_O,
	RIG_ENGINE_STATE_ATK_TC,
	RIG_ENGINE_STATE_ATK_T,
	RIG_ENGINE_STATE_ATK_DC,
	RIG_ENGINE_STATE_ATK_D,
	RIG_ENGINE_STATE_FALL_F,
	RIG_ENGINE_STATE_FALL_O,
	RIG_ENGINE_STATE_FALL_T,
	RIG_ENGINE_STATE_FALL_D,
	RIG_ENGINE_STATE_HURT_F,
	RIG_ENGINE_STATE_HURT_O,
	RIG_ENGINE_STATE_HURT_T,
	RIG_ENGINE_STATE_HURT_D,
	RIG_ENGINE_STATE_JUMP,
	RIG_ENGINE_STATE_ATK_FC,
	RIG_ENGINE_STATE_CAST_F,
	RIG_ENGINE_STATE_CAST_O,
	RIG_ENGINE_STATE_CAST_T,
	RIG_ENGINE_STATE_CAST_D,
	RIG_ENGINE_STATE_RUN_F,
	RIG_ENGINE_STATE_RUN_O,
	RIG_ENGINE_STATE_RUN_T,
	RIG_ENGINE_STATE_RUN_D,
	RIG_ENGINE_STATE_IDLE_F,
	RIG_ENGINE_STATE_IDLE_O,
	RIG_ENGINE_STATE_IDLE_T,
	RIG_ENGINE_STATE_IDLE_D,
	RIG_ENGINE_STATE_FIGHTIDLE_F,
	RIG_ENGINE_STATE_FIGHTIDLE_O,
	RIG_ENGINE_STATE_FIGHTIDLE_T,
	RIG_ENGINE_STATE_FIGHTIDLE_D,
	RIG_ENGINE_STATE_WALK_F,
	RIG_ENGINE_STATE_WALK_O,
	RIG_ENGINE_STATE_WALK_T,
	RIG_ENGINE_STATE_WALK_D,
	RIG_ENGINE_STATE_ZEN,
	// HORSE
	RIG_ENGINE_STATE_HORSE_ATK_FC,
	RIG_ENGINE_STATE_HORSE_ATK_OC,
	RIG_ENGINE_STATE_HORSE_ATK_TC,
	RIG_ENGINE_STATE_HORSE_ATK_DC,
	RIG_ENGINE_STATE_HORSE_ATK_F,
	RIG_ENGINE_STATE_HORSE_ATK_O,
	RIG_ENGINE_STATE_HORSE_ATK_T,
	RIG_ENGINE_STATE_HORSE_ATK_D,
	RIG_ENGINE_STATE_HORSE_FALL_F,
	RIG_ENGINE_STATE_HORSE_FALL_O,
	RIG_ENGINE_STATE_HORSE_FALL_T,
	RIG_ENGINE_STATE_HORSE_FALL_D,
	RIG_ENGINE_STATE_HORSE_HURT_F,
	RIG_ENGINE_STATE_HORSE_HURT_O,
	RIG_ENGINE_STATE_HORSE_HURT_T,
	RIG_ENGINE_STATE_HORSE_HURT_D,
	RIG_ENGINE_STATE_HORSE_CAST_F,
	RIG_ENGINE_STATE_HORSE_CAST_O,
	RIG_ENGINE_STATE_HORSE_CAST_T,
	RIG_ENGINE_STATE_HORSE_CAST_D,
	RIG_ENGINE_STATE_HORSE_RUN_F,
	RIG_ENGINE_STATE_HORSE_RUN_O,
	RIG_ENGINE_STATE_HORSE_RUN_T,
	RIG_ENGINE_STATE_HORSE_RUN_D,
	RIG_ENGINE_STATE_HORSE_WALK_F,
	RIG_ENGINE_STATE_HORSE_WALK_O,
	RIG_ENGINE_STATE_HORSE_WALK_T,
	RIG_ENGINE_STATE_HORSE_WALK_D,
	RIG_ENGINE_STATE_HORSE_IDLE_F,
	RIG_ENGINE_STATE_HORSE_IDLE_O,
	RIG_ENGINE_STATE_HORSE_IDLE_T,
	RIG_ENGINE_STATE_HORSE_IDLE_D
};

const unsigned char RIG_ENGINE_MALE_STATE_TABLE[70][6] = {
	// FRAME, CLOTH, ONEHAND_WEAPON, TWOHAND_WEAPON, DUAL_WEAPON, HORSE
	{18, 1, 0, 0, 0, 0},
	{18, 1, 0, 0, 0, 0},
	{18, 1, 0, 0, 0, 0},
	{18, 1, 0, 0, 0, 0},
	{18, 1, 0, 0, 0, 0},
	{18, 1, 0, 0, 0, 0},
	{18, 1, 0, 0, 0, 0},
	{18, 1, 0, 0, 0, 0},
	{18, 1, 0, 0, 0, 0},
	{18, 1, 0, 0, 0, 0},
	{18, 1, 0, 0, 0, 0},
	{18, 1, 0, 0, 0, 0},
	{18, 1, 0, 0, 0, 0},
	{18, 1, 0, 0, 0, 0},
	{18, 1, 0, 0, 0, 0},
	{18, 1, 0, 0, 0, 0},
	{18, 1, 0, 0, 0, 0},
	{18, 1, 0, 0, 0, 0},
	{18, 1, 0, 0, 0, 0},
	{18, 1, 0, 0, 0, 0},
	{18, 1, 0, 0, 0, 0},
	{18, 1, 0, 0, 0, 0},
	{18, 1, 0, 0, 0, 0}
};
const unsigned char RIG_ENGINE_FEMALE_STATE_TABLE[70][5] = {

};
// TODO: need to design a new state system, NEED!