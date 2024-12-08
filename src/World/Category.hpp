#pragma once

namespace Category
{
	enum Type
	{
		None				= 0,
		Scene				= 1 << 0,
		ItemScene			= 1 << 1,
		PlayerDough			= 1 << 2,
		Enemy 				= 1 << 3,
		Block 				= 1 << 4,
		Item 				= 1 << 5,
		Entity				= PlayerDough | Enemy,
		ALL 				= 0xFFFFFFFF,
	};
}