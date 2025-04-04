using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace GISB.Runtime
{
    public class GISB_SingleSoundPlayer : GISB_AudioPlayerTemplate<GISB_SingleSound>
    {
        public GISB_SingleSoundPlayer(GISB_SingleSound audioObject, GISB_BaseAudioPlayer parent = null) : base(audioObject, parent)
        { }

        public override void Play(Dictionary<string, string> activeParameters, GISB_EventInstance gisbEventInstance)
        {
            if (!RollForPlayProbability()) return;
            AudioSource audioSource = gisbEventInstance.GetAudioVoice();
            
            GISB_Attenuation attenuation = GetAttenuation();
            
            float volume = GetVolume();
            float pitch = GetPitch();
            float lowpass = GetLowpass();

            SetAttenuation(audioSource, attenuation);
            
            audioSource.clip = audioObject.soundClip;
            audioSource.loop = audioObject.loop;
            audioSource.volume = decibelsToLinear(volume);
            audioSource.pitch = centsToLinear(pitch);
            audioSource.GetComponent<AudioLowPassFilter>().cutoffFrequency = Mathf.Lerp(22000, 0, lowpass);
            audioSource.Play();
        }

        public override void UpdateParameters(Dictionary<string, string> activeParameters)
        {
            //Do nothing
        }

        private void SetAttenuation(AudioSource audioSource, GISB_Attenuation attenuation)
        {
            audioSource.spatialize = attenuation.active;
            audioSource.spatialBlend = attenuation.active ? 1.0f : 0.0f;
            audioSource.minDistance = attenuation.minDistance;
            audioSource.maxDistance = attenuation.maxDistance;
            
            if (attenuation.active)
            {
                AnimationCurve curve = new AnimationCurve(attenuation.curve.keys);
                switch (attenuation.preset)
                {
                    case GISB_Attenuation.AttenuationPreset.Linear:
                        curve = AnimationCurve.Linear(0,1,1,0);
                        break;
                    case GISB_Attenuation.AttenuationPreset.Logarithmic:
                        curve = GISB_Attenuation.GetAttenuationCurveForPreset(GISB_Attenuation.AttenuationPreset
                            .Logarithmic);
                        break;
                    case GISB_Attenuation.AttenuationPreset.Inverse:
                        curve = GISB_Attenuation.GetAttenuationCurveForPreset(GISB_Attenuation.AttenuationPreset
                            .Inverse);
                        break;
                    case GISB_Attenuation.AttenuationPreset.Custom:
                        break;
                    default:
                        throw new ArgumentOutOfRangeException();
                }
                
                audioSource.rolloffMode = AudioRolloffMode.Custom;
                //Volume
                curve.MoveKey(1, new Keyframe(1, attenuation.volumeAtMaxDistance));
                audioSource.SetCustomCurve(AudioSourceCurveType.CustomRolloff, curve);
                //Spread
                curve.MoveKey(0, new Keyframe(0, attenuation.spreadAtMinDistance));
                curve.MoveKey(1, new Keyframe(1, attenuation.spreadAtMaxDistance));
                audioSource.SetCustomCurve(AudioSourceCurveType.Spread, curve);
                curve.MoveKey(0, new Keyframe(0, lowpassToFilterValue(attenuation.lowPassAtMinDistance)));
                curve.MoveKey(1, new Keyframe(1, lowpassToFilterValue(attenuation.lowPassAtMaxDistance)));
                audioSource.GetComponent<AudioLowPassFilter>().customCutoffCurve = curve;
            }
        }
        
        private float decibelsToLinear(float db)
        {
            return Mathf.Pow(10f, db / 20f);
        }
        
        private float centsToLinear(float cents)
        {
            return Mathf.Pow(2f, cents / 1200f);
        }
        
        private float lowpassToFilterValue(float lowpass)
        {
            //When the lowpass is 0, the filter value is 1
            //When the lowpass is 1, the filter value is 0
            //When the lowpass is 0.5, the filter value is 0.5
            //When the lowpass is 2, the filter value is -1 (clamped to 0)
            //When the lowpass is -1, the filter value is 2 (clamped to 1)
            
            //Clamp the lowpass value to 0 and 1
            lowpass = Mathf.Clamp(lowpass, 0, 1);
            //Invert the lowpass value
            lowpass = 1 - lowpass;

            return lowpass;
        }
    }
}
