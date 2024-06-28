#pragma once
#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <map>
#include <string>
#include <memory>
#include <stdexcept>
#include <cassert>

template <typename Resource, typename Identifier>
class FileSystem
{
		public:
				void				load(Identifier id, const std::string& filename);

				template <typename Parameter>
				void				load(Identifier id, const std::string& filename, const Parameter& secondFile);

				Resource&			get(Identifier id);
				const Resource&		get(Identifier id) const;

		private:
				void				insertResource(Identifier id, std::unique_ptr<Resource> resource);

		private:

				std::map<Identifier, std::unique_ptr<Resource>> mResourceMap;
};
#include "FileSystem.inl"
#endif // !FILE_SYSTEM_H


