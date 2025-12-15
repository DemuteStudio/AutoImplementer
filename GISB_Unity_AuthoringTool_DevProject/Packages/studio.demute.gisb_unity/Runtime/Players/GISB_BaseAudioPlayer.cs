using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace GISB.Runtime
{
    public abstract class GISB_BaseAudioPlayer
    {
        protected GISB_BaseAudioPlayer parent = null;

        public abstract void Play(GISB_EventInstance gisbEventInstance,
            double fadeInTime, double scheduledTime);
        
        public abstract void Stop(double fadeOutTime);

        public abstract void Stop();
        
        public abstract void Break();

        public abstract void UpdateAudioThread(double dspTime);

        public abstract void UpdateGameThread(float deltaTime);

        public abstract void UpdateParameters(Dictionary<string, string> activeParameters);
        
        public abstract void UpdateParameters(Dictionary<string, float> activeParameters);
        
        public abstract GISB_Attenuation GetAttenuation();

        public abstract float GetVolume();
        public abstract float GetPitch();
        public abstract float GetLowpass();

        public abstract double GetDuration();

        public abstract bool IsPlaying();
    }
    
    public abstract class GISB_AudioPlayerTemplate<T> : GISB_BaseAudioPlayer where T : GISB_AudioObjectBase
    {
        protected T audioObject;
        
        protected GISB_EventInstance ownerInstance;
                
        //Volume calculated from the audioObject's volumeDB with random
        private float targetVolume;
        
        //Actual current volume in (this might be different from the target when fading)
        private float currentVolume;
        
        private float currentPitch;

        //The last time this player was triggered to play
        protected double playTime;
        
        private double fadeInTime = 0.0f;

        //The last time this player was stopped (used for fading out)
        private double stopTime;
        
        private double fadeOutTime = 0.0f;

        public override void Play(GISB_EventInstance gisbEventInstance, double fadeInTime, double scheduledTime)
        {
            if (audioObject.mute) return;
            
            targetVolume = GISB_VolumeParameter.decibelsToLinear(audioObject.volumeDB.GetValue());
            currentVolume = fadeInTime > 0 ? 0.0f : targetVolume; // Start with 0 volume if fading in, otherwise set to target volume immediately
            currentPitch = audioObject.pitchCents.GetValue();
            playTime = scheduledTime == 0 ? AudioSettings.dspTime : scheduledTime;
            this.fadeInTime = fadeInTime;
            this.ownerInstance = gisbEventInstance;
            
            // if(fadeInTime > 0) Debug.LogFormat("Starting fade-in for audio: {0}, Target Volume: {1}, Fade-in Time: {2}", 
            //     audioObject, targetVolume, fadeInTime);
            // else Debug.LogFormat("Playing audio: {0}, Target Volume: {1}", audioObject, targetVolume);
        }

        public override void Stop(double fadeOutTime)
        {
            if (fadeOutTime <= 0.0f) Stop();
            
            stopTime = AudioSettings.dspTime;
            this.fadeOutTime = fadeOutTime;
            targetVolume = currentVolume; //Just in case our volume when stopping isn't at the target volume
            
            // if (fadeOutTime > 0) Debug.LogFormat("Starting fade-out for audio: {0}, Current Volume: {1}, Fade-out Time: {2}", 
            //     audioObject, currentVolume, fadeOutTime);
            // else Debug.LogFormat("Stopping audio: {0}, Current Volume: {1}", audioObject, currentVolume);
        }

        public override void Break()
        {
            //Do nothing by default
        }

        public override void UpdateAudioThread(double dspTime)
        {
            // This method can be overridden by subclasses to handle time updates
            // For example, to handle fading in or out of the audio
            if (fadeInTime > 0)
            {
                double elapsed = dspTime - playTime;
                if (elapsed < fadeInTime)
                {
                    currentVolume = Mathf.Lerp(0.0f, targetVolume, (float)(elapsed / fadeInTime));
                    // Debug.LogFormat("Fading in audio: Current Volume: {0}, Target Volume: {1}, Elapsed Time: {2}", 
                    //     currentVolume, targetVolume, elapsed);
                }
                else
                {
                    currentVolume = targetVolume; // Reached target volume
                    fadeInTime = 0.0f; // Reset fade-in time
                }
            }
            
            if (fadeOutTime > 0)
            {
                double elapsed = dspTime - stopTime;
                if (elapsed < fadeOutTime)
                {
                    currentVolume = Mathf.Lerp(targetVolume, 0.0f, (float)(elapsed / fadeOutTime));
                    // Debug.LogFormat("Fading out audio: Current Volume: {0}, Target Volume: {1}, Elapsed Time: {2}", 
                    //     currentVolume, targetVolume, elapsed);
                }
                else
                {
                    currentVolume = 0.0f; // Reached zero volume
                    fadeOutTime = 0.0f; // Reset fade-out time
                    
                    //Enqueue stop action if volume reaches zero
                    ownerInstance.scheduledActions.Enqueue(Stop);
                }
            }
        }
        
        protected GISB_AudioPlayerTemplate(T audioObject, GISB_BaseAudioPlayer parent = null)
        {
            this.audioObject = audioObject;
            this.parent = parent;
        }
        public override GISB_Attenuation GetAttenuation()
        {
            if (audioObject.attenuation.overrideParent || parent == null)
            {
                return audioObject.attenuation.value;
            }
            else
            {
                return parent.GetAttenuation();
            }
        }

        public override float GetVolume()
        {
            float finalVolume = currentVolume;
            if(parent != null)
            {
                finalVolume *= parent.GetVolume();
            }
            //Modulators
            foreach (GISB_Modulator modulator in audioObject.modulators)
            {
                if (modulator.Target == GISB_Modulator.ModulatorTarget.Volume)
                {
                    if (ownerInstance.ownerComponent.activeFloatParameters.TryGetValue(modulator.ParameterName,
                            out float parameterValue))
                    {
                        finalVolume *=  GISB_VolumeParameter.decibelsToLinear(modulator.Curve.Evaluate(parameterValue));
                    }
                }
            }

            return finalVolume;
        }

        public override float GetPitch()
        {
            if (parent == null)
            {
                return currentPitch;
            }
            else
            {
                return parent.GetPitch() + currentPitch;
            }
        }

        public override float GetLowpass()
        {
            if (parent == null)
            {
                return audioObject.lowpassPercent.GetValue();
            }
            else
            {
                return parent.GetLowpass() + audioObject.lowpassPercent.GetValue();
            }
        }

        protected bool RollForPlayProbability()
        {
            if(audioObject.playbackProbabilityPercent < 100.0f)
            {
                float randomValue = Random.Range(0.0f, 100.0f);
                return randomValue <= audioObject.playbackProbabilityPercent;
            }
            else
            {
                return true;
            }
        }
    }
}

