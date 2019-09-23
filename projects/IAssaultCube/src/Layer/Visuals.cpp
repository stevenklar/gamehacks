#include "pch.h"
#include "Visuals.h"

bool Visuals::OnAttach()
{
    process.Attach(GetCurrentProcessId());

    if (process.valid())
    {
        /*
        auto& core = process.core();
        auto& memory = process.memory();
        auto& module = process.modules();
        
        auto mainMod = module.GetMainModule();

        std::cout << "[+] BlackBone attached" << std::endl;
        */

        std::cout << "[+] GetModuleHandle for opengl32" << std::endl;
        HMODULE hOpenglModule = GetModuleHandle(L"opengl32.dll");

        std::cout << "[+] Find wglSwapBuffers " << std::endl;
        auto wglSwapBuffersAddr = GetProcAddress((HMODULE)hOpenglModule, "wglSwapBuffers");
        DWORD o_wglSwapBuffers = reinterpret_cast<DWORD>(wglSwapBuffersAddr);

        std::cout << "[+] Hook wglSwapBuffers" << std::endl;
        if (d_wglSwapBuffers.Hook((int(__stdcall*)(HDC))o_wglSwapBuffers, (int(__stdcall*)(HDC&))h_wglSwapBuffers, HookType::Inline))
        {
            std::cout << "[+] Inline hooked wglSwapBuffers\n";
            return true;
        }
        else
        {
            std::cout << "[!] Failed to Hook: wglSwapBuffers\n";
            return false;
        }
    }
    else
    {
        std::cout << "[!] Not able to attach BlackBone" << std::endl;
        return false;
    }
}

bool Visuals::OnUpdate()
{
    if (GetAsyncKeyState(VK_F9) & 1)
    {
        // panic
        // should be implemented somewhere else instead, e. g. InputLayer
        return false;
    }

    return true;
};

bool Visuals::OnDetach()
{
    if (process.valid())
    {
        if (d_wglSwapBuffers.Restore())
        {
            std::cout << "[+] Restored wglSwapBuffers" << std::endl;
        }
        else
        {
            std::cout << "[!] Failed to restore wglSwapBuffers" << std::endl;
        }

        if (!process.Detach())
            std::cout << "[!] Not able to detach BlackBone" << std::endl;
    }
    else
    {
        std::cout << "[!] No attached BlackBone found" << std::endl;
    }

    std::cout << "[+] Free Console" << std::endl;
    FreeConsole();
}

BOOL __stdcall Visuals::h_wglSwapBuffers(HDC h)
{
	SetupOrtho();
	Draw();
	RestoreGL();
	return d_wglSwapBuffers.CallOriginal(std::move(h));
}

void Visuals::Draw()
{
	Game game;
	playerent* pLocalPlayer = game.GetLocalPlayer();
	playerent** entityList = game.GetEntityList();

	for (int i = 0; i < game.GetNumberOfPlayers(); i++)
	{
		playerent* pPlayer = entityList[i];
		DrawESP(pPlayer, pLocalPlayer);
	}
}

void Visuals::ESP(playerent* pTargetPlayer, playerent* pLocalPlayer)
{
	if (!IsValidPtr(pTargetPlayer))
		return;

	if (pTargetPlayer == pLocalPlayer)
		return;

	if (pTargetPlayer->m_Health <= 0 || pTargetPlayer->m_Health > 100)
		return;

	if (pTargetPlayer->m_Head.x == 0 && pTargetPlayer->m_Head.y == 0 && pTargetPlayer->m_Head.z == 0)
		return;

	Vec3 vHeadPos = pTargetPlayer->m_Head;
	Vec3 vFootPos = pTargetPlayer->m_Position;

	vHeadPos.z += 0.8F; //Regular head pos is not high enough

	Vec3 vScreenHead, vScreenFoot;
	if (OpenGL::WorldToScreen(vHeadPos, &vScreenHead) && OpenGL::WorldToScreen(vFootPos, &vScreenFoot))
	{
		double flHeight = abs(vScreenFoot.y - vScreenHead.y);
		double flWidth = flHeight / 2.0F;

		RenderFunctions::Color32 color = IsVisible(pLocalPlayer->m_Head, pTargetPlayer->m_Head) ? RenderFunctions::Color32(1.0F, 1.0F, 0.0F, 1.0F) : RenderFunctions::Color32(1.0F, 0.0F, 0.0F, 1.0F);

		OpenGL::DrawRect((int)(vScreenHead.x - flWidth / 2), (int)vScreenHead.y, (int)(vScreenHead.x + flWidth / 2), (int)(vScreenHead.y + flHeight), color, false);
		OpenGL::DrawHealthBar(vScreenHead.x - flWidth / 2, vScreenHead.y - 14, flWidth, 6.0F, static_cast <float>(static_cast <float>(pTargetPlayer->m_Health) / 100.0F), RenderFunctions::Color32(0.0F, 1.0F, 0.0F, 1.0F));
		//OpeNGL::DrawString(vScreenFoot.x - flWidth / 2, vScreenFoot.y + 4, static_cast <int>(color.r * 255), static_cast <int>(color.g * 255), static_cast <int>(color.b * 255), pTargetPlayer->m_Name);
	}
}