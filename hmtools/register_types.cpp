
#include "register_types.h"
#include "hm_tools.h"

void initialize_hmtools_module(ModuleInitializationLevel p_level) 
{
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) return;

	// Register Plugin
	EditorPlugins::add_by_type<HMTools>();

	// Verbose
	print_line("HM Tools Initialized.");
}

void uninitialize_hmtools_module(ModuleInitializationLevel p_level)
{
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) return;
}
