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
// Updated by Néstor Silveira Gorski for nsg-library

#pragma once
#include "BoundingBox.h"
#include "Types.h"
#include <array>
#include <set>
#include <vector>

namespace NSG {
static const unsigned M_MAX_UNSIGNED = 0xffffffff;
class Octree;
static const int NUM_OCTANTS = 8;
static const unsigned ROOT_INDEX = M_MAX_UNSIGNED;
class OctreeQuery;
class Octant {
public:
    Octant(const BoundingBox& box, unsigned level, Octant* parent, Octree* root,
           unsigned index = ROOT_INDEX);
    virtual ~Octant();

protected:
    void Insert(SceneNode* obj);
    void Initialize(const BoundingBox& box);
    Octant* GetOrCreateChild(unsigned index);
    void DeleteChild(unsigned index);
    void ResetRoot();
    bool CheckFit(const BoundingBox& box) const;
    void Add(SceneNode* obj);
    void Remove(SceneNode* obj, bool resetOctant = true);
    void IncDrawableCount();
    void DecDrawableCount();
    const BoundingBox& GetCullingBox() const { return cullingBox_; }
    Octree* GetRoot() const { return root_; }
    void ExecuteInternal(OctreeQuery& query, bool inside);

private:
    /// World bounding box.
    BoundingBox worldBoundingBox_;
    /// Bounding box used for drawable object fitting.
    BoundingBox cullingBox_;
    /// Drawable objects.
    std::vector<SceneNode*> drawables_;
    /// Child octants.
    std::array<Octant*, NUM_OCTANTS> children_;
    /// World bounding box center.
    Vector3 center_;
    /// World bounding box half size.
    Vector3 halfSize_;
    /// Subdivision level.
    unsigned level_;
    /// Number of drawable objects in this octant and child octants.
    unsigned numDrawables_;
    /// Parent octant.
    Octant* parent_;
    /// Octree root.
    Octree* root_;
    /// Octant index relative to its siblings or ROOT_INDEX for root octant
    unsigned index_;
    friend class Octree;
};

class Octree : public Octant {
public:
    Octree();
    ~Octree();
    unsigned GetNumLevels() const { return numLevels_; }
    void InsertUpdate(SceneNode* obj);
    void Remove(SceneNode* obj);
    void Execute(OctreeQuery& query);
    unsigned GetNumDrawables() const { return numDrawables_; }
    const std::vector<SceneNode*>& GetDrawables() const {
        return allDrawables_;
    }

private:
    unsigned numLevels_; // Subdivision level.
    std::vector<SceneNode*> allDrawables_;
    std::set<SceneNode*> allDrawablesSet_;
};
}