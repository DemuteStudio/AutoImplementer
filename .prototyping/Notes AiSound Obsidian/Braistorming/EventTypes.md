# AiSoundEvent
The base abstract class for all event classes
# SoundEvent
- The base of all event types, is the class between the events and the actual containers
- Only contains the event name and a container
- Created to allow the user to choose events instead of banks 
# SoundBank
- A collection of events that can be grouped together
	- Allows grouping for example a gun bank that has shot, reload, pickup, etc.
- Acts as a switch when calling the event