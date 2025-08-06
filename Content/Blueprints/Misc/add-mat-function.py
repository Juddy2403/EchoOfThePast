import unreal

# CONFIGURATION
TARGET_FOLDER = "/Game/Materials/Level1"  # Your folder path here
MATERIAL_FUNCTION_PATH = "/Game/Materials/PostProcess/MF_EnvironmentMask"  # Your material function path

# Load the material function asset
material_function = unreal.load_asset(MATERIAL_FUNCTION_PATH)

if not material_function:
    unreal.log_error(f"Material Function not found at: {MATERIAL_FUNCTION_PATH}")
    raise SystemExit

unreal.log(f"Loaded Material Function: {material_function.get_name()}")

# Get all assets in the folder
all_assets = unreal.EditorAssetLibrary.list_assets(TARGET_FOLDER, recursive=True, include_folder=False)

# Properly filter for actual Material assets
material_assets = [
    a for a in all_assets 
    if unreal.EditorAssetLibrary.does_asset_exist(a) 
    and unreal.EditorAssetLibrary.find_asset_data(a).get_asset().get_class().get_name() == "Material"
]

if not material_assets:
    unreal.log_warning(f"No materials found in folder: {TARGET_FOLDER}")
else:
    unreal.log(f"Found {len(material_assets)} materials in {TARGET_FOLDER}")

with unreal.ScopedEditorTransaction("Add Opacity Mask Function to Materials"):

    for material_path in material_assets:
        material = unreal.load_asset(material_path)
        if not material:
            unreal.log_warning(f"Failed to load material: {material_path}")
            continue

        blend_mode = material.get_editor_property("blend_mode")
        if blend_mode not in [unreal.BlendMode.BLEND_OPAQUE, unreal.BlendMode.BLEND_MASKED]:
            unreal.log(f"Skipping {material.get_name()}: Not Opaque or Masked (blend mode = {blend_mode})")
            continue

        unreal.log(f"Modifying: {material.get_name()}")

        ed_graph = unreal.MaterialEditingLibrary
        existing_mask_node = ed_graph.get_material_property_input_node(material, unreal.MaterialProperty.MP_OPACITY_MASK)

        # Add material function call node
        function_call = ed_graph.create_material_expression(material, unreal.MaterialExpressionMaterialFunctionCall, -300, 0)
        function_call.set_material_function(material_function)

        # If there's already an opacity mask
        # If there's already an opacity mask
        if existing_mask_node:
        # Check if it's already the same material function

            is_existing_function = (
                isinstance(existing_mask_node, unreal.MaterialExpressionMaterialFunctionCall)
                and existing_mask_node.get_editor_property("material_function") == material_function
            )

            if is_existing_function:
                unreal.log(f"Skipped {material.get_name()}: Already uses the same opacity mask function.")
                continue

            # Multiply the existing mask with the new function output
            multiply_node = ed_graph.create_material_expression(material, unreal.MaterialExpressionMultiply, -150, 0)

            # Connect existing node to A
            ed_graph.connect_material_expressions(existing_mask_node, "", multiply_node, "A")

            # Connect function output to B
            ed_graph.connect_material_expressions(function_call, "", multiply_node, "B")

            # Connect multiply result to Opacity Mask
            ed_graph.connect_material_property(multiply_node, "", unreal.MaterialProperty.MP_OPACITY_MASK)
            unreal.log(f"Multiplied existing opacity mask with function for: {material.get_name()}")
        else:
            # No existing input, just use the function directly
            ed_graph.connect_material_property(function_call, "", unreal.MaterialProperty.MP_OPACITY_MASK)
            unreal.log(f"Connected function directly to opacity mask for: {material.get_name()}")

        if blend_mode == unreal.BlendMode.BLEND_OPAQUE:
            material.set_editor_property("blend_mode", unreal.BlendMode.BLEND_MASKED)
            unreal.log(f"Changed blend mode of {material.get_name()} to Masked.")

        ed_graph.recompile_material(material)
        unreal.EditorAssetLibrary.save_asset(material_path)
        unreal.log(f"Saved material: {material.get_name()}") 