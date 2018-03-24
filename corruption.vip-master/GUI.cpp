#include "GUI.h"
#include "Surface.h"
#include "menu.h"
#include "RenderManager.h"
#include "MiscClasses.h"
#include <algorithm>
#include "tinyxml2.h"
#include "Controls.h"

float MenuAlpha = 0.05f;
float MenuAlpha3 = 0.05f;
float MenuAlpha5 = 0.05f;
float MenuAlpha7 = 0.05f;
float MenuAlpha9 = 0.05f;
float MenuAlpha11 = 0.05f;
float MenuAlpha13 = 0.05f;
float MenuAlpha15 = 0.05f;
float MenuAlpha17 = 0.05f;
float MenuAlpha19 = 0.05f;
float MenuAlpha21 = 0.05f;
float MenuAlpha23 = 0.05f;
float Globals::MenuAlpha24 = MenuAlpha23;
float Globals::MenuAlpha22 = MenuAlpha21;
float Globals::MenuAlpha20 = MenuAlpha19;
float Globals::MenuAlpha18 = MenuAlpha17;
float Globals::MenuAlpha16 = MenuAlpha15;
float Globals::MenuAlpha14 = MenuAlpha13;
float Globals::MenuAlpha12 = MenuAlpha11;
float Globals::MenuAlpha10 = MenuAlpha9;
float Globals::MenuAlpha8 = MenuAlpha7;
float Globals::MenuAlpha6 = MenuAlpha5;
float Globals::MenuAlpha2 = MenuAlpha;
float Globals::MenuAlpha4 = MenuAlpha3;


CGUI GUI;

CGUI::CGUI()
{

}

// Draws all windows 
void CGUI::Draw()
{
	bool ShouldDrawCursor = false;

	for (auto window : Windows)
	{
		if (window->m_bIsOpen)
			MenuAlpha = min(MenuAlpha + 15, 255);
		else
			MenuAlpha = max(MenuAlpha - 15, 0);

		if (window->m_bIsOpen)
			MenuAlpha5 = min(MenuAlpha5 + 15, 120);
		else
			MenuAlpha5 = max(MenuAlpha5 - 15, 0);

		if (window->m_bIsOpen)
			MenuAlpha3 = min(MenuAlpha3 + 15, 15);
		else
			MenuAlpha3 = max(MenuAlpha3 - 15, 0);

		if (window->m_bIsOpen)
			MenuAlpha7 = min(MenuAlpha7 + 15, 200);
		else
			MenuAlpha7 = max(MenuAlpha7 - 15, 0);

		if (window->m_bIsOpen)
			MenuAlpha9 = min(MenuAlpha9 + 15, 245);
		else
			MenuAlpha9 = max(MenuAlpha9 - 15, 0);

		if (window->m_bIsOpen)
			MenuAlpha11 = min(MenuAlpha11 + 15, 80);
		else
			MenuAlpha11 = max(MenuAlpha11 - 15, 0);

		if (window->m_bIsOpen)
			MenuAlpha13 = min(MenuAlpha13 + 15, 140);
		else
			MenuAlpha13 = max(MenuAlpha13 - 15, 0);

		if (window->m_bIsOpen)
			MenuAlpha15 = min(MenuAlpha15 + 15, 40);
		else
			MenuAlpha15 = max(MenuAlpha15 - 15, 0);

		if (window->m_bIsOpen)
			MenuAlpha17 = min(MenuAlpha17 + 15, 50);
		else
			MenuAlpha17 = max(MenuAlpha17 - 15, 0);

		if (window->m_bIsOpen)
			MenuAlpha21 = min(MenuAlpha21 + 15, 1);
		else
			MenuAlpha21 = max(MenuAlpha21 - 15, 0);

		if (window->m_bIsOpen)
			MenuAlpha19 = min(MenuAlpha19 + 15, 100);
		else
			MenuAlpha19 = max(MenuAlpha19 - 15, 0);

		if (window->m_bIsOpen)
			MenuAlpha23 = min(MenuAlpha23 + 15, 255);
		else
			MenuAlpha23 = max(MenuAlpha23 - 15, 0);
		{
			ShouldDrawCursor = true;
			DrawWindow(window);
		}
	}
	if (ShouldDrawCursor)
	{
		Render::Clear(Mouse.x + 1, Mouse.y, 1, 17, Color(3, 6, 26, MenuAlpha));
		for (int i = 0; i < 11; i++)
			Render::Clear(Mouse.x + 2 + i, Mouse.y + 1 + i, 1, 1, Color(3, 6, 26, MenuAlpha));
		Render::Clear(Mouse.x + 8, Mouse.y + 12, 5, 1, Color(3, 6, 26, MenuAlpha));
		Render::Clear(Mouse.x + 8, Mouse.y + 13, 1, 1, Color(3, 6, 26, MenuAlpha));
		Render::Clear(Mouse.x + 9, Mouse.y + 14, 1, 2, Color(3, 6, 26, MenuAlpha));
		Render::Clear(Mouse.x + 10, Mouse.y + 16, 1, 2, Color(3, 6, 26, MenuAlpha));
		Render::Clear(Mouse.x + 8, Mouse.y + 18, 2, 1, Color(3, 6, 26, MenuAlpha));
		Render::Clear(Mouse.x + 7, Mouse.y + 16, 1, 2, Color(3, 6, 26, MenuAlpha));
		Render::Clear(Mouse.x + 6, Mouse.y + 14, 1, 2, Color(3, 6, 26, MenuAlpha));
		Render::Clear(Mouse.x + 5, Mouse.y + 13, 1, 1, Color(3, 6, 26, MenuAlpha));
		Render::Clear(Mouse.x + 4, Mouse.y + 14, 1, 1, Color(3, 6, 26, MenuAlpha));
		Render::Clear(Mouse.x + 3, Mouse.y + 15, 1, 1, Color(3, 6, 26, MenuAlpha));
		Render::Clear(Mouse.x + 2, Mouse.y + 16, 1, 1, Color(3, 6, 26, MenuAlpha));
		for (int i = 0; i < 4; i++)
			Render::Clear(Mouse.x + 2 + i, Mouse.y + 2 + i, 1, 14 - (i * 2), Color(80, 80, 80, MenuAlpha));
		Render::Clear(Mouse.x + 6, Mouse.y + 6, 1, 8, Color(80, 80, 80, MenuAlpha));
		Render::Clear(Mouse.x + 7, Mouse.y + 7, 1, 9, Color(80, 80, 80, MenuAlpha));
		for (int i = 0; i < 4; i++)
			Render::Clear(Mouse.x + 8 + i, Mouse.y + 8 + i, 1, 4 - i, Color(80, 80, 80, MenuAlpha));
		Render::Clear(Mouse.x + 8, Mouse.y + 14, 1, 4, Color(80, 80, 80, MenuAlpha));
		Render::Clear(Mouse.x + 9, Mouse.y + 16, 1, 2, Color(80, 80, 80, MenuAlpha));
	}
}

// Handle all input etc
void CGUI::Update()
{
	static int bWasntHolding = false;
	static int bGrabbing = false;
	static int iOffsetX = 0, iOffsetY = 0;
	//Key Array
	std::copy(keys, keys + 255, oldKeys);
	for (int x = 0; x < 255; x++)
	{
		//oldKeys[x] = oldKeys[x] & keys[x];
		keys[x] = (GetAsyncKeyState(x));
	}

	// Mouse Location
	POINT mp; GetCursorPos(&mp);
	ScreenToClient(GetForegroundWindow(), &mp);
	Mouse.x = mp.x; Mouse.y = mp.y;

	RECT Screen = Render::GetViewport();

	// Window Binds
	for (auto& bind : WindowBinds)
	{
		if (GetKeyPress(bind.first))
		{
			bind.second->Toggle();
		}
	}

	// Stop dragging
	if (IsDraggingWindow && !GetKeyState(VK_LBUTTON))
	{
		IsDraggingWindow = false;
		DraggingWindow = nullptr;
	}

	// If we are in the proccess of dragging a window
	if (IsDraggingWindow && GetKeyState(VK_LBUTTON) && !GetKeyPress(VK_LBUTTON))
	{
		if (DraggingWindow)
		{
			DraggingWindow->m_x = Mouse.x - DragOffsetX;
			DraggingWindow->m_y = Mouse.y - DragOffsetY;
		}
	}

	// Process some windows
	for (auto window : Windows)
	{
		if (window->m_bIsOpen)
		{
			{
				// Used to tell the widget processing that there could be a click
				bool bCheckWidgetClicks = false;

				// If the user clicks inside the window
				if (GetKeyPress(VK_LBUTTON))
				{
					//if (IsMouseInRegion(window->m_x, window->m_y, window->m_x + window->m_iWidth, window->m_y + window->m_iHeight))
					//{
					// Is it inside the client area?
					if (IsMouseInRegion(window->GetClientArea()))
					{
						// User is selecting a new tab
						if (IsMouseInRegion(window->GetTabArea()))
						{
							/*// Loose focus on the control
							window->IsFocusingControl = false;
							window->FocusedControl = nullptr;

							int iTab = 0;
							int TabCount = window->Tabs.size();
							if (TabCount) // If there are some tabs
							{
							int TabSize = (window->m_iWidth - 4 - 12) / TabCount;
							int Dist = Mouse.x - (window->m_x + 8);
							while (Dist > TabSize)
							{
							if (Dist > TabSize)
							{
							iTab++;
							Dist -= TabSize;
							}
							}
							window->SelectedTab = window->Tabs[iTab];
							}*/
						}
						else
							bCheckWidgetClicks = true;
					}
					else
					{
						// Loose focus on the control
						window->IsFocusingControl = false;
						window->FocusedControl = nullptr;
					}
				}

				if (IsMouseInRegion(window->GetDragArea()))
				{
					// Must be in the around the title or side of the window
					// So we assume the user is trying to drag the window
					IsDraggingWindow = true;
					DraggingWindow = window;
					DragOffsetX = Mouse.x - window->m_x;
					DragOffsetY = Mouse.y - window->m_y;

					// Loose focus on the control
					window->IsFocusingControl = false;
					window->FocusedControl = nullptr;
				}

				// Controls 
				if (window->SelectedTab != nullptr)
				{
					// Focused widget
					bool SkipWidget = false;
					CControl* SkipMe = nullptr;

					// this window is focusing on a widget??
					if (window->IsFocusingControl)
					{
						if (window->FocusedControl != nullptr)
						{
							// We've processed it once, skip it later
							SkipWidget = true;
							SkipMe = window->FocusedControl;

							POINT cAbs = window->FocusedControl->GetAbsolutePos();
							RECT controlRect = { cAbs.x, cAbs.y, window->FocusedControl->m_iWidth, window->FocusedControl->m_iHeight };
							window->FocusedControl->OnUpdate();

							if (window->FocusedControl->Flag(UIFlags::UI_Clickable) && IsMouseInRegion(controlRect) && bCheckWidgetClicks)
							{
								window->FocusedControl->OnClick();

								// If it gets clicked we loose focus
								window->IsFocusingControl = false;
								window->FocusedControl = nullptr;
								bCheckWidgetClicks = false;
							}
						}
					}

					// Itterate over the rest of the control
					for (auto control : window->SelectedTab->Controls)
					{
						if (control != nullptr)
						{
							if (SkipWidget && SkipMe == control)
								continue;

							POINT cAbs = control->GetAbsolutePos();
							RECT controlRect = { cAbs.x, cAbs.y, control->m_iWidth, control->m_iHeight };
							control->OnUpdate();

							if (control->Flag(UIFlags::UI_Clickable) && IsMouseInRegion(controlRect) && bCheckWidgetClicks)
							{
								control->OnClick();
								bCheckWidgetClicks = false;

								// Change of focus
								if (control->Flag(UIFlags::UI_Focusable))
								{
									window->IsFocusingControl = true;
									window->FocusedControl = control;
								}
								else
								{
									window->IsFocusingControl = false;
									window->FocusedControl = nullptr;
								}

							}
						}
					}

					// We must have clicked whitespace
					if (bCheckWidgetClicks)
					{
						// Loose focus on the control
						window->IsFocusingControl = false;
						window->FocusedControl = nullptr;
					}
				}
			}
		}
	}
}

// Returns 
bool CGUI::GetKeyPress(unsigned int key)
{
	if (keys[key] == true && oldKeys[key] == false)
		return true;
	else
		return false;
}

bool CGUI::GetKeyState(unsigned int key)
{
	return keys[key];
}

bool CGUI::IsMouseInRegion(int x, int y, int x2, int y2)
{
	if (Mouse.x > x && Mouse.y > y && Mouse.x < x2 && Mouse.y < y2)
		return true;
	else
		return false;
}

bool CGUI::IsMouseInRegion(RECT region)
{
	return IsMouseInRegion(region.left, region.top, region.left + region.right, region.top + region.bottom);
}

POINT CGUI::GetMouse()
{
	return Mouse;
}

bool CGUI::DrawWindow(CWindow* window)
{
	int iProportion = window->m_iWidth - 4 - 12 + 90 - 2;
	iProportion = iProportion / 3;

	//RenderOutline(window->m_x + 7, window->m_y + 1 + 27 * 2, window->m_iWidth - 4 - 10, window->m_iHeight - 2 - 6 - 26, Color(20, 20, 20, 80));
	Render::Clear(window->m_x + 8 - 90, window->m_y + 1 + 28 * 2, window->m_iWidth - 4 - 12 + 90, window->m_iHeight - 2 - 8 - 26, Color(28, 28, 28, MenuAlpha));
	Render::Clear(window->m_x + 8 - 90, window->m_y + 1 + 28 * 2 - 6, window->m_iWidth - 4 - 12 + 90, 6, Color(40, 40, 40, MenuAlpha));
	Render::Clear(window->m_x + 8 - 90, window->m_y + 1 + 28 * 2 + window->m_iHeight - 2 - 8 - 26, window->m_iWidth - 4 - 12 + 90, 6, Color(40, 40, 40, MenuAlpha));
	Render::Clear(window->m_x + 8 - 90 - 6, window->m_y + 1 + 28 * 2 - 6, 6, window->m_iHeight - 2 - 8 - 26 + 12, Color(40, 40, 40, MenuAlpha));
	Render::Clear(window->m_x + 8 + window->m_iWidth - 4 - 12, window->m_y + 1 + 28 * 2 - 6, 6, window->m_iHeight - 2 - 8 - 26 + 12, Color(40, 40, 40, MenuAlpha));

	//Tab
	Render::Clear(window->m_x + 8 - 90, window->m_y + 1 + 28 * 2, 90 - 16, window->m_iHeight - 2 - 8 - 26, Color(21, 21, 19, MenuAlpha));

	Render::Outline(window->m_x + 8 - 90, window->m_y + 1 + 28 * 2, window->m_iWidth - 4 - 12 + 90, window->m_iHeight - 2 - 8 - 26, Color(48, 48, 48, MenuAlpha));
	//Render::Clear(window->m_x + 8, window->m_y + 1 + 28 * 2, 1, window->m_iHeight - 2 - 8 - 26 - 1, Color(129, 129, 129, 255));
	Render::Outline(window->m_x + 8 - 6 - 90, window->m_y + 1 + 28 * 2 - 6, window->m_iWidth - 4 - 12 + 90 + 12, window->m_iHeight - 2 - 8 - 26 + 12, Color(48, 48, 48, MenuAlpha));
	Render::Outline(window->m_x + 8 - 90, window->m_y + 1 + 28 * 2, 90 - 16, window->m_iHeight - 2 - 8 - 26, Color(48, 48, 48, MenuAlpha));

	//Skeet Bar
	Render::GradientSideways(window->m_x - 81, window->m_y + 1 + 28 * 2 + 1, iProportion, 1, Color(80, 159, 216, MenuAlpha), Color(159, 102, 209, MenuAlpha), 1);
	Render::GradientSideways(window->m_x - 81 + iProportion, window->m_y + 1 + 28 * 2 + 1, iProportion, 1, Color(159, 102, 209, MenuAlpha), Color(204, 81, 194, MenuAlpha), 1);
	Render::GradientSideways(window->m_x - 81 + (iProportion * 2), window->m_y + 1 + 28 * 2 + 1, iProportion, 1, Color(204, 81, 194, MenuAlpha), Color(204, 227, 53, MenuAlpha), 1);

	int tabcount = window->Tabs.size();
	if (tabcount) // If there are some tabs
	{
		bool isOut = true;

		for (int i = 0; i < tabcount; i++)
		{
			CTab *tab = window->Tabs[i];

			float xAxis;
			float yAxis;
			float yWinPos = window->m_y + 1 + 28 * 2;
			float yWinHeight = (window->m_iHeight - 200) - 2 - 8 - 26;

			float intercept = (yWinHeight - 40) / tabcount;
			int factor = i;

			yAxis = yWinPos + 16 + (i * 75) - 8;

			RECT TabDrawArea = { window->m_x + 8 - 90 + 1, yAxis + 20, 90 - 17, 75 };

			RECT TextSize;
			TextSize = Render::GetTextSize(Render::Fonts::Tabs, tab->Title.c_str());

			RECT ClickTabArea = { xAxis,
				yAxis,
				TextSize.right,
				TextSize.bottom };

			if (GetAsyncKeyState(VK_LBUTTON))
			{
				if (IsMouseInRegion(TabDrawArea))
				{
					window->SelectedTab = window->Tabs[i];
				}
			}

			if (window->SelectedTab == tab) {
				xAxis = window->m_x + 8 - (45 + TextSize.right / 2);
				Render::Clear(TabDrawArea.left, TabDrawArea.top, TabDrawArea.right, TabDrawArea.bottom, Color(28, 28, 28, MenuAlpha));
				Render::Line(TabDrawArea.left, TabDrawArea.top, TabDrawArea.left + TabDrawArea.right, TabDrawArea.top, Color(48, 48, 48, MenuAlpha));
				Render::Line(TabDrawArea.left, TabDrawArea.top + TabDrawArea.bottom, TabDrawArea.left + TabDrawArea.right, TabDrawArea.top + TabDrawArea.bottom, Color(48, 48, 48, MenuAlpha));
				Render::Text(TabDrawArea.left + (TabDrawArea.right / 2) - (TextSize.right / 2), TabDrawArea.top + (TabDrawArea.bottom / 2) - (TextSize.bottom / 2), Color(244, 244, 244, MenuAlpha), Render::Fonts::Tabs, tab->Title.c_str());
			}
			else {
				xAxis = window->m_x + 8 - (45 + TextSize.right / 2);
				Render::Text(TabDrawArea.left + (TabDrawArea.right / 2) - (TextSize.right / 2), TabDrawArea.top + (TabDrawArea.bottom / 2) - (TextSize.bottom / 2), Color(100, 100, 100, MenuAlpha), Render::Fonts::Tabs, tab->Title.c_str());
			}

			//Render::Clear(window->m_x + 8, window->m_y + 1 + 27, window->m_iWidth - 4 - 12, 2, Color(65, 55, 55, 255));
		}
	}

	//Render::Outline(window->m_x)
	// Controls 

	if (window->SelectedTab != nullptr)
	{
		// Focused widget
		bool SkipWidget = false;
		CControl* SkipMe = nullptr;

		// this window is focusing on a widget??
		if (window->IsFocusingControl)
		{
			if (window->FocusedControl != nullptr)
			{
				// We need to draw it last, so skip it in the regular loop
				SkipWidget = true;
				SkipMe = window->FocusedControl;
			}
		}


		// Itterate over all the other controls
		for (auto control : window->SelectedTab->Controls)
		{
			if (SkipWidget && SkipMe == control)
				continue;

			if (control != nullptr && control->Flag(UIFlags::UI_Drawable))
			{
				POINT cAbs = control->GetAbsolutePos();
				RECT controlRect = { cAbs.x, cAbs.y, control->m_iWidth, control->m_iHeight };
				bool hover = false;
				if (IsMouseInRegion(controlRect))
				{
					hover = true;
				}
				control->Draw(hover);
			}
		}

		// Draw the skipped widget last
		if (SkipWidget)
		{
			auto control = window->FocusedControl;

			if (control != nullptr && control->Flag(UIFlags::UI_Drawable))
			{
				POINT cAbs = control->GetAbsolutePos();
				RECT controlRect = { cAbs.x, cAbs.y, control->m_iWidth, control->m_iHeight };
				bool hover = false;
				if (IsMouseInRegion(controlRect))
				{
					hover = true;
				}
				control->Draw(hover);
			}
		}

	}


	return true;
}

void CGUI::RegisterWindow(CWindow* window)
{
	Windows.push_back(window);

	// Resorting to put groupboxes at the start
	for (auto tab : window->Tabs)
	{
		for (auto control : tab->Controls)
		{
			if (control->Flag(UIFlags::UI_RenderFirst))
			{
				CControl * c = control;
				tab->Controls.erase(std::remove(tab->Controls.begin(), tab->Controls.end(), control), tab->Controls.end());
				tab->Controls.insert(tab->Controls.begin(), control);
			}
		}
	}
}

void CGUI::BindWindow(unsigned char Key, CWindow* window)
{
	if (window)
		WindowBinds[Key] = window;
	else
		WindowBinds.erase(Key);
}

void CGUI::SaveWindowState(CWindow* window, std::string Filename)
{
	tinyxml2::XMLDocument Doc;

	tinyxml2::XMLElement *Root = Doc.NewElement("Corruption");
	Doc.LinkEndChild(Root);

	if (Root && window->Tabs.size() > 0)
	{
		for (auto Tab : window->Tabs)
		{
			tinyxml2::XMLElement *TabElement = Doc.NewElement(Tab->Title.c_str());
			Root->LinkEndChild(TabElement);

			if (TabElement && Tab->Controls.size() > 1)
			{
				for (auto Control : Tab->Controls)
				{
					if (Control && Control->Flag(UIFlags::UI_SaveFile) && Control->FileIdentifier.length() > 1 && Control->FileControlType)
					{
						tinyxml2::XMLElement *ControlElement = Doc.NewElement(Control->FileIdentifier.c_str());
						TabElement->LinkEndChild(ControlElement);

						if (!ControlElement)
						{
							return;
						}

						CCheckBox* cbx = nullptr;
						CComboBox* cbo = nullptr;
						CKeyBind* key = nullptr;
						CSlider* sld = nullptr;
						CSlider2* sld2 = nullptr;
						CSlider3* sld3 = nullptr;
						CSlider4* sld4 = nullptr;


						switch (Control->FileControlType)
						{
						case UIControlTypes::UIC_CheckBox:
							cbx = (CCheckBox*)Control;
							ControlElement->SetText(cbx->GetState());
							break;
						case UIControlTypes::UIC_ComboBox:
							cbo = (CComboBox*)Control;
							ControlElement->SetText(cbo->GetIndex());
							break;
						case UIControlTypes::UIC_KeyBind:
							key = (CKeyBind*)Control;
							ControlElement->SetText(key->GetKey());
							break;
						case UIControlTypes::UIC_Slider:
							sld = (CSlider*)Control;
							ControlElement->SetText(sld->GetValue());
							break;
						case UIControlTypes::UIC_Slider2:
							sld2 = (CSlider2*)Control;
							ControlElement->SetText(sld2->GetValue());
							break;
						case UIControlTypes::UIC_Slider3:
							sld3 = (CSlider3*)Control;
							ControlElement->SetText(sld3->GetValue());
							break;
						case UIControlTypes::UIC_Slider4:
							sld4 = (CSlider4*)Control;
							ControlElement->SetText(sld4->GetValue());
							break;
						}
					}
				}
			}
		}
	}

	if (Doc.SaveFile(Filename.c_str()) != tinyxml2::XML_NO_ERROR)
	{
		MessageBox(NULL, "Failed To Save Config File!", "Corruption.vip", MB_OK);
	}

}

void CGUI::LoadWindowState(CWindow* window, std::string Filename)
{
	tinyxml2::XMLDocument Doc;
	if (Doc.LoadFile(Filename.c_str()) == tinyxml2::XML_NO_ERROR)
	{
		tinyxml2::XMLElement *Root = Doc.RootElement();

		if (Root)
		{
			if (Root && window->Tabs.size() > 0)
			{
				for (auto Tab : window->Tabs)
				{
					tinyxml2::XMLElement *TabElement = Root->FirstChildElement(Tab->Title.c_str());
					if (TabElement)
					{
						if (TabElement && Tab->Controls.size() > 0)
						{
							for (auto Control : Tab->Controls)
							{
								if (Control && Control->Flag(UIFlags::UI_SaveFile) && Control->FileIdentifier.length() > 1 && Control->FileControlType)
								{
									tinyxml2::XMLElement *ControlElement = TabElement->FirstChildElement(Control->FileIdentifier.c_str());

									if (ControlElement)
									{
										CCheckBox* cbx = nullptr;
										CComboBox* cbo = nullptr;
										CKeyBind* key = nullptr;
										CSlider* sld = nullptr;
										CSlider2* sld2 = nullptr;
										CSlider3* sld3 = nullptr;
										CSlider4* sld4 = nullptr;

										switch (Control->FileControlType)
										{
										case UIControlTypes::UIC_CheckBox:
											cbx = (CCheckBox*)Control;
											cbx->SetState(ControlElement->GetText()[0] == '1' ? true : false);
											break;
										case UIControlTypes::UIC_ComboBox:
											cbo = (CComboBox*)Control;
											cbo->SelectIndex(atoi(ControlElement->GetText()));
											break;
										case UIControlTypes::UIC_KeyBind:
											key = (CKeyBind*)Control;
											key->SetKey(atoi(ControlElement->GetText()));
											break;
										case UIControlTypes::UIC_Slider:
											sld = (CSlider*)Control;
											sld->SetValue(atof(ControlElement->GetText()));
											break;
										case UIControlTypes::UIC_Slider2:
											sld2 = (CSlider2*)Control;
											sld2->SetValue(atof(ControlElement->GetText()));
											break;
										case UIControlTypes::UIC_Slider3:
											sld3 = (CSlider3*)Control;
											sld3->SetValue(atof(ControlElement->GetText()));
											break;
										case UIControlTypes::UIC_Slider4:
											sld4 = (CSlider4*)Control;
											sld4->SetValue(atof(ControlElement->GetText()));
											break;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}