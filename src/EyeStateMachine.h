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

   void Switch(const std::string& newStateName);
   Eye3dModel& GetEyeModel();

private:
   void Add(State* newState);

   std::unordered_map<std::string, State*> states;
   State* currentState;
   Eye3dModel& eyeModel;
};

#endif // EYESTATEMACHINE_H
