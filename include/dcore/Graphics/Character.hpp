#pragma once
#include <dcore/Graphics/SkeletalMesh.hpp>
#include <dcore/Graphics/StaticMesh.hpp>
#include <string>
#include <vector>

namespace dcore::graphics
{
	/** Skeletal Mesh + Clothing/Armor */
	class Character
	{
	public:
		struct ArmorPiece { std::string BoneName; StaticMesh *Mesh; };

	private:
		SkeletalMesh *Body_;
		std::vector<ArmorPiece> Armor_;
	};
}
