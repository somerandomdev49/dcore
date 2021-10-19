#pragma once
#include <string>
#include <dcore/Uni.hpp>
#include <dcore/Resource/ResourceLoader.hpp>
#include <dcore/Resource/ResourceManager.hpp>

namespace dcore::graphics::gui
{
	struct CodePoint
	{
		// The codepoint.
		int Char;
		// X advance. (how much to increase x offset to render the next glyph)
		int AdvanceWidth;
		// X offset of where to start rendering the glyph.
		int LeftSideBearing;
		// Offset in the texture atlas.
		int XOffset, YOffset;
	};

	class Font
	{
	public:
		Font(const byte *data, int pixelHeight, int fontNo = 0);
		~Font();

		int GetAscent() const;
		int GetDescent() const;
		int GetLineGap() const;
		int GetPixelHeight() const;
		float GetScale() const;
	private:
		int Ascent_, Descent_, LineGap_;
		float Scale_;
		int PixelHeight_;

		void *FontInfo__; // Implementation specific.
		int AtlasWidth_, AtlasHeight_;
		std::vector<CodePoint> CodePointTable_;
		RTexture *Atlas_;

		struct Bitmap { byte *data; int width, height; };
		Bitmap CreateAtlasBitmap_();
		void CreateAtlasTexture_(const Bitmap &tb);

		friend class GuiGraphics;
		friend class FontResourceManager;
		void Constructor_Font(const std::string &path, void *placement);
		void DeConstructor_Font(void *placement);
	};

	class FontResourceManager
	{
		static void Register(resource::ResourceLoader *rl);
	};
}
