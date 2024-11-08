# AiSoundContainer
- The abstract base class for all other containers
# SimpleSound
 - The end of all container chains, a SimpleSound simply contains an audio file, cannot contain other containers.
# RandomSound
- Plays a container sound from a weighted array
- Insert Shuffle functionality !
# SwitchSound
- Plays a container depending on a string parameter
- Currently requires there to be `default` key for default situations
# LoopSound
- Plays a loop intro, followed by the main loop body, and then a loop outro when the loop is stopped
- The Intro and Outro should be optional
# BlendSound
- Plays multiple containers, each containing a different time offset
# SequenceSound ?
- Every time it's called plays the next sound
