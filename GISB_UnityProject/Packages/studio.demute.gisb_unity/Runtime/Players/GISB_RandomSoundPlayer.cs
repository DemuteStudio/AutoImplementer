using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Random = UnityEngine.Random;

namespace GISB.Runtime
{
    public class GISB_RandomSoundPlayer : GISB_AudioPlayerTemplate<GISB_RandomSound>
    {
        private List<GISB_BaseAudioPlayer> instantiatedPlayers = new List<GISB_BaseAudioPlayer>();
        
        private List<int> indexes = new List<int>();
        private Queue<int> excludedIndexes = new Queue<int>();
        private int currentIndex = -1;
        
        private bool isLooping = false;
        private double lastChildTriggerTime = 0.0f;

        private Dictionary<string, string> lastParameters;
        
        private const double SCHEDULING_DELAY = 0.1f;
        
        public GISB_RandomSoundPlayer(GISB_RandomSound audioObject, GISB_BaseAudioPlayer parent = null) : base(audioObject, parent)
        {
        }

        public override void Play(Dictionary<string, string> activeParameters, GISB_EventInstance gisbEventInstance, double fadeInTime, double scheduledTime)
        {
            base.Play(activeParameters, gisbEventInstance, fadeInTime, scheduledTime);
            
            if (!RollForPlayProbability()) return;
            if(instantiatedPlayers.Count == 0)
            {
                for (int i = 0; i < audioObject.RandomPlaylist.Length; i++)
                {
                    indexes.Add(i);
                    instantiatedPlayers.Add(audioObject.RandomPlaylist[i].GetPlayer(this));
                }
            }
            
            //fade in isn't passed on to children as it is evaluated on the highest container
            if(instantiatedPlayers.Count > 0)
            {
                currentIndex = GetRandomIndex();
                instantiatedPlayers[currentIndex].Play(activeParameters, gisbEventInstance, 0.0f, scheduledTime);
            }

            isLooping = audioObject.loop;
            lastChildTriggerTime = scheduledTime == 0 ? AudioSettings.dspTime : scheduledTime;
            lastParameters = activeParameters;
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
                            lastGisbEventInstance.scheduledActions.Enqueue(() =>
                            {
                                currentIndex = GetRandomIndex();
                                instantiatedPlayers[currentIndex].Play(lastParameters, lastGisbEventInstance, 0.0f,
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
                            lastGisbEventInstance.scheduledActions.Enqueue(() => 
                                instantiatedPlayers[currentIndex].Stop(audioObject.crossfadeDuration));
                            lastGisbEventInstance.scheduledActions.Enqueue(() =>
                            {
                                currentIndex = GetRandomIndex();
                                instantiatedPlayers[currentIndex].Play(lastParameters, lastGisbEventInstance, audioObject.crossfadeDuration,
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
            lastParameters = activeParameters;
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
            int index = indexes[Random.Range(0, indexes.Count)];
            indexes.Remove(index);
            excludedIndexes.Enqueue(index);
            while(excludedIndexes.Count > audioObject.avoidLastPlayed)
            {
                indexes.Add(excludedIndexes.Dequeue());
            }
            return index;
        }
    }
}
