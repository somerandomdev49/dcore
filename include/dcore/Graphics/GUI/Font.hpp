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
		unsigned int AdvanceWidth;

		// Offset from baseline to left/top of glyph.
		glm::ivec2 Bearing;

		// Offset in the texture atlas.
		glm::ivec2 AtlasOffset;

		// Size of the character in the texture atlas.
		glm::ivec2 AtlasSize;

		// Offset for uv.
		glm::vec2 UVOffset;

		// Size for uv.
		glm::vec2 UVSize;
	};

	class Font
	{
	public:
		static void FontLibInitialize();
		static void FontLibDeInitialize();

		void Initialize(const char *name, int fontSize, int fontNo = 0);
		void DeInitialize();

		int       GetAscent() const;
		int       GetDescent() const;
		int       GetLineGap() const;
		int       GetPixelHeight() const;
		float     GetScale() const;
		RTexture *GetAtlasTexture() const;

	private:
		float Scale_, ScaleEm_;
		int   PixelHeight_;

		void     *FontInfo__; // Implementation specific.
		int       AtlasWidth_, AtlasHeight_;
		RTexture *Atlas_;

		std::vector<CodePoint> CodePointTable_;

		struct Bitmap
		{
			byte        *data;
			unsigned int width, height;
		};

		Bitmap CreateAtlasBitmap_();
		void   CreateAtlasTexture_(const Bitmap &tb);

		int GetKerning(int a, int b) const;

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
