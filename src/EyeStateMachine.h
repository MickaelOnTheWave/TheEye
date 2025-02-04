#ifndef EYESTATEMACHINE_H
#define EYESTATEMACHINE_H

#include <optional>
#include <unordered_map>

#include "Eye3dModel.h"
#include "math/Vector3.h"

class State;

class EyeStateMachine
{
public:
   EyeStateMachine(Eye3dModel& _eyeModel);

   void Update(const float deltaT, std::optional<Vector3> facePosition);

   Eye3dModel& GetEyeModel();

private:
   void Add(State* newState);
   void Switch(const std::string& newStateName);

   std::unordered_map<std::string, State*> states;
   State* currentState;
   Eye3dModel& eyeModel;
};

#endif // EYESTATEMACHINE_H
