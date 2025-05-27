using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace GISB.Runtime
{
    public class GISB_SingleSoundPlayer : GISB_AudioPlayerTemplate<GISB_SingleSound>
    {
        private AudioSource audioVoice;
        
        public GISB_SingleSoundPlayer(GISB_SingleSound audioObject, GISB_BaseAudioPlayer parent = null) : base(audioObject, parent)
        { }

        public override void Play(Dictionary<string, string> activeParameters, GISB_EventInstance gisbEventInstance, double scheduledTime)
        {
            if (!RollForPlayProbability()) return;
            audioVoice = gisbEventInstance.GetAudioVoice();
            
            GISB_Attenuation attenuation = GetAttenuation();
            
            float volume = GetVolume();
            float pitch = GetPitch();
            float lowpass = GetLowpass();

            SetAttenuation(audioVoice, attenuation);
            
            audioVoice.clip = audioObject.soundClip;
            audioVoice.loop = audioObject.loop;
            audioVoice.volume = decibelsToLinear(volume);
            audioVoice.pitch = centsToLinear(pitch);
            audioVoice.GetComponent<AudioLowPassFilter>().cutoffFrequency = Mathf.Lerp(22000, 0, lowpass);
            if(scheduledTime > 0)
            {
                audioVoice.PlayScheduled(scheduledTime);
                //Debug.LogFormat("Scheduled sound {0} to play at {1} with volume {2}, pitch {3}, lowpass {4}", 
                //    audioObject.soundClip, scheduledTime, audioVoice.volume, audioVoice.pitch, audioVoice.GetComponent<AudioLowPassFilter>().cutoffFrequency);
            }
            else
            {
                audioVoice.Play();
            }
        }

        public override void Stop()
        {
            if(audioVoice == null) return;
            audioVoice.Stop();
            audioVoice = null;
        }

        public override void UpdateTime(double dspTime)
        {
            //Do nothing
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
            //When the lowpass is 100, the filter value is 0
            //When the lowpass is 50, the filter value is 0.5
            //When the lowpass is 200, the filter value is -1 (clamped to 0)
            //When the lowpass is -100, the filter value is 2 (clamped to 1)
            
            //Clamp the lowpass value to 0 and 1
            lowpass = Mathf.Clamp(lowpass*0.01f, 0, 1);
            //Invert the lowpass value
            lowpass = 1 - lowpass;

            return lowpass;
        }
    }
}
