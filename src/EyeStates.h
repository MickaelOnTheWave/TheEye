#ifndef EYESTATES_H
#define EYESTATES_H

#include <optional>
#include <string>

#include "EyeStateMachine.h"
#include "FaceState.h"
#include "math/Vector3.h"

class State
{
public:
   State(EyeStateMachine& _stateMachine);

   virtual void Enter();

   /**
    * @brief Update
    * @param deltaT
    * @param facePosition
    * @return The name of the new state to switch to, or nothing if there is no change in state
    */
   virtual std::optional<std::string> Update(const float deltaT, std::optional<Vector3> facePosition) = 0;

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
   std::optional<std::string> Update(const float deltaT, std::optional<Vector3> facePosition) override;

protected:
   float animationFinishT = 0.1f;
   float modelAnimationFactor;
   float animationT = 0.f;
   std::string animationFinishedState;
};

class StateClosed : public State
{
public:
   StateClosed(EyeStateMachine& _stateMachine);

   void Enter() override;
   std::optional<std::string> Update(const float deltaT, std::optional<Vector3> facePosition) override;

   std::string GetName() const override;

private:
   const float openThresholdT = 3.f;
   float faceVisibleT = 0.f;
};

class StateClosing : public AnimatedState
{
public:
   StateClosing(EyeStateMachine& _stateMachine);

   std::optional<std::string> Update(const float deltaT, std::optional<Vector3> facePosition) override;

   std::string GetName() const override;
};

class StateOpen : public State
{
public:
   StateOpen(EyeStateMachine& _stateMachine);

   void Enter() override;
   std::optional<std::string> Update(const float deltaT, std::optional<Vector3> facePosition) override;

   std::string GetName() const override;

private:
   FaceState faceState;
   const float waitBeforeClosingT = 2.f;
   float faceHiddenT = 0.f;
};

class StateOpening : public AnimatedState
{
public:
   StateOpening(EyeStateMachine& _stateMachine);

   std::optional<std::string> Update(const float deltaT, std::optional<Vector3> facePosition) override;

   std::string GetName() const override;
};

class StateFocusing : public State
{
public:
   StateFocusing(EyeStateMachine& _stateMachine);

   void Enter() override;
   std::optional<std::string> Update(const float deltaT, std::optional<Vector3> facePosition) override;

   std::string GetName() const override;

protected:
   FaceState faceState;

   const float animationFinishT = 0.5f;
   const float focusingFinishT = 1.f;
   float animationT = 0.f;

private:
   bool firstUpdateAfterEnter = false;
};

class StateUnfocusing : public AnimatedState
{
public:
   StateUnfocusing(EyeStateMachine& _stateMachine);

   std::optional<std::string> Update(const float deltaT, std::optional<Vector3> facePosition) override;

   std::string GetName() const override;

private:
   const float animationStartT = 0.5f;
};

#endif // EYESTATES_H
