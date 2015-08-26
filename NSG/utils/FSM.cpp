/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

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
#include "FSM.h"
#include "Engine.h"

namespace NSG
{
    namespace FSM
    {
        Condition::Condition(State& newState)
            : pNewState_(&newState), conditionFunc_(nullptr)
        {
        }

        void Condition::When(CONDITION_FUNC conditionFunc)
        {
            conditionFunc_ = conditionFunc;
        }

        Condition::RESULT Condition::Evaluate()
        {
            if (conditionFunc_)
                return RESULT(conditionFunc_(), pNewState_);

            return RESULT(true, pNewState_);
        }

        State::State()
            : nextBeginState_(nullptr)
        {
        }

        State::~State()
        {
        }

        Condition& State::AddTransition(State& to)
        {
            Condition* pCondition(new Condition(to));
            conditions_.push_back(PCondition(pCondition));
            return *pCondition;
        }

        State* State::Evaluate()
        {
            if (nextBeginState_)
            {
                State* next = nextBeginState_;
                nextBeginState_ = nullptr;
                next->InternalBegin();
                return next;
            }
            else
            {
                for (auto& pCondition : conditions_)
                {
                    Condition::RESULT result = pCondition->Evaluate();
                    if (result.first)
                    {
                        InternalEnd();
                        nextBeginState_ = result.second;
                        return this;
                    }
                }
            }

            return this;
        }

        void State::InternalBegin()
        {
            Begin();
        }

        void State::InternalEnd()
        {
            End();
        }

        Machine::Machine()
            : pInitialState_(nullptr), pCurrentState_(nullptr)
        {

        }

        Machine::Machine(State& initialState, bool isFirstState)
            : pInitialState_(&initialState), pCurrentState_(&initialState)
        {
            if (isFirstState)
                pCurrentState_->InternalBegin();
        }

        Machine::~Machine()
        {
        }

        void Machine::SetInitialState(State& initialState)
        {
            pInitialState_ = &initialState;
            pCurrentState_ = &initialState;
            pCurrentState_->InternalBegin();
        }

        void Machine::Update()
        {
            State* pNewState = pCurrentState_->Evaluate();
            if (pNewState == pCurrentState_)
            {
                if (pCurrentState_->nextBeginState_ == nullptr)
                {
                    pCurrentState_->Stay();
                }
            }
            else
            {
                pCurrentState_ = pNewState;
            }
        }

        void Machine::InternalBegin()
        {
            Begin();
            pCurrentState_ = pInitialState_;
            pCurrentState_->InternalBegin();
        }

        void Machine::InternalEnd()
        {
            pCurrentState_->InternalEnd();
            End();
        }

        State* Machine::Evaluate()
        {
            State* pNewState = this->State::Evaluate();

            if (pNewState == this)
                Update();

            return pNewState;
        }

        void Machine::Go()
        {
            slotUpdate_ = Engine::SigUpdate()->Connect([this](float deltaTime)
            {
                Update();
            });
        }
    }
}