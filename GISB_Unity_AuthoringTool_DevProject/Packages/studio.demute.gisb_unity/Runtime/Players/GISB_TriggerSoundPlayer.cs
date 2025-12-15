using System;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UIElements;

namespace GISB.Runtime
{
    public class GISB_TriggerSoundPlayer : GISB_AudioPlayerTemplate<GISB_TriggerSound>
    {
        private GISB_BaseAudioPlayer instantiatedPlayer = null;
        private GISB_BaseAudioPlayer lastInstantiatedPlayer = null;
        private int currentTriggerCount = 0;
        private double lastTriggerTime = 0.0f;
        private const double SCHEDULING_DELAY = 0.1f;
        private bool broken = false;

        private bool shouldRetrigger => !broken && (audioObject.triggerAmount == -1 || currentTriggerCount < audioObject.triggerAmount);
        
        public GISB_TriggerSoundPlayer(GISB_TriggerSound audioObject, GISB_BaseAudioPlayer parent = null) : base(audioObject, parent)
        {
        }

        public override void Play(GISB_EventInstance gisbEventInstance, double fadeInTime, double scheduledTime)
        {
            base.Play(gisbEventInstance, fadeInTime, scheduledTime);
            
            if (!RollForPlayProbability()) return;
            
            if(instantiatedPlayer == null)
            {
                instantiatedPlayer = audioObject.TriggeredSound.GetPlayer(this);
            }
            broken = false;
            //fade in isn't passed on to children as it is evaluated on the highest container
            instantiatedPlayer.Play(gisbEventInstance, 0.0f, scheduledTime);
            currentTriggerCount = 1;
            lastTriggerTime = scheduledTime == 0 ? AudioSettings.dspTime : scheduledTime;
        }

        public override void Stop()
        {
            if(instantiatedPlayer != null)
            {
                instantiatedPlayer.Stop();
            }
            currentTriggerCount = 0;
            broken = true;
        }

        public override void Break()
        {
            broken = true;
        }

        public override void UpdateAudioThread(double dspTime)
        {
            base.UpdateAudioThread(dspTime);
            
            instantiatedPlayer.UpdateAudioThread(dspTime);
            if(lastInstantiatedPlayer != null)
            {
                lastInstantiatedPlayer.UpdateAudioThread(dspTime);
            }
            
            if (shouldRetrigger)
            {
                //So, the thing is that we need to schedule sounds to play later, but we don't want to schedule them
                //too early in case parameters change in between
                switch (audioObject.transitionMode)
                {
                    case GISB_TriggerSound.TransitionMode.TriggerRate:
                        //This is play a sound every triggerRate seconds

                        if (dspTime + SCHEDULING_DELAY > lastTriggerTime + audioObject.triggerRate)
                        {
                            double newScheduledTime = lastTriggerTime + audioObject.triggerRate;
                            ownerInstance.scheduledActions.Enqueue(() =>
                            {
                                instantiatedPlayer.Play(ownerInstance, 0.0f,
                                    newScheduledTime);
                            });
                            currentTriggerCount++;
                            lastTriggerTime = newScheduledTime;
                            //Debug.LogFormat("[Audio] Scheduling sound {0} to play at {1}", audioObject, newScheduledTime);
                        }
                        break;
                    case GISB_TriggerSound.TransitionMode.Crossfade:
                        //This is play a sound with a crossfade after the current sound finishes playing
                        //We don't use scheduling delay here because it is less time critical
                        if (dspTime > lastTriggerTime + instantiatedPlayer.GetDuration() - audioObject.crossfadeDuration)
                        {
                            lastInstantiatedPlayer = instantiatedPlayer;
                            ownerInstance.scheduledActions.Enqueue(() => 
                                lastInstantiatedPlayer.Stop(audioObject.crossfadeDuration));
                            instantiatedPlayer = audioObject.TriggeredSound.GetPlayer(this);
                            ownerInstance.scheduledActions.Enqueue(() =>
                            {
                                instantiatedPlayer.Play(ownerInstance, audioObject.crossfadeDuration,
                                    0.0f);
                            });
                            currentTriggerCount++;
                            lastTriggerTime = dspTime;
                            // Debug.LogFormat("[Audio] Starting crossfade {0}", audioObject);
                        }
                        break;
                    default:
                        throw new ArgumentOutOfRangeException();
                }
            }
        }

        public override void UpdateGameThread(float deltaTime)
        {
            instantiatedPlayer.UpdateGameThread(deltaTime);
            if(lastInstantiatedPlayer != null)
            {
                lastInstantiatedPlayer.UpdateGameThread(deltaTime);
            }
        }

        public override void UpdateParameters(Dictionary<string, string> activeParameters)
        {
            instantiatedPlayer.UpdateParameters(activeParameters);
            if(lastInstantiatedPlayer != null)
            {
                lastInstantiatedPlayer.UpdateParameters(activeParameters);
            }
        }

        public override void UpdateParameters(Dictionary<string, float> activeParameters)
        {
            instantiatedPlayer.UpdateParameters(activeParameters);
            if(lastInstantiatedPlayer != null)
            {
                lastInstantiatedPlayer.UpdateParameters(activeParameters);
            }
        }

        public override double GetDuration()
        {
            //Return duration of currently playing sound
            return instantiatedPlayer.GetDuration();
        }

        public override bool IsPlaying()
        {
            return shouldRetrigger || instantiatedPlayer.IsPlaying();
        }
    }
}