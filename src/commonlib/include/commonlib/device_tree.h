/* Taken from depthcharge: src/base/device_tree.h */
/* SPDX-License-Identifier: GPL-2.0-or-later */

#ifndef __COMMONLIB_DEVICE_TREE_H__
#define __COMMONLIB_DEVICE_TREE_H__

#include <stddef.h>
#include <stdint.h>
#include <commonlib/list.h>

/*
 * Flattened device tree structures/constants.
 */

struct fdt_header {
	uint32_t magic;
	uint32_t totalsize;
	uint32_t structure_offset;
	uint32_t strings_offset;
	uint32_t reserve_map_offset;

	uint32_t version;
	uint32_t last_comp_version;

	uint32_t boot_cpuid_phys;

	uint32_t strings_size;
	uint32_t structure_size;
};

#define FDT_HEADER_MAGIC	0xd00dfeed
#define FDT_SUPPORTED_VERSION	17
#define FDT_TOKEN_BEGIN_NODE	1
#define FDT_TOKEN_END_NODE	2
#define FDT_TOKEN_PROPERTY	3
#define FDT_TOKEN_NOP		4
#define FDT_TOKEN_END		9
#define FDT_PHANDLE_ILLEGAL	0xdeadbeef

struct fdt_property
{
	const char *name;
	void *data;
	uint32_t size;
};

/*
 * Unflattened device tree structures.
 */

struct device_tree_region {
	u64 addr;
	u64 size;
};

struct device_tree_property
{
	struct fdt_property prop;

	struct list_node list_node;
};

struct device_tree_node
{
	const char *name;
	uint32_t phandle;

	/* List of struct device_tree_property-s. */
	struct list_node properties;
	/* List of struct device_tree_nodes. */
	struct list_node children;

	struct list_node list_node;
};

struct device_tree_reserve_map_entry
{
	uint64_t start;
	uint64_t size;

	struct list_node list_node;
};

struct device_tree
{
	const void *header;
	uint32_t header_size;
	uint32_t max_phandle;

	struct list_node reserve_map;

	struct device_tree_node *root;
};

/*
 * Flattened device tree functions. These generally return the number of bytes
 * which were consumed reading the requested value.
 */

/* Checks if blob points to a valid FDT */
bool fdt_is_valid(const void *blob);
/* Read the property at offset, if any exists. */
int fdt_next_property(const void *blob, uint32_t offset,
		      struct fdt_property *prop);
/*
 * Read the node name into 'name' of the node behind 'node_offset'
 * and return total bytes used for name
 */
int fdt_next_node_name(const void *blob, uint32_t node_offset, const char **name);

void fdt_print_node(const void *blob, uint32_t offset);
int fdt_skip_node(const void *blob, uint32_t offset);

/* Read property and put into fdt_prop. Returns offset to property */
u32 fdt_read_prop(const void *blob, u32 node_offset, const char *prop_name,
		  struct fdt_property *fdt_prop);
/* Read reg property and save regions inside 'regions'. Returns number of regions read */
u32 fdt_read_reg_prop(const void *blob, u32 node_offset, u32 addr_cells, u32 size_cells,
		      struct device_tree_region regions[], size_t regions_count);
/* Reads value for a fdt_prop, considering the cells */
uint64_t fdt_read_int_prop(struct fdt_property *prop, u32 cells);
/* Find a node by a given path and return the offset */
u32 fdt_find_node_by_path(const void *blob, const char *path, u32 *addrcp, u32 *sizecp);
/* Find multiple nodes matching a given pattern. Returns number of nodes found */
size_t fdt_find_subnodes_by_prefix(const void *blob, u32 node_offset, const char *prefix,
				   u32 *addrcp, u32 *sizecp, u32 results[], size_t results_len);
/* Find a node by a given alias and return its offset */
u32 fdt_find_node_by_alias(const void *blob, const char *alias_name,
			   u32 *addr_cells, u32 *size_cells);

 /* Read memory regions from a flat device-tree. */
size_t fdt_read_memory_regions(const void *blob, struct device_tree_region regions[],
			       size_t regions_count);
 /* Find top of memory from a flat device-tree. */
uint64_t fdt_get_memory_top(const void *blob);

/* Read a flattened device tree into a hierarchical structure which refers to
   the contents of the flattened tree in place. Modifying the flat tree
   invalidates the unflattened one. */
struct device_tree *fdt_unflatten(const void *blob);

/*
 * Unflattened device tree functions.
 */

/* Figure out how big a device tree would be if it were flattened. */
uint32_t dt_flat_size(const struct device_tree *tree);
/* Flatten a device tree into the buffer pointed to by dest. */
void dt_flatten(const struct device_tree *tree, void *dest);
void dt_print_node(const struct device_tree_node *node);
size_t dt_read_reg_prop(struct device_tree_node *node, u32 addr_cells, u32 size_cells,
			struct device_tree_region regions[], size_t regions_count);
/* Read #address-cells and #size-cells properties from a node. */
void dt_read_cell_props(const struct device_tree_node *node, u32 *addrcp,
			u32 *sizecp);
/* Look up or create a node relative to a parent node, through its path
   represented as an array of strings. */
struct device_tree_node *dt_find_node(struct device_tree_node *parent,
				      const char *const *path, u32 *addrcp,
				      u32 *sizecp, int create);
struct device_tree_node *dt_find_node_by_phandle(struct device_tree_node *root,
						 uint32_t phandle);
/* Look up or create a node in the tree, through its path
   represented as a string of '/' separated node names. */
struct device_tree_node *dt_find_node_by_path(struct device_tree *tree,
			const char *path, u32 *addrcp, u32 *sizecp, int create);
/* Look up a node through an alias. */
struct device_tree_node *dt_find_node_by_alias(struct device_tree *tree,
					       const char *alias);
/* Look up a node relative to a parent node, through its compatible string. */
struct device_tree_node *dt_find_compat(struct device_tree_node *parent, const char *compatible);
/* Look up the next child of a parent node, through its compatible string. It
   uses child pointer as the marker to find next. */
struct device_tree_node *dt_find_next_compat_child(struct device_tree_node *parent,
					  struct device_tree_node *child,
					  const char *compat);
/* Look up a node relative to a parent node, through its property value. */
struct device_tree_node *dt_find_prop_value(struct device_tree_node *parent, const char *name,
				   void *data, size_t size);
/* Write src into *dest as a 'length'-byte big-endian integer. */
void dt_write_int(u8 *dest, u64 src, size_t length);
/* Delete a property */
void dt_delete_prop(struct device_tree_node *node, const char *name);
/* Add different kinds of properties to a node, or update existing ones. */
void dt_add_bin_prop(struct device_tree_node *node, const char *name,
		     void *data, size_t size);
void dt_add_string_prop(struct device_tree_node *node, const char *name,
			const char *str);
void dt_add_u32_prop(struct device_tree_node *node, const char *name, u32 val);
void dt_add_u64_prop(struct device_tree_node *node, const char *name, u64 val);
void dt_add_reg_prop(struct device_tree_node *node, const u64 *addrs, const u64 *sizes,
		     int count, u32 addr_cells, u32 size_cells);
void dt_add_iommu_addr_prop(struct device_tree_node *node, const u64 *addrs, const u64 *sizes,
			    int count, u32 addr_cells, u32 size_cells);
int dt_set_bin_prop_by_path(struct device_tree *tree, const char *path,
			    void *data, size_t size, int create);

void dt_find_bin_prop(const struct device_tree_node *node, const char *name,
		      const void **data, size_t *size);
const char *dt_find_string_prop(const struct device_tree_node *node,
				const char *name);

/* Apply an overlay to a base device tree. Ownership of the overlay data passes
   to the newly combined base tree -- do not free() or access it afterwards! */
int dt_apply_overlay(struct device_tree *tree, struct device_tree *overlay);

/*
 * Fixups to apply to a kernel's device tree before booting it.
 */

struct device_tree_fixup
{
	/**
	 * The function which does the fixing.
	 * 0 on success, non-zero on error.
	 */
	int (*fixup)(struct device_tree_fixup *fixup,
		struct device_tree *tree);

	struct list_node list_node;
};

extern struct list_node device_tree_fixups;

/**
 * Function to apply fixups.
 * 0 on success, non-zero on error.
 */
int dt_apply_fixups(struct device_tree *tree);

/*
 * Init/retrieve the /reserved-memory/ node.
 */
struct device_tree_node *dt_init_reserved_memory_node(struct device_tree *tree);

#endif /* __COMMONLIB_DEVICE_TREE_H__ */
