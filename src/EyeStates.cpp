#include "EyeStates.h"

#include <iostream>

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

void AnimatedState::Update(const float deltaT, std::optional<Vector3> facePosition)
{
   animationT += deltaT;

   if (animationT > animationFinishT)
   {
      stateMachine.Switch(animationFinishedState);
      animationT = animationFinishT;
   }
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

void StateClosed::Update(const float deltaT, std::optional<Vector3> facePosition)
{
   if (facePosition)
      faceVisibleT += deltaT;
   else
      faceVisibleT = 0.f;

   if (faceVisibleT > openThresholdT)
      stateMachine.Switch("Opening");
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

void StateClosing::Update(const float deltaT, std::optional<Vector3> facePosition)
{
   AnimatedState::Update(deltaT, facePosition);
   stateMachine.GetEyeModel().Close(animationT);
}

std::string StateClosing::GetName() const
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

void StateOpen::Update(const float deltaT, std::optional<Vector3> facePosition)
{
   if (facePosition)
   {
      stateMachine.GetEyeModel().LookAt(facePosition.value());
      faceHiddenT = 0.f;

      if (IsFaceStill(facePosition.value()))
      {
         faceStillT += deltaT;
         if (faceStillT > focusThresholdT)
            stateMachine.Switch("Focusing");
      }
      previousFacePosition = facePosition.value();
   }
   else
   {
      faceHiddenT += deltaT;
      faceStillT = 0.f;
      if (faceHiddenT > closeThresholdT)
         stateMachine.Switch("Closing");
   }
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

void StateOpening::Update(const float deltaT, std::optional<Vector3> facePosition)
{
   AnimatedState::Update(deltaT, facePosition);
   stateMachine.GetEyeModel().Open(animationT);
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
   previousFacePosition = std::nullopt;
}

void StateFocusing::Update(const float deltaT, std::optional<Vector3> facePosition)
{
   if (facePosition)
   {
      animationT += deltaT;
      if (!previousFacePosition)
         previousFacePosition = facePosition;
      if (!IsFaceStill(facePosition.value()))
         stateMachine.Switch("Open");

      stateMachine.GetEyeModel().LookAt(facePosition.value());

      if (animationT > focusingFinishT)
      {
         animationT = focusingFinishT;
      }

      const float scaledT = animationFinishT * (animationT / focusingFinishT);
      stateMachine.GetEyeModel().Close(scaledT);
   }
   else
      stateMachine.Switch("Open");
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
