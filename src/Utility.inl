template <typename T>	
sf::Vector2<T> toVector2(const nlohmann::json& json)
{
	return sf::Vector2<T>(json.at(0), json.at(1));
}