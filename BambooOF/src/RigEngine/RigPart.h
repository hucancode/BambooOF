#pragma once
#include "ofMain.h"

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
enum RIG_ENGINE_GENDER
{
	RIG_ENGINE_GENDER_MALE,
	RIG_ENGINE_GENDER_FEMALE
};
class RigPart
{
	friend class RigPartRender;
private:
	string				m_Name;
	RIG_ENGINE_PART		m_Part;
	RIG_ENGINE_GENDER	m_Gender;
	short				m_Action;
	bool				m_NameChange;
	bool				m_PartChange;
	bool				m_GenderChange;
	bool				m_ActionChange;
public:
	RigPart();
	~RigPart();
	void				SetPart(RIG_ENGINE_PART part);
	void				SetGender(RIG_ENGINE_GENDER gender);
	void				SetName(string name);
	void				SubmitChanges();

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

class RigPartRenderer
{
};