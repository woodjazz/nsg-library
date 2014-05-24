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

#include <memory>
#include <functional>
#include <vector>
#include "NonCopyable.h"

namespace NSG 
{

    namespace FSM 
    {

        class State;
        class Condition : NonCopyable
        {
        public:
            typedef std::function<bool()> CONDITION_FUNC;
            void When(CONDITION_FUNC conditionFunc);
        private:
            Condition(State& newState);
            typedef std::pair<bool, State*> RESULT;
            RESULT Evaluate();

            State* pNewState_;
            CONDITION_FUNC conditionFunc_;
            friend class State;
        };

        class State : NonCopyable
        {
        public:
            State();
            virtual ~State();
            Condition& AddTransition(State& to);
            virtual const State* GetState() const { return this; }
        protected:
            virtual void Begin(){}
            virtual void Stay(){}
            virtual void End(){}
        private:
            virtual State* Evaluate();
            virtual void InternalBegin();
            virtual void InternalEnd();

            typedef std::unique_ptr<Condition> PCondition;
            std::vector<PCondition> conditions_;
            State* nextBeginState_;
            friend class Machine;
        };

        class Machine : public State 
        {
        public:
            Machine(State& initialState, bool isFirstState = true);
            virtual ~Machine();
            void Update();
            const State* GetState() const { return pCurrentState_; }
        private:
            void InternalBegin();
            void InternalEnd();
            State* Evaluate();

            State* pInitialState_;
            State* pCurrentState_;
        };
    }
}
