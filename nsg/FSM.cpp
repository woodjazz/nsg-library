/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://nsg-library.googlecode.com/

Copyright (c) 2013-2014 N�stor Silveira Gorski

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
#include "FSM.h"

namespace nsg {

    namespace FSM {

        Condition::Condition(State& newState)
            : newState_(newState), conditionFunc_(nullptr) {
        }

        void Condition::When(CONDITION_FUNC conditionFunc) {
            conditionFunc_ = conditionFunc;
        }

        Condition::RESULT Condition::Evaluate() {
            if(conditionFunc_)
                return RESULT(conditionFunc_(), &newState_);
            return RESULT(true, &newState_);
        }

        State::State() {
        }

        State::~State() {
        }

        PCondition State::AddTransition(State& to) {
            PCondition pCondition(new Condition(to));
            conditions_.push_back(pCondition);
            return pCondition;
        }

        State* State::Evaluate() {
            for (auto &pCondition : conditions_) {
                Condition::RESULT result = pCondition->Evaluate();
                if(result.first) {
                    InternalEnd();
                    result.second->InternalBegin();
                    return result.second;
                }
            }

            return this;
        }

        void State::InternalBegin() {
            Begin();
        }

        void State::InternalEnd() {
            End();
        }


        Machine::Machine(State& initialState, bool isFirstState) 
            : pInitialState_(&initialState), 
            pCurrentState_(&initialState) {
                if(isFirstState) {
                    pCurrentState_->InternalBegin();
                }
        }

        Machine::~Machine() {
        }

        void Machine::Update() {
            State* pNewState = pCurrentState_->Evaluate();
            if(pNewState == pCurrentState_)
                pCurrentState_->Stay();
            else
                pCurrentState_ = pNewState;
        }

        void Machine::InternalBegin() {
            Begin();
            pCurrentState_ = pInitialState_;
            pCurrentState_->InternalBegin();
        }

        void Machine::InternalEnd() {
            pCurrentState_->InternalEnd();
            End();
        }


        State* Machine::Evaluate() {
            State* pNewState = this->State::Evaluate();
            if(pNewState == this) {
                Update();
            }
            return pNewState;
        }
    }
}
