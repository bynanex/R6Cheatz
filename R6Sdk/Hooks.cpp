#include "pch.h"
#include "Cheat.h"
#include "Hooks.h"
#include "SDK.h"
#include "D3dDraw.h"
#include "Aimbot.h"
Draw* D = new Draw;
CAimBot* AimBot = new CAimBot;
WNDPROC oWndProc = nullptr;
tD3D11Present Present = nullptr;
ID3D11Device* g_pDevice = NULL;
ID3D11DeviceContext* g_pContext = NULL;
ID3D11RenderTargetView* g_pRenderTargetView = NULL;

Vector RotatePoint(Vector EntityPos, Vector LocalPlayerPos, int posX, int posY, int sizeX, int sizeY, float angle, float zoom, bool* viewCheck, bool angleInRadians = false)
{
	float r_1, r_2;
	float x_1, y_1;

	r_1 = -(EntityPos.y - LocalPlayerPos.y);
	r_2 = EntityPos.x - LocalPlayerPos.x;

	float Yaw = angle - 90.0f;
	float yawToRadian = Yaw * (float)(M_PI / 180.0F);
	x_1 = (float)(r_2 * (float)cos((double)(yawToRadian)) - r_1 * sin((double)(yawToRadian))) / 20;
	y_1 = (float)(r_2 * (float)sin((double)(yawToRadian)) + r_1 * cos((double)(yawToRadian))) / 20;
	*viewCheck = y_1 < 0;
	x_1 *= zoom;
	y_1 *= zoom;
	int sizX = sizeX / 2;
	int sizY = sizeY / 2;
	x_1 += sizX;

	y_1 += sizY;
	if (x_1 < 5)
		x_1 = 5;
	if (x_1 > sizeX - 5)
		x_1 = sizeX - 5;
	if (y_1 < 5)
		y_1 = 5;
	if (y_1 > sizeY - 5)
		y_1 = sizeY - 5;
	x_1 += posX;
	y_1 += posY;
	return Vector(x_1, y_1, 0);
}
ImU32 GetU32(int r, int b, int g, int a)
{
	return ((a & 0xff) << 24) + ((g & 0xff) << 16) + ((b & 0xff) << 8)
		+ (r & 0xff);
}
void InitMyMenu()
{
	ImGuiStyle* style = &ImGui::GetStyle();
	style->WindowPadding = ImVec2(15, 15);
	style->WindowRounding = 5.0f;
	style->FramePadding = ImVec2(5, 5);
	style->FrameRounding = 4.0f;
	style->ItemSpacing = ImVec2(12, 8);
	style->ItemInnerSpacing = ImVec2(8, 6);
	style->IndentSpacing = 25.0f;
	style->ScrollbarSize = 15.0f;
	style->ScrollbarRounding = 9.0f;
	style->GrabMinSize = 5.0f;
	style->GrabRounding = 3.0f;
	style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
	style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
	style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
	style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_PopupBg] = ImVec4(0.19f, 0.18f, 0.21f, 1.00f);
	style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_Column] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ColumnHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_ColumnActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_CloseButton] = ImVec4(0.40f, 0.39f, 0.38f, 0.16f);
	style->Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.40f, 0.39f, 0.38f, 0.39f);
	style->Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.40f, 0.39f, 0.38f, 1.00f);
	style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
	style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
}
void AimbotTab()
{
	ImGui::Checkbox(XorString("Enable Aimbot"), &Vars.Aim.Enable);
	ImGui::Columns(1);
	ImGui::Separator();
	ImGui::Columns(2, NULL, true);
	{
		ImGui::BeginChild(XorString("RAGE1"), ImVec2(0, 0), true);
		{
			ImGui::Checkbox(XorString("AimKey"), &Vars.Aim.EnableAimBotKey);
			ImGui::Combo(XorString("Key"), &Vars.Aim.AimBotKey, keyNames, ARRAYSIZE(keyNames));
			ImGui::SliderFloat(XorString("Fov"), &Vars.Aim.Fov, 1.f, 360.f, "%.0f");
			ImGui::SliderInt(XorString("Soomth"), &Vars.Aim.aimSmooth, 0, 10);
			ImGui::Checkbox(XorString("OneShot"), &Vars.Aim.OneShot);
			ImGui::Separator();
			ImGui::Text(XorString("Target"));
			ImGui::Separator();
			ImGui::Combo(XorString("Scan Method"), &Vars.Aim.TargetMethod, targetMode, ARRAYSIZE(targetMode));
			ImGui::Text(XorString("HitBox"));
			ImGui::Separator();
			ImGui::Checkbox(XorString("Head"), &Vars.Aim.Hitboxs[0]);
			ImGui::Checkbox(XorString("Neck"), &Vars.Aim.Hitboxs[1]);
			ImGui::Checkbox(XorString("Hand"), &Vars.Aim.Hitboxs[2]);
			ImGui::Checkbox(XorString("Chest"), &Vars.Aim.Hitboxs[3]);
			ImGui::Checkbox(XorString("Stomach"), &Vars.Aim.Hitboxs[4]);
			ImGui::Checkbox(XorString("Pelvis"), &Vars.Aim.Hitboxs[5]);
			ImGui::Checkbox(XorString("Feet"), &Vars.Aim.Hitboxs[6]);
			ImGui::Separator();
			ImGui::EndChild();
		}

	}
	ImGui::NextColumn();
	{
		ImGui::BeginChild(XorString("RAGE2"), ImVec2(0, 0), true);
		{
			ImGui::Text(XorString("Accuracy"));
			ImGui::Separator();
			ImGui::Checkbox(XorString("No Recoil"), &Vars.Aim.NoRecoil);
			ImGui::Checkbox(XorString("No Spread"), &Vars.Aim.NoSpread);
			ImGui::Separator();
			ImGui::EndChild();
		}
	}
}

void VisualTab()
{
	ImGui::Checkbox(XorString("Enable Visual"), &Vars.Visuals.Enabled);
	ImGui::Columns(1);
	ImGui::Separator();
	ImGui::Columns(2, NULL, true);
	{
		ImGui::BeginChild(XorString("ESP1"), ImVec2(0, 0), true);
		{
			ImGui::Checkbox(XorString("Player Esp"), &Vars.Visuals.PlayerEnable);
			ImGui::Separator();
			ImGui::Columns(2, "##c1", false);
			ImGui::Checkbox(XorString("Name"), &Vars.Visuals.Name);
			ImGui::Checkbox(XorString("Box"), &Vars.Visuals.Box);
			ImGui::Checkbox(XorString("Skeleton"), &Vars.Visuals.Skeleton);
			ImGui::Checkbox(XorString("Distance"), &Vars.Visuals.Distance);
			ImGui::Checkbox(XorString("Health"), &Vars.Visuals.Health);
			ImGui::Separator();
			ImGui::EndChild();
		}

	}
	ImGui::NextColumn();
	{
		ImGui::BeginChild(XorString("ESP2"), ImVec2(0, 0), true);
		{
			ImGui::Checkbox(XorString("Enable Radar"), &Vars.Visuals.Radar.Enable);
			ImGui::Separator();
			ImGui::SliderFloat(XorString("MaxDistance"), &Vars.Visuals.Radar.MaxDis, 10.f, 2000.f, "%.1f");//×î´óÍæ¼Ò¾àÀë
			ImGui::Checkbox(XorString("Enemy Only"), &Vars.Visuals.Radar.EnemyOnly);
			ImGui::Checkbox(XorString("Visible Only"), &Vars.Visuals.Radar.VisibleOnly);
			ImGui::Checkbox(XorString("View Check"), &Vars.Visuals.Radar.ViewCheck);
			ImGui::Text("Alpha"); ImGui::SameLine();
			ImGui::SliderInt("Alpha##0", &Vars.Visuals.Radar.Alpha, 0, 255);
			ImGui::Text("Zoom"); ImGui::SameLine();
			ImGui::SliderFloat("Zoom##0", &Vars.Visuals.Radar.Zoom, 0.f, 10.f);
			ImGui::Text("Type");
			ImGui::Combo("##1", &Vars.Visuals.Radar.Type, "Box\0Filled box\0Circle\0Circle filled\0\0", -1);
			if (ImGui::Button(XorString("ResetRadar"), ImVec2(120, 0))) Vars.Visuals.Radar.NextResetRadar = true;
			ImGui::Separator();
			ImGui::EndChild();
		}
	}
}

void MiscHackTab()
{
	ImGui::Text(XorString("Misc"));
	ImGui::Separator();

	ImGui::Columns(2, NULL, true);
	{
		ImGui::BeginChild("MISC", ImVec2(0, 0), true);
		{
			ImGui::Text(XorString("Weapon"));
			ImGui::Separator();
			ImGui::Checkbox(XorString("Rapid Fire"), &Vars.Misc.RapidFire);
			ImGui::SliderFloat(XorString("Fire"), &Vars.Misc.RapidFireVal, 1.f, 130.f, "%.1f");
			ImGui::Checkbox(XorString("More DMG"), &Vars.Misc.MoreDmg);
			ImGui::SliderInt(XorString("DMG"), &Vars.Misc.MoreDmgVal, 1, 130, "%.1f");
			ImGui::Checkbox(XorString("Instant Reload"), &Vars.Misc.InstantReload);
			ImGui::Checkbox(XorString("No Recoil"), &Vars.Misc.NoRecoil);
			ImGui::Checkbox(XorString("No Spread"), &Vars.Misc.NoSpread);
			ImGui::Separator();
			ImGui::EndChild();
		}

	}
	ImGui::NextColumn();
	{
		ImGui::BeginChild("MISC2", ImVec2(0, 0), true);
		{
			ImGui::Text(XorString("MoveMent"));
			ImGui::Separator();
			ImGui::Checkbox(XorString("Speed Hack"), &Vars.Misc.SpeedHack);
			ImGui::SliderFloat(XorString("Speed"), &Vars.Misc.SpeedHackVal, 0.f, 20.f, "%.1f");
			ImGui::Separator();
			ImGui::EndChild();
		}
	}
}
void MainWindows()
{
	ImGui::GetIO().MouseDrawCursor = false;
	static int page = 0;
	if (Vars.Menu.Opened)
	{
		static int page = 0;
		ImGui::SetNextWindowSize(ImVec2(960, 645), ImGuiSetCond_FirstUseEver);
		if (ImGui::Begin(XorString("RooKie AA3Cheatz For Rainbow Six 2019"), &Vars.Menu.Opened, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar))
		{
			const char* tabs[] = {
				"Aim",
				"Visual",
				"Misc"
			};
			int tabs_size = sizeof(tabs) / sizeof(tabs[0]);

			for (int i = 0; i < tabs_size; i++)
			{
				ImVec2 windowSize = ImGui::GetWindowSize();
				int width = windowSize.x / tabs_size - 9;

				int distance;
				if (i == page)
					distance = 0;
				else
					distance = i > page ? i - page : page - i;
				if (ImGui::Button(tabs[i], ImVec2(width, 0)))
					page = i;
				if (i < tabs_size - 1)
					ImGui::SameLine();
			}

			switch (page)
			{
			case 0:
				AimbotTab();
				break;
			case 1:
				VisualTab();
				break;
			case 2:
				MiscHackTab();
				break;
			}
			ImGui::End();
		}
	}
}
void DrawBone(CBaseEntity* player)
{
	Vector out[7];
	Vector2D out2D[7];
	GColor c;
	for (int i = 0; i < 7; i++)
	{
		out[i] = player->GetBonePostionByID(i);
		I::Engine->WorldToScreen(out[i], out2D[i]);
	}
	c.SetGColor(0, 255, 255, 255);
	//	Í·->²±×Ó
	D->DrawLine(out2D[BONE_HEAD], out2D[BONE_NECK], c.GetRawGColor());
	//	×óÓÒËø¹Ç->×óÓÒ¼ç
	D->DrawLine(out2D[BONE_NECK], out2D[BONE_HAND], c.GetRawGColor());
	//	²±×Ó->¸¹²¿
	D->DrawLine(out2D[BONE_NECK], out2D[BONE_STOMACH], c.GetRawGColor());
	D->DrawLine(out2D[BONE_STOMACH], out2D[BONE_PELVIS], c.GetRawGColor());
	D->DrawLine(out2D[BONE_PELVIS], out2D[BONE_FEET], c.GetRawGColor());
}

void PlayerEsp()
{
	Array<CBaseEntity*> Entitylist = I::Engine->GetEntities();
	int LocalPlayerTeam = G::LocalPlayer->GetTeamNumber();
	//DbgPrintA("%f %f %f",G::LocalPlayer->GetViewAngle().x, G::LocalPlayer->GetViewAngle().y, G::LocalPlayer->GetViewAngle().z);
	for (uint32_t i = 0; i < I::Engine->GetEntitiesCount(); i++)
	{
		auto pEntity = Entitylist[i];
		if (pEntity)
		{
			int hp = pEntity->GetHealth();
			if (hp > 0 && pEntity->GetTeamNumber() != LocalPlayerTeam)
			{
				Vector2D screennike, screenHead;
				Vector orgin = pEntity->GetFeet(), head = pEntity->GetHead();
				float fDis = I::Engine->GetCamera()->GetViewTranslation().Distance(pEntity->GetHead());
				if (I::Engine->WorldToScreen(orgin, screennike) && I::Engine->WorldToScreen(head, screenHead))
				{
					GColor c;
					int iMiddle = (screennike.y - screenHead.y);
					int iWidth = iMiddle / 5;
					if (Vars.Visuals.Box)
					{
						c.SetGColor(0, 128, 255);
						D->DrawBox(Vector2D(screenHead.x - iWidth, screenHead.y), Vector2D(iWidth * 2, iMiddle), c.GetRawGColor(), 2.f);
						c.SetGColor(0, 0, 0, 220);
						D->DrawBox(Vector2D(screenHead.x + iWidth + 1, screenHead.y), Vector2D(4, iMiddle), c.GetRawGColor(), 1.f);
						c.SetGColor(181, 230, 29, 220);
					}
					if (Vars.Visuals.Health)
					{
						float fHP = hp * 100 / 100.f;
						D->DrawFilledBox(Vector2D(screenHead.x + iWidth + 2, screenHead.y + 1 + (100 - fHP) / 100 * iMiddle), Vector2D(2, fHP / 100 * iMiddle - 1), c.GetRawGColor());
					}
					if (Vars.Visuals.Skeleton)
					{
						D->DrawCircle(screenHead,10.f,c.GetRawGColor(), 10,1.f);
						DrawBone(pEntity);
					}
					
					if (Vars.Visuals.Name)
					{
						c.SetGColor(255, 255, 255, 220);
						D->DrawString(12, Vector2D(screennike.x, screennike.y - 35), c.GetRawGColor(), true, true, "%s", (pEntity->GetPlayerName().c_str()));
					}
					//»­¾àÀë
					if (Vars.Visuals.Distance)
					{
						c.SetGColor(255, 255, 255, 220);
						D->DrawString(12, Vector2D(screennike.x, screennike.y - 45), c.GetRawGColor(), true, true, XorString("[%.fm]"), fDis);
					}
				}
			}
		}
	}
}
/*
void setOutlineColor(bool team, Vector4 color) {
	Write<Vector4>(outlineComp + (team ? 0x50 : 0x60), color);
}
void setOutline(bool update, bool visible)
{

	uintptr_t objectArray = Read<uintptr_t>(outlineComp + 0x328);
	uint32_t objectArraySize = Read<uint32_t>(outlineComp + 0x330);
	for (size_t offset = 0x0; offset < objectArraySize * 0x90; offset += 0x90)
	{
		Write<BYTE>(objectArray + offset + 0x0, visible);
		Write<BYTE>(objectArray + offset + 0x2, update);
	}
}
void outlineEsp(bool toggle) {

	if (toggle) {

		setOutlineColor(false, teamOutlineColor);
		setOutlineColor(true, enemyOutlineColor);
		setOutline(true, false);
		Write<BYTE>(outlineComp + 0x348, toggle);
	}
	else {

		Write<BYTE>(outlineComp + 0x348, toggle);

	}
}*/
float CurrentSpeed = -1.f;
HRESULT Hooks::PresentHooked(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT flags)
{
	//SEProtectStartMutation();
	static bool bInit = true;
	if (bInit)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)(&g_pDevice))))
		{
			g_pDevice->GetImmediateContext(&g_pContext);
		}
		//»ñÈ¡ºó»º³åÇøµØÖ·
		ID3D11Texture2D* renderTargetTexture = nullptr;
		if (SUCCEEDED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<PVOID*>(&renderTargetTexture))))
		{
			g_pDevice->CreateRenderTargetView(renderTargetTexture, NULL, &g_pRenderTargetView);
			renderTargetTexture->Release();
		}

		ImGuiIO& io = ImGui::GetIO();
		ImFontConfig config;
		config.MergeMode = true;
		io.Fonts->AddFontDefault();
		io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\simhei.ttf", 15.0f, &config, io.Fonts->GetGlyphRangesChinese());
		ImGui_ImplDX11_Init(G::GameWnd, g_pDevice, g_pContext);
		ImGui_ImplDX11_CreateDeviceObjects();
		InitMyMenu();
		bInit = false;
	}
	g_pContext->OMSetRenderTargets(1, &g_pRenderTargetView, NULL);
	ImGui_ImplDX11_NewFrame();
	MainWindows();
	if (I::Engine->IsInGame())
	{
		if (!G::LocalPlayer)
			G::LocalPlayer = I::Engine->GetLocalPlayer();
		if (Vars.Visuals.Enabled)
		{
			if(Vars.Visuals.PlayerEnable)
				PlayerEsp();
		}
		if(Vars.Aim.Enable)
			AimBot->DoAimbot();
		if (Vars.Misc.NoRecoil)
			G::LocalPlayer->ZeroRecoil();
		if (Vars.Misc.NoSpread)
			G::LocalPlayer->ZeroSpread();
		if (Vars.Misc.InstantReload)
			G::LocalPlayer->InstantReload();
		if (Vars.Misc.RapidFire)
			G::LocalPlayer->SetFireNum(Vars.Misc.RapidFireVal);
		if (Vars.Misc.MoreDmg)
			G::LocalPlayer->SetDamage(Vars.Misc.MoreDmgVal);
		if (CurrentSpeed == -1.f)
			CurrentSpeed = G::LocalPlayer->GetSpeed();
		
		if (Vars.Misc.SpeedHack && CurrentSpeed != -1.f)
		{
			G::LocalPlayer->SedSpeed(CurrentSpeed + Vars.Misc.SpeedHackVal);
		}
		else if(!Vars.Misc.SpeedHack && CurrentSpeed != -1.f)
		{
			G::LocalPlayer->SedSpeed(CurrentSpeed);
		}
	}
	else
	{
		G::LocalPlayer = nullptr;
		CurrentSpeed = -1.f;
	}
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return Present(pSwapChain, SyncInterval, flags);
}
// simple as fuck
void Hooks::InitHook()
{
	//DWORD64* pSwapChainVTable = (DWORD64*)(*(DWORD64*)(*(DWORD64*)(OFFSET::uDX11Base)));
	DWORD64 dwHookVT = OFFSET::uDX11Base + 0x40;
	ImGui::CreateContext();
	oWndProc = (WNDPROC)SetWindowLongPtr(G::GameWnd, GWLP_WNDPROC, (DWORD_PTR)Hooks::hkWndProc);
	Present = (tD3D11Present)HookVirtual((void*)dwHookVT, Hooks::PresentHooked);
}

void OpenMenu()
{
	static bool is_down = false;
	static bool is_clicked = false;

	if (G::PressedKeys[VK_INSERT])
	{
		is_clicked = false;
		is_down = true;
	}
	else if (!G::PressedKeys[VK_INSERT] && is_down)
	{
		is_clicked = true;
		is_down = false;
	}
	else
	{
		is_clicked = false;
		is_down = false;
	}

	if (is_clicked)
	{
		//	Vars.Menu.main = !Vars.Menu.main;
		Vars.Menu.Opened = !Vars.Menu.Opened;
	}
}
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT Hooks::hkWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	ImGuiIO& io = ImGui::GetIO();
	switch (uMsg)
	{
		case WM_SIZE:
		{

			break;
		}
		case WM_LBUTTONDOWN:
			io.MouseDown[0] = true;
			G::PressedKeys[VK_LBUTTON] = true;
			break;
		case WM_LBUTTONUP:
			io.MouseDown[0] = false;
			G::PressedKeys[VK_LBUTTON] = false;
			break;
		case WM_RBUTTONDOWN:
			io.MouseDown[1] = true;
			G::PressedKeys[VK_RBUTTON] = true;
			break;
		case WM_RBUTTONUP:
			io.MouseDown[1] = false;
			G::PressedKeys[VK_RBUTTON] = false;
			break;
		case WM_MBUTTONDOWN:
			io.MouseDown[2] = true;
			G::PressedKeys[VK_MBUTTON] = true;
			break;
		case WM_MBUTTONUP:
			io.MouseDown[2] = false;
			G::PressedKeys[VK_MBUTTON] = false;
			break;
		case WM_MOUSEWHEEL:
			io.MouseWheel += GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? 1.0f : -1.0f;
			break;
		case WM_MOUSEMOVE:
			io.MousePos.x = (signed short)(lParam);
			io.MousePos.y = (signed short)(lParam >> 16);
			break;
		case WM_KEYDOWN:
			G::PressedKeys[wParam] = true;
			if (wParam == 52)
				break;
			if (wParam < 256)
				io.KeysDown[wParam] = 1;
			break;
		case WM_KEYUP:
			G::PressedKeys[wParam] = false;
			if (wParam == 52)
				break;
			if (wParam < 256)
				io.KeysDown[wParam] = 0;
			break;
		case WM_XBUTTONDOWN:
		{
			UINT button = GET_XBUTTON_WPARAM(wParam);
			if (button == XBUTTON1)
			{
				G::PressedKeys[VK_XBUTTON1] = true;
			}
			else if (button == XBUTTON2)
			{
				G::PressedKeys[VK_XBUTTON2] = true;
			}
			break;
		}
		case WM_XBUTTONUP:
		{
			UINT button = GET_XBUTTON_WPARAM(wParam);
			if (button == XBUTTON1)
			{
				G::PressedKeys[VK_XBUTTON1] = false;
			}
			else if (button == XBUTTON2)
			{
				G::PressedKeys[VK_XBUTTON2] = false;
			}
			break;
		}
	}

	OpenMenu();
	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}