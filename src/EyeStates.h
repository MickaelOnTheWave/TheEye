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
   virtual void Exit() = 0;

   virtual std::string GetName() const = 0;

protected:
   EyeStateMachine& stateMachine;
};

class StateClosed : public State
{
public:
   StateClosed(EyeStateMachine& _stateMachine);

   void Update(const float deltaT, std::optional<Vector3> facePosition) override;
   void Exit() override;

   std::string GetName() const override;
};

class StateClosing : public State
{
public:
   StateClosing(EyeStateMachine& _stateMachine);

   void Enter() override;
   void Update(const float deltaT, std::optional<Vector3> facePosition) override;
   void Exit() override;

   std::string GetName() const override;

private:
   const float animationFinishT = 1.f;
   float animationT = 0.f;
};

class StateOpen : public State
{
public:
   StateOpen(EyeStateMachine& _stateMachine);

   void Enter() override;
   void Update(const float deltaT, std::optional<Vector3> facePosition) override;
   void Exit() override;

   std::string GetName() const override;
};

class StateOpening : public State
{
public:
   StateOpening(EyeStateMachine& _stateMachine);

   void Enter() override;
   void Update(const float deltaT, std::optional<Vector3> facePosition) override;
   void Exit() override;

   std::string GetName() const override;

private:
   const float animationFinishT = 1.f;
   float animationT = 0.f;
};

#endif // EYESTATES_H
