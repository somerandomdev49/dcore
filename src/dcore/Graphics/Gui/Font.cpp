#include <dcore/Graphics/GUI/Font.hpp>
#include <dcore/Renderer/Renderer.hpp>
#include <cstring>
#include <fstream> // FIXME: add LoaderUntil::LoadBinary()
#include <iterator>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>
#undef STB_TRUETYPE_IMPLEMENTATION

using namespace dcore::graphics::gui;

#define F_INF_(EXPR) ((stbtt_fontinfo *)(EXPR))

void Font::Initialize(const byte *data, int pixelHeight, int fontNo)
{
	stbtt_fontinfo *fontinfo = new stbtt_fontinfo();
	stbtt_InitFont(fontinfo, data, stbtt_GetFontOffsetForIndex(data, fontNo));
	PixelHeight_ = pixelHeight;
	Scale_       = stbtt_ScaleForPixelHeight(fontinfo, pixelHeight);
	printf("Initialized font: pixel height = %d, scaling = %f\n", PixelHeight_, Scale_);
	FontInfo__ = fontinfo;
	CreateAtlasTexture_(CreateAtlasBitmap_());
}

void Font::DeInitialize() { delete F_INF_(FontInfo__); }

// TODO! Dynamic language.
#define ATLAS_BEGIN_CHAR_ASCII ('a')
#define ATLAS_END_CHAR_ASCII   ('b'+1)
#define GLTEX_ALIGNMENT 4
#define PAD_ALIGN_TO_BYTES(N, A) ((~(N) + 1) & (A - 1))

// All of the glyphs are on a single line in a very wide texture.
Font::Bitmap Font::CreateAtlasBitmap_()
{
	stbtt_fontinfo *fi = F_INF_(FontInfo__);
	printf("Font info: 0x%zx\n", fi);

	Bitmap bitmap = {nullptr, 0, 0};

	int length;
	const char *nm = stbtt_GetFontNameString(fi, &length, 0, 0, 0x409, 1);
	printf("Font name: '%s'\n", nm);

	CodePointTable_.reserve(ATLAS_END_CHAR_ASCII - ATLAS_BEGIN_CHAR_ASCII);
	for(int c = ATLAS_BEGIN_CHAR_ASCII; c < ATLAS_END_CHAR_ASCII; ++c)
	{
		printf("Generating character #%d '%c'\n", c, c);
		int ix0, ix1, iy0, iy1, aw, lsb;
		stbtt_GetCodepointBitmapBox(fi, c, Scale_, Scale_, &ix0, &iy0, &ix1, &iy1);
		stbtt_GetCodepointHMetrics(fi, c, &aw, &lsb);
		CodePointTable_.push_back(CodePoint {c, aw, lsb, 0, 0, 0, 0});
		bitmap.width += ix1 - ix0;
		bitmap.height = std::max(bitmap.height, iy1 - iy0);
	}
	
	printf("Total bitmap size (non-aligned): %dx%d\n", bitmap.width, bitmap.height);

	// OpenGL requires its textures' width and height to be aligned to 4 bytes:
	bitmap.width += PAD_ALIGN_TO_BYTES(bitmap.width, GLTEX_ALIGNMENT);
	bitmap.height += PAD_ALIGN_TO_BYTES(bitmap.height, GLTEX_ALIGNMENT);
	bitmap.data = new byte[bitmap.width * bitmap.height];

	std::memset(bitmap.data, 0, bitmap.width * bitmap.height);

	int currentX = 0;
	for(int c = ATLAS_BEGIN_CHAR_ASCII, i = 0; c < ATLAS_END_CHAR_ASCII; ++c, ++i)
	{
		int w, h;
		auto bitmapData = stbtt_GetCodepointBitmap(fi, Scale_, Scale_, c, &w, &h, 0, 0);

		// int wpad = PAD_ALIGN_TO_BYTES(w, GLTEX_ALIGNMENT);

		// Write upside-down rendered bitmap data to the output bitmap.
		for(int y = h - 1, yi = 0; y >= 0; --y, ++yi)
		{
			for(int x = 0; x < w; ++x)
			{
				printf("Pixel at X%d <- X%d\n", currentX + x, x);
				bitmap.data[currentX + x + yi * bitmap.width] = bitmapData[x + y * w];
			}
		}


		CodePointTable_[i].XOffset = currentX;
		CodePointTable_[i].YOffset = 0;
		CodePointTable_[i].Width = w;
		CodePointTable_[i].Height = h;
		printf("Current X: %d, Width: %d\n", currentX, w);
		currentX += w;
	}
	printf("Current X: %d\n", currentX);

	printf("Total bitmap size: %dx%d\n", bitmap.width, bitmap.height);

	for(int j = 0; j < bitmap.height; ++j)
	{
		for(int i = 0; i < bitmap.width; ++i)
		{
			putc(" .:ioVM@"[bitmap.data[j*bitmap.width+i]>>5], stdout);
		}
		putc('\n', stdout);
	}

	return bitmap;
}

void Font::CreateAtlasTexture_(const Bitmap &tb)
{
	Atlas_ = new RTexture();
	RenderResourceManager::CreateTexture(Atlas_, tb.data, glm::ivec2(tb.width, tb.height), RenderResourceManager::TextureFormat::Red,
	                                     RenderResourceManager::TextureScaling::Nearest);

	delete tb.data;
}

dcore::graphics::RTexture *Font::GetAtlasTexture() const { return Atlas_; }

void Font::Constructor_Font(const std::string &path, void *placement)
{
	Font *f = new(placement) Font;
	std::ifstream input(path, std::ios::binary);
	std::vector<byte> buffer(std::istreambuf_iterator<char>(input), {});
	printf("Read %zu bytes of font data\n", buffer.size());
	f->Initialize(buffer.data(), 24);
}

void Font::DeConstructor_Font(void *placement)
{
	Font *f = reinterpret_cast<Font *>(placement);
	f->DeInitialize();
}

void FontResourceManager::Register(resource::ResourceLoader *rl)
{
	rl->RegisterResourceType<Font>("Font");
	resource::ResourceManager::Instance()->RegisterConstructor<Font>(&Font::Constructor_Font);
	resource::ResourceManager::Instance()->RegisterDeConstructor<Font>(&Font::DeConstructor_Font);
}
