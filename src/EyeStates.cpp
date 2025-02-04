#include "EyeStates.h"

#include <iostream>

using namespace std;

/******************************/

State::State(EyeStateMachine &_stateMachine)
  : stateMachine(_stateMachine)
{
}

void State::Enter()
{
}

void State::Exit()
{
}

/******************************/

AnimatedState::AnimatedState(EyeStateMachine &_stateMachine)
  : State(_stateMachine)
{
}

void AnimatedState::Enter()
{
   animationT = 0.f;
}

optional<string> AnimatedState::Update(const float deltaT, optional<Vector3> facePosition)
{
   animationT += deltaT;

   if (animationT > animationFinishT)
   {
      animationT = animationFinishT;
      return make_optional(animationFinishedState);
   }
   return nullopt;
}

/******************************/

StateClosed::StateClosed(EyeStateMachine& _stateMachine)
  : State(_stateMachine)
{
}

void StateClosed::Enter()
{
   faceVisibleT = 0.f;
}

optional<string> StateClosed::Update(const float deltaT, optional<Vector3> facePosition)
{
   if (facePosition)
      faceVisibleT += deltaT;
   else
      faceVisibleT = 0.f;

   if (faceVisibleT > openThresholdT)
      return make_optional("Opening");
   return nullopt;
}

std::string StateClosed::GetName() const
{
   return "Closed";
}

/******************************/

StateClosing::StateClosing(EyeStateMachine &_stateMachine)
  : AnimatedState(_stateMachine)
{
   animationFinishedState = "Closed";
}

optional<string> StateClosing::Update(const float deltaT, optional<Vector3> facePosition)
{
   optional<string> newState = AnimatedState::Update(deltaT, facePosition);
   stateMachine.GetEyeModel().Close(animationT);
   return newState;
}

string StateClosing::GetName() const
{
   return "Closing";
}

/******************************/

StateOpen::StateOpen(EyeStateMachine &_stateMachine)
  : State(_stateMachine)
{

}

void StateOpen::Enter()
{
   faceHiddenT = 0.f;
   faceStillT = 0.f;
}

optional<string> StateOpen::Update(const float deltaT, optional<Vector3> facePosition)
{
   optional<string> newState = nullopt;
   if (facePosition)
   {
      stateMachine.GetEyeModel().LookAt(facePosition.value());
      faceHiddenT = 0.f;

      if (IsFaceStill(facePosition.value()))
      {
         faceStillT += deltaT;
         if (faceStillT > focusThresholdT)
            newState = make_optional("Focusing");
      }
      previousFacePosition = facePosition.value();
   }
   else
   {
      faceHiddenT += deltaT;
      faceStillT = 0.f;
      if (faceHiddenT > closeThresholdT)
         newState = make_optional("Closing");
   }
   return newState;
}

std::string StateOpen::GetName() const
{
   return "Open";
}

bool StateOpen::IsFaceStill(const Vector3 &currentPosition) const
{
   const float maxDeltaLength = 0.5f;
   const Vector3& faceDelta = currentPosition - previousFacePosition;
   std::cout << "Current delta : " << faceDelta.ComputeLength() << std::endl;
   return (faceDelta.ComputeLength() < maxDeltaLength);
}

/******************************/

StateOpening::StateOpening(EyeStateMachine &_stateMachine)
  : AnimatedState(_stateMachine)
{
   animationFinishedState = "Open";
}

optional<string> StateOpening::Update(const float deltaT, optional<Vector3> facePosition)
{
   optional<string> newState = AnimatedState::Update(deltaT, facePosition);
   stateMachine.GetEyeModel().Open(animationT);
   return newState;
}

std::string StateOpening::GetName() const
{
   return "Opening";
}

/******************************/

StateFocusing::StateFocusing(EyeStateMachine &_stateMachine)
  : State(_stateMachine)
{
}

void StateFocusing::Enter()
{
   animationT = 0.f;
   previousFacePosition = nullopt;
}

optional<string> StateFocusing::Update(const float deltaT, optional<Vector3> facePosition)
{
   optional<string> newState = nullopt;
   if (facePosition)
   {
      animationT += deltaT;
      if (!previousFacePosition)
         previousFacePosition = facePosition;
      if (!IsFaceStill(facePosition.value()))
         newState = make_optional("Open");

      stateMachine.GetEyeModel().LookAt(facePosition.value());

      if (animationT > focusingFinishT)
      {
         animationT = focusingFinishT;
      }

      const float scaledT = animationFinishT * (animationT / focusingFinishT);
      stateMachine.GetEyeModel().Close(scaledT);
   }
   else
      newState = make_optional("Open");
   return newState;
}

std::string StateFocusing::GetName() const
{
   return "Focusing";
}

bool StateFocusing::IsFaceStill(const Vector3 &currentPosition) const
{
   const float maxDeltaLength = 0.05f;
   const Vector3& faceDelta = currentPosition - previousFacePosition.value();
   return (faceDelta.ComputeLength() < maxDeltaLength);
}
