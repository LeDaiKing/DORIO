#pragma once

namespace Category
{
	enum Type
	{
		None				= 0,
		Scene				= 1 << 0,
		PlayerDough			= 1 << 1,
		Enemy 				= 1 << 2,
		Block 				= 1 << 3,
		Entity				= PlayerDough | Enemy,
	};
}