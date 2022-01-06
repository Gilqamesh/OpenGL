#pragma once
#ifndef MATERIALTYPES_HPP
# define MATERIALTYPES_HPP

# include <array>

struct Material
{
	std::array<float, 3> ambientColor;
	std::array<float, 3> diffuseColor;
	std::array<float, 3> specularColor;
	float				 shininess;
};

struct MaterialTypes
{
	static const Material emerald;
	static const Material jade;
	static const Material obsidian;
	static const Material pearl;
	static const Material ruby;
	static const Material turquoise;
	static const Material brass;
	static const Material bronze;
	static const Material chrome;
	static const Material copper;
	static const Material gold;
	static const Material silver;
	static const Material black_plastic;
	static const Material cyan_plastic;
	static const Material green_plastic;
	static const Material red_plastic;
	static const Material white_plastic;
	static const Material yellow_plastic;
	static const Material black_rubber;
	static const Material cyan_rubber;
	static const Material green_rubber;
	static const Material red_rubber;
	static const Material white_rubber;
	static const Material yellow_rubber;
};

#endif
