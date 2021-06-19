#pragma once

#include <vector>

class State;

class StateManager
{
private: // Public properties
	static std::vector<State*> s_states;
	static bool s_stateChange;

public: // Public methods
	static void Update(float deltaTime);
	static void Render();
	static void PushState(State* pState); // Used when going to PauseState
	static void PopState(); // Used when leaving PauseState
	static void ChangeState(State* pState);
	static void Quit(); // Used for cleanup
	static std::vector<State*>& GetStates(); // returns a reference to our State* vector
	static bool StateChanging() { return s_stateChange; }

private: // Private methods
	StateManager() {}
};
