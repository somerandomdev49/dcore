#include <dcore/Data/FileInput.hpp>

namespace dcore::data
{
	InputAdapter::~InputAdapter() {}
	FileInput::FileInput(const std::string &root, const std::string &initialPath)
	    : Resources(root), Path_(initialPath)
	{
	}

	void FileInput::SetAdapter(InputAdapter *adapter) { Adapter_ = adapter; }
	const Json &FileInput::Get() const { return RootValue_; }
	void FileInput::Read()
	{
		if(!Adapter_) return;
		RootValue_ = Adapter_->Read(FullPath(Path_));
	}
} // namespace dcore::data
