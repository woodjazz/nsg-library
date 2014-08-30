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
#include "BoundingBox.h"
#include <vector>

namespace NSG
{
    static const unsigned M_MAX_UNSIGNED = 0xffffffff;
    class Octree;
    static const int NUM_OCTANTS = 8;
    static const unsigned ROOT_INDEX = M_MAX_UNSIGNED;
	class OctreeQuery;
    class Octant
    {
    public:
        Octant(const BoundingBox& box, unsigned level, Octant* parent, Octree* root, unsigned index = ROOT_INDEX);
        virtual ~Octant();
        void InsertDrawable(const SceneNode* drawable);
	protected:
        void Initialize(const BoundingBox& box);
        Octant* GetOrCreateChild(unsigned index);
        void DeleteChild(unsigned index);
        void ResetRoot();
        bool CheckDrawableFit(const BoundingBox& box) const;
        void AddDrawable(const SceneNode* drawable);
        void RemoveDrawable(const SceneNode* drawable, bool resetOctant = true);
        void IncDrawableCount();
        void DecDrawableCount();
        const BoundingBox& GetCullingBox() const { return cullingBox_; }
        Octree* GetRoot() const { return root_; }
        void GetDrawablesInternal(OctreeQuery& query, bool inside) const;
    private:
        /// World bounding box.
        BoundingBox worldBoundingBox_;
        /// Bounding box used for drawable object fitting.
        BoundingBox cullingBox_;
        /// Drawable objects.
        std::vector<const SceneNode*> drawables_;
        /// Child octants.
        Octant* children_[NUM_OCTANTS];
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

    class Octree : public Octant
    {
    public:
        Octree();
        ~Octree();
        unsigned GetNumLevels() const { return numLevels_; }
        void Update(const SceneNode* drawable);
        void GetDrawables(OctreeQuery& query) const;
    private:
        // Subdivision level.
        unsigned numLevels_;
    };
}