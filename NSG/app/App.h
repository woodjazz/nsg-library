/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://nsg-library.googlecode.com/

Copyright (c) 2014-2015 Néstor Silveira Gorski

-------------------------------------------------------------------------------
This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/
#pragma once

#include "Types.h"
#include "Singleton.h"
#include "MapAndVector.h"
#include "SignalSlots.h"
#include <string>
#include <vector>
#include <set>
#include <mutex>

namespace NSG
{
	class Object;
    class App : public Singleton<App>
    {
    public:
        App();
        ~App();
        void InitializeGraphics();
        void AddObject(Object* object);
        void RemoveObject(Object* object);
        PTexture GetWhiteTexture();
        PWindow GetOrCreateWindow(const std::string& name, int x, int y, int width, int height);
        PBoxMesh CreateBoxMesh(float width = 2, float height = 2, float depth = 2, int resX = 2, int resY = 2, int resZ = 2);
        PCircleMesh CreateCircleMesh(float radius = 1, int res = 8);
        PEllipseMesh CreateEllipseMesh(float width = 2, float height = 1, int res = 8);
        PModelMesh GetOrCreateModelMesh(const std::string& name);
        PPlaneMesh CreatePlaneMesh(float width = 2, float height = 2, int columns = 2, int rows = 2);
        PRectangleMesh CreateRectangleMesh(float width = 2, float height = 1);
        PRoundedRectangleMesh CreateRoundedRectangleMesh(float radius = 1, float width = 2, float height = 2, int res = 8);
        PSphereMesh CreateSphereMesh(float radius = 1, int res = 8);
        PMaterial CreateMaterial(const std::string& name, const ProgramFlags& flags = (int)ProgramFlag::NONE);
        PMaterial GetOrCreateMaterial(const std::string& name, const ProgramFlags& flags = (int)ProgramFlag::NONE);
        PMaterial GetMaterial(const std::string& name);
        const std::vector<PMesh>& GetMeshes() const;
        PMesh GetMesh(const std::string& name) const;
        const std::vector<PMaterial>& GetMaterials() const;
        int GetMaterialSerializableIndex(const PMaterial& material) const;
        int GetMeshSerializableIndex(const PMesh& mesh) const;
        int Run();
        void SetMainWindow(Window* window);
		Window* GetMainWindow() const { return mainWindow_; }
		std::vector<PWeakWindow>& GetWindows() { return windows_; }
		void NotifyOneWindow2Remove() { ++nWindows2Remove_;  }
    private:
		static void RenderFrame(void* data);
        bool RenderFrame();
		void ClearAll();
        void HandleEvents();
        bool GetWindow(unsigned windowID, Window*& window, int& width, int& height);
        bool GetWindow(unsigned windowID, Window*& window);
        std::vector<PWeakWindow> windows_;
        PAppConfiguration configuration_;
        MapAndVector<std::string, Mesh> meshes_;
        MapAndVector<std::string, Material> materials_;
        PGraphics graphics_;
        PAppStatistics statistics_;
        PKeyboard keyboard_;
		std::set<Object*> objects_;
		PTexture whiteTexture_;
        int nWindows2Remove_;
		Window* mainWindow_;
		std::once_flag onceFlag_;
	};
}
