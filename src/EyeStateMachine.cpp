#include "EyeStateMachine.h"

#include "EyeStates.h"

EyeStateMachine::EyeStateMachine(Eye3dModel& _eyeModel)
  : eyeModel(_eyeModel)
{
   Add(new StateClosed(*this));
   Add(new StateClosing(*this));
   Add(new StateOpen(*this));
   Add(new StateOpening(*this));

   currentState = states["Closed"];
}

void EyeStateMachine::Update(const float deltaT, std::optional<Vector3> facePosition)
{
   currentState->Update(deltaT, facePosition);
}

void EyeStateMachine::Switch(const std::string &newStateName)
{
   State* newState = states[newStateName];

   currentState->Exit();
   newState->Enter();
   currentState = newState;
}

Eye3dModel &EyeStateMachine::GetEyeModel()
{
   return eyeModel;
}

void EyeStateMachine::Add(State *newState)
{
   const std::string stateName = newState->GetName();
   states[stateName] = newState;
}

