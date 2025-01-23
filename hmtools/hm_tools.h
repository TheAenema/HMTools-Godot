
#ifndef HM_TOOLS_H
#define HM_TOOLS_H

#include "core/object/ref_counted.h"
#include "editor/plugins/editor_plugin.h"

// HMTools Definition
class HMTools : public EditorPlugin
{
	GDCLASS(HMTools, EditorPlugin);

protected:
	static void _bind_methods();

public:
	void _notification(int p_notification);

public:
	void OnMenuItemClick(const String &category, const String &menuID);
};

#endif // HM_TOOLS_H
