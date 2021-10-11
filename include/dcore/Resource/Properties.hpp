#pragma once
#include <dcore/Resource/Resources.hpp>
#include <unordered_map>
#include <glm/glm.hpp>

namespace dcore::resource
{
	class Properties : Resources
	{
	public:
		Properties(const std::string &root);

		void Initialize();
		void Load(const std::string &location);

		const glm::ivec2 &GetWindowSize();
		void DeInitialize();

		static Properties *DefaultInstance();
		static void SetDefaultInstance(Properties *newDefault);

	private:
		std::unordered_map<std::string, std::string> Data_;
	};
} // namespace dcore::resource
