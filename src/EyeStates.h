#ifndef EYESTATES_H
#define EYESTATES_H

#include <optional>
#include <string>

#include "EyeStateMachine.h"
#include "math/Vector3.h"

class State
{
public:
   State(EyeStateMachine& _stateMachine);

   virtual void Enter();
   virtual void Update(const float deltaT, std::optional<Vector3> facePosition) = 0;
   virtual void Exit();

   virtual std::string GetName() const = 0;

protected:
   EyeStateMachine& stateMachine;
};

class AnimatedState : public State
{
public:
   AnimatedState(EyeStateMachine& _stateMachine);

   void Enter() override;
   virtual void Update(const float deltaT, std::optional<Vector3> facePosition) override;

protected:
   const float animationFinishT = 1.f;
   float animationT = 0.f;
   std::string animationFinishedState;
};

class StateClosed : public State
{
public:
   StateClosed(EyeStateMachine& _stateMachine);

   void Enter() override;
   void Update(const float deltaT, std::optional<Vector3> facePosition) override;

   std::string GetName() const override;

private:
   const float openThresholdT = 12.f;
   float faceVisibleT = 0.f;
};

class StateClosing : public AnimatedState
{
public:
   StateClosing(EyeStateMachine& _stateMachine);

   void Update(const float deltaT, std::optional<Vector3> facePosition) override;

   std::string GetName() const override;
};

class StateOpen : public State
{
public:
   StateOpen(EyeStateMachine& _stateMachine);

   void Enter() override;
   void Update(const float deltaT, std::optional<Vector3> facePosition) override;

   std::string GetName() const override;

private:
   const float closeThresholdT = 15.f;
   float faceHiddenT = 0.f;
};

class StateOpening : public AnimatedState
{
public:
   StateOpening(EyeStateMachine& _stateMachine);

   void Update(const float deltaT, std::optional<Vector3> facePosition) override;

   std::string GetName() const override;
};

#endif // EYESTATES_H
