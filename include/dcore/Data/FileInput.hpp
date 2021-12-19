#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <dcore/Resource/Resources.hpp>
#include <dcore/Data/Json.hpp>

namespace dcore::data
{
	/** Base class responsible from writing in a specific format. */
	class InputAdapter
	{
	public:
		virtual ~InputAdapter();
		virtual Json Read(const std::string &where) = 0;
	};

	/** Class for outpting serialized json data. */
	class FileInput : public resource::Resources
	{
	public:
		FileInput(const std::string &root, const std::string &initialPath);

		/** Gets the value of the file. */
		const Json &Get() const;

		/** Reads with the adapter. */
		void Read();

		InputAdapter *GetAdapter() const;
		void SetAdapter(InputAdapter *newAdapter);

		void SetPath(const std::string &newPath);
		const std::string &GetPath() const;

	private:
		std::string Path_;
		Json RootValue_;
		InputAdapter *Adapter_;
	};
} // namespace dcore::data
