#include "EyeStates.h"

/******************************/

State::State(EyeStateMachine &_stateMachine)
  : stateMachine(_stateMachine)
{
}

void State::Enter()
{
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

void StateClosed::Exit()
{

}

std::string StateClosed::GetName() const
{
   return "Closed";
}

/******************************/

StateClosing::StateClosing(EyeStateMachine &_stateMachine)
  : State(_stateMachine)
{
}

void StateClosing::Enter()
{
   animationT = 0.f;
}

void StateClosing::Update(const float deltaT, std::optional<Vector3> facePosition)
{
   animationT += deltaT;

   if (animationT > animationFinishT)
   {
      stateMachine.Switch("Closed");
      animationT = animationFinishT;
   }
   stateMachine.GetEyeModel().Close(animationT);
}

void StateClosing::Exit()
{
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
}

void StateOpen::Update(const float deltaT, std::optional<Vector3> facePosition)
{
   if (facePosition)
   {
      stateMachine.GetEyeModel().LookAt(facePosition.value());
      faceHiddenT = 0.f;
   }
   else
   {
      faceHiddenT += deltaT;
      if (faceHiddenT > closeThresholdT)
         stateMachine.Switch("Closing");
   }
}

void StateOpen::Exit()
{

}

std::string StateOpen::GetName() const
{
   return "Open";
}

/******************************/

StateOpening::StateOpening(EyeStateMachine &_stateMachine)
  : State(_stateMachine)
{
}

void StateOpening::Enter()
{
   animationT = 0.f;
}

void StateOpening::Update(const float deltaT, std::optional<Vector3> facePosition)
{
   animationT += deltaT;

   if (animationT > animationFinishT)
   {
      stateMachine.Switch("Open");
      animationT = animationFinishT;
   }
   stateMachine.GetEyeModel().Open(animationT);
}

void StateOpening::Exit()
{

}

std::string StateOpening::GetName() const
{
   return "Opening";
}
