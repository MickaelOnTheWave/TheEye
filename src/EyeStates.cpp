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
  : State(_stateMachine), modelAnimationFactor(1 / animationFinishT)
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
   stateMachine.GetEyeModel().Close(animationT * modelAnimationFactor);
   return newState;
}

string StateClosing::GetName() const
{
   return "Closing";
}

/******************************/

StateOpen::StateOpen(EyeStateMachine &_stateMachine)
  : State(_stateMachine), faceState(5.f, 0.02f)
{
}

void StateOpen::Enter()
{
   faceHiddenT = 0.f;
   faceState.Reset();
}

optional<string> StateOpen::Update(const float deltaT, optional<Vector3> facePosition)
{
   optional<string> newState = nullopt;
   if (facePosition)
   {
      stateMachine.GetEyeModel().LookAt(facePosition.value());
      faceHiddenT = 0.f;

      if (faceState.IsStill(deltaT, facePosition.value()))
         newState = make_optional("Focusing");
   }
   else
   {
      faceState.Reset();
      faceHiddenT += deltaT;
      if (faceHiddenT > waitBeforeClosingT)
         newState = make_optional("Closing");
   }
   return newState;
}

std::string StateOpen::GetName() const
{
   return "Open";
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
   stateMachine.GetEyeModel().Open(animationT * modelAnimationFactor);
   return newState;
}

std::string StateOpening::GetName() const
{
   return "Opening";
}

/******************************/

StateFocusing::StateFocusing(EyeStateMachine &_stateMachine)
  : State(_stateMachine), faceState(0.f, 0.04f)
{
}

void StateFocusing::Enter()
{
   animationT = 0.f;
   faceState.Reset();
   firstUpdateAfterEnter = true;
}

optional<string> StateFocusing::Update(const float deltaT, optional<Vector3> facePosition)
{
   optional<string> newState = nullopt;
   if (facePosition)
   {
      animationT += deltaT;

      if (firstUpdateAfterEnter)
      {
         firstUpdateAfterEnter = false;
         faceState.Reset(facePosition);
      }

      if (!faceState.IsStill(deltaT, facePosition.value()))
         newState = make_optional("Unfocusing");

      stateMachine.GetEyeModel().LookAt(facePosition.value());

      if (animationT > focusingFinishT)
      {
         animationT = focusingFinishT;
      }

      const float scaledT = animationFinishT * (animationT / focusingFinishT);
      stateMachine.GetEyeModel().Close(scaledT);
   }
   else
      newState = make_optional("Unfocusing");
   return newState;
}

std::string StateFocusing::GetName() const
{
   return "Focusing";
}

/******************************/

StateUnfocusing::StateUnfocusing(EyeStateMachine &_stateMachine)
  : AnimatedState(_stateMachine)
{
   animationFinishT = 0.5f;
   animationFinishedState = "Open";
}

std::optional<string> StateUnfocusing::Update(const float deltaT, std::optional<Vector3> facePosition)
{
   optional<string> newState = AnimatedState::Update(deltaT, facePosition);

   const float animationFinishedT = 1.f;
   const float deltaAnimationT = animationFinishedT - animationStartT;
   const float deltaTimeT = animationT / animationFinishT;
   const float scaledT = animationStartT + deltaAnimationT * deltaTimeT;
   stateMachine.GetEyeModel().Open(scaledT);
   return newState;
}

string StateUnfocusing::GetName() const
{
   return "Unfocusing";
}

