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
#include "Octree.h"
#include "OctreeQuery.h"
#include "SceneNode.h"
namespace NSG
{

    Octant::Octant(const BoundingBox& box, unsigned level, Octant* parent, Octree* root, unsigned index) :
        level_(level),
        numDrawables_(0),
        parent_(parent),
        root_(root),
        index_(index)
    {
        Initialize(box);

        for (unsigned i = 0; i < NUM_OCTANTS; ++i)
            children_[i] = 0;
    }

    Octant::~Octant()
    {
        if (root_)
        {
            // Remove the drawables (if any) from this octant to the root octant
            for (auto& obj : drawables_)
            {
                obj->SetOctant(root_);
                root_->drawables_.push_back(obj);
            }
            drawables_.clear();
            numDrawables_ = 0;
        }

        for (unsigned i = 0; i < NUM_OCTANTS; ++i)
            DeleteChild(i);
    }

    void Octant::Initialize(const BoundingBox& box)
    {
        worldBoundingBox_ = box;
        center_ = box.Center();
        halfSize_ = 0.5f * box.Size();
        cullingBox_ = BoundingBox(worldBoundingBox_.min_ - halfSize_, worldBoundingBox_.max_ + halfSize_);
    }

    Octant* Octant::GetOrCreateChild(unsigned index)
    {
        if (children_[index])
            return children_[index];

        Vector3 newMin = worldBoundingBox_.min_;
        Vector3 newMax = worldBoundingBox_.max_;
        Vector3 oldCenter = worldBoundingBox_.Center();

        if (index & 1)
            newMin.x = oldCenter.x;
        else
            newMax.x = oldCenter.x;

        if (index & 2)
            newMin.y = oldCenter.y;
        else
            newMax.y = oldCenter.y;

        if (index & 4)
            newMin.z = oldCenter.z;
        else
            newMax.z = oldCenter.z;

        children_[index] = new Octant(BoundingBox(newMin, newMax), level_ + 1, this, root_, index);
        return children_[index];
    }


    void Octant::DeleteChild(unsigned index)
    {
        CHECK_ASSERT(index < NUM_OCTANTS, __FILE__, __LINE__);
        delete children_[index];
        children_[index] = nullptr;
    }

    void Octant::ResetRoot()
    {
        root_ = nullptr;

        // The whole octree is being destroyed, just detach the drawables
        for (auto& obj : drawables_)
            obj->SetOctant(0);

        for (unsigned i = 0; i < NUM_OCTANTS; ++i)
        {
            if (children_[i])
                children_[i]->ResetRoot();
        }
    }

    void Octant::InsertDrawable(const SceneNode* drawable)
    {
        const BoundingBox& box = drawable->GetWorldBoundingBox();

        // If root octant, insert all non-occludees here, so that octant occlusion does not hide the drawable.
        // Also if drawable is outside the root octant bounds, insert to root
        bool insertHere;
        if (this == root_)
            insertHere = !drawable->IsOccludee() || cullingBox_.IsInside(box) != Intersection::INSIDE || CheckDrawableFit(box);
        else
            insertHere = CheckDrawableFit(box);

        if (insertHere)
        {
            Octant* oldOctant = drawable->GetOctant();
            if (oldOctant != this)
            {
                // Add first, then remove, because drawable count going to zero deletes the octree branch in question
                AddDrawable(drawable);
                if (oldOctant)
                    oldOctant->RemoveDrawable(drawable, false);
            }
        }
        else
        {
            Vector3 boxCenter = box.Center();
            unsigned x = boxCenter.x < center_.x ? 0 : 1;
            unsigned y = boxCenter.y < center_.y ? 0 : 2;
            unsigned z = boxCenter.z < center_.z ? 0 : 4;

            GetOrCreateChild(x + y + z)->InsertDrawable(drawable);
        }
    }

    bool Octant::CheckDrawableFit(const BoundingBox& box) const
    {
        Vector3 boxSize = box.Size();

        // If max split level, size always OK, otherwise check that box is at least half size of octant
        if (level_ >= root_->GetNumLevels() || boxSize.x >= halfSize_.x || boxSize.y >= halfSize_.y ||
                boxSize.z >= halfSize_.z)
            return true;
        // Also check if the box can not fit a child octant's culling box, in that case size OK (must insert here)
        else
        {
            if (box.min_.x <= worldBoundingBox_.min_.x - 0.5f * halfSize_.x ||
                    box.max_.x >= worldBoundingBox_.max_.x + 0.5f * halfSize_.x ||
                    box.min_.y <= worldBoundingBox_.min_.y - 0.5f * halfSize_.y ||
                    box.max_.y >= worldBoundingBox_.max_.y + 0.5f * halfSize_.y ||
                    box.min_.z <= worldBoundingBox_.min_.z - 0.5f * halfSize_.z ||
                    box.max_.z >= worldBoundingBox_.max_.z + 0.5f * halfSize_.z)
                return true;
        }

        // Bounding box too small, should create a child octant
        return false;
    }


    void Octant::AddDrawable(const SceneNode* drawable)
    {
        drawable->SetOctant(this);
        drawables_.push_back(drawable);
        IncDrawableCount();
    }

    void Octant::RemoveDrawable(const SceneNode* drawable, bool resetOctant)
    {
        auto it = std::find(drawables_.begin(), drawables_.end(), drawable);
        if (it != drawables_.end())
        {
            drawables_.erase(it);
            if (resetOctant)
                drawable->SetOctant(nullptr);
            DecDrawableCount();
        }
    }

    void Octant::IncDrawableCount()
    {
        ++numDrawables_;
        if (parent_)
            parent_->IncDrawableCount();
    }

    void Octant::DecDrawableCount()
    {
        Octant* parent = parent_;

        --numDrawables_;
        if (!numDrawables_)
        {
            if (parent)
                parent->DeleteChild(index_);
        }

        if (parent)
            parent->DecDrawableCount();
    }

    void Octant::GetDrawablesInternal(OctreeQuery& query, bool inside) const
    {
        if (this != root_)
        {
            Intersection res = query.TestOctant(cullingBox_, inside);
			if (res == Intersection::INSIDE)
                inside = true;
			else if (res == Intersection::OUTSIDE)
            {
                // Fully outside, so cull this octant, its children & drawables
                return;
            }
        }

        if (drawables_.size())
        {
            query.TestDrawables(drawables_.begin(), drawables_.end(), inside);
        }

        for (unsigned i = 0; i < NUM_OCTANTS; ++i)
        {
            if (children_[i])
                children_[i]->GetDrawablesInternal(query, inside);
        }
    }



    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    static const float DEFAULT_OCTREE_SIZE = 1000.0f;
    static const int DEFAULT_OCTREE_LEVELS = 8;

    Octree::Octree() :
        Octant(BoundingBox(-DEFAULT_OCTREE_SIZE, DEFAULT_OCTREE_SIZE), 0, nullptr, this),
        numLevels_(DEFAULT_OCTREE_LEVELS)
    {
    }

    Octree::~Octree()
    {
        ResetRoot();
    }

    void Octree::Update(const SceneNode* drawable)
    {
        Octant* octant = drawable->GetOctant();

        // Skip if no octant or does not belong to this octree anymore
        if (!octant || octant->GetRoot() != this)
            return;

        const BoundingBox& box = drawable->GetWorldBoundingBox();
        // Skip if still fits the current octant
        if (drawable->IsOccludee() && octant->GetCullingBox().IsInside(box) == Intersection::INSIDE && octant->CheckDrawableFit(box))
            return;

        InsertDrawable(drawable);

#ifdef _DEBUG
        // Verify that the drawable will be culled correctly
        octant = drawable->GetOctant();
        if (octant != this && octant->GetCullingBox().IsInside(box) != Intersection::INSIDE)
        {
            TRACE_LOG("Drawable is not fully inside its octant's culling bounds: drawable box " << box <<
                      " octant box " << octant->GetCullingBox());
        }
#endif
    }

    void Octree::GetDrawables(OctreeQuery& query) const
    {
        query.result_.clear();
        GetDrawablesInternal(query, false);
    }
}