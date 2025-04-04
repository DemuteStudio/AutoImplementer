using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace GISB.Runtime
{
    public abstract class GISB_BaseAudioPlayer
    {
        protected GISB_BaseAudioPlayer parent = null;
        
        public abstract void Play(Dictionary<string, string> activeParameters, GISB_EventInstance gisbEventInstance);

        public abstract void UpdateParameters(Dictionary<string, string> activeParameters);
        
        public abstract GISB_Attenuation GetAttenuation();
        public abstract float GetVolume();
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
                return audioObject.volume.GetRandomValue();
            }
            else
            {
                return parent.GetVolume() * audioObject.volume.GetRandomValue();
            }
        }
    }
}

