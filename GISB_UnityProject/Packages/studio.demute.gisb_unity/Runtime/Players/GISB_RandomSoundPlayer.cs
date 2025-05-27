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
        
        private bool isLooping = false;
        private double lastScheduledTime = 0.0f;

        private Dictionary<string, string> lastParameters;
        private GISB_EventInstance lastGisbEventInstance;
        
        private const double SCHEDULING_DELAY = 0.1f;
        
        public GISB_RandomSoundPlayer(GISB_RandomSound audioObject, GISB_BaseAudioPlayer parent = null) : base(audioObject, parent)
        {
        }

        public override void Play(Dictionary<string, string> activeParameters, GISB_EventInstance gisbEventInstance, double scheduledTime)
        {
            if (!RollForPlayProbability()) return;
            if(instantiatedPlayers.Count == 0)
            {
                for (int i = 0; i < audioObject.RandomPlaylist.Length; i++)
                {
                    indexes.Add(i);
                    instantiatedPlayers.Add(audioObject.RandomPlaylist[i].GetPlayer(this));
                }
            }
            
            if(instantiatedPlayers.Count > 0)
            {
                int index = GetRandomIndex();
                instantiatedPlayers[index].Play(activeParameters, gisbEventInstance, scheduledTime);
            }

            isLooping = audioObject.loop;
            lastScheduledTime = scheduledTime == 0 ? AudioSettings.dspTime : scheduledTime;
            lastParameters = activeParameters;
            lastGisbEventInstance = gisbEventInstance;
        }

        public override void Stop()
        {
            foreach (GISB_BaseAudioPlayer instantiatedPlayer in instantiatedPlayers)
            {
                instantiatedPlayer.Stop();
            }
            
            isLooping = false;
        }

        public override void UpdateTime(double dspTime)
        {
            foreach (GISB_BaseAudioPlayer instantiatedPlayer in instantiatedPlayers)
            {
                instantiatedPlayer.UpdateTime(dspTime);
            }

            if (isLooping)
            {
                //So, the thing is that we need to schedule sounds to play later, but we don't want to schedule them
                //too early in case parameters change in between
                
                switch (audioObject.transitionMode)
                {
                    case GISB_RandomSound.TransitionMode.TriggerRate:
                        //This is play a sound every triggerRate seconds
                        
                        if(dspTime + SCHEDULING_DELAY > lastScheduledTime + audioObject.triggerRate )
                        {
                            double newScheduledTime = lastScheduledTime + audioObject.triggerRate;
                            lastGisbEventInstance.scheduledActions.Enqueue(() => 
                                Play(lastParameters, lastGisbEventInstance, newScheduledTime));
                            lastScheduledTime = newScheduledTime;
                            //Debug.LogFormat("[Audio] Scheduling sound {0} to play at {1}", audioObject, newScheduledTime);
                        }
                        break;
                    case GISB_RandomSound.TransitionMode.Crossfade:
                        //This is play a sound with a crossfade after the current sound finishes playing
                        //TODO : One day, cause we also need to implement fade in and fade out then
                        break;
                    default:
                        throw new ArgumentOutOfRangeException();
                }
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
