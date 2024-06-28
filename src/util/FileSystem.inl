
template<typename Resource, typename Identifier>
void FileSystem<Resource, Identifier>::load(Identifier id, const std::string& filename)
{
	std::unique_ptr<Resource> resource(new Resource());
	if (!resource->loadFromFile(filename))
		throw std::runtime_error("FileSystem::load - Failed to load " + filename);

	// if loading was succesfull, insert resource map
	insertResource(id, std::move(resource));
}

template<typename Resource, typename Identifier>
template<typename Parameter>
void FileSystem<Resource, Identifier>::load(Identifier id, const std::string& filename, const Parameter& secondFile)
{
	std::unique_ptr<Resource> resource(new Resource());
	if (!resource->loadFromFile(filename, secondFile))
		throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);

	insertResource(id, std::move(resource));
}

template<typename Resource, typename Identifier>
Resource& FileSystem<Resource, Identifier>::get(Identifier id)
{
	auto found = mResourceMap.find(id);
	assert(found != mResourceMap.end());

	return *found->second;
}

template<typename Resource, typename Identifier>
const Resource& FileSystem<Resource, Identifier>::get(Identifier id) const
{
	auto found = mResourceMap.find(id);
	assert(found != mResourceMap.end());

	return *found->second;
}

template<typename Resource, typename Identifier>
void FileSystem<Resource, Identifier>::insertResource(Identifier id, std::unique_ptr<Resource> resource)
{
	auto inserted = mResourceMap.insert(std::make_pair(id, std::move(resource)));
	assert(inserted.second);
}
