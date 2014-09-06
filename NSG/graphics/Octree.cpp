//
// Copyright (c) 2008-2014 the Urho3D project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// Updated for nsg-library
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

    void Octant::Insert(SceneNode* obj)
    {
        const BoundingBox& box = obj->GetWorldBoundingBox();

        // If root octant, insert all non-occludees here, so that octant occlusion does not hide the drawable.
        // Also if drawable is outside the root octant bounds, insert to root
        bool insertHere;
        if (this == root_)
            insertHere = !obj->IsOccludee() || cullingBox_.IsInside(box) != Intersection::INSIDE || CheckFit(box);
        else
            insertHere = CheckFit(box);

        if (insertHere)
        {
            Octant* oldOctant = obj->GetOctant();
            if (oldOctant != this)
            {
                // Add first, then remove, because drawable count going to zero deletes the octree branch in question
                Add(obj);
                if (oldOctant)
                    oldOctant->Remove(obj, false);
            }
        }
        else
        {
            Vector3 boxCenter = box.Center();
            unsigned x = boxCenter.x < center_.x ? 0 : 1;
            unsigned y = boxCenter.y < center_.y ? 0 : 2;
            unsigned z = boxCenter.z < center_.z ? 0 : 4;

            GetOrCreateChild(x + y + z)->Insert(obj);
        }
    }

    bool Octant::CheckFit(const BoundingBox& box) const
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


    void Octant::Add(SceneNode* obj)
    {
        obj->SetOctant(this);
        drawables_.push_back(obj);
        IncDrawableCount();
    }

    void Octant::Remove(SceneNode* obj, bool resetOctant)
    {
        auto it = std::find(drawables_.begin(), drawables_.end(), obj);
        if (it != drawables_.end())
        {
            drawables_.erase(it);
            if (resetOctant)
                obj->SetOctant(nullptr);
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

    void Octant::ExecuteInternal(OctreeQuery& query, bool inside)
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
            query.Test(drawables_, inside);
        }

        for (unsigned i = 0; i < NUM_OCTANTS; ++i)
        {
            if (children_[i])
                children_[i]->ExecuteInternal(query, inside);
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

    void Octree::InsertUpdate(SceneNode* obj)
    {
         Octant* octant = obj->GetOctant();

        const BoundingBox& box = obj->GetWorldBoundingBox();
        // Skip if still fits the current octant
        if (octant && obj->IsOccludee() && octant->GetCullingBox().IsInside(box) == Intersection::INSIDE && octant->CheckFit(box))
            return;

        Insert(obj);

        // Verify that the drawable will be culled correctly
		CHECK_ASSERT((octant = obj->GetOctant()) && (octant == this || octant->GetCullingBox().IsInside(box) == Intersection::INSIDE), __FILE__, __LINE__);
    }

    void Octree::Remove(SceneNode* obj)
    {
        Octant* octant = obj->GetOctant();
        if(octant)
            octant->Remove(obj);
    }

    void Octree::Execute(OctreeQuery& query)
    {
        query.result_.clear();
        ExecuteInternal(query, false);
    }
}