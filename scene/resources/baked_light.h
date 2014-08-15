#ifndef BAKED_LIGHT_H
#define BAKED_LIGHT_H

#include "resource.h"
#include "scene/resources/texture.h"

class BakedLight : public Resource {

	OBJ_TYPE( BakedLight, Resource);
public:
	enum Mode {

		MODE_OCTREE,
		MODE_LIGHTMAPS
	};

	enum Format {

		FORMAT_RGB,
		FORMAT_HDR8,
		FORMAT_HDR16
	};

	enum BakeFlags {
		BAKE_DIFFUSE,
		BAKE_SPECULAR,
		BAKE_TRANSLUCENT,
		BAKE_CONSERVE_ENERGY,
		BAKE_MAX
	};

private:

	RID baked_light;
	Mode mode;
	struct LightMap {
		Size2i gen_size;
		Ref<Texture> texture;
	};


	Vector< LightMap> lightmaps;

	//bake vars
	int cell_subdiv;
	int lattice_subdiv;
	float plot_size;
	float energy_multiply;
	float gamma_adjust;
	float cell_extra_margin;
	float edge_damp;
	float normal_damp;
	int bounces;
	bool transfer_only_uv2;
	Format format;
	bool flags[BAKE_MAX];



	void _update_lightmaps();

	Array _get_lightmap_data() const;
	void _set_lightmap_data(Array p_array);

protected:

	bool _set(const StringName& p_name, const Variant& p_value);
	bool _get(const StringName& p_name,Variant &r_ret) const;
	void _get_property_list( List<PropertyInfo> *p_list) const;

	static void _bind_methods();

public:

	void set_cell_subdivision(int p_subdiv);
	int get_cell_subdivision() const;

	void set_initial_lattice_subdiv(int p_size);
	int get_initial_lattice_subdiv() const;

	void set_plot_size(float p_size);
	float get_plot_size() const;

	void set_bounces(int p_size);
	int get_bounces() const;

	void set_energy_multiplier(float p_multiplier);
	float get_energy_multiplier() const;

	void set_gamma_adjust(float p_adjust);
	float get_gamma_adjust() const;

	void set_cell_extra_margin(float p_margin);
	float get_cell_extra_margin() const;

	void set_edge_damp(float p_margin);
	float get_edge_damp() const;

	void set_normal_damp(float p_margin);
	float get_normal_damp() const;

	void set_bake_flag(BakeFlags p_flags,bool p_enable);
	bool get_bake_flag(BakeFlags p_flags) const;

	void set_format(Format p_margin);
	Format get_format() const;

	void set_transfer_lightmaps_only_to_uv2(bool p_enable);
	bool get_transfer_lightmaps_only_to_uv2() const;

	void set_mode(Mode p_mode);
	Mode get_mode() const;

	void set_octree(const DVector<uint8_t>& p_octree);
	DVector<uint8_t> get_octree() const;

	void add_lightmap(const Ref<Texture> &p_texture,Size2 p_gen_size=Size2(256,256));
	void set_lightmap_gen_size(int p_idx,const Size2& p_size);
	Size2 get_lightmap_gen_size(int p_idx) const;
	void set_lightmap_texture(int p_idx,const Ref<Texture> &p_texture);
	Ref<Texture> get_lightmap_texture(int p_idx) const;
	void erase_lightmap(int p_idx);
	int  get_lightmaps_count() const;
	void clear_lightmaps();

	virtual RID get_rid() const;

	BakedLight();
	~BakedLight();
};


VARIANT_ENUM_CAST(BakedLight::Format);
VARIANT_ENUM_CAST(BakedLight::Mode);
VARIANT_ENUM_CAST(BakedLight::BakeFlags);

#endif // BAKED_LIGHT_H
