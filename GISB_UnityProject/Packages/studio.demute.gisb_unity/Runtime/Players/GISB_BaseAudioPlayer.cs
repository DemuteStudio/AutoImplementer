using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace GISB.Runtime
{
    public abstract class GISB_BaseAudioPlayer
    {
        protected GISB_BaseAudioPlayer parent = null;
        
        public abstract void Play(Dictionary<string, string> activeParameters, GISB_EventInstance gisbEventInstance, double scheduledTime);

        public abstract void Stop();

        public abstract void UpdateTime(double dspTime);

        public abstract void UpdateParameters(Dictionary<string, string> activeParameters);
        
        public abstract GISB_Attenuation GetAttenuation();
        public abstract float GetVolume();
        public abstract float GetPitch();
        public abstract float GetLowpass();
    }
    
    public abstract class GISB_AudioPlayerTemplate<T> : GISB_BaseAudioPlayer where T : GISB_AudioObjectBase
    {
        protected T audioObject;
        
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
            if(parent == null)
            {
                return audioObject.volumeDB.GetRandomValue();
            }
            else
            {
                return parent.GetVolume() + audioObject.volumeDB.GetRandomValue();
            }
        }

        public override float GetPitch()
        {
            if (parent == null)
            {
                return audioObject.pitchCents.GetRandomValue();
            }
            else
            {
                return parent.GetPitch() + audioObject.pitchCents.GetRandomValue();
            }
        }

        public override float GetLowpass()
        {
            if (parent == null)
            {
                return audioObject.lowpassPercent.GetRandomValue();
            }
            else
            {
                return parent.GetLowpass() + audioObject.lowpassPercent.GetRandomValue();
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

