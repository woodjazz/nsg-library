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
#include "AppStatistics.h"
#include "AppConfiguration.h"
#include "App.h"
#include "Node.h"
#include "Pass.h"
#include "TextMesh.h"
#include "Material.h"
#include "Graphics.h"
#include "App.h"

namespace NSG
{
    template <> AppStatistics* Singleton<AppStatistics>::this_ = nullptr;

    AppStatistics::AppStatistics()
        : app_(*App::this_),
          frames_(0),
          collect_(true),
          pass_(new Pass),
          material_(app_.CreateMaterial("AppStatistics"))
    {
		
        startTime_ = Clock::now();

        pass_->EnableDepthTest(false);
        pass_->EnableStencilTest(false);

        material_->SetColor(Color(1, 1, 1, 1));
        material_->SetSerializable(false);

        for (int i = 0; i < (int)Stats::MAX_STATS; i++)
        {
            stats_[i] = 0;
            text_[i] = app_.CreateTextMesh();
            std::stringstream ss;
            ss << "AppStatistics::node " << i;
            node_[i] = PNode(new Node(ss.str()));
            if (i > 0)
                node_[i - 1]->AddChild(node_[i]);
        }

        pass_->SetProgram(text_[0]->GetProgram());
        material_->SetTexture0(text_[0]->GetTexture());

        node_[0]->SetPosition(Vertex3(-1, 1, 0));

        label_[(int)Stats::FPS] = "Fps:";
        label_[(int)Stats::DRAW_CALLS] = "Draw calls:";
        label_[(int)Stats::TRIANGLES] = "Triangles:";
        label_[(int)Stats::STATIC_VBO] = "Static VBO:";
        label_[(int)Stats::STATIC_IBO] = "Static IBO:";
        label_[(int)Stats::DYNAMIC_VBO] = "Dynamic VBO:";
        label_[(int)Stats::DYNAMIC_IBO] = "Dynamic IBO:";
        label_[(int)Stats::VISIBLES_NODES] = "Visibles nodes:";
        label_[(int)Stats::TOTAL_NODES] = "Total nodes::";
    }

    AppStatistics::~AppStatistics()
    {
        AppStatistics::this_ = nullptr;
    }

    void AppStatistics::NewDrawCall()
    {
        if (collect_)
            ++stats_[(int)Stats::DRAW_CALLS];
    }

    void AppStatistics::NewTriangles(size_t n)
    {
        if (collect_)
            stats_[(int)Stats::TRIANGLES] += n;
    }

    void AppStatistics::SetNodes(size_t total, size_t visibles)
    {
        stats_[(int)Stats::VISIBLES_NODES] = visibles;
        stats_[(int)Stats::TOTAL_NODES] = total;
    }

    void AppStatistics::NewFrame()
    {
        Milliseconds ms = std::chrono::duration_cast<Milliseconds>(Clock::now() - startTime_);
        if (ms.count() >= 1000)
        {
            startTime_ = Clock::now();
            stats_[(int)Stats::FPS] = frames_;
            frames_ = 1;
        }
        else
        {
            ++frames_;
        }

        stats_[(int)Stats::TRIANGLES] = 0;
        stats_[(int)Stats::DRAW_CALLS] = 0;
    }


    void AppStatistics::AddVertexBuffer(bool dynamic)
    {
        if (dynamic)
            ++stats_[(int)Stats::DYNAMIC_VBO];
        else
            ++stats_[(int)Stats::STATIC_VBO];
    }

    void AppStatistics::AddIndexBuffer(bool dynamic)
    {
        if (dynamic)
            ++stats_[(int)Stats::DYNAMIC_IBO];
        else
            ++stats_[(int)Stats::STATIC_IBO];
    }

    void AppStatistics::RemoveVertexBuffer(bool dynamic)
    {
        if (dynamic)
            --stats_[(int)Stats::DYNAMIC_VBO];
        else
            --stats_[(int)Stats::STATIC_VBO];
    }

    void AppStatistics::RemoveIndexBuffer(bool dynamic)
    {
        if (dynamic)
            --stats_[(int)Stats::DYNAMIC_IBO];
        else
            --stats_[(int)Stats::STATIC_IBO];
    }

    void AppStatistics::Show()
    {
        stats_[(int)Stats::DYNAMIC_VBO] -= (int)Stats::MAX_STATS;
        stats_[(int)Stats::DYNAMIC_IBO] -= (int)Stats::MAX_STATS;

        collect_ = false;

        Graphics::this_->Set(material_.get());

        std::stringstream ss;
        for (int i = 0; i < (int)Stats::MAX_STATS; i++)
        {
            Graphics::this_->SetNode(node_[i].get());
            Graphics::this_->Set(text_[i].get());

            if (i > 0)
                node_[i]->SetPosition(Vertex3(0, -text_[i - 1]->GetHeight(), 0));

            ss << stats_[i];
            text_[i]->SetText(label_[i] + ss.str(), LEFT_ALIGNMENT, TOP_ALIGNMENT);
            ss.str("");

            pass_->Render();
        }

        collect_ = true;

        stats_[(int)Stats::DYNAMIC_VBO] += (int)Stats::MAX_STATS;
        stats_[(int)Stats::DYNAMIC_IBO] += (int)Stats::MAX_STATS;

    }
}
