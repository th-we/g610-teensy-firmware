-- States:
-- * 0 initial
-- * 1 pressed
-- * 2 cooled down
-- * 3 released

local INITIAL = 0
local PRESSED = 1
local RELEASED = 2
local KEY_COUNT = 3
local symbols = {"d", "u"}
local coldKeys = {}
for i = 1, KEY_COUNT do
  coldKeys[i] = false
end


function stateIsFinal(state)
  for i = 1, KEY_COUNT do
    if state[i] == PRESSED then
      return false
    end
  end
  return true
end


function countHotKeys(state)
  local n = 0
  for i = 1, KEY_COUNT do
    if state.hotKeys[i] then
      n = n + 1
    end
  end
  return n
end


function getChangedState(state, component, newEvent)
  local newState = {
    events = state.events.." "..component..symbols[newEvent],
    nextKey = math.max(state.nextKey, component + 1),
    hotKeys = {unpack(state.hotKeys)},
    unpack(state)
  }
  newState[component] = newEvent
  newState.hotKeys[component] = newEvent == PRESSED
  return newState
end


function getCooledDownEvent(state)
  return {
    events = state.events.." cc",
    nextKey = state.nextKey,
    hotKeys = {unpack(coldKeys)},
    unpack(state)
  }
end


function nextEvents(state, indent)
  -- print(indent..state.events, state.nextCooldown)
  if stateIsFinal(state) then
    print(state.events)
    -- print(indent.."========")
    return
  end

  nextIndent = indent.." "

  if countHotKeys(state) > 0 then
    nextEvents(getCooledDownEvent(state), nextIndent)
  end
  for i = 1, math.min(state.nextKey, KEY_COUNT) do
    if state[i] < RELEASED then
      nextEvents(getChangedState(state, i, state[i] + 1), nextIndent)
    end
  end
end

local initialEvent = {
  events = "1d",
  nextKey = 2,
  hotKeys = {true},
  PRESSED,
}

for i = 2, KEY_COUNT do
  initialEvent[i] = INITIAL
  initialEvent.hotKeys[i] = false
end

nextEvents(initialEvent, "")
