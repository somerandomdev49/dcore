#pragma once
#include <string>
#include <dcore/Uni.hpp>
#include <dcore/Resource/ResourceLoader.hpp>
#include <dcore/Resource/ResourceManager.hpp>
#include <dcore/Renderer/RTexture.hpp>

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
		// Size of the character in the texture atlas.
		int Width, Height;
	};

	class Font
	{
	public:
		void Initialize(const byte *data, int pixelHeight, int fontNo = 0);
		void DeInitialize();

		int GetAscent() const;
		int GetDescent() const;
		int GetLineGap() const;
		int GetPixelHeight() const;
		float GetScale() const;
		RTexture *GetAtlasTexture() const;

	private:
		int Ascent_, Descent_, LineGap_;
		float Scale_;
		int PixelHeight_;

		void *FontInfo__; // Implementation specific.
		int AtlasWidth_, AtlasHeight_;
		std::vector<CodePoint> CodePointTable_;
		RTexture *Atlas_;

		struct Bitmap
		{
			byte *data;
			int width, height;
		};
		Bitmap CreateAtlasBitmap_();
		void CreateAtlasTexture_(const Bitmap &tb);

		friend class GuiGraphics;
		friend class FontResourceManager;
		static void Constructor_Font(const std::string &path, void *placement);
		static void DeConstructor_Font(void *placement);
	};

	class FontResourceManager
	{
	public:
		static void Register(resource::ResourceLoader *rl);
	};
} // namespace dcore::graphics::gui
