-- States:
-- * 0 initial
-- * 1 pressed
-- * 2 cooled down
-- * 3 released

local INITIAL = 0
local PRESSED = 1
local COOLED_DOWN = 2
local RELEASED = 3
local KEY_COUNT = 3
local symbols = {"d", "c", "u"}

function stateIsFinal(state)
  for i = 1, KEY_COUNT do
    if state[i] == PRESSED or state[i] == COOLED_DOWN then
      return false
    end
  end
  if state[1] == RELEASED and state[2] == RELEASED then
    return true
  end
  return false
end


function getChangedState(state, component, newEvent)
  local newState = {
    events = state.events..";"..component..symbols[newEvent],
    nextCooldown = state.nextCooldown,
    unpack(state)
  }
  newState[component] = newEvent
  if newEvent == PRESSED then
    newState.nextCooldown = component
  else
    for i = component + 1, KEY_COUNT do
      newState.nextCooldown = i
      if newState[i] == PRESSED then
        break
      end
    end
  end
  return newState
end


function nextEvents(state, indent)
  -- print(indent..state.events, state.nextCooldown)
  if stateIsFinal(state) then
    print(state.events)
    -- print(indent.."========")
    return
  end

  nextIndent = indent.." "

  for i = 1, KEY_COUNT do
    if state[i] == INITIAL then
      nextEvents(getChangedState(state, i, PRESSED), nextIndent)
    elseif state[i] == PRESSED then
      if state.nextCooldown == i then
        nextEvents(getChangedState(state, i, COOLED_DOWN), nextIndent)
      -- else
      --   local invalidState = getChangedState(state, i, COOLED_DOWN)
      --   invalidState.events = "_"..invalidState.events
      --   nextEvents(invalidState)
      end
      nextEvents(getChangedState(state, i, RELEASED), nextIndent)
    elseif state[i] == COOLED_DOWN then
      nextEvents(getChangedState(state, i, RELEASED), nextIndent)
    end
  end
end

local initialEvent = {
  events = "1d;2d",
  nextCooldown = 2,
  PRESSED, PRESSED
}

for i = 3, KEY_COUNT do
  initialEvent[i] = INITIAL
end

nextEvents(initialEvent, "")
