#include <dcore/Data/FileOutput.hpp>
using namespace dcore::data;

OutputAdapter::~OutputAdapter() {}
FileOutput::FileOutput(const std::string &root, const std::string &initialPath)
	: Resources(root), Path_(initialPath) {}

void FileOutput::SetAdapter(OutputAdapter *adapter) { Adapter_ = adapter; }
void FileOutput::Set(const picojson::value &value) { RootValue_ = value; }
void FileOutput::Write() { if(!Adapter_) return; Adapter_->Write(FullPath(Path_), RootValue_); }
