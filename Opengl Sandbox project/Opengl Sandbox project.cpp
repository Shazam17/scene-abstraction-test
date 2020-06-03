#include "Application.h"
#include "Model.h"
#include "OrbitCamera.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "BoundingBox.h"
#include "Mesh.h"
#include "Input.h"
#include "SciptTest.h"


void ShowExampleAppDockSpace(bool* p_open)
{
	static bool opt_fullscreen_persistant = true;
	bool opt_fullscreen = opt_fullscreen_persistant;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
    

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->GetWorkPos());
		ImGui::SetNextWindowSize(viewport->GetWorkSize());
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
	// and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", p_open, window_flags);
	ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}
	else
	{
	
	}

	//if (ImGui::BeginMenuBar())
	//{
	//	if (ImGui::BeginMenu("Docking"))
	//	{
	//		// Disabling fullscreen would allow the window to be moved to the front of other windows,
	//		// which we can't undo at the moment without finer window depth/z control.
	//		//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

	//		if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0))                 dockspace_flags ^= ImGuiDockNodeFlags_NoSplit;
	//		if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0))                dockspace_flags ^= ImGuiDockNodeFlags_NoResize;
	//		if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0))  dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode;
	//		if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0))     dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode;
	//		if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))          dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar;
	//		ImGui::Separator();
	//		if (ImGui::MenuItem("Close DockSpace", NULL, false, p_open != NULL))
	//			*p_open = false;
	//		ImGui::EndMenu();
	//	}
	//	

	//	ImGui::EndMenuBar();
	//}

	ImGui::End();
}

struct ExampleAppConsole
{
    char                  InputBuf[256];
    ImVector<char*>       Items;
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

        // "CLASSIFY" is here to provide the test case where "C"+[tab] completes to "CL" and display multiple matches.
        Commands.push_back("HELP");
        Commands.push_back("HISTORY");
        Commands.push_back("CLEAR");
        Commands.push_back("CLASSIFY");
        AutoScroll = true;
        ScrollToBottom = false;
        AddLog("Welcome to Dear ImGui!");
    }
    ~ExampleAppConsole()
    {
        ClearLog();
        for (int i = 0; i < History.Size; i++)
            free(History[i]);
    }

    // Portable helpers
    static int   Stricmp(const char* s1, const char* s2) { int d; while ((d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; } return d; }
    static int   Strnicmp(const char* s1, const char* s2, int n) { int d = 0; while (n > 0 && (d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; n--; } return d; }
    static char* Strdup(const char* s) { size_t len = strlen(s) + 1; void* buf = malloc(len); IM_ASSERT(buf); return (char*)memcpy(buf, (const void*)s, len); }
    static void  Strtrim(char* s) { char* str_end = s + strlen(s); while (str_end > s && str_end[-1] == ' ') str_end--; *str_end = 0; }

    void    ClearLog()
    {
        for (int i = 0; i < Items.Size; i++)
            free(Items[i]);
        Items.clear();
    }

    void    AddLog(const char* fmt, ...) IM_FMTARGS(2)
    {
        // FIXME-OPT
        char buf[1024];
        va_list args;
        va_start(args, fmt);
        vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
        buf[IM_ARRAYSIZE(buf) - 1] = 0;
        va_end(args);
        Items.push_back(Strdup(buf));
    }

    void    Draw(const char* title, bool* p_open)
    {
        ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
        if (!ImGui::Begin(title, p_open))
        {
            ImGui::End();
            return;
        }

        // As a specific feature guaranteed by the library, after calling Begin() the last Item represent the title bar.
        // So e.g. IsItemHovered() will return true when hovering the title bar.
        // Here we create a context menu only available from the title bar.
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Close Console"))
                *p_open = false;
            ImGui::EndPopup();
        }

        ImGui::TextWrapped(
            "This example implements a console with basic coloring, completion and history. A more elaborate "
            "implementation may want to store entries along with extra data such as timestamp, emitter, etc.");
        ImGui::TextWrapped("Enter 'HELP' for help, press TAB to use text completion.");

        // TODO: display items starting from the bottom

        if (ImGui::SmallButton("Add Dummy Text")) { AddLog("%d some text", Items.Size); AddLog("some more text"); AddLog("display very important message here!"); } ImGui::SameLine();
        if (ImGui::SmallButton("Add Dummy Error")) { AddLog("[error] something went wrong"); } ImGui::SameLine();
        if (ImGui::SmallButton("Clear")) { ClearLog(); } ImGui::SameLine();
        bool copy_to_clipboard = ImGui::SmallButton("Copy");
        //static float t = 0.0f; if (ImGui::GetTime() - t > 0.02f) { t = ImGui::GetTime(); AddLog("Spam %f", t); }

        ImGui::Separator();

        // Options menu
        if (ImGui::BeginPopup("Options"))
        {
            ImGui::Checkbox("Auto-scroll", &AutoScroll);
            ImGui::EndPopup();
        }

        // Options, Filter
        if (ImGui::Button("Options"))
            ImGui::OpenPopup("Options");
        ImGui::SameLine();
        Filter.Draw("Filter (\"incl,-excl\") (\"error\")", 180);
        ImGui::Separator();

        // Reserve enough left-over height for 1 separator + 1 input text
        const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
        ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);
        if (ImGui::BeginPopupContextWindow())
        {
            if (ImGui::Selectable("Clear")) ClearLog();
            ImGui::EndPopup();
        }

        // Display every line as a separate entry so we can change their color or add custom widgets.
        // If you only want raw text you can use ImGui::TextUnformatted(log.begin(), log.end());
        // NB- if you have thousands of entries this approach may be too inefficient and may require user-side clipping
        // to only process visible items. The clipper will automatically measure the height of your first item and then
        // "seek" to display only items in the visible area.
        // To use the clipper we can replace your standard loop:
        //      for (int i = 0; i < Items.Size; i++)
        //   With:
        //      ImGuiListClipper clipper(Items.Size);
        //      while (clipper.Step())
        //         for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
        // - That your items are evenly spaced (same height)
        // - That you have cheap random access to your elements (you can access them given their index,
        //   without processing all the ones before)
        // You cannot this code as-is if a filter is active because it breaks the 'cheap random-access' property.
        // We would need random-access on the post-filtered list.
        // A typical application wanting coarse clipping and filtering may want to pre-compute an array of indices
        // or offsets of items that passed the filtering test, recomputing this array when user changes the filter,
        // and appending newly elements as they are inserted. This is left as a task to the user until we can manage
        // to improve this example code!
        // If your items are of variable height:
        // - Split them into same height items would be simpler and facilitate random-seeking into your list.
        // - Consider using manual call to IsRectVisible() and skipping extraneous decoration from your items.
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
        if (copy_to_clipboard)
            ImGui::LogToClipboard();
        for (int i = 0; i < Items.Size; i++)
        {
            const char* item = Items[i];
            if (!Filter.PassFilter(item))
                continue;

            // Normally you would store more information in your item than just a string.
            // (e.g. make Items[] an array of structure, store color/type etc.)
            ImVec4 color;
            bool has_color = false;
            if (strstr(item, "[error]")) { color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f); has_color = true; }
            else if (strncmp(item, "# ", 2) == 0) { color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f); has_color = true; }
            if (has_color)
                ImGui::PushStyleColor(ImGuiCol_Text, color);
            ImGui::TextUnformatted(item);
            if (has_color)
                ImGui::PopStyleColor();
        }
        if (copy_to_clipboard)
            ImGui::LogFinish();

        if (ScrollToBottom || (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
            ImGui::SetScrollHereY(1.0f);
        ScrollToBottom = false;

        ImGui::PopStyleVar();
        ImGui::EndChild();
        ImGui::Separator();

        // Command-line
        bool reclaim_focus = false;
        ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
        if (ImGui::InputText("Input", InputBuf, IM_ARRAYSIZE(InputBuf), input_text_flags, &TextEditCallbackStub, (void*)this))
        {
            char* s = InputBuf;
            Strtrim(s);
            if (s[0])
                ExecCommand(s);
            strcpy_s(s,sizeof(s), "");
            reclaim_focus = true;
        }

        // Auto-focus on window apparition
        ImGui::SetItemDefaultFocus();
        if (reclaim_focus)
            ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

        ImGui::End();
    }

    void    ExecCommand(const char* command_line)
    {
        AddLog("# %s\n", command_line);

        // Insert into history. First find match and delete it so it can be pushed to the back.
        // This isn't trying to be smart or optimal.
        HistoryPos = -1;
        for (int i = History.Size - 1; i >= 0; i--)
            if (Stricmp(History[i], command_line) == 0)
            {
                free(History[i]);
                History.erase(History.begin() + i);
                break;
            }
        History.push_back(Strdup(command_line));

        // Process command
        if (Stricmp(command_line, "CLEAR") == 0)
        {
            ClearLog();
        }
        else if (Stricmp(command_line, "HELP") == 0)
        {
            AddLog("Commands:");
            for (int i = 0; i < Commands.Size; i++)
                AddLog("- %s", Commands[i]);
        }
        else if (Stricmp(command_line, "HISTORY") == 0)
        {
            int first = History.Size - 10;
            for (int i = first > 0 ? first : 0; i < History.Size; i++)
                AddLog("%3d: %s\n", i, History[i]);
        }
        else
        {
            AddLog("Unknown command: '%s'\n", command_line);
        }

        // On command input, we scroll to bottom even if AutoScroll==false
        ScrollToBottom = true;
    }

    // In C++11 you'd be better off using lambdas for this sort of forwarding callbacks
    static int TextEditCallbackStub(ImGuiInputTextCallbackData* data)
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
                AddLog("No match for \"%.*s\"!\n", (int)(word_end - word_start), word_start);
            }
            else if (candidates.Size == 1)
            {
                // Single match. Delete the beginning of the word and replace it entirely so we've got nice casing.
                data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
                data->InsertChars(data->CursorPos, candidates[0]);
                data->InsertChars(data->CursorPos, " ");
            }
            else
            {
                // Multiple matches. Complete as much as we can..
                // So inputing "C"+Tab will complete to "CL" then display "CLEAR" and "CLASSIFY" as matches.
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
                AddLog("Possible matches:\n");
                for (int i = 0; i < candidates.Size; i++)
                    AddLog("- %s\n", candidates[i]);
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

void showOneWindow(string name) {
	if (!ImGui::Begin(name.c_str(), NULL, NULL))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();

	}
	else {
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Menu"))
			{
                if (ImGui::MenuItem("item", "shortcut")) {

                }
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
		ImGui::End();
	}

}

void printTranform(Transform& tr) {
    if (!ImGui::Begin("Tranform", NULL, NULL))
    {
        // Early out if the window is collapsed, as an optimization.
        ImGui::End();

    }
    else {
        ImGui::Text("NAME");

        ImGui::Text("Position");
        ImGui::DragFloat3("T",&tr.getPos()[0],0.1f);
       
        

        ImGui::Text("Scale");
        ImGui::DragFloat3("S", &tr.getScale()[0], 0.1f);
       

        ImGui::Text("Rotation");
        ImGui::DragFloat3("R", &tr.getRot()[0], 0.1f);
        
        tr.recalculateMat();

        ImGui::Text("Components");
        

        ImGui::End();
    }
}

class Ray
{
public:
    Ray(const vec3& orig, const vec3& dir) : orig(orig), dir(dir)
    {
        invdir = 1.0f / dir ;
        sign[0] = (invdir.x < 0);
        sign[1] = (invdir.y < 0);
        sign[2] = (invdir.z < 0);
    }
    vec3 orig, dir;       // ray orig and dir 
    vec3 invdir;
    int sign[3];
};

bool intersect(const Ray& r,BoundingBox& box) 
{
    float tmin, tmax, tymin, tymax, tzmin, tzmax;

    tmin = (box.localBounds[r.sign[0]].x - r.orig.x) * r.invdir.x;
    tmax = (box.localBounds[1 - r.sign[0]].x - r.orig.x) * r.invdir.x;
    tymin = (box.localBounds[r.sign[1]].y - r.orig.y) * r.invdir.y;
    tymax = (box.localBounds[1 - r.sign[1]].y - r.orig.y) * r.invdir.y;

    if ((tmin > tymax) || (tymin > tmax))
        return false;
    if (tymin > tmin)
        tmin = tymin;
    if (tymax < tmax)
        tmax = tymax;

    tzmin = (box.localBounds[r.sign[2]].z - r.orig.z) * r.invdir.z;
    tzmax = (box.localBounds[1 - r.sign[2]].z - r.orig.z) * r.invdir.z;

    if ((tmin > tzmax) || (tzmin > tmax))
        return false;
    if (tzmin > tmin)
        tmin = tzmin;
    if (tzmax < tmax)
        tmax = tzmax;

    return true;
}
bool intersection(const BoundingBox& b,const  Ray r) {
    float tx1 = (b.globalCoords[0].x - r.orig.x) * r.invdir.x;
    float tx2 = (b.globalCoords[1].x - r.orig.x) * r.invdir.x;

    float tmin = min(tx1, tx2);
    float tmax = max(tx1, tx2);
    
    float ty1 = (b.globalCoords[0].y - r.orig.y) * r.invdir.y;
    float ty2 = (b.globalCoords[1].y - r.orig.y) * r.invdir.y;

    tmin = max(tmin, min(ty1, ty2));
    tmax = min(tmax, max(ty1, ty2));

    float tz1 = (b.globalCoords[0].z - r.orig.z) * r.invdir.z;
    float tz2 = (b.globalCoords[1].z - r.orig.z) * r.invdir.z;

    tmin = max(tmin, min(tz1, tz2));
    tmax = min(tmax, max(tz1, tz2));

    return tmax >= tmin;
}

struct Object {
    
    map<string, Component*> components;
    friend class Component;


    Object(const std::string& name) {
        AddComponent<Transform>(new Transform(vec3(0), vec3(1), vec3(0, 0, 0)));
        AddComponent<MeshRender>(new MeshRender(name));
    }

    Object() {

    }

    template<typename T>
    void AddComponent(const std::pair<string,Component*> &pair) {
        pair.second->components = &components;
        components.insert(pair);
    }

    template<typename T>
    void AddComponent(Component* cmp) {
        cmp->components = &components;
        components.insert({ typeid(T).name(),cmp });
    }

    template<typename T, typename std::enable_if<std::is_base_of<Component, T>::value>::type* = nullptr>
    T* GetComponent() const {
        try {
            auto res = components.find(typeid(T).name())->second;
            return (T*)(res);
        }
        catch (exception& ex) {
            std::cout << ex.what() << std::endl;
            return nullptr;
        }

    }

    void Update() {
        for (auto cmp : components) {
            cmp.second->onUpdate();
        }
    }

    void Start() {
        for (auto cmp : components) {
            cmp.second->onStart();
        }
    }
};
void printComponents(const Object& obj) {
   
        if (!ImGui::Begin("Components", NULL, NULL))
        {
            // Early out if the window is collapsed, as an optimization.
            ImGui::End();

        }
        else {
            for (auto cmp : obj.components) {
                ImGui::Text("NAME");
                ImGui::Text(cmp.first.c_str());
            }
        }
        ImGui::End();
}

void addModelWindow(string name, std::vector<Object*>& objs) {
    static bool addWindowOpened = false;
    static string modelName = "";
   
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("Add"))
            {
                if (ImGui::MenuItem("add model", "shortcut")) {
                    addWindowOpened = true;
                }
                if (ImGui::MenuItem("add tor", "shortcut")) {
                    
                    Object* pObj = new Object("tor.obj");
                    pObj->Start();
                    objs.push_back(pObj);
                    
                }
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }

        
    if (addWindowOpened) {
        if (!ImGui::Begin("model loading", &addWindowOpened, NULL))
        {
            ImGui::End();
        }
        else {
            static char buff[32] = "";
            ImGui::InputText("model name", &buff[0], IM_ARRAYSIZE(buff));
            if (ImGui::Button("load model")) {
                    addWindowOpened = false;
                    modelName = buff;
                    Object* pObj = new Object(modelName);
                    pObj->Start();
                    objs.push_back(pObj);
                
            }
            ImGui::End();
        }
    }
}

vec2 mouseToScreenCoord(float x, float y ,const Application& app) {
    float X = (app.mouseX * 2.0f) / app.getWidth() - 1.0f;
    float Y = 1.0f - (app.mouseY * 2.0f) / app.getHeight();
    return vec2(X, Y);
}

namespace Physics {

    

    void rayCast() {

    }

}

int main(void)
{
	Application app(1600, 900, "model interaction");
	OrbitCamera camera; 
    ExampleAppConsole console;
    Input::getInstance()->InitInput(&app);
	auto* resourceManager = ResourceManager::GetInstance();
    
	Shader shader = resourceManager->LoadShader("model.shader");
	Shader bbShader = resourceManager->LoadShader("whiteBasicShader.shader");
    Shader gizmoShader = resourceManager->LoadShader("gizmoShader.shader");

    bool show_demo_window = true;
    bool p_open = true;


  

    std::vector<Object *> objects;
    objects.push_back(new Object("tor.obj"));

  

 
	shader.setUniform4m("u_proj", camera.getProjectionMat(app));
    bbShader.setUniform4m("u_proj", camera.getProjectionMat(app));
    gizmoShader.setUniform4m("u_proj", camera.getProjectionMat(app));

	mat4 reversedProjetion = inverse(camera.getProjectionMat(app));
	mat4 reversedView = inverse(camera.getViewMat(app));	

    Object* selected = nullptr;

	app.setMouseClickListener([&reversedProjetion, &reversedView,&camera,&objects,&selected]
        (const Application& app ,Application::MOUSE_KEY_STATE key ,Application::KEY_PRESS_STATE state) {

                static vec2 last;
                if (Input::GetMouseDown(Application::MOUSE_KEY_STATE::LEFT)) {
                    last = vec2(app.mouseX, app.mouseY);
                }
                
                if (Input::GetMouseUp(Application::MOUSE_KEY_STATE::LEFT)
                    && !ImGui::GetIO().WantCaptureMouse 
                    && !ImGui::GetIO().WantCaptureKeyboard) {

                    vec2 cur(app.mouseX, app.mouseY);
                    vec2 diff = last - cur;
                    float magn = sqrt(diff.x * diff.x + diff.y * diff.y);

                    if (magn > 4) {
                        return;
                    }
			        std::cout << "clicked left key" << std::endl;
			        std::cout << "click coord: " << app.mouseX << ' ' << app.mouseY << std::endl;
			        std::cout << "RAYCASTING" << std::endl;


                    //RAYCAST BEGIN
			        float xNorm = (app.mouseX * 2.0f) / app.getWidth() - 1.0f;
			        float yNorm = 1.0f - (app.mouseY * 2.0f) / app.getHeight();
			        float zNorm = -1.0f;
         
			        vec4 rayEye = reversedProjetion * vec4(xNorm, yNorm, zNorm, 1.0f);
			        rayEye = vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);
			        vec4 point =  reversedView * rayEye;
			        point = normalize(point);
                    //RAYCAST END
			        std::cout << "casted: " << point.x << '|' << point.y << '|' << point.z << std::endl;

                    Ray ray(camera.getPos(), vec3(point));
              
                    bool wasSelected = false;
                    for (auto& obj : objects) {
                        auto& bBox = obj->GetComponent<MeshRender>()->getBoundingBox();
                        auto& mesh = obj->GetComponent<MeshRender>()->getMesh();
                        std::cout << "global bound min: " << bBox.globalCoords[0].x << std::endl;
                        obj->Update();
                        bool ret = intersection(bBox, ray);
                        if (ret && !wasSelected) {
                            mesh.setSelected(true);
                            selected = obj;
                            std::cout << "ray in" << std::endl;
                            wasSelected = true;
                        }
                        else {
                            mesh.setSelected(false);
                            std::cout << "ray out" << std::endl;
                        }
                    }
                    if (!wasSelected) {
                        selected = nullptr;
                    }   
		        }
	});

    for (auto& obj : objects) {
        obj->Start();
    }

	while (app.isRunning()) {
		app.clear();
				
		ShowExampleAppDockSpace(&p_open);
	
        addModelWindow("model bar", objects);

        if (selected != nullptr) {
            printTranform(*selected->GetComponent<Transform>());
            printComponents(*selected);
        }
        
		
		if (!ImGui::GetIO().WantCaptureMouse && !ImGui::GetIO().WantCaptureKeyboard) {
			camera.processMouseKeys(app);
			camera.processMouseMove(app);
		}

		shader.setUniform4m("u_view", camera.getViewMat(app));
        bbShader.setUniform4m("u_view", camera.getViewMat(app));
        gizmoShader.setUniform4m("u_view", camera.getViewMat(app));
		reversedView = inverse(camera.getViewMat(app));

        for (auto& obj : objects) {
            obj->Update();     
        }

        console.Draw("Example: Console", &p_open);
		app.swapBuffers();
	}

    for (auto& obj : objects) {
        delete obj;
    }

	resourceManager->terminate();
	return 0;
}