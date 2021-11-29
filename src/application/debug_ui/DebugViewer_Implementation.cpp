#include "project_configuration.h"

#if USE_GUI()

#include "project_config.h"

#pragma region preprocessor

#define _CRT_SECURE_NO_WARNINGS

#include "imgui.h"
#include <cctype>         
#include <climits>         
#include <cmath>          
#include <cstdio>         
#include <string>
#include <filesystem>
#include <vector>

#include "Utilities.h"

#include "Application.h"
#include "Ranges.h"

#pragma warning (disable: 4996) // 'This function or variable may be unsafe': strcpy, strdup, sprintf, vsnprintf, sscanf, fopen

#pragma endregion

#pragma region declaration


// Forward Declarations
static void ShowExampleAppMainMenuBar();
static void ShowExampleAppConsole(bool* p_open);

namespace GUI_Impl
{
	void ShowGUI();
	void LogText(LogVerbosity category, std::string_view text);
	void SaveCurrnetUILayout();
	void LoadUILayout(std::string_view layoutName);
}

//
// ---------------
//

static bool show_app_main_menu_bar = true;
static bool show_app_console = true;

namespace gui_colors
{

	ImVec4 const red_tint(0.8f, 0.2f, 0.2f, 1.0f);

	ImVec4 const green_tint(0.2f, 0.8f, 0.2f, 0.5f);

	ImVec4 const blue_tint(0.2f, 0.2f, 0.8f, 0.5f);

	ImVec4 const yellow_tint(253 / 255.0f, 231 / 255.0f, 76 / 255.0f, 0.5f);

	ImVec4 const levander_tint(149 / 255.0f, 125 / 255.0f, 173 / 255.0f, 0.5f);

	ImVec4 const pink_tint(225 / 255.0f, 138 / 255.0f, 170 / 255.0f, 0.5f);

	ImVec4 const dark_tint(50 / 255.0f, 50 / 255.0f, 50 / 255.0f, 1.0f);

}

static void HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

static std::vector<std::string> GetLayoutConfigFileNames()
{
	return {};
	std::vector<std::string> result;

	using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

	for (const auto& dirEntry : recursive_directory_iterator(get_init_file(R"(ui_layout\)")))
	{

		if (dirEntry.path().extension().string() == ".ini")
		{
			result.emplace_back(dirEntry.path().stem().string());
		}
	}

	return result;
}

#pragma endregion

#pragma region console - logs 

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
////															 	  ////
////				     ShowExampleAppConsole					      ////
////																  ////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

struct LogRecord
{
	std::string text;
	LogVerbosity category;
	//time_t time;
};

struct ExampleAppConsole
{
	char                  InputBuf[256];
	std::vector<LogRecord> Items;
	ImVector<const char*> Commands;
	ImVector<char*>       History;
	int                   HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
	ImGuiTextFilter       Filter;
	bool                  AutoScroll;
	bool                  ScrollToBottom;

	ExampleAppConsole()
	{
		ClearLog();
		memset(InputBuf, 0, sizeof(InputBuf));
		HistoryPos = -1;
		Commands.push_back("HELP");
		Commands.push_back("HISTORY");
		Commands.push_back("CLEAR");
		Commands.push_back("CLASSIFY");  // "classify" is only here to provide an example of "C"+[tab] completing to "CL" and displaying matches.
		AutoScroll = true;
		ScrollToBottom = true;
	}
	~ExampleAppConsole()
	{
		ClearLog();
		for (int i = 0; i < History.Size; i++)
			free(History[i]);
	}

	// Portable helpers
	static int   Stricmp(const char* str1, const char* str2) { int d; while ((d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; } return d; }
	static int   Strnicmp(const char* str1, const char* str2, int n) { int d = 0; while (n > 0 && (d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; n--; } return d; }
	static char* Strdup(const char* str) { size_t len = strlen(str) + 1; void* buf = malloc(len); IM_ASSERT(buf); return (char*)memcpy(buf, (const void*)str, len); }
	static void  Strtrim(char* str) { char* str_end = str + strlen(str); while (str_end > str && str_end[-1] == ' ') str_end--; *str_end = 0; }

	void    ClearLog()
	{
		Items.clear();
		ScrollToBottom = true;
	}

	void    AddLog(LogVerbosity category, std::string_view text) IM_FMTARGS(2)
	{
		Items.push_back({ std::string{text},category });
		if (AutoScroll)
			ScrollToBottom = true;
	}

	void    Draw(const char* title, bool* p_open)
	{
		ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
		if (!ImGui::Begin(title, p_open))
		{
			ImGui::End();
			return;
		}

		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Close Console"))
				*p_open = false;
			ImGui::EndPopup();
		}

		//ImGui::TextWrapped("Enter 'HELP' for help, press TAB to use text completion.");


		if (ImGui::SmallButton("Clear")) { ClearLog(); } ImGui::SameLine();
		bool copy_to_clipboard = ImGui::SmallButton("Copy"); ImGui::SameLine();
		if (ImGui::SmallButton("Scroll to bottom")) ScrollToBottom = true;
		//static float t = 0.0f; if (ImGui::GetTime() - t > 0.02f) { t = ImGui::GetTime(); AddLog("Spam %f", t); }

		ImGui::Separator();

		// Options menu
		if (ImGui::BeginPopup("Options"))
		{
			if (ImGui::Checkbox("Auto-scroll", &AutoScroll))
				if (AutoScroll)
					ScrollToBottom = true;
			ImGui::EndPopup();
		}

		// Options, Filter
		if (ImGui::Button("Options"))
			ImGui::OpenPopup("Options");
		ImGui::SameLine();
		Filter.Draw(R"(Filter ("incl,-excl") ("error"))", 180);
		ImGui::Separator();

		const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing(); // 1 separator, 1 input text
		ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar); // Leave room for 1 separator + 1 InputText
		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::Selectable("Clear")) ClearLog();
			ImGui::EndPopup();
		}

		// Display every line as a separate entry so we can change their color or add custom widgets. If you only want raw text you can use ImGui::TextUnformatted(log.begin(), log.end());
		// NB- if you have thousands of entries this approach may be too inefficient and may require user-side clipping to only process visible items.
		// You can seek and display only the lines that are visible using the ImGuiListClipper helper, if your elements are evenly spaced and you have cheap random access to the elements.
		// To use the clipper we could replace the 'for (int i = 0; i < Items.Size; i++)' loop with:
		//     ImGuiListClipper clipper(Items.Size);
		//     while (clipper.Step())
		//         for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
		// However, note that you can not use this code as is if a filter is active because it breaks the 'cheap random-access' property. We would need random-access on the post-filtered list.
		// A typical application wanting coarse clipping and filtering may want to pre-compute an array of indices that passed the filtering test, recomputing this array when user changes the filter,
		// and appending newly elements as they are inserted. This is left as a task to the user until we can manage to improve this example code!
		// If your items are of variable size you may want to implement code similar to what ImGuiListClipper does. Or split your data into fixed height items to allow random-seeking into your list.
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
		if (copy_to_clipboard)
			ImGui::LogToClipboard();

		for (auto const& item : Items)
		{
			const char* text = item.text.c_str();

			if (!Filter.PassFilter(text))
				continue;

			bool pop_color = false;

			switch (item.category)
			{
			case LogVerbosity::Trace:
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.8f, 1.0f)); pop_color = true;
				break;
			case LogVerbosity::Info:
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.4f, 8.0f, 0.4f, 1.0f)); pop_color = true;
				break;
			case LogVerbosity::Warn:
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.0f, 1.0f)); pop_color = true;
				break;
			case LogVerbosity::Error:
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.4f, 0.4f, 1.0f)); pop_color = true;
				break;
			case LogVerbosity::Critical:
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f)); pop_color = true;
				break;
			case LogVerbosity::Application:
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.678f, 0.847f, 0.992f, 1.0f)); pop_color = true;
				break;
			default:
				break;

			}

			ImGui::TextUnformatted(text);
			if (pop_color)
				ImGui::PopStyleColor();
		}

		if (copy_to_clipboard)
			ImGui::LogFinish();
		if (ScrollToBottom)
			ImGui::SetScrollHereY(1.0f);
		ScrollToBottom = false;
		ImGui::PopStyleVar();
		ImGui::EndChild();
		ImGui::Separator();



		ImGui::End();
	}



	static int TextEditCallbackStub(ImGuiInputTextCallbackData* data) // In C++11 you are better off using lambdas for this sort of forwarding callbacks
	{
		ExampleAppConsole* console = (ExampleAppConsole*)data->UserData;
		return console->TextEditCallback(data);
	}

	int     TextEditCallback(ImGuiInputTextCallbackData* data)
	{
		//AddLog("cursor: %d, selection: %d-%d", data->CursorPos, data->SelectionStart, data->SelectionEnd);
		switch (data->EventFlag)
		{
		case ImGuiInputTextFlags_CallbackCompletion:
		{
			// Example of TEXT COMPLETION

			// Locate beginning of current word
			const char* word_end = data->Buf + data->CursorPos;
			const char* word_start = word_end;
			while (word_start > data->Buf)
			{
				const char c = word_start[-1];
				if (c == ' ' || c == '\t' || c == ',' || c == ';')
					break;
				word_start--;
			}

			// Build a list of candidates
			ImVector<const char*> candidates;
			for (int i = 0; i < Commands.Size; i++)
				if (Strnicmp(Commands[i], word_start, (int)(word_end - word_start)) == 0)
					candidates.push_back(Commands[i]);

			if (candidates.Size == 0)
			{
				// No match
				//AddLog("No match for \"%.*s\"!\n", (int)(word_end-word_start), word_start);
			}
			else if (candidates.Size == 1)
			{
				// Single match. Delete the beginning of the word and replace it entirely so we've got nice casing
				data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
				data->InsertChars(data->CursorPos, candidates[0]);
				data->InsertChars(data->CursorPos, " ");
			}
			else
			{
				// Multiple matches. Complete as much as we can, so inputing "C" will complete to "CL" and display "CLEAR" and "CLASSIFY"
				int match_len = (int)(word_end - word_start);
				for (;;)
				{
					int c = 0;
					bool all_candidates_matches = true;
					for (int i = 0; i < candidates.Size && all_candidates_matches; i++)
						if (i == 0)
							c = toupper(candidates[i][match_len]);
						else if (c == 0 || c != toupper(candidates[i][match_len]))
							all_candidates_matches = false;
					if (!all_candidates_matches)
						break;
					match_len++;
				}

				if (match_len > 0)
				{
					data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
					data->InsertChars(data->CursorPos, candidates[0], candidates[0] + match_len);
				}

				// List matches
				/*AddLog("Possible matches:\n");
				for (int i = 0; i < candidates.Size; i++)
					AddLog("- %s\n", candidates[i]);*/
			}

			break;
		}
		case ImGuiInputTextFlags_CallbackHistory:
		{
			// Example of HISTORY
			const int prev_history_pos = HistoryPos;
			if (data->EventKey == ImGuiKey_UpArrow)
			{
				if (HistoryPos == -1)
					HistoryPos = History.Size - 1;
				else if (HistoryPos > 0)
					HistoryPos--;
			}
			else if (data->EventKey == ImGuiKey_DownArrow)
			{
				if (HistoryPos != -1)
					if (++HistoryPos >= History.Size)
						HistoryPos = -1;
			}

			// A better implementation would preserve the data on the current input line along with cursor position.
			if (prev_history_pos != HistoryPos)
			{
				const char* history_str = (HistoryPos >= 0) ? History[HistoryPos] : "";
				data->DeleteChars(0, data->BufTextLen);
				data->InsertChars(0, history_str);
			}
		}
		}
		return 0;
	}
};

static ExampleAppConsole examplpe_console;

static void ShowExampleAppConsole(bool* p_open)
{
	examplpe_console.Draw("Console: ", p_open);
}


#pragma endregion

#pragma region gui elements

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
////															 	  ////
////				     GUI ELEMENTS        					      ////
////																  ////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#define  SET_BUTTON_COLOR(color) \
ImGui::PushStyleColor(ImGuiCol_Button, color) 

#define  SET_BUTTON_COLOR_ACTIONS(color) \
float hsv[3] = { 0,0,0 }; \
ImGui::ColorConvertRGBtoHSV(color.x, color.y, color.z,hsv[0], hsv[1], hsv[2]); \
\
ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(hsv[0], hsv[1], hsv[2])); \
ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(hsv[0], hsv[1] + 0.1f, hsv[2] + 0.1f)); \
ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(hsv[0], hsv[1] + 0.2f, hsv[2] + 0.2f)) 

#define POP_BUTTON_COLOR() ImGui::PopStyleColor(1)
#define POP_BUTTON_COLOR_ACTIONS() ImGui::PopStyleColor(3)


#define STR_VALUE(text)  #text

#define DEFINE_PLOT(name, label , func ,display_count, scale_min,scale_max ) \
static float scale_min_##name {scale_min}; \
static float scale_max_##name {scale_max}; \
ImGui::PlotLines(label, func, NULL, display_count, 0, NULL,scale_min_##name , scale_max_##name, ImVec2(0, 100)); \
ImGui::BeginGroup(); \
ImGui::SliderFloat( STR_VALUE(range_min_##name) , &scale_min_##name, scale_min, scale_max); \
ImGui::SliderFloat( STR_VALUE(range_max_##name),  &scale_max_##name, scale_min, scale_max); \
ImGui::EndGroup()

#define DEFINE_OBJECT_PLOT(name , label , func , object , display_count, scale_min,scale_max ) \
static float scale_min_##name {scale_min}; \
static float scale_max_##name {scale_max}; \
ImGui::PlotLines(label, func, object, display_count, 0, NULL,scale_min_##name , scale_max_##name, ImVec2(0, 100)); \
ImGui::BeginGroup(); \
ImGui::SliderFloat( STR_VALUE(range_min_##name) , &scale_min_##name, scale_min, scale_max); \
ImGui::SliderFloat( STR_VALUE(range_max_##name),  &scale_max_##name, scale_min, scale_max); \
ImGui::EndGroup()


#define DEFINE_SYMMETRIC_OBJECT_PLOT(name , label , func , object , display_count, range ) \
static float range_##name {range}; \
ImGui::PlotLines(label, func, object, display_count, 0, NULL,-range_##name , range_##name, ImVec2(0, 100)); \
ImGui::BeginGroup(); \
ImGui::SliderFloat( STR_VALUE(range_##name),  &range_##name, 0, range); \
ImGui::EndGroup()

#define SEPARATOR_WITH_PADDING() 		\
ImGui::NewLine(); \
ImGui::Separator(); \
ImGui::NewLine()

#define TEXT_PARAGRAPH(label) 		\
ImGui::Separator(); \
ImGui::Text(label)


#define DEFINE_SLIDER_WITH_2_Values(slider_name, slider_label, value_a, value_b) \
enum Element_##slider_name { value_a, value_b, Element_COUNT_##slider_name }; \
const char* element_names_##slider_name[Element_COUNT_##slider_name] = { #value_a, #value_b }; \
static int current_element_##slider_name = value_a; \
const char* current_element_name_##slider_name = (current_element_##slider_name >= 0 && current_element_##slider_name < Element_COUNT_##slider_name) ? element_names_##slider_name[current_element_##slider_name] : "Unknown"; \
ImGui::SliderInt(slider_label, &current_element_##slider_name, 0, Element_COUNT_##slider_name - 1, current_element_name_##slider_name);

#define DEFINE_SLIDER_WITH_4_Values(slider_name, slider_label, value_a, value_b,value_c,value_d) \
enum Element_##slider_name { value_a, value_b, value_c, value_d, Element_COUNT_##slider_name }; \
const char* element_names_##slider_name[Element_COUNT_##slider_name] = { #value_a, #value_b, #value_c, #value_d }; \
static int current_element_##slider_name = value_a; \
const char* current_element_name_##slider_name = (current_element_##slider_name >= 0 && current_element_##slider_name < Element_COUNT_##slider_name) ? element_names_##slider_name[current_element_##slider_name] : "Unknown"; \
ImGui::SliderInt(slider_label, &current_element_##slider_name, 0, Element_COUNT_##slider_name - 1, current_element_name_##slider_name);

#define DEFINE_COMBO_BOX(name, label, items) \
static int input_value_##name = 0; \
ImGui::Combo(label, &input_value_##name, items, IM_ARRAYSIZE(items))

#define DEFINE_COMBO_BOX_REF(label, items,reference_name) \
ImGui::Combo(label, &reference_name, items, IM_ARRAYSIZE(items))

#define DEFINE_INPUT_TEXT(name, label , max_text_size) \
static char input_value_##name[max_text_size] = ""; \
ImGui::InputText(label, input_value_##name, max_text_size)

#define DEFINE_INPUT_TEXT_WITH_VALUE(name, label , max_text_size, text) \
static char input_value_##name[max_text_size] = text; \
ImGui::InputText(label, input_value_##name, max_text_size)

#define DEFINE_INPUT_MULTILINE_TEXT(name, label , max_text_size) \
static char input_value_##name[max_text_size] = ""; \
ImGui::InputTextMultiline(label, input_value_##name, max_text_size)

#define DEFINE_INPUT_INT(name, label , min_speed, max_speed,def_value) \
static int input_value_##name {def_value}; \
ImGui::InputInt(label, &input_value_##name, min_speed, max_speed)

#define DEFINE_INPUT_INT_REF(name, label , min_speed, max_speed,reference_name) \
ImGui::InputInt(label, &reference_name, min_speed, max_speed)

#define DEFINE_INPUT_FLOAT(name, label , min_speed, max_speed) \
static float input_value_##name {}; \
ImGui::InputFloat(label, &input_value_##name, min_speed, max_speed)

#define DEFINE_INPUT_FLOAT_REF(name, label , min_speed, max_speed,reference_name) \
ImGui::InputFloat(label, &reference_name, min_speed, max_speed)

#define DEFINE_INPUT_FLOAT_WITH_VALUE(name, label , min_speed, max_speed,default_value) \
static float input_value_##name {default_value}; \
ImGui::InputFloat(label, &input_value_##name, min_speed, max_speed)

#define DEFINE_INPUT_DOUBLE(name, label , min_speed, max_speed,def_value) \
static double input_value_##name {def_value}; \
ImGui::InputDouble(label, &input_value_##name, min_speed, max_speed)

#define DEFINE_INPUT_DOUBLE_REF(name, label , min_speed, max_speed,reference_name) \
static double& input_value_##name = reference_name; \
ImGui::InputDouble(label, &input_value_##name, min_speed, max_speed)


#define DEFINE_CHECKBOX(name,label,def_value) \
static bool input_value_##name {def_value}; \
ImGui::Checkbox(label, &input_value_##name)

#define DEFINE_CHECKBOX_REF(label,reference_name) \
ImGui::Checkbox(label, &reference_name)

#define DEFINE_DRAG_FLOAT(name, label , drag_speed, min_val, max_val,format) \
static float input_value_##name {}; \
ImGui::DragFloat(label, &input_value_##name, drag_speed, min_val,max_val,format);

#define DEFINE_DRAG_FLOAT_WITH_VALUE(name, label , drag_speed, min_val, max_val,default_value,format) \
static float input_value_##name {default_value}; \
ImGui::DragFloat(label, &input_value_##name, drag_speed, min_val,max_val,format);

#define DEFINE_SLIDER_FLOAT(name, label , min_val, max_val,format) \
static float input_value_##name {}; \
ImGui::SliderFloat(label, &input_value_##name,  min_val, max_val, format)

#define DEFINE_SLIDER_INT(name, label , min_val, max_val,format) \
static int input_value_##name {}; \
ImGui::SliderInt(label, &input_value_##name,  min_val, max_val, format)

#define DEFINE_SLIDER_INT_WITH_VALUE(name, label , min_val, max_val,format,value) \
static int input_value_##name {value}; \
ImGui::SliderInt(label, &input_value_##name,  min_val, max_val, format)

#define DEFINE_SLIDER_FLOAT_REF(label , min_val, max_val,format,reference_name) \
ImGui::SliderFloat(label, &reference_name,  min_val, max_val, format)

#define DEFINE_SLIDER_INT_REF(label , min_val, max_val,format,reference_name) \
ImGui::SliderInt(label, &reference_name,  min_val, max_val, format)

#define GET_INPUT_VALUE(name) \
input_value_##name 

#define GET_SLIDER_VALUE(slider_name) \
current_element_##slider_name 

enum SliderOnOffEnum { SliderOff, SliderOn, SliderOnOffCount };
const char* SliderOnOffEnumNames[SliderOnOffCount] = { "off", "on" };

#define DEFINE_SLIDER_ON_OFF(slider_label) \
static int slider_selection_on_off = SliderOff; \
ImGui::SliderInt(slider_label, &slider_selection_on_off, 0, SliderOnOffCount - 1, SliderOnOffEnumNames[slider_selection_on_off])

#define IS_SLIDER_ON() \
(slider_selection_on_off == SliderOn)

#pragma endregion

#pragma region menu bar


static const std::vector<std::string>  ui_layouts = GetLayoutConfigFileNames();
static std::string current_layout;

static void menu_bar_windows()
{
	if (ImGui::BeginMenu("Window"))
	{
		ImGui::MenuItem("Log", NULL, &show_app_console);
		ImGui::EndMenu();
	}
}

static void menu_bar_layouts()
{
	auto is_current_layout = [&](auto const& layout_name) { return layout_name == current_layout; };
	bool need_to_be_popped = false;

	if (ImGui::BeginMenu("Layout##menubar"))
	{
		ImGui::Text(current_layout.c_str());

		if (ImGui::Button("Save current##ui_layouts", { 150,30 }))
		{
			GUI_Impl::SaveCurrnetUILayout();
		}

		ImGui::Separator();
		ImGui::NewLine();

		for (auto const& layout : ui_layouts)
		{
			if (layout == current_layout)
			{
				ImGui::PushStyleColor(ImGuiCol_Button, gui_colors::green_tint);
				need_to_be_popped = true;
			}

			if (ImGui::Button(layout.c_str(), { 150,30 }))
			{
				GUI_Impl::LoadUILayout(layout);
			}

			if (need_to_be_popped)
			{
				ImGui::PopStyleColor(1);
				need_to_be_popped = false;
			}
		}

		ImGui::EndMenu();
	}

	for (auto const& layout : ui_layouts)
	{
		if (layout == current_layout)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, gui_colors::green_tint);
			need_to_be_popped = true;
		}

		if (ImGui::Button(layout.c_str(), { 150,25 }))
		{
			GUI_Impl::LoadUILayout(layout);
		}

		if (need_to_be_popped)
		{
			ImGui::PopStyleColor(1);
			need_to_be_popped = false;
		}
	}
}

static void ShowExampleAppMainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		ImGui::Separator();

		menu_bar_windows();

		ImGui::Separator();

		menu_bar_layouts();

		ImGui::Separator();

		ImGui::EndMainMenuBar();
	}
}


#pragma endregion

#pragma region api for viewer

namespace GUI_Impl
{
	void LogText(LogVerbosity category, std::string_view text)
	{
		examplpe_console.AddLog(category, text);
	}

	void SaveCurrnetUILayout()
	{
		if (ranges::find(ui_layouts, current_layout))
		{
			auto layout_file_path = get_init_file(R"(devel\ui_layout\)" + current_layout + ".ini");
			ImGui::SaveIniSettingsToDisk(layout_file_path.c_str());
		}
		else
		{
			ERROR_LOG("{} : not exists .ini layout", current_layout);
		}
	}

	void LoadUILayout(std::string_view layoutName)
	{
		/*	auto layout_file_path = get_init_file(fmt::format("devel\\ui_layout\\{}.ini", layoutName));
			ImGui::LoadIniSettingsFromDisk(layout_file_path.data());
			current_layout = layoutName;*/
	}

	void ShowGUI()
	{
		IM_ASSERT(ImGui::GetCurrentContext() != nullptr && "Missing dear imgui context. Refer to examples app!"); // Exceptionally add an extra assert here for people confused with initial dear imgui setup
		if (show_app_main_menu_bar)       ShowExampleAppMainMenuBar();
		if (show_app_console)             ShowExampleAppConsole(&show_app_console);
	}
}

#pragma endregion

#endif

