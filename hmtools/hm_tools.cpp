
#include "hm_tools.h"
#include "core/io/resource_loader.h"
#include "scene/main/node.h"
#include "scene/main/window.h"
#include "scene/main/scene_tree.h"
#include "scene/resources/animation.h"
#include "scene/gui/text_edit.h"
#include "editor/editor_interface.h"
#include "editor/gui/editor_file_dialog.h"

// Forward Declarations
void OpenAnimationRepathTool(SceneTree *scene_tree);

// Utilities
static Node* FindNodeByName(Node *parent, const String &name)
{
	if (!parent) return nullptr;
	if (parent->get_name() == name) return parent;
	for (int i = 0; i < parent->get_child_count(); ++i)
	{
		Node *child = parent->get_child(i);
		Node *result = FindNodeByName(child, name);
		if (result) return result;
	}
	return nullptr;
}

// HMTools Implementation
void HMTools::_bind_methods() {}
void HMTools::_notification(int p_notification)
{
	if (p_notification == Node::NOTIFICATION_ENTER_TREE)
	{
		// Register Menus
		add_tool_menu_item("Animation Repath Tool...", callable_mp(this, &HMTools::OnMenuItemClick).bind(String("Tools"), String("Animation-Repath")));
	}
	if (p_notification == Node::NOTIFICATION_EXIT_TREE)
	{
		// Register Menus
		remove_tool_menu_item("Animation Repath Tool...");
	}
}
void HMTools::OnMenuItemClick(const String &category, const String &menuID)
{
	// Open Animation Repath Tool
	if (category == "Tools" && menuID == "Animation-Repath")
	{
		OpenAnimationRepathTool(get_tree());
	}
}

// Animation Repath Tool Implementation
void EditSelectedAnimationResource(const String& path, EditorFileDialog* artWindow)
{
	// Load Animation Resource
	Ref<Animation> animRes = ResourceLoader::load(path);

	// Get Data
	auto injectionTextEdit = (TextEdit*)FindNodeByName(artWindow, "InjectionTextEdit");
	if (!injectionTextEdit)
	{
		ERR_PRINT("Error : Failed to Obtain Injection Data.");
		return;
	}
	int pathInjectionMode = artWindow->get_selected_options()["Path Injection Mode : "];
	int repathMode = artWindow->get_selected_options()["Repath Mode : "];
	String injectionText = injectionTextEdit->get_text();

	// Validate Data
	if (injectionText.is_empty()) return;

	// Validate Animation Resource
	if (animRes->is_class("Animation") && animRes.is_valid())
	{
		// Manipulate Tracks
		for (size_t i = 0; i < animRes->get_track_count(); i++)
		{
			String trackPath = animRes->track_get_path(i);
			if (repathMode == 1) trackPath = trackPath.get_file();
			if (pathInjectionMode == 0) trackPath = trackPath.insert(0, injectionText);
			if (pathInjectionMode == 1) trackPath += injectionText;
			animRes->track_set_path(i, trackPath);
		}

		// Save Resource
		if (ResourceSaver::save(animRes, path) != OK)
		{
			ERR_PRINT("Error : Unable to Save Changes.");
			return;
		};

		// Success
		print_line("Success : Animation Data Updated.");
	}
	else
	{
		ERR_PRINT("Error : Invalid Animation Resource.");
	}
}
void OpenAnimationRepathTool(SceneTree* scene_tree)
{
	// Get Scale Factor
	double scaleFactor = EditorInterface::get_singleton()->get_editor_scale();

	// Create Resource Picker
	EditorFileDialog* artWindow = memnew(EditorFileDialog);
	artWindow->set_name("AnimationRepathToolWindow");
	artWindow->set_size(Vector2i(1200 * scaleFactor, 550 * scaleFactor));
	artWindow->set_file_mode(EditorFileDialog::FILE_MODE_OPEN_FILE);
	artWindow->set_access(EditorFileDialog::ACCESS_RESOURCES);
	artWindow->add_filter("*.res", "Godot Animation Resource");
	artWindow->set_title("Select An Animation Resource");

	// Add Path Injection Mode Option
	auto pathInjectionMode = PackedStringArray();
	pathInjectionMode.push_back(" Add Prefix ");
	pathInjectionMode.push_back(" Add Postfix ");
	artWindow->add_option("Path Injection Mode : ", pathInjectionMode, 0);

	// Add Repath Mode Option
	auto repathMode = PackedStringArray();
	repathMode.push_back(" Keep Full Path ");
	repathMode.push_back(" Remove Full Path ");
	artWindow->add_option("Repath Mode : ", repathMode, 1);

	// Create Elements
	TextEdit* injectionTextEdit = memnew(TextEdit);
	injectionTextEdit->set_text("Skeleton3D:");
	injectionTextEdit->set_name("InjectionTextEdit");
	artWindow->add_side_menu(injectionTextEdit, "Path Injection Data");

	// Connect Signals
	artWindow->connect("file_selected", callable_mp_static(EditSelectedAnimationResource).bind(artWindow));

	// Display the dialog
	scene_tree->get_root()->add_child(artWindow);
	artWindow->popup_centered();
}
