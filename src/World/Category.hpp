#pragma once

namespace Category
{
	enum Type
	{
		None				= 0,
		Scene				= 1 << 0,
		ItemScene			= 1 << 1,
		EnemyScene			= 1 << 2,
		PlayerDough1		= 1 << 3,
		Enemy 				= 1 << 4,
		Block 				= 1 << 5,
		Item 				= 1 << 6,
		PlayerDough2 		= 1 << 7,
		PlayerDough 		= PlayerDough1 | PlayerDough2,
		Entity				= PlayerDough | Enemy,
		ALL 				= 0xFFFFFFFF,
	};
}