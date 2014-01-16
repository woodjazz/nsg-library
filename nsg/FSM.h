/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://nsg-library.googlecode.com/

Copyright (c) 2013-2014 Néstor Silveira Gorski

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

namespace nsg {

    namespace FSM {

        class State;
        class Condition {
        public:
            typedef std::function<bool()> CONDITION_FUNC;
            void When(CONDITION_FUNC conditionFunc);
        private:
            Condition(State& newState);
            Condition(const Condition&);
            Condition& operator=(const Condition&);
            typedef std::pair<bool, State*> RESULT;
            RESULT Evaluate();

            State& newState_;
            CONDITION_FUNC conditionFunc_;
            friend class State;
        };

        typedef std::shared_ptr<Condition> PCondition;

        class State {
        public:
            State();
            virtual ~State();
            PCondition AddTransition(State& to);
            virtual const State* GetState() const { return this; }
        protected:
            virtual void Begin(){}
            virtual void Stay(){}
            virtual void End(){}
        private:
            State(const State&);
            State& operator=(const State&);
            virtual State* Evaluate();
            virtual void InternalBegin();
            virtual void InternalEnd();
            std::vector<PCondition> conditions_;
            friend class Machine;
        };

        class Machine : public State {
        public:
            Machine(State& initialState, bool isFirstState = true);
            virtual ~Machine();
            void Update();
            const State* GetState() const { return pCurrentState_; }
        private:
            Machine(const Machine&);
            Machine& operator=(const Machine&);
            void InternalBegin();
            void InternalEnd();
            State* Evaluate();

            State* pInitialState_;
            State* pCurrentState_;
        };
    }
}




