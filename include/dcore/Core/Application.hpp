#pragma once
#include <string>
#include <vector>

namespace dcore
{
	struct Error
	{
		bool Critical;
		std::string Label, Desc;
	};

	class Application
	{
	public:
		static class InfoT
		{
		public:
			void SetError(const Error &err);
			void SetError(bool critical, const std::string &label, const std::string &desc);
			const Error &GetError();

		private:
			std::vector<Error> Errors_;
		} Info;
	};
} // namespace dcore
