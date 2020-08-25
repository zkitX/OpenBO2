#pragma once

union Weapon
{
	struct {
		unsigned __int32 weaponIdx : 8;
		unsigned __int32 attachment1 : 6;
		unsigned __int32 attachment2 : 6;
		unsigned __int32 attachment3 : 6;
		unsigned __int32 padding : 6;
	};
	unsigned int weaponData;
};

class g_weapon
{
};

