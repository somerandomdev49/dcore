#include <dcore/Graphics/GUI/Font.hpp>
#include <dcore/Renderer/Renderer.hpp>
#include <dcore/Core/Log.hpp>
#include <dcore/Core/Assert.hpp>
#include <cstring>
#include <fstream> // FIXME: add LoaderUntil::LoadBinary()
#include <iterator>

#include <ft2build.h>
#include FT_FREETYPE_H

// #define STB_TRUETYPE_IMPLEMENTATION
// #include <stb_truetype.h>
// #undef STB_TRUETYPE_IMPLEMENTATION

using namespace dcore::graphics::gui;

#define F_INF_(EXPR) ((FT_Face)(EXPR))

static FT_Library libft__;
void Font::FontLibInitialize() { DCORE_ASSERT(!FT_Init_FreeType(&libft__), "Could not initialize freetype"); }

void Font::FontLibDeInitialize() { DCORE_ASSERT(!FT_Done_FreeType(libft__), "Could not deinitialize freetype"); }

void Font::Initialize(const char *name, int fontSize, int fontNo)
{
	FT_Face fc;
	PixelHeight_ = fontSize;
	Scale_       = 1.0f;

	DCORE_ASSERT(!FT_New_Face(libft__, name, fontNo, &fc), "Could not create font face");
	FT_Set_Pixel_Sizes(fc, 0, PixelHeight_);

	FontInfo__ = fc;
	CreateAtlasTexture_(CreateAtlasBitmap_());
}

int Font::GetAscent() const { return F_INF_(FontInfo__)->ascender >> 6; }

void Font::DeInitialize() { FT_Done_Face(F_INF_(FontInfo__)); }

int Font::GetKerning(int a, int b) const
{
	if(!FT_HAS_KERNING(F_INF_(FontInfo__))) return 0;
	FT_Vector kern;
	FT_Get_Kerning(F_INF_(FontInfo__), a, b, FT_KERNING_DEFAULT, &kern);
	return kern.x;
}

// TODO! Dynamic language.
#define ATLAS_BEGIN_CHAR_ASCII   (' ')
#define ATLAS_END_CHAR_ASCII     ('~' + 1)
#define GLTEX_ALIGNMENT          4
#define PAD_ALIGN_TO_BYTES(N, A) ((~(N) + 1) & (A - 1))

// All of the glyphs are on a single line in a very wide texture.
Font::Bitmap Font::CreateAtlasBitmap_()
{
	FT_Face fc = F_INF_(FontInfo__);
	// printf("Font info: 0x%zx\n", fi);

	Bitmap bitmap = {nullptr, 0, 0};
	CodePointTable_.reserve(ATLAS_END_CHAR_ASCII - ATLAS_BEGIN_CHAR_ASCII);
	for(int c = ATLAS_BEGIN_CHAR_ASCII; c < ATLAS_END_CHAR_ASCII; ++c)
	{
		if(FT_Load_Char(fc, c, FT_LOAD_RENDER))
		{
			DCORE_LOG_ERROR << "Failed to load character '" << (char)c << "'from font face.";
			continue;
		}
		bitmap.width += fc->glyph->bitmap.width;
		bitmap.height = std::max(fc->glyph->bitmap.rows, bitmap.height);
		CodePointTable_.push_back(CodePoint());
	}

	printf("Total bitmap size: %dx%d\n", bitmap.width, bitmap.height);
	bitmap.data = new byte[bitmap.width * bitmap.height];
	std::memset(bitmap.data, 0, bitmap.width * bitmap.height);

	int currentX = 0;
	for(int c = ATLAS_BEGIN_CHAR_ASCII, i = 0; c < ATLAS_END_CHAR_ASCII; ++c, ++i)
	{
		if(FT_Load_Char(fc, c, FT_LOAD_RENDER))
		{
			DCORE_LOG_ERROR << "Failed to load character '" << (char)c << "'from font face.";
			continue;
		}

		// Write the rendered bitmap data to the output bitmap.
		for(size_t y = 0; y < fc->glyph->bitmap.rows; ++y)
		{
			for(size_t x = 0; x < fc->glyph->bitmap.width; ++x)
			{
				// printf("Pixel at X%d <- X%d\n", currentX + x, x);
				bitmap.data[currentX + x + y * bitmap.width] =
				    fc->glyph->bitmap.buffer[x + y * fc->glyph->bitmap.width];
			}
		}

		auto cp          = &CodePointTable_[i];
		cp->Char         = c;
		cp->AdvanceWidth = fc->glyph->advance.x;
		cp->Bearing      = glm::ivec2(fc->glyph->bitmap_left, fc->glyph->bitmap_top);
		cp->AtlasOffset  = glm::ivec2(currentX, 0);
		cp->AtlasSize    = glm::ivec2(fc->glyph->bitmap.width, fc->glyph->bitmap.rows);
		cp->UVOffset     = glm::vec2(cp->AtlasOffset) / glm::vec2(bitmap.width, bitmap.height);
		cp->UVSize       = glm::vec2(cp->AtlasSize) / glm::vec2(bitmap.width, bitmap.height);
		// printf("Glyph '%c':\n  Advance: %i,\n  AtlasOffset: %d, %d\n  AtlasSize: %d, %d"
		//        "\n  Bearing: %d, %d\n  UV: %f, %f; %f %f\n",
		//        c, cp->AdvanceWidth, cp->AtlasOffset.x, cp->AtlasOffset.y, cp->AtlasSize.x, cp->AtlasSize.y,
		//        cp->Bearing.x, cp->Bearing.y, cp->UVOffset.x, cp->UVOffset.y, cp->UVSize.x, cp->UVSize.y);
		currentX += fc->glyph->bitmap.width;
	}

	// int currentX = 0;
	// {
	// 	int w, h;
	// 	auto bitmapData = stbtt_GetCodepointBitmap(fi, Scale_, Scale_, c, &w, &h, 0, 0);

	// 	// int wpad = PAD_ALIGN_TO_BYTES(w, GLTEX_ALIGNMENT);

	// 	cp->XOffset   = currentX;
	// 	cp->YOffset   = 0;
	// 	cp->Width     = w;
	// 	cp->Height    = h;
	// 	cp->XOffsetUV = currentX / float(bitmap.width);
	// 	cp->YOffsetUV = 0 / float(bitmap.height);
	// 	cp->WidthUV   = w / float(bitmap.width);
	// 	cp->HeightUV  = h / float(bitmap.height);
	// 	printf("Offset UV: %f, %f, Size UV: %f, %f\n", cp->XOffsetUV, cp->YOffsetUV, cp->WidthUV, cp->HeightUV);
	// 	printf("Current X: %d, Width: %d\n", currentX, w);
	// 	currentX += w;
	// }
	// printf("Current X: %d\n", currentX);

	// printf("Total bitmap size: %dx%d\n", bitmap.width, bitmap.height);

	// for(size_t j = 0; j < bitmap.height; ++j)
	// {
	// 	for(size_t i = 0; i < bitmap.width; ++i)
	// 	{
	// 		putc(" .:ioVM@"[bitmap.data[j * bitmap.width + i] >> 5], stdout);
	// 	}
	// 	putc('\n', stdout);
	// }

	return bitmap;
}

void Font::CreateAtlasTexture_(const Bitmap &tb)
{
	Atlas_ = new RTexture();
	RenderResourceManager::CreateTexture(Atlas_, tb.data, glm::ivec2(tb.width, tb.height),
	                                     RenderResourceManager::TextureFormat::Red,
	                                     RenderResourceManager::TextureScaling::Linear,
	                                     /* alignment */ 1);

	delete tb.data;
}

dcore::graphics::RTexture *Font::GetAtlasTexture() const { return Atlas_; }

void Font::Constructor_Font(const std::string &path, void *placement)
{
	Font *f     = new(placement) Font;
	auto  split = path.rfind(':');
	int   size;
	if(split == path.npos)
	{
		DCORE_LOG_ERROR << "Error loading font: no size specified!";
		size = 64;
	}
	else
	{
		size = std::stoi(path.substr(split + 1));
	}
	std::string actualPath = path.substr(0, split);
	printf("Reading font from '%s'\n", actualPath.c_str());
	// std::ifstream input(actualPath, std::ios::binary);
	// std::vector<byte> buffer(std::istreambuf_iterator<char>(input), {});
	// printf("Read %zu bytes of font data\n", buffer.size());
	f->Initialize(actualPath.c_str(), size, 0);
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
