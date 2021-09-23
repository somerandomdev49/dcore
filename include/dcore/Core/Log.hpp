#pragma once
#include <quill/Quill.h>

#define DCORE_LOG_INFO(...) LOG_INFO(quill::get_logger(), __VA_ARGS__)
#define DCORE_LOG_DEBUG(...) LOG_DEBUG(quill::get_logger(), __VA_ARGS__)
#define DCORE_LOG_ERROR(...) LOG_ERROR(quill::get_logger(), __VA_ARGS__)
#define DCORE_LOG_CRITICAL(...) LOG_CRITICAL(quill::get_logger(), __VA_ARGS__)
#define DCORE_LOG_WARNING(...) LOG_WARNING(quill::get_logger(), __VA_ARGS__)
