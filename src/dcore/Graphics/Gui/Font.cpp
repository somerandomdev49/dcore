#include <dcore/Graphics/GUI/Font.hpp>
#include <dcore/Renderer/Renderer.hpp>
#include <cstring>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>
#undef STB_TRUETYPE_IMPLEMENTATION

using namespace dcore::graphics::gui;

#define F_INF_(EXPR) ((stbtt_fontinfo*)(EXPR)) 

Font::Font(const byte *data, int pixelHeight, int fontNo)
	: PixelHeight_(pixelHeight)
{
	stbtt_fontinfo *fontinfo = new stbtt_fontinfo();
	stbtt_InitFont(fontinfo, data, stbtt_GetFontOffsetForIndex(data, 0));
	CreateAtlasTexture_(CreateAtlasBitmap_());
	
	FontInfo__ = fontinfo;
}

Font::~Font() { delete F_INF_(FontInfo__); }


// TODO! Dynamic language.
#define ATLAS_BEGIN_CHAR_ASCII ' '
#define ATLAS_END_CHAR_ASCII '~'

// All of the glyphs are on a single line in a very wide texture.
Font::Bitmap Font::CreateAtlasBitmap_()
{
	stbtt_fontinfo *fi = F_INF_(FontInfo__);

	Bitmap bitmap = { nullptr, 0, 0 };

	CodePointTable_.reserve(ATLAS_END_CHAR_ASCII - ATLAS_BEGIN_CHAR_ASCII);
	for(int c = ATLAS_BEGIN_CHAR_ASCII; c < ATLAS_END_CHAR_ASCII; ++c)
	{
		int ix0, ix1, iy0, iy1, aw, lsb;
		stbtt_GetCodepointBitmapBox(fi, c, 0, Scale_, &ix0, &iy0, &ix1, &iy0);
		stbtt_GetCodepointHMetrics(fi, c, &aw, &lsb);
		CodePointTable_.push_back(CodePoint{ c, aw, lsb, 0, 0 });
		bitmap.width += ix1 - ix0;
		bitmap.height = std::max(bitmap.height, iy1 - iy0);
	}

	bitmap.data = new byte[bitmap.width * bitmap.height];

	int currentX = 0;
	for(int c = ATLAS_BEGIN_CHAR_ASCII, i = 0; c < ATLAS_END_CHAR_ASCII; ++c, ++i)
	{
		int w, h;
		auto bitmapData = stbtt_GetCodepointBitmap(fi, 0, Scale_, c, &w, &h, 0, 0);
		std::memcpy(bitmap.data + currentX, bitmapData, w * h);
		stbtt_FreeBitmap(bitmapData, nullptr);
		CodePointTable_[i].XOffset = currentX;
		CodePointTable_[i].YOffset = 0;
		currentX += w;
	}

	return bitmap;
}

void Font::CreateAtlasTexture_(const Bitmap &tb)
{
	Atlas_ = new RTexture();
	RenderResourceManager::CreateTexture(
		Atlas_,
		tb.data,
		glm::ivec2(tb.width, tb.height),
		RenderResourceManager::TextureFormat::Red);

	delete tb.data;
}

