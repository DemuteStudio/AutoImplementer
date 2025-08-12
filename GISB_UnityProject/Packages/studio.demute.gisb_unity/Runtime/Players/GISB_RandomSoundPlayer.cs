using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Random = UnityEngine.Random;

namespace GISB.Runtime
{
    public class GISB_RandomSoundPlayer : GISB_AudioPlayerTemplate<GISB_RandomSound>
    {
        public class RandomizationValues
        {
            private List<int> indexes;
            private Queue<int> excludedIndexes;
            private int lastTriggeredIndex;
            private int avoidLastPlayed;
            
            public RandomizationValues(int count, int avoidLastPlayed)
            {
                
                indexes = new List<int>(count);
                for (int i = 0; i < count; i++)
                {
                    indexes.Add(i);
                }
                lastTriggeredIndex = -1;
                excludedIndexes = new Queue<int>();
                this.avoidLastPlayed = Mathf.Min(avoidLastPlayed, count - 1); // Ensure we don't avoid more than available sounds
            }

            public int GetRandomIndex()
            {
                int index = indexes[Random.Range(0, indexes.Count)];
                indexes.Remove(index);
                excludedIndexes.Enqueue(index);
                while(excludedIndexes.Count > avoidLastPlayed)
                {
                    indexes.Add(excludedIndexes.Dequeue());
                }
                return index;
            }
        }

        
        private bool isLooping = false;
        private int currentIndex = -1;
        private double lastChildTriggerTime = 0.0f;
        private const double SCHEDULING_DELAY = 0.1f;
        
        private List<GISB_BaseAudioPlayer> instantiatedPlayers = new List<GISB_BaseAudioPlayer>();
        
        
        public GISB_RandomSoundPlayer(GISB_RandomSound audioObject, GISB_BaseAudioPlayer parent = null) : base(audioObject, parent)
        {
        }

        public override void Play(GISB_EventInstance gisbEventInstance, double fadeInTime, double scheduledTime)
        {
            base.Play(gisbEventInstance, fadeInTime, scheduledTime);
            
            if (!RollForPlayProbability()) return;

            if (!gisbEventInstance.ownerComponent.randomizationValuesCache.ContainsKey(audioObject))
            {
                gisbEventInstance.ownerComponent.randomizationValuesCache[audioObject] =
                    new RandomizationValues(audioObject.RandomPlaylist.Length, audioObject.avoidLastPlayed);
            }
            
            if(instantiatedPlayers.Count == 0)
            {
                for (int i = 0; i < audioObject.RandomPlaylist.Length; i++)
                {
                    instantiatedPlayers.Add(audioObject.RandomPlaylist[i].GetPlayer(this));
                }
            }
            
            //fade in isn't passed on to children as it is evaluated on the highest container
            if(instantiatedPlayers.Count > 0)
            {
                currentIndex = GetRandomIndex();
                instantiatedPlayers[currentIndex].Play(gisbEventInstance, 0.0f, scheduledTime);
            }

            isLooping = audioObject.loop;
            lastChildTriggerTime = scheduledTime == 0 ? AudioSettings.dspTime : scheduledTime;
        }

        public override void Stop()
        {
            foreach (GISB_BaseAudioPlayer instantiatedPlayer in instantiatedPlayers)
            {
                instantiatedPlayer.Stop();
            }
            
            isLooping = false;
        }

        public override void UpdateAudioThread(double dspTime)
        {
            base.UpdateAudioThread(dspTime);
            
            foreach (GISB_BaseAudioPlayer instantiatedPlayer in instantiatedPlayers)
            {
                instantiatedPlayer.UpdateAudioThread(dspTime);
            }

            if (isLooping)
            {
                //So, the thing is that we need to schedule sounds to play later, but we don't want to schedule them
                //too early in case parameters change in between
                
                switch (audioObject.transitionMode)
                {
                    case GISB_RandomSound.TransitionMode.TriggerRate:
                        //This is play a sound every triggerRate seconds

                        if (dspTime + SCHEDULING_DELAY > lastChildTriggerTime + audioObject.triggerRate)
                        {
                            double newScheduledTime = lastChildTriggerTime + audioObject.triggerRate;
                            ownerInstance.scheduledActions.Enqueue(() =>
                            {
                                currentIndex = GetRandomIndex();
                                instantiatedPlayers[currentIndex].Play(ownerInstance, 0.0f,
                                    newScheduledTime);
                            });
                            lastChildTriggerTime = newScheduledTime;
                            //Debug.LogFormat("[Audio] Scheduling sound {0} to play at {1}", audioObject, newScheduledTime);
                        }
                        break;
                    case GISB_RandomSound.TransitionMode.Crossfade:
                        //This is play a sound with a crossfade after the current sound finishes playing
                        //We don't use scheduling delay here because it is less time critical
                        if (dspTime > lastChildTriggerTime + instantiatedPlayers[currentIndex].GetDuration() - audioObject.crossfadeDuration)
                        {
                            ownerInstance.scheduledActions.Enqueue(() => 
                                instantiatedPlayers[currentIndex].Stop(audioObject.crossfadeDuration));
                            ownerInstance.scheduledActions.Enqueue(() =>
                            {
                                currentIndex = GetRandomIndex();
                                instantiatedPlayers[currentIndex].Play(ownerInstance, audioObject.crossfadeDuration,
                                    0.0f);
                            });
                            lastChildTriggerTime = dspTime;
                            //Debug.LogFormat("[Audio] Starting crossfade {0}", audioObject);
                        }
                        break;
                    default:
                        throw new ArgumentOutOfRangeException();
                }
            }
        }

        public override void UpdateGameThread(float deltaTime)
        {
            foreach (GISB_BaseAudioPlayer instantiatedPlayer in instantiatedPlayers)
            {
                instantiatedPlayer.UpdateGameThread(deltaTime);
            }
        }

        public override void UpdateParameters(Dictionary<string, string> activeParameters)
        {
            foreach (GISB_BaseAudioPlayer instantiatedPlayer in instantiatedPlayers)
            {
                instantiatedPlayer.UpdateParameters(activeParameters);
            }
        }

        public override double GetDuration()
        {
            //Return duration of currently playing sound
            if (isLooping) return -1;
            if (currentIndex < 0) return 0.0f; // No sound is currently playing
            return instantiatedPlayers[currentIndex].GetDuration();
        }

        private int GetRandomIndex()
        {
            return ownerInstance.ownerComponent.randomizationValuesCache[audioObject].GetRandomIndex();
        }

        public override bool IsPlaying()
        {
            if (isLooping) return true;
            if (currentIndex < 0) return false; // No sound is currently playing
            return instantiatedPlayers[currentIndex].IsPlaying();
        }
    }
}
