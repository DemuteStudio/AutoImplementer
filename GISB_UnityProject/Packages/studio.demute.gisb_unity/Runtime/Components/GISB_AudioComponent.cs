using System.Collections;
using System.Collections.Generic;
using Unity.Collections.LowLevel.Unsafe;
using UnityEngine;
using UnityEngine.Serialization;


namespace GISB.Runtime
{
    public class GISB_AudioComponent : MonoBehaviour
    {
        [FormerlySerializedAs("associatedPlayable")] public GISB_Asset associatedAsset;
        public Dictionary<string, string> activeParameters = new Dictionary<string, string>();
        
        public List<GISB_EventInstance> activeEventInstances = new List<GISB_EventInstance>();

        private List<GISB_AudioVoice> audioVoices = new List<GISB_AudioVoice>();
        private AudioSource audioClock;
        
        public Dictionary<GISB_RandomSound, GISB_RandomSoundPlayer.RandomizationValues> randomizationValuesCache = new Dictionary<GISB_RandomSound, GISB_RandomSoundPlayer.RandomizationValues>();
        
        public void PlayEvent(float fadeInTime = 0.0f)
        {
            PlayEvent(associatedAsset.GetEvent(""), fadeInTime);
        }
        
        public void PlayEvent(GISB_Event eventToPlay, float fadeInTime = 0.0f)
        {
            if(!eventToPlay) return;

            GISB_EventInstance newEventInstance = new GISB_EventInstance(eventToPlay, this);
            activeEventInstances.Add(newEventInstance);
            newEventInstance.Play(fadeInTime);
            
            if (audioClock == null)
            {
                audioClock = gameObject.AddComponent<AudioSource>();
            }

            if (audioClock != null)
            {
                audioClock.Play();
            }
        }
        
        public void PlayEvent(string eventName)
        {
            GISB_Event eventToPlay = associatedAsset.GetEvent(eventName);
            PlayEvent(eventToPlay);
        }

        public void SetParameter(string parameterName, string parameterValue)
        {
            activeParameters[parameterName] = parameterValue;
            foreach (GISB_EventInstance activeEventInstance in activeEventInstances)
            {
                activeEventInstance.UpdateParameters(activeParameters);
            }
        }
        
        public void StopEvent(GISB_Event eventToStop, float fadeOutTime = 0.0f)
        {
            foreach (GISB_EventInstance activeEventInstance in activeEventInstances)
            {
                if (activeEventInstance.eventDefinition == eventToStop)
                {
                    activeEventInstance.Stop(fadeOutTime);
                }
            }
        }
        
        public void StopEvent(string eventName)
        {
            GISB_Event eventToStop = associatedAsset.GetEvent(eventName);
            StopEvent(eventToStop);
        }
        
        public GISB_AudioVoice GetAudioVoice()
        {
            foreach (GISB_AudioVoice audioVoice in audioVoices)
            {
                if (audioVoice.owner == null)
                {
                    return audioVoice;
                }
            }
        
            GameObject newAudioVoiceObject = new GameObject("Audio Voice");
            newAudioVoiceObject.transform.SetParent(transform, false);
            GISB_AudioVoice newAudioVoice = newAudioVoiceObject.AddComponent<GISB_AudioVoice>();
            newAudioVoice.Init();
            audioVoices.Add(newAudioVoice);
            return newAudioVoice;
        }
        
        public void OnAudioFilterRead(float[] data, int channels)
        {
            foreach (GISB_EventInstance activeEventInstance in activeEventInstances)
            {
                activeEventInstance.UpdateAudioThread(AudioSettings.dspTime);
            }
        }


        public void Update()
        {
            foreach (GISB_EventInstance activeEventInstance in activeEventInstances)
            {
                activeEventInstance.UpdateGameThread(Time.deltaTime);
            }
            
            // Remove finished event instances
            for (int i = activeEventInstances.Count - 1; i >= 0; i--)
            {
                if (!activeEventInstances[i].IsPlaying())
                {
                    activeEventInstances.RemoveAt(i);
                }
            }
        }
    }
}

