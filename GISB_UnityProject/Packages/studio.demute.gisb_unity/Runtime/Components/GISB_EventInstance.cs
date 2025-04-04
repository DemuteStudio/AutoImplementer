using System.Collections;
using System.Collections.Generic;
using GISB.Runtime;
using UnityEngine;

public class GISB_EventInstance : MonoBehaviour
{
    public GISB_Event eventDefinition;
    private GISB_BaseAudioPlayer rootPlayerInstance;
    private List<AudioSource> audioSources = new List<AudioSource>();

    public void Play(Dictionary<string, string> activeParameters)
    {
        if(rootPlayerInstance == null)
        {
            rootPlayerInstance = eventDefinition.GetPlayer();
        }
        
        if(rootPlayerInstance != null)
        {
            rootPlayerInstance.Play(activeParameters, this);
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
        audioSources.Add(newAudioSource);
        return newAudioSource;
    }

    public void Stop()
    {
        foreach (AudioSource audioSource in audioSources)
        {
            audioSource.Stop();
        }
    }
}
