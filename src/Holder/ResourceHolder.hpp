#pragma once

#include <map>
#include <string>
#include <memory>
#include <stdexcept>
#include <cassert>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <iostream>


template <typename Resource, typename Identifier>
class ResourceHolder
{
	public:

		static ResourceHolder& getInstance();


		void load(Identifier id, const std::string& filename);

		template <typename Parameter>
		void load(Identifier id, const std::string& filename, const Parameter& secondParam);

		Resource& get(Identifier id);
		const Resource&	get(Identifier id) const;

	private:

		ResourceHolder() = default;
		~ResourceHolder() = default;

		ResourceHolder(const ResourceHolder&) = delete;
		ResourceHolder& operator=(const ResourceHolder&) = delete;

		void insertResource(Identifier id, std::unique_ptr<Resource> resource);

	private:
        std::map<Identifier, std::unique_ptr<Resource>>	nResourceMap;
};
// #include "ResourceHolder.inl"



#include "ResourceHolder.inl"