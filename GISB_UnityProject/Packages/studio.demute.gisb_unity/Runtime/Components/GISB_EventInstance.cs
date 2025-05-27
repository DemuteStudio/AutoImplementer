using System;
using System.Collections;
using System.Collections.Generic;
using GISB.Runtime;
using UnityEngine;

public class GISB_EventInstance : MonoBehaviour
{
    public GISB_Event eventDefinition;
    private GISB_BaseAudioPlayer rootPlayerInstance;
    private List<AudioSource> audioSources = new List<AudioSource>();
    private AudioSource audioClock;
    public Queue<Action> scheduledActions = new Queue<Action>();

    public void Play(Dictionary<string, string> activeParameters, float fadeInTime = 0.0f)
    {
        if(rootPlayerInstance == null)
        {
            rootPlayerInstance = eventDefinition.GetPlayer();
        }
        
        if(rootPlayerInstance != null)
        {
            rootPlayerInstance.Play(activeParameters, this, fadeInTime, 0.0f);
        }

        if (audioClock == null)
        {
            audioClock = gameObject.AddComponent<AudioSource>();
        }

        if (audioClock != null)
        {
            audioClock.Play();
        }
    }
    
    public void UpdateParameters(Dictionary<string, string> activeParameters)
    {
        if(rootPlayerInstance != null)
        {
            rootPlayerInstance.UpdateParameters(activeParameters);
        }
    }

    public AudioSource GetAudioVoice()
    {
        foreach (AudioSource audioSource in audioSources)
        {
            if (!audioSource.isPlaying)
            {
                return audioSource;
            }
        }
        
        GameObject newAudioSourceObject = new GameObject("Audio Voice");
        newAudioSourceObject.transform.SetParent(transform, false);
        AudioSource newAudioSource = newAudioSourceObject.AddComponent<AudioSource>();
        newAudioSourceObject.AddComponent<AudioLowPassFilter>();
        //newAudioSourceObject.AddComponent<AudioHighPassFilter>();
        audioSources.Add(newAudioSource);
        return newAudioSource;
    }

    public void Stop(float fadeOutTime = 0.0f)
    {
        if (rootPlayerInstance != null)
        {
            rootPlayerInstance.Stop(fadeOutTime);
        }
    }

    public void OnAudioFilterRead(float[] data, int channels)
    {
        if(rootPlayerInstance != null)
        {
            rootPlayerInstance.UpdateAudioThread(AudioSettings.dspTime);
        }
    }


    public void Update()
    {
        while(scheduledActions.Count > 0)
        {
            scheduledActions.Dequeue().Invoke();
        }
        
        if (rootPlayerInstance != null)
        {
            rootPlayerInstance.UpdateGameThread(Time.deltaTime);
        }
    }
}
