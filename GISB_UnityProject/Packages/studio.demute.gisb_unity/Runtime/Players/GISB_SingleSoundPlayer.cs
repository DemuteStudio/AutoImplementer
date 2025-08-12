using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace GISB.Runtime
{
    public class GISB_SingleSoundPlayer : GISB_AudioPlayerTemplate<GISB_SingleSound>
    {
        private List<GISB_AudioVoice> audioVoices = new List<GISB_AudioVoice>();

        private double soundDuration = -1f;
        
        public GISB_SingleSoundPlayer(GISB_SingleSound audioObject, GISB_BaseAudioPlayer parent = null) : base(audioObject, parent)
        { }

        public override void Play(GISB_EventInstance gisbEventInstance, double fadeInTime, double scheduledTime)
        {
            base.Play(gisbEventInstance, fadeInTime, scheduledTime);
            
            if (!RollForPlayProbability()) return;
            GISB_AudioVoice audioVoice = gisbEventInstance.ownerComponent.GetAudioVoice();
            audioVoice.owner = this;
            
            GISB_Attenuation attenuation = GetAttenuation();
            
            float pitch = GetPitch();
            float lowpass = GetLowpass();

            
            audioVoice.source.clip = audioObject.soundClip;
            audioVoice.source.loop = audioObject.loop;
            audioVoice.source.volume = GetVolume();
            audioVoice.source.pitch = centsToLinear(pitch);
            audioVoice.lowPassFilter.cutoffFrequency = Mathf.Lerp(22000, 0, lowpass);
            audioVoice.SetAttenuation(attenuation);

            //Store clip duration here because we can't get it from the audio thread
            if (audioVoice.source.loop)
            {
                soundDuration = -1f; //Looping sounds have no duration
            }
            else
            {
                soundDuration = audioObject.soundClip? audioObject.soundClip.length / audioVoice.source.pitch : 0f;
            }
            
            if(scheduledTime > 0)
            {
                audioVoice.source.PlayScheduled(scheduledTime);
                // Debug.LogFormat(
                //     "Scheduled sound {0} to play at {1} with volume {2}, pitch {3}, lowpass {4}, duration {5}",
                //     audioObject.soundClip, scheduledTime, audioVoice.volume, audioVoice.pitch,
                //     audioVoice.GetComponent<AudioLowPassFilter>().cutoffFrequency, soundDuration);
            }
            else
            {
                audioVoice.source.Play();
                // Debug.LogFormat("Playing sound {0} with volume {1}, pitch {2}, lowpass {3}, duration {4}",
                //     audioObject.soundClip, audioVoice.volume, audioVoice.pitch,
                //     audioVoice.GetComponent<AudioLowPassFilter>().cutoffFrequency, soundDuration);
            }
            audioVoices.Add(audioVoice);
        }

        public override void Stop()
        {
            foreach (GISB_AudioVoice audioVoice in audioVoices)
            {
                audioVoice.source.Stop();
                audioVoice.owner = null;
            }
            audioVoices.Clear();
        }

        public override void UpdateGameThread(float deltaTime)
        {
            foreach (GISB_AudioVoice audioVoice in audioVoices)
            {
                if (audioVoice.source.isPlaying)
                {
                    audioVoice.source.volume = GetVolume();
                }
                else
                {
                    audioVoice.owner = null;
                }
            }
            
            //Clean up voices that are no longer playing
            audioVoices.RemoveAll(voice => voice.owner == null || voice.owner != this);
        }

        public override void UpdateParameters(Dictionary<string, string> activeParameters)
        {
            //Do nothing
        }

        public override double GetDuration()
        {
            return soundDuration;
        }
        
        private float centsToLinear(float cents)
        {
            return Mathf.Pow(2f, cents / 1200f);
        }
        
        public override bool IsPlaying()
        {
            foreach (GISB_AudioVoice audioVoice in audioVoices)
            {
                if(audioVoice.source.isPlaying)
                {
                    return true;
                }
            }
            return false;
        }
    }
}
